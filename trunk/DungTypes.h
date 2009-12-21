     
#ifndef DUNGTYPES_H

    #define DUNGTYPES_H

    typedef struct
    {
        bool isOnBG0;
        bool isOnBG1;

        u8 type;
        u8 subType;
        u8 funcNum;
        u8 index; // indicates this objects placement in the current layer
        u8 width;
        u8 height;
        u8 layer;
   
        u32 x;
        u32 y;
        u32 mem_08;
        u32 mapAddr;
        u32 arg1;
        u32 arg2;
        u32 tileNum;
        u32 mapNum;
        u32 doMin;
        u32 doMax;

        bufPtr chr0; // the tile data of the completely rendered object
        bufPtr above0;
        bufPtr below0;
        bufPtr chr1;
        bufPtr above1;
        bufPtr below1;
        
    } DngObj;

    typedef struct
    {
        bufPtr data;
        u32 numObj;

    } DngObjList;

    typedef struct
    {
        // repositories for all our object data
        DngObjList type1[4];
        DngObjList type2[4];

        bool darkWorld; // flag indicating whether we're in the darkworld or not

        u32 numStars;
        u32 numChests;
        u32 numBigKeyLocks;
        u32 numTwoStateObjs;
        u32 twoStateLocs[0x10]; // two state object locations on tilemap
        u32 chestLocs[0x06]; // Chest and big key lock locations
        u32 floor[2];
        u32 layoutNum;
        u32 layersOn;

        DngObj* twoStateObjs[0x10]; // will supercede twoStateLocs eventually
        DngObj* chestObjs[0x06]; // will supercede chestLocs eventually

        bufPtr BG_Attr[2];
        bufPtr masterAttrTbl;
        bufPtr scrapReg[2]; // used for initializing objects, object priority rules are ignored here
        bufPtr BG0Registry[4]; // layout, layer1, layer2, layer3 object registries
        bufPtr BG1Registry[4]; // same but for BG1

        SprStruct sprites[16];
        u32 sprIndices[16];
        u32 numSprites;

        SprStruct overlords[8];
        u32 overIndices[8];
        u32 numOverlords;
        u32 sprChanged;

        HWND toolDlg;
       
    } DngObjStruct;

    typedef struct 
    {
        Buffer              headerData;
        u8                  headerLengths[320];
        Buffer              objectData[320];
        EntranceData        entrances;
        Buffer              chestLocationData;
        Buffer              spriteData[320];
        Buffer              layoutData[64];
        Buffer              secretsData[320]; // added in later

    } DungeonFile;

       typedef struct 
    {
        // The phrasing Byte X refers to where these values reside
        // in the acutal packed header in the rom.

        bufPtr data;

        unsigned char bg0_props;           // {
        unsigned char collision;
        unsigned char lightsOut;          // } Byte 0
          
        unsigned char paletteIndex;       // Byte 1
        unsigned char graphicsNum;        // Byte 2
        unsigned char spriteGfx;          // Byte 3
        unsigned char effect;             // Byte 4
        unsigned char tag_1;              // Byte 5
        unsigned char tag_2;              // Byte 6
          
        unsigned char warpPlane;          // {
        unsigned char staircasePlane1;
        unsigned char staircasePlane2;
        unsigned char staircasePlane3;    // } Byte 7

        unsigned char staircasePlane4;    // Byte 8

        unsigned char warpRoom;           // Byte 9
        unsigned char staircaseRoom1;     // Byte 10
        unsigned char staircaseRoom2;     // Byte 11
        unsigned char staircaseRoom3;     // Byte 12
        unsigned char staircaseRoom4;     // Byte 13

    } BM_DungeonHeader;

#endif