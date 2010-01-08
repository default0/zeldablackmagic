// Black Magic. The editor that brings you Dark Prophecy.
// 
// All new all powerful Zelda 3 editing machine!!!
// Author MathOnNapkins, Copyright 2005, 2006

    //includes

    // 1. #include "Black Magic.h"

    #include "Globals.h"
    #include "Strings.h"
    #include "Core.h"
    #include "Black Magic.h"
    #include "Objects.h"
    #include "DungMode.h"
    #include "OverMode.h"
    #include "OverModeGui.h"
    #include "Music.h"
    #include <windows.h>
    #include "Limits.h"
    #include <direct.h>

    #include <stack>

// ===============================================================

    int do_nothing(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch(msg)
        {
        case BM_TEARDOWN:

            return 1;

            break;
        }

        return 0;
    }

// ===============================================================

    int APIENTRY WinMain(HINSTANCE hInstance,
                         HINSTANCE hPrevInstance,
                         LPSTR     lpCmdLine,
                         int       nCmdShow)
    {
        // Gives us a global handle to the program
        thisProg = hInstance;

        // This is mainly a debugging stage precaution.
        if(_CrtCheckMemory() == FALSE)
            MessageBox(0, "crt failure", "winmain", MB_OK);

        RegisterBMClasses();
    
        ZeroMemory(windowList, sizeof(windowList));

        if(!GetFile(&bmOFN, NULL))
            return 0;

        MainLoop();
 
	    return 0;
    }

// ===============================================================

    void MainLoop()
    {
    
        while( (GetMessage(&mainMsg, 0, 0, 0) > 0) && !quit)
	    {
	        if(mainMsg.message == WM_QUIT)
		        quit = 1;

		    TranslateMessage(&mainMsg);
		    DispatchMessage(&mainMsg);
		    
        }
    }

// ===============================================================

    void RegisterBMClasses()
    {
        // Registers classes for different kind of windows
        mainClass.cbSize = sizeof(WNDCLASSEX);
	    mainClass.style = 0; //CS_HREDRAW|CS_VREDRAW; // nothing fancy.
	    mainClass.lpfnWndProc = MainProc; // set the window procedure.
	    mainClass.cbClsExtra = 0;
	    mainClass.cbWndExtra = 0x10;
	    mainClass.hInstance = thisProg;
        mainClass.hIcon = LoadIcon( thisProg, MAKEINTRESOURCE(IDI_ICON1) );
        mainClass.hIconSm = LoadIcon( thisProg, MAKEINTRESOURCE(IDI_ICON1) );
	    mainClass.hCursor = LoadCursor(0,IDC_ARROW);
	    mainClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	    mainClass.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);
	    mainClass.lpszClassName="BMMAINCLASS";

        RegisterClassEx(&mainClass);  

        // Look for WM_DROPFILES
        // See DragQueryFile(), wPARAM will be the HDROP structure. lparam is zero.
    
        // Now, we need to register another class used for picture boxes.
        pictureClass.cbSize = sizeof(WNDCLASSEX);
	    pictureClass.style = 0; //CS_HREDRAW|CS_VREDRAW; // nothing fancy.
	    pictureClass.lpfnWndProc = PictureProc; // set the window procedure.
	    pictureClass.cbClsExtra = 0;
	    pictureClass.cbWndExtra = 0x10;
	    pictureClass.hInstance = thisProg;
        pictureClass.hIcon = 0;
        pictureClass.hIconSm = 0;
	    pictureClass.hCursor = LoadCursor(0,IDC_ARROW);
	    pictureClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	    pictureClass.lpszMenuName = 0;
	    pictureClass.lpszClassName = "PICTURE";

        RegisterClassEx(&pictureClass);
    }

// ===============================================================

    void MakeNewEditWindow()
    {
        HWND ngWnd;

        ngWnd = CreateWindowEx(WS_EX_LEFT|WS_EX_ACCEPTFILES|WS_EX_OVERLAPPEDWINDOW,
                               "BMMAINCLASS",
                               0,
                               WS_OVERLAPPEDWINDOW|WS_VSCROLL|WS_HSCROLL,
                               CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               500,
                               500,
                               0,
                               NULL,
                               thisProg,
                               newGamePtr);

        newGamePtr->subWindow = ngWnd;
        newGamePtr->pictWin = 0;

        SetWindowLong(ngWnd, 0, (long) newGamePtr);
        windowList[newGamePtr->index] = ngWnd;
    
        SetWindowText(ngWnd, (const char*) ToString(newGamePtr->romName));
        ShowWindow(ngWnd, 1);
        DrawMenuBar(ngWnd);
    }

// ===============================================================

    long _stdcall PictureProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        u32 test = 0;
        u32 width = 0;
        u32 height = 0;
        u32 numPixels = 0;
        u32 headerSize = sizeof(BITMAPINFOHEADER);

        u32 i = 0;
        u32 stride = 0;
        u32 update = 0;
        u32 timer = 0;

        static u32 error = 0;
        static u32 errorCode = 0;

        ManagedBitmap *b;
        WindowElement *we = NULL;

        PAINTSTRUCT ps;
        RECT windowRect;

        HDC pictureDC;
        HDC memoryDC;
        HBITMAP pictureBitmap;
        HBITMAP oldBitmap;

        switch(msg)
        {
        case WM_CREATE:
 
            b = (ManagedBitmap*) malloc(sizeof(ManagedBitmap));

            GetWindowRect(hwnd, &windowRect);

            we = (WindowElement*) lParam;

            width  = we->x;
            height = we->y;

            numPixels = width * height;
        
            b->memory = AllocBuffer();
            InitBuffer( sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * numPixels), b->memory );

            b->header = (BITMAPINFOHEADER*) (b->memory->contents);
            b->structure = (BITMAPINFO*) (b->memory->contents);

            b->header->biBitCount = 32;
            b->header->biClrImportant = 0;
            b->header->biClrUsed = 0;
            b->header->biCompression = BI_RGB;
            b->header->biHeight = height;
            b->header->biPlanes = 1;
            b->header->biSize = sizeof(BITMAPINFOHEADER);
            b->header->biSizeImage = 0;
            b->header->biWidth = width;
            b->header->biXPelsPerMeter = 0;
            b->header->biYPelsPerMeter = 0;

            b->structure->bmiHeader = *(b->header);

            b->pixels = GetSubBuffer(b->memory, headerSize);
            b->stride = (b->header->biWidth) * 4;
            b->bottom = b->stride * (b->header->biHeight - 1);

            for( ; i < numPixels ; i++)
            {
                Put3Bytes(b->pixels, i * 4, 0x000000);
            }

            i = SetWindowLong(hwnd, 0, (long) b);

            i = GetWindowLong(hwnd, 0);

            SetTimer(hwnd,
                     0,
                     1000/60,
                     0);

            //goto paint;

            break;

        case WM_TIMER:
        
            SendMessage(GetParent(hwnd),
                        PICTURE_TIMER,
                        wParam,
                        lParam);
        
            break;
    
        case WM_NCLBUTTONDBLCLK:
        case WM_NCLBUTTONDOWN:

            SendMessage(GetParent(hwnd),
                        WM_LBUTTONDOWN,
                        wParam,
                        lParam);

            return 0;

            break;
   
       case WM_NCRBUTTONDBLCLK:
       case WM_NCRBUTTONDOWN:

           SendMessage(GetParent(hwnd),
                       WM_RBUTTONDOWN,
                       wParam,
                       lParam);

           return 0;

           break;

       case WM_MOUSEMOVE:

            SendMessage(GetParent(hwnd),
                        PICTURE_MOUSEMOVE,
                        wParam,
                        lParam);

            break;

       case WM_LBUTTONDOWN:
        
           SendMessage(GetParent(hwnd),
                       PICTURE_LBUTTON_DOWN,
                       wParam,
                       lParam);

           break;

       case WM_LBUTTONUP:
        
           SendMessage(GetParent(hwnd),
                       PICTURE_LBUTTON_UP,
                       wParam,
                       lParam);

           break;

       case WM_SETCURSOR:

           return TRUE;

           break;

       case WM_RBUTTONDOWN:
        
           SendMessage(GetParent(hwnd),
                       PICTURE_RBUTTON_DOWN,
                       wParam,
                       lParam);

           break;


        case WM_ERASEBKGND:

           return 1;

           break;

        case WM_DESTROY:

            break;

        case WM_CHAR:
        {
            i = i;
       
            break;
        }

        case WM_PAINT:

            BeginPaint(hwnd, &ps);
        
            pictureDC = ps.hdc;
        
            b = (ManagedBitmap*) GetWindowLong(hwnd, 0);  

            memoryDC = CreateCompatibleDC(pictureDC);

            pictureBitmap = CreateCompatibleBitmap(pictureDC, 
                                                   b->header->biWidth, 
                                                   b->header->biHeight);
        
            oldBitmap = (HBITMAP) SelectObject(memoryDC, pictureBitmap);
       
            GetClientRect(hwnd, &windowRect);

            i = StretchDIBits(memoryDC,
                              0,
                              0,
                              b->header->biWidth, /// 2) + errorCode,
                              b->header->biHeight, /// 2) + errorCode,
                              0,
                              0,
                              b->header->biWidth,
                              b->header->biHeight,
                              (b->pixels->contents),
                              b->structure,
                              DIB_RGB_COLORS,
                              SRCCOPY);

            // allows the owner window to do any further modifications 
            // as desired after the main image gets blitted
            SendMessage(GetParent(hwnd),
                        PICTURE_PAINT,
                        (WPARAM) memoryDC,
                        (LPARAM) pictureDC);
            
            i = BitBlt(pictureDC,
                       0,
                       0,
                       b->header->biWidth,
                       b->header->biHeight,
                       memoryDC,
                       0,
                       0,
                       SRCCOPY);

            i = (u32) SelectObject(memoryDC, oldBitmap);


            //ReleaseDC(hwnd, pictureDC);
            DeleteDC(memoryDC);
            //DeleteObject(pictureBitmap);
            DeleteObject(pictureBitmap);

            EndPaint(hwnd, &ps);

            break;

        case WM_NCPAINT:

            return 0;

            break;

        /*default:

            SendMessage(GetParent(hwnd),
                        msg,
                        wParam,
                        lParam);

            break;
            */
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

// ===============================================================

    long _stdcall MainProc(HWND mainHwnd, UINT mainMsg, WPARAM mainWparam, LPARAM mainLparam)    
    {
        // handles the main window, which will load Zelda 3 files for editing.
    
        u32 source = 0;
        u32 err = 0;
        u32 windowOffset = 0;
        u32 i = 0;
        u32 j = 0;
        u32 sum = 0;
        u32 index = 0;
        u32 index2 = 0;
        u32 index3 = 0;
        u32 index4 = 0;
	    
        u32 scrollDirection;
        u32 dx = 0, dy = 0;
        u32 *dPtr = NULL;
    
        bufPtr rom = 0;
    
        zgPtr game;
        WEStruct *currentWES;

        static HWND container;
        static HWND button = 0;

        char dummy[256];
        WEStruct *temp;
    
        SCROLLINFO si;
    
        // ------------------------------------

        if(GetWindowLong(mainHwnd,0))
        {
            game = (zgPtr) GetWindowLong(mainHwnd, 0);
            zgIndex = game->index;
            
            currentWES  = &(game->currentWES); 

            temp = currentWES;
        }
        else
        {
            game = NULL;
        }

        switch(mainMsg)
        {
   
        case WM_CREATE:
        
            SetWindowLong(mainHwnd, 0, (long) newGamePtr);

            game = (zgPtr) GetWindowLong(mainHwnd, 0);
            game->currentFunc = do_nothing;

            for(index = 0; index < 64; index++)
                game->cursor[index] = 0x01;

            game->cursorIndex = 0;

            game->cursorPal[0x00] = 0x000020F8;
            game->cursorPal[0x01] = 0x000030F0;
            game->cursorPal[0x02] = 0x000040E0;
            game->cursorPal[0x03] = 0x000050D0;
            game->cursorPal[0x04] = 0x000060C0;
            game->cursorPal[0x05] = 0x000070B0;
            game->cursorPal[0x06] = 0x000080A0;
            game->cursorPal[0x07] = 0x00007090;
            game->cursorPal[0x08] = 0x00006080;
            game->cursorPal[0x09] = 0x00005090;
            game->cursorPal[0x0A] = 0x000040A0;
            game->cursorPal[0x0B] = 0x000030B0;
            game->cursorPal[0x0C] = 0x000020C0;
            game->cursorPal[0x0D] = 0x000010D0;
            game->cursorPal[0x0E] = 0x000000E0;
            game->cursorPal[0x0F] = 0x000020F0;

            currentWES = 0;

            return 0;

            break;

        case WM_NCCREATE:

            return 1;

            break;

        case WM_ACTIVATE:

            goto bm_default; // let the handler function do this

            break;

        case WM_SIZE:
        {
            if(!temp->numElements)
                break;

            // set up horizontal scroll bar
            si.cbSize = sizeof(si);
            si.fMask  = (SIF_PAGE | SIF_POS | SIF_RANGE | SIF_DISABLENOSCROLL);

            GetScrollInfo(mainHwnd, SB_HORZ, &si);

            si.nMin   = INT_MAX;
            si.nMax   = INT_MIN;
            i = 0;

            while(i != temp->numElements)
            {
                si.nMax = max( si.nMax, (temp->WESet[i].x + temp->WESet[i].nWidth) );
                si.nMin = min( si.nMin, temp->WESet[i].x);
                i++;
            }
        
            si.nPage = LOWORD(mainLparam);
            si.nPage = min(si.nMax, si.nPage);

            si.nMax  -= si.nMin;
            si.nMin  -= si.nMin;

            si.nTrackPos = 0;

            SetScrollInfo(mainHwnd, SB_HORZ, &si, TRUE);

            // set up vertical scroll bar        
            si.cbSize = sizeof(si);
            si.fMask  = (SIF_PAGE | SIF_RANGE | SIF_POS | SIF_DISABLENOSCROLL);

            GetScrollInfo(mainHwnd, SB_VERT, &si);

            si.nMax   = INT_MIN;
            si.nMin   = INT_MAX;
            i = 0;

            while(i != temp->numElements)
            {
                si.nMax = max( si.nMax, (temp->WESet[i].y + temp->WESet[i].nHeight) );
                si.nMin = min( si.nMin, temp->WESet[i].y);
                i++;
            }

            si.nPage = HIWORD(mainLparam);
            si.nPage = min(si.nMax, si.nPage);

            si.nMax  -= si.nMin;
            si.nMin  -= si.nMin;

            si.nTrackPos = 0;

            SetScrollInfo(mainHwnd, SB_VERT, &si, TRUE);


            break;
        }
        case WM_VSCROLL:

            scrollDirection = SB_VERT;
            dPtr = &dy;

            goto common;

        case WM_HSCROLL:
        {
            scrollDirection = SB_HORZ;
            dPtr = &dx;

        common:

            si.cbSize = sizeof(si);
            si.fMask  = SIF_ALL;

            GetScrollInfo(mainHwnd, scrollDirection, &si);

            i = LOWORD(mainWparam);

            switch(i)
            {
                case SB_LINEUP:
                    (*dPtr) = 8; break;
                case SB_LINEDOWN:
                    (*dPtr) = -8; break;
                case SB_PAGEUP:
                    (*dPtr) = si.nPage; break;
                case SB_PAGEDOWN:
                    (*dPtr) = -si.nPage; break;
            }

            j = si.nPos;

            si.nPos -= (*dPtr);

            SetScrollInfo(mainHwnd,
                          scrollDirection,
                          &si,
                          true);

            GetScrollInfo(mainHwnd,
                          scrollDirection,
                          &si);

            (*dPtr) = j - si.nPos;

            i = 0;

            while(i != temp->numElements)
            {

                temp->WESet[i].x += dx;
                temp->WESet[i].y += dy;

                MoveWindow(temp->WESet[i].handle,
                           temp->WESet[i].x,
                           temp->WESet[i].y,
                           temp->WESet[i].nWidth,
                           temp->WESet[i].nHeight,
                           FALSE);

                i++;
            }   

            ScrollWindowEx(mainHwnd,
                           dx,
                           dy,
                           NULL,
                           NULL,
                           NULL,
                           NULL,
                           SW_ERASE | SW_INVALIDATE);
        
            break;
        }

        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:

            if(mainMsg == WM_LBUTTONDOWN)
                scrollDirection = 1;
            else
                scrollDirection = -1;

            i = 0;

            /*

            while(i != temp->numElements)
            {

                temp->WESet[i].x += scrollDirection;
                temp->WESet[i].y += scrollDirection;

                MoveWindow(temp->WESet[i].handle,
                        temp->WESet[i].x,
                        temp->WESet[i].y,
                        temp->WESet[i].nWidth,
                        temp->WESet[i].nHeight,
                        true);

                i++;
            }

            */

            //ScreenToClient(mainHwnd, 

            break;

        case WM_PAINT:

            //DrawBitmap(mainHwnd);

            break;
 
        case WM_CLOSE:

            goto quitting;

            break;

        case WM_DESTROY:
        case WM_QUIT:

            return DefWindowProc(mainHwnd, mainMsg, mainWparam, mainLparam);

            break;

        case WM_NCACTIVATE:

            goto def_proc;

            break;
        
        case WM_COMMAND:

            switch(mainWparam)
            {
            case IDC_LOADROM:

                GetFile(&bmOFN, mainHwnd);      

                break;

            case IDC_EXPANDROM:

                DialogBoxParam(thisProg, MAKEINTRESOURCE(IDD_EXPANDROM), mainHwnd, ExpandProc, 0);

                break;

            case IDC_ROMINFO:

                rom = game->image;

                for( index ; index < 0x200000; index++ )
                {
                    sum += GetByte(rom, index);

                    if( rom->error == 1)
                    {
                        rom->error = 0;
                        break;                    
                    }
                }

                sum &= 0xFFFF;

                wsprintf(msgBuffer, "%X", sum);

                MessageBox(mainHwnd, msgBuffer, "checksum", MB_OK);

                break;

            case IDC_DUNGEON:

                if(game->currentFunc != dung)
                { 
                    i = game->currentFunc(mainHwnd, BM_TEARDOWN, mainWparam, mainLparam);
                
                    if(i)
                        game->currentFunc = dung;
                    else
                        break;

                    game->currentDungRoom = 0;

                    SendMessage(mainHwnd, DUNG_INIT, mainWparam, mainLparam);
                }
        
                break;
    
            case IDC_OVERWORLD:

                if(game->currentFunc != overworld)
                {
                    i = game->currentFunc(mainHwnd, BM_TEARDOWN, mainWparam, mainLparam);
                                
                    if(i)
                        game->currentFunc = overworld;
                    else
                        break;

                    SendMessage(mainHwnd, OVER_INIT, mainWparam, mainLparam);
                }

                break;

            case ID_GRAPHICS_EXPORT:
            {
                char temp[10];
                char path[MAX_PATH];

                u32 a = 0, b = 0;
                u32 c = 0;

                HANDLE dumpFH;

                // -----------------------------

                chdir( (const char*) ToString(game->image) );

                i = _mkdir("gfx");
        
                // dump all sprite graphics
                for(i = 0; i < 0x100; ++i)
                {                
                    sprintf(path, "gfx\\spr_gfx_%x.bin", i);

                    ToFile(game->sprPacks[i], path);
                }

                // dump all background graphics
                for(i = 0; i < 0x100; ++i)
                {
                    sprintf(path, "gfx\\bg_gfx_%x.bin", i);

                    ToFile(game->bgPacks[i], path); 
                }
        
                // Output the 2bpp font graphics for the menus / dialogue
                ToFile(game->fontGfx, "gfx\\font_gfx.bin");

                // Output Link's 4bpp sprite graphics
                ToFile(game->linkGfx, "gfx\\link_gfx.bin");

                // Output mode7 graphics (to-do listed)
                ToFile(game->mode7Gfx, "gfx\\mode7_gfx.bin");

                MessageBox(mainHwnd, "Finished exporting", "success", MB_OK);
            
                break;
            }
            case ID_GRAPHICS_IMPORT:
            {
                char path[MAX_PATH];

                // ------------------------------

                chdir( (const char*) ToString(game->image) );

                // import all sprite packs
                for(i = 0; i < 0x100; ++i)
                {
                    sprintf(path, "gfx\\spr_gfx_%x.bin", i);

                    FromFile(game->sprPacks[i], path);
                }

                // import all background packs
                for(i = 0; i < 0x100; ++i)
                {
                    sprintf(path, "gfx\\bg_gfx_%x.bin", i);

                    FromFile(game->bgPacks[i], path);
                }

                FromFile(game->fontGfx,  "gfx\\font_gfx.bin");
                FromFile(game->linkGfx,  "gfx\\link_gfx.bin");
                FromFile(game->mode7Gfx, "gfx\\mode7_gfx.bin");

                MessageBox(mainHwnd, "Finished importing", "success", MB_OK);

                break;
            }
            case IM_DUNGEONS:

                ImportDungeons(game, thisProg);

                break;

            case EX_DUNGEONS:

                ExportDungeons(game, thisProg);

                break;

            case IM_OVERWORLD:
   
                ImportOverworld(game, thisProg);

                break;

            case EX_OVERWORLD:

                ExportOverworld(game, thisProg);

                break;

            case IDC_EXPORT_TEXT:

                DumpScript(game, thisProg);

                break;

            case IDC_EXPORT_MUSIC:

                ExportSpc(game);

                break;

            case IDC_IMPORT_TEXT:

                InsertScript(game, thisProg);

                break;

            case IDC_QUIT:
      
    quitting:

                SendMessage(mainHwnd, BM_TEARDOWN, 0, 0);

                if(SaveGame(mainHwnd) == IDCANCEL)
                {
                            // If the game is a valid file, "cancel"
                            // should do nothing.
                    if(game->image->length != 0)
                        return 0;
                }

                // Otherwise, delete this game structure off of the list.
                DeletePointer();
                //DestroyWindow(mainHwnd);
                game = 0;

                if(numGames == 0)
                    exit(0);

                goto def_proc;
                
                break;
        
            default:

                // run a handler that checks which editing mode we're in and
                // updates the screen accordingly. Might take a bit of machinery
                // to get working, but it will be worth it.
            
                // BM_COMMAND( );

                goto bm_default;

                break;
            }

            break;

    bm_default:

        default:
        
            if(!game)
                break;
            else
                game->currentFunc(mainHwnd, mainMsg, mainWparam, mainLparam);

            break;

        }

    def_proc:

        return DefWindowProc(mainHwnd, mainMsg, mainWparam, mainLparam);
    }

// ===============================================================

    int CALLBACK ExpandProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        HWND parent;
        int newSize = 0;
        int expansionIndex = 0;
        static int oldIndex = 0;
    
        zgPtr game = (zgPtr) GetWindowLong(windowList[zgIndex], 0);

        parent = GetParent(hwnd);

        if(!parent)
            EndDialog(hwnd, -1);

        switch(msg)
        {
        case WM_INITDIALOG:

            newSize = game->image->length;
        
            expansionIndex = (newSize / (_MEGABIT * 4) ) - 2 ;

            CheckDlgButton(hwnd, expansionList[expansionIndex], BST_CHECKED);

            oldIndex = expansionIndex;

            wsprintf(msgBuffer, "$%X Bytes", newSize);

            SetDlgItemText(hwnd, IDC_ROMSIZE, msgBuffer);

            break;
    
        case WM_COMMAND:
        
            switch(wParam)
            {

            case IDOK:

                for( expansionIndex = 0; expansionIndex < 7; expansionIndex++)
                {    
                    if( IsDlgButtonChecked(hwnd, expansionList[expansionIndex]) )
                        break;
                }

                if( expansionIndex >= 7)
                    EndDialog(hwnd, -1);

                newSize = (_4MEGABITS * expansionIndex) + _MEGABYTE;

                ExpandRom(&newSize, 0); // note the routine may change newSize.

                expansionIndex = (newSize / (_MEGABIT * 4) ) - 2;

                CheckDlgButton(hwnd, oldIndex, BST_UNCHECKED);
                CheckDlgButton(hwnd, expansionList[expansionIndex], BST_CHECKED);
            
                wsprintf(msgBuffer, "$%X Bytes", newSize);

                SetDlgItemText(hwnd, IDC_ROMSIZE, msgBuffer);

                break;
            
            default:

                DlgCommand(hwnd, wParam, lParam);

                break;
            }
                    
            break;

        default:

        

            break;
        }

        return 0; // dialogs can return 0 normally. windows are more complicated.
    }

// ===============================================================

    int DlgCommand(HWND dlg, WPARAM wParam, LPARAM lParam)
    {
        switch(wParam)
        {
        case IDCANCEL:

             EndDialog(dlg, -1);

            break;

        default:
    
            break;
        }

        return 0;
    }

// ===============================================================

    // The purpose of this procedure is to replace the default edit control with a control
    // That filters out input that isn't hexadecimal. Additionally, it converts lower case hexadecimal input to uppercase on the fly.
    int CALLBACK NewEditProc(HWND edit, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        int i = 0;
        int a = 0;
 
        char* mask = "0123456789ABCDEF";
        char* lowMask = "abcdef";
        char input;

        char test[256];

   

        if(msg == WM_CHAR)
        {
            input = (char) wParam;

            // the delete character
            if(input == 8)
                goto foundHex;
    
            GetWindowText(edit, test, 256);

            a = strlen(test);

            if(a >= 6)
                return 0;

            for(i = 0; i < 6; i++)
            {
                if(input == lowMask[i])
                {
                    input = mask[i + 10];
                    wParam = (WPARAM) input;
                }

            }   

            for(i = 0; i < 16; i++)
            {
                if(input == mask[i])
                    goto foundHex;
            }

            // In this case nonHex characters do absolutely nothing.
            return 0;
        }

    foundHex:

        return CallWindowProc((WNDPROC) oldEditProc, edit, msg, wParam, lParam);
    }

// ===============================================================

    int IsDuplicateRom(char compareBuf[MAX_PATH])
    {
        int counter = 0;

        zgPtr game;
        HWND  gameWindow;
    
        for( counter; counter < numGames; counter++)
        {
            gameWindow = windowList[counter];

            if(!gameWindow)
                return 0;

            game = (zgPtr) GetWindowLong(gameWindow, 0);

            if(strcmp(compareBuf, (const char*) game->romName->contents) == 0)
            {
                SetActiveWindow(gameWindow);

                // indicate we already know this file and it is currently open.
                return 1;
            }
        }

        // by default it is not duplicate. Innocent until proven guilty.
        return 0;
    }

// ===============================================================

    // Fulfills the role of a constructor

    void MakeNewGamePtr(char fileName[MAX_PATH],
                        HANDLE fileHandle,
                        u32 hasHeader,
                        u32 fileLength)
    {
        /* 
        Allocates a new entry into the gameList
        Allocates room for a name of the rom,
        and allocates a pointer to the buffer where the
        game image will be stored.
        */

        int dungSize = sizeof(WindowElement) * numDungElements;
        int overSize = sizeof(WindowElement) * numOverElements;
        int i = 0;

        unsigned long dummyRead = 0;

        // --------------------------

        numGames++;
    
        // allocate a new game pointer
        newGamePtr = (zgPtr) malloc(sizeof(ZeldaGame));

        // assign the game's index (out of a list of currently open files)
        newGamePtr->index = (numGames - 1);

        zgIndex = newGamePtr->index;

        // create a buffer to preserve the header
        newGamePtr->hasHeader = hasHeader;
        newGamePtr->headerBuf = CreateBuffer(0x200);
                
        // if the file actually has a header, preserve it            

        if(hasHeader)
        {
            // you should add exception / error handling to this at some point
            // like if(ReadFile(...) ; else {deallocate that shit; }
            // in fact it might be better to read the file before anything else
            ReadFile(fileHandle,
                     newGamePtr->headerBuf->contents,
                     0x200,
                     &dummyRead,
                     0 );

            fileLength -= 0x200;
        }

        newGamePtr->image = CreateBuffer(fileLength);

        ReadFile(fileHandle,
                 newGamePtr->image->contents,
                 fileLength,
                 &dummyRead,
                 0 );

        newGamePtr->zgFileHandle = fileHandle;

        newGamePtr->romName = CreateBuffer(MAX_PATH);
        strcpy( (char*) ToString(newGamePtr->romName), fileName);

        newGamePtr->dungeonWE = (WindowElement*) malloc(dungSize);
        newGamePtr->overWE    = (WindowElement*) malloc(overSize);

        memcpy(newGamePtr->dungeonWE, dungTemp, dungSize);
        memcpy(newGamePtr->overWE,    overTemp, overSize);

        OffsetWindowIds(newGamePtr->dungeonWE);
        OffsetWindowIds(newGamePtr->overWE);
   
        // When the window comes up, nothing will happen until we go into an editing mode first   
        newGamePtr->currentFunc = do_nothing;
    
        newGamePtr->currentWES.numElements = 0;
        newGamePtr->currentWES.WESet = 0;
    
        MakeNewEditWindow();

        return;
    }

// ===============================================================

    void OffsetWindowIds(WindowElement *we)
    {
        int counter = 0;
        int temp = (newGamePtr->index) * 0x100;

        // --------------------------------------

        for(counter = 0; ; counter++)
        {
            we[counter].hMenu = (HMENU) ((int) we[counter].hMenu + temp);

            // So the point of this is that the ID_DungPaletteNum, for 
            // example, for the first game opened will have value
            // 2000. The next game will have an hMenu value of 2100
            if((we[counter].endflag & END_FLAG) == END_FLAG)
                break;
        }

        return;
    }

// ===============================================================

    void AssignCurrentWES(WindowElement *we)
    {
        zgPtr game = (zgPtr) GetWindowLong(windowList[zgIndex], 0);

        // -------------------------------------------

        game->currentWES.WESet = we;
        game->currentWES.numElements = we[0].endflag & 0xFFF;

        SendMessage(windowList[zgIndex], WM_SIZE, 0, 0);

        return;
    }

// ===============================================================

    void ActivateCurrentWES(WEStruct *currentWES, HWND parent)
    {
        unsigned int i = 0;

        // --------------------------------------------

        for( i = 0; i < currentWES->numElements; i++)
        {
            if(currentWES->WESet[i].handle != NULL)
                continue;

            currentWES->WESet[i].parent = parent;

            currentWES->WESet[i].handle = CreateWindowEx(
                                                currentWES->WESet[i].exStyle,
                                                currentWES->WESet[i].className,
                                                currentWES->WESet[i].windowCaption,
                                                currentWES->WESet[i].style,
                                                currentWES->WESet[i].x,
                                                currentWES->WESet[i].y,
                                                currentWES->WESet[i].nWidth,
                                                currentWES->WESet[i].nHeight,
                                                currentWES->WESet[i].parent,
                                                currentWES->WESet[i].hMenu,
                                                thisProg,
                                                (LPVOID) &currentWES->WESet[i]);

             if(currentWES->WESet[i].handle == NULL)
                 MessageBox(parent, "problem", "problem", MB_OK);
               
        }   

        return;
    }

// ===============================================================

    // imported from SNESDisasm.cpp
    // opens a file dialog and retrieves an openfilename structure.
    // look out for a bug in WindowsNT 4.0 and the lStructSize of *ofn.

    int GetFile(OPENFILENAME *ofn, HWND win)
    {
        char tempFileName[MAX_PATH];

        u32 fLength = 0; 
        u32 i       = 0; // nothing but a work variable
        
        unsigned long dummyRead = 0; // Tells us how many bytes are read.

        HANDLE fHandle = 0;

        // ----------------------------------

	    ofn->lStructSize = sizeof(OPENFILENAME);
	    ofn->hwndOwner = win;
	    ofn->hInstance = thisProg;
	    ofn->lpstrFilter = "SNES roms\0*.SMC;*.SFC\0All files\0*.*\0";
	    ofn->lpstrCustomFilter = 0;
	    ofn->nFilterIndex = 1;
	    ofn->nMaxFile = MAX_PATH;
	    ofn->lpstrFile = tempFileName;
	    ofn->lpstrFile[0] = 0; // a particularity of using the open file dialogs.
	    ofn->lpstrFileTitle = NULL;
	    ofn->lpstrInitialDir = 0; // Sets to a default directory. (my documents?)
	    ofn->lpstrTitle = "Open Zelda 3 Rom";
	    ofn->Flags = OFN_FILEMUSTEXIST;
	    ofn->lpfnHook = 0; // ????
			    
        if(!GetOpenFileName(ofn)) 
	    {
            // if opening the file failed.
		    char* buffer = 0;

		    DWORD i = CommDlgExtendedError();

		    if(i)
		    {
			    wsprintf(buffer,"GUI error. Error: %08X",i);
			    MessageBox(0,buffer,"Bad error happened",MB_OK);
		    }

            return 0;
	    }
	    else
	    {
            // There's no point in opening a file that's open already
            if(IsDuplicateRom(tempFileName))
                return 0;

            // open the file in read mode
            fHandle = CreateFile(ofn->lpstrFile,
                                 GENERIC_READ,
                                 FILE_SHARE_READ,
                                 0,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL,
                                 0 );

            // if file handle is unabled to be created, return fail value
            if(!fHandle)
                return 0;

            fLength = GetFileSize(fHandle, 0);

            // checks for files larger than 4 megabytes (plus a header maybe)
            if( fLength > (0x400000 + 0x200) )
            {   
                MessageBox(0, 
                           "This rom is too large to be Zelda 3",
                           "fatal error",
                           MB_OK);

                return 0;
            }
            // checks for roms that are too small (less than 1 megabyte)
            else if( fLength < 0x100000 )
            {
                MessageBox(0, 
                           "This rom is too small to be Zelda 3",
                           "fatal error",
                           MB_OK);

                return 0;
            }
            //checks for irregular sized files and undersized files
            else if( fLength & 0x7FDFF )
            {                   
                i = MessageBox(0, 
                               "This file is irregular in size. It could be corrupt.\nHyrule Magic can cause this. Continue using?",
                               "potentially invalid file",
                               MB_YESNO );

                return 0; // temporary

                if(i == IDYES)
                {



                }
            }
            // here we expect normal sizes to be present
            else
            {
                i = fLength & 0x200 ? 1 : 0;

                MakeNewGamePtr(tempFileName, fHandle, i, fLength);           
            }

            // Note if the game had a header, I think the file pointer is already at 0x200
            // from the previous read.

            if(fLength > 0x100000)
            {
                 Check_BM_Header(newGamePtr);

                 newGamePtr->numRooms = 320;
                 newGamePtr->numEntrances = 0x85;
            }
            else
            {
                int newLength = 0x200000;

                // Expands the Rom to 2 megabytes.
                ExpandRom(&newLength, 1);
                    
                Create_BM_Header(newGamePtr);

                newGamePtr->numRooms = 320;
                newGamePtr->numEntrances = 0x85;
            }

            // Decompress all the graphics out of the rom so they're available at run time.
            newGamePtr->LoadAllGfx();

            newGamePtr->currentWES.numElements = 0;
            newGamePtr->currentWES.WESet = 0;

            newGamePtr->currentFile = &( newGamePtr->dungeonFile );

            LoadDungeonFile(newGamePtr, newGamePtr->currentFile);
            LoadDialogueFile(newGamePtr);

            for(i = 0; i < 2; ++i)
                newGamePtr->BG_Map[i] = CreateBuffer(0x2000);

            newGamePtr->cgram   = CreateBuffer(0x200);
            newGamePtr->aniVram = CreateBuffer(0x3000);
            newGamePtr->vram    = CreateBuffer(0x10000);

            newGamePtr->gi = (giPtr) malloc(sizeof(graphicsInfo));

            newGamePtr->overData = new OverData(newGamePtr->image, &newGamePtr->bm_Header);

            // Add code to ask user if they would care to pad out the game to make
            // it work. Perhaps also check to look for signs of Hyrule Magic's influence.

            CloseHandle(newGamePtr->zgFileHandle);

            SetWindowLong(newGamePtr->subWindow, 4, 1);
	    }	

	    return 1;
    }

// ===============================================================

    int SaveGame(HWND win)
    {
        int result;

        zgPtr game = (zgPtr) GetWindowLong(win, 0);

        // -------------------------------------------

        //if(FindGame(win) == -1)
        /*{
            // Means the data for the game file wasn't found in our list.
            // This is a bad error that should never happen theoretically.
            MessageBox(win, "Data Not Found!", "bad error", MB_OK);
        
            return IDCANCEL;
        }*/

        result = MessageBox(win, "Save Game?", "Black Magic Save", MB_YESNOCANCEL|MB_ICONWARNING);

        switch(result)
        {
        case IDCANCEL:

            return result;

            break;

        case IDYES:

            SaveDataToFile(game);

            return result;

            break;

        case IDNO:

            return result;

            break;

        default:

            break;
        }

        return IDCANCEL;
    }

// ===============================================================

    // When the user closes a window, we have to free up room in our game list.
    // We'll compact it down too to avoid bloating.

    void DeletePointer()
    {    
        int counter = zgIndex;

        // zgIndex is used as a global index to the game we need to be working on at the moment.    

        zgPtr game = (zgPtr) GetWindowLong(windowList[zgIndex], 0);

        if(game->image->length != 0)
        {
            DeallocBuffer(game->cgram);
            DeallocBuffer(game->aniVram);
            DeallocBuffer(game->vram);
        }
    
        DeallocBuffer(game->BG_Map[0]);
        DeallocBuffer(game->BG_Map[1]);

        DestroyBuffer(game->romName);
        DestroyBuffer(game->image);
    
        if(game->hasHeader)
            DestroyBuffer(game->headerBuf);

        if(zgIndex == (numGames - 1) )
        {
            // Handles the game being the last in the list.
            // Handles the case where there's only one game. In that case
            windowList[zgIndex] = 0;
        }
        else 
        {
            // Handles the rest by compacting the window List down.
            for(counter; counter < numGames - 1; counter++)
            {
                windowList[counter] = windowList[counter + 1];
                windowList[counter + 1] = NULL;
                game = (zgPtr) GetWindowLong(windowList[counter], 0);
                game->index--;
            }
        }

        numGames--;
    }

// ===============================================================

    int SaveDataToFile(zgPtr game)
    {
        unsigned long numBytesWritten;
    
        game->zgFileHandle = CreateFile((char*) game->romName->contents,
                                        GENERIC_WRITE,
                                        0,
                                        0,
                                        OPEN_ALWAYS,
                                        0,
                                        0);

        if( !(game->zgFileHandle) )
            return 0;

        SaveExtendedDataToFile(game);    

        if( game->hasHeader)
            WriteFile(game->zgFileHandle,
                      game->headerBuf->contents,
                      game->headerBuf->length,
                      &numBytesWritten,
                      0);


        WriteFile(game->zgFileHandle,
                  game->image->contents,
                  game->image->length,
                  &numBytesWritten,
                  0);

        SetEndOfFile(game->zgFileHandle);

        CloseHandle(game->zgFileHandle);

        STARTUPINFO sInfo;
        PROCESS_INFORMATION pi;
    
        char commandLine[0x400] = "";

        memset(&sInfo, 0, sizeof(sInfo));
        memset(&pi, 0, sizeof(pi));
        sInfo.cb = sizeof(sInfo); 

        sprintf(commandLine, "xkas.exe \".\\overworld\\owData.asm\" \"%s\"", (const char*) ToString(game->romName));

        if(CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pi))
        {
            WaitForSingleObject(pi.hProcess, 10000);
        }
        else
        {
            int i = GetLastError();


        }

        return 1;
    }

// ===============================================================

    void SaveExtendedDataToFile(zgPtr game)
    {
        // This organizes all the extended data that is written past the end
        // of the normal sized rom. This has to be done carefully so that
        // 1. no data crosses a bank boundary without being accounted for
        // 2. no RATS tags are overwritten (implement later)

        DungeonFile* dngFile = game->currentFile;
        Buffer temp1;
        bufPtr temp2;

        u32 offset1 = 0;
        u32 offset2 = 0;
        u32 offset3 = 0;
        u32 offset4 = 0;
    
        u32 index1 = 0;
        u32 index2 = 0;

        // Write the headers and update each pointer in the table
  
        temp2 = 0;

        offset1 = Get3Bytes(game->image, bm_header_loc);

        // start writing data right after where the bm_header will go
        // represents the start of where the new header data will go
        //offset2 = offset1 + sizeof(game->bm_Header);

        offset2 = 0x8000 + offset1 + sizeof(game->bm_Header);
   
   
        // represents the rom address of the beginnning of the header pointer table
        offset4 = CpuToRomAddr(game->bm_Header.dngHeaderOffset);

        game->bm_Header.dngHeaderBank = GetBank(RomToCpuAddr(offset2));
        PutByte(game->image, asm_header_bank, game->bm_Header.dngHeaderBank);
       
        // I put the headers somewhere I know will not overlap a bank. And the data is
        // small enough to fit in one bank anyway (1/16th the size roughly)
        for( index1 = index2 = 0; index1 < 320; index1++)
        {
            offset3 = (RomToCpuAddr(offset2) & 0xFFFF);
            Put2Bytes(game->image, offset4, offset3); offset4 += 2;
            CopyBufferWB(game->image, &dngFile->headerData, &offset2, &index2, 14);
        }

        // Now write in the object data
        index1 = 0;
        index2 = 0;
        offset3 = 0;
        offset4 = 0;
        temp2 = 0;

        // Retrieves the in-rom location of the dungeon obj pointer table
        offset4 = CpuToRomAddr(game->bm_Header.dngObjOffset);    
    
        for( index1 = 0; index1 < 320; index1++)
        {
            offset3 = RomToCpuAddr(offset2);
            temp2 = &(dngFile->objectData[index1]);
            index2 = 0;

            // Determine if a bank boundary would be crossed
            if( IsBankChange(offset3, temp2->length) )
            {
                // move into the next (program) bank
                offset3 = ((GetBank(offset3) + 1) << 0x10) | 0x8000;
                offset2 = CpuToRomAddr(offset3);
            }

            Put3Bytes(game->image, offset4, offset3); offset4 += 3;
            CopyBuffer(game->image, temp2, offset2, 0, temp2->length);

            offset2 += temp2->length;
        }

    // Write in the entrance Data

        // This is easy :)
        CopyBuffer(game->image, 
                   &dngFile->entrances.entranceBuffer,
                   game->bm_Header.dngEntranceOffset,
                   0,
                   dngFile->entrances.entranceBuffer.length);
   
    // Write in the chest data

        // The size of the Buffer
        index2 = dngFile->chestLocationData.length;
        offset3 = RomToCpuAddr(offset2);
        game->bm_Header.dngChestOffset = offset3;        
        temp2 = &(dngFile->chestLocationData);
    
        if( IsBankChange(offset3, index2) )
        {
            // move into the next (program) bank
            offset3 = ((GetBank(offset3) + 1) << 0x10) | 0x8000;
            offset2 = CpuToRomAddr(offset3);
        }

        Put2Bytes(game->image, asm_num_chests, index2);
        Put3Bytes(game->image, asm_chest_ref, offset3);

        index1 = 0;

        CopyBufferWB(game->image, temp2, &offset2, &index1, index2);
    
        // Saves dungeon sprite data to the file
        SaveSprites(game, &offset2);

        // Saves the main dialogue information to the file
        SaveText(game, &offset2);

        // Save Overworld Data
        SaveOverworldData(game, &offset2);

        // Compress any graphics that have been modified (via importing)
        SaveGraphics(game, &offset);        
        
        // Finally, write the updated BH_Header to file
        temp1.contents = (unsigned char*) &(game->bm_Header);
        temp1.length = sizeof(game->bm_Header);
        CopyBuffer(game->image, &temp1, offset1, 0, temp1.length);
        
        // All done

    }

// ===============================================================

    bool SaveText(zgPtr game, u32 *romOffset)
    {
        u32 b = 0;
        u32 textLen = 0;
        u32 pointerCpu = 0;
        u32 pointerRom = *romOffset;
        u32 dataCpu = 0;
        u32 dataRom = 0;
        u32 pointerTableSize = 0;
        u32 dummy = 0;
        bufPtr text = NULL;

        // calculate the pointer table size (each is 3 bytes)
        pointerTableSize = game->dialogueFile.numMsgs * 3;
        pointerCpu = RomToCpuAddr(pointerRom);

        if( IsBankChange(pointerCpu, pointerTableSize) )
        {
            pointerCpu = ( (GetBank(pointerCpu) + 1 ) << 0x10) | 0x8000;
            pointerRom = CpuToRomAddr(pointerCpu);
        }

        game->bm_Header.textOffset = pointerCpu;
        game->bm_Header.textNumMsgs = game->dialogueFile.numMsgs;

        Put3Bytes(game->image,
                  asm_text_ref,
                  pointerCpu);

        Put3Bytes(game->image,
                  asm_text_ref2,
                  pointerCpu + 2);

        dataRom = pointerRom + pointerTableSize;
        dataCpu = RomToCpuAddr(dataRom);

        for(b = 0; b < game->dialogueFile.numMsgs; b++, pointerRom += 3)
        {
            text = game->dialogueFile.data[b];
            textLen = text->length;
            dataCpu = RomToCpuAddr(dataRom);
            dummy = 0;

            if( IsBankChange(dataCpu, textLen) )
            {
                // move into the next (program) bank
                dataCpu     = ( (GetBank( dataCpu ) + 1) << 0x10) | 0x8000;
                dataRom    = CpuToRomAddr(dataCpu);
            }

            //store the CPU address to the pointer table
            Put3Bytes(game->image, pointerRom, dataCpu);

            CopyBufferWB(game->image,   text,
                         &dataRom,   &dummy,
                         text->length);        
        }

        *romOffset = dataRom;

        return true;
    }


// ===============================================================

    // Write in the Sprite data
    
    bool SaveSprites(zgPtr game, u32 *romOffset)
    {

        unsigned char sprite_hook_routine[] = { _SEP(0x30),
                                                _PEA(0xC1,0x72),
                                                _REP(0x30),
                                                _LDAA(0x04,0x8E),
                                                _TAX,
                                                _ASLA,
                                                _TAY,
                                                _SEP(0x20),
                                                _LDALX(0xFF,0xFF,0xFF),
                                                _PHA,
                                                _PLB,
                                                _REP(0x30),
                                                _TXA,
                                                _ASLA,
                                                _TAX,
                                                _LDALX(0x09, 0xD6, 0x2E),
                                                _JML(0x09,0xC2,0x9A),
                                                '\0'};

        const u32 bankTableSize = 0x140;
        u32 a = 0;
        u32 routineCpu = 0;
        u32 routineRom = *romOffset;
        u32 pointerCpu = 0;
        u32 pointerRom = 0;
        u32 dataCpu = 0;
        u32 dataRom = 0;
        u32 dummy = 0;

        Buffer temp1;
        bufPtr temp2;

        temp1.contents = sprite_hook_routine;
        temp1.length = strlen((char*) sprite_hook_routine);
 
        routineCpu = RomToCpuAddr(routineRom);

        if( IsBankChange(routineCpu, temp1.length) )
        {
            // move into the next (program) bank
            routineCpu = ((GetBank(routineCpu) + 1) << 0x10) | 0x8000;
            routineRom = CpuToRomAddr(routineCpu);
        }

        // This hooks the routine that calls the sprite loading function
        // (at 0x4C16E) and jumps it into extended data where we do some setup
        // of the data to change the data bank register
        PutByte(game->image, asm_sprite_hook, 0x5C);
        Put3Bytes(game->image, asm_sprite_hook + 1, routineCpu);

        pointerRom = routineRom + temp1.length;
        pointerCpu = RomToCpuAddr(pointerRom);

        if( IsBankChange(pointerCpu, bankTableSize) )
        {
            pointerCpu = ((GetBank(pointerCpu) + 1) << 0x10) | 0x8000;
            pointerRom = CpuToRomAddr(pointerCpu);
        }

        // this is the start of the banks table (each room has an associated bank
        // because the sprite data could be in more than one bank.)
        game->bm_Header.dngSpriteBanks = pointerRom;

        dataRom = pointerRom + bankTableSize;
        dataCpu = RomToCpuAddr(dataRom);

        game->bm_Header.dngSpriteOffset = pointerRom + bankTableSize;

        // This stores the address of the bank table into the routine
        Put3Bytes(&temp1, 
                  0x10, 
                  pointerCpu);

        // stores the routine itself to ROM
        CopyBufferWB(game->image, &temp1, &routineRom, &dummy, temp1.length);
       
        for(a = 0; a < 0x140 ; a++)
        {
            temp2 = &(game->currentFile->spriteData[a]);
            dataCpu = RomToCpuAddr(dataRom);

            if( IsBankChange(dataCpu, temp2->length) )
            {
                dataCpu = ( (GetBank(dataCpu) + 1 ) << 0x10 ) | 0x8000;
                dataRom = CpuToRomAddr(dataCpu);
            }

            // Store the bank entry
            PutByte(game->image,
                    game->bm_Header.dngSpriteBanks + a,
                    GetBank(dataCpu));
        
            // Store the pointer's absolute portion
            Put2Bytes(game->image,
                      asm_sprite_local + (a*2),
                      dataCpu);

            dummy = 0;

            CopyBufferWB(game->image,
                         temp2,
                         &dataRom,
                         &dummy,
                         temp2->length);
        }

        *romOffset = dataRom;

        return true;
    }

// ===============================================================

    BOOL CALLBACK MoveChildren(HWND hwndChild, LPARAM lParam)
    {
        RECT cRect;

        u32 scrollDireciton = (int) lParam >> 16;

        GetClientRect(hwndChild, &cRect);
    
        MoveWindow(hwndChild,
                   cRect.left + 20,
                   cRect.top + 20,
                   cRect.right - cRect.left,
                   cRect.bottom - cRect.top,
                   true);

        return 1;
    }

// ===============================================================

    void DrawBitmap(HWND win)
    {
        // apparently deprecated. consider removing (01/2009)

        /*int indic;

        HDC winDC = GetDC(win);
        BITMAP bm;

        SnesImage *testImage = gameList[zgIndex]->testImage;

        if(testImage)
            memcpy(bmInfoPtr->bmiColors, testImage, sizeof(SnesImage));

        mainBM = CreateDIBitmap(winDC, 
                                bmInfoHeadPtr,
                                CBM_INIT,
                                (bmInfoPtr)->bmiColors,
                                (bmInfoPtr),
                                DIB_RGB_COLORS);
    
        HBITMAP oldBM = (HBITMAP) SelectObject(winDC, mainBM);

        indic = GetObject(mainBM, sizeof(BITMAP), &bm);

        //HBITMAP oldBM = (HBITMAP) SelectObject(winDC, mainBM);

        indic = StretchDIBits(winDC,
                          50,
                          50,
                          64,
                          64,
                          0,
                          0,
                          8,
                          8,
                          bmInfoPtr->bmiColors,
                          bmInfoPtr,
                          DIB_RGB_COLORS,
                          SRCCOPY);

        SelectObject(winDC, oldBM);

    end:

        //DeleteDC(bmDC);
        DeleteObject(mainBM);

        */
    }

// ===============================================================

    void ExpandRom(int *argument, int force)
    {
        int newSize = *argument;
        int oldSize = 0;
        int newSizeMegabits = 0;
        int sizeDifference = 0;
        bufPtr gameImage;
    
        zgPtr game = (zgPtr) GetWindowLong(windowList[zgIndex], 0);
    
        newSizeMegabits = newSize / _MEGABIT;
             
        gameImage = game->image;
        oldSize = game->image->length;
                
        if( newSize > oldSize )
        {
            wsprintf(msgBuffer, "Expand ROM to %u Megabits?",newSizeMegabits);

            if(force)
                goto forceExpansion;

            if( MessageBox(0, msgBuffer, "Expand", MB_YESNO) == IDYES)
            {
            forceExpansion:

                sizeDifference = newSize - oldSize;

                if(ResizeBuffer(gameImage, newSize))
                    memset( (gameImage->contents) + oldSize, 0xFF, sizeDifference);

            }
        }
        else if( newSize == oldSize )
        {
            wsprintf(msgBuffer, "ROM is already %u Megabits", newSizeMegabits);

            MessageBox(0, msgBuffer, "Expand", MB_OK);
        }
        else
        {
            wsprintf(msgBuffer, "Shrink ROM to %u Megabits?",newSizeMegabits);

            if( MessageBox(0, msgBuffer, "Expand", MB_YESNO) == IDYES || force)
                ResizeBuffer(game->image, newSize);

                    // RATSSearch( )
                    // Add code here later to make sure critical data isn't being
                    // deleted. Probably use RATS tags to do that.
        }
    
        *argument = game->image->length;
    }

// ===============================================================

    void ChangeWES(WindowElement* we, HWND win, zgPtr game)
    {
        WEStruct *currentWES = &(game->currentWES);

        unsigned int index = 0;

        // Destroy all the old windows and controls.
        for( index = 0; index < currentWES->numElements; index++)
        {  
           DestroyWindow(currentWES->WESet[index].handle);
           currentWES->WESet[index].handle = NULL;               
        }

        currentWES->WESet = 0;
        currentWES->numElements = 0;

        AssignCurrentWES(we);
   
        ActivateCurrentWES(currentWES, win);  
    }

// ===============================================================

    void PopulateCombobox(HWND combobox, char** stringList)
    {
        unsigned int index = 0;

        // The empty string "" is used in my char**'s as a makeshift
        // null terminator.
        while(strcmp(stringList[index], "") != 0)
        {
            SendMessage(combobox,
                        CB_ADDSTRING,
                        0,
                        (LPARAM) stringList[index]);

            index++;
        }

        return;
    }

// ===============================================================
// End of Black Magic.cpp


