
#include "Black Magic.h"
#include "Globals.h"
#include "Core.h"
#include "OverMode.h"
#include "OverModeGui.h"
#include "Sprites.h"
#include "List.h"
#include "compresch_castlevania4.h"
#include "compresch_lttp.h"

// ===============================================================
// procedure for the main window

    int overworld(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
    {   
        u32 i = 0;

        zgPtr game = (zgPtr) GetWindowLong(hwnd, 0);
    
        WEStruct *wes = &(game->currentWES);
        ManagedBitmap *b = NULL;

        OverData *o = NULL;

        HDC memDC = (HDC) wp;
        HDC picDC = (HDC) lp;

        // ------------------------------

        if(game)
        {
            if(game->pictWin)
                b = (ManagedBitmap*) GetWindowLong(game->pictWin, 0);

            o = game->overData;
        }

        switch(msg)
        {
            case OVER_INIT:
            {

                ChangeWES(game->overWE, hwnd, game);

                game->pictWin = NULL;

                for(i = 0; i < wes->numElements; i++)
                {
                    if( !strcmp(wes->WESet[i].windowCaption, "OVERPIC") )
                    {
                        // If the caption name matched but the class name is incorrect, we must move on.
                        if( strcmp(wes->WESet[i].className, "PICTURE") )
                            continue;

                        game->pictWin = wes->WESet[i].handle; break;
                    }
                }

                InitOverMode(game);

                // now that graphics are ready and all that we can generate bitmaps for menus
                // and create this dialog
                game->toolDlg = CreateDialogParam(thisProg,
                                                  MAKEINTRESOURCE(IDD_OVER_TILES),
                                                  hwnd,
                                                  overTileDlgProc,
                                                  (long) game);

                game->active = 1;

                i = (u32) InvalidateRect(game->pictWin, NULL, true);
            }

            break;

            case OVER_SAVE:
    
                // needs implementation
                // SaveRoom(game);
                break;

            case OVER_CHANGE:
            {
                char temp[0x100];

                giPtr gi = game->gi;
            
                RECT r;

                // --------------------------------

                o->allEntr[o->area]     = o->areaEntr;
                o->allHoles[o->area]    = o->areaHoles;
                o->allExits[o->area]    = o->areaExits;
                o->allItems[0][o->area] = o->areaItems;
                o->allSpr[1][o->area]   = o->areaSpr;

                o->area = (u8) wp;
           
                o->LoadArea();

                if(o->area & 0x40)
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
                gi->backGfx1   = GetByte(game->image, CpuToRomAddr(0x00FC9C) + o->area);

                SendMessage(GetDlgItem(game->toolDlg, ID_AUX_BG_GFX),
                            CB_SETCURSEL,
                            (WPARAM) gi->backGfx1,
                            0);

                gi->spriteGfx0 = GetByte(game->image, CpuToRomAddr(0x00FA41) + o->area);

                sprintf(temp, "%0X", gi->spriteGfx0);
                SetDlgItemText(game->toolDlg, ID_AUX_SPR_GFX, temp);

                gi->backPal0   = 0;
                gi->spritePal0 = 0;
                gi->spritePal1 = 0;
                gi->spritePal2 = 0;

                AniDecompressOverworld(game);
                OverDecompress(game);
                LoadOverPalettes(game);

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

                DrawOW(game, b, o->map8Buf);

                break;
            }
            case PICTURE_PAINT:
            {        
                picDC = (HDC) lp;
                memDC = (HDC) wp;

                if(wp == 1)
                {



                }
        
                OwOnPicturePaint(game, picDC, memDC);    

                break;
            }
            case WM_KEYDOWN:
            {
                OwOnPictureKeydown(hwnd, wp, lp, game); break;
            }
            case WM_SETCURSOR:
            {
                return TRUE;

                break;
            }
            case WM_CHAR:
            {
                OwOnPictureChar(hwnd, wp, lp, game); break;
            }
            case PICTURE_MOUSEMOVE:
            {
                OwOnPictureMouseMove(hwnd, wp, lp, game); break;
            }
            case PICTURE_LBUTTON_DOWN:
            {
                OwOnPictureLButtonDown(hwnd, b, lp, game); break;
            }
            case PICTURE_LBUTTON_UP:
            {
                OwOnPictureLButtonUp(hwnd, b, lp, game); break;
            }
            case PICTURE_RBUTTON_DOWN:
            {
                OwOnPictureRButtonDown(hwnd, b, lp, game);

                u16 x = LOWORD(lp) >> 3;
                u16 y = HIWORD(lp) >> 3;

                /** game->overData->value = Get2Bytes(game->overData->map8Buf,
                                                  (x*2) + (y*0x100));*/

                break;
            }
            case BM_TEARDOWN:
            {
                o->allEntr[o->area]     = o->areaEntr;
                o->allHoles[o->area]    = o->areaHoles;
                o->allExits[o->area]    = o->areaExits;
                o->allItems[0][o->area] = o->areaItems;
                o->allSpr[1][o->area]   = o->areaSpr;

                break;
            }
            case PICTURE_TIMER:
            {
                if(!game->pictWin)
                {
                    break;
                }

                if(!game->active)
                {
                    MessageBox(0, "hurp", "a durp", MB_OK);
                    break;
                }

                game->aniTimer %= 9;

                if(game->aniTimer == 0)
                { 
                    // Blit 0x20 animated tiles to vram
                    CopyBuffer(game->vram, game->aniVram,
                               0x7800,      (0x1680) + (0x400 * game->aniFrame),
                               0x400);
            
                    // Then decode them so we can actually use them
                    DecodeTiles(game, 0x1C0, 0x1DF); 

                    game->aniFrame++;
                    game->aniFrame %= 3;

                    DrawOW(game, b, game->overData->map8Buf);

                    i = (u32) InvalidateRect(game->pictWin, NULL, TRUE);
                }
        
                game->aniTimer++;
        
                break;
            }

            default:

                break;
        }

        return 0;
    }

// ===============================================================

    static HPEN redPen          = CreatePen(PS_SOLID, 1, RGB(0xFF,0x00,0x00));
    static HPEN greenPen        = CreatePen(PS_SOLID, 1, RGB(0x00,0xFF,0x00));
    static HPEN blackPen        = CreatePen(PS_SOLID, 1, RGB(0x00,0x00,0x00));

    static HBRUSH whiteBrush    = CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));
    static HBRUSH redBrush      = CreateSolidBrush(RGB(0xFF, 0x00, 0x00));
    static HBRUSH purpleBrush   = CreateSolidBrush(RGB(0xFF, 0x00, 0xFF));
    static HBRUSH yellowBrush   = CreateSolidBrush(RGB(0xFF, 0xFF, 0x80));
    static HBRUSH blackBrush    = CreateSolidBrush(RGB(0x00, 0x00, 0x00));
    static HBRUSH transBrush    = (HBRUSH) GetStockObject(NULL_BRUSH);

// ===============================================================

    bool PaintOwObject(OverObj *obj, HDC memDC)
    {
        bool worked = false;

        RECT *r;

        HPEN pen, oldPen;
        HBRUSH brush, oldBrush;

        // -------------------------------------------

        pen     = greenPen;
        brush   = CreatePatternBrush(obj->hData);

        r = obj->GetRect();

        oldBrush = (HBRUSH)  SelectObject(memDC, brush);
        oldPen   = (HPEN)    SelectObject(memDC, pen);

        SetBrushOrgEx(memDC, obj->x, obj->y, NULL);

        worked = Rectangle(memDC, r->left, r->top, r->right, r->bottom) ? true : false;

        SelectObject(memDC, oldBrush);
        SelectObject(memDC, oldPen);
            
        // delete it b/c it was created with a bitmap that may change with each object
        DeleteObject(brush);

        return worked;
    }       

// ===============================================================

    void OwOnPicturePaint(zgPtr game, HDC picDC, HDC memDC)
    {
        BOOL worked = FALSE;

        u8 *buf = NULL;

        u32 i = 0;

        Entrance *entr  = NULL;

        OverData *o     = game->overData;
        
        RECT r;

        HBRUSH brush, oldBrush;
        HPEN   pen,   oldPen;

        // ------------------------------------

        switch(o->editMode)
        {
            case mode_drawtile:
            {
                break;
            }
            case mode_select:
            {
                OverObj *obj = o->selObj;

                if(obj->forming || obj->active)
                {
                    OverObjRect(o->selObj, &r);

                    brush   = transBrush;
                    pen     = greenPen;

                    oldBrush    = (HBRUSH)  SelectObject(memDC, brush);
                    oldPen      = (HPEN)    SelectObject(memDC, pen);

                    worked = Rectangle(memDC, r.left, r.top, r.right, r.bottom);

                    SelectObject(memDC, oldPen);
                    SelectObject(memDC, oldBrush);
                }

                obj = o->selObj2;

                if(obj->active)
                    PaintOwObject(obj, memDC);

                break;
            }
            
            case mode_stock:
            {
                if(!o->pObj)
                    break;

                PaintOwObject(o->pObj, memDC);

                break;
            }

            case mode_exit:
            {
                OverExit *exit = NULL;
                
                // ----------------------------------

                exit  = OverExit::GetHead(o->areaExits);
                buf   = o->exitBuf;
                brush = whiteBrush;
                
                SetTextColor(memDC, RGB(0x00, 0x00, 0x00));

                HFONT oldFont = (HFONT) SelectObject(memDC, courier);

                for(   ; exit != NULL; exit = exit->GetNext())
                {
                    sprintf(exit->label, "%04X", exit->room);

                    if(exit->selected)
                        pen   = greenPen;
                    else
                        pen   = redPen;

                    oldBrush = (HBRUSH)  SelectObject(memDC, brush);
                    oldPen   = (HPEN)    SelectObject(memDC, pen);

                    if((exit->doorType != over_exit_up) && (exit->doorType != over_exit_down))                  
                    {
                        r = *(exit->door->GetRect());
                    
                        worked = Ellipse(memDC, r.left, r.top, r.right, r.bottom);
                    }

                    r = *(exit->GetRect());

                    worked = RoundRect(memDC, r.left, r.top, r.right, r.bottom, 8, 80);

                    SetBkMode(memDC, TRANSPARENT);

                    if(exit->selected)
                        TextOut(memDC, r.left + 1, r.top, (const char*)  buf, 4);
                    else
                        TextOut(memDC, r.left + 1, r.top, exit->label, strlen(exit->label));

                    SelectObject(memDC, oldBrush);
                    SelectObject(memDC, oldPen);
                }

                SelectObject(memDC, oldFont);
                DeleteObject(oldFont);
                          
                break;
            }
            case mode_sprite:
            {
                buf   = o->sprBuf;
                brush = purpleBrush;
                
                SetTextColor(memDC, RGB(0xFF, 0xFF, 0xFF));

                HFONT oldFont = (HFONT) SelectObject(memDC, courier);

                OverSpr *s = o->areaSpr;

                for(   ; s != NULL; s = (OverSpr*) s->GetNext())
                {
                    //sprintf(s->label, "%02X", s->spriteNum);
                    sprintf(s->label, "%s", SprStrings[s->spriteNum]);

                    if(s->selected)
                        pen = greenPen;
                    else
                        pen = blackPen;

                    oldBrush = (HBRUSH)  SelectObject(memDC, brush);
                    oldPen   = (HPEN)    SelectObject(memDC, pen);

                    r = *(s->GetRect());

                    worked = Ellipse(memDC, r.left, r.top, r.right, r.bottom);

                    SetBkMode(memDC, TRANSPARENT);

                    /*
                    if(s->selected)
                        TextOut(memDC, r.left + 1, r.top, (const char*)  buf, 2);
                    else
                    // */
                        TextOut(memDC, r.left + 1, r.top, s->label, strlen(s->label));

                    DrawSprite(game, (ManagedBitmap*) GetWindowLong(game->pictWin, 0), 0x0068, 0x00, 0x10, 0x00, 0x00, false, false);
                    DrawSprite(game, (ManagedBitmap*) GetWindowLong(game->pictWin, 0), 0x0068, 0x08, 0x10, 0x00, 0x00, true, false);
                    DrawSprite(game, (ManagedBitmap*) GetWindowLong(game->pictWin, 0), 0x0078, 0x00, 0x18, 0x00, 0x00, false, false);
                    DrawSprite(game, (ManagedBitmap*) GetWindowLong(game->pictWin, 0), 0x0078, 0x08, 0x18, 0x00, 0x00, true, false);



                    r.left = 0;
                    r.right = 0x20;
                    r.top = 0;
                    r.bottom = 0x20;

                    InvalidateRect(game->pictWin, &r, FALSE);

                    SelectObject(memDC, oldBrush);
                    SelectObject(memDC, oldPen);
                }                

                SelectObject(memDC, oldFont);
                DeleteObject(oldFont);

                break;
            }
            case mode_item:
            {
                buf   = o->itemBuf;
                brush = redBrush;
                
                SetTextColor(memDC, RGB(0xFF, 0xFF, 0xFF));

                HFONT oldFont = (HFONT) SelectObject(memDC, courier);

                OverItem *item = o->areaItems;

                for(   ; item != NULL; item = (OverItem*) item->GetNext())
                {
                    //sprintf(item->label, "%02X", item->itemNum);
                    sprintf(item->label, "%s", ItemStrings[item->itemNum]);

                    if(item->selected)
                        pen = greenPen;
                    else
                        pen = blackPen;

                    oldBrush = (HBRUSH)  SelectObject(memDC, brush);
                    oldPen   = (HPEN)    SelectObject(memDC, pen);

                    r = *(item->GetRect());

                    worked = Ellipse(memDC, r.left, r.top, r.right, r.bottom);

                    SetBkMode(memDC, TRANSPARENT);

                    /*
                    if(item->selected)
                        TextOut(memDC, r.left + 1, r.top, (const char*)  buf, 2);
                    else // */
                        TextOut(memDC, r.left + 1, r.top, item->label, strlen(item->label));

                    SelectObject(memDC, oldBrush);
                    SelectObject(memDC, oldPen);
                }     
                
                SelectObject(memDC, oldFont);
                DeleteObject(oldFont);

                break;
            }
            case mode_entrance:

                entr  = (Entrance*) MarkerList::GetHead(o->areaEntr);
                buf   = o->entrBuf;
                brush = yellowBrush;

                SetTextColor(memDC, RGB(0x00, 0x00, 0x00));

                goto draw_markers;

                break;

            case mode_hole:

                entr  = (Entrance*) MarkerList::GetHead(o->areaHoles);
                buf   = o->holeBuf;
                brush = blackBrush;

                SetTextColor(memDC, RGB(0xFF, 0xFF, 0xFF));

            draw_markers:

                HFONT oldFont = (HFONT) SelectObject(memDC, courier);

                for(   ; entr != NULL; entr = (Entrance*) entr->GetNext())
                {
                    sprintf(entr->label, "%02X", entr->entrance);

                    if(entr->selected)
                        pen   = greenPen;
                    else
                        pen   = redPen;

                    oldBrush = (HBRUSH)  SelectObject(memDC, brush);
                    oldPen   = (HPEN)    SelectObject(memDC, pen);

                    r = *(entr->GetRect());

                    worked = Ellipse(memDC, r.left, r.top, r.right, r.bottom);

                    SetBkMode(memDC, TRANSPARENT);

                    if(entr->selected)
                        TextOut(memDC, r.left, r.top, (const char*)  buf, 2);
                    else
                        TextOut(memDC, r.left, r.top, entr->label, strlen(entr->label));

                    SelectObject(memDC, oldBrush);
                    SelectObject(memDC, oldPen);
                }

                SelectObject(memDC, oldFont);
                DeleteObject(oldFont); 
                
                break;
         }
    }

// ===============================================================

    static HCURSOR crossCurs = LoadCursor(0, IDC_CROSS);
    static HCURSOR arrowCurs = LoadCursor(0, IDC_ARROW);

    void OverObjMove(zgPtr game, OverObj *obj, u16 x, u16 y)
    {
        u16 modulus = 0;

        u32 dx, dy;

        // -------------

        switch(obj->mapType)
        {
            default:
            case fromMap8:
                modulus = 8; break;

            case fromMap16:
                modulus = 16; break;
    
            case fromMap32:
                modulus = 32; break;
        }

        InvalidateRect(game->pictWin, obj->GetRect(), false);

        dx = (x - obj->anchor.x);
        dx -= ((signed) dx % modulus);

        dy = (y - obj->anchor.y);
        dy -= ((signed) dy % modulus);

        obj->x += dx;
        obj->y += dy;

        obj->anchor.x += dx;
        obj->anchor.y += dy;

        InvalidateRect(game->pictWin, obj->GetRect(), false);               
    }

// ===============================================================

    void OwOnPictureMouseMove(HWND hwnd, WPARAM wp, LPARAM lp, zgPtr game)
    {
        char temp[0x200];

        u16 x = LOWORD(lp);
        u16 y = HIWORD(lp);

        u16 x32 = x - (x & 0x1F);
        u16 y32 = y - (y & 0x1F);

        u16 modulus = 0;

        u32 i = 0;

        OverObj *obj    = NULL;
        OverData* o     = game->overData;

        MarkerList *m = NULL;
   
        HWND zoomWnd = GetDlgItem(game->toolDlg, IDC_MAPTILES);
        HDC srcDC    = GetDC(game->pictWin);
        HDC destDC   = GetDC(zoomWnd);

        // ---------------------------------------

        // magnifies the current map32 tile by 8x and sends it to a view window
        StretchBlt(destDC,
                   0,       0,
                   128,     128,
                   srcDC,
                   x32 ,    y32,
                   32,      32,
                   SRCCOPY);

        ReleaseDC(game->pictWin, srcDC);
        ReleaseDC(zoomWnd, destDC);

        sprintf(temp, "%04X", GetMap8Tile(o->map8Buf, x >> 3, y >> 3));
        SetDlgItemText(game->toolDlg, ID_OW_EDIT2, temp);

        sprintf(temp, "%04X", GetMap16Tile(o->map16Buf, x >> 4, y >> 4));
        SetDlgItemText(game->toolDlg, ID_OW_EDIT3, temp);

        sprintf(temp, "%04X", o->GetMap32Tile(x >> 5, y >> 5));
        SetDlgItemText(game->toolDlg, ID_OW_EDIT4, temp);

        // next we look at the current edit mode and decide how to handle the mouse move
        switch(o->editMode)
        {
            default:
            case mode_drawtile:
            {
                break;
            }
            case mode_select:
            {
                obj = o->selObj2;

                // moving a pasted object takes priority over forming a new object
                if(obj->dragging && obj->active)
                {
                    SetCursor(arrowCurs);
                    OverObjMove(game, obj, x, y);
                    break;
                }
                
                obj = o->selObj;
                
                if(obj->forming == true)
                {
                    SetCursor(crossCurs);
    
                    // erase the old rectangle
                    InvalidateRect(game->pictWin, obj->GetRect(), FALSE);
                    
                    o->selObj->width    = ((x + (o->tileSize / 2)) & ~(o->tileSize - 1) ) - o->selObj->x;
                    o->selObj->height   = ((y + (o->tileSize / 2)) & ~(o->tileSize - 1) ) - o->selObj->y;

                    // draw the new rectangle
                    InvalidateRect(game->pictWin, obj->GetRect(), FALSE);
                }
                else if(obj->active)
                {
                    SetCursor(arrowCurs);

                    if(obj->dragging)
                        OverObjMove(game, obj, x, y);
                }
                else
                    SetCursor(crossCurs);

                break;
            }
            case mode_stock:
            {
                obj = o->pObj;

                if(!obj)
                    break;

                if(obj->dragging)
                    OverObjMove(game, obj, x, y);

                break;
            }
            case mode_sprite:
            {
                m = MarkerList::GetDragging(o->areaSpr);

                goto moveMarker;
            }
            case mode_item:
            {
                m = MarkerList::GetDragging(o->areaItems);

                goto moveMarker;
            }
            case mode_entrance:
            {
                m = MarkerList::GetDragging(o->areaEntr);

                goto moveMarker;
            }
            case mode_hole:
            {   
                m = MarkerList::GetDragging(o->areaHoles);

                goto moveMarker;
            }
            moveMarker:
            {
                if(m)
                {
                    if(!m->IsPos(x, y) && !m->GetAt(x, y))
                    {
                        InvalidateRect(game->pictWin, m->GetRect(), false);
                        m->SetPos(x, y);
                        InvalidateRect(game->pictWin, m->GetRect(), false);
                    }
                }

                break;
            }
            case mode_exit:
            {
                OverExit *exit = OverExit::GetDraggingExit(o->areaExits);

                if(exit)
                {
                    if(!exit->IsPos(x, y) && !exit->GetAt(x, y))
                    {
                        InvalidateRect(game->pictWin, exit->GetRect(), false);
                        exit->SetPos(x, y);
                        InvalidateRect(game->pictWin, exit->GetRect(), false);
                    }
                }

                break;
            }
        }

        o->prevX =  x;
        o->prevY =  y;
    }

// ===============================================================

    void CheckAreaChange(zgPtr game, HWND hwnd, u16 x, u16 y)
    {
        int large_changes[4][4] =   {  
                                        { -9, -8, -7, -6 },
                                        { -1,  0,  0,  2 },  
                                        {  7,  0,  0, 10 },
                                        { 15, 16, 17, 18 }
                                    };

        int small_changes[4][4] =   {
                                        { -9, -8, -8, -7 },
                                        { -1,  0,  0,  1 },
                                        { -1,  0,  0,  1 },
                                        {  7,  8,  8,  9 }
                                    };
        
        u32 i = 0, j = 0;        
        u32 height = 0, width = 0;
        u32 newArea = 0;

        u32 bounds[4] = { 1, 4, 7, 8};

        OverData *o = game->overData;
        
        RECT pictRect;

        // ------------------------

        GetClientRect(game->pictWin, &pictRect);

        width  = pictRect.right  - pictRect.left;
        height = pictRect.bottom - pictRect.top;

        for(i = 0; i < 4; ++i)
        {
            if(x < ((width * bounds[i]) / 8))
                break;
        }

        for(j = 0; j < 4; ++j)
        {
            if(y < ((height * bounds[j]) / 8))
                break;
        }

        newArea = o->area + ((o->largeArea) ? large_changes[j][i] : small_changes[j][i]);
        newArea %= 0x80;
        
        if(newArea != o->area)
            PostMessage(hwnd, OVER_CHANGE, (WPARAM) newArea, 0);
    }

// ===============================================================

    void OwOnPictureLButtonDown(HWND hwnd, ManagedBitmap* b, LPARAM lp, zgPtr game)
    {
        OverData *o      = game->overData;

        u8 *pos = NULL;
        u8 *buf = NULL;

        u16 a = GetKeyState(VK_CONTROL);

        u16 xFull = LOWORD(lp);
        u16 yFull = HIWORD(lp);

        u16 x = xFull >> 3;    // map8 aligned coordinates (8x8 tiles)
        u16 y = yFull >> 3;

        u16 x16 = x >> 1;       // map16 aligned coordinates
        u16 y16 = y >> 1; 

        u16 x32 = x >> 2;       // map32 aligned coordinates
        u16 y32 = y >> 2;       // map32 aligned coordinates

        u16 addr16       = ((x & 0xFFFE) * 2) + ((y & 0xFFFE) * 0x100);
        u16 addr32       = ((x16 & 0xFFFE) * 2) + ((y16 & 0xFFFE) * 0x80);

        u16 map8Vals[4]  = {0, 0, 0, 0};
        u16 map16Vals[4] = {0, 0, 0, 0};
    
        u16 index        = 0;

        u32 newMap16     = 0;
        u32 newMap32     = 0;
                    
        u16 oldMap16     = 0;
        u16 oldMap32     = 0;

        u32 area         = o->area;

        OverObj *obj     = NULL;
        Entrance *entr   = NULL;
        Entrance *list   = NULL;
        MarkerList *m    = NULL;
        MarkerList *n    = NULL;

        RECT r;
        POINT pt;

        // --------------------------------------

        // If the control key is being held down...
        if(a & 0x8000)
        {
            CheckAreaChange(game, hwnd, xFull, yFull);                

            return;
        }

        switch(o->editMode)
        {
            default:
                break;

            case mode_stock:
            {
                if(o->pObj)
                {
                    OverObjRect(o->pObj, &r);
                    pt.x = xFull;
                    pt.y = yFull;

                    if(PtInRect(&r, pt))
                    {
                        o->pObj->dragging = true;
                        o->pObj->anchor.x = xFull;
                        o->pObj->anchor.y = yFull;
                    }
                    else
                    {
                        // at this point we need to draw the data to the internal map as well as update the display 
                        // to reflect the tilemap changing
                        PlaceObject(game, o, o->pObj);

                        o->pObj = NULL;
                    }
                }
        
                break;
            }
            case mode_select:
            {
                RECT *r = NULL;        

                pt.x = xFull; pt.y = yFull;

                obj = o->selObj2;

                if(obj->active)
                {
                    if(PtInRect(obj->GetRect(), pt))
                    {
                        obj->dragging = true;
                        obj->anchor.x = xFull;
                        obj->anchor.y = yFull;
                    }
                    else
                    {
                        // at this point we need to draw the data to the internal map as well as update the display 
                        // to reflect the tilemap changing
                        PlaceObject(game, o, obj);

                        obj->active  = false;
                        obj->forming = false;
                    }
                    
                    break;
                }
                
                obj = o->selObj;

                if(!obj->forming && !obj->active)
                {
                    obj->x = (xFull + (o->tileSize / 2)) & ~(o->tileSize - 1);
                    obj->y = (yFull + (o->tileSize / 2)) & ~(o->tileSize - 1);
                    obj->width    = 0;
                    obj->height   = 0;
                    obj->forming  = true;
                    obj->dragging = false;
                    obj->active   = false;
                }
                else if(obj->active && !PtInRect(obj->GetRect(), pt))
                {
                    obj->forming = false;
                    obj->active  = false;
                    obj->width   = 0;
                    obj->height  = 0;
                }

                break;
            }
            case mode_exit:
            {
                OverExit *list = o->areaExits;
                OverExit *exit = NULL;

                for(exit = OverExit::GetHead(list); exit; exit = exit->GetNext() )
                {
                    InvalidateRect(game->pictWin, exit->GetRect(), false);
                    exit->selected = false;
                }

                exit = list->GetClicked(xFull, yFull);

                if(exit)
                {
                    Marker::Select( (Marker*) exit );

                    o->exitPos = 0;
                    sprintf( (char*) o->exitBuf, "%04X", exit->room);
                }

                break;
            }
            case mode_sprite:
            {
                m = MarkerList::GetSelected(o->areaSpr);

                if(m) { InvalidateRect(game->pictWin, m->GetRect(), false); }

                m = MarkerList::Select(o->areaSpr, xFull, yFull);

                if(m) 
                {
                    InvalidateRect(game->pictWin, m->GetRect(), false); 
                    o->sprPos = 0;
                    sprintf( (char*) o->sprBuf, "%02X", ((OverSpr*) m)->spriteNum);
                }

                break;

            }
            case mode_item:
            {
                m = MarkerList::GetSelected(o->areaItems);

                if(m) { InvalidateRect(game->pictWin, m->GetRect(), false); }

                m = MarkerList::Select(o->areaItems, xFull, yFull);

                if(m) 
                {
                    InvalidateRect(game->pictWin, m->GetRect(), false); 
                    o->itemPos = 0;
                    sprintf( (char*) o->itemBuf, "%02X", ((OverItem*) m)->itemNum);
                }

                break;

            }
            case mode_entrance:
            case mode_hole:
            {
                if(o->editMode == mode_entrance)
                {
                    list = o->areaEntr;
                    pos  = &o->entrPos;
                    buf  = o->entrBuf;
                }
                else
                {
                    list    = o->areaHoles;
                    pos  = &o->holePos;
                    buf  = o->holeBuf;
                }
                                
                // if a entrance ended up getting clicked, select it
                for(entr = (Entrance*) MarkerList::GetHead(list); entr != NULL; entr = (Entrance*) entr->GetNext())
                {       // refresh all entrs that have been deselected
                    InvalidateRect(game->pictWin, entr->GetRect(), false);
                    entr->selected = false;
                }

                entr =  (Entrance*) list->GetAt(xFull, yFull);

                if(entr)
                {
                    m = MarkerList::Select(list, xFull, yFull);

                    (*pos) = 0;
                    sprintf( (char*) buf, "%02X", entr->entrance);
                }

                break;
            }
            case mode_drawtile:
            {
                if(x32 >= 0x10)
                {
                    x32 -= 0x10;
                    area++;
                }

                if(y32 >= 0x10)
                {
                    y32 -= 0x10;
                    area += 0x08;
                }

                oldMap16 = GetMap16Tile(game->overData->map16Buf, x16, y16);
                oldMap32 = GetMap32Tile(game->overData->map32Data[area], x32, y32);

                index = (x & 0x01) + ((y & 0x01) << 1);

                map8Vals[0] = Get2Bytes(game->overData->map8Buf, addr16); 
                map8Vals[1] = Get2Bytes(game->overData->map8Buf, addr16 + 0x02);
                map8Vals[2] = Get2Bytes(game->overData->map8Buf, addr16 + 0x100);
                map8Vals[3] = Get2Bytes(game->overData->map8Buf, addr16 + 0x102);
                map8Vals[index] = game->overData->tile8;                

                index = ((x & 0x02) >> 1) + (y & 0x02); 

                map16Vals[0] = Get2Bytes(o->map16Buf, addr32); 
                map16Vals[1] = Get2Bytes(o->map16Buf, addr32 + 0x02);
                map16Vals[2] = Get2Bytes(o->map16Buf, addr32 + 0x80);
                map16Vals[3] = Get2Bytes(o->map16Buf, addr32 + 0x82);

                if(o->tileSize == fromMap8)
                {
                    newMap16 = o->FindMap16(map8Vals);

                    // failure. attempt to allocate a new map16 value
                    if(newMap16 == -1)
                            newMap16 = AllocateMap16(o->map16Counts, map8Vals, oldMap16);

                    if(newMap16 == -1)
                    {
                        MessageBox(hwnd, "No more map16 tiles can be allocated.", "error", MB_OK);
                        return;
                    }

                    map16Vals[index] = newMap16; 

                    // now we need to figure out whether we can find an existing map32 tile
                    // and if not, generate a new one if space is available
                    newMap32 = o->FindMap32(map16Vals);                    

                    // failure. try to allocate a map32 file
                    if(newMap32 == -1)
                        newMap32 = AllocateMap32(o->map32Counts, map16Vals, oldMap32);

                    if(newMap32 == -1)
                    {
                        MessageBox(hwnd, "No more map32 tiles can be allocated.", "error", MB_OK);
                        return;
                    }
    
                    SetMap8Tile(o->map8Buf, o->tile8, x, y);
                    SetMap16Tile(o->map16Buf, newMap16, x16, y16);
                    SetMap32Tile(o->map32Data[area], newMap32, x32, y32);
   
                    // increment resource counts for map16 and map32 tiles
                    o->DecMapCounts(oldMap16, usingMap16);
                    o->DecMapCounts(oldMap32, usingMap32);

                    o->IncMapCounts(newMap16, usingMap16, map8Vals);
                    o->IncMapCounts(newMap32, usingMap32, map16Vals);

                    DrawMap8(game, b, o->tile8, x, y);
                    InvalidateTile(game, x, y);

                    SendMessage(game->pictWin, WM_PAINT, 0, 0);
                    PostMessage(hwnd, PICTURE_MOUSEMOVE, 0, lp);
                }
                else if(o->tileSize == fromMap16)
                {
                    newMap16 = o->tile16;
    
                    map16Vals[index] = newMap16;                

                    newMap32 = o->FindMap32(map16Vals);                    

                    // failure. try to allocate a map32 file
                    if(newMap32 == -1)
                            newMap32 = AllocateMap32(o->map32Counts, map16Vals, oldMap32);

                    if(newMap32 == -1)
                    {
                        MessageBox(hwnd, "No more map32 tiles can be allocated.", "error", MB_OK);
                        return;
                    }

                    SetMap16Tile(o->map16Buf, newMap16, x16, y16);
                    SetMap32Tile(o->map32Data[area], newMap32, x32, y32);

                        // increment resource counts for map16 and map32 tiles
                    o->DecMapCounts(oldMap16, usingMap16);
                    o->DecMapCounts(oldMap32, usingMap32);

                    o->IncMapCounts(newMap16, usingMap16, map8Vals);
                    o->IncMapCounts(newMap32, usingMap32, map16Vals);

                    Map16To8(game, newMap16, map8Vals);

                    SetMap8Tile(o->map8Buf, map8Vals[0], (x & 0xFFFE) + 0, (y & 0xFFFE) + 0);
                    SetMap8Tile(o->map8Buf, map8Vals[1], (x & 0xFFFE) + 1, (y & 0xFFFE) + 0);
                    SetMap8Tile(o->map8Buf, map8Vals[2], (x & 0xFFFE) + 0, (y & 0xFFFE) + 1);
                    SetMap8Tile(o->map8Buf, map8Vals[3], (x & 0xFFFE) + 1, (y & 0xFFFE) + 1);

                    InvalidateTile(game, (x & 0xFFFE) + 0, (y & 0xFFFE) + 0);
                    InvalidateTile(game, (x & 0xFFFE) + 1, (y & 0xFFFE) + 0);
                    InvalidateTile(game, (x & 0xFFFE) + 0, (y & 0xFFFE) + 1);
                    InvalidateTile(game, (x & 0xFFFE) + 1, (y & 0xFFFE) + 1);

                    //DrawMap16(game, b, o->tile16, x16, y16);

                    SendMessage(game->pictWin, WM_PAINT, 0, 0);

                    PostMessage(hwnd, PICTURE_MOUSEMOVE, 0, lp);
                } 

                break;
            }
        }
    }

// ===============================================================

    u32 HexDigit(char c)
    {
        u32 retVal = 0;

        if(c >= '0' && c <= '9')
            retVal = (u32) (c - '0');
        else if(c >= 'A' && c <= 'F')
            retVal = (u32) (c - 'A' + 0x0A);
        else if(c >= 'a' && c <= 'f')
            retVal = (u32) (c - 'a' + 0x0A);
        else
            retVal = (u32) -1;

        return retVal;
    }

// ===============================================================

    void OwOnPictureChar(HWND hwnd, WPARAM wp, LPARAM lp, zgPtr game)
    {
        char c   = (char) wp;

        u8 *buf  = NULL;
        u8 *pos  = NULL;

        u32 value = 0;

        OverSpr *spr   = NULL;

        OverItem *item = NULL;

        Entrance *entr = NULL;
        
        MarkerList *m = NULL;

        OverData *o = game->overData;

        // -----------------------------------

        switch(o->editMode)
        {
            default:
                break;

            case move_overlay:
            {


                break;
            }
            case mode_drawtile:
            {
                

                break;
            }
            case mode_stock:
            {


                break;
            }
            case mode_custom:
            {


                break;
            }
            case mode_sprite:
            {
                m = MarkerList::GetSelected(o->areaSpr);

                if(!m) break;

                pos = &o->sprPos;
                buf = o->sprBuf;

                goto handle_char;

                break;
            }   
            case mode_item:
            {
                m = MarkerList::GetSelected(o->areaItems);

                if(!m) break;

                pos = &o->itemPos;
                buf = o->itemBuf;

                goto handle_char;

                break;
            }
            case mode_exit:
            {
                OverExit *exit = OverExit::GetSelectedExit(o->areaExits);

                if(!exit)
                    break;

                pos = &o->exitPos;
                buf = o->exitBuf;
               
                value = HexDigit(c);

                if(value == -1)
                    break;

                switch( (*pos) )
                {
                    default:
                    case 0:
                        sprintf( (char*) buf, "%c   ", c); break;
                    case 1:
                        sprintf( (char*) buf, "%c%c  ", buf[0], c); break;
                    case 2:
                        sprintf( (char*) buf, "%c%c%c ", buf[0], buf[1], c); break;
                    case 3:
                        sprintf( (char*) buf, "%c%c%c%c", buf[0], buf[1], buf[2], c); break;
                }

                strupr( (char*) buf);

                value = hextodec(buf, 4);

                if((*pos) == 3)
                    exit->room = value;

                (*pos) = (*pos + 1) % 4;

                break;
            }
            case mode_entrance:
            {   
                pos = &o->entrPos;
                buf = o->entrBuf;

                m = MarkerList::GetSelected(o->areaEntr);

                goto handle_char;

                break;
            }
            case mode_hole:
            {
                pos = &o->holePos;
                buf = o->holeBuf;

                m = MarkerList::GetSelected(o->areaHoles);

                goto handle_char;

                break;
            }

            // -------------

            handle_char:

                if(m)
                {
                    value = HexDigit(c);
                    
                    if(value == -1)
                        break; // not a hex a decimal value

                    if((*pos) == 0)
                        sprintf( (char*) buf, "%c ", c);
                    else
                        sprintf( (char*) buf, "%c%c", buf[0], c);

                    strupr( (char*) buf);

                    value = hextodec(buf, 2);

                    if(*pos)
                    {
                        switch(o->editMode)
                        {
                            case mode_hole:
                            case mode_entrance:
                                entr = (Entrance*) m;

                        if(value >= game->numEntrances)
                            sprintf( (char*) buf, "%02X", entr->entrance);
                        else
                            entr->entrance = value;

                                break;

                            case mode_sprite:

                                spr = (OverSpr*) m;

                                if(!spr->SetType(value))
                                    sprintf( (char*) buf, "%02X", spr->spriteNum);

                                break;

                            case mode_item:

                                item = (OverItem*) m;

                                if(!item->SetType(value))
                                    sprintf( (char*) buf, "%02X", item->itemNum);

                                break;
                        }                
                    }

                    (*pos) ^= 1; // alternate between two positions
                }

                break;
            }
        }


// ===============================================================

    void OwOnPictureKeydown(HWND hwnd, WPARAM wp, LPARAM lp, zgPtr game)
    {
        short a = 0;

        OverData *o = game->overData;

        // -----------------------------------

        a = GetKeyState(VK_CONTROL);

        if(a & 0x8000)
        {
            switch(wp)
            {
                case VK_LEFT:

                    o = o;

                    break;

                case VK_RIGHT:

                    o = o;

                    break;






            }

            return;
        }

        switch(o->editMode)
        {
            case mode_exit:
            {
                OverExit *exit = OverExit::GetSelectedExit(o->areaExits);

                switch(wp)
                {
                    case VK_LEFT:

                        if(exit)
                            if(exit->GetPrev())
                                ; 


                        break;

                    case VK_RIGHT:

                        break;

                    default:
                        break;
                }
                break;
            }
        }
   
    }

// ===============================================================

    void OwOnPictureLButtonUp(HWND hwnd, ManagedBitmap* b, LPARAM lp, zgPtr game)
    {
        OverObj    *obj = NULL;
        OverData   *o   = game->overData;
        MarkerList *m   = NULL;

        // -----------------------------------
        
        switch(o->editMode)
        {
            case mode_drawtile:
            {
                break;
            }
            case mode_stock:
            {
                if(o->pObj)
                    o->pObj->dragging = false;

                break;
            }
            case mode_select:
            {
                obj = o->selObj2;

                if(obj->active)
                {
                    obj->dragging = false;
                    break;
                }
                
                obj = o->selObj;

                if(obj->forming)
                {
                    u32 i    = 0;
                    u32 j    = 0;
                    u32 tile = 0;

                    // ------------------------

                    NormalizeObj(obj);

                    obj->forming = false;
                    obj->mapType = o->tileSize;

                    if(!obj->width || !obj->height)
                    {
                        obj->active = false;
                        break;
                    }
                    
                    obj->active  = true;
                }

                break;
            }
            case mode_sprite:
            {
                m = MarkerList::GetSelected(o->areaSpr);

                if(m)
                    m->dragging = false; 

                break;
            }
            case mode_item:
            {
                m = MarkerList::GetSelected(o->areaItems);

                if(m)
                    m->dragging = false; 

                break;
            }
            case mode_exit:
            {
                OverExit *exit = OverExit::GetDraggingExit(o->areaExits);

                if(exit)
                    exit->dragging = false;

                break;
            }
            case mode_entrance:
            {
                m = MarkerList::GetDragging(o->areaEntr);

                if(m)
                    m->dragging = false;

                break;
            }
            case mode_hole:
            {
                m = MarkerList::GetDragging(o->areaHoles);

                if(m)
                    m->dragging = false;

                break;
            }
        }
    }

// ===============================================================

    static HBITMAP key = LoadBitmap(thisProg, MAKEINTRESOURCE(IDB_BITMAP3));
    static HBITMAP key2 = LoadBitmap(thisProg, MAKEINTRESOURCE(IDB_BITMAP1));
    static HBITMAP test;
    static ManagedBitmap* b1 = CreateManagedBitmap(16,16);    

    void OwOnPictureRButtonDown(HWND hwnd, ManagedBitmap* b, LPARAM lp, zgPtr game)
    {
        u16 x = 0, y = 0;
        /// u16 map8Vals[4];

        int menuNum = IDR_BLANK;

        int subMenuIds[6] = { ID_EXIT_DOWN, ID_EXIT_UP, ID_EXIT_WOODEN, ID_EXIT_BOMBABLE, ID_EXIT_SANCTUARY, ID_EXIT_PALACE };
        over_exit_type doorTypes[6] = { over_exit_down, over_exit_up, over_exit_wooden, over_exit_bombable, over_exit_sanctuary, over_exit_palace };

        u32 i = 0, k = 0, area32 = 0;

        Entrance *e     = NULL;
        OverExit *exit  = NULL;
        OverData *o     = game->overData;
        MarkerList *m   = NULL;

        POINT pt;
        HMENU context;
        HMENU subContext;
        
        // ----------------------------------------------

        if(!b)
            return;

        GetCursorPos(&pt);

        x = LOWORD(lp);
        y = HIWORD(lp);

        switch(o->editMode)
        {
            default:
            {
                break;
            }

            case mode_drawtile:
            {
                if(o->tileSize == fromMap8)
                    o->tile8  = GetMap8Tile(o->map8Buf, x >> 3, y >> 3);
                if(o->tileSize == fromMap16)
                    o->tile16 = GetMap16Tile(o->map16Buf, x >> 4, y >> 4);
                else
                {
                    area32 = o->area;

                    if(x >= 0x200)
                        area32 += 1;

                    if(y >= 0x200)
                        area32 += 8;
                        
                    o->tile32 = GetMap32Tile(o->map32Data[area32], x >> 5, y >> 5);
                }

                return;
            }
            case mode_stock:     
            {
                if(o->pObj)
                    if(o->pObj->dragging)
                        return;

                UpdateStockIcons(hwnd, game);

                context = o->stockContext;
        
                break;
            }
            case mode_select:
            {
                // don't interrupt the selection process.
                if(o->selObj->forming)
                    break;

                if(o->selObj2->dragging)
                    break;

                context = o->selectContext;

                break;

            }
            case mode_sprite:
            {
                if(OverSpr::GetDragging(o->areaSpr))
                    return;

                context = o->sprContext;

                m = MarkerList::GetClicked(o->areaSpr, x, y);

                if(m)
                    EnableMenuItem(context, ID_REMOVE_SPRITE, MF_BYCOMMAND | MF_ENABLED);
                else
                    EnableMenuItem(context, ID_REMOVE_SPRITE, MF_BYCOMMAND | MF_GRAYED);

                CheckMenuItem(context, ID_FALLING_ROCKS, MF_BYCOMMAND | (o->fallingRocks[1][o->area] ? MF_CHECKED : MF_UNCHECKED));

                break;
            }
            case mode_item:
            {
                if(OverItem::GetDragging(o->areaItems))
                    return;

                context = o->itemContext;

                m = MarkerList::GetClicked(o->areaItems, x, y);

                if(m)
                    EnableMenuItem(context, ID_REMOVE_ITEM, MF_BYCOMMAND | MF_ENABLED);
                else
                    EnableMenuItem(context, ID_REMOVE_ITEM, MF_BYCOMMAND | MF_GRAYED);

                break;
            }
            case mode_exit:
            {
                if(OverExit::GetDraggingExit(o->areaExits))
                    return;

                context = o->exitContext;

                exit = o->areaExits->GetClicked(x, y);

                if(exit)
                {
                    EnableMenuItem(context, ID_REMOVE_EXIT, MF_BYCOMMAND | MF_ENABLED);
                    EnableMenuItem(context, 1, MF_BYPOSITION | MF_ENABLED);

                    subContext = GetSubMenu(context, 1); // door type submenu
    
                    for(i = 0; i < 6; ++i)
                    {
                        if(doorTypes[i] == exit->doorType)
                            CheckMenuItem(subContext, i, MF_BYPOSITION | MF_CHECKED);
                        else
                            CheckMenuItem(subContext, i, MF_BYPOSITION | MF_UNCHECKED);
                    }
                }
                else
                {
                    EnableMenuItem(context, ID_REMOVE_EXIT, MF_BYCOMMAND | MF_GRAYED);
                    EnableMenuItem(context, 1, MF_BYPOSITION | MF_GRAYED);
                }

                break;
            }
            case mode_entrance:
            {
                // don't bother if we're still dragging the hole around
                if(Entrance::GetDragging(o->areaEntr))
                    return;

                // otherwise pop out a popup menu
                context = o->entrContext;

                m = MarkerList::GetClicked(o->areaEntr, x, y);

                EnableMenuItem(context, ID_REMOVE_ENTRANCE, MF_BYCOMMAND | (m ? MF_ENABLED : MF_GRAYED) );

                break;
            }
            case mode_hole:
            {
                // don't bother if we're still dragging the hole around
                if(Entrance::GetDragging(o->areaHoles))
                    return;

                // otherwise pop out a popup menu
                context = o->holeContext;

                m = MarkerList::GetClicked(o->areaHoles, x, y);

                EnableMenuItem(context, ID_REMOVE_HOLE, MF_BYCOMMAND | (m ? MF_ENABLED : MF_GRAYED) );
            }

        }

        k = TrackPopupMenu(context, TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_LEFTALIGN|TPM_TOPALIGN|TPM_NONOTIFY,
                           pt.x,    pt.y,
                           0,       hwnd,
                           0);

        if(k)
            OwContextMenu(hwnd, game, k, x, y);
    }

// ===============================================================

    void OwContextMenu(HWND hwnd, zgPtr game, u32 k, u16 x, u16 y)
    {
        u32 i = 0;
        u16 gridX = 0;
        u16 gridY = 0;
        u16 mask  = 0;
        
        OverObj *obj    = NULL;

        MarkerList *m = NULL;
        OverData   *o = game->overData;
        RECT r;
    
        // -------------------------------------------------

        switch(o->editMode)
        {
            default:
                break;

            case mode_stock:
            {
                for(i = 0; i < 12; i++)
                {
                    if(over_stock_objs[i].id == k)
                    {
                        // copy the data from a stock object into the current object.
                        o->obj = over_stock_objs[i];

                        o->pObj = &o->obj;

                        switch(o->pObj->mapType)
                        {
                            default:
                            case fromMap8:
                                mask = ~0x07; break;

                            case fromMap16:
                                mask = ~0x0F; break;

                            case fromMap32:
                                mask = ~0x1F; break;
                        }

                        gridX = (x & mask); 
                        gridY = (y & mask);
 
                        InitOverObj(o->pObj, gridX, gridY, o->hStock[i], o->bStock[i]);

                        OverObjRect(o->pObj, &r);
                        InvalidateRect(game->pictWin, &r, FALSE);

                        break;            
                    }
                }   
                
                break;
            }
            case mode_select:
            {
                OwSelectContextMenu(game, k, x, y);

                break;
            }
            case mode_sprite:
            {
                OverSpr *spr = (OverSpr*) MarkerList::GetClicked(o->areaSpr, x, y);
                
                if(k == ID_ADD_SPRITE)
                    o->areaSpr = (OverSpr*) MarkerList::Add(o->areaSpr, new OverSpr(0, x, y, false));
                else if(k == ID_REMOVE_SPRITE)
                {
                    if(spr)
                    {
                        r = *(spr->GetRect());
                        r.right = (strlen(spr->label) * 8) + r.left;
                        InvalidateRect(game->pictWin, &r, FALSE);
                        
                        o->areaSpr = (OverSpr*) spr->Delete( (List**) &o->areaSpr);
                    }
                }
                else if(k == ID_DELETE_SPRITES_PART)
                {
                    while(o->areaSpr)
                        o->areaSpr = (OverSpr*) o->areaSpr->Delete( (List**) &o->areaSpr);
                }
                else if(k == ID_FALLING_ROCKS)
                {
                    o->fallingRocks[1][o->area] = !o->fallingRocks[1][o->area];

                }

                break;
            }
            case mode_item:
            {
                OverItem *item = (OverItem*) MarkerList::GetClicked(o->areaItems, x, y);
                
                if(k == ID_ADD_ITEM)
                    o->areaItems = (OverItem*) MarkerList::Add(o->areaItems, new OverItem(0, x, y));
                else if(k == ID_REMOVE_ITEM)
                {
                    if(item)
                    {
                        r = *(item->GetRect());
                        r.right = (strlen(item->label) * 8) + r.left;
                        InvalidateRect(game->pictWin, &r, FALSE);
                        
                        o->areaItems = (OverItem*) item->Delete( (List**) &o->areaItems);
                    }
                }
                else if(k == ID_DELETE_ITEMS)
                    while(o->areaItems)
                        o->areaItems = (OverItem*) o->areaItems->Delete( (List**) &o->areaItems);

                break;
            }
            case mode_exit:
            {
                OverExit *exit = o->areaExits->GetClicked(x, y);
               
                switch(k)
                {
                    case ID_ADD_EXIT:
                    {
                        o->areaExits = OverExit::Add(o->areaExits, new OverExit(x, y, o->area, 0, over_exit_down));

                        break;
                    }
                    case ID_DELETE_EXITS:
                    {
                        while(o->areaExits)
                            o->areaExits->Delete( (List**) &o->areaExits);

                        break;
                    }
                    case ID_REMOVE_EXIT:
                    {
                        if(exit) exit->Delete( (List**) &o->areaExits);

                        break;
                    }
                    case ID_EXIT_DOWN:
                        if(exit) exit->doorType = over_exit_down;      break;

                    case ID_EXIT_UP:
                        if(exit) exit->doorType = over_exit_up;        break;

                    case ID_EXIT_WOODEN:
                        if(exit) exit->doorType = over_exit_wooden;    break;

                    case ID_EXIT_BOMBABLE:
                        if(exit) exit->doorType = over_exit_bombable;  break;

                    case ID_EXIT_SANCTUARY:
                        if(exit) exit->doorType = over_exit_sanctuary; break;
                    
                    case ID_EXIT_PALACE:
                        if(exit) exit->doorType = over_exit_palace;    break;                    
                }

                break;
            }
            case mode_entrance:
            {
                switch(k)
                {
                    case IDC_ADD_ENTRANCE:
                    {
                        m = new Entrance(x, y, 0, o->area);

                        o->areaEntr = (Entrance*) MarkerList::Add(o->areaEntr, m);
                        InvalidateRect(game->pictWin, m->GetRect(), FALSE);

                        break;
                    }

                    case IDC_DELETE_ENTRANCES:
                    {
                        while(o->areaEntr)
                        {
                            InvalidateRect(game->pictWin, o->areaEntr->GetRect(), FALSE);
                            o->areaEntr = (Entrance*) o->areaEntr->Delete( (List**) &o->areaEntr);
                        }

                        break;
                    }

                    case ID_REMOVE_ENTRANCE:
                    {
                        Entrance *entr = o->areaEntr ? (Entrance*) o->areaEntr->GetAt(x, y) : NULL;

                        // --------------------------------------

                        if(entr)
                        {
                            InvalidateRect(game->pictWin, entr->GetRect(), FALSE);
                            entr->Delete( (List**) &o->areaEntr);
                        }

                        break;
                    }
                }
            }
            case mode_hole:
            {
                switch(k)
                {
                    case IDC_ADD_HOLE:
                    {
                        Entrance *hole = new Entrance(x, y, 0, o->area);

                        o->areaHoles = (Entrance*) MarkerList::Add(o->areaHoles, hole);
                        
                        InvalidateRect(game->pictWin, hole->GetRect(), FALSE);
                        break;
                    }

                    case IDC_DELETE_HOLES:
                    {
                        while(o->areaHoles)
                        {
                            InvalidateRect(game->pictWin, o->areaHoles->GetRect(), FALSE);
                            o->areaHoles = (Entrance*) o->areaHoles->Delete( (List**) &o->areaHoles);
                        }

                        break;
                    }

                    case ID_REMOVE_HOLE:
                    {
                        Entrance *hole = o->areaHoles ? (Entrance*) o->areaHoles->GetAt(x, y) : NULL;

                        // --------------------------------------

                        if(hole)
                        {
                            InvalidateRect(game->pictWin, hole->GetRect(), FALSE);
                            hole->Delete( (List**) &o->areaHoles);
                        }

                        break;
                    }
                }
            }
        }
    }

// ===============================================================

    // helper function that refreshes the graphics on an overworld object

    void RefreshOverObj(OverObj *obj, zgPtr game, HDC dc)
    {
        u32 i = 0, j = 0;
        u32 tile = 0;

        // ---------------------------

        for(i = 0; i < obj->width >> 3; ++i)
        {
            for(j = 0; j < obj->height >> 3; ++j)
            {
                tile = obj->mapData[ i + (j * obj->width >> 3) ];
                DrawMap8(game, obj->bData, tile, i, j);
            }
        }

        i = SetDIBits(dc, obj->hData, 0, obj->bData->header->biHeight, obj->bData->pixels->contents, obj->bData->structure, DIB_RGB_COLORS);

        InvalidateRect(game->pictWin, obj->GetRect(), FALSE);
    }

// ===============================================================

    void OwSelectContextMenu(zgPtr game, u32 command, u16 x, u16 y)
    {
        u16 tile = 0;
        u32 i, j = 0;
    
        HDC dc;

        OverObj *obj = NULL;

        OverData *o  = game->overData;
  
        // --------------------------

        obj = o->selObj2;

        dc = GetDC(game->pictWin);

        switch(command)
        {
            case ID_COPY:
            {
                // don't want to copy if an object is already active.
                if(obj->active)
                    break;
                
                // copy data from the selection object to the secondary (manipulating object)
                obj->width   = o->selObj->width;
                obj->height  = o->selObj->height;
                obj->x       = o->selObj->x;
                obj->y       = o->selObj->y;

                obj->bData   = CreateManagedBitmap(obj->width, obj->height);
                obj->mapData = new u32[(obj->width >> 3) * (obj->height >> 3)];

                obj->mapType = fromMap8;

                for(i = 0; i < obj->width >> 3; ++i)
                {
                    for(j = 0; j < obj->height >> 3; ++j)
                    {
                        tile = GetMap8Tile(o->map8Buf, (obj->x >> 3) + i, (obj->y >> 3) + j);
                        obj->mapData[i + (j * (obj->width >> 3))] = tile;
                        DrawMap8(game, obj->bData, tile, i, j);
                    }
                }

                obj->hData = CreateDIBitmap(dc,                    obj->bData->header, 
                                            CBM_INIT,              obj->bData->structure->bmiColors, 
                                            obj->bData->structure, DIB_RGB_COLORS);

                obj->active = false;

                break;
            }
            case ID_PASTE:
            {
                if(!o->selObj->active) break;
                if(o->selObj2->active) break;

                o->selObj2->active = true;
                o->selObj2->SetPos(x, y);

                break;
            }
            case ID_HFLIP:
            {
                u32 tile;

                if(obj->active)
                {
                    obj->HFlip();

                    RefreshOverObj(obj, game, dc);
                }

                break;
            }
            case ID_VFLIP:
            {
                u32 tile;

                if(obj->active)
                {
                    obj->VFlip();

                    RefreshOverObj(obj, game, dc);
                }

                break;
            }
            case ID_BOTH_FLIP:
            {
                u32 tile;

                if(obj->active)
                {
                    obj->VFlip();
                    obj->HFlip();

                    RefreshOverObj(obj, game, dc);
                }

                break;
            }
            case ID_LOW_PRIORITY:
            {
                u32 tile;

                if(obj->active)
                {
                    obj->SetPriority(false);

                    RefreshOverObj(obj, game, dc);
                }

                break;
            }
            case ID_HIGH_PRIORITY:
            {
                u32 tile;

                if(obj->active)
                {
                    obj->SetPriority(true);
                    
                    RefreshOverObj(obj, game, dc);
                }

                break;
            }
            case ID_PAL0:
            case ID_PAL1:
            case ID_PAL2:
            case ID_PAL3:
            case ID_PAL4:
            case ID_PAL5:
            case ID_PAL6:
            case ID_PAL7:
            {
                u16 palette = 0;

                u32 tile;

                int paletteIDs[8] = { ID_PAL0, ID_PAL1, ID_PAL2, ID_PAL3, ID_PAL4, ID_PAL5, ID_PAL6, ID_PAL7 };

                // ----------------------------------

                // notice that it will failsafe to using palette 0 (it's a slight trick)
                for(palette = 7; palette > 0; --palette)
                    if(paletteIDs[palette] == command)
                        break;

                if(obj->active)
                {
                    obj->SetPalette(palette);
                    
                    RefreshOverObj(obj, game, dc);
                }

                break;
            }
        }

        ReleaseDC(game->pictWin, dc);
    }

// ===============================================================
// Sets RECT values based on the Overworld Object's data

    void OverObjRect(OverObj* obj, RECT* r)
    {
        if(!r || !obj) 
            return;

        // fill in the rectangle structure
        r->left   = obj->x;
        r->right  = obj->width + obj->x;
        r->top    = obj->y;
        r->bottom = obj->height + obj->y;
    }

// ===============================================================
// normalize the object's coordinates and dimensions to be positive.

    void NormalizeObj(OverObj *obj)
    {
        if(!obj)
            return;

        if( (signed long) obj->width < 0)
        {
            obj->width  = -(obj->width);
            obj->x     -= obj->width;
        }

        if( (signed long) obj->height < 0)
        {
            obj->height = -(obj->height);
            obj->y     -= obj->height;
        }
    }

// ===============================================================

    void InitOverObj(OverObj* obj, u32 x, u32 y, HBITMAP h, ManagedBitmap* b)
    {
        u32 bitmapWidth     = obj->width  * (u32) obj->mapType;
        u32 bitmapHeight    = obj->height * (u32) obj->mapType;

        // -----------------------------

        obj->dragging = false;

        obj->x = x - (x % 8);    
        obj->y = y - (y % 8);    
    
        obj->width = b->header->biWidth;
        obj->height = b->header->biHeight;

        obj->bData = b;

        obj->bData = CreateManagedBitmap(bitmapWidth, bitmapHeight);

        obj->hData = h;
    }

// ===============================================================

    void InvalidateTile(zgPtr game, u32 x, u32 y)
    {
        RECT r;

        r.left = x << 3;
        r.right = r.left + 0x08;

        r.top = y << 3;
        r.bottom = r.top + 0x08;

        InvalidateRect(game->pictWin, &r, true);
    }

// ===============================================================

    int CALLBACK overTileDlgProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        u32 a = 0, i = 0, j = 0;

        zgPtr game = NULL;

        const int numCommands = 9;

        int bitmapIDs[numCommands]      = { IDB_STOCK, IDB_CUSTOM, IDB_SELECT, IDB_DRAWTILE, IDB_SPRITE, IDB_EXIT, IDB_HOLE, IDB_ENTRANCE, IDB_ITEM };
        int commandIDs[numCommands]     = { IDC_STOCK, IDC_CUSTOM, IDC_SELECT, IDC_DRAWTILE, IDC_SPRITE, IDC_EXIT, IDC_HOLE, IDC_ENTRANCE, IDC_ITEM };
        owEditMode modes[numCommands]   = { mode_stock, mode_custom, mode_select, mode_drawtile, mode_sprite, mode_exit, mode_hole, mode_entrance, mode_item }; 

        OverObj  *obj    = NULL;
        OverData *o      = NULL;

        POINT p1, p2;

        RECT r, r2;

        HBITMAP buttonBitmaps[numCommands];
        HWND    buttonWindows[numCommands];
        HWND    tileSizeCombo = GetDlgItem(hwnd, IDC_TILE_SIZE);
        HWND    auxGfxCombo   = GetDlgItem(hwnd, ID_AUX_BG_GFX);

        // ---------------------------------------------

        game = (zgPtr) GetWindowLong(hwnd, DWL_USER);

        switch(msg)
        {
            case WM_INITDIALOG:
            {
                PopulateCombobox(auxGfxCombo, (char**) auxGfxStrings);

                SendMessage(tileSizeCombo, CB_ADDSTRING, 0, (LPARAM) "8x8 Tiles");
                SendMessage(tileSizeCombo, CB_ADDSTRING, 0, (LPARAM) "16x16 Tiles");
                SendMessage(tileSizeCombo, CB_ADDSTRING, 0, (LPARAM) "32x32 Tiles");
                SendMessage(tileSizeCombo, CB_SETCURSEL, (WPARAM) 0, 0);

                GetClientRect(GetDlgItem(hwnd, IDC_CUSTOM1), &r);
                GetClientRect(hwnd, &r2);

                p1.x = r.left;  p1.y = r.top;
                p2.x = r2.left; p2.y = r2.top;

                ClientToScreen(GetDlgItem(hwnd, IDC_CUSTOM1), &p1);
                ClientToScreen(hwnd, &p2);
                
                p1.x -= p2.x;
                p1.y -= p2.y;

                MoveWindow(GetDlgItem(hwnd, IDC_CUSTOM1), p1.x, p1.y, 512, 64, TRUE);

                game = (zgPtr) lp;
                o = game->overData;

                SetWindowLong(hwnd, DWL_USER, (long) game);

                // Load bitmaps for all the dialog's buttons
                for(i = 0; i < numCommands; ++i)
                {
                    buttonBitmaps[i] = LoadBitmap(thisProg, MAKEINTRESOURCE(bitmapIDs[i]));
                    buttonWindows[i] = GetDlgItem(hwnd, commandIDs[i]);
                    SendMessage(buttonWindows[i], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM) buttonBitmaps[i]);
                }

                UpdateStockIcons(hwnd, game);

                ShowWindow(hwnd, SW_SHOW);
                SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

                CheckDlgButton(hwnd, IDC_DRAWTILE, BST_CHECKED);

                return 1; break;
            }
            case PICTURE_LBUTTON_DOWN:
            {
                a = 0;


                break;

            }
            case WM_COMMAND:
            {
                o = game->overData;

                switch(HIWORD(wp))
                {
                    case CBN_SELCHANGE:
                    {
                        if(LOWORD(wp) == IDC_TILE_SIZE)
                        {
                            o->tileSize = (objMapType) (8 * 1 << SendMessage(tileSizeCombo, CB_GETCURSEL, 0, 0));
                        }

                        break;
                    }

                    default:
                    {
                        switch(LOWORD(wp))
                        {
                            case ID_EDIT_OVERLAY:

                                o->editOverlay = !o->editOverlay;

                                // refresh the screen after the overlay setting has changed.
                                SendMessage(game->pictWin, PICTURE_PAINT, 1, 0);

                                break;

                            default:

                                for(i = 0; i < numCommands; ++i)
                                {
                                    if(wp == commandIDs[i])
                                    {
                                        o->editMode = modes[i];
                                        CheckDlgButton(hwnd, commandIDs[i], BST_CHECKED);
                        
                                        for(j = 0; j < numCommands; ++j)
                                        {
                                            if(j == i) continue;

                                            CheckDlgButton(hwnd, commandIDs[j], BST_UNCHECKED);
                                        }
                        
                                        break;
                                    }
                                }

                                break;

                        }

                        break;
                    }
                }
            }

            default:

                return FALSE; break;
        }

        return TRUE;
    }

// ===============================================================
    
    void UpdateStockIcons(HWND hwnd, zgPtr game)
    {
        u32 i       = 0;
        u32 value   = 0;
        u32 x       = 0;
        u32 y       = 0;

        bufPtr tileData = NULL;

        OverObj *obj = NULL;

        OverData *o  = game->overData;

        ManagedBitmap* b = NULL;        

        HDC hdc;

        // ---------------------------

        if(o == NULL)
            return;

        hdc = GetDC(hwnd);

        for(i = 0; i < 12; i++)
        {
            obj = &(over_stock_objs[i]);
            tileData = FromString((char*) obj->mapData, obj->width, obj->height, sizeof(u32));

            if(o->bStock[i] == NULL)
                o->bStock[i] = CreateManagedBitmap(obj->width * (u32) obj->mapType, obj->height * (u32) obj->mapType);

            for(x = 0; x < obj->width; ++x)
            {
                for(y = 0; y < obj->height; ++y)
                {
                    value = Get2Bytes(tileData, x, y);

                    switch(obj->mapType)
                    {
                        default:
                        case fromMap8:
                            DrawMap8(game, o->bStock[i], value, x, y); break;

                        case fromMap16:
                            DrawMap16(game, o->bStock[i], value, x, y); break;

                        case fromMap32:
                            // not implemented yet 
                            //DrawMap32(  );
                        
                            break;
                    }
                }
            }

            b = o->bStock[i];

            if(o->hStock[i] == NULL)
            {
                o->hStock[i] = CreateDIBitmap(hdc,          b->header, 
                                              CBM_INIT,     b->structure->bmiColors, 
                                              b->structure, DIB_RGB_COLORS);
            }
            else
            {
                x = SetDIBits(hdc, o->hStock[i], 0, b->header->biHeight,
                          b->structure->bmiColors, b->structure, DIB_RGB_COLORS);
            }

            SetMenuItemBitmaps(o->stockContext, over_stock_objs[i].id,
                               MF_BYCOMMAND,    o->hStock[i],
                               o->hStock[i]);

        }

        ReleaseDC(hwnd, hdc);        
    }

// ***************************************************************