
    #include "Strings.h"
    #include "OverHole.h"
    #include "OverTypes.h"

#ifndef OVERDATA_H

    #define OVERDATA_H

    extern const bool usingMap16;
    extern const bool usingMap32;

    extern const u32 mask16;
    extern const u32 mask32;

    #define forgrid(x) { u32 i =0; u32 j = 0; for(i = 0; i < data->width; ++i) { for(j = 0; j < data->height; ++j) { (x); } } }

    // forward declaration so OverData is aware of the
    // existence of the ZeldaGame class
    class _ZeldaGame;

// ================================================

    class Map16Buf
    {
    public:

        u8 width;
        u8 height;

        bufPtr data;

    // --------------------------

    public:

        Map16Buf(u8 width, u8 height)
        {
            this->width  = width;
            this->height = height;

            data = CreateBuffer(width, height, 2);
        }

    // --------------------------

        ~Map16Buf()
        {
            DeallocBuffer(data);
        }

    // --------------------------

        Map16Buf* operator = (Map16Buf *m)
        {
            if(this != m)
            {
                this->width = m->width;
                this->height = m->height;

                CopyBuffer(data, m->data, 0, 0, m->data->length);
            }

            return this;
        }

    // --------------------------

        u16 GetTile(u16 x, u16 y)
        {
            return Get2Bytes(data, (x * 2) + (y * 0x80));
        }

    // --------------------------

        static u16 GetX(u16 addr)
        {
            return (addr & 0x007E) << 3;
        }

        static u16 GetY(u16 addr)
        {
            return (addr & 0x1F80) >> 3;
        }

    // --------------------------

        bool SetTile(u16 addr, u16 value)
        {
            u8 x = GetX(addr) >> 4;
            u8 y = GetY(addr) >> 4;

            if(addr > 0x2000)
                return false;

            return SetTile(x, y, value);
        }

    // --------------------------

        bool SetTile(u16 x, u16 y, u16 value)
        {
            return (Put2Bytes(data, (x * 2) + (y * 0x80), value) == 1) ? true : false;
        }
    };

// ================================================

    class OverArea
    {

        // member variables

    public:

        // area number (large areas resolve down to one area number)
        u8 areaNum;

        bool darkWorld;
        bool fallingRocks[0x03];
        bool largeArea;

        // specific graphics settings for the area
        graphicsInfo *gi;

        // 0x2000 byte array for a full map16 tilemap
        Map16Buf *map16;

        // used for backing up the map16 buffer when event overlay editing is enabled
        Map16Buf *map16Backup;
        
        // area specific map16 flags
        bufPtr map16Flags;

        // map32 data used only on initial load, will not be saved into the black magic modded rom
		bufPtr          map32Data;
        bufPtr          rom;

        // lists of data entries for each area
        Entrance        *entr;
        Entrance        *holes;
        OverExit        *exits;
        OverSpr         *spr[0x03];
        OverItem        *items[0x03];
        Map16Buf        *eOverlay;

    public:

        // member functions
        OverArea(u8 area, bufPtr rom);
        ~OverArea();

        bool    LoadOverlay();
        bool    UnloadOverlay();

        bufPtr      LoadMap16();
    };

// ================================================

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
        
        // 0x8000 byte array for a full 1024x1024 area
        bufPtr map8Buf;

        // buffer used for currently loaded area
        bufPtr map16Buf;

        // bitfield telling us which map16 tiles are being used globally
        bufPtr map16Flags;

        // for each map16 tile, tells us how many there are in use
        bufPtr map16Counts;

        // buffer containing data to convert map16 to map8 tiles.
        bufPtr map16To8;

        // buffer of map32 buffers used during first time load
        bufPtr map32Data[0xA0];

        // buffer that tells us how to convert map32 tiles to map16 tiles (for first time load only)
        bufPtr map32To16;

        // passed in from the ZeldaGame structure
        bufPtr rom;

        OverObj obj;
        OverObj *pObj;

        // selection related data
        OverObj *selObj;
        OverObj *selObj2;

        OverArea *area;
        OverArea *areas[0xC0];

        // used for modifying markers
        u8 pos;
        u8 buf[0x10];

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

        /// test function
        void    Map16Analysis();
                
        void    DecMapCounts(u16 mapVal);
        void    IncMapCounts(u16 mapVal, u16 *map2x2 = NULL);

        void    LoadMap16To8();
        void    LoadMap32To16();

        void    LoadOverlaysOld();

        void    LoadMapFlags();
        
        bool    LoadArea();
        bool    UnloadArea();

        bool    LoadAllEntranceData();
        bool    LoadAllHoleData();
        bool    LoadAllExitData();
        bool    LoadAllSpriteData();
        bool    LoadAllItemData();
    
        u8      ResolveArea(u8 area);


        u32     FindMap16(u16 map8Vals[4]);
        u32     Map16To8(u16 map16Val, u16 map8Vals[4]);
        u32     Map32To16(u16 map32Val, u16 map16Vals[4]);
        u32     AllocateMap16(u16 map8Vals[4], u16 oldMap16, u16 threshold = 0);

        bufPtr  LoadMap8();
        bufPtr  LoadMap16(u32 index, map16Pos p);
        bufPtr  LoadMap32(u32 mapNum);

    };


#endif