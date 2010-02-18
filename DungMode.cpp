
    #include "Black Magic.h"
    #include "Core.h"
    #include "Objects.h"
    #include "DungMode.h"

//********************************************************

    int dung(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        zgPtr game = (zgPtr) GetWindowLong(hwnd, 0);
        ManagedBitmap *b;
        DngObjStruct *d = game->dngObjs;

        bufPtr decomp = 0;
        u32 dummyInt = 0;

        // test variables
        u32 test = 0;
        char dummy[256];
        u32 x = 0, x_pix = 0;
        u32 y = 0, y_pix = 0;
        u32 address = 0;
        u32 attribute = 0;

        static bool t = false;
        static HWND mDlg = NULL;

        u32 i = 0;
        u32 j = 0;
        u32 pixel = 0;
        u32 stride = 0;

        u32 index = 0, index2 = 0, index3 = 0, index4 = 0, source = 0;

        SprStruct *spr;

        HDC memDC = NULL;
        HDC picDC = NULL;
        HMENU context;

        switch(msg)
        {
         case DUNG_INIT:

            ChangeWES(game->dungeonWE, hwnd, game);

            game->dngObjs = CreateDngObjStruct();
            game->currentFunc = dung;
            game->bm_dung_header.data = CreateBuffer(0x0E);
       
            SendMessage(hwnd, DUNG_CHANGE, wp, lp);

            game->toolDlg = CreateDialog(thisProg,
                                         MAKEINTRESOURCE(IDD_TESTTOOL),
                                         hwnd,
                                         toolDlgProc);

            ShowWindow(game->toolDlg, SW_SHOW);
            SetWindowPos(game->toolDlg, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

            break;

        case DUNG_CHANGE:

            game->aniTimer = 0;
            game->aniFrame = 0;

            InitDngObjStruct(game->dngObjs);

            UnpackDungeonHeader(&game->bm_dung_header);
        
            DngDecompress(game);
            AniDecompress(game);
            LoadPalettes(game);           
            LoadTileMap(game);
            DecodeTiles(game);
            DecodePalettes(game);
            PopulateRegistries(game);
            LoadSprites(game);
            DrawTiles(game);

            game->aniFrame = 0;
            game->aniTimer = 0;
            game->active = 1;

            index = game->currentDungRoom;
            itoa(index, dummy, 0x10);
            strupr(dummy);

            SetDlgItemText(hwnd, ID_DungRoomNum, dummy);
            SetWindowLong(game->toolDlg, DWL_USER, 0);

            break;

        case DUNG_SAVE:
    
            SaveRoom(game);

            break;

        case WM_ACTIVATEAPP:
        
            if(wp == TRUE)
                game->active = 1;
            else
                game->active = 0;

            return 0;
        
            break;

        case DUNG_SEARCH:
        
            i = game->currentDungRoom;

            for(j = i; ; )
            {   
                j = (j + 1) % (game->numRooms);

                if(j == i)
                    break;
            
                InitDngObjStruct(d);

                game->currentDungRoom = j;
                LoadObjects(game, 2);
      
                if(SearchForObj(d, 2, 1, 0x0, 0x14))
                {                
                    SendMessage(hwnd, DUNG_CHANGE, wp, lp);
                
                    return 1;
                }
            }

            game->currentDungRoom = i;
            SendMessage(hwnd, DUNG_CHANGE, wp, lp);
        
            break;

        case BM_TEARDOWN:

            // needs implementation badly

            return 0;

            break;

        case PICTURE_LBUTTON_UP:

            SendMessage(game->toolDlg, SPRITE_STOPPED, 0, 0);

            break;

        case PICTURE_LBUTTON_DOWN:

            x = LOWORD(lp);
            y = HIWORD(lp);

            spr = SpriteClick(game->dngObjs, x, y);
            SetWindowLong(game->toolDlg, DWL_USER, (long) spr);

            if(spr)
            {
                x = GetLastError();
                SendMessage(game->toolDlg, TOOL_SPRITE, (WPARAM) spr, 0);
            }

            break;

        case PICTURE_RBUTTON_DOWN:
        {
            x = LOWORD(lp);
            y = HIWORD(lp);

            int k = 0;
            POINT pt;
            GetCursorPos(&pt);
            //context = CreatePopupMenu();

            spr = SpriteClick(game->dngObjs, x, y);

            if(spr)
            {
                SetWindowLong(game->toolDlg, DWL_USER, (long) spr);
                GetLastError();
                SendMessage(game->toolDlg, TOOL_SPRITE, (WPARAM) spr, 0);
                spr->selected = 0;
            }

            context = GetSubMenu(LoadMenu( thisProg, MAKEINTRESOURCE(IDR_SPRITE_CONTEXT) ),
                                 0);

            k = TrackPopupMenu(context,
                               TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_LEFTALIGN|TPM_TOPALIGN|TPM_NONOTIFY,
                               pt.x,
                               pt.y,
                               0,
                               hwnd,
                               0);

            if(k)
                DngSprContextMenu(hwnd, game, k, x, y);
        }
            break;

        case PICTURE_MOUSEMOVE:
 
            game->xPos = (lp & 0xFFFF) >> 3;
            game->yPos = lp >> 0x13;

            if(GetWindowLong(game->toolDlg, DWL_USER) != 0)
                SendMessage(game->toolDlg, SPRITE_MOVE, game->xPos, game->yPos);

            x = game->xPos;
            y = game->yPos * 0x40;

            index = GetByte(game->dngObjs->BG_Attr[1], x + y);
            itoa(index, dummy, 0x10);
            strupr(dummy);

            SetDlgItemText(hwnd, ID_DungAttrNum + (game->index * 0x100), dummy);
        
            index = (x + y + 0x7F2000);
            itoa(index, dummy, 0x10);
            strupr(dummy);

            SetDlgItemText(hwnd, ID_DungAttrAddress + (game->index * 0x100), dummy);

            index = (2*(x + y)) + 0x7E2000;
            itoa(index, dummy, 0x10);
            strupr(dummy);

            SetDlgItemText(hwnd, ID_DungTilemapAddress + (game->index * 0x100), dummy);

            index = Get2Bytes(game->BG_Map[1], 2*(x + y));
            itoa(index, dummy, 0x10);
            strupr(dummy);

            SetDlgItemText(hwnd, ID_DungTilemapNum + (game->index * 0x100), dummy); 
      
            break;

        case PICTURE_TIMER:

            if(!game->pictWin)
                break;

            if(!game->active)
                break;

            game->aniTimer %= 9;

            if(game->aniTimer == 0)
            { 
                // Blit 0x20 animated tiles to vram
                CopyBuffer(game->vram, game->aniVram,
                           0x7600,      0x400 * game->aniFrame,
                           0x400);
            
                // Then decode them so we can actually use them
                DecodeTiles(game, 0x1B0, 0x1CF); 

                DrawTiles(game);

                game->aniFrame++;
                game->aniFrame %= 3;
            }
        
            game->aniTimer++;
      
            b = (ManagedBitmap*) GetWindowLong(game->pictWin, 0);

            stride = b->header->biWidth * 4;
            x = (game->xPos << 3) * 4;
            y = ((b->header->biHeight - 1 - (game->yPos << 3)) * stride);

            game->cursorPal[1] = game->cursorPal[game->cursorIndex++];
            game->cursorIndex %= 0x10;
        
            RedrawTile(b, game->xPrev, game->yPrev, game);
        
            BlitTranslucent(b, x + y , stride, &game->cursor, &game->cursorPal, 0, 0);
            BlitTranslucent(b, x + 0x20 + y , stride, &game->cursor, &game->cursorPal, 0, 0);
            BlitTranslucent(b, x + y - (stride * 8) , stride, &game->cursor, &game->cursorPal, 0, 0);
            BlitTranslucent(b, x + 0x20 + y - (stride * 8), stride, &game->cursor, &game->cursorPal, 0, 0);

            game->xPrev = game->xPos;
            game->yPrev = game->yPos; 

            i = (u32) InvalidateRect(game->pictWin, NULL, true);

            break;

        case WM_MOUSEHOVER:

            test = test;

            break;

        case WM_COMMAND:
        // This is nonconstant, so the case expression doesn't work.
            if(wp == (ID_DungHeaderButton | (game->index * 0x100) ) )
            {    
                GetDlgItemText(hwnd, ID_DungAttrNum + (game->index * 0x100), dummy, 256);

                //game->currentDungRoom = atoi(dummy);

                DialogBoxParam(thisProg,
                               MAKEINTRESOURCE(IDD_HEADER),
                               hwnd,
                               DungHeaderProc,
                               0);

            }

            if(wp == (ID_DungJumpButton | (game->index * 0x100) ) )
            {
                SendMessage(hwnd, DUNG_SAVE, 0, 0);

                if(game->currentDungRoom == 0x13F)
                    game->currentDungRoom = 0;
                else
                    game->currentDungRoom++;

                SendMessage(hwnd, DUNG_CHANGE, wp, lp);
            }

            if(wp == (ID_DungRoomButton | (game->index * 0x100) ) )
            {
                SendMessage(hwnd, DUNG_SAVE, 0, 0);

                if(game->currentDungRoom == 0)
                    game->currentDungRoom = 0x13F;
                else
                    game->currentDungRoom--;

                SendMessage(hwnd, DUNG_CHANGE, wp, lp);
            }

            if(wp == (ID_DungSearchButton | (game->index * 0x100) ) )
            {
                SendMessage(hwnd, DUNG_SEARCH, wp, lp);
            }
        
            if(wp == (ID_DungLayer1Radio | (game->index * 0x100) ) )
            {
                //SendMessage(hwnd, DUNG_SEARCH, wp, lp);
            }

            if(wp == (ID_DungLayer2Radio | (game->index * 0x100) ) )
            {
                //SendMessage(hwnd, DUNG_SEARCH, wp, lp);
            }

            break;

        case PICTURE_PAINT:
        
            memDC = (HDC) wp;
            picDC = (HDC) lp;

            DrawDungeonSprites(game, memDC, picDC);

            break;
  
        case 0: case 1:

            wp = wp;

            break;

        default:

            msg = msg;

            //DefWindowProc(hwnd, msg, wp, lp);

            break;       
        }

        return 0;
    }

//********************************************************

    void DrawDungeonSprites(zgPtr game, HDC memDC, HDC picDC)
    {
        u32 x = 0, y = 0, i = 1;
        static u8 byte = 0;

        char temp[0x100];

        SprStruct *sprites = &game->dngObjs->sprites[0];
        SprStruct *overlords = &game->dngObjs->overlords[0];
        SprStruct *spr = (SprStruct*) GetWindowLong(game->toolDlg, DWL_USER);

        RECT meh;
        HBITMAP oldBM = NULL;
        HBITMAP picBM = NULL;
        HDC other = CreateCompatibleDC(picDC);
        
        HPEN oldP = NULL;
        HBRUSH oldB = NULL;

        // P = pen, B = brush
        static HPEN sprP = CreatePen(PS_SOLID, 1, RGB(0x00,0xFF,0x00));
        static HBRUSH sprB = CreateSolidBrush(RGB(0xFF,0x00,0x00));

        static HPEN overP = CreatePen(PS_SOLID, 1, RGB(0x00,0x00,0xFF));
        static HBRUSH overB = CreateSolidBrush(RGB(0x00,0xFF,0x00));

        static BLENDFUNCTION bf = { AC_SRC_OVER,
                                    0,
                                    0xFF,
                                    AC_SRC_ALPHA } ;

        static HBITMAP yay = LoadBitmap(thisProg, MAKEINTRESOURCE(IDB_BITMAP1));
        static HBITMAP mask = LoadBitmap(thisProg, MAKEINTRESOURCE(IDB_BITMAP2));
        static HBITMAP bigKey = LoadBitmap(thisProg, MAKEINTRESOURCE(IDB_BITMAP3));
        static HBITMAP bigKeyMask = LoadBitmap(thisProg, MAKEINTRESOURCE(IDB_BITMAP4));

        // initializae the rectangle for the filloperation
        meh.top  = 0;  meh.bottom = 15;
        meh.left = 0;  meh.right  = 15;

        // Get a bitmap that works with the screen's DC
        // and color the 16x16 bitmap all black
        picBM = CreateCompatibleBitmap(picDC, 16, 16);
        oldBM = (HBITMAP) SelectObject(other, picBM);
        FillRect(other, &meh, (HBRUSH) GetStockObject(BLACK_BRUSH));

        oldP = (HPEN) SelectObject(other, sprP);
        oldB = (HBRUSH) SelectObject(other, sprB);

        i = GetLastError();

        meh.top = y; meh.bottom = y + 16;
        meh.left = x; meh.right = x + 256;

        SelectObject(other, sprP);
        SelectObject(other, sprB);
        Ellipse(other, 0, 0, 15, 15);
    
        for(i = 0; i < 16; i++)
        {
            if(game->dngObjs->sprIndices[i] >= 16)
                continue;
        
            x = sprites[i].x;
            y = sprites[i].y;

            if(AlphaBlend(memDC, x, y, 16, 16, other, 0, 0, 16, 16, bf))
                meh.top = meh.top;

            if(sprites[i].key == 1)
            {
                picBM = (HBITMAP) SelectObject(other, mask);
                BitBlt(memDC, x - 4, y, 16, 16, other, 0, 0, SRCAND);

                SelectObject(other, yay);
                BitBlt(memDC, x - 4, y, 16, 16, other, 0, 0, SRCPAINT);

                SelectObject(other, picBM);
            }
            else if(sprites[i].key == 2)
            {
                picBM = (HBITMAP) SelectObject(other, bigKeyMask);
                BitBlt(memDC, x - 5, y, 16, 16, other, 0, 0, SRCAND);

                SelectObject(other, bigKey);
                BitBlt(memDC, x - 5, y, 16, 16, other, 0, 0, SRCPAINT);

                SelectObject(other, picBM);
            }


            if( (&sprites[i] == spr) && spr)
            {
                SetBkColor(memDC, 0xC02000);
                SetBkMode(memDC, OPAQUE);
            }
            else
                SetBkMode(memDC, TRANSPARENT); 

            strcpy( temp, SprStrings[sprites[i].type] );
            
            SetTextColor(memDC, RGB(0x00, 0x00, 0x00));
            TextOut(memDC, x+9, y+1, temp, strlen((const char*) temp));
            SetTextColor(memDC, RGB(0xFF, 0xFF, 0xFF));
            TextOut( memDC, x+8, y, temp, strlen((const char*) temp) );
        }

        SelectObject(other, overP);
        SelectObject(other, overB);
        Ellipse(other, 0, 0, 15, 15);

        for(i = 0; i < game->dngObjs->numOverlords; i++)
        {
            x = overlords[i].x;
            y = overlords[i].y;

            if(AlphaBlend(memDC, x, y, 16, 16, other, 0, 0, 16, 16, bf))
                meh.top = meh.top;

            if( (&overlords[i] == spr) && spr)
            {
                SetBkColor(memDC, 0xC02000);
                SetBkMode(memDC, OPAQUE);
            }
            else
                SetBkMode(memDC, TRANSPARENT); 
            strcpy( temp, OverStrings[overlords[i].type] );
            
            SetTextColor(memDC, RGB(0x00, 0x00, 0x00));
            TextOut(memDC, x+9, y+1, temp, strlen((const char*) temp));
            SetTextColor(memDC, RGB(0xFF, 0xFF, 0xFF));
            TextOut( memDC, x+8, y, temp, strlen((const char*) temp) );
        }

        SelectObject(other, oldP);
        SelectObject(other, oldB);
        SelectObject(other, oldBM);

        DeleteDC(other);
        DeleteObject(picBM);

        bf.SourceConstantAlpha = ((byte++) / 4) + 0xC0 ;
    
        return;
    }

//********************************************************

    void LoadSprites(zgPtr game)
    {
        u32 x = 0, y = 0, type = 0, i = 1;
        u32 raw0 = 0, raw1 = 0, raw2 = 0;
        u32 numSprites = 0; 
        u32 numOverlords = 0;

        bool errFlag = false;

        bufPtr sprData = &game->currentFile->spriteData[game->currentDungRoom];
        bufPtr adjustSprData = NULL;

        SprStruct* spr  = game->dngObjs->sprites;
        SprStruct* over = game->dngObjs->overlords;

        u32 *sprIndices  = (u32*) &game->dngObjs->sprIndices[0];
        u32 *overIndices = (u32*) &game->dngObjs->overIndices[0];

        // ------------------------------------------------------

        for(i = 0; i < 16; i++)
            sprIndices[i] = -1;

        for(i = 0; i < 8; i++)
            overIndices[i] = -1;

        for(i = 1; ; )
        {
            if(i > sprData->length)
                break;

            if((numSprites == 16 ) || (numOverlords == 8))
                break;

            raw0 = GetByte(sprData, i);
            y = (raw0 & 0x1F) << 4;

            raw1 = GetByte(sprData, i + 1);
            x = (raw1 & 0x1F) << 4;

            raw2 = GetByte(sprData, i + 2);
            type = raw2;

            i += 3;

            if(raw0 == 0xFF)
                break;

            // It's a key of some sort
            if(raw2 == 0xE4)
            {
                if(raw0 == 0xFE)
                {
                    // normal key that is attached to a sprite

                    if(numSprites == 0)
                    {
                        errFlag = true;
                        continue;
                    }
       
                    spr[numSprites - 1].key = 1;
                }
                else if(raw0 == 0xFD)
                {
                    // It's a big key attached to a sprite.

                    if(numSprites == 0)
                    {
                        errFlag = true;
                        continue;
                    }

                    spr[numSprites - 1].key = 2;
                }
                else
                    goto normalSprite; // normal key that is visible initially
            }
            else if(raw1 < 0xE0)
            {
                // It's a normal sprite
    normalSprite:

                if(numSprites == 16)
                { errFlag = true; continue; }

                spr[numSprites].x = x;
                spr[numSprites].y = y;

                if( (type > 0xF2) || (type == 0x03) )
                    errFlag = true;

                spr[numSprites].type = type;
                spr[numSprites].key = 0;
                spr[numSprites].floor = (raw0 & 0x80) ? 1 : 0;
                spr[numSprites].subType = (((raw0 & 0x60) >> 2) | (raw1 >> 4)) >> 1;
                spr[numSprites].flag = 0;
                sprIndices[numSprites] = numSprites;
     
                numSprites++;
            }
            else
            {
                // It's an overlord
          
                if(numOverlords == 8)
                { errFlag = true; continue; }

                over[numOverlords].x = x;
                over[numOverlords].y = y;

                if(type > 0x1A)
                    errFlag = true;

                over[numOverlords].type = type;
                over[numOverlords].key = 0;
                over[numOverlords].floor = (raw0 & 0x80) ? 1 : 0;
                over[numOverlords].subType = 0;
                over[numOverlords].flag = 1;
                overIndices[numOverlords] = numOverlords;

                numOverlords++;
            }
        }

        if(errFlag == true)
            errFlag = true;

        game->dngObjs->numSprites = numSprites;
        game->dngObjs->numOverlords = numOverlords;

        adjustSprData = RepackSprites(game->dngObjs);

        ResizeBuffer(sprData, adjustSprData->length);
        CopyBuffer(sprData, adjustSprData, 0, 0, adjustSprData->length);

        DeallocBuffer(adjustSprData);
   
        return;
    }

//*****************************************************

    bufPtr RepackSprites(DngObjStruct *d)
    {
        u32 *indicS = &d->sprIndices[0];
        SprStruct *s = &d->sprites[0];

        u32 *indicO = &d->overIndices[0];
        SprStruct *o = &d->overlords[0];

        // sprite data shouldn't ever get this big to begin with
        // just a nice round number to start with, then size down
        bufPtr saveSpr = CreateBuffer(0x100);
        SprStruct *spr = NULL;

        u32 numKeys = 0;
        u32 i = 0; // iterator
        u32 j = 0; // pointer within saveSpr
        u32 raw0 = 0, raw1 = 0, raw2 = 0;

        j = 1;

        for(i = 0; i < 16; i++)
        {
            if(indicS[i] >= 16)
                continue;

            // get a pointer to the sprite's struct
            spr = &s[indicS[i]];

            raw0 =  ( (spr->y & 0x1F0) >> 4 );
            raw0 |= ( (spr->subType & 0x0C) << 3 );
            raw0 |= spr->floor ? 0x80 : 0;

            raw1 =  ( (spr->x & 0x1F0) >> 4 );
            raw1 |= ( (spr->subType & 0x03) << 5 );

            raw2 = (spr->type & 0xFF) % 0xF3;

            PutByte(saveSpr, j, raw0);
            PutByte(saveSpr, j+1, raw1);
            PutByte(saveSpr, j+2, raw2);
            j += 3;

            if(spr->key)
            {
                if(numKeys >= 2)
                    continue;

                PutByte(saveSpr, j, (spr->key == 2) ? 0xFD : 0xFE);
                PutByte(saveSpr, j+1, 0);
                PutByte(saveSpr, j+2, (spr->key == 2) ? 0xE5 : 0xE4);
                j += 3; numKeys++;
            }
        }

        for(i = 0; i < 8; i++)
        {
            if(indicO[i] >= 8)
                continue;

            // get a pointer to the sprite's struct
            spr = &o[indicO[i]];

            raw0 =  ( (spr->y & 0x1F0) >> 4 );
            raw0 |= ( (spr->subType & 0x0C) << 3 );
            raw0 |= spr->floor ? 0x80 : 0;

            raw1 =  ( (spr->x & 0x1F0) >> 4 );
            raw1 |= 0xE0; // indicates its an overlord

            raw2 = (spr->type & 0xFF) % 0x1B;

            PutByte(saveSpr, j, raw0);
            PutByte(saveSpr, j+1, raw1);
            PutByte(saveSpr, j+2, raw2);
            j += 3;
        }

        PutByte(saveSpr, j++, 0xFF);

        ResizeBuffer(saveSpr, j);

        return saveSpr;
    }

//********************************************************

    void SaveRoom(zgPtr game)
    {
        int a = 0;
        bool sprChange = false;

        DngObjStruct *d = game->dngObjs;

        bufPtr oldSpr = &game->currentFile->spriteData[game->currentDungRoom];
        bufPtr newSpr = RepackSprites(d);

        // if the sprite data is not identical, it has changed
        if(!CompareBuffer(oldSpr, newSpr))
        {
            sprChange = true;

            if(MessageBox(0, "Sprites have changed. Save?\n", 0, MB_YESNO) == IDYES)
            {
                ResizeBuffer(oldSpr, newSpr->length);
                CopyBuffer(oldSpr, newSpr, 0, 0, newSpr->length);
            }
        }

        DeallocBuffer(newSpr);

        return;
    }

// **********************************************************

    int CALLBACK DungHeaderProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        // declarations
        zgPtr game = 0;
        unsigned int index = 0;
        BM_DungeonHeader *header;
        static HWND combo[10];
        static int edit[8];
        static char** strings[10];
        static int  numbers[8];
        static char dummy[256];
        static int  limits[8];

        int b = 0;

        HWND dummyHwnd = 0;
    
        // setup
        //FindGame(GetParent(dlg));
        game = (zgPtr) GetWindowLong(windowList[zgIndex],0);
        header = &(game->bm_dung_header);
   
        switch(msg)
        {
        case WM_INITDIALOG:
       
            // edit control values

            edit[0]  = IDC_DUNGPAL;
            edit[1]  = IDC_DUNGGFX;
            edit[2]  = IDC_DUNGSPR;
            edit[3]  = IDC_DUNGROOM1;
            edit[4]  = IDC_DUNGROOM2;
            edit[5]  = IDC_DUNGROOM3;
            edit[6]  = IDC_DUNGROOM4;
            edit[7]  = IDC_DUNGROOM5;

            numbers[0] = header->paletteIndex;
            numbers[1] = header->graphicsNum;
            numbers[2] = header->spriteGfx;
            numbers[3] = header->warpRoom;
            numbers[4] = header->staircaseRoom1;
            numbers[5] = header->staircaseRoom2;
            numbers[6] = header->staircaseRoom3;
            numbers[7] = header->staircaseRoom4;

            limits[0] = 0x48;
            limits[1] = 0x18;
            limits[2] = 0x100;
            limits[3] = 0x100;
            limits[4] = 0x100; 
            limits[5] = 0x100;
            limits[6] = 0x100;
            limits[7] = 0x100;

            // combobox values

            combo[0] = GetDlgItem(dlg, IDC_BG0);
            combo[1] = GetDlgItem(dlg, IDC_COLLISION);
            combo[2] = GetDlgItem(dlg, IDC_EFFECT);
            combo[3] = GetDlgItem(dlg, IDC_ROUTINE1);
            combo[4] = GetDlgItem(dlg, IDC_ROUTINE2);
            combo[5] = GetDlgItem(dlg, IDC_PLANE1);
            combo[6] = GetDlgItem(dlg, IDC_PLANE2);
            combo[7] = GetDlgItem(dlg, IDC_PLANE3);
            combo[8] = GetDlgItem(dlg, IDC_PLANE4);
            combo[9] = GetDlgItem(dlg, IDC_PLANE5);

            strings[0] = bg0_prop_str;
            strings[1] = collision_str;
            strings[2] = special_effect_str;
            strings[3] = special_routine_str;
            strings[4] = special_routine_str;
            strings[5] = plane_str;
            strings[6] = plane_str;
            strings[7] = plane_str;
            strings[8] = plane_str;
            strings[9] = plane_str;

            for(index = 0; index < 10; index++)
            {
                PopulateCombobox(combo[index], strings[index]);
            }

            SendMessage(combo[0], CB_SETCURSEL, header->bg0_props, 0);
            SendMessage(combo[1], CB_SETCURSEL, header->collision, 0);
            SendMessage(combo[2], CB_SETCURSEL, header->effect, 0);
            SendMessage(combo[3], CB_SETCURSEL, header->tag_1, 0);
            SendMessage(combo[4], CB_SETCURSEL, header->tag_2, 0);
            SendMessage(combo[5], CB_SETCURSEL, header->warpPlane, 0);
            SendMessage(combo[6], CB_SETCURSEL, header->staircasePlane1, 0);
            SendMessage(combo[7], CB_SETCURSEL, header->staircasePlane2, 0);
            SendMessage(combo[8], CB_SETCURSEL, header->staircasePlane3, 0);
            SendMessage(combo[9], CB_SETCURSEL, header->staircasePlane4, 0);

            if(header->lightsOut)
                CheckDlgButton(dlg,
                               IDC_DARKEN,
                               BST_CHECKED);
        
            for( index = 0; index < 8; index++)
            {
                itoa(numbers[index], dummy, 16);
                strupr(dummy);

                dummyHwnd = GetDlgItem(dlg, edit[index]);

                oldEditProc = GetWindowLong(dummyHwnd, GWL_WNDPROC);
                SetWindowLong(dummyHwnd, GWL_WNDPROC, (long) NewEditProc);
            
                SetDlgItemText(dlg, edit[index], dummy);
            }
        
            break;

        case WM_COMMAND:
    
            // This for loop checks to see if any of the edit boxes have changed
            // If they have, it checks to see what the maximum values are for that particular
            // edit box. If it's gone over the limit (if the user enters in too big a number)
            // then it will put it at the max value and put the cursor at the end of the 
            // editable area.
            for(index = 0; index < 8 ;)
            {
                b = 1;

                if(wParam == (edit[index] | (EN_CHANGE << 0x10)) )
                {
                    GetDlgItemText(dlg, edit[index], dummy, 256);
                    b = hextodec((u8*) dummy, strlen(dummy));

                    if( (b % limits[index]) != b)
                    {   
                        b = limits[index] - 1;
                        itoa(b, dummy, 16);
                        strupr(dummy);

                        SetDlgItemText(dlg, edit[index], dummy);

                        PostMessage(GetDlgItem(dlg, edit[index]),
                                    EM_SETSEL,
                                    strlen(dummy),
                                    strlen(dummy));
                    }                
                }

                index++;

                if(index == 8)
                    break;
            }

            switch(wParam)
            {

            case IDCANCEL:

                // Insert some code to put the header data back into the rom.

                b = MessageBox(dlg, "Save modified header?", "Save?", MB_YESNO);

                if(b == IDNO)
                    goto defaultCase;

                index = 0;

                header->bg0_props       = SendMessage(combo[index++], CB_GETCURSEL, 0, 0);
                header->collision       = SendMessage(combo[index++], CB_GETCURSEL, 0, 0);
                header->effect          = SendMessage(combo[index++], CB_GETCURSEL, 0, 0);
                header->tag_1           = SendMessage(combo[index++], CB_GETCURSEL, 0, 0);
                header->tag_2           = SendMessage(combo[index++], CB_GETCURSEL, 0, 0);
                header->warpPlane       = SendMessage(combo[index++], CB_GETCURSEL, 0, 0);
                header->staircasePlane1 = SendMessage(combo[index++], CB_GETCURSEL, 0, 0);
                header->staircasePlane2 = SendMessage(combo[index++], CB_GETCURSEL, 0, 0);
                header->staircasePlane3 = SendMessage(combo[index++], CB_GETCURSEL, 0, 0);
                header->staircasePlane4 = SendMessage(combo[index++], CB_GETCURSEL, 0, 0);

                index = 0;

                header->paletteIndex    = (u8) GetHexEditNum( GetDlgItem(dlg, edit[index++]) );
                header->graphicsNum     = (u8) GetHexEditNum( GetDlgItem(dlg, edit[index++]) );
                header->spriteGfx       = (u8) GetHexEditNum( GetDlgItem(dlg, edit[index++]) );
                header->warpRoom        = (u8) GetHexEditNum( GetDlgItem(dlg, edit[index++]) );
                header->staircaseRoom1  = (u8) GetHexEditNum( GetDlgItem(dlg, edit[index++]) );
                header->staircaseRoom2  = (u8) GetHexEditNum( GetDlgItem(dlg, edit[index++]) );
                header->staircaseRoom3  = (u8) GetHexEditNum( GetDlgItem(dlg, edit[index++]) );
                header->staircaseRoom4  = (u8) GetHexEditNum( GetDlgItem(dlg, edit[index++]) );

                header->lightsOut = IsDlgButtonChecked(dlg, IDC_DARKEN);

                if(header->lightsOut == BST_CHECKED)
                    header->lightsOut = 1;
                else
                    header->lightsOut = 0;

                RepackDungeonHeader(game, header, header->data);

                goto defaultCase; // Now go to the default handler b/c we need to close the window.

                break;

            default:

    defaultCase:

                DlgCommand(dlg, wParam, lParam);

                break;

            }

            break;

        }

        return 0;    
    }

// ***********************************************************************************************