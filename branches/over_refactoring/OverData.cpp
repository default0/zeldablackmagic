
	#include "Globals.h"
	#include "Core.h"
    #include "OverData.h"
	#include "OverMode.h"
	#include "OverModeGui.h"
    #include "compresch_lttp.h"

    const bool usingMap16 = false;
    const bool usingMap32 = true;

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

    const u32 mask16      = ~0x01;
    const u32 mask32      = ~0x03;

    const u32 maxHoles    = 0x1998;

// ===============================================================

    // OverData Constructor function

    OverData::OverData(bufPtr rom, BM_Header *header)
    {
        bool j  = true;

        u32 i   = 0;

        //---------------------------
        
        this->editOverlay   = false;

        this->tile8         = 0;
        this->tile16        = 0;

        this->tileSize      = fromMap8;

        this->editMode      = mode_select;

        this->rom           = rom;

        this->map8Buf       = CreateBuffer(0x80, 0x80, 2);

        this->map16Flags    = CreateBuffer(0x10000 / 8);

        this->map16Counts   = CreateBuffer(0x10000, 0x01, 4);

        this->map16To8      = CreateBuffer(0x40000);

        // copy of the BM_Header struct from the parent ZeldaGame object
        this->header        = header;

        this->pObj          = NULL;

        this->selObj        = new OverObj();
        this->selObj2       = new OverObj();

        // Load all the Map32 data ahead of time
        for(i = 0; i < 0xC0; ++i)
        {
            u8 a = (u8) i;

            // initialize all area pointers to NULL to start with
            areas[a] = NULL;

            // large areas group together 4 small areas
            a = ResolveArea(a);

            if(areas[a] == NULL)
            {
                areas[a] = new OverArea(a, rom);
            }
            else
            {
                // copy the pointer from the slot for the large area
                // that this area is part of, to avoid confusion
                areas[i] = areas[a];
            }

            j &= (this->areas[i] == 0 ? true : false);
        }

        this->area          = areas[0x03];

        LoadMap16To8();
        
        // Initializes all the map16 and map32 flags so we can know which tiles are being used and which aren't.
        LoadMapFlags();
        
        if(1) /// change this to something like "usingOld" later when we get an implementation up and running
        {
            for(i = 0; i < 0xA0; ++i)
            {
                map32Data[i] = LoadMap32(i);

            }

            LoadMap32To16();

            // load various types of different data
            LoadOverlaysOld();
            LoadAllHoleData();
            LoadAllEntranceData();
            LoadAllExitData();
            LoadAllSpriteData();
            LoadAllItemData();

            j &= !!this->map32To16;
        }

        j &= !!this->map16Buf; 
        j &= !!this->map16To8;
        j &= !!this->map16Flags; 

        j &= !!this->map8Buf;

        /// GUI related crap that needs to be phased somewhere else.
        this->stockContext  = GetSubMenu(LoadMenu(thisProg, MAKEINTRESOURCE(IDR_OVER_STOCK)), 0);
        this->selectContext = GetSubMenu(LoadMenu(thisProg, MAKEINTRESOURCE(IDR_OVER_SELECT)), 0);
        this->sprContext    = GetSubMenu(LoadMenu(thisProg, MAKEINTRESOURCE(IDR_OVER_SPRITE)), 0);
        this->itemContext   = GetSubMenu(LoadMenu(thisProg, MAKEINTRESOURCE(IDR_OVER_ITEM)), 0);
        this->exitContext   = GetSubMenu(LoadMenu(thisProg, MAKEINTRESOURCE(IDR_OVER_EXIT)), 0);
        this->entrContext   = GetSubMenu(LoadMenu(thisProg, MAKEINTRESOURCE(IDR_OVER_ENTR)), 0);
        this->holeContext   = GetSubMenu(LoadMenu(thisProg, MAKEINTRESOURCE(IDR_OVER_HOLE)), 0);

        for(i = 0; i < 13; ++i)
        {
            this->bStock[i] = NULL;
            this->hStock[i] = NULL;
        }
        /// End of GUI crap
    }

// ===============================================================

    OverData::~OverData()
    {
        /// insert code to destroy all OverArea objects later!!!


    }

    static FILE* mapCountFile = fopen("C:\\map32log.log", "wt");

// ===============================================================

    u8 OverData::ResolveArea(u8 area)
    {
        if(area < 0x80)
        {
            // Maps 512x512 area number to the larger 1024x1024 area number that it belongs to.
            return ( GetByte(rom, 0x125EC + (area & 0x3F)) + (area & 0x40) );
        }
        
        // areas 0x80 and above just return the normal area index....
        return area;
    }

// ===============================================================

    bool OverData::LoadArea()
    {
        bool result     = true;
        bool darkWorld  = false;

        // ----------------------------

        /**

        if(area->areaNum < 0x80)
        {
            // Check whether we're in the dark world.
            darkWorld = (area->areaNum & 0x40) ? true : false;
            
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

        */

        Map16Analysis();

        /**        
        areaHoles = allHoles[area];
        areaEntr  = allEntr[area];
        areaExits = allExits[area];
        areaItems = allItems[0][area];
        areaSpr   = allSpr[1][area];
        */

        if(this->editOverlay == true)
        {
            area->LoadOverlay();
        }

        // refresh the tiles in the map8 buffer
        LoadMap8();
        
        return true;
    }

// ===============================================================

    void OverData::Map16Analysis()
    {
        u16 i = 0, j = 0;
        u16 x = 0, y = 0;
        u16 baseX = 0, baseY = 0;

        u16 numBytes = 0;

        u16 val1 = 0, val2 = 0;

        u32 k = 0;

        FILE *f = NULL;

        u16 map8_1[4];
        u16 map8_2[4];

        bufPtr flags = CreateBuffer(0x10000 / 8);

        Map16Buf *map16 = area->map16;

        // -----------------------

        // return;

        for(i = 0; i < map16->width; ++i)
        {
            for(j = 0; j < map16->height; ++j)
            {
                val1 = map16->GetTile(i, j);

                if(!GetBit(flags, val1))
                    k += 1;

                PutBit(flags, 1, val1);
            }
        }

        return;

        for(i = 0; i < numMap16Tiles; ++i)
        {
            for(j = 0; j < i; ++j)
            {
                Map16To8(i, map8_1);
                Map16To8(j, map8_2);

                if(map8_1[0] != map8_2[0])
                    continue;
                else if(map8_1[1] != map8_2[1])
                    continue;
                else if(map8_1[2] != map8_2[2])
                    continue;
                else if(map8_1[3] != map8_2[3])
                    continue;
                else
                    k += 1;

            }
        }

        k = 0;

        return;

        f = fopen("map16Dist.log", "wt");

        for(k = 0; k < map16Counts->length; k += 4)
        {
            val1 = Get4Bytes(map16Counts, k);

            if(val1)
                fprintf(f, "%04X\t%d\n", (k / 4), val1);
        }

        fclose(f);

        return;

        for(y = 0; y < map16->height; ++y)
        {
            for(x = 0; x < map16->width; ++x)
            {
                baseY = 0;

                for( ; baseY < map16->height; ++baseY)
                {
                    baseX = 0;

                    if( ( (baseX) + ( (baseY) << 6)) >= (x + (y << 6)) )
                        break;                        

                    for( ; baseX < map16->width; ++baseX)
                    {
                        if( ( (baseX) + ( (baseY) << 6)) >= (x + (y << 6)) )
                            break;

                        numBytes = 0;

                        // width first loop
                        for(i = 0, j = 0; (baseY + j) < map16->height; ++j)
                        {
                            if( ( (baseX + i) + ( (baseY + j) << 6)) >= (x + (y << 6)) )
                                break;

                            val1 = Get2Bytes(map16Buf, x     + i, y     + j);
                            val2 = Get2Bytes(map16Buf, baseX + i, baseY + j);

                            if(val1 != val2)
                                break;

                            for(i = 0; (baseX + i) < map16->width; ++i)
                            {
                                if( ( (baseX + i) + ( (baseY + j) << 6)) >= (x + (y << 6)) )
                                    break;

                                val1 = map16->GetTile(x     + i, y     + j);
                                val2 = map16->GetTile(baseX + i, baseY + j);

                                if(val1 != val2)
                                    break;
                                else
                                    numBytes += 1;

                            }

                        }

                        if(numBytes > 0x17)
                            numBytes = numBytes;

                        numBytes = 0;

                        // height first loop
                        for(i = 0, j = 0; (baseX + i) < map16->width; ++i)
                        {
                            if( ( (baseX + i) + ( (baseY + j) << 6)) >= (x + (y << 6)) )
                                break;

                            val1 = Get2Bytes(map16Buf, x     + i, y     + j);
                            val2 = Get2Bytes(map16Buf, baseX + i, baseY + j);

                            if(val1 != val2)
                                break;

                            for(j = 0; (baseY + j) < map16->height; ++j)
                            {
                                if( ( (baseX + i) + ( (baseY + j) << 6)) >= (x + (y << 6)) )
                                    break;

                                val1 = map16->GetTile(x     + i, y     + j);
                                val2 = map16->GetTile(baseX + i, baseY + j);

                                if(val1 != val2)
                                    break;
                                else
                                    numBytes += 1;
                            }
                        }
                        
                        if(numBytes > 0x17)
                            numBytes = numBytes;
                    }
                }
            }
        }

        return;
    }

// ===============================================================

    bool OverData::UnloadArea()
    {
        // since these area all dynamic lists, the pointer to the head of each list
        // could have changed during editing
        // that's why I'm updating the pointers in the main structures

        /**
        allEntr[area]     = areaEntr;
        allHoles[area]    = areaHoles;
        allExits[area]    = areaExits;
        allItems[0][area] = areaItems;
        allSpr[1][area]   = areaSpr;

        // If we don't make sure to unload the overlay when switching areas,
        // The changes to the map16 overlay will be lost.
        if(editOverlay == true)
            UnloadOverlay();

        */

        return true;
    }

// ===============================================================

    void OverData::LoadMap16To8()
    {
        CopyBuffer(this->map16To8, this->rom,
                   0,              CpuToRomAddr(0xF8000),
                   0x7540);
    }

// ===============================================================

    void OverData::LoadMap32To16()
    {
        u32 i        = 0;
        u16 value    = 0;
        u16 base     = 0;
        u16 index    = 0;
    
        u16 map16Vals[4]    = {0, 0, 0, 0};

        // default offsets for the conversion arrays.
        u32 offsets[4]      = { 0x18000, 0x1B400, 0x20000, 0x23400 };

        // corresponds to the upper left, upper right, lower left, lower right map16 values of the map32 block
        const u8 shift[4]   = {0x04, 0x00, 0x0C, 0x08}; 

        bufPtr a            = this->map32To16;

        // --------------------------------------

        for(i = 0, value = 0; value < 0x2AA7; i += 8, value++)
        {
            base = (value / 4) * 6;

            index = value & 0x03;

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

    u32 OverData::Map16To8(u16 map16Val, u16 map8Vals[4])
    {
        u32 index = map16Val * 8;

        map8Vals[0] = Get2Bytes(this->map16To8, index);
        map8Vals[1] = Get2Bytes(this->map16To8, index + 2);
        map8Vals[2] = Get2Bytes(this->map16To8, index + 4);
        map8Vals[3] = Get2Bytes(this->map16To8, index + 6);

        return 1;
    }

// ===============================================================

    u32 OverData::Map32To16(u16 map32Val, u16 map16Vals[4])
    {
        u32 index = map32Val * 8;

        map16Vals[0] = Get2Bytes(this->map32To16, index);
        map16Vals[1] = Get2Bytes(this->map32To16, index + 2);
        map16Vals[2] = Get2Bytes(this->map32To16, index + 4);
        map16Vals[3] = Get2Bytes(this->map32To16, index + 6);

        return 1;
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

    u32 OverData::AllocateMap16(u16 map8Vals[4], u16 oldMap16, u16 threshold)
    {
        u32 a = 0;
        u32 i = 0;

        for(i = threshold; i < 0x4000; i++)
        {
            // Get the number of times this particular map16 value occurs in the rom.
            a = Get4Bytes(map16Counts, i * 4);

            if(a > 1)
                continue;
            else if(a == 1)
            {
                // in this case the new map16 tile would replace the old
                if( (u16) i == oldMap16 )
                    return i;

                continue;
            }
            else
                return i;
        }

        return -1;
    }

// ===============================================================

    void OverData::DecMapCounts(u16 mapVal)
    {
        u16     *total  = &(this->numMap16Tiles);
        
        u32     a       = 0;

        bufPtr  counts  = this->map16Counts;
        bufPtr  flags   = this->map16Flags;
        bufPtr  assoc   = this->map16To8;

        // ------------------------------------------------------

        a = Get4Bytes(counts, mapVal * 4);

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

    void OverData::IncMapCounts(u16 mapVal, u16 *map2x2)
    {
        u16     *total  = &(this->numMap16Tiles);

        u32     a       = 0;
        u32     offset  = mapVal * 8;

        bufPtr  counts  = this->map16Counts;
        bufPtr  flags   = this->map16Flags;
        bufPtr  assoc   = this->map16To8;

        // ------------------------------------------------------

        a = Get4Bytes(counts, mapVal * 4);

        if(a == 0xFFFFFFFF)
            a = a;

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

        tableLoc = CpuToRomAddr(Get3Bytes(rom, asm_overmap_ptrs));
        offset   = Get3Bytes(rom, tableLoc + (mapNum * 3));
        data1    = DecompressBank02(rom, CpuToRomAddr(offset) );

        tableLoc = CpuToRomAddr(Get3Bytes(rom, asm_overmap_ptrs2));
        offset   = Get3Bytes(rom, tableLoc + (mapNum * 3));
        data2    = DecompressBank02(rom, CpuToRomAddr(offset) );

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

    void OverData::LoadMapFlags()
    {
        bool usingMap32 = true;
        bool usingMap16 = false; // these values are rigged for the sake of Inc / Dec MapCounts routines
        
        u16 map16Val = 0;

        u32 i = 0, j = 0;
        u32 numReserved = sizeof(reservedMap16) / sizeof(u16);

        bufPtr map16Buf = this->map16Buf;

        // -----------------------------------

        for(i = 0; i < 0xA0; i += 4)
        {
            LoadMap16(i + 0, upper_left);
            LoadMap16(i + 1, upper_right);
            LoadMap16(i + 2, lower_left);
            LoadMap16(i + 3, lower_right);

            // At this point we're done loading the 4 map32 sets and now we process the map16
            // Flags for the result of that.
            for(j = 0; j < map16Buf->length; j += 2)
            {
                map16Val = Get2Bytes(this->map16Buf, j);
                IncMapCounts(map16Val);
            }
        }
        
        // Make sure to protect the map16 tiles that have special purposes
        for(i = 0; i < numReserved; ++i)
            IncMapCounts(reservedMap16[i]);

        // counting the number of used map16 tiles
        for(j = 0, i = 0; i < this->map16Flags->length * 8; i++)
        {
            if(GetBit(this->map16Flags, i))
                j++;
        }

        this->numMap16Tiles = j;
    }

// ===============================================================

    bool OverData::LoadAllHoleData()
    {
        // (a stands for area)
        u32 i           = 0, x = 0, y = 0, a = 0, entrance = 0;
        u32 check       = 0;
        u32 numHoles    = 0;
        u32 mapOffset   = 0, areaOffset = 0, entrOffset  = 0;
        u32 map16Val    = 0;
        
        Entrance *hole  = NULL;
        Entrance **list = NULL;

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
            a        = Get2Bytes(rom, areaOffset + (i << 1) );
            entrance = GetByte(rom, entrOffset + i);

            if(areas[a]->areaNum >= 0xC0)
                continue;

            hole = new Entrance(x, y, entrance, a);

            list = &(areas[a]->holes);

            Entrance::Add(list, hole);
        }

        return true;
    }

// ===============================================================

    bool OverData::LoadAllEntranceData()
    {
        // (a stands for area)
        u32 i           = 0, x = 0, y = 0, a = 0, entrance = 0;
        u32 check       = 0;
        u32 numEntr     = 0;
        u32 mapOffset   = 0, areaOffset = 0, entrOffset  = 0;
        u32 map16Val    = 0;

        Entrance *entr  = NULL;
        Entrance **list = NULL; 

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
            a        = Get2Bytes(rom, areaOffset + (i << 1) );
            entrance = GetByte(rom, entrOffset + i);

            if(a >= 0xC0)
                continue;

            entr = new Entrance(x, y, entrance, a);
            
            list = &(areas[a]->entr);

            Entrance::Add(list, entr);
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

        OverExit *exit  = NULL;

        OverArea *a     = NULL;

        // -------------------------------

        numExits    = 0x4F; // hard coded for now.
        roomOffset  = CpuToRomAddr(0x02DD8A);
        areaOffset  = roomOffset + (numExits * 2);
        yOffset     = areaOffset + (numExits * 7);
        xOffset     = yOffset    + (numExits * 2);
        doorOffset  = xOffset    + (numExits * 8);
        doorOffset2 = doorOffset + (numExits * 2);

        /// basic error checking
        if(numExits > 0x1000) /// hardcoded for now
            return false;

        for(i = 0; i < numExits; ++i)
        {
            room        =Get2Bytes(rom, roomOffset + (i * 2) );
            area        = GetByte(rom, areaOffset + i);

            a           = areas[area];
            
            xStart      = (area & 0x07) << 9;
            yStart      = (area & 0x38) << 6;

            x           = Get2Bytes(rom, xOffset + (i * 2) ) - xStart;
            y           = Get2Bytes(rom, yOffset + (i * 2) ) - yStart;

            doorData    = Get2Bytes(rom, doorOffset + (i * 2) );

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

            OverExit::Add(&a->exits, exit);
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

        u32 phase   = 0, area   = 0;
        u32 i       = 0, j      = 0, k = 0;
        u32 value   = 0, offset = 0;

        u32 tableSizes[3]   = { 0x90, 0x90, 0x90 }; // number of areas in each table
        u32 tableOffset[3]  = { header->overSprOffset0, header->overSprOffset2, header->overSprOffset3 };

        OverSpr *s      = NULL;
        OverSpr **list  = NULL;

        OverArea *a     = NULL;
        
        // ---------------------------
        
        usingOld = (tableOffset[0] == 0x9C881) ? true : false;

        // after the rom has been saved once by Black Magic, sprites are stored for all 0xC0 areas, rather than the first 0x90.
        if(!usingOld) 
        { 
            for(i = 0; i < 3; ++i) 
            {
                tableSizes[3] = 0xC0;
            }
        }

        for(phase = 0; phase < 3; ++i)
        {
            for(area = 0; area < tableSizes[phase]; ++area)
            {
                // only going to operate on canonical area numbers
                // to avoid duplication of data loading
                if(ResolveArea(area) != area)
                    continue;

                a                       = areas[area];

                list                    = &a->spr[phase];

                a->fallingRocks[phase]  = false;

                a->spr[phase]           = NULL;

                // dark world and extended areas in the "beginning" part don't have sprites in the original rom.
                if( (usingOld) && (phase == 0) && (area >= 0x40) )
                    continue;

                offset = CpuToRomAddr(tableOffset[phase]);

                // the new mechanism and the old one are different in that the old mechanism uses 2-byte pointers in the same bank as the table.
                offset = (usingOld) ? CpuToRomAddr( (0x090000) | Get2Bytes(rom, offset + (area * 2) ) ) : 
                                      CpuToRomAddr( Get3Bytes(rom, offset + (area * 3) ) );
               
                for(i = offset; (i & 0xFFFF) < 0xFFFD; i += 3)
                {
                    y   = GetByte(rom, i);

                    if(y == 0xFF)
                       break;

                    x   = GetByte(rom, i + 1);
                    id  = GetByte(rom, i + 2);

                    // falling rocks don't really count as a sprite so we skip it
                    if(id == 0xF4)
                    {
                        a->fallingRocks[phase] = true;
                        continue;
                    }
                    else if(id >= 0xF5)
                    {
                        // overworld overlords are not actually implemented yet
                        overlord = true;
                        id -= 0xF5;
                    }

                    OverSpr::Add(list, new OverSpr(id, x << 4, y << 4) ); 
                }

            }

            for(area = tableSizes[i]; area < 0xC0; ++area)
            {
                if(ResolveArea(area) != area)
                    continue;

                areas[area]->spr[phase] = NULL;
            }
        }

	    return true;
    }

// ===============================================================

    bool OverData::LoadAllItemData()
    {
        u32 i       = 0;
        u32 area    = 0;
        u32 x       = 0, y = 0;
        u32 map16   = 0;
        u32 offset  = 0;
        u32 itemNum = 0;

        OverArea *a = NULL;

        // ---------------------------
        
        for(area = 0; area < 0xC0; ++area)
        {
            // There's no item data in the extended areas
            // Though we might put some there eventually
            if(area >= 0x80)
                continue;

            // don't operate on unauthentic area numbers.
            if(ResolveArea(area) != area)
                continue;

            a = areas[area];

            if(header->overItemOffset == 0x1BC2F9)
            {
                // older method
                offset = CpuToRomAddr(header->overItemOffset + (i * 2) );
                offset = CpuToRomAddr(Get2Bytes(rom, offset) | (0x1B << 16) );
            }
            else
            {
                // newer method
                offset = CpuToRomAddr(header->overItemOffset + (i * 3) );
                offset = CpuToRomAddr(Get3Bytes(rom, offset));
            }
    
            for(i = 0;  ; i += 3)
            {
                map16  = Get2Bytes(rom, offset + i);

                if(map16 == 0xFFFF)
                    break;
                
                x = GetMap16X(map16);
                y = GetMap16Y(map16);

                itemNum = GetByte(rom, offset + i + 2);
                
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

                // In the original game, changing the items available in each phase
                // I'm toying with the idea of doing that, though.
                // For now all items reside in phase 0.
                OverItem::Add( &a->items[0], new OverItem(itemNum, x, y));
            }
        }
       
        return true;
    }

// ===============================================================

    void OverData::LoadOverlaysOld()
    {
        bool done  = false;
        bool write = false;
        
        u8 opcode = 0;

        POINT min = {0,    0};
        POINT max = {0x3F, 0x3F};
        
        u32 i;
        u32 map16Val    = 0;
        u32 map16Addr   = 0;
        u32 offset      = 0;
        u32 x_reg       = 0;
       
        Map16Buf *lay   = NULL;
       
        // -----------------------

        for(i = 0; i < 0x80; ++i) 
        {
            done   = false;

            /// make sure to intialize to all 0xFF bytes
            lay    = areas[i]->eOverlay;

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
                        else
                            map16Addr &= 0xFFFF;
                        
				        offset += 3;
                        write = true;
                        break;
			        
                    // STA long indexed instruction.
                    case 0x9f:
                        map16Addr = Get3Bytes(rom, offset) + x_reg;

                        // bad bank, must be 0x7E!
                        if(GetBank(map16Addr) != 0x7E)
                            done = true;
                        else
                            map16Addr &= 0xFFFF;
                            
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
                        
                        IncMapCounts(map16Val, false);
                    }
                }
		    }
        }
    }

// ===============================================================
