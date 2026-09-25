#pragma once
#include "loader.h"
#include "console.h"
#include "config_worker.h"

#define INTERFACE_COLS 100
#define INTERFACE_ROWS 2
#define CHANGE_STRING

inline void DrawGUI(){
    CHAR16 box[] = {BOXDRAW_HORIZONTAL, 0};
    con_out->ClearScreen(con_out);
    EFI_STATUS status = con_out->SetCursorPosition(con_out, (console_columns / 2) - (INTERFACE_COLS / 2), (console_rows / 2));

    for(int i = 0; i < 100; i++){
        ConsolePrint(box);
    }
    con_out->SetCursorPosition(con_out, (console_columns / 2) - (boot_name_size / 2), (console_rows / 2) - 1);
    ConsolePrint(boot_name);
}
