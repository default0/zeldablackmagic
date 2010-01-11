
    #include "direct.h"

    #include "Black Magic.h"
    #include "Globals.h"
    #include "Strings.h"
    #include "Core.h"

    #include "PathName.h"

    #include "compresch_lttp.h"

// ===============================================================

    typedef enum 
    {
        mode_no_compress,
        mode_byte_repeat,
        mode_word_repeat,
        mode_increment,
        mode_copy

    } comp_mode;

// ===============================================================

    // CompressStandard is an adaptation of the compression algorithm found in Hyrule Magic.
    // For my part, this is the only piece of code that was taken from HM, and I don't even use it currently.
    // Compresch, Disch's compression library has pretty much tied up all my loose ends on the compression front.

    bufPtr CompressStandard(bufPtr srcBuf, int flag)
    {
        if(!srcBuf)
            return NULL;

        // allocate a 2^12 sized buffer
        bufPtr destBuf = CreateBuffer(0x1000); 

        if(!destBuf)
            return NULL;

        destBuf->wOffset = 0;

        int i = 0, j = 0, k = 0;
    
        int byte      = 0;
        int highByte  = 0;
        int remainder = 0;
        int word      = 0;
        int identical = 0;
        int oldSize = srcBuf->length;
    
        comp_mode p = mode_no_compress;
    
        int n = 0;
        int x = 0, q = 0, r;
    
        // flag is an endianness flag as follows
        // flag ?  bigEndian() : littleEndian();
        // hence default behavior is littleEndian

        // int *size; this is the return size of the compressed data
        // after the routine has finished
    
	    for(i = 0; i < oldSize; ) 
	    {
		    byte = GetByte(srcBuf, i); // grab a char from the buffer.
		    
		    k = 0;
		    
            // r = the same logical value (0 or 1) as q, but not the same value necesarily.
    	    r = !!q; 
        
            // looking at the bytes from the start of the array up until the byte at i for some reason...
		    for(j = 0; j < i - 1; j++) 
            {
                // 
                if( GetByte(srcBuf, j) == byte) 
			    {
				    for(n = j; n < oldSize; n++) 
				    {	
					    if( GetByte(srcBuf, n) != GetByte(srcBuf, n + i) )
						    break;
                    }
			    
                    if(n > k) 
                        k = n, word = j;
                }
            }
 
            identical = 0;

            // get the number of bytes starting at i that are identical to
            // the byte at i
            for( j = (i + 1); j < oldSize; j++ ) 
            {
                if( GetByte(srcBuf, j) != byte ) 
                    break;
                
                identical++;
            }
    
            if( identical > (1 + r) ) 
                p = mode_byte_repeat; 
            else 
            {
                // if looking for repeating bytes failed, we're going to try
                // to find a sequence of repeating words (16-bit) instead

                word = Get2Bytes(srcBuf, i);
                identical = 0;

                for(j = i + 2; j < oldSize; j++) 
                {            
                    // while words are matched, the total number of actual bytes
                    // matched is recorded, not the number of words
                    if(Get2Bytes(srcBuf, j) != word)
                    {
                        if(GetByte(srcBuf, j) == byte)
                            identical++;
                    
                        break;
                    }
                    else
                        identical += 2;
                }
        
                if( identical > (2 + r) ) 
                    p = mode_word_repeat; 
                else 
                {
                    // well that didn't work so we're going to try to
                    // find an incrementing byte series

                    // MON is iffy on how well this will actually work...
                    // due to how the actual algorithm decompresses stuff...

                    identical = 0;

                    for(j = (i + 1); j < oldSize; j++) 
                    {
                        if(GetByte(srcBuf, j) != (byte + j) ) 
                            break;

                        identical++;
                    }
            
                    if( identical > (1 + r) ) 
                        p = mode_increment; 
                    else 
                        p = mode_no_compress;
                }
            }
		    
            // what the fuck is this
            if( (k > r + 3) && (k > n + (p % 2)) ) 
            {
                p = mode_copy;
                n = k;
            }
    
            if(!p) 
                q++, i++; 
            else 
            {
                if(q) 
                {
                    q--; 

                    if(q > 0x1F) 
                        PutByte(destBuf, 0xE0 + (q >> 8) );
            
                    PutByte(destBuf, q);
                    q++;

                    CopyBuffer(destBuf, srcBuf, 
                               destBuf->wOffset,      i - q, 
                               q);

                    destBuf->wOffset += q;
                    q = 0;
                }

                i += identical;
                identical--;
                        
                if(identical > 0x1F)
                {
                    PutByte(destBuf, 0xE0 + (identical >> 8) + (p << 2));
                    PutByte(destBuf, identical);
                }
                else 
                    PutByte(destBuf, ((p << 5)+ identical));

                switch(p) 
                {
                case mode_byte_repeat: 
                case mode_increment: 

                    PutByte(destBuf, byte); 
                    break;
        
                case mode_word_repeat: 

                    Put2Bytes(destBuf, word);
                    break;

                case mode_copy: 
                    if(flag) 
                    { 
                        PutByte(destBuf, word >> 8);
                        PutByte(destBuf, word);
                    }
                    else 
                    { 
                        Put2Bytes(destBuf, word);
                    }

                    break;

                case mode_no_compress:
                default:

                    break;
                }

                continue;
            }
	    }
	
	    if(q) 
	    {
		    q--; 
		    
		    if(q > 0x1F) 
			    PutByte( destBuf, 0xE0 + (q >> 8) );
		    
		    PutByte(destBuf, q);
		    q++;
	    
            CopyBuffer(destBuf, srcBuf,
                       destBuf->wOffset, i - q,
                       q);

            destBuf->wOffset += q; 
	    }
	    
        // ensure that the data we currently have is terminated and won't
        // crash the game
	    PutByte(destBuf, 0xFF);

        // eliminates excess allocated space and resizes the buffer
        // to just fit the data we have
        ResizeBuffer(destBuf, destBuf->wOffset);
	    
	    return destBuf;
    }

// ===============================================================

    bufPtr DecompressBank02(bufPtr inputBuf, int source)
    {
        // This routine is meant to be a nearly exact replica of the decompression
        // in the actual rom. ($00E7A3)
        // Make sure to determine the source pointer before this routine in the future!

        int a_reg = 0; // corresponds to the accumulator
        int code = 0; // the decompression code
        int target = 0; // corresponds to the Y register
        int amount = 0; // corresponds to the X register
        int copyOffset = 0;
        int iterations = 0;
        int a_pushed = 0;

        bufPtr storeBuf = CreateBuffer(0x1000);
   
        // start decompressing
    
        while(target < 0x1000)
        {
            a_reg = GetByte(inputBuf, source++);
        
            if(a_reg == 0xFF)
                break;
    
            code = a_reg;
            code &= 0xE0;

            if(code == 0xE0)
                goto expanded; // code [111]

            a_pushed = a_reg;
            amount = (a_reg & 0x1F);

            goto normal;

        expanded: // code [111]
    
            code = (a_reg << 0x03) & 0xE0; // acquire a new code.
            amount = (a_reg & 0x03) << 0x08;
            amount |= GetByte(inputBuf, source++);

        normal: // 

            amount++; // inc A

            code >>= 0x05;

            switch(code)
            {
            case 0x000: non_repeating:

                a_reg = GetByte(inputBuf, source++);
                PutByte(storeBuf, target++, a_reg);

                amount--;

                if(amount > 0)
                    goto non_repeating;

                break;

            case 0x001: // repeating byte mode (RLE byte)

                a_reg = GetByte(inputBuf, source++);
            
            repeat:

                PutByte(storeBuf, target++, a_reg);
                amount--;

                if(amount > 0)
                    goto repeat;

                break;

            case 0x002: // repeating word mode (RLE word)

                a_reg = GetByte(inputBuf, source++) << 0x08;
                a_reg |= GetByte(inputBuf, source++);

            more_bytes:

                a_reg = XBA(a_reg);

                PutByte(storeBuf, target++, a_reg);
                amount--;

                if(!amount)
                    goto out_of_bytes;

                a_reg = XBA(a_reg);

                PutByte(storeBuf, target++, a_reg);
                amount--;

                if(amount > 0)
                    goto more_bytes;

            out_of_bytes:

                break;

            case 0x003: // increment mode
        
                a_reg = GetByte(inputBuf, source++);
            
            increment_write:

                PutByte(storeBuf, target++, a_reg);
                a_reg = (a_reg + 1) & 0xFF;
                amount--;

                if(amount > 0)
                    goto increment_write;

                break;

            default: // copy mode, handles codes 4-7 (LZ)

                // Loads up an offset of where to look previously
                // in the target array.
                a_reg = GetByte(inputBuf, source++);
                a_reg = XBA(a_reg);
                a_reg |= GetByte(inputBuf, source++);

                copyOffset = a_reg;

            copy_more:

                a_reg = GetByte(storeBuf, copyOffset++);
                PutByte(storeBuf, target++, a_reg);
                amount--;
            
                if(amount > 0)
                    goto copy_more;

                break;
            }

            iterations++;
        }

        // Size the buffer down if it needs it.
        ResizeBuffer(storeBuf, target);

        return storeBuf;
    }

// ===============================================================

    bufPtr DecompressStandard(bufPtr inputBuf, int source)
    {
        // This routine is meant to be a nearly exact replica of the decompression
        // in the actual rom. ($00E7A3)
        // Make sure to determine the source pointer before this routine in the future!

        int a_reg = 0; // corresponds to the accumulator
        int code = 0; // the decompression code
        int target = 0; // corresponds to the Y register
        int amount = 0; // corresponds to the X register
        int copyOffset = 0;
        int iterations = 0;
        int a_pushed = 0;

        bufPtr storeBuf = CreateBuffer(0x1000);
   
        // start decompressing
    
        while(target < 0x1000)
        {
            a_reg = GetByte(inputBuf, source++);
        
            if(a_reg == 0xFF)
                break;
    
            code = a_reg;
            code &= 0xE0;

            if(code == 0xE0)
                goto expanded; // code [111]

            a_pushed = a_reg;
            amount = (a_reg & 0x1F);

            goto normal;

        expanded: // code [111]
    
            code = (a_reg << 0x03) & 0xE0; // acquire a new code.
            amount = (a_reg & 0x03) << 0x08;
            amount |= GetByte(inputBuf, source++);

        normal: // 

            amount++; // inc A

            code >>= 0x05;

            switch(code)
            {
            case 0x000: non_repeating:

                a_reg = GetByte(inputBuf, source++);
                PutByte(storeBuf, target++, a_reg);

                amount--;

                if(amount > 0)
                    goto non_repeating;

                break;

            case 0x001: // repeating byte mode

                a_reg = GetByte(inputBuf, source++);
            
            repeat:

                PutByte(storeBuf, target++, a_reg);
                amount--;

                if(amount > 0)
                    goto repeat;

                break;

            case 0x002: // repeating word mode

                a_reg = GetByte(inputBuf, source++) << 0x08;
                a_reg |= GetByte(inputBuf, source++);

            more_bytes:

                a_reg = XBA(a_reg);

                PutByte(storeBuf, target++, a_reg);
                amount--;

                if(!amount)
                    goto out_of_bytes;

                a_reg = XBA(a_reg);

                PutByte(storeBuf, target++, a_reg);
                amount--;

                if(amount > 0)
                    goto more_bytes;

            out_of_bytes:

                break;

            case 0x003: // increment mode
        
                a_reg = GetByte(inputBuf, source++);
            
            increment_write:

                PutByte(storeBuf, target++, a_reg);
                a_reg = (a_reg + 1) & 0xFF;
                amount--;

                if(amount > 0)
                    goto increment_write;

                break;

            default: // LZ mode, handles codes 4-7

                // Loads up an offset of where to look previously
                // in the target array.
                a_reg = GetByte(inputBuf, source++);
                a_reg = XBA(a_reg);
                a_reg |= GetByte(inputBuf, source++);
                a_reg = XBA(a_reg);

                copyOffset = a_reg;

            copy_more:

                a_reg = GetByte(storeBuf, copyOffset++);
                PutByte(storeBuf, target++, a_reg);
                amount--;
            
                if(amount > 0)
                    goto copy_more;

                break;
            }

            iterations++;
        }

        // Size the buffer down if it needs it.
        ResizeBuffer(storeBuf, target);

        return storeBuf;
    }

// ===============================================================

    void Do3To4Low(bufPtr targBuf, bufPtr srcBuf, u32 *targPtr, u32 *srcPtr, u32 tiles)
    {
        // Upconverts 3bpp graphics to 4bpp, and restricts to the lower 8 colors of the palette
        // In other words it ensures that the 4th byte of each line in a tile is always 0x00

        u32 i = 0;
        u32 j = 0;

        // ------------------------------

        for(i = 0; i < tiles; i++)
        {
            CopyBufferWB(targBuf, srcBuf,
                         targPtr, srcPtr,
                         0x10);
    
            for(j = 0; j < 16; j += 2)
            {
                Put2Bytes(targBuf, *targPtr, GetByte(srcBuf, *srcPtr));
                *targPtr += 2;
                (*srcPtr)++;
            }
        }
    }

// ===============================================================

    void Do3To4High(bufPtr targBuf, bufPtr srcBuf, u32 *targPtr, u32 *srcPtr, u32 cap)
    {
        // upconverts 3bpp graphics to 4bpp and makes is so that any nonzero color index
        // has the high bits set (meaning it uses the upper 8 colors of the palette

        u32 oldPos = 0;
        u32 start = (*srcPtr);
        u32 i = start;
        u32 j = (*targPtr);
        u32 k = 0;

        u8 value = 0, accum = 0;

        // ------------------------------

        for( ; i < srcBuf->length; )
        {
            if((i - start) >= cap) 
                return;

            // save this position - we'll need it.
            oldPos = i;

            CopyBufferWB(targBuf, srcBuf,
                         &j,      &i,
                         0x10);

            for(k = 0; k < 0x10; i++, j += 2, k += 2)
            {
                value = GetByte(srcBuf, i);

                accum = value;
                accum |= GetByte(srcBuf, oldPos + k);
                accum |= GetByte(srcBuf, oldPos + k + 1);

                PutByte(targBuf, j, value);
                PutByte(targBuf, j + 1, accum);
            }
        }

        (*srcPtr) = i;
        (*targPtr) = j;
    }

// ===============================================================

    void LoadTilesets(zgPtr game)
    {
        // this routine corresponds to $00:E19B in the rom

        u32 i = 0, j = 0;
        u32 index = 0, index2 = 0, index3 = 0, index4 = 0, dummyInt = 0, source = 0; 

        bufPtr decomp = NULL;
        giPtr gi = game->gi;

        // -----------------------------------------------

        // Find the pointer to the graphics to decompress
        // should optionally use this or 0x01 or 0x0A depending on whether you're in the dark world or not.
        index4 = game->GetSprGfxPtr(gi->spriteGfx1);
        
        // Decompress them using a special method
        index = 0x8800;

        Do3To4High(game->vram, game->image,
                   &index,     &index4,
                   0x600);

        // --------------------------------------------------------

        // 0x06 is always used for this graphics slot
        index4 = game->GetSprGfxPtr(0x6);
    
        Do3To4Low(game->vram, game->image,
                  &index,     &index4,
                  0x80);
 
        // --------------------------------------------------------

        // Now we move on to decompressing and storing the 4 graphics subsets
        // that are dependent upon the room you are in.

        index3 = gi->spriteGfx0;
        index3 *= 4;
        index3 += 0x5B57;

        for(i = 0; i < 4; i++, index3++)
        {
            // Get one of the current 4 graphics set numbers
            index2 = GetByte(game->image, index3);
            index4 = game->GetSprGfxPtr(index2);
    
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

                decomp = DecompressStandard(game->image, index4);
                index4 = 0;

                Do3To4High(game->vram, decomp,
                           &index,     &index4,
                           0x600);

                DeallocBuffer(decomp);

                break;

            default:
            
                decomp = DecompressStandard(game->image, index4);
                index4 = 0;

                Do3To4Low(game->vram,   decomp,
                          &index,       &index4,
                          0x40);

                DeallocBuffer(decomp);

                break;
            }
        }



        // Next decompress the BG graphic data
        // Target in VRAM is 0x4000

        index = 0x4000;

        // The primary BG tile set 
        index3 = gi->backGfx0;
        index3 *= 8;
        index3 += 0x6073;

        // graphicsNum corresponds to $0AA2
        index2 = gi->backGfx1;
        index2 *= 4;
        index2 += 0x5D97;

        for(i = 0; i < 8; i++, index3++)
        {
            j = 7 - i;

            source = GetByte(game->image, index3);     
            source = game->GetBgGfxPtr(source);

            index4 = GetByte(game->image, index2 + i - 3);

            if( j >= 1 && j <= 4)
                if( index4 != 0)
                    source = game->GetBgGfxPtr(index4);

            index4 = 0;

            decomp = DecompressStandard(game->image, source);

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

            DeallocBuffer(decomp);
        
        }

        return;
    }

// ===============================================================

    void LoadPalettes(zgPtr game, u32 offset, u32 numPalettes, u32 numColors, u32 cgOffset)
    {
        bufPtr rom   = game->image;

        u32 i        = 0;
        u32 ret      = 0;
        u32 numBytes = (numColors * 2);

        // ------------------------------

        for(i = 0; i < numPalettes; i++, offset += numBytes)
        {
            ret = CopyBuffer(game->cgram,         rom,
                             cgOffset + (i*0x20), offset,
                             numBytes);
        }
    }

// ===============================================================

    void LoadPalettes(zgPtr game)
    {
        // Notes on terminology

        // BP = Background Palette
        // SP = Sprite Palette
        // e.g. BP-0 would refer to the first Background palette
        // which is $0-$1F in CGRAM
        // SP-0 would be the first sprite palette ($100-$11F)
        // All writes to CGRAM are indented by 2 bytes
        // Partially because of the way LTTP was designed to swap 3bpp graphics from 
        // "side to side," so to speak,
        // and partically because the first color of every 16 color palette is ignored.
        // (or 4 or 256 color palette for that matter)

        //   Halves of each palette     Memory Involved
        //   first  | second
        /*
        BP-0 $DEE52 | (full)            $0AB2
        BP-1 $DEE52 | ( .. )            ( .. )
        BP-2 $DEE74 | ( .. )            $0AB6
        BP-3 $DEE74 | ( .. )            ( .. )
        BP-4 $DEE74 | ( .. )            ( .. )
        BP-5 $DEE74 | ( .. )            ( .. )
        BP-6 $DEE74 | ( .. )            ( .. )
        BP-7 $DEE74 | ( .. )            ( .. )

        SP-0 $DEC77 | $DEE74            $0AAC | $0AB6
        SP-1 $DEC9E | (full)            lightworld / darkworld
        SP-2 $DEC9E | ( .. )            ( .. )                        
        SP-3 $DEC9E | ( .. )            ( .. )
        SP-4 $DEC9E | ( .. )            ( .. )
        SP-5 $DECC5 | $DED03 & $DED29   $0AAD | $7EF359-A
        SP-6 $DECE4 | $DED6E            $0AAE | $0AB1 (different for outdoors)
        SP-7 $DEDF9 | (full)            $
        */

        u32 bp_2bpp = 0; // could be 1 in some situations like ending mode.
        u32 paletteNum = game->bm_dung_header.paletteIndex * 4;

        u32 bp_4bpp = GetByte(game->image, paletteNum + 0x75460);
        u32 sp_0 = GetByte(game->image, paletteNum + 0x75461);
        u32 sp_1 = GetByte(game->image, paletteNum + 0x75462);
        u32 sp_2 = GetByte(game->image, paletteNum + 0x75463);
        u32 sword = 4, shield = 3, armor = 2;
        u32 sp_throw = 0;

        u32 source;
        u32 bank = 0x1B0000;
        u32 i = 0;

        BM_DungeonHeader *header = &game->bm_dung_header;
        bufPtr rom = game->image;
        bufPtr cgram = game->cgram;

        const u32 full = 0x1E, second = 7*2;

        // ------------------------------

        if(game->currentFunc == dung)
            sp_throw = 0xA;

        // Loads BP-0 through BP-1 (full) --------------------------
        source = GetByte(rom, bp_2bpp + 0xDEC47);
        source = CpuToRomAddr( (source + 0xD660) | bank ) ;

        CopyBuffer(cgram, rom, 0, source, 0x20 * 2);
    
        // Loads BP-2 through BP-7 and SP-0 (second half) ----------
        source = Get2Bytes(rom, bp_4bpp + 0xDEC4B);
        source = CpuToRomAddr( (source + 0xD734) | bank );
    
        CopyBuffer(cgram, rom, 0x112, source, second); 

        for(i = 0; i < 6; i++, source += full)
        {
            CopyBuffer(cgram,               rom,
                       0x42 + (i * 0x20),   source,
                       full);
        }

        // Loads SP-0 (first half) ---------------------------------
        source = GetByte(rom, sp_0 + 0xDEBC6);
        source = CpuToRomAddr( (source + 0xD39E) | bank );

        CopyBuffer(cgram, rom, 0x102, source, second);

        // Loads SP-1 through SP-4 (full)
        // Note: look up later how to find the world for each room
        source = game->currentDungRoom & 0x40 ? 2 : 0;
        source = 0;
        source = Get2Bytes(rom, source + 0xDEC73);
        source = CpuToRomAddr( (source + 0xD218) | bank );

        for(i = 0; i < 4; i++, source += full)
        {
            CopyBuffer(cgram,                   rom,
                       0x122 + (i * 0x20),      source,
                       full);
        }

        // Loads SP-5 (first half)

        source = Get2Bytes(rom, (sp_1 * 2) + 0xDEBD6);
        source = CpuToRomAddr( (source + 0xD4E0) | bank );

        CopyBuffer(cgram, rom, 0x1A2, source, second);

        // Loads SP-5 (second half)
        // Uses a sword value to load a 3 color palette

        source = GetByte(rom, sword + 0xDEBB4);
        source = CpuToRomAddr( (source + 0xD630) | bank );

        CopyBuffer(cgram, rom, 0x1B2, source, 6);

        // Uses the shield value to load a 4 color palette
        source = GetByte(rom, shield + 0xDEBC1);
        source = CpuToRomAddr( (source + 0xD648) | bank );

        CopyBuffer(cgram, rom, 0x1B8, source, 8);

        // Loads SP-6 (first half)

        source = Get2Bytes(rom, (sp_2 * 2) + 0xDEBD6);
        source = CpuToRomAddr( (source + 0xD4E0) | bank );

        CopyBuffer(cgram, rom, 0x1C2, source, second);

        // Loads SP-6 (second half)

        source = GetByte(rom, sp_throw + 0xDEBC6);
        source = CpuToRomAddr( (source + 0xD446) | bank );

        CopyBuffer(cgram, rom, 0x1D2, source, second);

        // Loads SP-7 (full)

        source = GetByte(rom, armor + 0xDEC06) << 1;
        source = CpuToRomAddr( (source + 0xD308) | bank );

        CopyBuffer(cgram, rom, 0x1E2, source, full);
    }


// ===============================================================

    SnesTile* SnesToTile(bufPtr image, SnesTile* product, unsigned int offset)
    {
        // see planning.c for more info on how this works.

        u8 color_bits[4];
        u8 color_value = 0;

        int line = 0;
        int column = 0;
        int masked_bits;
        int currentline = 0;

        u32 mask;

        // ------------------------------

        for(  ; line < 8; line++)
        {
            currentline = Get2Bytes(image, (line * 2) + offset);
            currentline |= ( Get2Bytes( image, (line * 2) + 0x10 + offset) << 0x10 ); 

            mask = 0x80808080;

            for( column = 0; column < 8; column++)
            {
                masked_bits = (currentline & mask);
        
                color_bits[0] = (u8) (masked_bits);
                color_bits[0] >>= (7 - column);
        
                color_bits[1] = (u8) (masked_bits >> 0x08);
                color_bits[1] >>= (7 - column);
                color_bits[1] <<= 1;
        
                color_bits[2] = (u8) (masked_bits >> 0x10);
                color_bits[2] >>= (7 - column); 
                color_bits[2] <<= 2;

                color_bits[3] = (u8) (masked_bits >> 0x18);
                color_bits[3] >>= (7 - column);
                color_bits[3] <<= 3;

                color_value = color_bits[0] | 
                              color_bits[1] | 
                              color_bits[2] | 
                              color_bits[3];

                color_value &= 0x0F;
        
                (*product)[column + (8*(7-line))] = color_value;

                mask >>= 1;
            }
        }

        return product;
    }

// ===============================================================

    SnesTile* FlipTile(SnesTile* input, SnesTile* product, int x, int y)
    {
        int column = 0;
        int line = 0;

        SnesTile temp;

        // ------------------------------

        if(!x && !y)
        {
            memcpy(*product, *input, sizeof(SnesTile));

            return product;
        }

        // if we need to x flip, do so.
        if(x)
        {
            column = 0;
            line = 0;

            while(line < 64)
            {
                for( column = 0; column < 8; column++)
                {
                    (*product)[column + line] = (*input)[(7 - column) + line];
                }

                line += 8;
            }

            memcpy(temp, *product, sizeof(SnesTile));
        }
        else
            memcpy(temp, *input, sizeof(SnesTile));



        // same with y flip.
        if(y)
        {
            line = 0;
            column = 0;

            while( column < 8)
            {
                for( line = 0; line < 64; line += 8)
                {
                    (*product)[line + column] = (temp)[(56 - line) + column];
                }

                column++;
            }

        }

        return product;
    }

// ===============================================================

    int TileToBitmap(SnesTile* tile, SnesImage* product, bufPtr cgram, u32 palette)
    {
        /*
        int index = 0;
        unsigned char color_value;

        for( ; index < 64; index++)
        {
            color_value = (*tile)[index];

            (*product)[index] = (*palette)[color_value];
        }
    */
        return 1;
    }

// ===============================================================

    GameBoyImage* GameBoyToBitmap(bufPtr image, GameBoyPal* palette, GameBoyImage* product)
    {

        return product;
    }

// ===============================================================

    ManagedBitmap* CreateManagedBitmap(u32 width, u32 height)
    {
        u32 headerSize = sizeof(BITMAPINFOHEADER);    
        u32 numPixels = width * height;
    
        ManagedBitmap* b = (ManagedBitmap*) malloc(sizeof(ManagedBitmap));
    
        // --------------------------------------

        if(!b)
            return NULL;

        b->memory = CreateBuffer(headerSize + (sizeof(RGBQUAD) * numPixels) );
        b->header = (BITMAPINFOHEADER*) (b->memory->contents);
        b->structure = (BITMAPINFO*) (b->memory->contents);

        b->header->biBitCount = 32;
        b->header->biClrImportant = 0;
        b->header->biClrUsed = 0;
        b->header->biCompression = BI_RGB;
        b->header->biHeight = height;
        b->header->biPlanes = 1;
        b->header->biSize = headerSize;
        b->header->biSizeImage = 0;
        b->header->biWidth = width;
        b->header->biXPelsPerMeter = 0;
        b->header->biYPelsPerMeter = 0;

        b->structure->bmiHeader = *(b->header);

        b->pixels = GetSubBuffer(b->memory, headerSize);
        b->stride = (b->header->biWidth) * 4;
        b->bottom = b->stride * (b->header->biHeight - 1);

        return b;
    }


// ===============================================================

    void DecodePalettes(zgPtr game)
    {
        u32 color   = 0;
        u32 i       = 0;
        u32 j       = 0;

        // masks for extracting each 5-bit component
        // of color data from the 15-bit BGR (SNES) color.
        u32 red     = 0x1F;
        u32 green   = 0x1F << 5;
        u32 blue    = 0x1F << 10;

        bufPtr cgram = game->cgram;

        // --------------------------
    
        for(i = 0; i < 0x10; ++i)
        {
            for(j = 0; j < 0x10; ++j)
            {
                color = Get2Bytes(cgram, (0x20*i) + (2*j));
            
                (game->Pals[i])[j]  = ((color & red)   << 0x13);
                (game->Pals[i])[j] |= ((color & green) << 0x06);
                (game->Pals[i])[j] |= ((color & blue)  >> 0x07);
                (game->Pals[i])[j] &= 0xFFFFFF;

            }
        }
    }

// ===============================================================

    // The default offset is 0. In the case of animated tiles you might
    // use a later offset to only exclude
    void DecodeTiles(zgPtr game, u32 firstTile, u32 lastTile)
    {
        bufPtr vram = game->vram;

        u32 i = firstTile;
        u32 j = 0x20 * firstTile;

        for( ; i <= lastTile ; ++i, j += 0x20)
            SnesToTile(vram, &game->BGTiles[i], 0x4000 + j);

        return;
    }

// ===============================================================

    void DecodeOAMTiles(zgPtr game)
    {
        bufPtr vram = game->vram;

        u32 i = 0;
        u32 j = 0;

        for(i = 0; i < 0x200; ++i, j += 0x20)
            SnesToTile(vram, &game->OAMTiles[i], 0x8000 + j);
    }

// ===============================================================

    int ZeldaGame::GetSprGfxPtr(int index)
    {
        u32 base    = CpuToRomAddr(bm_Header.sprGfxOffset);
        u32 defBase = CpuToRomAddr(default_spr_gfx_ptr);
        u32 source  = 0;

        // -------------------------------------

        if(base == defBase)
        {
            // broke ass old method in the original game
            source  = GetByte(image, base               + index) << 0x10;
            source |= GetByte(image, base + 0xDF        + index) << 0x08;
            source |= GetByte(image, base + (0x0DF * 2) + index);

            source = CpuToRomAddr(source);
        }
        else
        {
            // new school method
            source = Get3Bytes(image, index * 3);
        }

        return source;
    }

// ===============================================================

    int ZeldaGame::GetBgGfxPtr(int index)
    {
        u32 base    = CpuToRomAddr(bm_Header.bgGfxOffset);
        u32 defBase = CpuToRomAddr(default_bg_gfx_ptr);
        u32 source  = 0;

        // -------------------------------------

        if(base == defBase)
        {
            // broke ass old method in the original game
            source  = GetByte(image, base              + index) << 0x10;
            source |= GetByte(image, base + 0xDF       + index) << 0x08;
            source |= GetByte(image, base + (0xDF * 2) + index);

            source = CpuToRomAddr(source);
        }
        else
        {
            // new school method
            source = Get3Bytes(image, index * 3);
        }

        return source;
    }

// ===============================================================

    void ZeldaGame::LoadAllGfx()
    {
        u32 i           = 0;
        u32 offset      = 0;
        u32 numPacks    = 0;
        
        bufPtr decomp = NULL;

        // ----------------------------------
        
        numPacks = (bm_Header.sprGfxOffset == default_spr_gfx_ptr) ? 0x73 : 0x100;

        // dump all sprite graphics
        for(i = 0; i < 0x100; ++i)
        {
            if(i >= numPacks)
            {
                this->sprPacks[i] = CreateBuffer(0x600); 

                continue;
            }

            offset = GetSprGfxPtr(i);

            if(i < 0x0C)
            {
                // The first 12 graphics sets are uncompressed in the rom, hence
                // the differing treatment.
                this->sprPacks[i] = CreateBuffer(0x600);

                CopyBuffer(this->sprPacks[i], this->image, 0, offset, 0x600);
            }
            else
            {
                // The rest are compressed.
                this->sprPacks[i] = DecompressStandard(this->image, offset);
            }
        }

        numPacks = (bm_Header.bgGfxOffset == default_bg_gfx_ptr) ? 0x6C : 0x100;
        
        // decompress all background graphics
        for(i = 0; i < 0x100; ++i)
        {
            if(i >= numPacks)
            {
                this->bgPacks[i] = CreateBuffer(0x600);

                continue;
            }
        
            offset = GetBgGfxPtr(i);

            this->bgPacks[i] = DecompressStandard(this->image, offset);
        }

        // Copy the 2bpp font graphics for the menus / dialogue
        this->fontGfx = CreateBuffer(0x1000);
        CopyBuffer(this->fontGfx, this->image, 0, 0x70000, 0x1000);

        // Copy Link's 4bpp sprite graphics to a buffer
        this->linkGfx = CreateBuffer(0x7000);
        CopyBuffer(this->linkGfx, this->image, 0, 0x80000, 0x7000);

        // Copy mode7 graphics to a buffer
        this->mode7Gfx = CreateBuffer(0x4000);
        CopyBuffer(this->mode7Gfx, this->image, 0, 0xC4000, 0x4000);

        // That should about do it... don't think there are any other graphics in the game...
        return;
    }

// ===============================================================

    bool ZeldaGame::SaveGraphics(u32 *offset)
    {
        char *gfxText   = NULL;

        u32 cpuOffset   = 0;
        u32 i           = 0;
        u32 size        = 0;

        BM_Header *h    = &this->bm_Header;

        bufPtr compBuf  = CreateBuffer(0x2000);

        FILE *f         = NULL;

        Compresch_LTTP *lt  = new Compresch_LTTP(false);

        // set the current directory to the directory that the rom was found in.
        PathName *p = new PathName( (const char*) ToString(romName) );

        // ---------------------

        gfxText = GetAsmTextResource(IDR_GRAPHICS_ASM);

        if(!gfxText)
        {
            return false;
        }

        (*offset) = RomToCpuAddr(*offset);

        chdir( (const char*) p->GetDirectory() );

        f = fopen("graphics.asm", "wt");

        if(f)
        {
            // leave enough room for the hooked in routine
            fprintf(f, "\norg $%06X", AdvancePointer(this, offset, 0x80));
            fprintf(f, "%s", gfxText);

            // compress all the sprite graphics packs and construct the
            // table of their pointers in the asm file.
            h->sprGfxOffset = AdvancePointer(this, offset, 0x300);

            fprintf(f, "\norg $%06X", h->sprGfxOffset);
            fprintf(f, "\n    SprGfxPtrTable:");

            for(i = 0; i < 0x100; ++i)
            {
                size      = lt->Compress(sprPacks[i]->contents, sprPacks[i]->length, compBuf->contents);
                cpuOffset = AdvancePointer(this, offset, size);

                fprintf(f, "\n        dl $%06X", cpuOffset);

                CopyBuffer(image, compBuf, CpuToRomAddr(cpuOffset), 0, size);
            }

            // Same as before, but this time for background graphics
            h->bgGfxOffset = AdvancePointer(this, offset, 0x300);

            fprintf(f, "\norg $%06X", h->bgGfxOffset);
            fprintf(f, "\n    BgGfxPtrTable:");

            for(i = 0; i < 0x100; ++i)
            {
                size      = lt->Compress(bgPacks[i]->contents, bgPacks[i]->length, compBuf->contents);
                cpuOffset = AdvancePointer(this, offset, size);

                fprintf(f, "\n        dl $%06X", cpuOffset);

                CopyBuffer(image, compBuf, CpuToRomAddr(cpuOffset), 0, size);
            }

            fclose(f);
        }

        // the locations of these graphics don't change
        CopyBuffer(image, fontGfx,  0x70000, 0, 0x1000);
        CopyBuffer(image, linkGfx,  0x80000, 0, 0x7000);
        CopyBuffer(image, mode7Gfx, 0xC4000, 0, 0x4000);

        (*offset) = CpuToRomAddr(*offset);

        // clean up ------------------------------------------

        delete p;

        delete lt;

        DeallocBuffer(compBuf);

        return true;
    }

// ===============================================================
