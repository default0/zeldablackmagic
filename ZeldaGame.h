
    #include "List.h"
    #include "Marker.h"

    #include "DungTypes.h"
    #include "OverTypes.h"
    #include "OverHole.h"
    #include "OverData.h"
    #include "TextMode.h"

#ifndef ZELDAFILE_H

    #define ZELDAFILE_H

    class ZeldaGame
    //typedef struct _ZeldaGame
    {
    public:
        
        int hasHeader; // Flag indicating whether the file has an extra $200 bytes
        int index; // tells us which file it is among many
        int beenModified; // Nonzero if the file has been modified in some way in a
        
        u32 numRooms; // tells us how many dung. rooms it has. 296 by default.
        u32 numEntrances;
        u32 currentDungRoom;
        u32 currentOverArea;

        bufPtr        headerBuf; // pointer for the header's data so we can logically separate rom and header
        bufPtr        image; // The game file as a char*
        bufPtr        romName; // Pointer to the full path name of the rom.

        HWND          toolDlg; // general purpose tool dialog that can be used for a variety of things
        HWND          pictWin;
        HWND          subWindow; // The window used to edit this rom
        HANDLE        zgFileHandle; // handle to this particular game's file.
        HANDLE        portFileHandle;

        WindowElement *dungeonWE;
        WindowElement *overWE;

        WEStruct      currentWES;

        BM_Header     bm_Header;
        BM_DungeonHeader bm_dung_header;  

        DungeonFile  dungeonFile;
        DungeonFile  portFile;
        DungeonFile* currentFile;

        DialogueFile dialogueFile;

        OverData    *overData;

        giPtr        gi;
        
        // All the background graphics packs
        bufPtr       bgPacks[0x100];

        // All the sprite graphics packs
        bufPtr       sprPacks[0x100];

        bufPtr       linkGfx;
        bufPtr       fontGfx;
        bufPtr       mode7Gfx;

        bufPtr       vram;
        bufPtr       cgram;
        bufPtr       aniVram; // for animated tiles

        editMode     currentFunc;

        bufPtr       testPtr;
        bufPtr       BG_Map[2];

        // $200 decoded BG tiles
        SnesTile     BGTiles[0x400];
        SnesTile     OAMTiles[0x400];

        u32 xPos;
        u32 yPos;
        u32 xPrev;
        u32 yPrev;
        u32 cursorIndex;
        u32 aniTimer;
        u32 aniFrame;
        u32 active;

        // sixteen decoded palettes to work with.
        SnesPal      Pals[0x10];

        SnesTile cursor;
        SnesPal cursorPal;

        DngObjStruct *dngObjs;

    public:

        void    LoadAllGfx();

        int     GetSprGfxPtr(int index);
        int     GetBgGfxPtr(int index);

        bool    SaveGraphics(u32 *offset);

    };
    //ZeldaGame, *zgPtr;

    typedef ZeldaGame* zgPtr;

    // externally defined variables
    extern zgPtr    newGamePtr;

#endif