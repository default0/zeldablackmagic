

    #include "Globals.h"
    #include "Strings.h"
    #include "Core.h"

// ===============================================================

    int GetSprGfxPtr(bufPtr inputBuf, int offset)
    {
        int source = (GetByte(inputBuf, 0x4FF3 + offset) << 0x10);
        source |= (GetByte(inputBuf, 0x50D2 + offset) << 0x08);
        source |= GetByte(inputBuf, 0x51B1 + offset);

        source = CpuToRomAddr(source);

        return source;
    }

// ===============================================================

    int GetBgGfxPtr(bufPtr inputBuf, int offset)
    {
        int source = (GetByte(inputBuf, 0x4F80 + offset) << 0x10);
        source |= (GetByte(inputBuf, 0x505F + offset) << 0x08);
        source |= GetByte(inputBuf, 0x513E + offset);

        source = CpuToRomAddr(source);

        return source;
    }

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
        u32 paletteNum = header->paletteIndex * 4;

        u32 bp_4bpp = GetByte(rom, paletteNum + 0x75460);
        u32 sp_0 = GetByte(rom, paletteNum + 0x75461);
        u32 sp_1 = GetByte(rom, paletteNum + 0x75462);
        u32 sp_2 = GetByte(rom, paletteNum + 0x75463);
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
        bufPtr cgram = game->cgram;
    
        u32 i = 0;
        u32 j = 0;
        u32 red = 0x1F;
        u32 green = 0x1F << 5;
        u32 blue = 0x1F << 10;
        u32 color = 0;

        for(i = 0; i < 0x10; i++)
        {
            for(j = 0; j < 0x10; j++)
            {
                color = Get2Bytes(cgram, (0x20*i) + (2*j));
            
                (game->Pals[i])[j] = ((color & red) << 0x13);
                (game->Pals[i])[j] |= ((color & green) << 0x6);
                (game->Pals[i])[j] |= ((color & blue) >> 7);
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

    ZeldaGame::ZeldaGame()
    {
        bufPtr decomp = Create

        game->testPtr = CreateBuffer(0x800);
        
        // dump all background graphics
        for(i = 0; i < 0x73; i++)
        {
            c = GetBgGfxPtr(game->image, i);
    
            sprintf(path, "gfx\\bg_gfx_%x.bin", i);

            decomp = DecompressStandard(game->image, c);

            ToFile(decomp, path); 

            DeallocBuffer(decomp);                    
        }

        decomp = CreateBuffer(0x600);

        // dump all sprite graphics
        for(i = 0; i < 0x6C; i++)
        {                
            c = GetSprGfxPtr(game->image, i);

            sprintf(path, "gfx\\spr_gfx_%x.bin", i);

            if(i < 0x0C)
            {
                // The first 12 graphics sets are uncompressed in the rom, hence
                // the differing treatment.
                CopyBuffer(decomp, game->image, 0, c, 0x600);
                ToFile(decomp, path);
            }
            else
            {
                // The rest are compressed.
                game->testPtr = DecompressStandard(game->image, c);
                ToFile(game->testPtr, path, 0x600);
                DeallocBuffer(game->testPtr);                    
            }
        }

        DeallocBuffer(decomp);

            // Output the 2bpp font graphics for the menus / dialogue
            sprintf(path, "gfx\\font_gfx.bin");
            ToFile(game->image, path, 0x1000, 0x70000);

            // Output Link's 4bpp sprite graphics
            sprintf(path, "gfx\\link_gfx.bin");
            ToFile(game->image, path, 0x7000, 0x80000);

            // Output mode7 graphics (to-do listed)
            sprintf(path, "gfx\\mode7_gfx.bin");
            ToFile(game->image, path, 0x4000, 0xC4000);


        break;
    }

// ===============================================================
