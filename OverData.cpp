
	#include "Globals.h"
	#include "Core.h"
	#include "Sprites.h"
    #include "OverData.h"
	#include "OverMode.h"
	#include "OverModeGui.h"
    #include "compresch_lttp.h"

    const bool usingMap16 = false;
    const bool usingMap32 = true;

    const u32 mask16      = ~0x01;
    const u32 mask32      = ~0x03;

    const u32 maxHoles    = 0x1998;

// ===============================================================

    // OverData Constructor function

    OverData::OverData(bufPtr rom, BM_Header *header)
    {
        u32 i       = 0;
        u32 j       = true;

        //---------------------------
        
        this->largeArea   = false;
        this->editOverlay = false;

        this->rom         = rom;

        this->pObj        = NULL;
        // obsolete this->holeObj     = NULL;

        this->editMode      = mode_drawtile;

        this->map16Buf      = CreateBuffer(0x40, 0x40, 2);
        this->map8Buf       = CreateBuffer(0x80, 0x80, 2);

        this->map16Flags    = CreateBuffer(0x10000 / 8);
        this->map32Flags    = CreateBuffer(0x10000 / 8);

        this->map16Counts   = CreateBuffer(0x10000 * 4);
        this->map32Counts   = CreateBuffer(0x10000 * 4);

        this->map32To16     = CreateBuffer(0x40000);

        this->upperLeft32   = CreateBuffer(0x8000);
        this->upperRight32  = CreateBuffer(0x8000);
        this->lowerLeft32   = CreateBuffer(0x8000);
        this->lowerRight32  = CreateBuffer(0x8000);
        
        this->map16To8      = CreateBuffer(0x40000);

        this->area          = 0x45;
        this->gi            = new graphicsInfo;

        this->header        = header;

        this->stockContext  = GetSubMenu(LoadMenu(thisProg, MAKEINTRESOURCE(IDR_OVER_STOCK)), 0);
        this->selectContext = GetSubMenu(LoadMenu(thisProg, MAKEINTRESOURCE(IDR_OVER_SELECT)), 0);
        this->sprContext    = GetSubMenu(LoadMenu(thisProg, MAKEINTRESOURCE(IDR_OVER_SPRITE)), 0);
        this->itemContext   = GetSubMenu(LoadMenu(thisProg, MAKEINTRESOURCE(IDR_OVER_ITEM)), 0);
        this->exitContext   = GetSubMenu(LoadMenu(thisProg, MAKEINTRESOURCE(IDR_OVER_EXIT)), 0);
        this->entrContext   = GetSubMenu(LoadMenu(thisProg, MAKEINTRESOURCE(IDR_OVER_ENTR)), 0);
        this->holeContext   = GetSubMenu(LoadMenu(thisProg, MAKEINTRESOURCE(IDR_OVER_HOLE)), 0);

        this->selObj        = new OverObj();

        this->selObj->forming   = false;
        this->selObj->active    = false;

        this->selObj2       = new OverObj();

        this->areaHoles     = NULL;
        this->areaEntr      = NULL;
        this->areaExits     = NULL;

        this->tile8  = 0;
        this->tile16 = 0;
        this->tile32 = 0;

        this->tileSize = fromMap8;

        // Load all the Map32 data ahead of time
        for(i = 0; i < 0xC0; ++i)
        {
            this->allHoles[i]    = NULL;
            this->allEntr[i]     = NULL;
            this->allExits[i]    = NULL;
            this->allItems[0][i] = NULL;

            if(i >= 0xA0)
            {
                this->map32Data[i] = NULL;
                continue;
            }

            this->map32Data[i] = LoadMap32(i);
            j &= (bool) this->map32Data[i];
        }

        for(i = 0; i < 12; ++i)
        {
            this->bStock[i] = NULL;
            this->hStock[i] = NULL;
        }

        LoadMap32To16();
        LoadMap16To8();

        // load various types of different data
        LoadAllHoleData();
        LoadAllEntranceData();
        LoadAllExitData();
        LoadAllSpriteData();
        LoadAllItemData();
        LoadOverlaysOld();
        
        // Initializes all the map16 and map32 flags so we can know which tiles are being used and which aren't.
        LoadMapFlags();

        j &= !!this->gi;
        j &= !!this->map16Buf; 
        j &= !!this->map32Data; 
        j &= !!this->map32To16;
        j &= !!this->map16To8;
        j &= !!this->map32Flags; 
        j &= !!this->map16Flags; 
        j &= !!this->map8Buf;
    }

// ===============================================================

    bool OverData::LoadArea()
    {
        bool result     = true;
        bool darkWorld  = false;

        // ----------------------------

        if(area < 0x80)
        {
            // Check whether we're in the dark world.
            darkWorld = (area & 0x40) ? true : false;

            // Check whether the map is 1024x1024 (loads 4 areas) or 512x512 (loads 1 area)
            largeArea = GetByte(rom, 0x12844 + (area & 0x3F) ) != 0 ? true : false;

            if(largeArea)
            {
                // Maps 512x512 area number to the larger 1024x1024 area number that it belongs to.
                area = GetByte(rom, 0x125EC + (area & 0x3F)) + (darkWorld ? 0x40 : 0x00);

                // Loads all four of the 512x512 submaps that make up the large area.
                LoadMap16(area + 0, upper_left);
                LoadMap16(area + 1, upper_right);
                LoadMap16(area + 8, lower_left);
                LoadMap16(area + 9, lower_right);
            }
            else
                LoadMap16(area, upper_left);

        }

        this->areaHoles = this->allHoles[area];
        this->areaEntr  = this->allEntr[area];
        this->areaExits = this->allExits[area];
        this->areaItems = this->allItems[0][area];
        this->areaSpr   = this->allSpr[1][area];

        /// disabled temporarily
        /// LoadOverlay();

        // refresh the tiles in the map8 buffer
        LoadMap8();

        return true;
    }

// ===============================================================

    void OverData::LoadMap32To16()
    {
        u32 i        = 0;
        u16 value    = 0;
        u16 base     = 0;
        u16 index    = 0;
    
        u16 map16Vals[4]  = {0, 0, 0, 0};

        // default offsets for the conversion arrays.
        u32 offsets[4]    = { 0x18000, 0x1B400, 0x20000, 0x23400 };

        // corresponds to the upper left, upper right, lower left, lower right map16 values of the map32 black
        const u8 shift[4] = {0x04, 0x00, 0x0C, 0x08}; 

        bufPtr a   = this->map32To16;

        // --------------------------------------

        offsets[0] = CpuToRomAddr(header->map32To16UL);
        offsets[1] = CpuToRomAddr(header->map32To16UR);
        offsets[2] = CpuToRomAddr(header->map32To16LL);
        offsets[3] = CpuToRomAddr(header->map32To16LR);

        for(i = 0, value = 0; value < 0x2AA7; i += 8, value++)
        {
            base = (value / 4) * 6;

            index = value & 0x03;
            //index = value & 0x40;

            if(value % 0x100 == 0)
                value = value;

            map16Vals[0] = ((Get2Bytes(this->rom, offsets[0] + 4 + base)) >> shift[index]);
            map16Vals[0] = (map16Vals[0] & 0x0F) << 8;
            map16Vals[0] |= GetByte(this->rom, offsets[0] + index + base);

            map16Vals[1] = ((Get2Bytes(this->rom, offsets[1] + 4 + base)) >> shift[index]);
            map16Vals[1] = (map16Vals[1] & 0x0F) << 8;
            map16Vals[1] |= GetByte(this->rom, offsets[1] + index + base);

            map16Vals[2] = ((Get2Bytes(this->rom, offsets[2] + 4 + base)) >> shift[index]);
            map16Vals[2] = (map16Vals[2] & 0x0F) << 8;
            map16Vals[2] |= GetByte(this->rom, offsets[2] + index + base);

            map16Vals[3] = ((Get2Bytes(this->rom, offsets[3] + 4 + base)) >> shift[index]);
            map16Vals[3] = (map16Vals[3] & 0x0F) << 8;
            map16Vals[3] |= GetByte(this->rom, offsets[3] + index + base);

            Put2Bytes(a, i + 0x00, map16Vals[0]); // top left
            Put2Bytes(a, i + 0x02, map16Vals[1]); // top right
            Put2Bytes(a, i + 0x04, map16Vals[2]); // bottom left
            Put2Bytes(a, i + 0x06, map16Vals[3]); // bottom right
        }

        return;
    }

// ===============================================================

    void OverData::PackMap32To16()
    {
        u32 i = 0, j = 0;
        u32 value = 0;
        u32 group[4]  = {0, 0, 0, 0};
        u32 shifts[4] = {4, 0, 12, 8};

        bufPtr a      = NULL;
        bufPtr packed = CreateBuffer(6);

        // ----------------------------------

        a = this->upperLeft32;
        a->wOffset = 0;

        for(i = 0; i < this->map32To16->length; i += 32)
        {
            for(j = 0, value = 0; j < 4; ++j)
            {
                group[j] = Get2Bytes(this->map32To16, i + (j * 8));
                PutByte(a, group[j]);
                value |= ((group[j] >> 8) & 0x0F) << shifts[j];
            }

            Put2Bytes(a, value);
        }

        // ----------------------------------

        a = this->upperRight32;
        a->wOffset = 0;

        for(i = 2; i < this->map32To16->length; i += 32)
        {
            for(j = 0, value = 0; j < 4; ++j)
            {
                group[j] = Get2Bytes(this->map32To16, i + (j * 8));
                PutByte(a, group[j]);
                value |= ((group[j] >> 8) & 0x0F) << shifts[j];
            }

            Put2Bytes(a, value);
        }

        // ----------------------------------

        a = this->lowerLeft32;
        a->wOffset = 0;

        for(i = 4; i < this->map32To16->length; i += 32)
        {
            for(j = 0, value = 0; j < 4; ++j)
            {
                group[j] = Get2Bytes(this->map32To16, i + (j * 8));
                PutByte(a, group[j]);
                value |= ((group[j] >> 8) & 0x0F) << shifts[j];
            }

            Put2Bytes(a, value);
        }

        // ----------------------------------

        a = this->lowerRight32;
        a->wOffset = 0;

        for(i = 6; i < this->map32To16->length; i += 32)
        {
            for(j = 0, value = 0; j < 4; ++j)
            {
                group[j] = Get2Bytes(this->map32To16, i + (j * 8));
                PutByte(a, group[j]);
                value |= ((group[j] >> 8) & 0x0F) << shifts[j];
            }

            Put2Bytes(a, value);
        }

        DeallocBuffer(packed);

        return;
    }

// ===============================================================

    void OverData::LoadMap16To8()
    {
        CopyBuffer(this->map16To8, this->rom,
                   0,              CpuToRomAddr(0xF8000),
                   0x7540);
    }

// ===============================================================

    u32 OverData::Map16To8(u16 map16Val, u16 map8Vals[4])
    {
        u32 index = map16Val * 8;

        map8Vals[0] = Get2Bytes(this->map16To8, index);
        map8Vals[1] = Get2Bytes(this->map16To8, index + 2);
        map8Vals[2] = Get2Bytes(this->map16To8, index + 4);
        map8Vals[3] = Get2Bytes(this->map16To8, index + 6);

        if(this->rom->error)
            return 0;

        return 1;
    }

// ===============================================================

    OverData::~OverData()
    {
        u32 i = 0;

        delete gi;
        
        for(i = 0; i < 0xA0; ++i)
            DestroyBuffer(this->map32Data[i]);
    }

    static FILE* mapCountFile = fopen("C:\\map32log.log", "wt");

// ===============================================================

    void OverData::DecMapCounts(u16 mapVal, bool map32)
    {
        u32 a = 0;
        
        u16    *total = &(this->numMap16Tiles);
        bufPtr counts = this->map16Counts;
        bufPtr flags  = this->map16Flags;
        bufPtr assoc  = this->map16To8;

        // ------------------------------------------------------

        if(map32)
        {
            total  = &(this->numMap32Tiles);
            counts = this->map32Counts;
            flags  = this->map32Flags;
            assoc  = this->map32To16;
        }

        a = Get4Bytes(counts, mapVal * 4);

        if(a == 0)
        {
            mapCountFile = fopen("C:\\map32log.log", "at");

            fprintf(mapCountFile, "map32: %u    value: %4x\n", (int) map32, mapVal);
            fflush(mapCountFile);

            fclose(mapCountFile);
            
        }

        // subtract one but don't wrap under 0
        if(a)
            a = a - 1;

        if(!a)
            a = a;

        Put4Bytes(counts, mapVal * 4, a);

        if(a)
            PutBit(flags, 1, mapVal);
        else
        {
            PutBit(flags, 0, mapVal);
            (*total) -= 1;
        }

    }

// ===============================================================

    void OverData::IncMapCounts(u16 mapVal, bool map32, u16 *map2x2)
    {
        u32 a = 0;
        u32 offset = mapVal * 8;

        u16    *total = &(this->numMap16Tiles);
        bufPtr counts = this->map16Counts;
        bufPtr flags  = this->map16Flags;
        bufPtr assoc  = this->map16To8;

        // ------------------------------------------------------

        if(map32)
        {
            total  = &(this->numMap32Tiles);  
            counts = this->map32Counts;
            flags  = this->map32Flags;
            assoc  = this->map32To16;
        }

        a = Get4Bytes(counts, mapVal * 4);

        if(a == 0xFFFFFFFF)
        {
            mapCountFile = fopen("C:\\map32log.log", "at");

            fprintf(mapCountFile, "map32: %u    value: %4x\n", (int) map32, mapVal);
            fflush(mapCountFile);

            fclose(mapCountFile);
        }

        if(!a)
            a = a;

        a = (a < 0xFFFFFFFF) ? a + 1 : a; // add one but don't wrap past 0xFFFFFFFF

        Put4Bytes(counts, mapVal * 4, a);
        PutBit(flags, 1, mapVal);

        // On a zero to one transition on counts, we replace
        // the map16 to map8 correspondances, as they might have changed.
        if( (a == 1) && (map2x2) )
        {
            Put2Bytes(assoc, offset    , map2x2[0]);
            Put2Bytes(assoc, offset + 2, map2x2[1]);
            Put2Bytes(assoc, offset + 4, map2x2[2]);
            Put2Bytes(assoc, offset + 6, map2x2[3]);

            (*total) += 1;
        }

    }

// ===============================================================

    bufPtr OverData::LoadMap32(u32 mapNum)
    {
        u8 a         = 0;
        u8 b         = 0;

        u32 i        = 0;
        u32 tableLoc = 0;
        u32 offset   = 0;

        bufPtr data1 = NULL;
        bufPtr data2 = NULL;

        // map32 in Zelda should contain 0x200 bytes of data
        bufPtr map32Buf = CreateBuffer(0x10, 0x10, 2);

        // ---------------------------------------------

        tableLoc = CpuToRomAddr(Get3Bytes(this->rom, asm_overmap_ptrs));
        offset   = Get3Bytes(this->rom, tableLoc + (mapNum * 3));
        data1    = DecompressBank02(this->rom, CpuToRomAddr(offset) );

        tableLoc = CpuToRomAddr(Get3Bytes(this->rom, asm_overmap_ptrs2));
        offset   = Get3Bytes(this->rom, tableLoc + (mapNum * 3));
        data2    = DecompressBank02(this->rom, CpuToRomAddr(offset) );

        for(i = 0; i < 0x100; i++)
        {
            // notice how this data is interlaced from two separate sources
            // strange storage method... maybe it compresses better that way
            a = GetByte(data1, i);
            b = GetByte(data2, i);
            PutByte(map32Buf, (i*2)+1, a);
            PutByte(map32Buf, (i*2),   b);
        }

        DeallocBuffer(data1);
        DeallocBuffer(data2);

        // signal that there's a problem if the map32 data is not of the correct size
        if(map32Buf->length != 0x200)
            map32Buf->error = 1;

        return map32Buf;
    }

// ===============================================================

    u16 OverData::GetMap32Tile(u32 x, u32 y)
    {
        u32 area = this->area;
        bufPtr map32Buf;

        // ----------------------------

        if(x >= 0x10)
        {
            x -= 0x10;
            area += 1;
        }

        if(y >= 0x10)
        {
            y -= 0x10;
            area += 0x08;
        }

        if(area > 0xC0)
            return -1;

        map32Buf = this->map32Data[area];

        return Get2Bytes(map32Buf, x, y);
    }

// ===============================================================

    u32 OverData::PutMap32Tile(u32 x, u32 y, u32 value)
    {
        u32 area = this->area;
        bufPtr map32Buf;

        if(x >= 0x10)
        {
            x -= 0x10;
            area += 1;
        }

        if(y >= 0x10)
        {
            y -= 0x10;
            area += 0x08;
        }

        if(area > 0xC0)
            return -1;

        map32Buf = this->map32Data[area];

        return Put2Bytes(map32Buf, x, y, value);
    }

// ===============================================================

    static u16 reservedMap16[] = 
    {
        stock_bush,
        stock_bush2,
        stock_rock_sl,
        stock_rock_sh,
        stock_peg,
        stock_rock_bl0,
        stock_rock_bl1,
        stock_rock_bl2,
        stock_rock_bl3,
        stock_rock_bh0,
        stock_rock_bh1,
        stock_rock_bh2,
        stock_rock_bh3,
        stock_warp,
        stock_rockPile0,
        stock_rockPile1,
        stock_rockPile2,
        stock_rockPile3,
        stock_sign,
        stock_grass,
        stock_jumpPoint,
        stock_stump0,
        stock_stump1,
        stock_stump2,
        stock_stump3,
        0x0212,
        0x0DA4,
        0x0DA5,
        0x0DA6,
        0x0DA7,
        0x0DB5,
        0x0DC7,
        0x0DC8,
        0x0E1B,
        0x0E21,
        0x0E25,
        0x0E3F,
        0x0E48,
        0x0E54,
        0x0E64,
        0x0E88,
        0x0E8C,
        0x0E92,
        0x0E9A,
        0x0E9C
    };

    void OverData::LoadMapFlags()
    {
        bool usingMap32 = true;
        bool usingMap16 = false; // these values are rigged for the sake of Inc / Dec MapCounts routines
        
        u16 map16Val = 0;
        u16 map32Val = 0;
        u32 i = 0, j = 0;
        u32 numReserved = sizeof(reservedMap16) / sizeof(u16);

        bufPtr map16Buf = this->map16Buf;
        bufPtr map32Buf = NULL;

        // -----------------------------------

        for(i = 0; i < 0xA0; i += 4)
        {
            // upper left            
            map32Buf = this->map32Data[i + 0];

            for(j = 0; j < map32Buf->length; j += 2)
            {
                map32Val = Get2Bytes(map32Buf, j);
                IncMapCounts(map32Val, usingMap32);
            }
        
            LoadMap16(i + 0, upper_left);

            // upper right
            map32Buf = this->map32Data[i + 1];

            for(j = 0; j < map32Buf->length; j += 2)
            {
                map32Val = Get2Bytes(map32Buf, j); 
                IncMapCounts(map32Val, usingMap32);
            }

            LoadMap16(i + 1, upper_right);

            // lower left
            map32Buf = this->map32Data[i+2];

            for(j = 0; j < map32Buf->length; j += 2)
            {
                map32Val = Get2Bytes(map32Buf, j); 
                IncMapCounts(map32Val, usingMap32);
            }

            LoadMap16(i + 2, lower_left);

            // lower right
            map32Buf = this->map32Data[i+3];

            for(j = 0; j < map32Buf->length; j += 2)
            {
                map32Val = Get2Bytes(map32Buf, j); 
                IncMapCounts(map32Val, usingMap32);
            }
        
            LoadMap16(i + 3, lower_right);

            // At this point we're done loading the 4 map32 sets and now we process the map16
            // Flags for the result of that.
            for(j = 0; j < map16Buf->length; j += 2)
            {
                map16Val = Get2Bytes(this->map16Buf, j);
                IncMapCounts(map16Val, usingMap16);
            }
        }
        
        // Make sure to protect the map16 tiles that have special purposes
        for(i = 0; i < numReserved; ++i)
            IncMapCounts(reservedMap16[i], usingMap16);

        // counting the number of used map32 tiles
        for(j = 0, i = 0; i < this->map32Flags->length * 8; i++)
        {
            if(GetBit(this->map32Flags, i))
                j++;
        }

        this->numMap32Tiles = j;

        // counting the number of used map16 tiles
        for(j = 0, i = 0; i < this->map16Flags->length * 8; i++)
        {
            if(GetBit(this->map16Flags, i))
                j++;
        }

        this->numMap16Tiles = j;
    }

// ===============================================================

    bufPtr OverData::LoadMap8()
    {
        u16 map8Vals[4]; 

        u32 i = 0;
        u32 j = 0;
        u32 k = 0;
        u32 index = 0;

        // --------------------------------

        // convert the map16 data into map8 tiles
        for
        (
            i = 0, j = 0, k = 0;
            i < map16Buf->length;
            i += 2, j += 4, k += 4
        )
        {
            if(k == 0x100)
            {
                k = 0;
                j += 0x100;
            }

            index = Get2Bytes(map16Buf, i) * 8;

            map8Vals[0] = Get2Bytes(map16To8, index + 0);
            Put2Bytes(map8Buf, j, map8Vals[0]);

            map8Vals[1] = Get2Bytes(map16To8, index + 2);
            Put2Bytes(map8Buf, j + 0x02, map8Vals[1]);

            map8Vals[2] = Get2Bytes(map16To8, index + 4);
            Put2Bytes(map8Buf, j + 0x100, map8Vals[2]);

            map8Vals[3] = Get2Bytes(map16To8, index + 6);
            Put2Bytes(map8Buf, j + 0x102, map8Vals[3]);
        }

        return map8Buf;
    }

// ===============================================================

    bufPtr OverData::LoadMap16(u32 index, map16Pos p)
    {   
        u32 map32Val = 0;
        u16 map16Vals[4];

        u32 i = 0;
        u32 j = 0;
        u32 k = 0;

        bufPtr map32Buf  = this->map32Data[index & 0xFF];
        bufPtr map32To16 = this->map32To16;

        // -----------------------------------------------

        // pretty complex for loop eh?
        for
        (
            k = 0x10, i = 0, j = (u32) p;
            i < 0x200;
            i += 2, j += 4, k--
        )
        { 
            // this conditional statement when activated, positions j
            // so that it will start writing to the next "line" of the map16 buffer
            if(!k)
            {
                j += 0xC0;
                k  = 0x10;
            }

            map32Val = Get2Bytes(map32Buf, i) * 8;

            map16Vals[0] = Get2Bytes(map32To16, map32Val + 0);
            map16Vals[1] = Get2Bytes(map32To16, map32Val + 2);
            map16Vals[2] = Get2Bytes(map32To16, map32Val + 4);
            map16Vals[3] = Get2Bytes(map32To16, map32Val + 6);

            Put2Bytes(this->map16Buf, j + 0x00, map16Vals[0]);
            Put2Bytes(this->map16Buf, j + 0x02, map16Vals[1]);
            Put2Bytes(this->map16Buf, j + 0x80, map16Vals[2]);
            Put2Bytes(this->map16Buf, j + 0x82, map16Vals[3]);
        }

        return map16Buf; 
    }


// ===============================================================

    u32 OverData::FindMap32(u16 map16Vals[4])
    {
        u16 i = 0;

        const u16 theEnd = 0x22A8;

        u16* search = (u16*) this->map32To16->contents;

        // shouldn't ever happen if the rom is the right size
        if(this->map32To16->length < 0x2AA00)
            return -1;

        for(i = 0; i < 0x5554; i++, search += 4)
        {
            if(i == theEnd) // Eureka Seven reference
                return -1;

            if(search[0] != map16Vals[0])
                continue;
            else if(search[1] != map16Vals[1])
                continue;
            else if(search[2] != map16Vals[2])
                continue;
            else if(search[3] != map16Vals[3])
                continue;
            else
                return (((u32) i) & 0x0000FFFF);
        }

        return -1;
    }

// ===============================================================

    u32 OverData::FindMap16(u16 map8Vals[4])
    {
        u16 i = 0;

        const u16 theEnd = 0x0DF4;

        u16* search = (u16*) this->map16To8->contents;

        for(i = 0; i < 0x3000; i++, search += 4)
        {
            if(i == theEnd) // Eureka Seven reference
                return -1;

            if(search[0] != map8Vals[0])
                continue;
            else if(search[1] != map8Vals[1])
                continue;
            else if(search[2] != map8Vals[2])
                continue;
            else if(search[3] != map8Vals[3])
                continue;
            else
                return (((u32) i) & 0x0000FFFF);
        }

        return -1;
    }

// ===============================================================

    bool OverData::LoadAllHoleData()
    {
        u32 i           = 0, x = 0, y = 0, area = 0, entrance = 0;
        u32 check       = 0;
        u32 numHoles    = 0;
        u32 mapOffset   = 0, areaOffset = 0, entrOffset  = 0;
        u32 map16Val    = 0;
        
        Entrance *hole  = NULL;

        // -------------------------------

        numHoles    = header->overNumHoles;
        mapOffset   = CpuToRomAddr(header->overHoleOffset);
        areaOffset  = mapOffset  + (numHoles << 1);
        entrOffset  = areaOffset + (numHoles << 1);

        // basic error checking
        if(numHoles > maxHoles)
            return false;

        for(i = 0; i < numHoles; ++i)
        {
            // note the odd formula for the map16
            map16Val = Get2Bytes(rom, mapOffset + (i << 1) ) + 0x400;
            x        = ((map16Val & 0x007F) >> 1) << 4;
            y        = ((map16Val & 0x1F80) >> 7) << 4;
            area     = Get2Bytes(rom, areaOffset + (i << 1) );
            entrance = GetByte(rom, entrOffset + i);

            if(area >= 0xC0)
                continue;

            hole = new Entrance(x, y, entrance, area);

            allHoles[area] = (Entrance*) MarkerList::Add(allHoles[area], hole);
        }

        return true;
    }

// ===============================================================

    bool OverData::LoadAllEntranceData()
    {
        u32 i           = 0, x = 0, y = 0, area = 0, entrance = 0;
        u32 check       = 0;
        u32 numEntr     = 0;
        u32 mapOffset   = 0, areaOffset = 0, entrOffset  = 0;
        u32 map16Val    = 0;

        Entrance *entr = NULL;

        // -------------------------------

        numEntr     = header->overNumEntr;      // hardcoded for now
        areaOffset  = CpuToRomAddr(header->overEntrOffset);   // hardcoded for now
        mapOffset   = areaOffset + (numEntr << 1);
        entrOffset  = mapOffset  + (numEntr << 1);

        // basic error checking
        if(numEntr > maxHoles)
            return false;

        for(i = 0; i < numEntr; ++i)
        {
            // note the normal formula for entrances versus holes. strange.
            map16Val = Get2Bytes(rom, mapOffset + (i << 1) );
            x        = ((map16Val & 0x007F) >> 1) << 4;
            y        = ((map16Val & 0x1F80) >> 7) << 4;
            area     = Get2Bytes(rom, areaOffset + (i << 1) );
            entrance = GetByte(rom, entrOffset + i);

            if(area >= 0xC0)
                continue;

            entr = new Entrance(x, y, entrance, area);

            allEntr[area] = (Entrance*) MarkerList::Add(allEntr[area], entr);
        }

        return true;
    }

// ===============================================================

    bool OverData::LoadAllExitData()
    {
        u32 yStart      = 0;
        u32 xStart      = 0;
        u32 i           = 0, x = 0, y = 0, area = 0, room = 0, doorData = 0, doorData2 = 0;
        u32 check       = 0;
        u32 numExits    = 0;
        u32 roomOffset  = 0, areaOffset = 0, yOffset = 0, xOffset = 0, doorOffset = 0, doorOffset2 = 0;
        u32 map16Val    = 0;

        over_exit_type exit_type = over_exit_down; // defaults

        OverExit *exit = NULL;

        // -------------------------------

        numExits    = 0x4F; // hard coded for now.
        roomOffset  = CpuToRomAddr(0x02DD8A);
        areaOffset  = roomOffset + (numExits * 2);
        yOffset     = areaOffset + (numExits * 7);
        xOffset     = yOffset    + (numExits * 2);
        doorOffset  = xOffset    + (numExits * 8);
        doorOffset2 = doorOffset + (numExits * 2);

        // basic error checking
        if(numExits > 0x1000) // hardcoded for now
            return false;

        for(i = 0; i < numExits; ++i)
        {
            // note the normal formula for entrances versus holes. strange.
            room     = Get2Bytes(rom, roomOffset + (i * 2) );
            area     = GetByte(rom, areaOffset + i);
            
            xStart   = (area & 0x07) << 9;
            yStart   = (area & 0x38) << 6;

            x        = Get2Bytes(rom, xOffset + (i * 2) ) - xStart;
            y        = Get2Bytes(rom, yOffset + (i * 2) ) - yStart;

            doorData = Get2Bytes(rom, doorOffset + (i * 2) );

            if(doorData == 0x0000)
                exit_type = over_exit_down;
            else if(doorData == 0xFFFF)
                exit_type = over_exit_up;
            else if(doorData & 0x8000)
                exit_type = over_exit_bombable;
            else 
                exit_type = over_exit_wooden;

            if(!doorData)
            {
                doorData2 = Get2Bytes(rom, doorOffset2 + (i * 2) );

                if(doorData2 & 0x8000)
                    exit_type = over_exit_palace;
                else if(doorData2 != 0x0000)
                    exit_type = over_exit_sanctuary;

                doorData = doorData2;
            }

            exit = new OverExit(x, y, area, room, exit_type);

            exit->door->SetPos(GetMap16X(doorData), GetMap16Y(doorData));

            allExits[area] = OverExit::Add(allExits[area], exit);
        }

        return true;
    }

// ===============================================================

    bool OverData::LoadAllSpriteData()
    {
        bool overlord       = false;
        bool usingOld       = false;

        // sprite id, x coordinate, and y coordinate
        u8  id      = 0, x      = 0, y = 0;

        u32 i       = 0, j      = 0, k = 0;
        u32 value   = 0, offset = 0;

        u32 tableSizes[3]   = { 0x90, 0x90, 0x90 }; // number of areas in each table
        u32 tableOffset[3] = { header->overSprOffset0, header->overSprOffset2, header->overSprOffset3 };

        OverSpr *s = NULL;
        
        // ---------------------------
        
        usingOld = (tableOffset[0] == 0x9C881) ? true : false;

        if(!usingOld) { for(i = 0; i < 3; ++i) {  tableSizes[3] = 0xC0; } }

        for(i = 0; i < 3; ++i)
        {               
            for(j = 0; j < tableSizes[i]; ++j)
            {
                allSpr[i][j] = NULL;

                // assumed to be nonpresent.
                fallingRocks[i][j] = false;

                // dark world and extended areas in the "beginning" part don't have sprites in the original rom.
                if( (usingOld) && (i == 0) && (j >= 0x40) )
                    continue;

                offset = CpuToRomAddr(tableOffset[i]);

                // the new mechanism and the old one are different in that the old mechanism uses 2-byte pointers in the same bank as the table.
                offset = (usingOld) ? CpuToRomAddr( (0x090000) | Get2Bytes(rom, offset + (j * 2) ) ) : 
                                      CpuToRomAddr( Get3Bytes(rom, offset + (j * 3) ) );
               
                for(k = offset; (k & 0xFFFF) < 0xFFFD; k += 3)
                {
                    y   = GetByte(rom, k);

                    if(y == 0xFF)
                       break;

                    x   = GetByte(rom, k + 1);
                    id  = GetByte(rom, k + 2);

                    // falling rocks don't really count as a sprite so we skip it
                    if(id == 0xF4)
                    {
                        fallingRocks[i][j] = true;
                        continue;
                    }
                    else if(id >= 0xF5)
                    {
                        overlord = true;
                        id -= 0xF5;
                    }

                    allSpr[i][j] = (OverSpr*) MarkerList::Add(allSpr[i][j], new OverSpr(id, x << 4, y << 4) ); 
                }

            }

            for(j = tableSizes[i]; j < 0xC0; ++j)
                allSpr[i][j] = NULL;
        }

	    return true;
    }

// ===============================================================

    bool OverData::LoadAllItemData()
    {
        u32 i       = 0, j = 0;
        u32 x       = 0, y = 0;
        u32 map16   = 0;
        u32 offset  = 0;
        u32 itemNum = 0;

        // ---------------------------
        
        for(i = 0; i < 0xC0; ++i)
        {
            if(i >= 0x80)
                continue;
            

            if(header->overItemOffset == 0x1BC2F9)
            {
                offset = CpuToRomAddr(header->overItemOffset + (i * 2) );
                offset = CpuToRomAddr(Get2Bytes(rom, offset) | (0x1B << 16) );
            }
            else
            {
                offset = CpuToRomAddr(header->overItemOffset + (i * 3) );
                offset = CpuToRomAddr(Get3Bytes(rom, offset));
            }
    
            for(j = 0;  ; j += 3)
            {
                map16  = Get2Bytes(rom, offset + j);

                if(map16 == 0xFFFF)
                    break;
                
                x = GetMap16X(map16);
                y = GetMap16Y(map16);

                itemNum = GetByte(rom, offset + j + 2);
                
                if(itemNum > 0x16)
                {
                    switch(itemNum)
                    {
                        case 0x80:
                        case 0x82:
                        case 0x84:
                        case 0x86:
                        case 0x88:

                            break; // valid item numbers

                        default:

                            itemNum = 0x00;
                   }
                }

                allItems[0][i] = (OverItem*) OverItem::Add(allItems[0][i], 
                                                           new OverItem(itemNum, x, y));
            }
        }
       
        return true;
    }

// ===============================================================

        OwOverlay::OwOverlay()
        {
            data = CreateBuffer(0x40, 0x40, 2);

            Initialize();
        }

        OwOverlay::~OwOverlay()
        {
            DeallocBuffer(data);
        }

        void OwOverlay::Initialize()
        {
            // fill with -1 (null) map16 values)
            forgrid(Put2Bytes(data, i, j, 0xFFFF));
        }
        
        bool OwOverlay::SetTile(u16 addr, u16 value)
        {
            u8 x = GetMap16X(addr) >> 4;
            u8 y = GetMap16Y(addr) >> 4;

            if(addr > 0x2000)
                return false;

            return SetTile(value, x, y);
        }

        bool OwOverlay::SetTile(u16 value, u8 x, u8 y)
        {
            Put2Bytes(data, (u32) x, (u32) y, value);

            return true;
        }

        u16 OwOverlay::GetTile(u8 x, u8 y)
        {
            if(x >= 0x40)
                return -1;

            if(y >= 0x40)
                return -1;

            return Get2Bytes(data, (u32) x, (u32) y);
        }

// ===============================================================

    void OverData::LoadOverlaysOld()
    {
        bool done  = false;
        bool write = false;
        
        u8 opcode = 0;

        POINT min = {0, 0};
        POINT max = {0x3F, 0x3F};
        
        u32 i;
        u32 map16Val    = 0;
        u32 map16Addr   = 0;
        u32 offset      = 0;
        u32 x_reg       = 0;
       
        OwOverlay *lay  = NULL;
       
        // -----------------------

        for(i = 0; i < 0x80; ++i) 
        {
            done   = false;

            lay    = overlays[i] = new OwOverlay();

            // don't use bad pointers.
            if(!lay)
                return;
            
            // Get a pointer to the overlay's routine in the rom.
		    offset = Get2Bytes(rom, 0x77664 + (i << 1) ) + (0x0E << 0x10);
            offset = CpuToRomAddr(offset);
        
            // handles parsing of different ASM opcodes because 
            // the overlays are all stored as ASM instructions rather than data... (Yeah, dumb, I know)

            while(!done)
            {
                write  = false;
                opcode = GetByte(rom, offset++);

                switch(opcode) 
                {
                    // increments the map16 value.
                    case 0x1a:
                        map16Val += 1;
				        break;

                    // JMP instruction (tells us to go to another location in the rom to finish the routine)
                    case 0x4c:
                        offset = Get2Bytes(rom, offset) + (0x0E << 0x10);
                        offset = CpuToRomAddr(offset);
				        break;
			        
                    // RTS instruction (ends the routine)
                    case 0x60:
                        done = true;
                        break;
			        
                    // STA absolute instruction. The map16 address is the argument.
                    case 0x8d:
                        map16Addr = Get2Bytes(rom, offset);
				        offset += 2;
                        write = true;
                        break;
           
                    // STA long instruction. Uses a 2-byte indexed pointer
                    case 0x9d:
                        // Get the 3-byte pointer but strip off the 0x7E.
                        map16Addr = Get2Bytes(rom, offset) + x_reg;

                        offset += 2;
                        write = true;
                        break;
			        
                    // STA long instruction.
                    case 0x8f:
                        // Get the 3-byte pointer but strip off the 0x7E.
                        map16Addr = Get3Bytes(rom, offset);

                        if(GetBank(map16Addr) != 0x7E)
                            done = true;
                        
				        offset += 3;
                        write = true;
                        break;
			        
                    // STA long indexed instruction.
                    case 0x9f:
                        map16Addr = Get3Bytes(rom, offset) + x_reg;

                        // bad bank, must be 0x7E!
                        if(GetBank(map16Addr) != 0x7E)
                            done = true;
                            
                        offset += 3;
                        write = true;
                        break;
			        
                    case 0xa2:
				        x_reg = Get2Bytes(rom, offset);
				        offset += 2;
				        break;
			        
                    case 0xa9:
				        map16Val = Get2Bytes(rom, offset);
				        offset += 2;
				        break;
                    
                    case 0xea:
                        offset = offset;
				        break;

                    default:
                        done = true;
                        break;
			    }
               
                if(offset > 0x78000)
                    done = true;
               
                if(write)
                {
                    if((map16Addr < 0x2000) || (map16Addr > 0x3FFF))
                        done = true;
                    else
                    {
                        //map16Addr -= 0x2000;
                    
                        if(!lay->SetTile(map16Addr - 0x2000, map16Val))
                            done = true;                            
                        else
                            IncMapCounts(map16Val, false);
                    }
                }
		    }
        }
    }

// ===============================================================

    bool OverData::LoadOverlay()
    {
        u32 i        = 0;
        u32 j        = 0;
        u32 map16Val = 0;

        OwOverlay *lay = overlays[area];

        // -----------------------

        ///return false;

        for(i = 0; i < 0x40; ++i)
        { 
            for(j = 0; j < 0x40; ++j)
            {
                map16Val = lay->GetTile(i, j);

                if(map16Val == 0xFFFF)
                    continue;

                SetMap16Tile(map16Buf, map16Val, i, j);
            }
        }

        return true;
    }

// ===============================================================
