
    // Functions for handling strings
    // MathOnNapkins Copyright 2005

    // 1. #include "Strings.h"
    #include "Globals.h"
    #include "Black Magic.h"
    #include "Strings.h"

// ===============================================================

    bufPtr CreateBuffer(int length)
    {
        bufPtr newBufPtr = AllocBuffer();

        InitBuffer(length, newBufPtr);

        return newBufPtr;
    }

// ===============================================================

    bufPtr CreateBuffer(int width, int height, int unit)
    {
        bufPtr newBufPtr = CreateBuffer( (width * height * unit) );

        if(newBufPtr)
        {
            newBufPtr->width  = width;
            newBufPtr->height = height;
            newBufPtr->unit   = unit;


        }

        return newBufPtr;
    }

// ===============================================================

    int InitBuffer(int length, bufPtr bufArg)
    {
        unsigned int counter = 0;
        unsigned int bufferSize = (length + 1) * charSize;

        static int allocations = 0;

        char errorBuf[256];
  
        if(length == 0)
        {
            //MessageBox(0, "shit", "d'oh", MB_OK);
        }
      
        itoa(allocations, errorBuf, 10);

        if(_CrtCheckMemory() == FALSE)
            MessageBox(0, errorBuf, "initbuff", MB_OK);

        try{
            allocations++;
            bufArg->contents = (unsigned char*) malloc(bufferSize);
        }
        catch(...)
        {
            MessageBox(0, "Memory Exception Caught!", "shit!!!", MB_OK);

            exit(-1);
        }

        if(bufArg->contents == NULL)
        {
            MessageBox(0, "Init Buffer Failure", "error", MB_OK);
            return 0;
        }

        ZeroMemory(bufArg->contents, bufferSize);

        totalMem += bufferSize;

        bufArg->length = length; 
        // The actual length is the number of meaningful bytes it stores.

        bufArg->error   = 0;
        bufArg->wOffset = 0;
        bufArg->width   = length;
        bufArg->height  = 1;
        bufArg->unit    = 1;

        return 1;
    }

// ===============================================================

    bufPtr DuplicateBuffer(bufPtr bufArg)
    {
        bufPtr newBufPtr = NULL;

        // -----------------------

        if(bufArg)
            newBufPtr = CreateBuffer(bufArg->width, bufArg->height, bufArg->unit);

        CopyBuffer(newBufPtr, bufArg, 0, 0, bufArg->length);
    
        return newBufPtr;
    }

// ===============================================================

    int ResizeBuffer(bufPtr bufArg, int newSize)
    {
        if(newSize == 0)
            return 0;

        bufArg->contents = (unsigned char*) realloc(bufArg->contents, newSize + 1);
        bufArg->contents[newSize] = 0; 

        if(bufArg)
        {
            totalMem -= bufArg->length;
            totalMem += newSize;

            bufArg->length = newSize;

            return 1;
        }
        else
        {
            bufArg->length = -1;
            return 0;
        }
 
    
    }

// ===============================================================

    void DestroyBuffer(bufPtr bufArg)
    {
        // Deallocates the contents of a buffer but does not free the buffer struct itself!

        if(!bufArg)
            return;

        if(bufArg->contents != 0)
        {
            //bufArg->contents = (char*) realloc(bufArg->contents,0);

            try
            {
            free(bufArg->contents);
            }
            catch(int ex)
            {
                if(ex == 0xC0000005)
                    return;            

                return;
            }

            bufArg->contents = 0;
            totalMem -= bufArg->length + 1;
            bufArg->length = -1;
        }

        return;
    }

// ===============================================================

    int ConcatBuffer(bufPtr first, bufPtr second)
    {

        // needs work but too lazy.
        int newBufSize = (first->length + second->length + 1) * charSize;
        int counter = first->length;

        // temporary buffer used to extend the 
        unsigned char* temp = (unsigned char*) malloc(newBufSize);

        if(!temp)
        {
            MessageBox(0, "ConcatBuffer Failure", "error", MB_OK);
            return 0;
        }

        // Only copy the relevant bytes. Ignore the zero terminator.
        memcpy(temp, first->contents, first->length);

        // Copy the second buffer where the first one ends, then add in the null terminator.
        for(counter ; counter < newBufSize ; counter += charSize)
                temp[counter] = second->contents[counter - (first->length)];
    

        // might generate an error. not sure.
        free(second->contents);

        //Also might generate an error.
        free(first->contents);
    
        first->contents = temp;
        first->length = newBufSize;

        return 1;
    }

// ===============================================================

    // Sample buffer - assume 32 bit int
    // 00 02 04 06 08 0A 0C 0F
    //             [ intsize ]
    // [    new intsize      ]
    // then assume 64 bit int

    int Get4Bytes(bufPtr source, unsigned int offset)
    {
        unsigned int checker = offset + 4;

        if(source->contents == NULL)
            return 0;

        if(checker > source->length)
        {
            //MessageBox(0, "Out of bounds in Get4Bytes", "error", MB_OK); 
        
            source->error = 1;

            return 0;
        }

        if(offset < 0)
        {
            //MessageBox(0, "Out of bounds in Get4Bytes", "error", MB_OK); 

            source->error = 1;

            return 0;
        }

        checker = (int)  ( ( (source->contents) + offset)[0] & 0xFF);
        checker |= (int) ( ( (source->contents) + offset)[1] << 8 & 0x00FF00);
        checker |= (int) ( ( (source->contents) + offset)[2] << 16 & 0xFF0000);
        checker |= (int) ( ( (source->contents) + offset)[3] << 24 & 0xFF000000);

        return checker;
    }

// ===============================================================

    int Get4Bytes(bufPtr source, u32 x, u32 y)
    {
        if(source)
            return Get4Bytes(source,  (x + (y * source->width)) * source->unit );
        else
            return -1;
    }

// ===============================================================

    // returns a 24 bit integer.

    int Get3Bytes(bufPtr source, int offset)
    {
        unsigned int checker = offset + 3;

        if(source->contents == NULL)
            return 0;

        if(checker > source->length)
        {
            //MessageBox(0, "Out of bounds in Get3Bytes", "error", MB_OK); 
        
            source->error = 1;

            return 0;
        }

        if(offset < 0)
        {
            //MessageBox(0, "Out of bounds in Get3Bytes", "error", MB_OK); 

            source->error = 1;

            return 0;
        }

        checker = (int) ( ( (source->contents) + offset)[0] & 0xFF );
        checker |= (int) ( ( (source->contents) + offset)[1] << 8 & 0x00FF00);
        checker |= (int) ( ( (source->contents) + offset)[2] << 16 & 0xFF0000);

        checker &= 0xFFFFFF;

        return checker;
    }

// ===============================================================

    int Get3Bytes(bufPtr source, u32 x, u32 y)
    {
        if(source)
            return Get3Bytes(source,  (x + (y * source->width)) * source->unit );
        else
            return -1;
    }

// ===============================================================

    // returns a 16 bit integer

    int Get2Bytes(bufPtr source, int offset)
    {
        int checker = offset + 2;

        if(source->contents == NULL)
            return 0;

        if((unsigned int) checker > source->length)
        {
            //MessageBox(0, "Out of bounds in Get2Bytes", "error", MB_OK);

        
            source->error = 1;

            return GetByte(source, offset);
        }

        if(offset < 0)
        {
            //MessageBox(0, "Out of bounds in Get2Bytes", "error", MB_OK); 

            source->error = 1;
            
            return 0;
        }

        checker = *(int*) ( (source->contents) + offset);

        checker &= 0xFFFF;

        return checker;
    }

// ===============================================================

    int Get2Bytes(bufPtr source, u32 x, u32 y)
    {
        if(source)
            return Get2Bytes(source, (x + (y * source->width)) * source->unit );
        else
            return -1;
    }

// ===============================================================

    // returns an 8 bit integer
    int GetByte(bufPtr source, int offset)
    {
        int checker = offset + 1;
        
        if(!source)
            return 0;

        if(source->contents == NULL)
            return 0;

        if((unsigned int) checker > source->length)
        {
            //MessageBox(0, "Out of bounds in GetByte", "error", MB_OK); 

            source->error = 1;

            return 0;
        }

        if(offset < 0)
        {
            //MessageBox(0, "Out of bounds in GetByte", "error", MB_OK); 

            source->error = 1;

            return 0;
        }

        checker = *( (source->contents) + offset);

        checker &= 0xFF;

        source->error = 0;
  
        return checker;
    }

// ===============================================================

    int GetByte(bufPtr source, u32 x, u32 y)
    {
        if(source)
            return GetByte(source,  (x + (y * source->width)) * source->unit );
        else
            return -1;
    }

// ===============================================================

    bufPtr AllocBuffer()
    {
        bufPtr temp = (bufPtr) malloc(sizeof(Buffer));

        if(temp)
        {
            totalMem += sizeof(Buffer);
            temp->contents = 0;
            return temp;
        }
        else
            return 0;
    }

// ===============================================================

    u32 DeallocBuffer(bufPtr a) // opposite of CreateBuffer( )
    {
        DestroyBuffer(a);

        if(a)
        { 
            free(a);
            totalMem -= sizeof(Buffer);
        }
        else
            return 0;

        return 1;
    }

// ===============================================================

    u32 GetBit(bufPtr source, u32 bit)
    {
        u32 offset = (bit / 8);
        u32 remainder = (bit % 8);
        u32 checker = offset + 1;
        u8 mask = 0;

        if(source->contents == NULL)
            return 2;

        if(checker > source->length)
        {
            //MessageBox(0, "Out of bounds in GetBit", "error", MB_OK); 
           
            source->error = 1;
            return 2;
        }

        mask = source->contents[offset];
        mask &= (1 << remainder);
        
        return (mask ? 1 : 0);
    }

// ===============================================================

    u32 PutBit(bufPtr target, u32 value, u32 bit)
    {
        // checker is the byte of the buffer to look at.
        // remainder is 0 to 7 and helps us find the

        u32 offset = (bit / 8);
        u32 remainder = (bit % 8); 
        u32 checker = offset + 1;
        u8 mask = 0;

        if(target->contents == NULL)
            return 2;

        if(checker > target->length)
        {
            //MessageBox(0, "Out of bounds in PutBit", "error", MB_OK); 
           
            target->error = 1;
            return 2;
        }
        
        mask = 1 << remainder;
        target->contents[offset] &= ~mask;

        mask = value ? mask : 0;
        target->contents[offset] |= mask;

        return 1;
    }

// ===============================================================

    int PutByte(bufPtr target, int offset, int value)
    {
        int checker = offset + 1;

        if(target->contents == NULL)
            return 0;

        if( (unsigned int) checker > target->length)
        {
            //MessageBox(0, "Out of bounds in PutByte", "error", MB_OK); 
           
            target->error = 1;

            return 0;
        }

        if(offset < 0)
        {
            //MessageBox(0, "Out of bounds in PutByte", "error", MB_OK); 
    
            target->error = 1;

            return 0;
        }    

        value &= 0xFF;

        target->contents[offset] = (char) value;

        return 1;
    }

// ===============================================================

    int PutByte(bufPtr target, int value)
    {
        if(!target)
            return 0;

        return PutByte(target, target->wOffset++, value);
    }

// ===============================================================

    int PutByte(bufPtr target, u32 x, u32 y, u32 value)
    {
        if(target)
            return PutByte(target,  (x + (y * target->width)) * target->unit, value);
        else
            return -1;
    }

// ===============================================================

    u32 Put2Bytes(bufPtr target, u32 *offset, u32 value)
    {    
        if(!target)
            return 0;

        if(!Put2Bytes(target, (*offset), value ))
            return 0;

        *offset += 2;    

        return 1;
    }

// ===============================================================

    u32 Put2Bytes(bufPtr target, u32 value)
    {    
        if(!target)
            return 0;

        if(!Put2Bytes(target, target->wOffset, value))
            return 0;

        target->wOffset += 2;

        return 1;
    }

// ===============================================================

    int Put2Bytes(bufPtr target, int offset, int value)
    {
        if(target->contents == NULL)
            return 0;

        int checker = offset + 2;
        int low = value & 0xFF;
        int high = (value & 0xFF00) >> 0x08;

        if( (unsigned int) checker > target->length)
        {
            //MessageBox(0, "Out of bounds in Put2Bytes", "error", MB_OK); 
        
            target->error = 1;

            return 0;
        }

        if(offset < 0)
        {
            //MessageBox(0, "Out of bounds in Put2Bytes", "error", MB_OK); 

            target->error = 1;

            return 0;
        }    
    
        target->contents[offset] = (char) low;
        target->contents[offset + 1] = (char) high;        

        return 1;
    }

// ===============================================================

    int Put2Bytes(bufPtr target, u32 x, u32 y, u32 value)
    {
        if(target)
            return Put2Bytes(target,  (x + (y * target->width)) * target->unit, value);
        else
            return -1;
    }

// ===============================================================

    int Put3Bytes(bufPtr target, int offset, int value)
    {
        if(target->contents == NULL)
            return 0;

        int checker = offset + 3;
        int low = value & 0xFF;
        int high = (value & 0xFF00) >> 0x08;
        int bank = (value & 0xFF0000) >> 0x10;

        if( (unsigned int) checker > target->length)
        {
            //MessageBox(0, "Out of bounds in Put3Bytes", "error", MB_OK); 
        
            target->error = 1;

            return 0;
        }

        if(offset < 0)
        {
            //MessageBox(0, "Out of bounds in Put3Bytes", "error", MB_OK); 

            target->error = 1;

            return 0;
        }    
    
        target->contents[offset] = (char) low;
        target->contents[offset + 1] = (char) high;
        target->contents[offset + 2] = (char) bank;

        return 1;
    }

// ===============================================================

    int Put3Bytes(bufPtr target, u32 x, u32 y, u32 value)
    {
        if(target)
            return Put3Bytes(target,  (x + (y * target->width)) * target->unit, value);
        else
            return -1;
    }

// ===============================================================

    int Put4Bytes(bufPtr target, int offset, int value)
    {
        if(target->contents == NULL)
            return 0;

        int checker = offset + 4;
        int byte1 = value & 0xFF;
        int byte2 = (value & 0xFF00) >> 0x08;
        int byte3 = (value & 0xFF0000) >> 0x10;
        int byte4 = (value & 0xFF000000) >> 0x18;

        if( (unsigned int) checker > target->length)
        {
            //MessageBox(0, "Out of bounds in Put3Bytes", "error", MB_OK); 
        
            target->error = 1;

            return 0;
        }

        if(offset < 0)
        {
            //MessageBox(0, "Out of bounds in Put4Bytes", "error", MB_OK); 

            target->error = 1;

            return 0;
        }    
    
        target->contents[offset] = (char) byte1;
        target->contents[offset + 1] = (char) byte2;
        target->contents[offset + 2] = (char) byte3;
        target->contents[offset + 3] = (char) byte4;

        return 1;
    }

// ===============================================================

    int Put4Bytes(bufPtr target, u32 x, u32 y, u32 value)
    {
        if(target)
            return Put4Bytes(target,  (x + (y * target->width)) * target->unit, value);
        else
            return -1;
    }

// ===============================================================

    int CopyBuffer(bufPtr target, bufPtr src, u32 numBytes)
    {
        if(!CopyBuffer(target, src, target->wOffset, 0, numBytes))
            return 0;

        if(target)
            target->wOffset += numBytes;

        return 1;
    }

// ===============================================================

    int CopyBuffer(bufPtr target, bufPtr src, unsigned int tOffset, unsigned int sOffset, unsigned int numBytes)
    {
        char errorBuf[256];

        if(target->contents == NULL || src->contents == NULL)
            MessageBox(0, "trouble!", "argh", MB_OK);

        if( (tOffset + numBytes) > target->length )
        {
            //MessageBox(0, "error in Copy Buffer", "target", MB_OK);

            target->error = 1; 
        
            return 0;
        }

        if( (sOffset + numBytes) > src->length )
        {
            itoa( (sOffset + numBytes) - src->length, errorBuf, 16);

            //MessageBox(0, "error in Copy Buffer", "source", MB_OK);
        
            src->error = 1; 
        
            return 0;
        }

        memcpy(target->contents + tOffset, src->contents + sOffset, numBytes);

        return 1;
    }

// ===============================================================

    /* block copy version of CopyBuffer() */

    int CopyBuffer(bufPtr target, bufPtr src, u32 xTarg, u32 yTarg, u32 xSrc, u32 ySrc, u32 width, u32 height)
    {
        u32 i = 0, j = 0, result = 0;

        if(target->unit != src->unit)
            return 0;

        for(i = 0; i < width; ++i)
        {
            for(j = 0; j < height; ++j)
            {
                result = CopyBuffer(target,
                                    src, 
                                    (xTarg + (yTarg * target->width)) * target->unit,
                                    (xSrc + (ySrc * src->width)) * src->unit,
                                    src->unit);

                if(!result)
                    return 0;
            }
        }

        return 1;
    }

// ===============================================================

    // The WB stands for "write back" meaning it will update the pointers automatically
    // After writing them
    int CopyBufferWB(bufPtr target, bufPtr src, unsigned int *tPtr, unsigned  int *sPtr, unsigned int numBytes)
    {
        if(!CopyBuffer(target, src, *tPtr, *sPtr, numBytes))
        {
            *tPtr += numBytes;
            *sPtr += numBytes;

            return 0;
        }   
        else
        {
            *tPtr += numBytes;
            *sPtr += numBytes;
       
            return 1;
        }

        // dummy return. The if/else is to improve readability
        return 1;
    }

// ===============================================================

    int ZeroBuffer(bufPtr arg)
    {
        if(arg->contents == 0)
            return 0;

        ZeroMemory(arg->contents, arg->length);

        return 1;
    }

// ===============================================================

    bool CompareBuffer(bufPtr a, bufPtr b)
    {
        // just a wrapper around memcmp really

        if(!a || !b)
        {
            MessageBox(0, "Error in CompareBuffer", 0, MB_OK);
            return false;
        }

        if(a->length != b->length)
            return false;
    
        // memcmp returns 0 if the memory is the same, so I'm returning
        // the boolean version of that. (true if they are the same)
        return memcmp(a->contents, b->contents, a->length) ? false : true; 
    }

// ===============================================================

    unsigned int GetBank(unsigned int cpuAddr)
    {
        // assumes the value is a Cpu Address

        // precaution
        cpuAddr &= 0xFFFFFF;
        cpuAddr >>= 0x10;
        cpuAddr &= 0x000000FF;

        return cpuAddr;

    }

// ===============================================================

    int IsBankChange(u32 cpuAddr, int size)
    {
        if(GetBank(cpuAddr) != GetBank(cpuAddr + size - 1) )
            return 1;

        return 0;
    }

// ===============================================================

    int AdvancePointer(zgPtr game, u32 *offset, u32 size)
    {
        u8 oldBank = 0;
        u8 newBank = 0;
        u8 maxBank = 0;
        u32 oldOffset = 0;

        // =======================
        
        // invalid pointer
        if(!offset || !game)
            return 0;

        oldOffset = (*offset);

        if(IsBankChange(oldOffset, size))
        {
            oldBank = GetBank(oldOffset);
            newBank = oldBank + 1;
            maxBank = GetBank(RomToCpuAddr(game->image->length)) - 1;

            // No more room left in the rom!
            if(newBank > maxBank)
            {
                // should throw an exception here....
                return 0;
            }

            // Moves the address into the start of the next bank.
            oldOffset = (newBank << 0x10) | 0x8000;
        }
        
        (*offset) = oldOffset + size;

        if(!((*offset) & 0x8000))
            (*offset) = (GetBank(*offset) << 0x10) | 0x8000;

        return oldOffset;
    }

// ===============================================================

    u32 hextodec(u8 *input, u32 length)
    {
        u32 result = 0;
	    u32 value;
        u32 ceiling = strlen((char*) input) - 1;
	    u32 power16 = 0;

        // ceiling is intepreted as the highest number of digits - 1 to handle
        // j is a counter through the array that starts at the end
        // (from our perspective us the lowest place)
        // length is the largest number of digits we want back
        // since the return value is an int, we can't have
        // length be any longer than 8
        // but with that leaves the question of... are there higher
        // digits that would add to the value and cause it to overflow
        // if that is the case I'm going to return 0xFFFFFFFF

        length = length > 8 ? 8 : length - 1;

        length = ceiling < length ? ceiling : length;

	    int j = ceiling;    

	    for( ;  j >= 0; j--)
	    {       
		    if(input[j] >= 'A' && input[j] <= 'F')
		    {
			    value = input[j] - 'F';
			    value += 15;
		    }
   		    else if(input[j] >= 'a' && input[j] <= 'f')
		    {
			    value = input[j] - 'f';
			    value += 15;
		    }
		    else if(input[j] >= '0' && input[j] <= '9')
		    {
			    value = input[j] - '9';
			    value += 9;
		    }
            else
                value = 0;

            if(j >= (ceiling - length))
            {
                result += (value << (4 * power16));
		        power16++;

                continue;
            }


            if(length != 7)
                break;

            // at this point the useful data has expired
            // we're just looking for overflow 
            if(value)
            {
                result = 0xFFFFFFFF;
                break;
            }


	    }
	    
	    return result;
    }

// ===============================================================

    void makeHexStr(bufPtr input, u32 value)
    {
        char temp[8];

        u32 a = min(sizeof(temp), input->length);

        value &= 0xFFFFFFFF;

        itoa(value, temp, 0x10);
        strupr(temp);

        strncpy((char*) input->contents, temp, a);
    }

// ===============================================================

    unsigned char* ToString(bufPtr input)
    {
        return input->contents;
    }

// ===============================================================

    bufPtr FromString(char *contents, int length)
    {
        bufPtr newBuf = CreateBuffer(length);

        if(newBuf)
            memcpy(newBuf->contents, contents, length);

        return newBuf;
    }

// ===============================================================

    bufPtr FromString(char *contents, int width, int height, int unit)
    {
        bufPtr newBuf =  CreateBuffer(width, height, unit);

        if(newBuf)
            memcpy(newBuf->contents, contents, (width * height * unit) );
       
        return newBuf;
    }

// ===============================================================

    bool FromFile(bufPtr target, char* pathName, u32 length, u32 offset)
    {
        bool success = true;

        u32 fileLength = 0;

        FILE* f = NULL;

        // --------------------------

        if((!target) || (target->length == 0))
            return false;

        f = fopen(pathName, "rb");

        if(!f)
            return false;

        fseek(f, 0, SEEK_END);

        fileLength  = ftell(f);

        fseek(f, 0, SEEK_SET);

        if(fileLength == target->length)
            fread(target->contents + offset, fileLength, 1, f);
        else
            success = false;

        fclose(f);
    
        return success;
    }

// ===============================================================

    bool ToFile(bufPtr source, char* pathName, u32 length, u32 offset)
    {
        FILE* f = NULL;

        // --------------------------

        if((!source) || (source->length == 0))
            return false;

        f = fopen(pathName, "wb");

        if(!f)
            return false;

        if(length)
        {
            // set to safe defaults if the length and offset values don't make sense
            if( (length + offset) > source->length)
            {
                length = source->length;
                offset = 0;
            }            

            fwrite(source->contents + offset, length, 1, f);
        }
        else
            fwrite(source->contents, source->length, 1, f);

        fclose(f);
    
        return true;
    }

// ===============================================================

    bufPtr GetSubBuffer(bufPtr source, u32 offset, u32 newLength)
    {
        bufPtr subBuffer = AllocBuffer();

        if(source->contents == NULL)
            return NULL;

        if(offset > source->length - 1)
            return NULL;

        if(offset + newLength > source->length)
            return NULL;

        subBuffer->contents = &(source->contents[offset]);
        subBuffer->error = 0;
        subBuffer->unit = 1;
        subBuffer->wOffset = 0;

        if(newLength)
            subBuffer->length = newLength;
        else
            subBuffer->length = (source->length - offset);

        return subBuffer; 
    }

// ===============================================================

    int GetHexEditNum(HWND win)
    {
        char dummy[256];
    
        GetWindowText(win, dummy, 256);
    
        return hextodec((u8*) dummy, strlen(dummy));
    }

// ===============================================================