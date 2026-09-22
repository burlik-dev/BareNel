#include <efi/efi.h>
#include <efi/efiapi.h>
char* config = (char*)0x80000000;
CHAR16* boot_name;
CHAR16* file_path;
UINTN config_size = 512;
CHAR16* autoboot;
