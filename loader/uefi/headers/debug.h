#include "loader.h"
#include "console.h"

#define MAX_INTEGER_SIZE 40


inline CHAR16* UNSIGNED_INTEGER_TO_STR(EFI_BOOT_SERVICES* boot_services, UINTN number, int *size){
    CHAR16* result;
    int size_result = 0;
    boot_services->AllocatePool(EfiLoaderData, (MAX_INTEGER_SIZE + 1) * 2, (void**)&result);
    result += (MAX_INTEGER_SIZE - 1) * 2;
    *(result + 1) = '\0';
    do{
        unsigned char pivot = number % 10;
        *result = 48 + pivot;
        result-= 1;
        size_result++;
        number /= 10;
    }
    while(number > 0);
    if(size != NULL){*size = size_result;}
    return result + 1;
}


inline void PrintMemoryMap(EFI_BOOT_SERVICES* boot_services, unsigned char mode){
    EFI_FILE_PROTOCOL* dump_file;
    void* dump_file_buffer;
    void* dump_file_buffer_start_pos;

    if(mode == 1){
        EFI_STATUS status = root->Open(root, &dump_file, u"memory_map.dump\0", EFI_FILE_MODE_WRITE | EFI_FILE_MODE_READ, EFI_FILE_VALID_ATTR);

        if(status != EFI_SUCCESS){
            ConsolePrint(u"Unable to open dump file!");
            ConsoleNewRow();
            ConsolePrint(UNSIGNED_INTEGER_TO_STR(boot_services, status, NULL));
            return;
        }

        boot_services->AllocatePool(EfiLoaderData, 100000, &dump_file_buffer);
        dump_file_buffer_start_pos = dump_file_buffer;
    }

    UINTN buffer_size = 0;
    EFI_MEMORY_DESCRIPTOR* buffer;
    UINTN buffer_key;
    UINTN descriptor_size;
    UINT32 descriptor_ver;

    boot_services->GetMemoryMap(&buffer_size, buffer, &buffer_key, &descriptor_size, &descriptor_ver);
    boot_services->AllocatePool(EfiLoaderData, buffer_size, (void**)&buffer);
    EFI_STATUS status = boot_services->GetMemoryMap(&buffer_size, buffer, &buffer_key, &descriptor_size, &descriptor_ver);

    ConsolePrint(UNSIGNED_INTEGER_TO_STR(boot_services, descriptor_size, NULL));
    ConsoleNewRow();

    if(status != EFI_SUCCESS){
        ConsolePrint(u"Something went wrong!");
        return;
    }

    CHAR16* messages[] = {u"Memory type - \0", u"Physical Start - \0", u"Virtual Start - \0", u"NumberOfPages - \0", u"Attribute - \0"};

    CHAR16* pivot_str;
    int pivot_size;
    for(int i = 0; i < buffer_size / descriptor_size; i++){
        EFI_MEMORY_DESCRIPTOR* actual_descriptor = (EFI_MEMORY_DESCRIPTOR*)(((char*)buffer) + (descriptor_size * i));

        ConsolePrint(messages[0]);
        pivot_str = UNSIGNED_INTEGER_TO_STR(boot_services, actual_descriptor->Type, &pivot_size);
        ConsolePrint(pivot_str);
        ConsoleNewRow();

        if(mode == 1){
            boot_services->CopyMem(dump_file_buffer, (void*)messages[0], 14 * 2);
            dump_file_buffer = (char*)dump_file_buffer + 14 * 2;
            boot_services->CopyMem(dump_file_buffer, (void*)pivot_str,  pivot_size * 2);
            dump_file_buffer = (char*)dump_file_buffer + pivot_size * 2;
            *(CHAR16*)dump_file_buffer = '\n';
            dump_file_buffer = (char*)dump_file_buffer + 2;
        }


        ConsolePrint(messages[1]);
        pivot_str = UNSIGNED_INTEGER_TO_STR(boot_services, actual_descriptor->PhysicalStart, &pivot_size);
        ConsolePrint(pivot_str);
        ConsoleNewRow();


        if(mode == 1){
            boot_services->CopyMem(dump_file_buffer, (void*)messages[1],  17 * 2);
            dump_file_buffer = (char*)dump_file_buffer + 17 * 2;
            boot_services->CopyMem(dump_file_buffer, (void*)pivot_str, pivot_size * 2);
            dump_file_buffer = (char*)dump_file_buffer + pivot_size * 2;
            *(CHAR16*)dump_file_buffer = '\n';
            dump_file_buffer = (char*)dump_file_buffer + 2;
        }

        ConsolePrint(messages[2]);
        pivot_str = UNSIGNED_INTEGER_TO_STR(boot_services, actual_descriptor->VirtualStart, &pivot_size);
        ConsolePrint(pivot_str);
        ConsoleNewRow();


        if(mode == 1){
            boot_services->CopyMem(dump_file_buffer, (void*)messages[2], 16 * 2);
            dump_file_buffer = (char*)dump_file_buffer + 16 * 2;
            boot_services->CopyMem(dump_file_buffer, (void*)pivot_str,  pivot_size * 2);
            dump_file_buffer = (char*)dump_file_buffer + pivot_size * 2;
            *(CHAR16*)dump_file_buffer = '\n';
            dump_file_buffer = (char*)dump_file_buffer + 2;
        }


        ConsolePrint(messages[3]);
        pivot_str = UNSIGNED_INTEGER_TO_STR(boot_services, actual_descriptor->NumberOfPages, &pivot_size);
        ConsolePrint(pivot_str);
        ConsoleNewRow();


        if(mode == 1){
            boot_services->CopyMem(dump_file_buffer, (void*)messages[3],  16 * 2);
            dump_file_buffer = (char*)dump_file_buffer + 16 * 2;
            boot_services->CopyMem(dump_file_buffer, (void*)pivot_str,  pivot_size * 2);
            dump_file_buffer = (char*)dump_file_buffer + pivot_size * 2;
            *(CHAR16*)dump_file_buffer = '\n';
            dump_file_buffer = (char*)dump_file_buffer + 2;
        }

        ConsolePrint(messages[4]);
        pivot_str = UNSIGNED_INTEGER_TO_STR(boot_services, actual_descriptor->Attribute, &pivot_size);
        ConsolePrint(pivot_str);
        ConsoleNewRow();


        if(mode == 1){
            boot_services->CopyMem(dump_file_buffer, (void*)messages[4], 12 * 2);
            dump_file_buffer = (char*)dump_file_buffer + 12 * 2;
            boot_services->CopyMem(dump_file_buffer, (void*)pivot_str,  pivot_size * 2);
            dump_file_buffer = (char*)dump_file_buffer + pivot_size * 2;
            *(CHAR16*)dump_file_buffer = '\n';
            *((CHAR16*)(dump_file_buffer) + 1)  = '\n';
            dump_file_buffer = (char*)dump_file_buffer + 4;
        }

        ConsoleNewRow();
    }
    if(mode == 1){
        UINTN dump_size = 100000;
        EFI_STATUS status = dump_file->Write(dump_file, &dump_size, dump_file_buffer_start_pos);
        if(status != EFI_SUCCESS){
            ConsolePrint(u"Unable to open dump file!");
            ConsoleNewRow();
            ConsolePrint(UNSIGNED_INTEGER_TO_STR(boot_services, status, NULL));
            return;
        }
        dump_file->Flush(dump_file);
        dump_file->Close(dump_file);
    }
}
