
#include "Globals.h"

// **********************************************
/*
int CALLBACK spriteDlgProc(HWND dlg, UINT msg, WPARAM wp, LPARAM lp)
{
    u32 a = 0, i = 0;

    char dummy[0x10];

    bufPtr digits = NULL;

    zgPtr game = (zgPtr) GetWindowLong(GetParent(dlg), 0);

    SprStruct *spr = (SprStruct*) GetWindowLong(dlg, DWL_USER);

    HWND keyCombo = GetDlgItem(dlg, IDC_DROPITEM);
    HWND sprCombo = GetDlgItem(dlg, IDC_SPRITENAME);
    HBITMAP sprBM = NULL;
    HBITMAP oldBM = NULL;
    HDC dlgDC = NULL;
    HDC memDC = NULL;
    PAINTSTRUCT ps;

    switch(msg)
    {  
        case WM_PAINT:

            dlgDC = GetDC(dlg);
            memDC = CreateCompatibleDC(dlgDC);
            sprBM = LoadBitmap(thisProg, MAKEINTRESOURCE(IDB_SPRITE42));

            oldBM = (HBITMAP) SelectObject(memDC, sprBM);

            BitBlt(dlgDC, 0, 0, 64, 64, memDC, 0, 0, SRCCOPY);  

            SelectObject(memDC, oldBM);
            DeleteObject(sprBM);

            return 0;
            
            break;

        case WM_INITDIALOG:

            SendMessage(keyCombo, CB_ADDSTRING, 0, (LPARAM) "Nothing");
            SendMessage(keyCombo, CB_ADDSTRING, 0, (LPARAM) "Key");
            SendMessage(keyCombo, CB_ADDSTRING, 0, (LPARAM) "Big Key");

            for(i = 0; strlen(SprStrings[i]) > 0; i++)
                SendMessage(sprCombo, CB_ADDSTRING, 0, (LPARAM) SprStrings[i]);

            oldEditProc = GetWindowLong(GetDlgItem(dlg, IDE_TYPE), GWL_WNDPROC);

            if(oldEditProc)
            {
                SetWindowLong(GetDlgItem(dlg, IDE_TYPE), GWL_WNDPROC, (long) NewEditProc);
                SetWindowLong(GetDlgItem(dlg, IDE_SUBTYPE), GWL_WNDPROC, (long) NewEditProc);
            }

            break;

        case TOOL_SPRITE:
                    
            if(!spr)
                return TRUE;

            SyncGUI(dlg, spr);
            spr->selected = 1;

            break;
        
        case SPRITE_MOVE:
        {
            if(!spr) {   break; }
            if(!spr->selected) {   break; }

            // needs mod

            u32 x = (u32) wp << 3;
            u32 y = (u32) lp << 3;
            x %= 0x200; y %= 0x200; // boundary checking

            spr->x = x & 0x1F0;
            spr->y = y & 0x1F0;       
        }
 
            break;

        case SPRITE_STOPPED:

            // needs mod?

            if(!spr)
                break;

            spr->selected = 0;

            break;

        case WM_COMMAND:

            switch(HIWORD(wp))
            {
            case EN_CHANGE:
                
                if(LOWORD(wp) == IDE_TYPE)
                {                                 
                    GetDlgItemText(dlg, IDE_TYPE, dummy, 0x0F);

                    if(strlen(dummy) > 2)
                    {
                        dummy[2] = 0;
                        SetDlgItemText(dlg, IDE_TYPE, dummy);
                        break;
                    }

                    if(!spr)
                        break;

                    a = hextodec((u8*) dummy, 2);

                    if(spr->flag)
                    {
                        if(a > 0x1A)
                        {
                            SetDlgItemText(dlg, IDE_TYPE, "1A");
                            break;
                        }
                    }
                    else
                    {
                        if(a > 0xF2)
                        {
                            SetDlgItemText(dlg, IDE_TYPE, "F2");
                            break;
                        }
                    }

                    spr->type = a;

                    if(SendMessage(sprCombo, CB_GETCURSEL, 0, 0) != a)
                        SendMessage(sprCombo, CB_SETCURSEL, (WPARAM) a, 0);
                }
                else if(LOWORD(wp) == IDE_SUBTYPE)
                {
                    GetDlgItemText(dlg, IDE_SUBTYPE, dummy, 0x0F);

                    if(strlen(dummy) > 2)
                    {
                        SetDlgItemText(dlg, IDE_SUBTYPE, "0");
                        break;
                    }

                    if(!spr)
                        break;

                    a = hextodec((u8*) dummy, 2);

                    if(a >= 0x10)
                    {
                        SetDlgItemText(dlg, IDE_SUBTYPE, "0");
                        break;
                    }

                    spr->subType = a;
                }

                break;
            
            case BN_CLICKED:
    
                if(!spr)
                    break;

                switch(LOWORD(wp))
                {
                    case IDC_OVERLORD:

                        a = IsDlgButtonChecked(dlg, IDC_OVERLORD);
                        
                        if(a)
                        {
                            // if trying to make it into an overlord...
                            if(game->dngObjs->numOverlords >= 8)
                            {
                                MessageBox(dlg, "Room already has 8 overlords!", "error", MB_OK);
                                CheckDlgButton(dlg, IDC_OVERLORD, BST_UNCHECKED);
                                break;                               
                            }

                            spr->flag = 1;
                            spr->type %= 0x1B;
                            spr = DngSprContextMenu(dlg, game, ID_CLONESPRITE, spr->x, spr->y);
                            DngSprContextMenu(dlg, game, ID_DELETESPRITE, spr->x, spr->y);
                            SetWindowLong(dlg, DWL_USER, (long) spr);
                        }
                        else
                        {
                            // if trying to make it in to a sprite
                            if(game->dngObjs->numSprites >= 16)
                            {
                                MessageBox(dlg, "Room already has 16 sprites!", "error", MB_OK);
                                CheckDlgButton(dlg, IDC_OVERLORD, BST_CHECKED);
                                break;                               
                            }

                            spr->flag = 0;
                            spr->type %= 0xF3;
                            spr = DngSprContextMenu(dlg, game, ID_CLONESPRITE, spr->x, spr->y);
                            DngSprContextMenu(dlg, game, ID_DELETESPRITE, spr->x, spr->y);
                            SetWindowLong(dlg, DWL_USER, (long) spr);
                        }

                        SyncGUI(dlg, spr);                        
                        
                        break;
                        
                    case IDR_FLOOR1:
                    case IDR_FLOOR2:

                        spr->floor = (LOWORD(wp) == IDR_FLOOR2) ? 1 : 0;

                        break;

                    default:

                        break;                
                }

                break;

            case CBN_EDITUPDATE:
            case CBN_EDITCHANGE:

                wp = wp; // temporary test code

                break;

            case CBN_SELCHANGE:

                // handling the different comboboxes for the sprites
                switch(LOWORD(wp))
                {
                case IDC_DROPITEM:

                    if(!spr)
                        break;
                    
                    if(!spr->flag)
                        spr->key = SendMessage(keyCombo, CB_GETCURSEL, 0, 0) % 3;
                    else
                        spr->key = 0;
                                        
                    //SendMessage(GetParent(dlg), WM_PAINT, 0, 0);

                    break;

                case IDC_SPRITENAME:
                
                    a = SendMessage(sprCombo, CB_GETCURSEL, 0, 0);
                    itoa(a, dummy, 0x10);
                    SetDlgItemText(dlg, IDE_TYPE, dummy);

                    if(!spr)  break;

                    spr->type = a;

                    break;

                default:

                    wp = wp;

                    break;
                }
            
                break;

            default:

                wp = wp;
                lp = lp;

                break;   
            }

            break;

        default:

            return FALSE;
    }
    return TRUE;
}
*/
//**********************************************
