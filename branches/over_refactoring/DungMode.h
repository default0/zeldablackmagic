#include "Strings.h"
#include "Globals.h"

#ifndef DUNGMODE_H
#define DUNGMODE_H

    bufPtr  RepackSprites(DngObjStruct *d);
    void    SaveRoom(zgPtr game);
    int     CALLBACK DungHeaderProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void    DrawDungeonSprites(zgPtr game, HDC memDC, HDC picDC);


#endif