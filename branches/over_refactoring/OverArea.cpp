
	#include "Globals.h"
    #include "OverData.h"
    #include "Core.h"       // only used so we can have access to graphics.cpp functions

// ===============================================================

    OverArea::OverArea(u8 area, bufPtr rom)
    {
        this->rom     = rom;
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
        {
            map16       = new Map16Buf(0x40, 0x40);
            map16Backup = new Map16Buf(0x40, 0x40);
        }
        else
        {
            map16       = new Map16Buf(0x20, 0x20);
            map16Backup = new Map16Buf(0x20, 0x20);
        }

        /// not sure if I'm going to need a flags buffer for each area
        map16Flags      = CreateBuffer(0x10000 / 8);

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
    }

// ===============================================================

    OverArea::~OverArea()
    {



    }

// ===============================================================

    bufPtr OverData::LoadMap16(u32 index, map16Pos p)
    {   
        /**

        u32 map32Val = 0;
        u16 map16Vals[4];

        u32 i = 0;
        u32 j = 0;
        u32 k = 0;

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

        */

        return NULL;

    }

// ===============================================================

    bool OverArea::LoadOverlay()
    {
        u32 i        = 0;
        u32 j        = 0;
        u32 map16Val = 0;

        // -----------------------

        // copy map16 data to a backup buffer
        (*map16Backup) = (*map16);

        for(i = 0; i < map16->width; ++i)
        { 
            for(j = 0; j < map16->height; ++j)
            {
                map16Val = eOverlay->GetTile(i, j);

                if(map16Val == 0xFFFF)
                    continue;

                map16->SetTile(i, j, map16Val);
            }
        }

        /// LoadMap8();

        return true;
    }

// ===============================================================

    bool OverArea::UnloadOverlay()
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
                newMap16 = map16->GetTile(i, j);
                oldMap16 = map16Backup->GetTile(i, j);

                if(newMap16 == oldMap16)
                    eOverlay->SetTile(0xFFFF, i, j);
                else
                    eOverlay->SetTile(newMap16, i, j);
            }
        }
        
        (*map16) = (*map16Backup);

        /// LoadMap8();

        return true;
    }

// ===============================================================




