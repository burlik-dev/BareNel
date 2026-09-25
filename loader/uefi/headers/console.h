#pragma once
#include "loader.h"

extern UINTN console_rows;
extern UINTN console_columns;


inline void InitConsole(EFI_HANDLE console_handle, EFI_HANDLE_PROTOCOL handle_protocol){
    EFI_GRAPHICS_OUTPUT_PROTOCOL* graphics_protocol;
    EFI_GUID graphics_protocol_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    handle_protocol(console_handle,&graphics_protocol_guid,(void**)&graphics_protocol);

    graphics_protocol->SetMode(graphics_protocol, graphics_protocol->Mode->Mode);
    con_out->QueryMode(con_out, con_out->Mode->Mode, &console_columns, &console_rows);
}



inline EFI_STATUS EFIAPI ConsolePrint(CHAR16* string){
    return con_out->OutputString(con_out, string);
}

inline EFI_STATUS EFIAPI ConsoleNewRow(){
    return con_out->SetCursorPosition(con_out, 0, con_out->Mode->CursorRow + 1);
}

