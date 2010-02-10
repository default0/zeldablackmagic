// Declares the core data and manipulation functions


#include "Strings.h"
#include "Globals.h"


#ifndef CORE_H
    #define CORE_H

    // data types

    // empty...

    // function prototypes

    bufPtr CompressStandard(bufPtr srcBuf, int flag = 0);

    bufPtr DecompressStandard(bufPtr inputBuf, int offset);

    int CpuToRomAddr(int holderLong);

    int RomToCpuAddr(int holderLong);

    bufPtr DecompressOther(bufPtr inputBuf, int source);

    int XBA(int value);

    SnesTile* SnesToTile(bufPtr tile, SnesTile* product, unsigned int offset);

    SnesTile* FlipTile(SnesTile* input, SnesTile* product, int x, int y);

    int TileToBitmap(SnesTile* tile, SnesImage* product, SnesPalPtr);

    void ImportDungeons(zgPtr game, HINSTANCE prog);

    void ExportDungeons(zgPtr game, HINSTANCE prog);

    void ImportOverworld(zgPtr game, HINSTANCE prog);

    void ExportOverworld(zgPtr game, HINSTANCE prog);

    int CALLBACK PortDungProc(HWND dlg, UINT msg, WPARAM wp, LPARAM lp);

    GameBoyImage* GameBoyToBitmap(bufPtr tile, GameBoyPal palette, GameBoyImage* product);

    void ImportDungeonFile(zgPtr game);

    void UnpackDungeonHeader(BM_DungeonHeader *header);

    void RepackDungeonHeader(zgPtr game, BM_DungeonHeader *header, bufPtr data);

    void LoadDungeonFile(zgPtr game, DungeonFile *file);

    void ExportDungeonFile(zgPtr game);

    void Create_BM_Header(zgPtr game);

    void Check_BM_Header(zgPtr game);

    void Do3To4High(bufPtr, bufPtr, u32*, u32*, u32);
    void Do3To4Low(bufPtr, bufPtr, u32*, u32*, u32);

    void LoadPalettes(zgPtr, u32, u32, u32, u32);
    void LoadPalettes(zgPtr game);

    void LoadTileMap(zgPtr game);

    void Draw4x4s(bufPtr, bufPtr, u32, u32*, u32);

    void DecodePalettes(zgPtr game);

    void DecodeTiles(zgPtr game, u32 firstTile = 0, u32 lastTile = 0x3FF);

    void DecodeOAMTiles(zgPtr game); 

    void DrawTiles(zgPtr game);

    void PopulateRegistries(zgPtr game);

    void EraseTile(ManagedBitmap *b, u32 location, u32 bgColor);

    void BlitTile(ManagedBitmap*, u32 location, SnesTile*, SnesPal*, bool, bool);

    void BlitTranslucent(ManagedBitmap*, u32, u32, SnesTile*, SnesPal*, bool, bool);

    void RedrawTile(ManagedBitmap*, u32, u32, zgPtr);

    void DrawLayout(zgPtr game, u32 layoutNum);

    void LoadObjects(zgPtr game, u32 typeSelect = 0x3);
    void LoadDngObjTiles(zgPtr, DngObj*);

    void ParseType1(DngObjList*, bufPtr, u32, zgPtr, u32);
    void ParseType2(DngObjList*, bufPtr, u32, zgPtr, u32);
    void DrawObjects(zgPtr game);
    void DngDecompress(zgPtr game);
    void AniDecompress(zgPtr game);

    void LoadAttributes(zgPtr game);

    ManagedBitmap* CreateManagedBitmap(u32 width, u32 height);

    // Text.cpp function prototypes

    void DumpScript(zgPtr game, HINSTANCE prog);
    bufPtr GetOldTextTable(zgPtr game);
    bufPtr GetNewTextTable(zgPtr game);
    void InsertScript(zgPtr game, HINSTANCE prog);
    void ParseScript(zgPtr game, HINSTANCE prog);
    void LoadDialogueFile(zgPtr game);

    int AdvancePointer(zgPtr game, u32 *offset, u32 size);

#endif