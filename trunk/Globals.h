// Not necessarily needed right now, but it's a useful for testing.

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

    #ifndef GLOBAL_H

	    #define GLOBAL_H

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

    // structs typedefs

        typedef struct 
        {
          unsigned char* contents;
          unsigned int length;
          unsigned int error;
          unsigned int wOffset; // write offset

          // "unit" is added in case the internal data is in X byte chunks
          unsigned int width;
          unsigned int height;
          unsigned int unit;

        } Buffer;
    
        typedef Buffer*     bufPtr;

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

        typedef struct
        {
            u32 stride;
            u32 bottom;

            bufPtr memory;
            bufPtr pixels;
            BITMAPINFO* structure;
            BITMAPINFOHEADER* header;

        } ManagedBitmap;

        // data related typedefs

        // typedefs for dungeon data types ... 
       
            typedef char       ScrollEdges[8];

            #define            ENTRANCE_BASE_SIZE     32

        typedef struct
        {
            Buffer         roomNumber;          // 2
            Buffer         scrollEdgeData;      // 8
            Buffer         xCoord;              // 2
            Buffer         yCoord;              // 2
            Buffer         xScroll;             // 2
            Buffer         yScroll;             // 2
            Buffer         yLowerScrollBound;   // 2
            Buffer         xLowerScrollBound;   // 2
            Buffer         blockSet;            // 1
            Buffer         floorValue;          // 1
            Buffer         dungeonValue;        // 1
            Buffer         doorwayType;         // 1
            Buffer         ladderValue;         // 1 // what layer Link enters on.
            Buffer         scrollFlags;         // 1
            Buffer         quadrantValue;       // 1
            Buffer         owExitData;          // 2
            Buffer         musicValue;          // 1


            Buffer         entranceBuffer;

        } EntranceData;
    
        // ... results in this file

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

    //struct DngObj { };
    //struct ZeldaGame { };
    //typedef ZeldaGame* zgPtr;

        typedef int (*editMode)(HWND,UINT,WPARAM,LPARAM);

    // music related structs

        typedef struct
        {
            u8 srcn;
            u8 adsr1;
            u8 adsr2;
            u8 gain;
            u16 pitch; // ???? not really sure about this
        } Instrument;

        typedef struct
        {    
            bufPtr     samp[0x2A];
            Instrument inst[0x2A];
            u32 d[0x2A];

        } MusicBank;

        //end of music related defines

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
            u32 x;
            u32 y;
            u32 key;
            u32 type;
            u32 subType;
            u32 floor;
            u32 flag; // Indicates sprite if 0, overlord if 1
            u32 selected;
        } SprStruct;


        typedef struct
        {
            bufPtr *data; // contains 
            u32 numMsgs; // the number of messages it actually uses

        } DialogueFile;

        
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

        #include "List.h"
        #include "Marker.h"

        #include "DungTypes.h"
        #include "OverTypes.h"
        #include "OverHole.h"
        #include "OverData.h"

        class ZeldaGame
        //typedef struct _ZeldaGame
        {
        public:
            
            int hasHeader; // Flag indicating whether the file has an extra $200 bytes
            int index; // tells us which file it is among many
            int beenModified; // Nonzero if the file has been modified in some way in a
            
            u32 numRooms; // tells us how many dung. rooms it has. 296 by default.
            u32 numEntrances;
            u32 currentDungRoom;
            u32 currentOverArea;

            bufPtr        headerBuf; // pointer for the header's data so we can logically separate rom and header
            bufPtr        image; // The game file as a char*
            bufPtr        romName; // Pointer to the full path name of the rom.

            HWND          toolDlg; // general purpose tool dialog that can be used for a variety of things
            HWND          pictWin;
            HWND          subWindow; // The window used to edit this rom
            HANDLE        zgFileHandle; // handle to this particular game's file.
            HANDLE        portFileHandle;

            WindowElement *dungeonWE;
            WindowElement *overWE;

            WEStruct      currentWES;

            BM_Header     bm_Header;
            BM_DungeonHeader bm_dung_header;  

            DungeonFile  dungeonFile;
            DungeonFile  portFile;
            DungeonFile* currentFile;

            DialogueFile dialogueFile;

            OverData    *overData;

            giPtr        gi;
            
            // All the background graphics packs
            bufPtr       bgPacks[0x100];

            // All the sprite graphics packs
            bufPtr       sprPacks[0x100];

            bufPtr       linkGfx;
            bufPtr       fontGfx;
            bufPtr       mode7Gfx;

            bufPtr       vram;
            bufPtr       cgram;
            bufPtr       aniVram; // for animated tiles

            editMode     currentFunc;

            bufPtr       testPtr;
            bufPtr       BG_Map[2];

            // $200 decoded BG tiles
            SnesTile     BGTiles[0x400];
            SnesTile     OAMTiles[0x400];

            u32 xPos;
            u32 yPos;
            u32 xPrev;
            u32 yPrev;
            u32 cursorIndex;
            u32 aniTimer;
            u32 aniFrame;
            u32 active;

            // sixteen decoded palettes to work with.
            SnesPal      Pals[0x10];

            SnesTile cursor;
            SnesPal cursorPal;

            DngObjStruct *dngObjs;

        public:

            void LoadAllGfx();

            int GetSprGfxPtr(int index);
            int GetBgGfxPtr(int index);

        };
        //ZeldaGame, *zgPtr;

        typedef ZeldaGame* zgPtr;

    // more typedefs based on earlier typedefs

    extern BITMAPINFOHEADER* bmInfoHeadPtr;
    extern HBITMAP mainBM;
    extern SnesPalPtr defaultPal;

    extern int totalMem;

    // testing stuff
    extern HANDLE   testFH;
    extern zgPtr    newGamePtr;
    extern HWND     windowList[0x100];

    extern OPENFILENAME bmOFN;

    //extern DungeonFile portFile;

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

    extern RGBQUAD mainPalette[];

    extern DungeonFile portFile;

    extern OverObj over_stock_objs[];
    
    void stupidFunc();

#endif
