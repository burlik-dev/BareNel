#include "headers/loader.h"
#include "headers/console.h"
#include "headers/config_worker.h"
#include "headers/gui.h"
#include "headers/debug.h"

extern void InitConsole(EFI_HANDLE console_handle, EFI_HANDLE_PROTOCOL handle_protocol);
extern EFI_STATUS EFIAPI ConsolePrint(CHAR16* string);
extern EFI_STATUS EFIAPI ConsoleNewRow();
extern EFI_STATUS EFIAPI ParseRootFS(EFI_LOADED_IMAGE_PROTOCOL* image_protocol, EFI_HANDLE_PROTOCOL handle_protocol);
extern void Sleep(int seconds, EFI_RUNTIME_SERVICES* runtime_services);
extern EFI_STATUS EFIAPI ParseConfig(EFI_ALLOCATE_POOL allocate_protocol);
extern void UTF8_TO_UTF16(char* source, CHAR16* dest, int size);
extern void AnalyzeConfig(EFI_ALLOCATE_POOL allocate_protocol, EFI_COPY_MEM copy_mem_protocol);
extern void DrawGUI();

extern CHAR16* UNSIGNED_INTEGER_TO_STR(EFI_BOOT_SERVICES* boot_services, UINTN number, int *size);
extern void PrintMemoryMap(EFI_BOOT_SERVICES* boot_services, unsigned char mode);

EFI_STATUS EFIAPI EFI_MAIN (IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE* SystemTable){
    con_out = SystemTable->ConOut;
    InitConsole(SystemTable->ConsoleOutHandle, SystemTable->BootServices->HandleProtocol);
    ConsolePrint(u"BareNel Loader by Burlik\0");
    ConsoleNewRow();
    ConsolePrint(u"BareNel Loader is ready!\0");
    ConsoleNewRow();

    EFI_LOADED_IMAGE_PROTOCOL* image_protocol;
    EFI_GUID loaded_image_protocol_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    SystemTable->BootServices->HandleProtocol(ImageHandle, &loaded_image_protocol_guid, (void**)&image_protocol);


    EFI_STATUS status = ParseRootFS(image_protocol, SystemTable->BootServices->HandleProtocol);
    if(status == EFI_SUCCESS){
        ConsolePrint(u"Succesfully opened root filesystem!\0");
        ConsoleNewRow();
    }
    else{
        ConsolePrint(u"Unable to open root filesystem!\0");
        return 1;
    }

    status = ParseConfig(SystemTable->BootServices->AllocatePool);
    if(status == EFI_SUCCESS){
        ConsolePrint(u"Succesfully loaded config file!\0");
        ConsoleNewRow();
    }
    AnalyzeConfig(SystemTable->BootServices->AllocatePool, SystemTable->BootServices->CopyMem);

    ConsolePrint(u"Loader configuration : ");
    ConsoleNewRow();
    ConsolePrint(u"Boot record name : ");
    ConsolePrint(boot_name);
    ConsoleNewRow();
    ConsolePrint(u"Kernel path : ");
    ConsolePrint(file_path);
    ConsoleNewRow();
    ConsolePrint(u"Autoboot : ");
    ConsolePrint(autoboot);
    ConsoleNewRow();

    Sleep(5, SystemTable->RuntimeServices);


    con_out->ClearScreen(con_out);

    //ConsolePrint(UNSIGNED_INTEGER_TO_STR(SystemTable->BootServices, 253667));
    ConsoleNewRow();
    PrintMemoryMap(SystemTable->BootServices, 1);

    //DrawGUI();

    while(1){
        //DrawGUI();
    }



    return 0;
}
