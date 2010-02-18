
    #include "ZeldaGame.h"

#ifndef OVERMODE_H

    #define OVERMODE_H

    // externally defined variables
    extern OverObj over_stock_objs[];

    // function declarations
    bool        PlaceObject(zgPtr game, OverData *o, OverObj *obj);
            
    void        InitOverMode(zgPtr game);
    void        DrawOW(zgPtr game, ManagedBitmap *b, bufPtr map8Buf, bool justAnimated = false);
    void        LoadMapFlags(zgPtr game);

    void        DrawSprite(zgPtr game, ManagedBitmap *b, u16 spriteVal, u32 x = 0, u32 y = 0, u8 palette = 0, u8 large = 0, bool hFlip = false, bool vFlip = false);

    void        DrawMap16(zgPtr game, ManagedBitmap *b, u16 map16Val, u32 x, u32 y);
    void        DrawMap8(zgPtr game, ManagedBitmap *b, u32 entry, u32 x, u32 y);

    void        LoadOldMap32To16(zgPtr game);
    void        LoadMap16To8(zgPtr game);
    void        PlaceStockObject(HWND hwnd, ManagedBitmap* b, LPARAM lp, zgPtr game);

    void        OverDecompress(zgPtr game);
    void        AniDecompressOverworld(zgPtr game);
    void        LoadOverPalettes(zgPtr game);

    bufPtr      DecompressBank02(bufPtr inputBuf, int source);

    u16         GetMap16X(u16 addr);
    u16         GetMap16Y(u16 addr);

    u16         GetMap8Tile(bufPtr map8Buf, u16 x, u16 y);

    u32         AllocateMap16(bufPtr counts, u16 map8Vals[], u16 oldMap16,  u16 threshold = 0);
 
    u32         SetMap8Tile(bufPtr map8Buf, u16 value, u16 x, u16 y);

    u32         SaveOverworldData(zgPtr game, u32 *offset);

#endif