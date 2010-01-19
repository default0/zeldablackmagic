
	#include "Globals.h"

#ifndef OVERDATA_H

    #define OVERDATA_H

    extern const bool usingMap16;
    extern const bool usingMap32;

    extern const u32 mask16;
    extern const u32 mask32;

    #define forgrid(x) { u32 i =0; u32 j = 0; for(i = 0; i < data->width; ++i) { for(j = 0; j < data->height; ++j) { (x); } } }

    class OwOverlay
    {
    private:
        bufPtr data;

    public:
        OwOverlay();
        ~OwOverlay();

        void Initialize();

        bool SetTile(u16 value, u16 addr);
        bool SetTile(u16 value, u8 x, u8 y);

        u16 GetTile(u8 x, u8 y);
    };

    class _ZeldaGame;

    class OverData
    {
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

        bool    LoadOverlay();
        bool    UnloadOverlay();

        bool    LoadAllEntranceData();
        bool    LoadAllHoleData();
        bool    LoadAllExitData();
        bool    LoadAllSpriteData();
        bool    LoadAllItemData();

        //bool    AddAreaHole(u32 x, u32 y, u32 entrance = 0);
        bool    DeleteAreaHole(u32 index);

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


    public:

        bool largeArea;
        bool editOverlay;

        u8 area;                // current area        
        
        u16 prevX;
        u16 prevY;

        u16 numMap32Tiles;
        u16 numMap16Tiles;

        u16 tile8, tile16, tile32;
        
        bufPtr map16Buf;        // 0x2000 byte array for a full map16 tilemap
        bufPtr map16Backup;     // used for backing up the map16 buffer when event overlay editing is enabled
        
        bufPtr map8Buf;         // 0x8000 byte array for a full 1024x1024 area

        owEditMode editMode;

        bufPtr rom;             // passed in from the ZeldaGame structure

		bufPtr map32Data[0xC0];

        bufPtr map32Flags;      // bitfield telling us which map32 tiles are being used
        bufPtr map16Flags;      // bitfield telling us which map16 tiles are being used.

        bufPtr map32To16;

        bufPtr upperLeft32;
        bufPtr upperRight32;
        bufPtr lowerLeft32;
        bufPtr lowerRight32;

        bufPtr map16To8;

        bufPtr map16Counts;     // for each map16 tile, tells us how many there are in use
        bufPtr map32Counts;     // for each map32 tile, tells us how many there are in use

        graphicsInfo *gi;

        objMapType tileSize;    // determines whether we're currently working in map8, map16, or map32 tiles

        ManagedBitmap *bStock[0x10];

        OverObj obj;
        OverObj *pObj;

        // selection related data
        OverObj *selObj;
        OverObj *selObj2;

        // entrance related data
        u8 entrPos;
        u8 entrBuf[0x10];

        Entrance *areaEntr;
        Entrance *allEntr[0xC0];
        
        // hole related data
        u8 holePos;
        u8 holeBuf[0x10];

        Entrance *areaHoles;
        Entrance *allHoles[0xC0];

        // exit related data
        u8 exitPos;
        u8 exitBuf[0x10];

        OverExit *areaExits;
        OverExit *allExits[0xC0];

        // sprite related data
        u8 sprPos;
        u8 sprBuf[0x10];        
 
        OverSpr *areaSpr;
        OverSpr *allSpr[0x03][0xC0];
        bool fallingRocks[0x03][0xC0];

        // item related data
        u8 itemPos;
        u8 itemBuf[0x10];

        OverItem *areaItems;
        OverItem *allItems[0x03][0xC0];

        // overlay related data
        OwOverlay *overlays[0x80];

        // ----------------------
        BM_Header *header;

        _ZeldaGame *durp;

        HBITMAP hStock[0x10];
        
        HMENU stockContext;
        HMENU selectContext;
        HMENU sprContext;
        HMENU itemContext;
        HMENU exitContext;
        HMENU entrContext;
        HMENU holeContext;
    };


#endif