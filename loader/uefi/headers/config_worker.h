#pragma once
#include "loader.h"
#include "console.h"



#define MAX_CONFIG_SIZE 512
#define TABLE_PARAMETRS_COUNT 26
#define POINTER_SIZE 8
#define ASCII_ALPHABET_OFFSET 65

extern char* config;
extern CHAR16* boot_name;
extern CHAR16* file_path;
extern UINTN config_size;
extern CHAR16* autoboot;

extern int boot_name_size;
extern int file_path_size;
extern int autoboot_size;

extern EFI_STATUS EFIAPI ConsolePrint(CHAR16* string);


inline EFI_STATUS EFIAPI ParseConfig(EFI_ALLOCATE_POOL allocate_protocol){
    EFI_STATUS status = allocate_protocol(EfiLoaderData, MAX_CONFIG_SIZE, (void**)&config);
    if(status == EFI_SUCCESS){
        ConsolePrint(u"Memory was allocated succesfully! ");
    }


    EFI_FILE_PROTOCOL* root_dir;
    EFI_FILE_PROTOCOL* config_file;
    root_fs->OpenVolume(root_fs, &root_dir);
    root = root_dir;
    root_dir->Open(root_dir, &config_file, u"config.conf\0", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    //config_file->Open(config_file, &config_file, u"config.conf\0", EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    return config_file->Read(config_file, &config_size, (void*)config);
}

inline void UTF8_TO_UTF16(char* source, CHAR16* dest, int size){
    for(int i = 0; i < size && source[i] != '\0'; i++){
        *dest = source[i];
        dest++;
    }
    *dest = 0;
}



inline void AnalyzeConfig(EFI_ALLOCATE_POOL allocate_protocol, EFI_COPY_MEM copy_mem_protocol){
    CHAR16* config_utf16;
    allocate_protocol(EfiLoaderData, MAX_CONFIG_SIZE * 2, (void**)&config_utf16);
    UTF8_TO_UTF16(config, config_utf16, config_size);

    CHAR16*** option_table;
    int** size_table;

    allocate_protocol(EfiLoaderData, TABLE_PARAMETRS_COUNT * POINTER_SIZE, (void**)&option_table);
    allocate_protocol(EfiLoaderData, TABLE_PARAMETRS_COUNT * POINTER_SIZE, (void**)&size_table);


    option_table['N' - ASCII_ALPHABET_OFFSET] = &boot_name;
    option_table['F' - ASCII_ALPHABET_OFFSET] = &file_path;
    option_table['A' - ASCII_ALPHABET_OFFSET] = &autoboot;


    size_table['N' - ASCII_ALPHABET_OFFSET] = &boot_name_size;
    size_table['F' - ASCII_ALPHABET_OFFSET] = &file_path_size;
    size_table['A' - ASCII_ALPHABET_OFFSET] = &autoboot_size;


    CHAR16 service_char = 0;
    CHAR16** current_parametr;
    int current_parametr_length = 0;


    for(int i = 0; i < config_size; i++){
        service_char = config_utf16[i];
        current_parametr = option_table[service_char - ASCII_ALPHABET_OFFSET];
        for(;config_utf16[i] != '=';i++);
        i++;
        for(;config_utf16[i] != '\n' && config_utf16[i] != '\0'; i++){current_parametr_length++;}
        allocate_protocol(EfiLoaderData, (current_parametr_length + 1) * 2, (void**)current_parametr);
        copy_mem_protocol(*current_parametr, config_utf16 + (i - current_parametr_length), current_parametr_length * 2);
        *size_table[service_char - ASCII_ALPHABET_OFFSET] = current_parametr_length;
        ((CHAR16*)*current_parametr)[current_parametr_length] = 0;
        current_parametr_length = 0;
    }
}
