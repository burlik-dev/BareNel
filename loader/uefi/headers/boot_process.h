#include "loader.h"
#include "console.h"
#include "config_worker.h"

#define MCFG_SIGNATURE 0x4746434D
#define APIC_SIGNATURE 0x43495041


inline void* FindACPITable(void* xsdt, int signature){
    xsdt = (char*)xsdt + 36;
    int current_signature;
    void* table;

    for(int i = 0;1;i++){
        current_signature = *((int*)(*((UINTN*)xsdt + i)));
        if(current_signature == signature){
            table = (void*)(*((UINTN*)xsdt + i));
            break;
        }
    }
    return table;
}


inline void LoadKernel(EFI_BOOT_SERVICES* boot_services, void* rsdp_pointer){
    con_out->ClearScreen(con_out);

    //First we need to get the actual Memory Map, initialized by the firmware
    UINTN memory_map_size = 0;
    EFI_MEMORY_DESCRIPTOR* memory_map;
    UINTN map_key;
    UINTN descriptor_size;
    UINT32 descriptor_ver;

    boot_services->GetMemoryMap(&memory_map_size, memory_map, &map_key, &descriptor_size, &descriptor_ver);
    boot_services->AllocatePool(EfiLoaderData, memory_map_size, (void**)&memory_map);
    boot_services->GetMemoryMap(&memory_map_size, memory_map, &map_key, &descriptor_size, &descriptor_ver);

    UINTN descriptors_amount = memory_map_size / descriptor_size;

    //Loading the kernel archive
    EFI_FILE_PROTOCOL* kernel_file;
    void* kernel_buffer;
    UINTN kernel_buffer_size = 0;

    root->Open(root, &kernel_file, file_path, EFI_FILE_MODE_READ, 0);
    kernel_file->Read(kernel_file, &kernel_buffer_size, &kernel_buffer);
    boot_services->AllocatePool(EfiLoaderData, kernel_buffer_size, &kernel_buffer);
    kernel_file->Read(kernel_file, &kernel_buffer_size, &kernel_buffer);
    kernel_file->Close(kernel_file);


    //Configuring virtual memory map
    void* acpi_tables = rsdp_pointer;
    void* xsdt = (void*)(*((UINTN*)((char*)acpi_tables + 24)));

    void* mcfg = FindACPITable(xsdt, MCFG_SIGNATURE);

    CHAR16* message;
    boot_services->AllocatePool(EfiLoaderData, 10, (void**)&message);

    UTF8_TO_UTF16((char*)mcfg, message, 4);
    ConsolePrint(message);
    ConsoleNewRow();

}
