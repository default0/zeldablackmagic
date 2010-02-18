// Black Magic.h
// Contains declarations for Black Magic.
//
// Will handle things like file io.

    #include "ZeldaGame.h"

#ifndef BLACK_MAGIC_H

    #define BLACK_MAGIC_H

    // function prototypes
    void    ChangeWES(WindowElement* we, HWND win, zgPtr game);
    void    RegisterBMClasses();
    void    SetupAsmPatching(zgPtr game);
    void    SaveExtendedDataToFile(zgPtr game);

    bool    SaveText(zgPtr game, u32 *romOffset);
    bool    SaveSprites(zgPtr game, u32 *romOffset);

    char*   GetAsmTextResource(int resourceID);

    int     SaveDataToFile(zgPtr game);

#endif