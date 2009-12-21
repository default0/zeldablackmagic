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

            char designation[12];
            unsigned int header_version;
            unsigned int dngHeaderOffset;
            unsigned int dngHeaderBank;
            unsigned int dngObjOffset;
            unsigned int dngEntranceOffset;
            unsigned int dngChestOffset;
            unsigned int dngNumChests;
            unsigned int dngSpriteOffset;
            unsigned int dngSpriteBanks;
            unsigned int header_size;
            unsigned int dngLayoutOffset;   // Offset in the rom where the dungeon layout (template) object data is at.
            unsigned int textOffset;        // The offset where the pointer table for all the dialogue is at.
            unsigned int textNumMsgs;       // Number of dialogue messages being stored in the rom.
            
            unsigned int overNumParts;      // Number of parts used in the game (not implemented yet)
            
            unsigned int overHoleOffset;    // CPU pointer to the beginning of the overworld hole data.
            unsigned int overNumHoles;      // The number of hole markers that are on the overworld.
            
            unsigned int overEntrOffset;    // CPU pointer to the beginning of the overworld entrance data.
            unsigned int overNumEntr;       // The number of entrance markers (not the same as the number of entrances to the dungeons) on the overworld.
            
            unsigned int overItemOffset;    // CPU pointer to the table that has all the overworld item data.

            unsigned int overSprOffset0;    // CPU pointer to the sprite data table for parts 0 and 1
            unsigned int overSprOffset2;    // CPU pointer to the sprite data table for part 2
            unsigned int overSprOffset3;    // CPU pointer to the sprite data table for part 3
            
            unsigned int map32To16UL;   // CPU pointer to the map32 to map16 conversion array (upper left)
            unsigned int map32To16UR;   // CPU pointer to the map32 to map16 conversion array (upper right)
            unsigned int map32To16LL;   // CPU pointer to the map32 to map16 conversion array (lower left)
            unsigned int map32To16LR;   // CPU pointer to the map32 to map16 conversion array (lower right)

            unsigned int reserved17;
            unsigned int reserved18;
            unsigned int reserved19;
            unsigned int reserved21;
            unsigned int reserved22;
            unsigned int reserved23;
            unsigned int reserved24;
            unsigned int reserved25;
            unsigned int reserved26;
            unsigned int reserved27;
            unsigned int reserved28;
            unsigned int reserved29;
            unsigned int reserved30;
            unsigned int reserved31;
            unsigned int reserved32;

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
            ZeldaGame();

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
            
            // All the bg graphics packs
            bufPtr       bgPacks[0x100];

            // 
            bufPtr       sprPackets[0x100];


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

        extern unsigned int entranceBufSizes[];
        extern char* SprStrings[];
        extern char* OverStrings[];
        extern char* ItemStrings[];

        extern RGBQUAD mainPalette[];

        extern DungeonFile portFile;

	    extern OverObj over_stock_objs[];

    void stupidFunc();

#endif
