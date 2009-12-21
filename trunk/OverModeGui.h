
#ifndef OVERMODEGUI_H

    #define OVERMODEGUI_H

    void        OwOnPicturePaint(zgPtr game, HDC picDC, HDC memDC);
    void        OwOnPictureMouseMove(HWND hwnd, WPARAM wp, LPARAM lp, zgPtr game);
    void        OwOnPictureLButtonDown(HWND hwnd, ManagedBitmap* b, LPARAM lp, zgPtr game);
    void        OwOnPictureLButtonUp(HWND hwnd, ManagedBitmap* b, LPARAM lp, zgPtr game);
    void        OwOnPictureChar(HWND hwnd, WPARAM wp, LPARAM lp, zgPtr game);
    void        OwOnPictureKeydown(HWND hwnd, WPARAM wp, LPARAM lp, zgPtr game);
    void        OwOnPictureRButtonDown(HWND hwnd, ManagedBitmap* b, LPARAM lp, zgPtr game);

    void        OwContextMenu(HWND hwnd, zgPtr game, u32 k, u16 x, u16 y);
    void        OwSelectContextMenu(zgPtr game, u32 command, u16 x, u16 y);

    void        InvalidateTile(zgPtr game, u32 x, u32 y);    
    void        OverObjRect(OverObj* obj, RECT* r);
    void        NormalizeObj(OverObj *obj);

    int         overworld(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
    int         CALLBACK overTileDlgProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);

    void        UpdateStockIcons(HWND hwnd, zgPtr game);

    void		InitOverObj(OverObj *obj, u32 x, u32 y, HBITMAP h, ManagedBitmap* b);


#endif