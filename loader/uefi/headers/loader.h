#pragma once
#include <efi/efi.h>
#include <efi/efiapi.h>


extern EFI_SIMPLE_TEXT_OUT_PROTOCOL* con_out;
extern EFI_SIMPLE_TEXT_IN_PROTOCOL* con_in;
extern EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* root_fs;

inline EFI_STATUS EFIAPI ParseRootFS(EFI_LOADED_IMAGE_PROTOCOL* image_protocol, EFI_HANDLE_PROTOCOL handle_protocol){
    EFI_GUID simple_fs_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    return handle_protocol(image_protocol->DeviceHandle, &simple_fs_guid, (void**)&root_fs);
}
