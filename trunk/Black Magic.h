// Black Magic.h
// Contains declarations for Black Magic.
//
// Will handle things like file io.

    #include "Core.h"
    #include "Strings.h"
    #include "Globals.h"

#ifndef BLACK_MAGIC_H

    #define BLACK_MAGIC_H

    //#include "Strings.h"
    //#include <windows.h>
    //#include "resource.h"

    /*
    typedef struct
    {
        int hasHeader; // Flag indicating whether the file has an extra $200 bytes
        int index; // tells us which file it is among many

        bufPtr headerBuf; // pointer for the header's data so we can logically separate rom and header
        bufPtr image; // The game file as a char*
        bufPtr romName; // Pointer to the full path name of the rom.

        HWND subWindow; // The window used to edit this rom
        HANDLE zgFileHandle; // handle to this particular game's file.
    
        // other declarations like the location of headers

    } ZeldaGame;
    */

    // windows variables

    /*
    HMENU mainMenu;
    HMENU editMenu;

    HINSTANCE thisProg;
    HDC mainDC;
    RECT mainRect;
    MSG mainMsg;
    WNDCLASSEX mainClass;
    HWND mainWin;

    // image testing stuff
    BITMAPINFO* bmInfoPtr;
    BITMAPINFOHEADER* bmInfoHeadPtr;
    HBITMAP mainBM;
    SnesPalPtr defaultPal;
    SnesImage* testImage;
    SnesTile*  testTile;
    SnesTile*  testTile2;


    // testing stuff
    HANDLE testFH;
    bufPtr testPtr;

    OPENFILENAME bmOFN;

    // normal variables

    int quit = 0;
    int numGames = 0;
    int zgIndex = 0;

    unsigned long numBytesRead = 0;


    zgPtr* gameList;
    zgPtr newGamePtr;

    int expansionList[7] = {IDC_8MB,
                            IDC_12MB,
                            IDC_16MB,
                            IDC_20MB,
                            IDC_24MB,
                            IDC_28MB,
                            IDC_32MB};


    HWND edit = NULL;

    // Dungeon window template

    WindowElement dungTemp[] =
    {

        { editExStyle, "EDIT", editBuf, editStyle, 200, 200 , 100, 20, 0, (HMENU) ID_DungPaletteNum, 0, 0, 0, 2},
        { buttonExStyle, "BUTTON", "Jump", buttonStyle|WS_GROUP, 0, 0, 50, 50, 0, (HMENU) ID_DungJumpButton, 0, 0, 0, END_FLAG}

    };

    */

    // function prototypes

    int     SaveDataToFile(zgPtr game);

    void    SaveExtendedDataToFile(zgPtr game);

    bool    SaveText(zgPtr game, u32 *romOffset);

    bool    SaveSprites(zgPtr game, u32 *romOffset);

    void ChangeWES(WindowElement* we, HWND win, zgPtr game);

#endif