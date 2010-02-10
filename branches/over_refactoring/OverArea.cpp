
	#include "Globals.h"
    #include "OverData.h"
    #include "Core.h" // only used so we can have access to graphics.cpp functions

// ===============================================================

    OverArea::OverArea(u8 area, bufPtr rom)
    {
        this->rom = rom;
        this->areaNum = area;

        if(areaNum < 0x80)
        {
            // Check whether we're in the dark world.
            darkWorld = (areaNum & 0x40) ? true : false;
        
            // Check whether the map is 1024x1024 (loads 4 areas) or 512x512 (loads 1 area)
            largeArea = GetByte(rom, 0x12844 + (areaNum & 0x3F) ) != 0 ? true : false;

            if(largeArea)
            {
                // Maps 512x512 area number to the larger 1024x1024 area number that it belongs to.
                area = GetByte(rom, 0x125EC + (areaNum & 0x3F)) + (darkWorld ? 0x40 : 0x00);

                // Loads all four of the 512x512 submaps that make up the large area.
                LoadMap16(area + 0, upper_left);
                LoadMap16(area + 1, upper_right);
                LoadMap16(area + 8, lower_left);
                LoadMap16(area + 9, lower_right);
            }
            else
                LoadMap16(area, upper_left);
        }

        if(largeArea)
            map16Buf = CreateBuffer(0x40, 0x40, 2);
        else(largeArea)
            map16Buf = CreateBuffer(0x20, 0x20, 2);

        eOverlay = new EventOverlay();

        /*
        if(this->editOverlay == true)
        {
            // 

            LoadOverlay();
        }
        else
        {
            // refresh the tiles in the map8 buffer
            LoadMap8();
        }
        */

        
        return true;
    }

// ===============================================================

    OverArea::~OverArea()
    {



    }

// ===============================================================

    bufPtr OverArea::LoadMap32()
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
        offset   = Get3Bytes(this->rom, tableLoc + (areaNum * 3));
        data1    = DecompressBank02(this->rom, CpuToRomAddr(offset) );

        tableLoc = CpuToRomAddr(Get3Bytes(this->rom, asm_overmap_ptrs2));
        offset   = Get3Bytes(this->rom, tableLoc + (areaNum * 3));
        data2    = DecompressBank02(this->rom, CpuToRomAddr(offset) );

        for(i = 0; i < 0x100; ++i)
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

    bool OverData::LoadOverlay()
    {
        u32 i        = 0;
        u32 j        = 0;
        u32 map16Val = 0;

        // -----------------------

        CopyBuffer(map16Backup, map16Buf, 0, 0, map16Buf->length);

        for(i = 0; i < 0x40; ++i)
        { 
            for(j = 0; j < 0x40; ++j)
            {
                map16Val = eOverlay->GetTile(i, j);

                if(map16Val == 0xFFFF)
                    continue;

                SetMap16Tile(map16Buf, map16Val, i, j);
            }
        }

        LoadMap8();

        return true;
    }

// ===============================================================

    bool OverData::UnloadOverlay()
    {
        u32 i        = 0;
        u32 j        = 0;
        u32 newMap16 = 0;
        u32 oldMap16 = 0;

        // -----------------------
        
        // figure out what the differences are between the map
        // with the overlay enabled and the overlay not enabled.
        for(i = 0; i < 0x40; ++i)
        { 
            for(j = 0; j < 0x40; ++j)
            {
                newMap16 = GetMap16Tile(map16Buf, i, j);
                oldMap16 = GetMap16Tile(map16Backup, i, j);

                if(newMap16 == oldMap16)
                    eOverlay->SetTile(0xFFFF, i, j);
                else
                    eOverlay->SetTile(newMap16, i, j);
            }
        }
        
        // restore the map16 buffer to the way it was before the overlay was activated
        CopyBuffer(map16Buf, map16Backup, 0, 0, map16Backup->length);

        LoadMap8();

        return true;
    }

// ===============================================================




