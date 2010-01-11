    // Core functions of the editor. Moves data, manipulates data, etc. nuff said.
    // MathOnNapkins Copyright 2005

    #include "Globals.h"
    #include "Strings.h"
    #include "Core.h"
    #include "Black Magic.h"
    #include "Objects.h"

// ===============================================================

    int CpuToRomAddr(int holderLong)
    {
        // converts CPU address to ROM addresses (without a copier header)

        holderLong &= 0x7FFFFF;

        /*
        if((holderLong & 0x8000) == 0)
            MessageBox(0,"Illegal Jump Address detected.","eek",MB_OK);
        */


        holderLong = ( (holderLong & 0x7F0000) >> 1 ) + (holderLong & 0x7FFF);
	    

        return holderLong;
    }

// ===============================================================

    int RomToCpuAddr(int holderLong)
    {
        // converts a ROM Address to a CPU Address
        
        holderLong &= 0x7FFFFF;
        
        holderLong = ( (holderLong & 0x3F8000) << 1 ) + ( holderLong & 0x7FFF ) | 0x8000;
        
        return holderLong;
    }

// ===============================================================

    void Create_BM_Header(zgPtr game)
    {
        int newLength = 0x200000;
        int inRomHeaderOffset = 0x109000;

        u32 a = 0;
    
        bufPtr temp = NULL;
        bufPtr rom  = game->image;

        BM_Header *h = &game->bm_Header;
        
        // --------------------------------------------

        // initialize the header's memory entirely
        ZeroMemory(h, sizeof(BM_Header));
        
        h->header_version = current_header_version;

        strcpy(h->designation, "BM_HEADER");

        h->dngHeaderOffset  = Get3Bytes(rom, asm_header_ref);
        
        h->dngHeaderBank    = GetByte(rom, asm_header_bank);
        
        h->dngObjOffset     = Get3Bytes(rom, asm_object_ref);
        
        h->dngLayoutOffset  = Get3Bytes(rom, asm_layout_ref);
        
        h->dngEntranceOffset = asm_entrance_ref;
        
        h->dngChestOffset   = Get3Bytes(rom, asm_chest_ref);

        h->dngNumChests     = Get2Bytes(rom, asm_num_chests) / 3;

        h->dngSpriteOffset  = Get2Bytes(rom, asm_sprite_ref);

        h->map32To16UL      = Get3Bytes(rom, asm_map32To16UL);
        h->map32To16UR      = Get3Bytes(rom, asm_map32To16UR);
        h->map32To16LL      = Get3Bytes(rom, asm_map32To16LL);
        h->map32To16LR      = Get3Bytes(rom, asm_map32To16LR);
        
        // Check to see if there is a hooked JML to extended regions
        // If there is a hook, this will be the offset in-rom for the bank table
        if(GetByte(rom, asm_sprite_hook) == 0x5C)
            h->dngSpriteBanks = CpuToRomAddr(Get3Bytes(rom, asm_sprite_hook + 1)) + 0x0E;
        // In this case there isn't ;) And in that case the sprite data is all in bank 09
        else
            h->dngSpriteBanks = 0x090000;

        h->overHoleOffset   = 0x1BB800;
        h->overNumHoles     = 0x13;

        h->overEntrOffset   = 0x1BB96F;
        h->overNumEntr      = 0x81;
        
        h->overSprOffset0   = 0x9C881;
        h->overSprOffset2   = 0x9C901;
        h->overSprOffset3   = 0x9CA21;

        // default location of this data.
        h->overItemOffset   = 0x1BC2F9;

        // initialize default information about the structure of the graphics data
        h->sprGfxOffset     = Get2Bytes(rom, asm_spr_gfx_ptr);
        h->bgGfxOffset      = Get2Bytes(rom, asm_bg_gfx_ptr);
    
        // Expand the rom to 2 megabytes if the initial size is 1 megabyte or less
        if(game->image->length <= 0x100000)
            ExpandRom(&newLength, 1);

        temp = FromString( (char*) &(game->bm_Header), sizeof(BM_Header) );

        CopyBuffer(rom, temp, inRomHeaderOffset, 0, temp->length);
        Put3Bytes(rom, bm_header_loc, inRomHeaderOffset);
    }

// ===============================================================

    void Check_BM_Header(zgPtr game)
    {
        Buffer temp;

        // This is the alleged header offset.
        u32 offset = Get3Bytes(game->image, bm_header_loc);

        if( offset > (game->image->length - sizeof(BM_Header) ) )
        {
            Create_BM_Header(game);
            return;
        }

        InitBuffer(12, &temp);
        CopyBuffer(&temp, game->image, 0, offset, 12);
        
        if(strcmp((const char*) temp.contents, "BM_HEADER") == 0)
        {
            // Add version support for headers! soon!
            //MessageBox(0, "header found", ":)", MB_OK);
        }
        else
        {
            Create_BM_Header(game);
            DestroyBuffer(&temp);
            return;
        }

        DestroyBuffer(&temp);

        temp.contents = (unsigned char*) &(game->bm_Header);
        temp.length = sizeof(BM_Header);

        CopyBuffer(&temp, game->image, 0, offset, temp.length);
        
    }

// ===============================================================

    bufPtr DecompressOther(bufPtr inputBuf, int source)
    {
        // deprecated!!!!
        // Seems to take type one or type 
        // two pointers in the lower spectrum. 

        int x_reg = 0x0E;
        u32 y_reg = 0x40;
        u32 a_reg = 0;
        u32 target = 0;
        u8 infoArray[0x10];
        ZeroMemory(infoArray, 0x10);

        int dummy = 0;

        bufPtr storeBuf = CreateBuffer(0x1000);

        y_reg = 0x40;

    nextSegment:

        x_reg = 0x0E;

    loop1:

        a_reg = Get2Bytes(inputBuf, source); // lda [$00]
        
        Put2Bytes(storeBuf, target, a_reg);
        target += 2; // sta $2118
        
        a_reg = XBA(a_reg); // xba

        a_reg |= Get2Bytes(inputBuf, source); // ora [$00]
        a_reg &= 0xFF; // and #$00ff
        infoArray[x_reg]   = (u8) a_reg; // sta $bf, x
        infoArray[x_reg+1] = (u8) (a_reg >> 8);

        source += 2; x_reg -= 2; // inc $00, inc $00, x--, x--;

        a_reg = Get2Bytes(inputBuf, source); // lda [$00]
        
        Put2Bytes(storeBuf, target, a_reg);
        target += 2; // sta $2118
        
        a_reg = XBA(a_reg); // xba

        a_reg |= Get2Bytes(inputBuf, source); // ora [$00]
        a_reg &= 0xFF; // and #$00ff
        infoArray[x_reg]   = (u8) a_reg; // sta $bf, x
        infoArray[x_reg+1] = (u8) (a_reg >> 8);

        source += 2; x_reg -= 2; // inc $00, inc $00, x--, x--;

        if(x_reg >= 0)
            goto loop1; // bpl loop1
        
        x_reg = 0x0E;

    loop2:

        a_reg = Get2Bytes(inputBuf, source);
        a_reg &= 0x00FF;
        dummy = a_reg;
        
        a_reg |= (infoArray[x_reg] | (infoArray[x_reg+1] << 8));
        a_reg = XBA(a_reg);
        a_reg |= dummy;

        Put2Bytes(storeBuf, target, a_reg);
        target += 2;

        source++; x_reg -= 2;

        a_reg = Get2Bytes(inputBuf, source);
        a_reg &= 0x00FF;
        dummy = a_reg;
        
        a_reg |= (infoArray[x_reg] | (infoArray[x_reg+1] << 8));
        a_reg = XBA(a_reg);
        a_reg |= dummy;

        Put2Bytes(storeBuf, target, a_reg);
        target += 2;

        source++; x_reg -= 2;

        if(x_reg >= 0)
            goto loop2;

        y_reg--;

        if(y_reg > 0)
            goto nextSegment;

        ResizeBuffer(storeBuf, target);

        return storeBuf;
    }

// ===============================================================

    int XBA(int value)
    {
        int temp = (value >> 0x08) & 0xFF;

        value <<= 0x08;
        value &= 0xFF00;

        value |= temp;

        return value;
    }

// ===============================================================

    // Import/Export routines

    void ImportDungeons(zgPtr game, HINSTANCE prog)
    {

        DialogBoxParam(prog,
                       MAKEINTRESOURCE(IDD_PORTDUNG),
                       game->subWindow,
                       PortDungProc,
                       IMPORT_MODE);

        // 1. Load dungeon file.
        // Bring up a dialog similar to how the export dialog works.


        // 2. Load dungeon headers
    
    

        // 3. Load dungeon object data
        // 4. Load dungeon entrance data
        // 5. Assemble them to a free location in expanded rom.
    }

// ===============================================================

    void ExportDungeons(zgPtr game, HINSTANCE prog)
    {
        // Bring up a dialog requesting which entrances and rooms' data will be extracted.
        // 

        // Dump all this info plus any parsable strings into a header portion. The header will contain
        // A pointer to the data in the rest of the file. The header will itself start off with a 32bit value
        // of how long it is.


        DialogBoxParam(prog,
                       MAKEINTRESOURCE(IDD_PORTDUNG),
                       game->subWindow,
                       PortDungProc,
                       EXPORT_MODE);

        return;
    }

// ===============================================================

    int CALLBACK PortDungProc(HWND dlg, UINT msg, WPARAM wp, LPARAM lp)
    {
        static int mode = 0;
        bool test = 0;

        int headerLoc = 0;
        int roomNum = 0;
    
        zgPtr game = (zgPtr) GetWindowLong(windowList[zgIndex], 0);

        switch(msg)
        {
            case WM_INITDIALOG:
                        
                mode = lp; // modes are 0 or 1
            
                mode ? SetWindowText(dlg, "Import Dungeons") :
                       SetWindowText(dlg, "Export Dungeons");

                break;

            case WM_COMMAND:

                if(wp == IDC_PORTDUNG)
                {
                    if(mode == EXPORT_MODE)
                    {
                        ExportDungeonFile(game);

                        EndDialog(dlg, -1);               
                    }
                    else
                    {
                        ImportDungeonFile(game);

                        EndDialog(dlg, -1);
                    }
                }

                if(wp == IDCANCEL)
                {
                    EndDialog(dlg, -1);
                }

                break;

            case 0:

                break;




            default:

                break;

        }

        return 0; // do the default handling anyways.
    }

// ===============================================================

    void LoadDungeonFile(zgPtr game, DungeonFile* file)
    {
        unsigned int headerBase = CpuToRomAddr(game->bm_Header.dngHeaderOffset);
        unsigned int headerBank = (game->bm_Header.dngHeaderBank) << 0x10;
        unsigned int objectBase = CpuToRomAddr(game->bm_Header.dngObjOffset);
        unsigned int layoutBase = CpuToRomAddr(game->bm_Header.dngLayoutOffset);

        unsigned int entranceBase = game->bm_Header.dngEntranceOffset; 
        // This is the default entrance data location
        // The plan is to later maybe expand entrance data but for now there's enough.

        unsigned int chestLocationBase = game->bm_Header.dngChestOffset;

        // Also remember to change not only $E9FB, but $EA0C, and $EC10
        // Also, $EBF6 is the max number of chests * 3 from the game's perspective.

        u32 spritePtrBase = game->bm_Header.dngSpriteOffset;
        u32 currentPtr = 0;
        u32 spritePtrBank = game->bm_Header.dngSpriteBanks;
        u32 spritePtrTbl  = GetBank(spritePtrBank) << 0x10;

        u32 roomNum = 0;
        u32 roomOffset = 0;
        u32 nextRoomOffset = 0;
        u32 size = 0;
        u32 objectSize = 0;
        u32 index = 0;
        u32 index2 = 0;
        u32 sum = 0;
        u32 headerLength = 0;
        u32 i;

        bufPtr      temp1;
        bufPtr      temp3;
    
        // Note that not the whole buffer will be filled up in all cases
        InitBuffer(320*14, &(file->headerData) );

        /*
        if(_CrtCheckMemory() == FALSE)
            MessageBox(0, "crt failure", "write headers", MB_OK);

        */
  
        // 1. write the headers to the file structure
        for(roomNum = 0; roomNum < 320 ; roomNum++)
        {
            roomOffset = Get2Bytes(game->image, headerBase + (roomNum * 2));

            file->headerLengths[roomNum] = 0x0E;

            for(i = 0; i < 320; i++)
            {
                if(i == roomNum)
                    continue;

                nextRoomOffset = Get2Bytes(game->image, headerBase + (i*2));
                headerLength = nextRoomOffset - roomOffset;

                if(!headerLength)
                    continue;

                if(headerLength == 5)
                    headerLength = 5;

                if( (headerLength) < 0x0E )
                {
                    file->headerLengths[roomNum] = (headerLength);
                    break;
                }
            }
        }

        // 1b. write the headers with the given size.
        for(roomNum = 0 ; roomNum < 320 ; roomNum++)
        {
            roomOffset = Get2Bytes(game->image, headerBase + (roomNum * 2)) | headerBank;
        
            roomOffset = CpuToRomAddr(roomOffset);

            CopyBuffer(&(file->headerData),
                       game->image,
                       roomNum*14,
                       roomOffset,
                       file->headerLengths[roomNum]);
        }
   
        unsigned int layer = 0; // Overlooked this the first time I implemented object handling
        // Unfortunately this proved to be a catastrophic mistake as you'd get rooms with objects from
        // three different rooms upon importing. As you can tell from a cursory glance at 
        // HM's dungeon editor there are 3 layers of objects. >_> d'oh. I only originally looked
        // for one.

        // 2. write the dungeon object data to the file structure
        for(roomNum = 0; roomNum < 320 ; roomNum++)
        {   
            // provides a cpu address for this particular room's data
            roomOffset = Get3Bytes(game->image, objectBase + (roomNum * 3));

            // convert that to an in-rom address.
            roomOffset = CpuToRomAddr(roomOffset);

            // the temp1 pointer will be used to shorten the size of the code
            // Basically it saves from having to write out the whole name of the particular
            // bufPtr and all its modifications like -> and ., etc.
            temp1 = &(file->objectData[roomNum]);
        
            // index 2 is used as placeholder so we remember where the data started at.
            index2 = roomOffset; 

            // start off with the size of the object data being 2 bytes long
            // This accounts for the two properties entries
            size = 2;
            roomOffset += 2;
            layer = 0;

    nextLayer:

            // we start by reading 3 byte objects
            objectSize = 3;
        
            // next determine the size (length) of this object data before we copy the data to a buffer.
            while(1)
            {
                // minor precaution to avoid crashes
                if(size > 0xFFFF)
                {
                    exit(-1); // might comment out if I start experiencing crashes
                }

                // 0xFFFF is the terminator for each room's object data.
                if(Get2Bytes(game->image, roomOffset) == 0xFFFF)
                {
                    // since roomOffset double functions as size, we increment by 2.
                    roomOffset += 2;
                    size += 2;

                    // and end the loop
                    goto end;
                }

                // check to see if we are going to switch to door objects
                // which are two byte objects rather than three byte
                // not all rooms have them >_>
                if(Get2Bytes(game->image, roomOffset) == 0xFFF0)
                {
                    if(objectSize == 3)
                    {   
                        objectSize = 2;
                        roomOffset += 2;
                        size += 2;
                        continue;
                    }
                }            

                roomOffset += objectSize;
                size += objectSize;
            }

        end:

            layer++;

            // Layers range from 0 to 2
            if(layer <= 2)
                goto nextLayer;

            InitBuffer(size, temp1);

            CopyBuffer(temp1,
                       game->image,
                       0,
                       index2,
                       size);

        
        }

        // 2.5 (inserted) layout data which is similar to object data but easier :)
        //if(layoutBase == 0x26F2F)
            index = 8;
       // else
            //index = 64;

        for( i = 0; i < index; i++)
        {
            roomOffset = Get3Bytes(game->image, layoutBase + (i*3));
            roomOffset = CpuToRomAddr(roomOffset);

            size = 0;
            index2 = roomOffset;
            objectSize = 3;

            temp1 = &(file->layoutData[i]);

            while(1)
            {
                if(size > 0xFFFF)
                    exit(-1);

                if(Get2Bytes(game->image, roomOffset) == 0xFFFF)
                {
                    roomOffset += 2;
                    size += 2;

                    goto doneLayout;
                }

                if(Get2Bytes(game->image, roomOffset) == 0xFFF0)
                {
                    if(objectSize == 3)
                    {   
                        objectSize = 2;
                        roomOffset += 2;
                        size += 2;
                        continue;
                    }
                }            

                roomOffset += objectSize;
                size += objectSize;
            }

    doneLayout:

            InitBuffer(size, temp1);

            CopyBuffer(temp1,
                       game->image,
                       0,
                       index2,
                       size);
        }
    


        // 3. next handle the entrance data
        InitBuffer( game->numEntrances * ENTRANCE_BASE_SIZE,
                    &(file->entrances.entranceBuffer)
                    );

            temp3 = &(file->entrances.roomNumber);

            index = 0;
            sum = 0;

            // Set up the structure of the entrance data in its underlying buffers
            while(1)
            {
                temp3[index].unit = entranceBufSizes[index];
                temp3[index].error = 0;
                temp3[index].length = ( game->numEntrances * entranceBufSizes[index] );
                temp3[index].contents = (file->entrances.entranceBuffer.contents) + sum;

                sum += temp3[index].length;

                if( &(temp3[index]) == &(file->entrances.musicValue) )
                    break;

                if( sum >= file->entrances.entranceBuffer.length )
                    break;

                index++;
            }

            // determine where the entrance data resides.
                // insert this procedure later b/c we're not going to fuck with it now.

        index = 0;

        temp3 = &(file->entrances.entranceBuffer);

        CopyBuffer(temp3, game->image, 0, entranceBase, temp3->length);

        // 4. Handle the chest location data.
        temp3 = &(file->chestLocationData);

        InitBuffer( game->bm_Header.dngNumChests * 3, temp3);

        index = 0;

        // Retrieve the rom address of this data.
        chestLocationBase = CpuToRomAddr(chestLocationBase);

        CopyBuffer(temp3,
                   game->image,
                   0,
                   chestLocationBase,
                   temp3->length);
    
        //MessageBox(0, "sprites!", "sprites!", MB_OK);

        // 5. Finally go for the sprite data.
    
        if(spritePtrBank == 0x090000)
            spritePtrBase |= spritePtrBank;
        else
            spritePtrBase = RomToCpuAddr(spritePtrBase);

        for( index = 0 ; index < 0x140; index++)
        {
            // This will provide the bank the data is stored in.
            // Do not confuse this with spritePtrTbl, which says which bank
            // the sprite pointer table is in. The latter never changes
            if(spritePtrBank != 0x090000)
            {   
                spritePtrBank = GetByte(game->image,
                                        game->bm_Header.dngSpriteBanks + index);
                spritePtrBank <<= 0x10;            
            }

            // Get the rom address of the current room's sprites
            currentPtr = Get2Bytes( game->image, asm_sprite_local + (index*2) );
            currentPtr |= spritePtrBank;
            currentPtr = CpuToRomAddr(currentPtr);
       
            temp3 = &(file->spriteData[index]);

            // temporarily save the current position for use later
            index2 = currentPtr;

            // account for the fact that the first byte is always there.
            currentPtr++;

            // The size check is kind of arbirtary. Just to make sure
            // our data doesn't go crazy.
            for( size = 1 ; size < 0xFFFF ; currentPtr += 3)
            {
                if( GetByte(game->image, currentPtr) == 0x000000FF)
                {  
                   size++;
                   currentPtr++;

                   break;
                }

                size += 3;
            }
        
            InitBuffer(size, temp3);

            // retrieve the start of this room's sprite data
            currentPtr = index2;

            CopyBuffer(temp3, 
                       game->image,
                       0,
                       currentPtr,
                       temp3->length);
        }

        // Need to insert secrets (part 6 :( )

        // End of LoadDungeonFile( )

    }

// ===============================================================

    void ExportDungeonFile(zgPtr game)
    {
        // first, set up the stuff to save the file

        Buffer exportName;
        DungeonFile* file = &(game->dungeonFile);
        bufPtr temp3;

        unsigned int index, index2 = 0;

        InitBuffer(game->romName->length, &exportName);

        CopyBuffer(&exportName,
                   game->romName,
                   0,
                   0,
                   exportName.length);

        // change the extension to *.dng
        index = strlen((const char*) exportName.contents) - 3;
        PutByte(&exportName, index++, 'd');
        PutByte(&exportName, index++, 'n');
        PutByte(&exportName, index++, 'g');

        bmOFN.lStructSize = sizeof(OPENFILENAME);
	    bmOFN.hwndOwner = NULL;
	    bmOFN.hInstance = thisProg;
	    bmOFN.lpstrFilter = NULL;
	    bmOFN.lpstrCustomFilter = NULL;
	    bmOFN.nFilterIndex = 0;
	    bmOFN.nMaxFile = MAX_PATH;
        bmOFN.lpstrFile = (LPSTR) exportName.contents;
	    //bmOFN.lpstrFile[0] = 0; // a particularity of using the open file dialogs.
	    bmOFN.lpstrFileTitle = NULL;
	    bmOFN.lpstrInitialDir = 0; // Sets to a default directory. (my documents?)
	    bmOFN.lpstrTitle = "Save Dungeon Data";
	    bmOFN.Flags = OFN_OVERWRITEPROMPT;
        bmOFN.lpstrDefExt = "dng";

        if(GetSaveFileName(&bmOFN))

        game->portFileHandle = CreateFile(bmOFN.lpstrFile,
                               GENERIC_WRITE,
                               0,
                               0,
                               OPEN_ALWAYS,
                               0,
                               0);

        index = 0;

        unsigned long numBytesWritten;

        temp3 = &(file->headerData);

        index2 = temp3->length;

        // write the header data to file
        WriteFile(game->portFileHandle,
                  (char*)(&index2),
                  4,
                  &numBytesWritten,
                  0);

        WriteFile(game->portFileHandle,
                  temp3->contents,
                  index2,
                  &numBytesWritten,
                  0);

        // write the object data to file.
        for(index = 0, index2 = 0 ; index < 0x140; index++)
            index2 += file->objectData[index].length;

        WriteFile(game->portFileHandle,
                  (char*)(&index2),
                  4,
                  &numBytesWritten,
                  0);    

        for(index = 0 ; index < 0x140; index++)
        {
            temp3 = &(file->objectData[index]);

            WriteFile(game->portFileHandle,
                      temp3->contents,
                      temp3->length,
                      &numBytesWritten,
                      0);
        }

        // write the entrance data to file

        temp3 = &(file->entrances.entranceBuffer);

        index2 = temp3->length;

        WriteFile(game->portFileHandle,
                  (char*)(&index2),
                  4,
                  &numBytesWritten,
                  0);

        WriteFile(game->portFileHandle,
                      temp3->contents,
                      temp3->length,
                      &numBytesWritten,
                      0);

        // write the chest data to file
        temp3 = &(file->chestLocationData);

        index2 = temp3->length;

        WriteFile(game->portFileHandle,
                  (char*)(&index2),
                  4,
                  &numBytesWritten,
                  0);

        WriteFile(game->portFileHandle,
                  temp3->contents,
                  temp3->length,
                  &numBytesWritten,
                  0);

        //write the sprite data to file
        for(index = 0, index2 = 0 ; index < 0x140; index++)
            index2 += file->spriteData[index].length;

        WriteFile(game->portFileHandle,
                  (char*)(&index2),
                  4,
                  &numBytesWritten,
                  0);    

        for(index = 0 ; index < 0x140; index++)
        {
            temp3 = &(file->spriteData[index]);

            WriteFile(game->portFileHandle,
                      temp3->contents,
                      temp3->length,
                      &numBytesWritten,
                      0);
        }
    
        SetEndOfFile(game->portFileHandle);
        CloseHandle(game->portFileHandle);

        DestroyBuffer(&exportName);

        return;
    }

// ===============================================================

    void ImportDungeonFile(zgPtr game)
    {
        Buffer importName;
        Buffer importBuffer;

        bufPtr temp1;
        bufPtr temp2;

        unsigned int bufLength;
        unsigned int offset;
        unsigned int nextBlock;

        unsigned int loopIndex;
        unsigned int loopIndex2;
    
        unsigned int spriteOffset = 0;

        unsigned int layer = 0;

        unsigned long dummy; // just used for the ReadFile() call

        // declare some extra needed variables
        unsigned int objectSize;
        unsigned int objectOffset;
        unsigned int blockSize = 0;
    
        char errorBuf[256];


        DungeonFile* file;

        file = &(game->portFile);

        InitBuffer(MAX_PATH, &importName);

        bmOFN.lStructSize = sizeof(OPENFILENAME);
	    bmOFN.hwndOwner = NULL;
	    bmOFN.hInstance = thisProg;
	    bmOFN.lpstrFilter = "Dungeon Files\0*.dng\0All files\0*.*\0";
	    bmOFN.lpstrCustomFilter = NULL;
	    bmOFN.nFilterIndex = 0;
	    bmOFN.nMaxFile = MAX_PATH;
        bmOFN.lpstrFile = (LPSTR) importName.contents;
	    bmOFN.lpstrFile[0] = 0; // a particularity of using the open file dialogs.
	    bmOFN.lpstrFileTitle = NULL;
	    bmOFN.lpstrInitialDir = 0; // Sets to a default directory. (my documents?)
	    bmOFN.lpstrTitle = "Load Dungeon Data";
	    bmOFN.Flags = OFN_FILEMUSTEXIST;
        bmOFN.lpstrDefExt = "dng";

        if( !GetOpenFileName(&bmOFN) ) 
	    {
            // if opening the file failed.
		    char* buffer = 0;

		    DWORD i = CommDlgExtendedError();

		    if(i)
		    {
			    wsprintf(buffer,"GUI error. Error: %08X",i);
			    MessageBox(0,buffer,"Bad error happened",MB_OK);
		    }

            return;
	    }

        game->portFileHandle = CreateFile(bmOFN.lpstrFile,
                                          GENERIC_READ,
                                          FILE_SHARE_READ,
                                          0,
                                          OPEN_EXISTING,
                                          FILE_ATTRIBUTE_NORMAL,
                                          0);


        InitBuffer(GetFileSize(game->portFileHandle,0),
                   &importBuffer
                   );

        ReadFile(game->portFileHandle,
                 importBuffer.contents,
                 importBuffer.length,
                 &dummy,
                 0);

        CloseHandle(game->portFileHandle);

        // Do some file diagnostics to see if it's the proper size and structure.
        importBuffer.error = 0;

        offset = 0;
        temp1 = &importBuffer;

        // check the header data length
        bufLength = Get4Bytes(temp1, 0);
        offset += (bufLength + 4);

        // check the length of the object data
        bufLength = Get4Bytes(temp1, offset);
        offset += (bufLength + 4);

        // check the entrance data length
        bufLength = Get4Bytes(temp1, offset);
        offset += (bufLength + 4);

        // check the chest data length
        bufLength = Get4Bytes(temp1, offset);
        offset += (bufLength + 4);

        // check the sprite data length
        bufLength = Get4Bytes(temp1, offset);
        offset += (bufLength + 4);
   

        if(offset != importBuffer.length || temp1->error == 1)
        {
            MessageBox(0, "Incorrect file format!", "Not a .dng file", MB_OK);
        
            goto cleanup;
        }

        // Otherwise we're in the clear.

        bufLength = offset = 0;
        nextBlock = offset + 4;
        temp2 = &(file->headerData);

        // See how long the header data is for real now.
        bufLength = Get4Bytes(temp1, 0);
        offset += (bufLength + 4);

        InitBuffer(bufLength, temp2);
        CopyBuffer(temp2, temp1, 0, nextBlock, bufLength); 

        // next load up the object data
        nextBlock = offset + 4; // the next actual data starts after its length indicator.
   
        objectOffset = nextBlock;

        bufLength = Get4Bytes(temp1, offset);
        offset += (bufLength + 4);

        layer = 0;

        for(loopIndex = 0; loopIndex < 320; loopIndex++)
        {
            // point to the next available object buffer
            temp2 = &(file->objectData[loopIndex]);
       
            // loopIndex2 is being used here as a temporary variable
            // so when we copy the buffer we can remember where we started from
            loopIndex2 = objectOffset;

            // accounts for extra two properties items.
            blockSize = 2;
            objectOffset += 2; 
            layer = 0;

    nextLayer:

            // the first objects to look for are 3 byte objects
            objectSize = 3;

            temp2->error = 0;

            if( (objectOffset - nextBlock) > (bufLength + 0x1))
            {
                itoa(objectOffset - nextBlock, errorBuf, 16);

                MessageBox(0, "Object Data is longer than stated in the file", errorBuf, MB_OK);
            
                //goto cleanup;
            }        

            // the < 0xFFFF check is pretty much just a precaution
            while(blockSize < 0xFFFF)
            {
                // 0xFFFF is the terminating signal
                if( Get2Bytes(temp1, objectOffset) == 0xFFFF)
                {
                    blockSize += 2; 
                    objectOffset += 2;
                
                    goto endloop;
                }

                if( Get2Bytes(temp1, objectOffset) == 0xFFF0)
                {
                    if(objectSize == 3)
                    {
                        blockSize += 2;
                        objectSize = 2;
                        objectOffset += 2;
                        continue;
                    }
                }
            
                objectOffset += objectSize;
                blockSize += objectSize;

            }

    endloop:

            layer++;

            if(layer <= 2)
                goto nextLayer;

            InitBuffer(blockSize, temp2);
            CopyBuffer(temp2, temp1, 0, loopIndex2, blockSize);

        }

        // next load up the entrance
        nextBlock = offset + 4;

        itoa(nextBlock, errorBuf, 16);

        //MessageBox(0, errorBuf, "_", MB_OK);


  

        bufLength = Get4Bytes(temp1, offset);
        offset += (bufLength + 4);

        temp2 = &(file->entrances.entranceBuffer);

        InitBuffer(bufLength, temp2);
        CopyBuffer(temp2, temp1, 0, nextBlock, bufLength);

    

        // next load up the chest data
        nextBlock = offset + 4; 
        bufLength = Get4Bytes(temp1, offset);
        offset += (bufLength + 4);

        temp2 = &(file->chestLocationData);

        InitBuffer(bufLength, temp2);
        CopyBuffer(temp2, temp1, 0, nextBlock, bufLength);

    

        // next load up the sprite data
        nextBlock = offset + 4;
        bufLength = Get4Bytes(temp1, offset);
        offset += (bufLength + 4);

        spriteOffset = nextBlock;

        for(loopIndex = 0; loopIndex < 0x140; loopIndex++)
        {
            temp2 = &(file->spriteData[loopIndex]);

            loopIndex2 = spriteOffset;

            // account for the properties byte at the start of each segment.
            spriteOffset++;

            for( blockSize = 1 ; blockSize < 0xFFFF; )
            {
                if( GetByte(temp1, spriteOffset) == 0x000000FF)
                {
                    if(temp1->error == 1)
                    {
                        itoa(spriteOffset, errorBuf, 16);

                        //MessageBox(0, errorBuf, "?", MB_OK);
                    }

                    spriteOffset++;
                    blockSize++;
                
                    break;
                }

                spriteOffset += 3;
                blockSize += 3; 
            }

            InitBuffer(blockSize, temp2);
            CopyBuffer(temp2, temp1, 0, loopIndex2, blockSize);
        }

        // At this point we are done moving the data into an import buffer
        // Now we must figure out how the extended data in this rom is organized.
        // This requires us to look at the BM_Header structure

        if(MessageBox(0, "Overwrite existing dungeon data?", "warning", MB_YESNO) == IDYES)
            game->currentFile = &(game->portFile);
                                      
    // clean up

    cleanup:

        DestroyBuffer(&importBuffer);
        DestroyBuffer(&importName);
    }

// ===============================================================

    void ImportOverworld(zgPtr game, HINSTANCE prog)
    {

    }

// ===============================================================

    void ExportOverworld(zgPtr game, HINSTANCE prog)
    {

    }

// ===============================================================

    void UnpackDungeonHeader(BM_DungeonHeader *header)
    {
        bufPtr data;
        u8 currentByte;
        zgPtr game = (zgPtr) GetWindowLong(windowList[zgIndex], 0);

        data = header->data;

        CopyBuffer(data,    &(game->currentFile->headerData),
                   0,       game->currentDungRoom * 0x0E,
                   0x0E);

        currentByte = GetByte(data, 0);

        header->bg0_props = (currentByte >> 5) & 0x07;
        header->collision = (currentByte >> 2) & 0x07;
        header->lightsOut = (currentByte)      & 0x01;

        header->paletteIndex = GetByte(data, 1);
        header->graphicsNum  = GetByte(data, 2);
        header->spriteGfx    = GetByte(data, 3) + 0x40;
        header->effect       = GetByte(data, 4);
        header->tag_1        = GetByte(data, 5);
        header->tag_2        = GetByte(data, 6);
     
        currentByte = GetByte(data, 7);

         // The modulo 3 (% 3) is because 3 is not actually a valid value.
        header->warpPlane       = ((currentByte >> 0) & 0x03) % 3;
        header->staircasePlane1 = ((currentByte >> 2) & 0x03) % 3;
        header->staircasePlane2 = ((currentByte >> 4) & 0x03) % 3;
        header->staircasePlane3 = ((currentByte >> 6) & 0x03) % 3;
     
        header->staircasePlane4 = (GetByte(data, 8) & 0x03 ) % 3;

        header->warpRoom        = GetByte(data, 9);
        header->staircaseRoom1  = GetByte(data, 10);
        header->staircaseRoom2  = GetByte(data, 11);
        header->staircaseRoom3  = GetByte(data, 12);
        header->staircaseRoom4  = GetByte(data, 13);        

        return;
    }

// ===============================================================

    void RepackDungeonHeader(zgPtr game, BM_DungeonHeader *header, bufPtr data)
    {
        u8 byte;

        byte = (header->bg0_props & 0x07) << 5;
        byte |= (header->collision & 0x07) << 2;
        byte |= (header->lightsOut & 0x01);

        PutByte(data, 0, byte);
        PutByte(data, 1, header->paletteIndex);
        PutByte(data, 2, header->graphicsNum);
        PutByte(data, 3, header->spriteGfx - 0x40);
        PutByte(data, 4, header->effect);
        PutByte(data, 5, header->tag_1);
        PutByte(data, 6, header->tag_2);

        byte =  (header->warpPlane % 0x03)       << 0;
        byte |= (header->staircasePlane1 % 0x03) << 2;
        byte |= (header->staircasePlane2 % 0x03) << 4;
        byte |= (header->staircasePlane3 % 0x03) << 6;

        PutByte(data, 7, byte);
        PutByte(data, 8, header->staircasePlane4 % 0x03);
        PutByte(data, 9, header->warpRoom);
        PutByte(data, 10, header->staircaseRoom1);
        PutByte(data, 11, header->staircaseRoom2);
        PutByte(data, 12, header->staircaseRoom3);
        PutByte(data, 13, header->staircaseRoom4);

        CopyBuffer(&game->currentFile->headerData, data,
                   game->currentDungRoom * 0x0E,   0,
                   0x0E);

        return;
    }

// ===============================================================

    void LoadTileMap(zgPtr game)
    {
        u32 room = game->currentDungRoom;
        u32 tileOffset = 0;
        u32 i = 0;
        u32 j = 0;
        u32 map = 0;

        DngObjStruct *d = game->dngObjs;

        bufPtr rom = game->image;
        bufPtr objects = &game->currentFile->objectData[room];

        bufPtr BG0_Map = game->BG_Map[0];
        bufPtr BG1_Map = game->BG_Map[1];
  
        // Each 64x64 tile BG is painted with a preset pattern of tiles
        // floor0 is for BG0 and floor1 is for BG1
        d->floor[0] = (GetByte(objects, 0) & 0xF0);
        d->floor[1] = (GetByte(objects, 0) & 0x0F) << 4;

        for(i = 0, map = 0;   ; i += 2)
        {
            if(i == 8)
                i = 0, map++;
        
            if(map == 2)
                break;
 
            // Tells us what part of the tilemap to write into
            // tileOffset will correspond to Y in routine $8A44 if you're confused
            // i corresponds to $0C
            tileOffset = Get2Bytes(rom, 0x1B02 + i );

            for(j = 0; j < 8; j++)
            {
                Draw4x4s(rom, game->BG_Map[map], 8, &tileOffset, d->floor[map]);
                tileOffset += 0x1C0;
            }
        }

        d->layoutNum = GetByte(objects, 1) >> 2;

        LoadObjects(game); 
        //LoadBlocks(game);
        //LoadTorches(game);

        DrawObjects(game);
        LoadAttributes(game);
    }

// ===============================================================

    void Draw4x4s(bufPtr rom, bufPtr BG_Map, u32 numBlocks, u32 *tileOffset, u32 index)
    {
        // index corresponds to 
        // numBlocks is the number of 4x4 tile blocks to draw from right to left.

        u32 i = 0;
        u32 j = 0; // just loop counters
        u32 k = *tileOffset; // Corresponds to Y in $8A44

        for( i = 0; i < numBlocks; i++)
        {     
            // The fixed constants 0, 2, 4, 6, 80, are risky... they might vary in reality
            for( j = 0; j < 2; j++ )
            {
                Put2Bytes( BG_Map, 0 + k,    Get2Bytes(rom, 0x1B52 + index) );
                Put2Bytes( BG_Map, 2 + k,    Get2Bytes(rom, 0x1B54 + index) );
                Put2Bytes( BG_Map, 4 + k,    Get2Bytes(rom, 0x1B56 + index) );
                Put2Bytes( BG_Map, 6 + k,    Get2Bytes(rom, 0x1B58 + index) );
                Put2Bytes( BG_Map, 0x80 + k, Get2Bytes(rom, 0x1B5A + index) );
                Put2Bytes( BG_Map, 0x82 + k, Get2Bytes(rom, 0x1B5C + index) );
                Put2Bytes( BG_Map, 0x84 + k, Get2Bytes(rom, 0x1B5E + index) );
                Put2Bytes( BG_Map, 0x86 + k, Get2Bytes(rom, 0x1B60 + index) );

                k += 0x100;
            }

            k -= 0x1F8;

        }

        *tileOffset = k;

        return;
    }

// ===============================================================

    void LoadObjects(zgPtr game, u32 typeSelect)
    {
        u32 i = 0; // simple index
        u32 c = 0;
        u32 roomNum = game->currentDungRoom;
        u32 index = 0;
        u32 layoutNum = 0;
        u32 objNum = 0;
        u32 objectSize = 3;
        u32 placeHolder1 = 0;
        u32 placeHolder2 = 0;

        DngObjStruct *d = game->dngObjs;
        DngObjList* type1[4];
        DngObjList* type2[4];   

        bufPtr objects = &(game->currentFile->objectData[roomNum]);
        bufPtr layout = &(game->currentFile->layoutData[ d->layoutNum ]);
        bufPtr objData = 0; // used to select between layout and objects 

        for(i = 0; i < 4; i++)
        {
            type1[i] = &d->type1[i]; // temporary I hope >:(
            type2[i] = &d->type2[i];
        }

        u32 layerType1Obj[3] = {0, 0, 0};
        u32 layerType2Obj[3] = {0, 0, 0};
  
        typeSelect &= 0x3;

        for(c = 0; c < 4; c++) // --------------------------for(counter)
        {
            if(c == 0)
                objData = layout, index = 0;
        
            if(c == 1)
                objData = objects, index = 2;

            objectSize = 3;
            placeHolder1 = placeHolder2 = index;

            if(index > objData->length)
                exit(-1); // precaution

            while(1)        // ---------------------------------------------------------------
            {
                objNum = Get2Bytes(objData, index);

                if(objNum == 0xFFFF)
                {
                    index += 2;
                    break;
                }

                if(objNum == 0xFFF0)
                {
                    if(objectSize == 2)
                        goto alreadySize2;

                    objectSize = 2;
                    index += 2;
                    placeHolder2 = index;
               
                    continue;
                }

                if(objectSize == 3)
                {
                    type1[c]->numObj++;
                    index += objectSize;
                }
                else
                {
                alreadySize2:
                    type2[c]->numObj++;
                    index += objectSize;
                }
            }               //------------------------------------------------- while( 1 )

            if(typeSelect & 0x01)
                ParseType1(type1[c], objData, placeHolder1, game, c);
        
            if(typeSelect & 0x02)
                ParseType2(type2[c], objData, placeHolder2, game, c);
        


        } // ---------------------------------------------------for( counter )

        return; 
    }

// ===============================================================

    void ParseType1(DngObjList *doList, bufPtr objects, u32 placeHolder, zgPtr game, u32 layerNum)
    {
        u32 objNum = 0;
        u32 index = placeHolder;
        u32 a = 0;
        u32 numObjs = doList->numObj;
        u32 f = 0, g = 0, h = 0, i = 0, j = 0, k = 0;
        u32 layers[4] = {1, 1, 0, 1};

        bufPtr rom = game->image;
        DngObj *doPtr;

        if(numObjs == 0)
        {
            ResizeBuffer(doList->data, 0);

            return;
        }

        ResizeBuffer(doList->data, sizeof(DngObj)*numObjs);
         
        for(a = 0; a < numObjs; a++, index += 3)
        {
            f = g = h = i = j = k = 0;

            doPtr = GetDngObj(doList, a);

            objNum = Get3Bytes(objects, index);
        
            if( (objNum & 0xFC) == 0xFC)
                goto subtype2;

            i = (objNum & 0xFC) >> 1;
            i |= (objNum & 0xFC00) >> 3;
            j = objNum & 0x03;
            k = (objNum & 0x0300) >> 8;
            h = (objNum >> 0x10);
                
            if(h >= 0xF8)
                goto subtype3;

    //subtype1: no label needed really

            g = Get2Bytes(rom, 0x8000 + (h * 2));
            doPtr->subType = 1;
       
            goto parsed;

    subtype2:

            // i corresponds to $08
            i = (XBA(objNum & 0xFFFF) & 0x03F0) >> 3;

            j = (XBA( objNum >> 8) & 0x0FC0) << 1 ;
            i |= j;

            j = (XBA(objNum >> 8) & 0x003F);
            g = Get2Bytes(rom, 0x83F0 + (j * 2));

            doPtr->subType = 2;
        
            h = j;

            goto parsed;

    subtype3:

            h <<= 1;
            h = (h & 0x000E) << 3;
            h = (h | j | (k << 2));

            g = Get2Bytes(rom, 0x84F0 + (h * 2));

            doPtr->subType = 3;

    parsed:
     
            doPtr->doMin = 0;
            doPtr->doMax = 0; 

            doPtr->isOnBG0 = false; 
            doPtr->isOnBG1 = false;

            doPtr->type = 1;
            doPtr->mapAddr = i;

            doPtr->arg1 = j;
            doPtr->arg2 = k;
            doPtr->tileNum = g;

            doPtr->funcNum = h;
            doPtr->index = a;

            doPtr->mapNum = layers[layerNum];
            doPtr->layer = layerNum;
        }

        return;
    }

// ===============================================================

    void ParseType2(DngObjList* doList, bufPtr objects, u32 placeHolder, zgPtr game, u32 layerNum)
    {
        u32 objNum = 0;
        u32 index = 0;
        u32 a = 0;
        u32 numObj = doList->numObj;
        u32 f = 0, g = 0, h = 0, i = 0, j = 0, k = 0;
        u32 layers[4] = {1, 1, 0, 1};

        DngObj *doPtr;

        if(numObj == 0)
        {
            ResizeBuffer(doList->data, 0);
            return;
        }

        ResizeBuffer(doList->data, sizeof(DngObj) * numObj );

        for(a = 0, index = placeHolder; a < numObj; a++, index += 2)
        {
            f = g = h = i = j = k = 0;

            doPtr = GetDngObj(doList, a); 

            objNum = Get2Bytes(objects, index);

            i = (objNum & 0x00F0) >> 3;
            f = j = (XBA(objNum) & 0x00FF);
            k = (objNum & 0x0003);

            doPtr->doMin = 0; 
            doPtr->doMax = 0; 
        
            doPtr->isOnBG0 = false; 
            doPtr->isOnBG1 = false;
        
            doPtr->type = 2;
            doPtr->subType = 1; 
        
            doPtr->mapAddr = g;
            doPtr->arg1 = j; 
            doPtr->arg2 = f;
            doPtr->tileNum = i;

            doPtr->funcNum = k; 
            doPtr->index = a;
            doPtr->mapNum = layers[layerNum]; 
            doPtr->layer = layerNum;
        }

        return;
    }

// ===============================================================

    void LoadAttributes(zgPtr game)
    {
        bufPtr rom = game->image; 
        DngObjStruct *d = game->dngObjs;

        bufPtr mat = d->masterAttrTbl;
        bufPtr attr[2] = {d->BG_Attr[0], d->BG_Attr[1]};

        u32 i = 0;
        u32 j = 0;
        u32 mapNum = 0;
        u32 tableIndex = 0;
        u32 argument = 0;
        u32 attribute = 0;
        u32 flips = 0;
        u32 graphicsIndex = game->bm_dung_header.graphicsNum;
        u32 attrOffset = 0;
        bool bigChest = false;

        CopyBuffer(mat,   rom,
                   0,     0x71659,
                   0x140);

        CopyBuffer(mat,   rom,
                   0x1C0, 0x71799,
                   0x40);

        attrOffset = Get2Bytes(rom, 0x71000 + (graphicsIndex << 1));

        CopyBuffer(mat,     rom,
                   0x140, 0x7102A,
                   0x80);

        // Set the attributes based on the CHR values of the tilemap
        for( tableIndex = 0; tableIndex < 2; tableIndex++)
        {
            for(i = 0; i < 0x1000; i++)
            {
                flips = 0;
            
                // in the game it's 0x03FF but this is all you need really
                argument = Get2Bytes(game->BG_Map[tableIndex], i * 2);
                argument &= 0x01FF; 
                attribute = GetByte(mat, argument);
            
                // Note that while here the attributes are pulled directly from rom
                // In memory they will be at $7EFE00-$7EFFFF

                if(attribute >= 0x10 && attribute < 0x1C)
                    flips = (argument & 0xC000) >> 0x0E;

                attribute |= flips;

                PutByte(attr[tableIndex], i, attribute);
            }
        }

        //load special attributes for two state objects

        i = j = 0; attribute = 0x7070;

        while(d->numTwoStateObjs > 0)
        {
            mapNum = 1; j = d->twoStateLocs[i] >> 1;

            if(j & 0x1000)
            {
                j ^= 0x1000; mapNum = 0;
            }

            Put2Bytes(attr[mapNum], j + 0x00, attribute);
            Put2Bytes(attr[mapNum], j + 0x40, attribute);

            attribute += 0x0101; i++; d->numTwoStateObjs--;
        }

        //load special attribute values for chests and big key locks
        for(i = 0, attribute = 0x5858; i < 6; i++)
        {
            while(d->numChests > 0)
            {
                mapNum = 1; j = d->chestLocs[i] >> 1; bigChest = false;

                if(j & 0x4000)
                    bigChest = true, j ^= 0x4000;

                if(j & 0x1000)
                    mapNum = 0, j ^= 0x1000;

                Put2Bytes(attr[mapNum], j + 0x00, attribute);
                Put2Bytes(attr[mapNum], j + 0x40, attribute);

                if(bigChest)
                {
                    Put2Bytes(attr[mapNum], j + 0x80, attribute);
                    Put2Bytes(attr[mapNum], j + 0x42, attribute);
                    Put2Bytes(attr[mapNum], j + 0x82, attribute);
                }
                                                
                attribute += 0x0101;

                i++; d->numChests--; d->numBigKeyLocks--;
            }

            while(d->numBigKeyLocks > 0)
            {
                j = d->chestLocs[i] >> 1;
                d->chestLocs[i] |= 0x8000;

                Put2Bytes(attr[1], j + 0x00, attribute);
                Put2Bytes(attr[1], j + 0x40, attribute);

                attribute += 0x0101;

                i++; d->numBigKeyLocks--;
            }


        }

        return;
    }

// ===============================================================

    void DrawObjects(zgPtr game)
    {
        u32 a = 0, b = 0, c = 0;

        DngObjStruct *d = game->dngObjs;
        DngObjList *doListPtr = 0;

        object *Type1Func[3] = {Type1_1Object,
                                Type1_2Object,
                                Type1_3Object
                             };

        DngObj* doPtr;
        DngObj doParam;

        // Go through each layer, drawing the type 1 and type 2 objects on each pass
        for(a = 0; a < 8; a++)
        {    
            if(!(a % 2) )
            {
                doListPtr = &d->type1[a >> 1], c = 0;
            }
            else
            {
                doListPtr = &d->type2[a >> 1]; c = 1;
            }

            for(b = 0; b < doListPtr->numObj; b++)
            {
                doPtr = GetDngObj(doListPtr, b);
            
                if(!doPtr)
                    continue;

                doParam = *doPtr;
            
                if(c == 0)
                    Type1Func[doPtr->subType-1][doPtr->funcNum](game, &doParam);
                else
                    Type2_Object[doPtr->funcNum](game, &doParam);

                doPtr->doMin = doParam.doMin;
                doPtr->doMax = doParam.doMax;
                doPtr->isOnBG0 = doParam.isOnBG0;
                doPtr->isOnBG1 = doParam.isOnBG1;

                doPtr->width = (((doPtr->doMax & 0x7F) - (doPtr->doMin & 0x7F)) / 2) + 1 ;
                doPtr->height = (0x80 + (doPtr->doMax & 0xFF80) - (doPtr->doMin & 0xFF80)) / 0x80;

                LoadDngObjTiles(game, doPtr);
            }

        }

        return;
    }

// ===============================================================

    void PopulateRegistries(zgPtr game)
    {
        u32 i = 0;
        u32 j = 0;
        u32 maxObjs = 0;

        DngObjStruct *d = game->dngObjs;
        DngObjList *doList = 0;
        DngObj *doPtr;

        for(i = 0; i < 4; i++)
        {
            maxObjs = max(d->type1[i].numObj, d->type2[i].numObj);

            for(j = 0; j < maxObjs; j++)
            {
                doList = &d->type1[i];

                if(j < doList->numObj)
                {
                    doPtr = GetDngObj(doList, j);

                    if(!doPtr)
                        goto type2;

                    if(doPtr->funcNum == 0x63)
                        j = j;

                    RegDngObj(game, d, doPtr);                                            
                }

            type2:

                doList = &d->type2[i];

                if(j < doList->numObj)
                {
                    doPtr = GetDngObj(doList, j);

                    if(!doPtr)
                        continue;
           
                    RegDngObj(game, d, doPtr); 
                }




            }
        }
    }

// ===============================================================

    void DrawTiles(zgPtr game)
    {
        u32 i = 0;
        u32 j = 0;
        u32 k = 0;
   
        u32 entry;
        u32 palette;
        u32 chrNum;
        u32 stride;
        bool hFlip;
        bool vFlip;

        // wtf is this doing in this routine! move it soon
        WEStruct *wes = &(game->currentWES);
        ManagedBitmap *b;

        bufPtr BG_Map = game->BG_Map[0];

        for(i = 0; i < wes->numElements; i++)
        {
            if( !strcmp(wes->WESet[i].windowCaption, "DUNGPIC") )
            {
                // If the caption name matched but the class name is incorrect, we must move on.
                if( strcmp(wes->WESet[i].className, "PICTURE") )
                    continue;

                game->pictWin = wes->WESet[i].handle; break;
            }
        }

        if( !(game->pictWin) )
            return;

        b = (ManagedBitmap*) GetWindowLong(game->pictWin, 0);

        j = b->header->biHeight * b->header->biWidth;
        k = game->Pals[0][0];

        for(i = 0 ; i < j ; i++)
        {
            Put3Bytes(b->pixels, i * 4, k);
        }

        // number of bytes per scanline
        stride = b->header->biWidth * 4;

        j = (stride * (b->header->biHeight - 1) );

        for(i = 0; i < BG_Map->length; i += 2)
        {
            if(i % 0x80 == 0 && i)
                j -= (stride * 9);

            entry = Get2Bytes(BG_Map, i);
            palette = (entry & 0x1C00) >> 0x0A;
            chrNum = (entry & 0x3FF);
            hFlip = (entry & 0x4000) ? true : false;
            vFlip = (entry & 0x8000) ? true : false;
        
            BlitTile(b, i*0x10 + j, &game->BGTiles[chrNum], &game->Pals[palette], hFlip, vFlip);  
        }

        // now draw BG1
        BG_Map = game->BG_Map[1];

        j = (stride * (b->header->biHeight - 1) );

        for(i = 0; i < BG_Map->length; i += 2)
        {
            if( (i % 0x80 == 0) && i)
                j -= (stride * 9);

            entry = Get2Bytes(BG_Map, i);
            palette = (entry & 0x1C00) >> 0x0A;
            chrNum = (entry & 0x3FF);
            hFlip = (entry & 0x4000) ? true : false;
            vFlip = (entry & 0x8000) ? true : false;
        
            BlitTile(b, i*0x10 + j, &game->BGTiles[chrNum], &game->Pals[palette], hFlip, vFlip);  
         }
    }

// ===============================================================

    void EraseTile(ManagedBitmap *b, u32 location, u32 bgColor)
    {
        u32 i = 0;
        u32 j = 0;

        bufPtr pixels = b->pixels;
        u32 scans = 8;
        u32 gun = 0;
        u32 actual = 0;
        u32 colorLoc = 0;

        for(scans = 0; scans < 8; scans++)
        { 
            for( i = 0, gun = location; i < 8; i++, gun += 4 )
            {
                actual = ((7 - scans) * b->stride);
                Put4Bytes(pixels, gun - actual, bgColor);
            }
        }
    }

// ===============================================================

    void BlitTile(ManagedBitmap *b, u32 location, SnesTile* tiles, SnesPal* pal, bool hFlip, bool vFlip)
    {
        u32 i = 0;
        u32 j = 0;
        u32 colorIndex = 0;
        u32 result = 0;

        bufPtr pixels = b->pixels;
        u32 scans = 8;
        u32 gun = 0;
        u32 actual = 0;
        u32 colorLoc = 0;

        for(scans = 0; scans < 8; scans++)
        { 
            for( i = 0, gun = location; i < 8; i++, gun += 4 )
            {
                actual = ((7 - scans) * b->stride);

                if(hFlip)
                    colorLoc = (7 - i);
                else
                    colorLoc = i;

                if(vFlip)
                    colorLoc += (56 - (8*scans));
                else
                    colorLoc += (8 * scans);

                colorIndex = (*tiles)[colorLoc];

                if(!colorIndex)
                    continue;

                result = Put4Bytes(b->pixels, gun - actual, ((*pal)[colorIndex]));

                if(!result)
                    i = i;
            }
        }
    }

// ===============================================================

    void BlitTranslucent(ManagedBitmap *b, u32 location, u32 stride, SnesTile *tile, SnesPal *pal, bool hFlip, bool vFlip)
    {
        u32 i = 0;
        u32 j = 0;
        u32 colorIndex = 0;

        bufPtr pixels = b->pixels;
        u32 scans = 8;
        u32 gun = 0;
        u32 actual = 0;
        u32 colorLoc = 0;
        u32 beforePixel = 0;
        u32 blendPixel = 0;
        u32 newPixel;

        for(scans = 0; scans < 8; scans++)
        { 
            for( i = 0, gun = location; i < 8; i++, gun += 4 )
            {
                actual = ((7 - scans) * stride);

                if(hFlip)
                    colorLoc = (7 - i);
                else
                    colorLoc = i;

                if(vFlip)
                    colorLoc += (56 - (8*scans));
                else
                    colorLoc += (8 * scans);

                colorIndex = (*tile)[colorLoc];

                if(!colorIndex)
                    continue;

                beforePixel = Get4Bytes(b->pixels, gun - actual);
                blendPixel = ((*pal)[colorIndex]);

                newPixel = (beforePixel + ((blendPixel >> 1) & 0xF8F8F8));

                if(newPixel & 0x700)
                    newPixel = (newPixel & 0xFFFFFFF8) | 0x0000F8;

                if(newPixel & 0x70000)
                    newPixel = (newPixel & 0xFFFFF8FF) | 0x00F800;

                if(newPixel & 0x7000000)
                    newPixel = (newPixel & 0x00F8FFFF) | 0xF80000;
            
                Put4Bytes(b->pixels, gun - actual, newPixel);
            }

            //location -= stride;
        }

        return;
    }

// ===============================================================

    void RedrawTile(ManagedBitmap *b, u32 x, u32 y, zgPtr game)
    {
        u32 stride = b->header->biWidth * 4;
        u32 mapAddr = (x * 2) + (0x80 * y);
        u32 adjMapAddr = 0;
        u32 i = 0;
        u32 j = 0;
        u32 offsets[4] = {0, 0x20, -(stride*8), 0x20 - (stride * 8) };
        u32 mapOffsets[4] = {0, 2, 0x80, 0x82 };
        u32 entry;
        u32 chrNum;
        u32 palette;
        bool hFlip;
        bool vFlip;
    
        x = (x << 3) * 4;
        y = ((b->header->biHeight - 1 - (y << 3)) * stride);

        for(i = 0; i < 4; i++)
        {
            j = offsets[i];

            adjMapAddr = mapAddr + mapOffsets[i];

            entry = Get2Bytes(game->BG_Map[0], adjMapAddr);
            palette = (entry & 0x1C00) >> 0x0A;
            chrNum = (entry & 0x3FF);
            hFlip = (entry & 0x4000) ? true : false;
            vFlip = (entry & 0x8000) ? true : false;

            BlitTile(b, x + y + j, &game->BGTiles[chrNum], &game->Pals[palette], hFlip, vFlip);
        }

        for(i = 0; i < 4; i++)
        {
            j = offsets[i];

            adjMapAddr = mapAddr + mapOffsets[i];

            entry = Get2Bytes(game->BG_Map[1], adjMapAddr);
            palette = (entry & 0x1C00) >> 0x0A;
            chrNum = (entry & 0x3FF);
            hFlip = (entry & 0x4000) ? true : false;
            vFlip = (entry & 0x8000) ? true : false;

            BlitTile(b, x + y + j, &game->BGTiles[chrNum], &game->Pals[palette], hFlip, vFlip);
        }

        return;
    }

// ===============================================================

    void AniDecompress(zgPtr game)
    {
        // Simulates $00D337
        bool check = false;

        u8 *cTest = (u8*) malloc(0x1000);

        int testLen = 0;

        u32 index   = 0;
        u32 gfxNum  = game->bm_dung_header.paletteIndex;
        u32 targPtr = 0, srcPtr = 0;
        u32 x       = 0;
        u32 i       = 0;
        u32 j       = game->currentDungRoom;

        DungeonFile *dFile = game->currentFile;

        bufPtr rom      = game->image;
        bufPtr aniV     = game->aniVram;
        bufPtr headers  = &dFile->headerData;
        bufPtr durp     = NULL;
        bufPtr durp2    = NULL;
        bufPtr durp3    = NULL;
        bufPtr durp4    = NULL;

        BM_DungeonHeader tempHeader;

        // ---------------------------------

        if(game->currentFunc == dung)
        {
            for( i = 0; i < game->numEntrances ; i++)
            {
            

                /*
                index2 = Get2Bytes( &(game->currentFile->entrances.roomNumber) , i * 2);
                dummyInt = GetByte( &(game->currentFile->entrances.blockSet) , i);

                if(index2 == game->currentDungRoom)
                {    
                    index3 = dummyInt; 
                    // If there's a match, this is the entrance that leads to this room.
                    break;
                }*/               
            }
      
            // Represents $0AA1)
            index = GetByte(rom, 0x75463 + gfxNum);
            index = GetByte(rom, 0x1011E + index);
       
            //index = game->GetBgGfxPtr(index);
            index = game->GetBgGfxPtr(0x5D);
        
            game->testPtr = DecompressStandard(rom, index);

            //durp = CompressStandard(game->testPtr);

            //durp2 = DecompressStandard(durp,0);

            //durp3 = CompressStandard(durp2);

            //durp4 = DecompressStandard(durp3, 0);



            //testLen = sup->Compress((const u8*) durp4->contents, durp4->length, cTest);

            //check = CompareBuffer(durp2, durp4);

            //ResizeBuffer(durp, 0x100000);

        

            //testLen = sup->Decompress(cTest, testLen, durp->contents);

            //testLen = sup->Compress(durp->contents, testLen, cTest);

    // */

            /*DeallocBuffer(durp4);
            DeallocBuffer(durp3);
            DeallocBuffer(durp2);
            DeallocBuffer(durp);*/
        

            targPtr = 0; srcPtr = 0;

            Do3To4Low(aniV,     game->testPtr,
                      &targPtr, &srcPtr,
                      0x30);

            index = game->GetBgGfxPtr(0x5C); // This constant is fixed in the game. Could
                                              // Find a way to change it I guess.

            DeallocBuffer(game->testPtr); // Deallocates the previous buffer that the 
                                    // DecompressStandard routine generates

            game->testPtr = DecompressStandard(rom, index);

            targPtr = 0x600; srcPtr = 0;

            Do3To4Low(aniV,     game->testPtr,
                      &targPtr, &srcPtr,
                      0x30);

            DeallocBuffer(game->testPtr);

            // next... loop through and rearrange the tiles so that they animate properly

            while(x < 0x200)
            {
                index = Get2Bytes(aniV, 0x200 + x);

                Put2Bytes(aniV, 0x200 + x, Get2Bytes(aniV, 0x600 + x));
                Put2Bytes(aniV, 0x600 + x, Get2Bytes(aniV, 0x800 + x));
                Put2Bytes(aniV, 0x800 + x, Get2Bytes(aniV, 0x400 + x));       
                Put2Bytes(aniV, 0x400 + x, index);   
                x += 2;
            }
        }
        else
        {


        }
    
        testFH = CreateFile("aniTest.bin", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
  
                
        WriteFile(testFH, 
                  game->aniVram->contents, 
                  game->aniVram->length, 
                  &numBytesRead, 
                  0);

        CloseHandle(testFH);
    }

// ===============================================================

    #include "compresch_lttp.h"

    void DngDecompress(zgPtr game)
    {
        // this routine corresponds to $00:E19B in the rom

        u32 index = 0, index2 = 0, index3 = 0, index4 = 0, dummyInt = 0, source = 0; 
        u32 i = 0, j = 0;
        bufPtr decomp = NULL;

        Compresch_LTTP *lt = new Compresch_LTTP(true);

        u8* test  = new u8[0x2000];
        u8* test2 = new u8[0x2000];

        // ---------------------------------

        // Find the pointer to the graphics to decompress
        // should optionally use this or 0x01 or 0x0A depending on... something >_<
        index4 = game->GetSprGfxPtr(0xB);
        
        // Decompress them using a special method
        game->testPtr = DecompressOther(game->image, index4);

        index = 0x8800;
        index4 = 0;

        CopyBufferWB(game->vram,    game->testPtr,
                     &index,          &index4,
                     game->testPtr->length);

        // this happens for all modes regardless of dungeons or outdoors
        index4 = game->GetSprGfxPtr(0x6);
        
        Do3To4Low(game->vram,
                  game->image,
                  &index,
                  &index4,
                  0x80);

        DeallocBuffer(game->testPtr);

 
        // Now we move on to decompressing and storing the 4 graphics subsets
        // that are dependent upon the room you are in.
    
        index3 = game->bm_dung_header.spriteGfx;
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
                    PutByte(game->cgram, index, 0);

                break;

            case 0x52:
            case 0x53:
            case 0x5A:
            case 0x5B:
            case 0x5C:
            case 0x5E:
            case 0x5F:

                decomp = DecompressStandard(game->image, index4);

                // END TEST CODE

                game->testPtr = DecompressOther(decomp, 0);

                DeallocBuffer(decomp); // since we don't need it anymore

                index4 = 0;
                CopyBufferWB(game->vram, game->testPtr,
                             &index,      &index4,
                             game->testPtr->length);

                DeallocBuffer(game->testPtr);

                break;

            default:
            
                game->testPtr = DecompressStandard(game->image, index4);

                // TEST CODE, REMOVE LATER
                /**
                i = lt->Decompress(game->image->contents + index4, 0x2000, test);
                i = lt->Compress(test, i, test2);
            
                i = memcmp(decomp->contents, test, decomp->length);
                */

                index4 = 0;
                Do3To4Low(game->vram,   game->testPtr,
                          &index,       &index4,
                          0x40);

                DeallocBuffer(game->testPtr);

                break;
            }
        }

        index3 = 0; // main tile set for this entrance

        for( i = 0; i < game->numEntrances ; i++)
        {
            index2 = Get2Bytes( &(game->currentFile->entrances.roomNumber) , i * 2);
            dummyInt = GetByte( &(game->currentFile->entrances.blockSet) , i);

            if(index2 == game->currentDungRoom)
            {    
                index3 = dummyInt; 
                // If there's a match, this is the entrance that leads to this room.
                break;
            }                
        }

        // Next decompress the BG graphic data
    
        // Target in VRAM is 0x4000

        index = 0x4000;
    
        index3 *= 8;
        index3 += 0x6073;

        // graphicsNum corresponds to $0AA2?
        index2 = game->bm_dung_header.graphicsNum;
        index2 *= 4;
        index2 += 0x5D97;

        for( i = 0; i < 8; i++, index3++)
        {
            source = GetByte(game->image, index3);     
            source = game->GetBgGfxPtr(source);

            index4 = GetByte(game->image, index2 + i - 3);

            if( i >= 3 && i <= 6)
                if( index4 != 0)
                    source = game->GetBgGfxPtr(index4);

            index4 = 0;

            //if( test $AA1) do something();
            if( dummyInt < 0x20 )
            {   
                if(i >= 4)
                    goto lower_8_colors;

                decomp = DecompressStandard(game->image, source);
            
                game->testPtr = DecompressOther(decomp, 0);

                DeallocBuffer(decomp);

                CopyBufferWB(game->vram, game->testPtr,
                             &index,             &index4,
                             game->testPtr->length);

                DeallocBuffer(game->testPtr);
            }
            else
            { 
            lower_8_colors:

                game->testPtr = DecompressStandard(game->image, source);
        
                Do3To4Low(game->vram,   game->testPtr,
                          &index,       &index4,
                          0x40);

                DeallocBuffer(game->testPtr);
            }
        }
    
        testFH = CreateFile("test.bin", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
  
                
        WriteFile(testFH, 
                  game->vram->contents, 
                  game->vram->length, 
                  &numBytesRead, 
                  0);


        CloseHandle(testFH);

        return;
    }

// ===============================================================

    /*

    Ganon's OAM data for one position

        ->

    OAM last two bytes:

    cccccccc vhoopppc

    X minimum = 0x8D
    Y minimum = 0x25

    10 10 00 00 00 00 10 10 10 10 10 10 10 10 10 10 10 10 10


    AF 2F 0A 6D         22 A  chr# 10A pal#6 hflip priority 2 
    AF 37 1A 6D         22 12 chr# 11A
    BC 29 64 A9         2F 4  chr# 164
    B7 2E 64 A9         2A 9      
    B1 34 64 A9         24 F
    AB 3A 64 A9         1E 15
    9F 3E 44 A9         12 19         
    AF 37 1A 6D         22 12
    97 28 00 2D         A  13
    A7 28 00 6D         1A 13
    97 25 04 2B         A  0
    A7 25 04 6B         1A 0
    8D 2B 38 2D         0  6
    8D 33 48 2D         0  E
    AF 2F 0A 6D         22 A
    97 35 24 2B         A  10
    A7 35 24 6B         1A 10
    94 3A 20 2D         7  15
    AA 3A 20 6D         1D 15
                
    */

// ===============================================================

    void LoadDngObjTiles(zgPtr game, DngObj *doPtr)
    {
    u32 i = 0;
    u32 j = 0;
    u32 k = 0;
    u32 pos = 0; // variable used to write to the chr[] and mask[] arrays
    u32 size = 0;
    u32 value = 0;
    u32 regInfo = 0;
    u32 regStart = 0;
    u32 bg = 0;

    DngObjStruct *d = game->dngObjs;
  
    bufPtr tilemaps[2] = {game->BG_Map[0], game->BG_Map[1]};
    bufPtr regs[2] = {d->scrapReg[0], d->scrapReg[1]};

    bufPtr chr[2] = { doPtr->chr0, doPtr->chr1 };
    bufPtr aboves[2] = { doPtr->above0, doPtr->above1 };
    bufPtr belows[2] = { doPtr->below0, doPtr->below1 };

    bool isOn[2] = {doPtr->isOnBG0, doPtr->isOnBG1 };
  
        // ----------------------------------------------
  
    while(bg <= 1)
    {
        if(!isOn[bg])
        {
            chr[bg] = 0;
            aboves[bg] = 0;
            belows[bg] = 0;
            bg++; 
            
            continue; 
        }

        // number of 8x8 tiles the object needs for its rectangle
        size = doPtr->height * (doPtr->width << 1);
         
        chr[bg] = CreateBuffer(size);
        belows[bg] = CreateBuffer(size);
        aboves[bg] = CreateBuffer(size);

        for(j = 0; j < size; j += 2)
        {
            i = doToWin(doPtr, j);

            value = Get2Bytes(tilemaps[bg], i);
            regInfo = Get2Bytes(regs[bg], i);
            
            if(regInfo & 0x8000)
            {
                if( (regInfo & 0x3FFF) == doPtr->index) 
                {
                    Put2Bytes(chr[bg], j, value);
                    Put2Bytes(belows[bg], j, 0x8000);
                }
            }
            else
            {
                Put2Bytes(chr[bg], j, 0);
                PutByte(belows[bg], j, 0);
            }
        }

        bg++;
    }

        //after the while loop ends reassign all these pointers back to the doPtr itself.
        doPtr->chr0 = chr[0];
        doPtr->chr1 = chr[1];
        doPtr->above0 = aboves[0];
        doPtr->above1 = aboves[1];
        doPtr->below0 = belows[0];
        doPtr->below1 = belows[1];
    }

// ===============================================================

