#include "headers/loader.h"
#include "headers/console.h"
#include "headers/config_worker.h"

extern EFI_STATUS EFIAPI ConsolePrint(CHAR16* string);
extern EFI_STATUS EFIAPI ConsoleNewRow();
extern EFI_STATUS EFIAPI ParseRootFS(EFI_LOADED_IMAGE_PROTOCOL* image_protocol, EFI_HANDLE_PROTOCOL handle_protocol);
extern EFI_STATUS EFIAPI ParseConfig(EFI_ALLOCATE_POOL allocate_protocol);
extern void UTF8_TO_UTF16(char* source, CHAR16* dest, int size);
extern void AnalyzeConfig(EFI_ALLOCATE_POOL allocate_protocol, EFI_COPY_MEM copy_mem_protocol);

EFI_STATUS EFIAPI EFI_MAIN (IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE* SystemTable){


    con_out = SystemTable->ConOut;
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

    ConsolePrint(boot_name);
    ConsolePrint(file_path);
    ConsolePrint(autoboot);

    while(1){}

    return 0;
}
