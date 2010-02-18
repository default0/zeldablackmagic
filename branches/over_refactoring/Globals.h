// Not necessarily needed right now, but it's a useful for testing.

#ifndef GLOBAL_H

    #define GLOBAL_H

	#include <map>
	#include <stdio.h>
	#include <Windows.h>
	#include <windowsx.h>
	#include <Shellapi.h>
	#include <malloc.h>
	#include <commctrl.h>
	#include <crtdbg.h>

	#include "resource.h"
	#include "constants.h"

	extern BITMAPINFO* bmInfoPtr;
	extern HINSTANCE thisProg;
	extern HDC mainDC;
	extern RECT mainRect;
	extern MSG mainMsg;
	extern WNDCLASSEX mainClass;
	extern WNDCLASSEX pictureClass;

	extern char msgBuffer[256];
	extern char editBuf[512];

	extern LONG oldEditProc;

	extern int expansionList[];

    extern HFONT courier;

	typedef unsigned char   u8;
	typedef unsigned short  u16;
	typedef unsigned int    u32;

    // structure types
    typedef struct 
    {
        DWORD exStyle;
        LPCTSTR className;
        LPCTSTR windowCaption;
        DWORD style;
        int x;
        int y;
        int nWidth;
        int nHeight;
        HWND parent;
        HMENU hMenu;
        HINSTANCE hInstance;
        LPVOID lpParam;

        HWND handle;
        int endflag;

    } WindowElement;

    typedef struct 
    {
      WindowElement* WESet;

      unsigned int numElements;

    } WEStruct;

    // graphics related typedefs
    typedef RGBQUAD     Mode7Pal[256];

    typedef u32         SnesPal[16];
    typedef RGBQUAD     SnesImage[64];

    typedef RGBQUAD     GameBoyPal[4];
    typedef RGBQUAD     GameBoyImage[64];

    typedef SnesPal*    SnesPalPtr;

    typedef unsigned char   SnesTile[64];
    typedef unsigned char   GameBoyTile[64];

    // A function typedef
    typedef int (*editMode)(HWND,UINT,WPARAM,LPARAM);

    typedef struct
    {
        u8 backPal0;    // same as $0AB6      
        u8 backGfx0;    // same as $0AA1       
        u8 backGfx1;    // same as $0AA2       
        u8 spriteGfx0;  // same as $0AA3    
        u8 spriteGfx1;  // same as $0AA4 (usually only one of 3 values)
        u8 spritePal0;  // same as $0AAC
        u8 spritePal1;  // same as $0AAD
        u8 spritePal2;  // same as $0AAE 
    } graphicsInfo;

    typedef graphicsInfo*   giPtr;
    
    typedef struct
    {
        // Want this to be a header that has room for expansion

        char designation[12];   // defaults to "BM_HEADER"

        u32 header_version;
        u32 dngHeaderOffset;
        u32 dngHeaderBank;
        u32 dngObjOffset;
        u32 dngEntranceOffset;
        u32 dngChestOffset;
        u32 dngNumChests;
        u32 dngSpriteOffset;
        u32 dngSpriteBanks;
        u32 header_size;
        
        u32 dngLayoutOffset;    // Offset in the rom where the dungeon layout (template) object data is at.
        u32 textOffset;         // The offset where the pointer table for all the dialogue is at.
        u32 textNumMsgs;        // Number of dialogue messages being stored in the rom.
        
        u32 overNumParts;       // Number of parts used in the game (not implemented yet)
        
        u32 overHoleOffset;     // CPU pointer to the beginning of the overworld hole data.
        u32 overNumHoles;       // The number of hole markers that are on the overworld.
        
        u32 overEntrOffset;     // CPU pointer to the beginning of the overworld entrance data.
        u32 overNumEntr;        // The number of entrance markers (not the same as the number of entrances to the dungeons) on the overworld.
        
        u32 overItemOffset;     // CPU pointer to the table that has all the overworld item data.

        u32 overSprOffset0;     // CPU pointer to the sprite data table for parts 0 and 1
        u32 overSprOffset2;     // CPU pointer to the sprite data table for part 2
        u32 overSprOffset3;     // CPU pointer to the sprite data table for part 3
        
        u32 map32To16UL;        // CPU pointer to the map32 to map16 conversion array (upper left)
        u32 map32To16UR;        // CPU pointer to the map32 to map16 conversion array (upper right)
        u32 map32To16LL;        // CPU pointer to the map32 to map16 conversion array (lower left)
        u32 map32To16LR;        // CPU pointer to the map32 to map16 conversion array (lower right)

        u32 sprGfxOffset;       // CPU pointer to the sprite graphics pointer array
        u32 bgGfxOffset;        // CPU pointer to the background graphics pointer array

        u32 reserved20;
        u32 reserved21;
        u32 reserved22;
        u32 reserved23;
        u32 reserved24;
        u32 reserved25;
        u32 reserved26;
        u32 reserved27;
        u32 reserved28;
        u32 reserved29;
        u32 reserved30;
        u32 reserved31;
        u32 reserved32;

    } BM_Header;

    // more typedefs based on earlier typedefs
    extern BITMAPINFOHEADER* bmInfoHeadPtr;
    extern HBITMAP mainBM;
    extern SnesPalPtr defaultPal;

    extern int totalMem;

    // testing stuff
    extern HANDLE   testFH;
    extern HWND     windowList[0x100];

    extern OPENFILENAME bmOFN;

    // normal variables
    extern char* bg0_prop_str[];
    extern char* collision_str[];
    extern char* special_effect_str[];
    extern char* special_routine_str[];
    extern char* plane_str[];

    extern int quit;
    extern int numGames;
    extern int zgIndex;

    extern unsigned long numBytesRead;


    // Dungeon window template
    extern WindowElement dungTemp[];
    extern WindowElement overTemp[];

    extern u32 entranceBufSizes[];
    extern char* SprStrings[];
    extern char* OverStrings[];
    extern char* ItemStrings[];

    extern char* mainGfxStrings[];
    extern char* auxGfxStrings[];

    extern RGBQUAD mainPalette[];
    
    void stupidFunc();

#endif
