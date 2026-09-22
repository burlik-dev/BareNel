#pragma once
#include "loader.h"


inline EFI_STATUS EFIAPI ConsolePrint(CHAR16* string){
    return con_out->OutputString(con_out, string);
}

inline EFI_STATUS EFIAPI ConsoleNewRow(){
    return con_out->SetCursorPosition(con_out, 0, con_out->Mode->CursorRow + 1);
}

