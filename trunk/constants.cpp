#include "Globals.h"

    #include "windows.h"

    HFONT courier = CreateFont(15,7,
                               0,0,
                               FW_THIN, FALSE, FALSE, FALSE,
                               ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                               CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH | FF_DONTCARE, "courier new");
  
    // hvop ppcc cccc cccc (quick map8 guide)

    u32 test[]       = { 0x1106, 0x1107, 0x5107, 0x5106, 0x1138, 0x1139, 0x5139, 0x5138 };
    u32 bush[]       = { stock_bush };
    u32 bush2[]      = { stock_bush2 };
    u32 rock_sl[]    = { stock_rock_sl};
    u32 rock_sh[]    = { stock_rock_sh };
    u32 peg[]        = { stock_peg };
    u32 rock_bl[]    = { stock_rock_bl0, stock_rock_bl1, stock_rock_bl2, stock_rock_bl3 };
    u32 rock_bh[]    = { stock_rock_bh0, stock_rock_bh1, stock_rock_bh2, stock_rock_bh3 };
    u32 warp[]       = { stock_warp };
    u32 rockPile[]   = { stock_rockPile0, stock_rockPile1, stock_rockPile2, stock_rockPile3 };
    u32 sign[]       = { stock_sign };
    u32 grass[]      = { stock_grass };
    u32 jumpPoint[]  = { stock_jumpPoint };

    OverObj over_stock_objs[12] = 
    {
        OverObj(bush,      1, 1, IDC_STOCK_BUSH,      fromMap16),
        OverObj(bush2,     1, 1, IDC_STOCK_BUSH2,     fromMap16),
        OverObj(rock_sl,   1, 1, IDC_STOCK_ROCK_SL,   fromMap16),
        OverObj(rock_sh,   1, 1, IDC_STOCK_ROCK_SH,   fromMap16),
        OverObj(peg,       1, 1, IDC_STOCK_PEG,       fromMap16),
        OverObj(rock_bl,   2, 2, IDC_STOCK_ROCK_BL,   fromMap16),
        OverObj(rock_bh,   2, 2, IDC_STOCK_ROCK_BH,   fromMap16),
        OverObj(warp,      1, 1, IDC_STOCK_WARP,      fromMap16),
        OverObj(rockPile,  2, 2, IDC_STOCK_ROCKPILE,  fromMap16),
        OverObj(sign,      1, 1, IDC_STOCK_SIGN,      fromMap16),
        OverObj(grass,     1, 1, IDC_STOCK_GRASS,     fromMap16),
        OverObj(jumpPoint, 1, 1, IDC_STOCK_JUMPPOINT, fromMap16)
    };

    /*
    u32 over_stock_map16[0x10] = 
    {
        stock_bush,         
        stock_bush2,
        stock_rock_sl,
        stock_rock_sh,
        stock_peg,
        stock_rock_bl0,
        stock_rock_bh0,
        stock_warp,
        stock_rockpile0,
        stock_sign,
        stock_grass,
        stock_jumpPoint
    };
    */

    /*
    int over_stock_ids[0x10] = 
    {
        IDC_STOCK_BUSH,  
        IDC_STOCK_BUSH2,    
        IDC_STOCK_ROCK_SL,
        IDC_STOCK_ROCK_SH,
        IDC_STOCK_PEG,      
        IDC_STOCK_ROCK_BL,  
        IDC_STOCK_ROCK_BH,  
        IDC_STOCK_WARP,     
        IDC_STOCK_ROCKPILE, 
        IDC_STOCK_SIGN,     
        IDC_STOCK_GRASS,    
        IDC_STOCK_JUMPPOINT
    };
    */
