

	#include "Globals.h"
	#include "Core.h"
    #include "OverData.h"
	#include "OverMode.h"
	#include "OverModeGui.h"
	#include "Sprites.h"
    #include "PathName.h"

    #include "compresch_lttp.h"
    
    #include "direct.h"

// **************************************************

    u16 GetMap16X(u16 addr)
    {
        return (addr & 0x007E) << 3;
    }

    u16 GetMap16Y(u16 addr)
    {
        return (addr & 0x1F80) >> 3;
    }

    u16 xyToMap16(u16 x, u16 y)
    {
        return ( ((x >> 3) & 0x007E) | ((y << 3) & 0x1F80) );
    }

// ***********************************************

    void AniDecompressOverworld(zgPtr game)
    {
        // Simulates $00D394 in the rom
        u32 index = 0,   source = 0;
        u32 targPtr = 0, srcPtr = 0;
        
        bufPtr rom = game->image;
        bufPtr ani = game->aniVram;
        bufPtr gfx = NULL;
        bufPtr gfx2 = NULL;

        // ------------------------------

        switch(game->overData->area->areaNum)
        {
        case 0x03: case 0x05: case 0x07:
        case 0x43: case 0x45: case 0x47:

            source = 0x58;

            break;

        default:

            source = 0x5A;

            break;
        }

        targPtr = 0x1680; 
        srcPtr  = 0;

        gfx = game->bgPacks[source];
        
        Do3To4Low(ani, gfx, &targPtr, &srcPtr, 0x40);

        targPtr = 0x1E80; 
        srcPtr  = 0;

        gfx2 = game->bgPacks[source + 1];

        Do3To4Low(ani, gfx2, &targPtr, &srcPtr, 0x20);

        ToFile(ani, "C:\\aniTestOver.smc");
    }

// **************************************************

    void LoadOverPalettes(zgPtr game)
    {
        u32 i = 0;

        u32 a = game->overData->area;
        u32 x = 0;
        u32 offset = 0;

        // ------------------------------------

        if(a > 0x7F)
            a = 0x01;

        // corresponds to $DEC9E
        if(a & 0x40)
            x = 2;

        // writes SP1 through SP4 (full)
        offset = GetByte(game->image, CpuToRomAddr(0x1BEC73 + x));
        offset = CpuToRomAddr(0x1B0000 + offset + 0xD218);
        LoadPalettes(game, offset, 4, 0x0F, SP1 + 2);

        // corresponds to $DED6E
        // uses different palettes based on whether in the DW or LW
        x = a & 0x40 ? 0x09 : 0x07;
        offset = GetByte(game->image, CpuToRomAddr(0x1BEBC6 + x));
        offset = CpuToRomAddr(0x1B0000 + offset + 0xD446);

        // Set SP0 (second half)
        LoadPalettes(game, offset, 1, 7, SP0 + 0x12);

        // Set SP6 (second half) (why the same palette... I'm not sure)
        LoadPalettes(game, offset, 1, 7, SP6 + 0x12);

        // corresponds to $DECC5
        // Set SP5 (first half) 
        x = game->gi->spritePal1 << 1;
        offset = GetByte(game->image, CpuToRomAddr(0x1BEBD6 + x));
        offset = CpuToRomAddr(0x1B0000 + offset + 0xD4E0);
        LoadPalettes(game, offset, 1, 7, SP5 + 2);

        // corresponds to $DECE4
        // Set SP6 (first half) 
        x = game->gi->spritePal2 << 1;
        offset = GetByte(game->image, CpuToRomAddr(0x1BEBD6 + x));
        offset = CpuToRomAddr(0x1B0000 + offset + 0xD4E0);
        LoadPalettes(game, offset, 1, 7, SP6 + 2);

        // corresponds to $DECE4 (loads sword palette)
        // Set SP5 (second half), 1st three colors
        x = 0;
        offset = GetByte(game->image, CpuToRomAddr(0x1BEBB4 + x));
        offset = CpuToRomAddr(0x1B0000 + offset + 0xD360);
        LoadPalettes(game, offset, 1, 3, SP5 + 0x12);

        // corresponds to $DECE4 (loads shield palette)
        // Set SP5 (second half), last 4 colors
        x = 0;
        offset = GetByte(game->image, CpuToRomAddr(0x1BEBC1 + x));
        offset = CpuToRomAddr(0x1B0000 + offset + 0xD648);
        LoadPalettes(game, offset, 1, 4, SP5 + 0x18);

        // corresponds to $DEDF9 (loads armor palette)
        x = 0;
        offset = GetByte(game->image, CpuToRomAddr(0x1BE606 + x));
        offset = CpuToRomAddr(0x1B0000 + offset + 0xD308);
        LoadPalettes(game, offset, 1, 14, SP7 + 2);

        // corresponds to $DEC77
        // Set SP0 (first half)
        x = a & 0x40 ? 0x03 : 0x01; // depends on whether in dark world or not
        offset = GetByte(game->image, CpuToRomAddr(0x1BEBC6 + x));
        offset = CpuToRomAddr(0x1B0000 + offset + 0xD39E);
        LoadPalettes(game, offset, 1, 16, SP0 + 2);  
 
        // corresponds to $DEE52
        // Set BP0 through BP1 (full)
        x = 0; // unfamiliar setting (based off of $0AB2 on the overworld)
        offset = GetByte(game->image, CpuToRomAddr(0x1BEC47 + x));
        offset = CpuToRomAddr(0x1B0000 + offset + 0xD660);
        LoadPalettes(game, offset, 2, 16, BP0);

        // corresponds to $DEEC7, Sets BP2 through BP6 (first halves)
        // depends on $0AB3 on the overworld (which depends upon the area number
        switch(a)
        {
            case 0x03: case 0x05: case 0x07:
                x = 0x02; break;

            case 0x43: case 0x45: case 0x47:
                x = 0x03; break;

            default:
                x = (a & 0x40) ? 1 : 0; break;
        }


        offset = Get2Bytes(game->image, CpuToRomAddr(0x1BEC3B + (x << 1)));
        offset = CpuToRomAddr(0x1B0000 + offset + 0xE6C8);
        LoadPalettes(game, offset, 5, 7, BP2 + 0x02);

        // corresponds to $DEEE8
        // Set BP2 through BP4 (second halves)
        // depends on $0AB4
        x = GetByte(game->image, 0x007D1C + a);
        x = GetByte(game->image, 0x75504 + (x << 2)) << 1;

        if(x & 0x80)
        {
            ;
        }
        {
            offset = Get2Bytes(game->image, CpuToRomAddr(0x1BEC13 + x));
            offset = CpuToRomAddr(0x1B0000 + offset + 0xE86C);
            LoadPalettes(game, offset, 3, 7, BP2 + 0x12);
        }

        // corresponds to $DEF0C
        // Set BP5 through BP7 (second halves)
        // depends on $0AB5
        x = GetByte(game->image, 0x007D1C + a);
        x = GetByte(game->image, 0x75505 + (x << 2)) << 1;

        if(x & 0x80)
        {
            ;
        }
        else
        {
            offset = Get2Bytes(game->image, CpuToRomAddr(0x1BEC13 + x));
            offset = CpuToRomAddr(0x1B0000 + offset + 0xE86C);
            LoadPalettes(game, offset, 3, 7, BP5 + 0x12);
        }
    
        // corresponds to $DEEA8, sets BP7 (first half), depends on $0AB8
        x = GetByte(game->image, 0x007D1C + a);
        x = GetByte(game->image, 0x75506 + (x << 2));

        if(x & 0x80)
        {
            ;
        }
        else
        {
            offset = GetByte(game->image, CpuToRomAddr(0x1BEBC6 + x));
            offset = CpuToRomAddr(0x1B0000 + offset + 0xE604);
            LoadPalettes(game, offset, 1, 7, BP7 + 0x02);
        }

        // at this point in the game's code it will do something pretty redundant so I'm omitting it

        // set the background color (this is the default color at the back of the screen)

        switch(a)
        {
            case 0x03: case 0x05: case 0x07:

                Put2Bytes(game->cgram, 0, 0x2669);

                break;

            case 0x43: case 0x45: case 0x47:

                Put2Bytes(game->cgram, 0, 0x2A32);

                break;

            default:
                if(a == 0x83)
                    Put2Bytes(game->cgram, 0, 0x19C6);
                else if(a == 0x82)
                    Put2Bytes(game->cgram, 0, 0x19C6);
                else if(a == 0x80)
                    Put2Bytes(game->cgram, 0, 0x19C6);
                else if(a == 0x81)
                    Put2Bytes(game->cgram, 0, 0x2669);
                else if(a > 0x80)
                    Put2Bytes(game->cgram, 0, 0x2669);
                else if(a >= 0x40)
                    Put2Bytes(game->cgram, 0, 0x2A32);
                else
                    Put2Bytes(game->cgram, 0, 0x2669);

                break;
        }
    }

    BOOL GetChildRect(HWND child, RECT *rChild)
    {
        // retrieves a child window's coordinates relative to its parent's client area

        POINT p;

        RECT rParent;

        if(!GetClientRect(child, rChild))
            return FALSE;

        if(!GetClientRect(GetParent(child), &rParent))
            return FALSE;

        p.x = rChild->left; p.y = rChild->top;
        ClientToScreen(child, &p);
        rChild->left = p.x; rChild->top = p.y;

        p.x = rParent.left; p.y = rParent.top;
        ClientToScreen(child, &p);
        rParent.left = p.x; rParent.top = p.y;

        p.x = rChild->right; p.y = rChild->bottom;
        ClientToScreen(child, &p);
        rChild->right = p.x; rChild->bottom = p.y;

        p.x = rParent.right; p.y = rParent.bottom;
        ClientToScreen(child, &p);
        rParent.right = p.x; rParent.bottom = p.y;

        rChild->left    -= rParent.left;
        rChild->top     -= rParent.top;
        rChild->right   -= rParent.right;
        rChild->bottom  -= rParent.bottom;

        return TRUE;
    }

// ****************************************

    void InitOverMode(zgPtr game)
    {
        u16 map32Val = 0;
        u16 map16Val = 0;

        u32 area = game->overData->area;
        u32 i = 0, j = 0;
    
        giPtr gi = game->gi;
    
        bufPtr map32Buf = NULL;

        ManagedBitmap* b = NULL;

        OverData *o = game->overData;

        RECT r;

        // ---------------------------------

        game->currentOverArea = area;

        o->LoadArea();

        if(area & 0x40)
        {
            gi->backGfx0 = 0x21;
            gi->spriteGfx1 = 0x0B;
        }
        else
        {
            gi->backGfx0 = 0x20;
            gi->spriteGfx1 = 0x01;
        }

        // Load up graphics info
        gi->backGfx1   = GetByte(game->image, CpuToRomAddr(0x00FC9C) + area);
        gi->spriteGfx0 = GetByte(game->image, CpuToRomAddr(0x00FA41) + area);
    
        gi->backPal0   = 0;
        gi->spritePal0 = 0;
        gi->spritePal1 = 0;
        gi->spritePal2 = 0;

        // Now we need to load all the necessary palettes
        LoadOverPalettes(game);

        // decompress graphics and put them into a usable format that the GUI front end can use.
        AniDecompressOverworld(game);
        OverDecompress(game);

        DecodePalettes(game);
        DecodeTiles(game);
        DecodeOAMTiles(game);

        // Blank out the screen
        if(game->pictWin)
        {
            WEStruct *wes     = &game->currentWES;
            WindowElement *we = &wes->WESet[0];

            b = (ManagedBitmap*) GetWindowLong(game->pictWin, 0);

            for(i = 0; i < b->pixels->length; i += 4)
            {
                Put4Bytes(b->pixels, i, game->Pals[0][0]);
            }

            GetWindowRect(game->pictWin, &r);

            // locate which window handle
            for(i = 0; ((HWND) we->handle != game->pictWin) && i < wes->numElements; ++i, we = &wes->WESet[i])
                continue;

            if(i != wes->numElements)
            {
                if(o->largeArea)
                    we->nHeight = we->nWidth = 1024;
                else
                    we->nHeight = we->nWidth = 512;

                MoveWindow(game->pictWin, we->x, we->y, we->nWidth, we->nHeight, TRUE);
            }
        }

        // for debugging purposes
        ToFile(game->vram,  "C:\\Dark_Prophecy\\testVram.bin");
        ToFile(o->map16Buf, "C:\\Dark_Prophecy\\testMap16.bin");
        ToFile(game->cgram, "C:\\Dark_Prophecy\\testPal.bin");

        DrawOW(game, b, o->map8Buf);

        return;
    }

// ===============================================================

    void OverDecompress(zgPtr game)
    {
        // this routine corresponds to $00:E19B in the rom

        u32 i = 0, j = 0;
        u32 index = 0, index2 = 0, index3 = 0, index4 = 0, dummyInt = 0, source = 0; 

        bufPtr decomp = NULL;
        giPtr gi = game->gi;

        // -----------------------------------------------

        // Find the pointer to the graphics to decompress
        // should optionally use this or 0x01 or 0x0A depending on whether you're in the dark world or not.
        
        // Decompress them using a special method
        index  = 0x8800;
        index4 = 0;

        Do3To4High(game->vram, game->sprPacks[gi->spriteGfx1], &index, &index4, 0x600);

        // --------------------------------------------------------

        // 0x06 is always used for this graphics slot
        index4 = 0;
    
        Do3To4Low(game->vram, game->sprPacks[0x06], &index, &index4, 0x80);
 
        // --------------------------------------------------------

        // Now we move on to decompressing and storing the 4 graphics subsets
        // that are dependent upon the room you are in.

        // These explicit offsets really ought to be generalized
        index3 = gi->spriteGfx0;
        index3 *= 4;
        index3 += 0x5B57;

        for(i = 0; i < 4; ++i, ++index3)
        {
            // Get one of the current 4 graphics set numbers
            index2 = GetByte(game->image, index3);
            index4 = 0;

            decomp = game->sprPacks[index2];
    
            switch(index2)
            {
            case 0x00:

                for(j = 0; j < 0x800; j++, index++)
                    PutByte(game->vram, index, 0);

                break;

            case 0x52:
            case 0x53:
            case 0x5A:
            case 0x5B:
            case 0x5C:
            case 0x5E:
            case 0x5F:

                Do3To4High(game->vram, decomp, &index, &index4, 0x600);

                break;

            default:
            
                Do3To4Low(game->vram, decomp, &index, &index4, 0x40);

                break;
            }
        }

        // Next decompress the BG graphic data
        // Target in VRAM is 0x4000

        index = 0x4000;

        // The primary BG tileset (corresponds to $0AA1)
        index3 = gi->backGfx0;
        index3 *= 8;
        index3 += 0x6073;

        // (corresponds to $0AA2)
        index2 = gi->backGfx1;
        index2 *= 4;
        index2 += 0x5D97;

        for(i = 0; i < 8; ++i, ++index3)
        {
            j = 7 - i;

            source = GetByte(game->image, index3);     

            decomp = game->bgPacks[source];

            // these graphics can override the graphics specified by backGfx0, if the tileset number is nonzero.
            index4 = GetByte(game->image, index2 + i - 3);

            if( (j >= 1) && (j <= 4) && (index4 != 0) )
                decomp = game->bgPacks[index4];

            index4 = 0;

            // depending on the value of $0AA1 we use different 3bpp to 4bpp methods
            if( (gi->backGfx0 < 0x20) && (j < 3))
                Do3To4Low(game->vram, decomp, &index, &index4, 0x40);
            else if(gi->backGfx0 < 0x20)
                Do3To4High(game->vram, decomp, &index, &index4, 0x600);
            else 
            {
                switch(j)
                {
                case 2:
                case 3:
                case 4:
                case 7:
                    Do3To4High(game->vram, decomp, &index, &index4, 0x600);
                    break;

                default:
                    Do3To4Low(game->vram, decomp, &index, &index4, 0x40);
                    break;
                }
            }
        }

        return;
    }

// ===============================================================

    void DrawOW(zgPtr game, ManagedBitmap* b, bufPtr map8Buf, bool justAnimated)
    {
        u32 x       = 0, y = 0;
        u32 entry   = 0;
        u32 chr     = 0;
 
        for(x = 0; x < 0x80; ++x)
        {
            for(y = 0; y < 0x80; ++y)
            {
                entry = Get2Bytes(map8Buf, x, y);
                chr   = entry & 0x3FF;

                // bounds check so we only redraw tiles that are meant to be animated.
                if(!justAnimated)
                    DrawMap8(game, b, entry, x, y);
                else if( (chr >= 0x1C0) && (chr < 0x1E0))
                    DrawMap8(game, b, entry, x, y);
            }
        }
    }

// ===============================================================
    
    void DrawSprite(zgPtr game, ManagedBitmap *b, u16 spriteVal, u32 x, u32 y, u8 palette, u8 large, bool hFlip, bool vFlip)
    {
        x *= 4;
        y = b->bottom - (y * b->stride);

        if(large)
        {
            BlitTile(b, x        + y,             &game->OAMTiles[spriteVal          & 0x1FF], &game->Pals[palette], hFlip, vFlip);
            BlitTile(b, x + 0x20 + y,             &game->OAMTiles[(spriteVal + 1)    & 0x1FF], &game->Pals[palette], hFlip, vFlip);
            BlitTile(b, x        + y - b->stride, &game->OAMTiles[(spriteVal + 0x10) & 0x1FF], &game->Pals[palette], hFlip, vFlip);
            BlitTile(b, x + 0x20 + y - b->stride, &game->OAMTiles[(spriteVal + 0x11) & 0x1FF], &game->Pals[palette], hFlip, vFlip);
        }
        else
            BlitTile(b, x + y, &game->OAMTiles[spriteVal & 0xFF], &game->Pals[palette], hFlip, vFlip);
    }

// ===============================================================

    void DrawMap16(zgPtr game, ManagedBitmap *b, u16 map16Val, u32 x, u32 y)
    {
        u16 map8Vals[4];

        // ----------------------------------

        // acquire the tilemap entry values needed
        game->overData->Map16To8(map16Val, map8Vals);

        x <<= 1; 
        y <<= 1;

        DrawMap8(game, b, map8Vals[0], x    , y);
        DrawMap8(game, b, map8Vals[1], x + 1, y);
        DrawMap8(game, b, map8Vals[2], x,     y + 1);
        DrawMap8(game, b, map8Vals[3], x + 1, y + 1);

        return;
    }

// ===============================================================

    void DrawMap8(zgPtr game, ManagedBitmap *b, u32 entry, u32 x, u32 y)
    {
        u32 palette = (entry & 0x1C00) >> 0x0A;
        u32 chr     = (entry & 0x3FF);
        u32 hFlip   = (entry & 0x4000) ? true : false;
        u32 vFlip   = (entry & 0x8000) ? true : false;  

        // same as x *= 0x20
        x <<= 5;

        // same y *= 8
        y <<= 3;

        y = b->bottom - (y * b->stride);

        EraseTile(b,
                  x + y,
                  game->Pals[0][0]);

        BlitTile(b,
                 x + y,
                 &game->BGTiles[chr],
                 &game->Pals[palette],
                 hFlip ? true : false,
                 vFlip ? true : false);

        return;
    }

// ===============================================================

    u32 AllocateMap16(bufPtr counts, u16 map8Vals[4], u16 oldMap16, u16 threshold)
    {
        u32 a = 0;
        u32 i = 0;

        for(i = threshold; i < 0x4000; i++)
        {
            // Get the number of times this particular map16 value occurs in the rom.
            a = Get4Bytes(counts, i * 4);

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

    u32 AllocateMap32(bufPtr counts, u16 map16Vals[4], u16 oldMap32, u16 threshold)
    {
        u32 a = 0;
        u32 i = 0;

        for(i = threshold; i < 0x4000; i++)
        {
            a = Get4Bytes(counts, i * 4);

            if(a > 1)
                continue;
            else if(a == 1)
            {
                // in this case the new map16 tile would replace the old
                if( (u16) i == oldMap32 )
                    return i;

                continue;
            }
            else
                return i;
        }

        return -1;
    }

// ===============================================================

    u16 GetMap32Tile(bufPtr map32Buf, u16 x, u16 y)
    {
        return Get2Bytes(map32Buf, (x * 2) + (y * 0x20));
    }

// ===============================================================

    u16 GetMap8Tile(bufPtr map8Buf, u16 x, u16 y)
    {
        return Get2Bytes(map8Buf, (x * 2) + (y * 0x100) );
    }

// ===============================================================

    u32 SetMap32Tile(bufPtr map32Buf, u16 value, u16 x, u16 y)
    {
        return Put2Bytes(map32Buf, (x * 2) + (y * 0x20), value);
    }

// ===============================================================

    u32 SetMap8Tile(bufPtr map8Buf, u16 value, u16 x, u16 y)
    {
        return Put2Bytes(map8Buf, (x * 2) + (y * 0x100), value);
    }

// ===============================================================

    u32 bound(u32 val, u32 lower, u32 higher)
    {   
        if( (signed) val > (signed) higher)
            val = (signed) higher;
            
        if( (signed) val < (signed) lower)
            val = (signed) lower;

        return val; 
    }

// ===============================================================
    
    void GetMap2x2(u16 *square, bufPtr source, u32 x, u32 y)
    {
        square[0] = Get2Bytes(source, x,     y    );
        square[1] = Get2Bytes(source, x + 1, y    );
        square[2] = Get2Bytes(source, x    , y + 1);
        square[3] = Get2Bytes(source, x + 1, y + 1);
    }   
    
// ===============================================================
    
    int CompareMap2x2(u16 *square1, u16 *square2)
    {
        return ( (square1[0] == square2[0]) && (square1[1] == square2[1]) && (square1[2] == square2[2]) && (square1[3] == square2[3]) );
    }    

// ===============================================================
    
    u32 MatchMap2x2(bufPtr smallMap, bufPtr largeMap, u16 *square, u32 x, u32 y)
    {
        u16 mapVals[4];
        u32 i = 0;
        u32 j = 0;
        
        for(j = 0; j < smallMap->height; j += 2)
        {
            for(i = 0; i < smallMap->width; i += 2)
            {
                if( (i == x) && (j == y) )
                    return -1;

                GetMap2x2(mapVals, smallMap, i, j);
                
                if(CompareMap2x2(square, mapVals))
                    return Get2Bytes(largeMap, i >> 1, j >> 1);
            }
        }
        
        return -1;
    } 

// ************************************************************
    
    bool PlaceObject(zgPtr game, OverData *o, OverObj *obj)
    {
        u32 i = 0, j = 0;
        u32 value = 0;

        bool map16Failure = false;

        u8  area        = o->area;

        u16 oldMap16    = 0;
        u16 map8Vals[4];
        u16 map16Vals[4];

        u32 threshold = 0;
        
        u32 x8  = obj->x >> 3;
        u32 y8  = obj->y >> 3;

        u32 x16 = 0;
        u32 y16 = 0;

        u32 xBase = 0;
        u32 yBase = 0;
        
        u32 dx8 = obj->width  / obj->mapType;
        u32 dy8 = obj->height / obj->mapType;
          
        u32 x16Min = bound(x8 >> 1, 0, 0x3F);
        u32 x16Max = bound( (x8 + dx8 - 1) >> 1, 0, 0x3F);
        
        u32 y16Min = bound(y8 >> 1, 0, 0x3F);
        u32 y16Max = bound( (y8 + dy8 - 1) >> 1, 0, 0x3F);

        u32 dx16   = (x16Max - x16Min + 1);
        u32 dy16   = (y16Max - y16Min + 1);

        bufPtr mapData   = NULL;
        
        bufPtr tempMap16 = NULL;
        bufPtr tempMap8  = NULL;

        bufPtr saveMap16 = NULL;
    
        ManagedBitmap *b = (ManagedBitmap*) GetWindowLong(game->pictWin, 0);


        FILE *errFile    = fopen("C:\\mapError.log", "wt");

        // ---------------------

        switch(obj->mapType)
        {
            case fromMap8:
            {
                tempMap16 = CreateBuffer(dx16, dy16, 2);
                tempMap8  = CreateBuffer(dx16 << 1, dy16 << 1, 2);
                mapData   = FromString( (char*) obj->mapData, dx8, dy8, 4);

                // copy the contents of a subsection of the map8 buffer to a temporary array
                for(j = 0; j < tempMap8->height; ++j)
                {
                    for(i = 0; i < tempMap8->width; ++i)
                    {
                        value = Get2Bytes(o->map8Buf, (x32Min << 2) + i, (y32Min << 2) + j);
                        Put2Bytes(tempMap8, i, j, value);
                    }
                }

                fprintf(errFile, "Old Map16:\n");

                // do the same for the map16 tiles
                for(j = 0; j < tempMap16->height; ++j)
                {
                    for(i = 0; i < tempMap16->width; ++i)
                    {
                        value = Get2Bytes(o->map16Buf, (x32Min << 1) + i, (y32Min << 1) + j);
                        Put2Bytes(tempMap16, i, j, value);

                        fprintf(errFile, "%04X\t", value);

                        if(Get4Bytes(o->map16Counts, value * 4) == 0)
                            i = i;
                    }

                    fprintf(errFile, "\n");
                }
                
                saveMap16 = DuplicateBuffer(tempMap16);
                
                // now write in the map8 parts of the object to the tempMap8 buffer
                for(j = 0; j < dy8; ++j)
                {
                    for(i = 0; i < dx8; ++i)
                    {
                        value = Get4Bytes(mapData, i, j);
                        
                        if(value != -1)
                            Put2Bytes(tempMap8, 
                                      (x8 - (x32Min << 2)) + i,
                                      (y8 - (y32Min << 2)) + j,
                                      value);
                    }
                }   
                
                fprintf(errFile, "\n Decreased Map16 counts:\n");

                // temporarily decrease the map counts of all the original map16 values involved
                for(j = 0; j < saveMap16->height; ++j)
                {
                    for(i = 0; i < saveMap16->width; ++i)
                    {
                        value = Get2Bytes(saveMap16, i, j);
                        o->DecMapCounts(value);

                        fprintf(errFile, "%04X\t", value);
                    }

                    fprintf(errFile, "\n");
                }
                
                // going to do a check of all the map8 clusters to see if we are able to allocate all the map16
                // tiles we need
                for(j = 0; j < dy32 << 2; j += 2)
                {          
                    for(i = 0; i < dx32 << 2; i += 2)
                    {
                        // get the old map16 value
                        oldMap16 = Get2Bytes(tempMap16, i >> 1, j >> 1);
                        
                        // fill a 2x2 array with the corresponding map8 values.
                        GetMap2x2(map8Vals, tempMap8, i, j);
                        
                        // determine if the same combination of map8 tiles appears earlier in the object buffer
                        // if so, we'll just use that map16 value as it's already been determined.
                        value = MatchMap2x2(tempMap8, tempMap16, map8Vals, i, j);
                        
                        if(value == -1)
                        {
                            // the map8 combination did not appear in the area the object affects, so resort to
                            // finding the proper map16 value for them in the buffer at large.
                            value = o->FindMap16(map8Vals);
                                                                        
                            if(value == -1)
                            {        
                                // map16 value was not found for that map8 combination, so we'll have to find a new one.
                                value = o->AllocateMap16(map8Vals, oldMap16, threshold);

                                if(value == -1)
                                    map16Failure = true;
                                else
                                    threshold = value + 1;
                            }
                        }

                        Put2Bytes(tempMap16, i >> 1, j >> 1, value);
                    }
                }

                if(!map16Failure)
                {
                    threshold = 0;
                     
                    for(i = 0; i < (dx32 << 2); i += 2)
                    {
                        for(j = 0; j < (dy32 << 2); j += 2)
                        {
                            value = Get2Bytes(tempMap16, i >> 1, j >> 1);
                            GetMap2x2(map8Vals, tempMap8, i, j);
                            o->IncMapCounts(value, map8Vals);
                        }
                    }

                    for(i = 0; i < dx8; ++i)
                    {
                        for(j = 0; j < dy8; ++j)
                        {
                            value = Get2Bytes(mapData, i, j);
                            Put2Bytes(o->map8Buf, i + x8, j + y8, value);
                            DrawMap8(game, b, value, i + x8, j + y8);
                            InvalidateTile(game, i + x8, j + y8);
                        }
                    }

                    for(i = 0; i < tempMap16->width; ++i)
                    {
                        for(j = 0; j < tempMap16->height; ++j)
                        {
                            value = Get2Bytes(tempMap16, i, j);
                            Put2Bytes(o->map16Buf, i + (x32Min << 1), j + (y32Min << 1), value);
                        }
                    }
                }
                
                if(map16Failure)
                {
                    // increase the counts of the map16 values we subtracted off earlier.
                    // because the map16 allocation failed, we just revert to the old state.
                    for(i = 0; i < saveMap16->width; ++i)
                    {
                        for(j = 0; j < saveMap16->height; ++j)
                        {
                            value = Get2Bytes(saveMap16, i, j);
                            o->IncMapCounts(value);
                        }
                    }

                }

                // clean up and get rid of all the memory we allocated
                DeallocBuffer(tempMap8);
                DeallocBuffer(tempMap16);
                DeallocBuffer(saveMap16);

                DeallocBuffer(mapData);
                    
                fclose(errFile);

                break;
            }

            case fromMap16:
            {
                x16     = obj->x >> 4;
                y16     = obj->y >> 4;

                dx16    = obj->width / obj->mapType;
                dy16    = obj->height / obj->mapType;

                tempMap16 = CreateBuffer(dx32 << 1, dy32 << 1, 2);
                mapData = FromString( (char*) obj->mapData, dx16, dy16, 4);

                for(i = 0; i < tempMap16->width; ++i)
                {
                    for(j = 0; j < tempMap16->height; ++j)
                    {
                        value = Get2Bytes(o->map16Buf, xBase + i, yBase + j);
                        Put2Bytes(tempMap16, i, j, value);
                    }
                }

                // now write in the map16 parts of the object to the tempMap16 buffer
                for(i = 0; i < dx16; ++i)
                {
                    for(j = 0; j < dx16; ++j)
                    {
                        value = Get4Bytes(mapData, i, j);
                        
                        if(value != -1)
                            Put2Bytes(tempMap16, 
                                      (x16 - xBase) + i,
                                      (y16 - yBase) + j,
                                      value);
                    }
                }   
                
                // going to do a check of all the map 8 clusters to see if we are able to allocate all the map16
                // tiles we need
                for(i = 0; i < tempMap16->width; i += 2)
                {
                    for(j = 0; j < tempMap16->height; j += 2)
                    {
                        /// value = Get2Bytes(tempMap32, i >> 1, j >> 1);
                        GetMap2x2(map16Vals, tempMap16, i, j);
                    }
                }

                for(i = 0; i < tempMap16->width; ++i)
                {
                    for(j = 0; j < tempMap16->height; ++j)
                    {
                        value = Get2Bytes(tempMap16, i, j);
                        Put2Bytes(o->map16Buf, i + xBase, j + yBase, value);
                        DrawMap16(game, b, value, i + xBase, j + yBase);
                    }
                }

                for(i = 0; i < tempMap16->width; ++i)
                {
                    for(j = 0; j < tempMap16->height; ++j)
                    {
                        value = Get2Bytes(tempMap16, i, j);
                        o->Map16To8(value, map8Vals);
                   
                        Put2Bytes(o->map8Buf, (xBase << 1) + i*2 + 0, (yBase << 1) + j*2 + 0, map8Vals[0]);
                        Put2Bytes(o->map8Buf, (xBase << 1) + i*2 + 1, (yBase << 1) + j*2 + 0, map8Vals[1]);
                        Put2Bytes(o->map8Buf, (xBase << 1) + i*2 + 0, (yBase << 1) + j*2 + 1, map8Vals[2]);
                        Put2Bytes(o->map8Buf, (xBase << 1) + i*2 + 1, (yBase << 1) + j*2 + 1, map8Vals[3]);
                    }
                }

                DeallocBuffer(tempMap16);
                DeallocBuffer(mapData);

                break;
            }


            default:

                break;
        }

        return 0;
    }

// ************************************************************

    u32 SaveOverworldData(zgPtr game, u32 *offset)
    {
        char path[0x200];
        char path2[0x300];

        char *spriteParts[3] = { "Beginning", "Part1", "Part2" };

        u32 i         = 0;
        u32 j         = 0;
        u32 size      = 0;
        u32 part      = 0;
        u32 oldOffset = 0;

        bufPtr data1 = CreateBuffer(0x100);
        bufPtr data2 = CreateBuffer(0x100);

        bufPtr comp1 = CreateBuffer(0x400);
        bufPtr comp2 = CreateBuffer(0x400);

        BM_Header *h   = &game->bm_Header;

        Entrance *e    = NULL;
        Entrance *list = NULL;

        OverItem *item    = NULL;
        MarkerList *list2 = NULL;

        OverData *o       = NULL;
        OverSpr  *sprite  = NULL;

        PathName *p       = NULL;

        Compresch_LTTP *lt = new Compresch_LTTP();

        FILE *f = NULL;

        // -------------------------------------------------------------

        p = new PathName( (const char*) ToString(game->romName) );

        chdir( (const char*) p->GetDirectory() );
        mkdir("overworld");
        chdir("overworld");

        delete p;

        f = fopen("owData.asm", "wt");

        (*offset) = RomToCpuAddr(*offset);

        o = game->overData;
        
        _getcwd(path, 0x1FF);

        if(!f || !game || !offset)     { return 0; }
        if(!game->overData)            { return 0; }

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        // update the map32 to map16 conversion arrays and pack them together
        // there's a special, nonlinear format that uses 12 bits for each map32 to map16 conversion

        /**

        h->map32To16UL = AdvancePointer(game, offset, 0x8000);
        fprintf(f, "\norg $%06X", h->map32To16UL);
        fprintf(f, "\n    owMap32To16UpperLeft:");
        fprintf(f, "\n        incbin overworld/owMap32To16UpperLeft.bin");
        ToFile(o->upperLeft32, "owMap32To16UpperLeft.bin");

        h->map32To16UR = AdvancePointer(game, offset, 0x8000);
        fprintf(f, "\norg $%06X", h->map32To16UR);
        fprintf(f, "\n    owMap32To16UpperRight:");
        fprintf(f, "\n        incbin overworld/owMap32To16UpperRight.bin");
        ToFile(o->upperRight32, "owMap32To16UpperRight.bin");

        h->map32To16LL = AdvancePointer(game, offset, 0x8000);
        fprintf(f, "\norg $%06X", h->map32To16LL);
        fprintf(f, "\n    owMap32To16LowerLeft:");
        fprintf(f, "\n        incbin overworld/owMap32To16LowerLeft.bin");
        ToFile(o->lowerLeft32, "owMap32To16LowerLeft.bin");

        h->map32To16LR = AdvancePointer(game, offset, 0x8000);
        fprintf(f, "\norg $%06X", h->map32To16LR);
        fprintf(f, "\n    owMap32To16LowerRight:");
        fprintf(f, "\n        incbin overworld/owMap32To16LowerRight.bin");
        ToFile(o->lowerRight32, "owMap32To16LowerRight.bin");
        */
    

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        
        // output map16 to map8 conversion array
        CopyBuffer(game->image, o->map16To8, CpuToRomAddr(0xF8000), 0, 0x7540);

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        // output map data

        fprintf(f, "\norg $%06X", AdvancePointer(game, offset, 0xC0 * 3));
        fprintf(f, "\n    owMap32Ptrs_1:");

        for(i = 0; i < 0xC0; ++i)
            fprintf(f, "\n        dl owMap32Ptr_1_%X", i);

        fprintf(f, "\norg $%06X", AdvancePointer(game, offset, 0xC0 * 3));
        fprintf(f, "\n    owMap32Ptrs_2:");

        for(i = 0; i < 0xC0; ++i)
            fprintf(f, "\n        dl owMap32Ptr_2_%X", i);

        for(i = 0; i < 0xC0; ++i)
        {
            if(o->map32Data[i] == NULL)
            {
                fprintf(f, "\n    owMap32Ptr_1_%X:", i);
                fprintf(f, "\n    owMap32Ptr_2_%X:", i);
                continue;
            }

            // De-interlace the data.
            for(j = 0; j < 0x200; j += 2)
            {
                PutByte(data1, (j >> 1), GetByte(o->map32Data[i], j + 1));
                PutByte(data2, (j >> 1), GetByte(o->map32Data[i], j + 0));
            }
    
            sprintf(path2, "owMap32Ptr_1_%X.bin", i);
            size = lt->Compress(data1->contents, 0x100, comp1->contents, false);
            
            if(size > comp1->length)
                j = j;

            ToFile(comp1, path2, size);

            fprintf(f, "\norg $%06X", AdvancePointer(game, offset, size));
            fprintf(f, "\n    owMap32Ptr_1_%X:", i);
            fprintf(f, "\n        incbin overworld/%s", path2);

            sprintf(path2, "owMap32Ptr_2_%X.bin", i);
            size = lt->Compress(data2->contents, 0x100, comp2->contents, false);

            if(size > comp1->length)
                j = j;

            ToFile(comp2, path2, size);

            fprintf(f, "\norg $%06X", AdvancePointer(game, offset, size));
            fprintf(f, "\n    owMap32Ptr_2_%X:", i);
            fprintf(f, "\n        incbin overworld/%s", path2); 
        }

        DeallocBuffer(data1);
        DeallocBuffer(data2);
        DeallocBuffer(comp1);
        DeallocBuffer(comp2);

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        // output hole data

        // merge all the hole data together into one array
        for(list = NULL, i = 0; i < 0xC0; ++i)
            Entrance::Add( &list, o->areas[i]->holes);

        h->overNumHoles = list->GetSize();
        size      = 5 * h->overNumHoles;

        oldOffset = AdvancePointer(game, offset, size);

        h->overHoleOffset = oldOffset;

        // Set the position in the file for the 
        fprintf(f, "\n\norg $%06x", oldOffset);
        fprintf(f, "\n    holeMap16Coords:\n");

        for(e = (Entrance*) MarkerList::GetHead(list); e != NULL; e = (Entrance*) e->GetNext())
            fprintf(f, "        dw $%04x\n", xyToMap16(e->x, e->y) - 0x400);

        fprintf(f, "\n    holeAreaNumbers:\n");

        for(e = (Entrance*) MarkerList::GetHead(list); e != NULL; e = (Entrance*) e->GetNext())
            fprintf(f, "        dw $%04x\n", e->area);

        fprintf(f, "\n    holeEntranceNumbers:\n");

        for(e = (Entrance*) MarkerList::GetHead(list); e != NULL; e = (Entrance*) e->GetNext())
            fprintf(f, "        db $%02x\n", e->entrance);

        // The Chris Houlihan entrance
        fprintf(f, "        db $82");

        (*offset) += (game->bm_Header.overNumHoles * 5);

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        // output entrance data
 
        // merge all the entrance data together
        for(list = NULL, i = 0; i < 0xC0; ++i)
            Entrance::Add( &list, o->areas[i]->entr);

        h->overNumEntr = list->GetSize();
        /// number of entrances isn't tracked...?
        size        = 5 * h->overNumEntr;
        oldOffset   = AdvancePointer(game, offset, size);

        h->overEntrOffset = oldOffset;

        // Set the position in the file for the entrance data
        fprintf(f, "\n\norg $%06x", oldOffset);

        fprintf(f, "\n    entranceAreaNumbers:\n");

        for(e = (Entrance*) MarkerList::GetHead(list); e != NULL; e = (Entrance*) e->GetNext())
            fprintf(f, "        dw $%04x\n", e->area);
        
        fprintf(f, "\n    entranceMap16Coords:\n");

        for(e = (Entrance*) MarkerList::GetHead(list); e != NULL; e = (Entrance*) e->GetNext())
            fprintf(f, "        dw $%04x\n", xyToMap16(e->x, e->y));

        fprintf(f, "\n    entranceNumbers:\n");

        for(e = (Entrance*) MarkerList::GetHead(list); e != NULL; e = (Entrance*) e->GetNext())
            fprintf(f, "        db $%02x\n", e->entrance);

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        // output secrets (item) pointer table.
        fprintf(f, "\norg $%06X", AdvancePointer(game, offset, 0x20));
        fprintf(f, "\n    LoadOwSecretPointer:"
                   "\n        ASL A : !ADD $8A : TAX"
                   "\n        LDA.l owSecretPointers, X   : STA $00"
                   "\n        LDA.l owSecretPointers+2, X : AND.w #$00FF : STA $02"
                   "\n        RTL\n");

        h->overItemOffset = AdvancePointer(game, offset, 0xC0 * 3);

        fprintf(f, "\norg $%06X"
                   "\n    owSecretPointers:", 
                   h->overItemOffset);

        // write the pointer table out
        for(i = 0; i < 0x80; ++i)
        {
            /// refactor this whole routine!!!!!!!
            if(o->areas[i]->items[0] == NULL)
                fprintf(f, "\n        dl nullSecretSet");
            else
                fprintf(f, "\n        dl owSecretPointers%X", i);
        }

        for(i = 0; i < 0x80; ++i)
        {
            list2 = o->areas[i]->items[0];

            if(list2)
                size = (list2->GetSize() * 3) + 2;
            else
                continue;

            fprintf(f, "\norg $%06X", AdvancePointer(game, offset, size));
            fprintf(f, "\n    owSecretPointers%X:", i);

            // write the secrets data for each room out.
            for(item = (OverItem*) MarkerList::GetHead(list2); item != NULL; item = (OverItem*) item->GetNext())
            {
                fprintf(f, "\n        dw $%04X ; map16 coordinate"
                           "\n        db $%02X ; item value", xyToMap16(item->x, item->y), item->itemNum);
            }

            fprintf(f, "\n        dw $FFFF");
        }

        // generate an entry for the empty item sets.
        fprintf(f, "\norg $%06X", AdvancePointer(game, offset, 2));
        fprintf(f, "\n    nullSecretSet:"
                   "\n        dw $FFFF");

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        // output sprite data

        for(part = 0; part < 3; ++part)
        {
            oldOffset = AdvancePointer(game, offset, 0xC0 * 3);

            switch(part)
            {
                case 0: h->overSprOffset0 = oldOffset; break;
                case 1: h->overSprOffset2 = oldOffset; break;
                case 2: h->overSprOffset3 = oldOffset; break;
            }

            // set the file position for the "beginning" sprites.
            fprintf(f, "\norg $%06X", oldOffset);
            fprintf(f, "\n    ow%sSpritePtrs:", spriteParts[part]);

            // output the pointer table
            for(i = 0; i < 0xC0; ++i)
            {
                if(o->areas[i]->spr[part] == NULL)
                    fprintf(f, "\n        dl nullSpriteSet");
                else
                    fprintf(f, "\n        dl ow%sSpritePtr%X", spriteParts[part], i);
            }

            // now create the data that the pointers link to.
            for(i = 0; i < 0xC0; ++i)
            {
                list2 = o->areas[i]->spr[part];

                if(list2)
                    size = (list2->GetSize() * 3) + 1;
                else
                    continue;

                // need an additional sprite to have falling rocks in the area
                if(o->areas[i]->fallingRocks[part] == true)
                    size += 3;

                fprintf(f, "\norg $%06X", AdvancePointer(game, offset, size));
                fprintf(f, "\n    ow%sSpritePtr%X:", spriteParts[part], i);

                // write the sprite data for each area in this part.
                for(sprite = (OverSpr*) MarkerList::GetHead(list2); sprite != NULL; sprite = (OverSpr*) sprite->GetNext())
                    fprintf(f, "\n        db $%02X, $%02X, $%02X ; y coord, x coord, sprite num", sprite->y >> 4, sprite->x >> 4, sprite->spriteNum);

                if(o->areas[i]->fallingRocks[i] == true)
                    fprintf(f, "\n        db $00, $00, $F4 ; falling rocks overlord sprite");

                fprintf(f, "\n        db $FF ; terminating byte");
            }

        }

        // generate an entry for the empty sprite sets.
        fprintf(f, "\norg $%06X", AdvancePointer(game, offset, 1));
        fprintf(f, "\n    nullSpriteSet:"
                   "\n        db $FF");

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    
        // output exit data.

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        // wrap things up
        (*offset) = CpuToRomAddr(*offset);

        fclose(f);

        delete lt;

        return 1;
    }




// ****************************************************************************************************************
  
