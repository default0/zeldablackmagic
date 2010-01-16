// constants.h 
// contains, you guessed it, constant definitions


#ifndef CONSTANTS_H
#define CONSTANTS_H

    #define charSize        1

    #define  _MEGABIT       0x20000   // size of a megabit, in terms of bytes.
    #define  _4MEGABITS     (_MEGABIT * 4)
    #define  _MEGABYTE      (_MEGABIT * 8)

    #define END_FLAG            0x1000

    // window styles ---------------------------------------------

    #define buttonStyle         WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_TABSTOP
    #define buttonExStyle       0

    #define editStyle           WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL|ES_LEFT
    #define editExStyle         WS_EX_CLIENTEDGE

    #define staticStyle         buttonStyle
    #define staticExStyle       0

    #define pictureStyle        buttonStyle
    #define pictureExStyle      0

    #define radioStyle          buttonStyle
    #define radioExStyle        0

    #define numDungElements     12

    // dungeon asm pointers -------------------------------------

    #define asm_header_ref      0xB5DD
    #define asm_header_bank     0xB5E7

    #define asm_object_ref      0x874C
    #define asm_object_ref_2    0x8746
    #define asm_object_ref_3    0x8845
    #define asm_object_ref_4    0x883F

    #define asm_entrance_ref    0x14813

    #define asm_num_chests      0xEBF6
    #define asm_chest_ref       0xEBFB

    #define asm_sprite_ref      0x4C298
    #define asm_sprite_hook     0x4C16E

    // #define asm_sprite_local   0x4CEE7 Shards of Might's location (doesn't work anyway)
    #define asm_sprite_local    0x4D62E

    #define asm_text_ref        0x744ED // where the code gets the absolute portion
    #define asm_text_ref2       0x744F3 // the code needs this for the bank byte
    #define asm_code_lengths    0x7536B

    // overworld asm pointers -------------------------------------

    // table of 24-bit pointers to the first half of each overworld map
    // In a clean rom this should be 0x2F94D (0x1794D in rom)
    #define asm_overmap_ptrs        0x1759E

    // table of 24-bit pointers to the second half of each overworld map
    // In a clean rom this should be 0x2F5C9 (0x175C9 in rom)
    #define asm_overmap_ptrs2       0x175C9

    // if the map16 loader is untouched these should all be 0x038000
    #define asm_map32To16UL         0x176AC
    #define asm_map32To16UR         0x176EC
    #define asm_map32To16LL         0x1772C
    #define asm_map32To16LR         0x1776C

    // defaults to $1BB96F ($DB96F in rom)
    #define asm_entrance_ptr_old    0xDBD1A

    // in a clean rom this is 0xB800 (2 bytes) which references 0x1BB800 (0xDB800) in rom
    #define asm_hole_ptr_old        0xDB894

    // graphics asm pointers ---------------------------------------

    // ROM addresses of asm location where an untouched Zelda 3 rom will load graphics from
    #define asm_spr_gfx_ptr             0x553C
    #define asm_bg_gfx_ptr              0x6790

    #define default_spr_gfx_ptr         0xCFF3
    #define default_bg_gfx_ptr          0xCF80

    // black magic specific pointers ------------------------------
    #define bm_header_loc       0xFFFFC

    // assembler macros for generating code -----------------------    

    #define _ASLA               0x0A

    #define _REP(x)             0xC2,x
    #define _SEP(x)             0xE2,x

    #define _LDAD(x)            0xA5,x
    #define _LDAI(x)            0xA9,x
    #define _LDALX(x,y,z)       0xBF,z,y,x
    #define _LDAX(x,y)          0xBD,y,x
    #define _LDAY(x)            0xB9,x
    #define _LDAA(x,y)          0xAD,y,x

    #define _STADP(x)           0x85,x

    #define _PEA(x,y)           0xF4,y,x

    #define _TAY                0xA8
    #define _TAX                0xAA
    #define _TXA                0x8A

    #define _JML(x,y,z)         0x5C,z,y,x
    #define _JSL(x,y,z)         0x22,z,y,x
    #define _PHA                0x48
    #define _PLB                0xAB

    // symbol defined after VS 6.0 used with alpha channel --------

	#ifndef AC_SRC_ALPHA
    	#define AC_SRC_ALPHA	   0
	#endif

    // main window identifiers ------------------------------------	

    enum    {   
                ID_DungPaletteNum = 0x3000,
                ID_DungJumpButton,
                ID_DungHeaderButton,    
                ID_DungAttrAddress,
                ID_DungAttrNum,
                ID_DungLayer1Radio,
                ID_DungLayer2Radio,
                ID_DungPictureBox,
                ID_DungRoomButton,
                ID_DungRoomNum,
                ID_DungSearchButton,
                ID_DungTilemapAddress,
                ID_DungTilemapNum
            };

    enum { ID_OverPaletteNum };

    // application specific window message ------------------------

    enum    {
                DUNG_INIT = WM_APP,
                DUNG_CHANGE,
                DUNG_SEARCH,
                DUNG_SAVE,
                PICTURE_TIMER,
                PICTURE_MOUSEMOVE,
                PICTURE_LBUTTON_DOWN,
                PICTURE_LBUTTON_UP,
                PICTURE_RBUTTON_DOWN,
                PICTURE_PAINT,
                PICTURE_CHAR,
                PICTURE_KEYDOWN,
                BM_TEARDOWN,

                TOOL_SPRITE,
                SPRITE_STOPPED,
                SPRITE_MOVE,

                OVER_INIT,
                OVER_CHANGE,
                OVER_SAVE,            
            };

    // palette offsets. corresponds to positions in CGRAM (palette RAM)
    // BP = background palette
    // SP = sprite palette

    enum
    {
        BP0 = 0x00,
        BP1 = 0x20,
        BP2 = 0x40,
        BP3 = 0x60,
        BP4 = 0x80,
        BP5 = 0xA0,
        BP6 = 0xC0,
        BP7 = 0xE0,
        SP0 = 0x100,
        SP1 = 0x120,
        SP2 = 0x140,
        SP3 = 0x160,
        SP4 = 0x180,
        SP5 = 0x1A0,
        SP6 = 0x1C0,
        SP7 = 0x1E0
    };

    // map16 offsets for use in interlacing the map32 maps into the map16 structure
    typedef enum
    {
        upper_left  = 0x0000,
        upper_right = 0x0040,
        lower_left  = 0x1000,
        lower_right = 0x1040
    } map16Pos;

    // overworld editing modes
    typedef enum
    {
        mode_drawtile    = 0,   // drawing one tile at a time
        mode_stock,             // drawing stock objects
        mode_custom,            // drawing custom objects
        mode_select,            // select rectangular regions and move / copy them
        mode_sprite,            // manipulating the sprite sets
        mode_item,              // manipulating the things that appear under rocks and such
        mode_hole,              // manipulating the game's holes
        mode_exit,              // placement and management of exits (links back to the overworld)
        mode_entrance,          // placement and management of entrances (links to dungeons)
        move_overlay            // management of overlays (flag triggered map modifications)
    } owEditMode;

    // stock object (overworld) types, listed out as map16 values

    typedef enum
    {
        stock_bush      = 0x0036,
        stock_bush2     = 0x072A,
        
        stock_rock_sl   = 0x020F,
        stock_rock_sh   = 0x0239,
        
        stock_peg       = 0x021B,

        stock_rock_bl0  = 0x036D,
        stock_rock_bl1  = 0x036E,
        stock_rock_bl2  = 0x0374,
        stock_rock_bl3  = 0x0375,

        stock_rock_bh0  = 0x023B,
        stock_rock_bh1  = 0x023C,
        stock_rock_bh2  = 0x023D,
        stock_rock_bh3  = 0x023E,

        stock_warp      = 0x0212,

        stock_rockPile0 = 0x0226,
        stock_rockPile1 = 0x0227,
        stock_rockPile2 = 0x0228,
        stock_rockPile3 = 0x0229,

        stock_sign      = 0x0101,

        stock_grass     = 0x037E,

        stock_jumpPoint = 0x066B,

        stock_stump0    = 0x008E,
        stock_stump1    = 0x008F,
        stock_stump2    = 0x0098,
        stock_stump3    = 0x0099,

        stock_burrow0   = 0x0918,
        stock_burrow1   = 0x0919,
        stock_burrow2   = 0x091A,
        stock_burrow3   = 0x091B

    } stockMap16;

#endif
