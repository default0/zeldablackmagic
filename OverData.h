
	#include "Globals.h"

#ifndef OVERDATA_H

    #define OVERDATA_H

    extern const bool usingMap16;
    extern const bool usingMap32;

    extern const u32 mask16;
    extern const u32 mask32;

    #define forgrid(x) { u32 i =0; u32 j = 0; for(i = 0; i < data->width; ++i) { for(j = 0; j < data->height; ++j) { (x); } } }

// ================================================

    class EventOverlay
    {
    private:
        bufPtr data;

    public:
        EventOverlay();
        ~EventOverlay();

        void Initialize();

        bool SetTile(u16 value, u16 addr);
        bool SetTile(u16 value, u8 x, u8 y);

        u16 GetTile(u8 x, u8 y);
    };

// ================================================

    class OverArea
    {

    public:

        // area number (large areas resolve down to one area number)
        u8 index;
            
        // member variables
        bool largeArea;
        bool fallingRocks[0x03];
                
        // specific graphics settings for the area
        graphicsInfo *gi;

        // 0x2000 byte array for a full map16 tilemap
        bufPtr map16Buf;

        // used for backing up the map16 buffer when event overlay editing is enabled
        bufPtr map16Backup;
        
        // area specific map16 flags
        bufPtr map16Flags  = CreateBuffer(0x10000 / 8);

        // map32 data used only on initial load, will not be saved into the black magic modded rom
		bufPtr          map32Data;
        bufPtr          rom;

        // lists of data entries for each area
        Entrance   *entr;
        Entrance   *holes;
        OverExit   *exits;
        OverSpr    *spr[0x03];
        OverItem   *items[0x03];
        EventOverlay    *eOverlay;

    public:

        // member functions
        OverArea(u8 area, bufPtr rom);
        ~OverArea();

        bufPtr      LoadMap32();

        bool        LoadOverlay();
        bool        UnloadOverlay();
    };

// ================================================

    // forward declaration so OverData is aware of the
    // existence of the ZeldaGame class
    class _ZeldaGame;

    class OverData
    {
    public:

        bool editOverlay;

        u8 phase;               // 0 - beginning, 1 - first part, 2 - second part
        
        u16 prevX;
        u16 prevY;

        u16 numMap32Tiles;
        u16 numMap16Tiles;

        u16 tile8, tile16, tile32;
        
        // the current editing mode
        owEditMode editMode;

        // determines whether we're currently working in map8, map16, or map32 tiles
        objMapType tileSize;
        
        bufPtr map8Buf;         // 0x8000 byte array for a full 1024x1024 area

        bufPtr rom;             // passed in from the ZeldaGame structure

        bufPtr map32Flags;      // bitfield telling us which map32 tiles are being used
        bufPtr map16Flags;      // bitfield telling us which map16 tiles are being used.

        bufPtr map32To16;

        // pointers to map32 to map16 arrays (will probably later use this for map16 to map8... refactoring sucks ...)
        bufPtr upperLeft32;
        bufPtr upperRight32;
        bufPtr lowerLeft32;
        bufPtr lowerRight32;

        bufPtr map16To8;

        bufPtr map16Counts;     // for each map16 tile, tells us how many there are in use
        bufPtr map32Counts;     // for each map32 tile, tells us how many there are in use

        OverObj obj;
        OverObj *pObj;

        // selection related data
        OverObj *selObj;
        OverObj *selObj2;

        OverArea *area;
        OverArea *areas[0xC0];

        // entrance related data
        u8 entrPos;
        u8 entrBuf[0x10];

        // hole related data
        u8 holePos;
        u8 holeBuf[0x10];

        // exit related data
        u8 exitPos;
        u8 exitBuf[0x10];

        // sprite related data
        u8 sprPos;
        u8 sprBuf[0x10];        
 
        // item related data
        u8 itemPos;
        u8 itemBuf[0x10];

        // ----------------------------------------------
        // variables I'm not sure need to be in the class

        BM_Header *header;

        _ZeldaGame *durp;

        // ----------------------------------------------
        // variables that are Windows specific.
        // should be moved elsewhere eventually,
        // divorcing platform from business logic

        ManagedBitmap *bStock[0x10];

        HBITMAP hStock[0x10];
        
        HMENU stockContext;
        HMENU selectContext;
        HMENU sprContext;
        HMENU itemContext;
        HMENU exitContext;
        HMENU entrContext;
        HMENU holeContext;

    public:

        // member functions
            
                OverData(bufPtr rom = NULL, BM_Header *header = NULL);
                ~OverData();
                
        void    DecMapCounts(u16 mapVal, bool map32);
        void    IncMapCounts(u16 mapVal, bool map32, u16 *map2x2 = NULL);

        void    LoadMap16To8();
        void    LoadMap32To16();
        void    LoadOverlaysOld();
        void    PackMap32To16();

        void    LoadMapFlags();
        
        //void    DeleteAreaHoleData();

        bool    LoadArea();
        bool    UnloadArea();

        bool    LoadAllEntranceData();
        bool    LoadAllHoleData();
        bool    LoadAllExitData();
        bool    LoadAllSpriteData();
        bool    LoadAllItemData();

        //bool    AddAreaHole(u32 x, u32 y, u32 entrance = 0);
        bool    DeleteAreaHole(u32 index);

        void    Map16Analysis();

        u16     GetMap32Tile(u32 x, u32 y);
        
        u32     PutMap32Tile(u32 x, u32 y, u32 value);
        u32     FindMap32(u16 map16Vals[4]);
        u32     FindMap16(u16 map8Vals[4]);

        u32     Map16To8(u16 map16Val, u16 map8Vals[4]);
        u32     Map32To16(u16 map32Val, u16 map16Vals[4]);

        u32     AllocateMap16(u16 map8Vals[4], u16 oldMap16, u16 threshold = 0);
        u32     AllocateMap32(u16 map16Vals[4], u16 oldMap32, u16 threshold = 0);

        bufPtr  LoadMap8();
        bufPtr  LoadMap16(u32 index, map16Pos p);
       
        bufPtr  LoadMap32(u32 mapNum);

    };


#endif