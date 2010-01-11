
    // Not necessarily needed right now, but it's a useful for testing.
    // 1. #include "Globals.h"

    #include "Globals.h"
    #include "Strings.h"
    #include "Core.h"
    #include "Black Magic.h"

    char* bg0_prop_str[] = 
    {
        "(0) Off",
	    "(1) Parallaxing",
	    "(2) Dark",
        "(3) On top",
	    "(4) Translucent",
	    "(5) Parallaxing2",
	    "(6) Normal",
	    "(7) Addition",
        ""
    };

    char* collision_str[] =
    {
	    "(0) One",
	    "(1) Both",
	    "(2) Both w/scroll",
	    "(3) Moving floor",
        "(4) Moving water",
        ""
    };

    char* special_effect_str[] =
    {
	    "(0) Nothing",
        "(1) Unknown",
        "(2) Moving floor",
        "(3) Moving water",
        "(4) Unknown",
        "(5) Red flashes",
        "(6) Light torch to see floor",
        "(7) Ganon room",
        ""
    };

    char* special_routine_str[] =
    {
		"(00) Nothing",
        "(01) NW Kill enemy to open",
        "(02) NE Kill enemy to open",
        "(03) SW Kill enemy to open",
        "(04) SE Kill enemy to open",
        "(05) W Kill enemy to open",
        "(06) E Kill enemy to open",
        "(07) N Kill enemy to open",
        "(08) S Kill enemy to open",
        "(09) Clear quadrant to open",
        "(0A) Clear room to open",
		"(0B) NW Move block to open",
        "(0C) NE Move block to open",
        "(0D) SW Move block to open",
        "(0E) SE Move block to open",
        "(0F) W Move block to open",
        "(10) E Move block to open",
        "(11) N Move block to open",
		"(12) S Move block to open",
        "(13) Move block to open",
        "(14) Pull lever to open",
        "(15) Clear level to open door",
        "(16) Switch opens door(Hold)",
        "(17) Switch opens door(Toggle)",
		"(18) Turn off water",
        "(19) Turn on water",
        "(1A) Water gate",
        "(1B) Water twin",
        "(1C) Secret wall (Right)",
        "(1D) Secret wall (Left)",
        "(1E) Crash",
        "(1F) Crash",
		"(20) Use switch to bomb wall",
        "(21) Holes(0)",
        "(22) Open chest for holes(0)",
        "(23) Holes(1)",
        "(24) Holes(2)",
        "(25) Kill enemy to clear level",
        "(26) SE Kill enemy to move block",
        "(27) Trigger activated chest",
		"(28) Use lever to bomb wall",
        "(29) NW Kill enemy for chest",
        "(2A) NE Kill enemy for chest",
        "(2B) SW Kill enemies for chest",
        "(2C) SE Kill enemy for chest",
        "(2D) W Kill enemy for chest",
        "(2E) E Kill enemy for chest",
        "(30) N Kill enemy for chest",
		"(31) S Kill enemy for chest",
        "(32) Clear quadrant for chest",
        "(33) Clear room for chest",
        "(34) Light torches to open",
        "(35) Holes(3)",
        "(36) Holes(4)",
        "(37) Holes(5)",
        "(38) Holes(6)",
		"(39) Agahnim's room",
        "(3A) Holes(7)",
        "(3B) Holes(8)",
        "(3C) Open chest for holes(8)",
        "(3D) Move block to get chest",
        "(3E) Kill to open Ganon's door",
        "(3F) Light torches to get chest",
        "(40) Kill boss again",
        ""
	};

    char* plane_str[] = 
    {
        "(0) BG1 - high road",
        "(1) BG1 - low road",
        "(2) BG0",
        ""
    };

    char*  SprStrings[0xF4] =
    {
        "Raven",
        "Vulture",
        "Stalfos Head",
        "Don't use!",
        "Good Switch",
        "Unknown Switch (bad)",
        "Bad Switch",
        "Unknown Switch (bad)",
        "Octorock",
        "Moldorm (boss)",
        "Octorock (4-way)",
        "Chicken",
        "Octorock shot",
        "Buzz Blob",
        "Snap Dragon",
        "Octoballoon",
        "Octoballoon spawn",
        "Hinox",
        "Moblin",
        "Helmasaur",
        "Gargoyle Gate",
        "Fire Faerie",
        "Village Elder",
        "Water Bubble",
        "Moldorm",
        "Poe/Hue",
        "Dwarf",
        "Projectiles",
        "Moveable Statue",
        "Weathervane",
        "Crystal Switch",
        "Sick Kid",
        "Bomb Slug",
        "Push Switch",
        "Ropa",
        "Red Bari",
        "Blue Bari",
        "Talkative Tree",
        "Hardhat Beetle",
        "Deadrock",
        "Shrug Guy",
        "Guy in Blind's House",
        "Sweeping Lady",
        "Bridge Bum",
        "Lumberjack Bros.",
        "Telepathic Stones",
        "Flute Notes",
        "Race Guy/Girl",
        "Uknown Person 30",
        "Fortune Teller",
        "Uknown Person 32",
        "PullForRupees",
        "Scared Redhead",
        "Innkeeper",
        " Witch / Cane of Byrna?",
        "Waterfall",
        "Arrow Switch",
        "Middle Aged Guy",
        "Magic Powder Bat",
        "Dash Items",
        "Village Kid",
        "Uknown Sprite 3D",
        "Rock Crab",
        "Tutorial Soldier",
        "Electric Barrier",
        "Green Soldier",
        "Blue Soldier",
        "Red Spear Soldier",
        "Crazy Blue Soldier",
        "Crazy Red Soldier",
        "Blue Archer",
        "Green Archer (bush)",
        "Red Spear Soldier",
        "Red Spear Soldier (bush)",
        "Red Bomb Soldier",
        "Weak Green Soldier",
        "Geldman",
        "Swamp Bunny",
        "Snakebasket (fast)",
        "Snakebasket (slow)",
        "Metal Ball",
        "Armos",
        "Zora King",
        "Armos Knight",
        "Lanmola",
        "Zora / Ku",
        "Walking Zora",
        "Desert Barrier",
        "Green Crab",
        "Bird",
        "Squirrel",
        "Spark (clockwise)",
        "Spark (counter)",
        "Roller (down->up)",
        "Roller (up->down)",
        "Roller (right->left)",
        "Roller (left->right)	",
        "Statue Sentry	",
        "Master Sword	",
        "Sand Lion Pit	",
        "Sand Lion	",
        "Archery Shop Guy	",
        "Moving Cannon (66)	",
        "Moving Cannon (67)	",
        "Moving Cannon (68)	",
        "Moving Cannon (69)	",
        "Morning Star	",
        "Cannon Soldier	 WTF?)",
        "Warp Vortex	",
        "Rat / Bazu	",
        "Rope / Skullrope	",
        "Keese / Chasupa	",
        "Helmasaur Fireball",
        "Leever",
        "Pond Trigger",
        "Uncle / Sage",
        "Speedy Red Guy",
        "Village Merchant",
        "Princess Zelda",
        "Fire Faerie (invincible)",
        "Village Elder Lady",
        "Good Bee",
        "Agahnim",
        "Agahnim Energy Blast",
        "Timid Stalfos Skull",
        "Large Spike Trap",
        "Fireblade (counter)",
        "Fireblade (clockwise)",
        "Firesnake",
        "Waterbug / Tektite",
        "4 Fire Faeries",
        "Green Rocklops / Goriya",
        "Red Rocklops / Goriya",
        "Yellow Trap Stalfos",
        "Kodondo",
        "Kodondo Flames",
        "Mothula",
        "Mothula Beam",
        "Small Spike Block",
        "Gibdos",
        "Arrghus",
        "Arrghus Spawn",
        "Turtle Shell",
        "Slime / Blobs",
        "Wallmaster",
        "Stalfos Knight",
        "Helmasaur King",
        "Bungie / Red Orb",
        "Pirogisu / Floor Tile",
        "Eye Laser (right)",
        "Eye Laser (left)",
        "Eye Laser (down)",
        "Eye Laser (up)",
        "Penguin",
        "Live Water Bubble",
        "Wizzrobe",
        "Zoro (vermin)",
        "Babusu (black evil things)",
        "Ostrich",
        "Flute",
        "Bird (Flute Boy)",
        "Freezor (Ice man)",
        "Kholdstare",
        "Kholdstare Shell",
        "Falling Iceball",
        "Blue Mohack Lizard",
        "Red Mohawk Lizard",
        "Stalfos / Red Stalfos",
        "Zirro (Green Mushroom Bomber)",
        "Zirro (Blue Mushroom Bomber)",
        "Pikit",
        "Crystal Maiden",
        "Apple",
        "Old Man on Death Mountain",
        "Down Pipe",
        "Up Pipe",
        "Right Pipe",
        "Left Pipe",
        "Good Bee",
        "Hylian Writing",
        "Purple Mystery Chest",
        "Bomb Salesman (Dark World)",
        "Kiki the Monkey",
        "Blind the Maiden",
        "Text Debugger",
        "Feuding Friends",
        "Whirpool",
        "Chest Game Guy",
        "Drunk Guy in Bar",
        "Vitreous (large)",
        "Vitreous (small)",
        "Vitreous Lightning",
        "Quake Medallion Monster",
        "Agahnim (Scripted)",
        "Boulder",
        "Symbion (vulnerable)",
        "Forest Thief",
        "Fireball Emitter",
        "4-way Fireball Emitter",
        "Fuzzy Stack",
        "Big Faerie",
        "Tektite",
        "Chain Chomp",   
        "Trinexx Piece 1",
        "Trinexx Piece 2",
        "Trinexx Piece 3",
        "Blind the Thief (boss)",
        "Swamola",
        "Lynel (Centaur)",
        "Bunny Transformer",
        "Flopping Fish",
        "Reanimated Skull",
        "Landmine",
        "Digging Game Guy",
        "Ganon",
        "Ganon with Invincibility",
        "Heart",
        "Green Rupee",
        "Blue Rupee",
        "Red Rupee",
        "Bomb Refill (1)",
        "Bomb Refill (4)",
        "Bomb Refill (8)",
        "Small Magic Refill",
        "Full Magic Refill",
        "Arrow Refill (5)",
        "Arrow Refill (10)",
        "Faerie",
        "Key",
        "Big Key",
        "Red Shield",
        "Mushroom",
        "Fake Master Sword",
        "Magic Potion Guy / Potions",
        "Full Heart Container",
        "Quarter Heart Container",
        "Bush (thrown)",
        "Somaria Platform",
        "Hyrule Castle Mantle",
        "Somaria Platform",
        "Somaria Platform",
        "Somaria Platform",
        "Medallion Tablet",
        ""
    };

    char* OverStrings[0x1C] =
    {
        "Cannon Room",
        "??? 01",
        "Cannon Room",
        "Cannon Balls? 03",
        "Snake Trap 04 ???",
        "Stalfos Trap",
        "Snake Trap",
        "Moving Floor",
        "Falling blobs",
        "Wallmaster Spawner",
        "Falling tiles Overlord 0A",
        "Falling tiles Overlord 0B",
        "Falling tiles Overlord 0C",
        "Falling tiles Overlord 0D",
        "Falling tiles Overlord 0E",
        "Falling tiles Overlord 0F",
        "Fish producer (right)",
        "Fish producer (left)",
        "Fish producer (down)",
        "Fish producer (up)",
        "Attacking Floor Tiles",
        "Wizzrobe Spawner",
        "Vermin Spawner",
        "Floor Trap Trigger",
        "Stalfos Poofing In 18",
        "Armos Knights Overlord",
        "Bomb Trap",
        ""
    };

    char* ItemStrings[0x101] =
    {
        "Nothing", "Green Rupee",   "Rupee Crab", "Bee",         "Green Rupee", "Bomb",        "Heart",   "Blue Rupee", 
        "Key",     "Arrow",         "Bomb",       "Heart",       "Small Magic", "Large Magic", "Chicken", "Blue Soldier", 
        "Skull",   "Green Soldier", "Landmine",   "Green Rupee", "Faerie",      "Heart",       "",        "",
        "", "", "", "", "", "", "", "", 
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 0x20
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 0x30
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 0x40
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 0x50
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 0x60
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 0x70
        "Hole", "", "Warp", "", "Staircase", "Bombable", "Switch",  "", // 0x80
        "",     "", "",     "", "",          "",         "",        "", 
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 0x90
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 0xA0
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 0xB0
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 0xC0
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 0xD0
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 0xE0
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", // 0xF0
        "" // empty terminator
    };

    unsigned int entranceBufSizes[] = { 2, 8, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1 }; 
    char editBuf[512];
    char msgBuffer[256];

    int quit;
    int numGames;
    int zgIndex;

    RGBQUAD mainPalette[256];

    DungeonFile portFile;

    OPENFILENAME bmOFN;

    BITMAPINFOHEADER* bmInfoHeadPtr;
    HBITMAP mainBM;
    SnesPalPtr defaultPal;
    int totalMem;

    HANDLE testFH;
    HWND windowList[0x100];
    zgPtr newGamePtr;

    BITMAPINFO* bmInfoPtr;

    HINSTANCE thisProg;
    HDC mainDC;
    RECT mainRect;
    MSG mainMsg;
    WNDCLASSEX mainClass;
    WNDCLASSEX pictureClass;

    LONG oldEditProc;

    int expansionList[] =  {IDC_8MB,
                            IDC_12MB,
                            IDC_16MB,
                            IDC_20MB,
                            IDC_24MB,
                            IDC_28MB,
                            IDC_32MB};


    WindowElement dungTemp[] =
    {
        { editExStyle,    "EDIT",    editBuf,   editStyle | ES_READONLY, 60, 0, 30, 20, 0, (HMENU) ID_DungAttrNum, 0, 0, 0, 12},
        { editExStyle,    "EDIT",    editBuf,   editStyle | ES_READONLY, 90, 0, 60, 20, 0, (HMENU) ID_DungAttrAddress, 0, 0, 0, 0},
        { editExStyle,    "EDIT",    editBuf,   editStyle | ES_READONLY, 150, 0, 60, 20, 0, (HMENU) ID_DungTilemapAddress, 0, 0, 0, 0},
        { editExStyle,    "EDIT",    editBuf,   editStyle | ES_READONLY, 210, 0, 60, 20, 0, (HMENU) ID_DungTilemapNum, 0, 0, 0, 0},
        { editExStyle,    "EDIT",    editBuf,   editStyle | ES_READONLY, 270, 0, 60, 20, 0, (HMENU) ID_DungRoomNum, 0, 0, 0, 0},
        { pictureExStyle, "PICTURE", "DUNGPIC", pictureStyle|WS_GROUP, 80, 80, 512, 512, 0, (HMENU) ID_DungPictureBox, 0, 0, 0, 0},
        { buttonExStyle,  "BUTTON",  "->",      buttonStyle|WS_GROUP, 0, 0, 50,  30, 0, (HMENU) ID_DungJumpButton, 0, 0, 0, 0},
        { buttonExStyle,  "BUTTON",  "Header",  buttonStyle|WS_GROUP, 0, 60, 50, 30, 0, (HMENU) ID_DungHeaderButton, 0, 0, 0, 0},
        { buttonExStyle,  "BUTTON",  "<-",      buttonStyle|WS_GROUP, 0, 30, 50, 30, 0, (HMENU) ID_DungRoomButton, 0, 0, 0, 0},
        { buttonExStyle,  "BUTTON",  "Search",  buttonStyle|WS_GROUP, 0, 90, 50, 30, 0, (HMENU) ID_DungSearchButton, 0, 0, 0, 0},
        { radioExStyle,   "BUTTON",  "BG1",     radioStyle|WS_GROUP, 0, 120, 50, 30, 0, (HMENU) ID_DungLayer1Radio, 0, 0, 0, 0},
        { radioExStyle,   "BUTTON",  "BG2",     radioStyle|WS_GROUP, 0, 150, 50, 30, 0, (HMENU) ID_DungLayer2Radio, 0, 0, 0, END_FLAG}
    };

    WindowElement overTemp[] =
    {
        { editExStyle, "EDIT", editBuf, editStyle, 0, 0x400, 100, 20, 0, (HMENU) ID_OverPaletteNum, 0, 0, 0, 2},
        { pictureExStyle, "PICTURE", "OVERPIC", pictureStyle|WS_GROUP, 0, 0, 0x400, 0x400, 0, (HMENU) ID_DungPictureBox, 0, 0, 0, END_FLAG},

    };

    // List of what values of $0AA1 correspond to

    // 0x00 - hyrule castle throne room
    // 0x01 - dungeon interior?
    // 0x02 - agahnim's room
    // 0x03 - houses
    // 0x04 - hyrule castle part 1 / sanctuary / hyrule castle part 2
    // 0x05 - eastern palace (dungeon) / tower of hera
    // 0x06 - cave
    // 0x08 - swamp palace / watergate
    // 0x09 - skull woods palace
    // 0x0A - gargoyle's domain / blind's old hideout
    // 0x0B - ice palace (dungeon)
    // 0x0C - misery mire
    // 0x0D - turtle rock
    // 0x0E - Ganon's tower
    // 0x0F - sahashralah's hideout

    // 0x11 - fortune teller / bow & arrow game
    // 0x12 - desert palace / pyramid of power fountain
    // 0x13 - Ganon pit room
    // 0x14 - shopkeeper / storytellers
    // 0x15 - ???

    
    
    // 0x20 - light world overworld / map screen
    // 0x21 - dark world overworld
    // 0x22 - 2bpp graphics? why?
    // 0x23 - title screen / menus
    // 0x24 - triforce room


    // List of what values of $0AA2 correspond to

    // 0x02 - hyrule castle part 2
    // 0x03 - houses
    // 0x04 - sanctuary
    // 0x05 - desert palace / eastern palace (dungeon) / tower of hera
    // 0x06 - cave interior
    // 0x08 - swamp palace / watergate
    // 0x09 - skull woods palace
    // 0x0A - gargoyle's domain / blind's old hideout
    // 0x0B - ice palace (dungeon)
    // 0x0C - misery mire
    // 0x0D - turtle rock
    // 0x0E - Ganon's tower
    // 0x0F - sahashralah's

    // 0x10 - cape room
    // 0x11 - fortune teller / bow & arrow game
    // 0x13 - Ganon's room
    // 0x14 - shopkeeper / storytellers

    // 0x20 - default light world
    // 0x21 - forest
    // 0x22 - light world death mountain
    // 0x23 - village / fortune teller
    // 0x24 - hyrule castle
    // 0x25 - eastern palace (ruins)
    // 0x26 - desert area / village area
    // 0x27 - wood bridge and stone bridge near hyrule castle
    // 0x28 - unused
    // 0x29 - sanctuary / graveyard / witch hut
    // 0x2A - library
    // 0x2B - desert area
    // 0x2C - unused
    // 0x2D - unused
    // 0x2E - unused
    // 0x2F - master sword / under bridge / zora falls

    // 0x30 - default dark world
    // 0x31 - forest (repeated)
    // 0x32 - light world death mountain (repeated)
    // 0x33 - village / fortune teller (repeated)
    // 0x34 - hyrule castle (repeated)
    // 0x35 - eastern palace (ruins) (repeated)
    // 0x36 - desert area / village area (repeated)
    // 0x37 - wood bridge / stone bridge near hyrule castle (repeated)
    // 0x38 - unused
    // 0x39 - sanctuary / graveyard / witch hut
    // 0x3A - library (without village elements)
    // 0x3B - pyramid of power
    // 0x3C - turtle rock
    // 0x3D - maze / dark palace area
    // 0x3E - skull forest
    // 0x3F - dark world village

    // 0x40 - lake of ill omen / dark world graveyard / map screen
    // 0x41 - dark world death mountain
    // 0x42 - swamp of evil / dark world lake hylia
    // 0x43 - unused
    // 0x44 to 0x4F - unused
    // 0x50 - 2bpp graphics
    // 0x51 - triforce room

    unsigned long numBytesRead;

    void stupidFunc()
    {
        ;
    }

    // image testing stuff





