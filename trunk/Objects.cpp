// This file contains code for rendering the dungeon objects into a tilemap


#include "Objects.h"
#include "Core.h"

object Type1_1Object[0xF8] = 
{
    do_8B89,    do_8A92,    do_8A92,    do_8B0D, // 0x00
    do_8B0D,    do_8C73,    do_8C73,    do_8B79,
    do_8B79,    do_8C58,    do_8C61,    do_8C61,
    do_8C58,    do_8C58,    do_8C61,    do_8C61,
    do_8C58,    do_8C58,    do_8C61,    do_8C61, // 0x10
    do_8C58,    sh_8C6A,    sh_8CB9,    sh_8CB9,
    sh_8C6A,    sh_8C6A,    sh_8CB9,    sh_8CB9,
    sh_8C6A,    sh_8C6A,    sh_8CB9,    sh_8CB9,
    sh_8C6A,    do_8D5D,    sh_8EF0,    do_8F62, // 0x20
    do_8F62,    do_8F62,    do_8F62,    do_8F62,
    do_8F62,    do_8F62,    do_8F62,    do_8F62,
    do_8F62,    do_8F62,    do_8F62,    do_8FBD,
    do_9001,    do_free,    do_free,    do_9111, // 0x30
    do_9136,    do_free,    do_92FB,    do_92FB,
    do_9323,    do_936F,    do_9387,    do_9387,
    do_93B7,    do_936F,    do_9456,    do_8F62,
    do_8F62,    do_8F62,    do_8F62,    do_8F62, // 0x40
    do_8F62,    do_8F62,    do_8F62,    do_9466,
    do_9488,    do_94B4,    do_94B4,    do_9456,
    do_94BD,    do_94DF,    do_94DF,    do_94DF,
    do_96DC,    do_9CC6,    do_9CC6,    do_8B79, // 0x50
    do_free,    do_96F9,    do_96F9,    do_free,
    do_free,    do_free,    do_free,    do_9CC6,
    do_9CC6,    do_8F36,    do_9338,    sh_8EEB,
    do_8B7E,    do_8A89,    do_8A89,    do_8AA4, // 0x60
    do_8AA4,    do_8C4F,    do_8C4F,    do_8B74,
    do_8B74,    sh_8EC3,    do_8F8A,    do_8F8A,
    do_9045,    do_908F,    do_free,    do_free,
    do_90F9,    do_9120,    do_free,    do_930E, // 0x70
    do_930E,    do_9357,    do_939F,    do_939F,
    do_9446,    do_8F8A,    do_8F8A,    do_9446,
    do_96E4,    do_8B74,    do_free,    do_9702,
    do_9702,    do_971B,    do_971B,    do_971B, // 0x80
    do_971B,    do_9CEB,    do_9CEB,    do_9357,
    do_8F0C,    do_9347,    sh_8EBE,    do_90E2,
    do_90E2,    do_8F8A,    do_8F8A,    do_97B5,
    do_8A89,    do_8A89,    do_8B7E,    do_8B7E, // 0x90
    do_90F9,    do_B381,    do_B47F,    do_free,
    do_free,    do_free,    do_free,    do_free,
    do_free,    do_free,    do_free,    do_free,
    do_8BE0,    do_8BF4,    do_8C0E,    do_8C22, // 0xA0
    do_8DDC,    do_8E67,    do_8E7B,    do_8E95,
    do_8EA9,    do_8E67,    do_8E7B,    do_8E95,
    do_8EA9,    do_free,    do_free,    do_free,
    do_90D9,    do_90D9,    do_9111,    do_8F62, // 0xB0
    do_8F62,    do_97DC,    do_8A92,    do_8A92,
    do_8B89,    do_8B89,    do_9111,    do_9338,
    do_B376,    do_B474,    do_free,    do_free,
    do_8B94,    do_8CC7,    do_8B94,    do_8D9E, // 0xC0
    do_8FA2,    do_8FA5,    do_8FA5,    do_8FA5,
    do_8FA5,    do_8FA5,    do_8FA5,    do_free,
    do_free,    do_null,    do_null,    do_free, // do invisible walls later :(
    do_free,    do_8FA5,    do_8FA5,    do_free, // 0xD0
    do_free,    do_free,    do_free,    do_8D9E,
    do_9501,    do_8FA5,    do_95EF,    do_8F9D,
    do_9733,    do_93DC,    do_9429,    do_8FA5,
    do_8FA5,    do_8FA5,    do_8FA5,    do_8FA5, // 0xE0
    do_8FA5,    do_8FA5,    do_8FA5,    do_8FA5,
    do_8FA5,    do_free,    do_free,    do_free,
    do_free,    do_free,    do_free,    do_free,
    do_free,    do_free,    do_free,    do_free, // 0xF0
    do_free,    do_free,    do_free,    do_free
};

object Type1_2Object[0x40] = 
{
    do_97ED,    do_97ED,    do_97ED,    do_97ED, // 0x00
    do_97ED,    do_97ED,    do_97ED,    do_97ED,
    do_9813,    do_9813,    do_9813,    do_9813,
    do_9813,    do_9813,    do_9813,    do_9813,
    do_9854,    do_9854,    do_9854,    do_9854, // 0x10
    do_985C,    do_985C,    do_985C,    do_985C,
    do_9895,    do_9895,    do_9895,    do_9895, // 0x18
    do_97ED,    do_8F30,    do_B2AF,    do_9A6F,
    do_9892,    do_8F30,    do_9AEE,    do_99E6, // 0x20
    do_97ED,    do_97ED,    do_8F30,    do_9895,
    do_9AEE,    do_97ED,    do_9B48,    do_9895, // 0x28
    do_9B50,    do_A41B,    do_A458,    do_A486,
    do_A25D,    do_A26D,    do_A2C7,    do_A2DF, // 0x30
    do_9895,    do_9B1E,    do_A3AE,    do_9BF8,
    do_A4B4,    do_A533,    do_A4F5,    do_A584,
    do_9B56,    do_99E6,    do_9A0C,    do_9A12
};

object Type1_3Object[0x80] = 
{
    do_9D29,    do_9D5D,    do_9D67,    do_9C3B, // 0x00
    do_9C3E,    do_9C3E,    do_9C3E,    do_9C3E,
    do_9C3E,    do_9C3E,    do_9C3E,    do_9C3E,
    do_9C3E,    do_9C44,    do_9C3B,    do_9C3E,
    do_9895,    do_9895,    do_9AA9,    do_9895, // 0x10
    do_99E6,    do_9D96,    do_B493,    do_9C44,
    do_98AE,    do_98D0,    do_9895,    do_A30C,
    do_A31C,    do_A36E,    do_A5D2,    do_A5F4,
    do_A607,    do_A626,    do_9895,    do_9895, // 0x20
    do_9895,    do_9895,    do_A664,    do_A695,
    do_A71C,    do_A74A,    do_9DE5,    do_B306,
    do_B310,    do_9E30,    do_9EA3,    do_B395,
    do_B30B,    do_99BB,    do_9A00,    do_A380, // 0x30
    do_9BD9,    do_9BD9,    do_9B50,    do_9B50,
    do_9BD9,    do_9BD9,    do_9A90,    do_9A90,
    do_9AA3,    do_9AA3,    do_9895,    do_9895,
    do_9895,    do_9895,    do_9895,    do_9895, // 0x40
    do_9895,    do_9895,    do_9895,    do_B3E1,
    do_97ED,    do_9895,    do_9895,    do_9A06,
    do_9A66,    do_9A0C,    do_99E6,    do_9895,
    do_B2AF,    do_9895,    do_9895,    do_9895, // 0x50
    do_A095,    do_A194,    do_9895,    do_9895,
    do_9895,    do_9895,    do_9D6C,    do_A194,
    do_9AA3,    do_9A0C,    do_9895,    do_9895,
    do_A7A3,    do_A7A3,    do_A1D1,    do_9895, // 0x60
    do_9895,    do_9895,    do_97ED,    do_99E6,
    do_99E6,    do_99EC,    do_99EC,    do_97ED,
    do_99E6,    do_99E6,    do_99EC,    do_99EC,
    do_A7B6,    do_A7D3,    do_9DD9,    do_A255, // 0x70
    do_A7DC,    do_9895,    do_9A06,    do_9A06,
    do_A7F0,    do_99E6,    do_97ED,    do_A809,
    do_9895,    do_9895,    do_9895,    do_free
};

object Type2_Object[4] =
{
    do_A81C,    do_A984,    do_AAD7,    do_AB99
};

/*
object Type2_Object[4] = 
{
    do_A81C,    do_A984,    do_AAD7,    do_AB99
};*/

void do_8A1F(zgPtr game, DngObj *doPtr, u32 index)
{
    // index is $0C

    u32 x = doPtr->tileNum;
    u32 y = index;
    u32 i = 0; // i is $0E
    bufPtr rom = game->image;

    while(index < 0x08)
    {
        y = index;
        y = Get2Bytes(rom, 0x1B02 + y); i = 0x08;
                
        while(i > 0)
        {
            doPtr->mapAddr = y; doPtr->tileNum = x; 

            do_8A44(game, doPtr, 0x08);

            x = doPtr->tileNum; y = doPtr->mapAddr; i--;
            y += 0x1C0;
        }

        index += 0x02;
    }

    return;
}

void do_8A44(zgPtr game, DngObj *doPtr, u32 index)
{
    u32 i = index; // i is $0A
    u32 j = 0; // j is $04
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;
    bufPtr rom = game->image;

    while(i > 0)
    {
        j = 0x02;

        while(j > 0)
        {
            RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x)); 
            RegObjTile(game, doPtr, 0x002 + y, Get2Bytes(rom, 0x1B54 + x)); 
            RegObjTile(game, doPtr, 0x004 + y, Get2Bytes(rom, 0x1B56 + x)); 
            RegObjTile(game, doPtr, 0x006 + y, Get2Bytes(rom, 0x1B58 + x)); 
            RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B5A + x)); 
            RegObjTile(game, doPtr, 0x082 + y, Get2Bytes(rom, 0x1B5C + x)); 
            RegObjTile(game, doPtr, 0x084 + y, Get2Bytes(rom, 0x1B5E + x)); 
            RegObjTile(game, doPtr, 0x086 + y, Get2Bytes(rom, 0x1B60 + x)); 

            y += 0x100; j--;
        }
        
        y -= 0x1F8; i--;
    }

    doPtr->tileNum = x; doPtr->mapAddr = y;

    return;
}

void do_8A89(zgPtr game, DngObj *doPtr)
{
    u32 index = 0x100;

    do_B0BE(doPtr);

    do_B220(game, doPtr, index);

    return;
}

void do_8A92(zgPtr game, DngObj *doPtr)
{
    // simulation of $B0BE in rom
    // arg1 corresponds to $B2
    // arg2 corresponds to $B4
    // arg3 corresponds to the value plucked from $8000
    // but it basically is X or $0A in this context
    // mapNum is the index for which tilemap to draw into (0 or 1)
    // mapAddr corresponds to Y or $08 in this context

    u32 index = 0;
    u32 x = doPtr->tileNum;

    do_B0BE(doPtr);

    while(doPtr->arg1 > 0)
    {
        index = 2;

        do_97F0(game, doPtr, index);

        doPtr->tileNum = x;

        doPtr->arg1--;
    }

    return;
}

void do_8AA4(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;
    u32 a = 0;
    u32 y = doPtr->mapAddr;
    u32 mapNumTemp = doPtr->mapNum;

    a = x; x = y; y = a;

    bufPtr BG_0 = game->BG_Map[0];
    bufPtr BG_1 = game->BG_Map[1];
    bufPtr rom = game->image;

    do_B0AC(doPtr);

    while( doPtr->arg1 > 0 )
    {
        doPtr->mapNum = 0;

        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x002 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x004 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x006 + x, Get2Bytes(rom, 0x1B58 + y));
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B5A + y)); 
        RegObjTile(game, doPtr, 0x082 + x, Get2Bytes(rom, 0x1B5C + y));
        RegObjTile(game, doPtr, 0x084 + x, Get2Bytes(rom, 0x1B5E + y));
        RegObjTile(game, doPtr, 0x086 + x, Get2Bytes(rom, 0x1B60 + y));

        doPtr->mapNum = 1;

        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x002 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x004 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x006 + x, Get2Bytes(rom, 0x1B58 + y));
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B5A + y)); 
        RegObjTile(game, doPtr, 0x082 + x, Get2Bytes(rom, 0x1B5C + y));
        RegObjTile(game, doPtr, 0x084 + x, Get2Bytes(rom, 0x1B5E + y));
        RegObjTile(game, doPtr, 0x086 + x, Get2Bytes(rom, 0x1B60 + y));

        x += 0x100;
        doPtr->arg1--;
    }

    doPtr->mapNum = mapNumTemp;

    return;
}

void do_8B0D(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;
    u32 a = 0;
    u32 y = doPtr->mapAddr;
    u32 mapNumTemp = doPtr->mapNum;

    a = x;
    x = y;
    y = a;

    bufPtr BG_0 = game->BG_Map[0];
    bufPtr BG_1 = game->BG_Map[1];
    bufPtr rom = game->image;

    do_B0AC(doPtr);

    while( doPtr->arg1 > 0 )
    {
        doPtr->mapNum = 0;

        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));
        RegObjTile(game, doPtr, 0x002 + x, Get2Bytes(rom, 0x1B5A + y)); 
        RegObjTile(game, doPtr, 0x082 + x, Get2Bytes(rom, 0x1B5C + y));
        RegObjTile(game, doPtr, 0x102 + x, Get2Bytes(rom, 0x1B5E + y));
        RegObjTile(game, doPtr, 0x182 + x, Get2Bytes(rom, 0x1B60 + y));

        doPtr->mapNum = 1;

        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));
        RegObjTile(game, doPtr, 0x002 + x, Get2Bytes(rom, 0x1B5A + y)); 
        RegObjTile(game, doPtr, 0x082 + x, Get2Bytes(rom, 0x1B5C + y));
        RegObjTile(game, doPtr, 0x102 + x, Get2Bytes(rom, 0x1B5E + y));
        RegObjTile(game, doPtr, 0x182 + x, Get2Bytes(rom, 0x1B60 + y));

        x += 4;
        doPtr->arg1--;
    }

    doPtr->mapNum = mapNumTemp;

    return;
}

void do_8B74(zgPtr game, DngObj *doPtr)
{
    do_B0AC(doPtr);
 
    while(doPtr->arg1 > 0)
    {
        do_B2AF(game, doPtr);

        doPtr->arg1--;
    }

    return;
}


void do_8B79(zgPtr game, DngObj *doPtr)
{
    do_B0AC(doPtr);

    while(doPtr->arg1 > 0)
    {
        do_9895(game, doPtr);

        doPtr->arg1--;
    }

    return;
}

void do_8B7E(zgPtr game, DngObj *doPtr)
{
    do_B0CC(doPtr);

    while(doPtr->arg1 > 0)
    {
        do_B2AF(game, doPtr);

        doPtr->arg1--;
    }

    return;
}

void do_8B89(zgPtr game, DngObj *doPtr)
{
    do_B0CC(doPtr);

    while(doPtr->arg1 > 0)
    {
        do_9895(game, doPtr);

        doPtr->arg1--;
    }

    return;
}

void do_8B94(zgPtr game, DngObj *doPtr)
{
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];
    bufPtr rom = game->image;

    u32 value = 0;
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;
    u32 i = 0;

    doPtr->arg1++; doPtr->arg2++;

    while(doPtr->arg2 > 0)
    {
        i = doPtr->arg1;

        while(i > 0)
        {
            value = Get2Bytes(rom, 0x1B52 + x);
            RegObjTile(game, doPtr, 0x000 + y, value);
            RegObjTile(game, doPtr, 0x002 + y, value);
            RegObjTile(game, doPtr, 0x004 + y, value);
            RegObjTile(game, doPtr, 0x006 + y, value);
            RegObjTile(game, doPtr, 0x080 + y, value);
            RegObjTile(game, doPtr, 0x082 + y, value);
            RegObjTile(game, doPtr, 0x084 + y, value);
            RegObjTile(game, doPtr, 0x086 + y, value);

            y += 0x100;

            value = Get2Bytes(rom, 0x1B52 + x);
            RegObjTile(game, doPtr, 0x000 + y, value);
            RegObjTile(game, doPtr, 0x002 + y, value);
            RegObjTile(game, doPtr, 0x004 + y, value);
            RegObjTile(game, doPtr, 0x006 + y, value);
            RegObjTile(game, doPtr, 0x080 + y, value);
            RegObjTile(game, doPtr, 0x082 + y, value);
            RegObjTile(game, doPtr, 0x084 + y, value);
            RegObjTile(game, doPtr, 0x086 + y, value);

            y -= 0xF8;
            i--;
        }

        doPtr->mapAddr += 0x200;
        y = doPtr->mapAddr;

        doPtr->arg2--;
    }

    return;
}

void do_8BE0(zgPtr game, DngObj *doPtr)
{
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    do_B0AF(doPtr, 4);
    
    while(doPtr->arg1 > 0)
    {
        do_B2CE(game, doPtr);

        y += 0x80;
        doPtr->mapAddr = y;
       
        doPtr->arg1--;
    }

    return;
}

void do_8BF4(zgPtr game, DngObj *doPtr)
{
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    do_B0AF(doPtr, 4);
    
    doPtr->arg2++;

    while(doPtr->arg1 > 0)
    {
        do_B2D0(game, doPtr, doPtr->arg2);

        y += 0x80;
        doPtr->mapAddr = y;
        
        doPtr->arg2++;
        doPtr->arg1--;
    }

    return;
}

void do_8C0E(zgPtr game, DngObj *doPtr)
{
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    do_B0AF(doPtr, 4);

    while(doPtr->arg1 > 0)
    {
        do_B2CE(game, doPtr);

        y += 0x82;
        doPtr->mapAddr = y;

        doPtr->arg1--;
    }

    return;
}

void do_8C22(zgPtr game, DngObj *doPtr)
{ 
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    do_B0AF(doPtr, 4);

    while(doPtr->arg1 > 0)
    {
        do_B2CE(game, doPtr);

        y -= 0x7E;
        doPtr->mapAddr = y;

        doPtr->arg1--;
    }

    return;
}

void do_8C4F(zgPtr game, DngObj *doPtr)
{
    do_B0AC(doPtr);
    do_B220(game, doPtr, 0x300);

    return;
}

void do_8C58(zgPtr game, DngObj *doPtr)
{
    do_B0AF(doPtr, 7);
    do_B2AA(game, doPtr);

    return;
}

void do_8C61(zgPtr game, DngObj *doPtr)
{
    do_B0AF(doPtr, 7);
    do_B29C(game, doPtr);

    return;
}

void sh_8C6A(zgPtr game, DngObj *doPtr)
{
    do_B0AF(doPtr, 6);

    do_8C6A(game, doPtr, 0xFF82);
}

void do_8C6A(zgPtr game, DngObj *doPtr, u32 index)
{
    u32 x = doPtr->tileNum;
    u32 a = 0;
    u32 y = doPtr->mapAddr;
    u32 mapNumTemp = doPtr->mapNum;

    a = x; x = y; y = a;

    bufPtr BG_0 = game->BG_Map[0];
    bufPtr BG_1 = game->BG_Map[1];
    bufPtr rom = game->image;

    while( doPtr->arg1 > 0 )
    {
        doPtr->mapNum = 0;

        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));
        RegObjTile(game, doPtr, 0x200 + x, Get2Bytes(rom, 0x1B5A + y)); 
        
        doPtr->mapNum = 1;

        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));
        RegObjTile(game, doPtr, 0x200 + x, Get2Bytes(rom, 0x1B5A + y));

        // since this is 16 bit addition in the actual game
        // the AND operator is necessary
        x = (x + index) & 0xFFFF; 
        doPtr->arg1--;
    }

    doPtr->mapNum = mapNumTemp;

    return;
}

void sh_8CB9(zgPtr game, DngObj *doPtr)
{
    do_B0AF(doPtr, 6);

    do_8C6A(game, doPtr, 0x0082);

    return;
}

void do_8C73(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;

    do_B0AC(doPtr);

    while(doPtr->arg1 > 0)
    {
        do_97F0(game, doPtr, 2);

        doPtr->mapAddr += 8;

        doPtr->tileNum = x;
        doPtr->arg1--;
    }

    return;
}

void do_8CC7(zgPtr game, DngObj *doPtr)
{
    doPtr->arg1 += 0x04;

    u32 i = doPtr->arg1; // is $0A
    u32 j = 0; // is $0E
    u32 mem_04 = 0;
    u32 mem_06 = 0;
    u32 mem_08 = doPtr->mapAddr;
    u32 value = 0;
    doPtr->arg2++;

    do_8D47(game, doPtr, i);

    mem_06 = doPtr->tileNum;
    mem_04 = mem_08;
    mem_08 += 0x0180;
    j = doPtr->arg2;

    while(j > 0)
    {
        i = doPtr->arg1;

        doPtr->mapAddr = mem_08; doPtr->tileNum = mem_06;

        do_9BD9(game, doPtr);

        doPtr->tileNum += 0x0C; doPtr->mapAddr += 0x06;

        while(i > 0)
        {
            do_9895(game, doPtr);

            i--;
        }

        doPtr->tileNum += 0x08;

        do_9BD9(game, doPtr);

        mem_08 += 0x0100; j--;
    }

    doPtr->tileNum += 0x0C; doPtr->mapAddr = mem_08; i = doPtr->arg1;

    do_8D47(game, doPtr, i);

    value = 0xFF80;

    while(doPtr->arg2 > 0)
    {   
        value -= 0x80;
        value &= 0xFFFF;

        doPtr->arg2--;
    }

    value += mem_08;
    value &= 0xFFFF;

    doPtr->arg1 += 2; doPtr->arg1 <<= 1;
    value += doPtr->arg1;
    value &= 0xFFFF;

    doPtr->mapAddr = value; doPtr->tileNum = 0x0590;

    do_9895(game, doPtr);

    return;  
}

void do_8D47(zgPtr game, DngObj *doPtr, u32 index)
{
    // This routine is some what simplified from its ASM counter part
    // checks out

    u32 i = 2;

    do_8D80(game, doPtr, 3); // JSR $9216

    while(index > 0)
    {
        do_8D80(game, doPtr, 2);

        doPtr->tileNum -= 0x0C; index--;
    }

    doPtr->tileNum += 0x0C;

    do_8D80(game, doPtr, 3);

    return;
}

void do_8D5D(zgPtr game, DngObj *doPtr)
{
    doPtr->arg1 = ((doPtr->arg1 << 2) | doPtr->arg2) << 1;
    doPtr->arg1++;

    do_8D80(game, doPtr, 2);

    while(doPtr->arg1 > 0)
    {
        doPtr->tileNum -= 0x06;

        do_8D80(game, doPtr, 1);

        doPtr->arg1--;
    }

    do_8D80(game, doPtr, 1);

    return;
}

void do_8D80(zgPtr game, DngObj *doPtr, u32 index)
{
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];
    bufPtr rom = game->image;

    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    while( index > 0)
    {
        RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x)); 
        RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B54 + x));
        RegObjTile(game, doPtr, 0x100 + y, Get2Bytes(rom, 0x1B56 + x));

        x += 6;
        y += 2;
        index--;
    }
    
    doPtr->mapAddr = y;
    doPtr->tileNum = x;

    return;
}

void do_8D9E(zgPtr game, DngObj *doPtr)
{
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];
    bufPtr rom = game->image;

    u32 mem_0A = 0;
    u32 index = 0;
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;

    doPtr->arg1++; doPtr->arg2++;

    while(doPtr->arg2 > 0)
    {
        mem_0A = doPtr->arg1;

        while(mem_0A > 0)
        {
            index = Get2Bytes(rom, 0x1B52 + x);
            RegObjTile(game, doPtr, y + 0x000, index);
            RegObjTile(game, doPtr, y + 0x002, index);
            RegObjTile(game, doPtr, y + 0x004, index);
            RegObjTile(game, doPtr, y + 0x080, index);
            RegObjTile(game, doPtr, y + 0x082, index);
            RegObjTile(game, doPtr, y + 0x084, index);

            y += 0x100;

            RegObjTile(game, doPtr, y + 0x000, index);
            RegObjTile(game, doPtr, y + 0x002, index);
            RegObjTile(game, doPtr, y + 0x004, index);

            y -= 0x0FA;

            mem_0A--;
        }

        doPtr->mapAddr += 0x180;
        y = doPtr->mapAddr;

        doPtr->arg2--;
    }

    return;
}

void do_8DDC(zgPtr game, DngObj *doPtr)
{
    do_B0AF(doPtr, 4);
    doPtr->arg2 = doPtr->arg1; // To undo the stz $B4 effect of $B0AF
    
    u32 i = 0;
    u32 j = 0;
    u32 mem_0C = 0;
    u32 mem_08 = doPtr->mapAddr;
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;
    u32 pushY = y;
    u32 value = 0;
    bufPtr rom = game->image;

    i = doPtr->arg2;

    while(i > 0)
    {
        do_B2CE(game, doPtr);

        mem_0C = mem_08;
        mem_08 += 0x0080;
        doPtr->mapAddr = mem_08;

        i--;
    }

    doPtr->mapAddr = pushY;
    mem_08 = pushY;
    y = doPtr->mapAddr;

    i = 2;
    x = 0x063C;

    while(i > 0)
    {
        j = doPtr->arg1 - 2; // j is $0A

        RegObjTile(game, doPtr, y + 0x000, Get2Bytes(rom, 0x1B52 + x));

        value = Get2Bytes(rom, 0x1B54 + x);

        while(j > 0)
        {
            RegObjTile(game, doPtr, y + 0x002, value);

            y += 2;
            j--;
        }
        
        RegObjTile(game, doPtr, y + 0x002, Get2Bytes(rom, 0x1B56 + x));

        x += 0x0006;
        y = mem_0C;
        i--;
    }

    value = mem_08 + 0x0080;
    j = doPtr->arg1 - 1;
    doPtr->arg2 = j - 1;

    while(j > 0)
    {
        value += 2;
        j--;
    }

    mem_0C = value;

    i = 2; // i is $0E

    y = mem_08 + 0x0080;
    x = 0x0648;

    while(i > 0)
    {
        j = doPtr->arg2;

        while(j > 0)
        {
            RegObjTile(game, doPtr, y + 0x000, Get2Bytes(rom, 0x1B52 + x));

            y += 0x0080;    
            j--; // DEC $0A;
        }

        x += 0x02;
        y = mem_0C;
        i--; // DEC $0E;
    }

    return; 
}

void do_8E67(zgPtr game, DngObj *doPtr)
{
    do_B0AF(doPtr, 4);

    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    while(doPtr->arg1 > 0)
    {
        do_B2CE(game, doPtr);
        y += 0x0080;
        doPtr->mapAddr = y;

        doPtr->arg1--;
    }   
    
    return;
}

void do_8E7B(zgPtr game, DngObj *doPtr)
{
    do_B0AF(doPtr, 4);

    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    doPtr->arg2++;

    while(doPtr->arg1 > 0)
    {
        do_B2D0(game, doPtr, doPtr->arg2);

        y += 0x0080;
        doPtr->mapAddr = y;
        doPtr->arg2++;
        doPtr->arg1--;
    }  
    
    return;
}

void do_8E95(zgPtr game, DngObj *doPtr)
{
    do_B0AF(doPtr, 4);

    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    while(doPtr->arg1 > 0)
    {
        do_B2CE(game, doPtr);
        y += 0x0082;
        doPtr->mapAddr = y;

        doPtr->arg1--;
    }   

    return;
}

void do_8EA9(zgPtr game, DngObj *doPtr)
{
    do_B0AF(doPtr, 4);

    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    while(doPtr->arg1 > 0)
    {
        do_B2CE(game, doPtr);
        y -= 0x007E;
        doPtr->mapAddr = y;

        doPtr->arg1--;
    }   

    return;
}

void sh_8EBE(zgPtr game, DngObj *doPtr)
{
    do_B0AF(doPtr, 0x15);
    do_8EC3(game, doPtr);
}

void sh_8EC3(zgPtr game, DngObj *doPtr)
{
    do_B0AF(doPtr, 2);
    do_8EC3(game, doPtr);
}

void do_8EC3(zgPtr game, DngObj *doPtr)
{
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];
    bufPtr rom = game->image;

    u32 index = 0;
    u32 index2 = 0;
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;

    index = 0x00E3;
    index2 = Get2Bytes(BG_Map, y) & 0x03FF;

    if(index != index2)
        RegObjTile(game, doPtr, y, Get2Bytes(rom, 0x1B52 + x));

    while(doPtr->arg1 > 0)
    {
        y += 0x80;

        RegObjTile(game, doPtr, y, Get2Bytes(rom, 0x1B54 + x));
        
        doPtr->arg1--;
    }

    RegObjTile(game, doPtr, y + 0x80, Get2Bytes(rom, 0x1B56 + x));

    return;
}

void sh_8EEB(zgPtr game, DngObj *doPtr)
{
    do_B0AF(doPtr, 0x15);
    do_8EF0(game, doPtr);

    return;
}

void sh_8EF0(zgPtr game, DngObj *doPtr)
{
    do_B0AF(doPtr, 2);
    do_8EF0(game, doPtr);

    return;
}

void do_8EF0(zgPtr game, DngObj *doPtr)
{
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];
    bufPtr rom = game->image;

    u32 index = 0;
    u32 index2 = 0;
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;
    u32 mem_08 = doPtr->tileNum;
   
    index = 0x00E2;
    index2 = Get2Bytes(BG_Map, y) & 0x03FF;

    if(index != index2)
        RegObjTile(game, doPtr, y, Get2Bytes(rom, 0x1B52 + x));
   
    do_B2CA(game, doPtr);
    
    y = doPtr->mapAddr;
    x = doPtr->tileNum;

    RegObjTile(game, doPtr, y, Get2Bytes(rom, 0x1B54 + x));    

    return;
}

void do_8F0C(zgPtr game, DngObj *doPtr)
{
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];
    bufPtr rom = game->image;

    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    do_B0AC(doPtr);
    do_B2AF(game, doPtr);

    x += 8;

    y = doPtr->mapAddr;

    while(doPtr->arg1 > 0)
    {
        RegObjTile(game, doPtr, y + 0x000,  Get2Bytes(rom, 0x1B52 + x));
        RegObjTile(game, doPtr, y + 0x002,  Get2Bytes(rom, 0x1B54 + x));

        y += 0x80;
    
        doPtr->arg1--;
    }

    doPtr->tileNum = x + 4;
    doPtr->mapAddr = y;

    do_8F30(game, doPtr);

    return;
}

void do_8F30(zgPtr game, DngObj *doPtr)
{
    do_8D80(game, doPtr, 2);

    return;
}

void do_8F36(zgPtr game, DngObj *doPtr)
{
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    bufPtr rom = game->image;
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];

    do_B0AC(doPtr);

    doPtr->arg1++;

    do_8D80(game, doPtr, 2);

    x = doPtr->tileNum;
    y = doPtr->mapAddr;

    while(doPtr->arg1 > 0)
    {
        RegObjTile(game, doPtr, y + 0x000,  Get2Bytes(rom, 0x1B52 + x));
        RegObjTile(game, doPtr, y + 0x080, Get2Bytes(rom, 0x1B54 + x));
        RegObjTile(game, doPtr, y + 0x100, Get2Bytes(rom, 0x1B56 + x));

        y += 2;
        doPtr->arg1--;
    }

    doPtr->tileNum = x + 6;
    doPtr->mapAddr = y;

    do_8D80(game, doPtr, 2);

    return;
}

void do_8F62(zgPtr game, DngObj *doPtr)
{
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;
    u32 index = 0;

    bufPtr rom = game->image;
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];

    do_B0AC(doPtr);

    index = Get2Bytes(BG_Map, y) & 0x03FF;

    switch(index)
    {
    case 0x01DB:
    case 0x01A6:
    case 0x01DD:
    case 0x01FC:
        goto match;
            
        break;

    default: 
        
        index = Get2Bytes(rom, 0x1B52 + x);
        RegObjTile(game, doPtr, y + 0x000, index);

        break;
    }

match:

    do_B2CA(game, doPtr);

    x = doPtr->tileNum;
    y = doPtr->mapAddr;

    RegObjTile(game, doPtr, y + 0x000, Get2Bytes(rom, 0x1B54 + x));

    return;
}

void do_8F8A(zgPtr game, DngObj *doPtr)
{
    do_B0AC(doPtr);

    bufPtr rom = game->image;
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];

    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    while(doPtr->arg1 > 0)
    {
        RegObjTile(game, doPtr, y + 0x000, Get2Bytes(rom, 0x1B52 + x));

        y += 0x0080;
        doPtr->arg1--;
    }

    return;
}

void do_8F9D(zgPtr game, DngObj *doPtr)
{
    // Get the floor0 value
    doPtr->tileNum = game->dngObjs->floor[0];

    do_8FA5(game, doPtr);

    return;
}

void do_8FA2(zgPtr game, DngObj *doPtr)
{
    bufPtr obj = &game->currentFile->objectData[game->currentDungRoom];

    doPtr->tileNum = (GetByte(obj, 0) & 0x0F) << 4;

    do_8FA5(game, doPtr);

    return;
}

void do_8FA5(zgPtr game, DngObj *doPtr)
{
    bufPtr rom = game->image;
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];

    u32 y = doPtr->mapAddr;

    doPtr->arg1++; doPtr->arg2++;

    while(doPtr->arg2 > 0)
    {
        do_8A44(game, doPtr, doPtr->arg1);

        y += 0x200;
        doPtr->mapAddr = y;

        doPtr->arg2--;
    } 

    return;
}

void do_8FBD(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;
    u32 i = 0; // i is $0E
    u32 j = 0;
    bufPtr rom = game->image;
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];

    do_B0AF(doPtr, 0x0A);

    i = Get2Bytes(rom, 0x1B52 + x);
    x += 2; doPtr->tileNum = x; // synchronizing x and tileNum

    j = Get2Bytes(BG_Map, 0x000 + y) & 0x03FF;

    if(j != 0x00E2)
    {
        do_8FEC(game, doPtr, i);
        y = doPtr->mapAddr; x = doPtr->tileNum; // synchronize again
    }

    doPtr->tileNum += 4; x += 4;

    while(doPtr->arg1 > 0)
    {
        RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x));
        RegObjTile(game, doPtr, 0x080 + y, i);

        y += 2; doPtr->arg1--;
    }

    x += 2;
    doPtr->tileNum = x; doPtr->mapAddr = y;

    do_8FEC(game, doPtr, i);

    return;
}

void do_8FEC(zgPtr game, DngObj *doPtr, u32 index)
{
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;
    bufPtr rom = game->image;

    RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x));
    RegObjTile(game, doPtr, 0x002 + y, Get2Bytes(rom, 0x1B54 + x));

    RegObjTile(game, doPtr, 0x080 + y, index);
    RegObjTile(game, doPtr, 0x082 + y, index);

    y += 4;
    doPtr->mapAddr = y; doPtr->tileNum = x;

    return;
}

void do_9001(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;
    u32 i = 0; // i is $0E
    u32 j = 0;
    bufPtr rom = game->image;
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];

    do_B0AF(doPtr, 0x0A);

    i = Get2Bytes(rom, 0x1B52 + x);
    x += 2; doPtr->tileNum = x; // synchronizing x and tileNum

    j = Get2Bytes(BG_Map, 0x080 + y) & 0x03FF;

    if(j != 0x00E2)
    {
        do_9030(game, doPtr, i);
        y = doPtr->mapAddr; x = doPtr->tileNum; // synchronize again
    }

    doPtr->tileNum += 4; x += 4;

    while(doPtr->arg1 > 0)
    {
        RegObjTile(game, doPtr, 0x000 + y, i);
        RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B52 + x));

        y += 2; doPtr->arg1--;
    }

    x += 2;
    doPtr->tileNum = x; doPtr->mapAddr = y;

    do_9030(game, doPtr, i);

    return;
}

void do_9030(zgPtr game, DngObj *doPtr, u32 index)
{
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;
    bufPtr rom = game->image;

    RegObjTile(game, doPtr, 0x000 + y, index);
    RegObjTile(game, doPtr, 0x002 + y, index);

    RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B52 + x));
    RegObjTile(game, doPtr, 0x082 + y, Get2Bytes(rom, 0x1B54 + x));

    y += 4;

    doPtr->mapAddr = y; doPtr->tileNum = x;

    return;
}

void do_9045(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;
    u32 i = 0; // i is $0E
    u32 j = 0;
    bufPtr rom = game->image;
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];

    do_B0AF(doPtr, 0x0A);

    i = Get2Bytes(rom, 0x1B52 + x);
    x += 2; doPtr->tileNum = x; // synchronizing x and tileNum

    j = Get2Bytes(BG_Map, 0x000 + y) & 0x03FF;

    if(j != 0x00E3)
    {
        do_9078(game, doPtr, i);
        y = doPtr->mapAddr; x = doPtr->tileNum; // synchronize again
    }

    doPtr->tileNum += 4; x += 4;

    while(doPtr->arg1 > 0)
    {   
        RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x));
        RegObjTile(game, doPtr, 0x002 + y, i);

        y += 0x0080; doPtr->arg1--;
    }

    x += 2;
    doPtr->tileNum = x; doPtr->mapAddr = y;

    do_9078(game, doPtr, i);
 
    return;
}

void do_9078(zgPtr game, DngObj *doPtr, u32 index)
{
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;
    bufPtr rom = game->image;

    RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x));
    RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B54 + x));

    RegObjTile(game, doPtr, 0x002 + y, index);
    RegObjTile(game, doPtr, 0x082 + y, index);

    y += 0x100;

    doPtr->mapAddr = y; doPtr->tileNum = x;

    return;
}

void do_908F(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;
    u32 i = 0; // i is $0E
    u32 j = 0;
    bufPtr rom = game->image;
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];

    do_B0AF(doPtr, 0x0A);

    i = Get2Bytes(rom, 0x1B52 + x);
    x += 2; doPtr->tileNum = x; // synchronizing x and tileNum

    j = Get2Bytes(BG_Map, 0x002 + y) & 0x03FF;

    if(j != 0x00E3)
    {
        do_90C2(game, doPtr, i);
        y = doPtr->mapAddr; x = doPtr->tileNum; // synchronize again
    }

    doPtr->tileNum += 4; x += 4;

    while(doPtr->arg1 > 0)
    {   
        RegObjTile(game, doPtr, 0x000 + y, i);
        RegObjTile(game, doPtr, 0x002 + y, Get2Bytes(rom, 0x1B52 + x));

        y += 0x0080; doPtr->arg1--;
    }

    x += 2;
    doPtr->tileNum = x; doPtr->mapAddr = y;

    do_90C2(game, doPtr, i);
 
    return;
}

void do_90C2(zgPtr game, DngObj *doPtr, u32 index)
{
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;
    bufPtr rom = game->image;
    
    RegObjTile(game, doPtr, 0x000 + y, index);
    RegObjTile(game, doPtr, 0x080 + y, index);

    RegObjTile(game, doPtr, 0x002 + y, Get2Bytes(rom, 0x1B52 + x));
    RegObjTile(game, doPtr, 0x082 + y, Get2Bytes(rom, 0x1B54 + x));

    y += 0x100;

    doPtr->mapAddr = y; doPtr->tileNum = x;

    return;
}

void do_90D9(zgPtr game, DngObj *doPtr)
{
    do_B0AF(doPtr, 0x08);
    do_B2CE(game, doPtr);

    return;
}


void do_90E2(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;
    bufPtr rom = game->image;

    do_B0AF(doPtr, 0x08);

    while(doPtr->arg1 > 0)
    {
        RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x));

        y += 0x0080; doPtr->arg1--;
    }

    return;
}

void do_90F9(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;

    do_B0AC(doPtr);

    while(doPtr->arg1 > 0)
    {
        doPtr->tileNum = x;

        do_97ED(game, doPtr);

        y += 0x200;
        doPtr->mapAddr = y;

        doPtr->arg1--;
    }

    return;
}

void do_9111(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;

    do_B0AC(doPtr);

    while(doPtr->arg1 > 0)
    {   
        doPtr->tileNum = x;

        do_97ED(game, doPtr);

        doPtr->arg1--;
    }
}

void do_9120(zgPtr game, DngObj* doPtr)
{
    do_B0AF(doPtr, 4);

    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;
    bufPtr rom = game->image;
    
    while(doPtr->arg1 > 0)
    {
        RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x));

        y += 0x80;

        doPtr->arg1--;
    }

    return;
}

void do_9136(zgPtr game, DngObj* doPtr)
{
    do_B0AF(doPtr, 4);
    do_B2CE(game, doPtr);

    return;
}

void do_9298(zgPtr game, DngObj *doPtr)
{
    

    return;
}

void do_92FB(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;

    do_B0AC(doPtr);

    while(doPtr->arg1 > 0)
    {
        doPtr->tileNum = x;
        
        do_97ED(game, doPtr);

        doPtr->mapAddr += 0x0004;
        doPtr->arg1--;
    }
    return;
}

void do_930E(zgPtr game, DngObj *doPtr)
{
    do_B0AC(doPtr);

    u32 x = doPtr->tileNum;

    while(doPtr->arg1 > 0)
    {
        doPtr->tileNum = x;

        do_97ED(game, doPtr);

        doPtr->mapAddr += 0x2F8;
        doPtr->arg1--;
    }

    return;

}

void do_9323(zgPtr game, DngObj *doPtr)
{    
    do_B0AC(doPtr);

    while(doPtr->arg1 > 0)
    {
        doPtr->tileNum = 0x0E26;

        do_8D80(game, doPtr, 2);

        doPtr->mapAddr += 4;
        doPtr->arg1--;
    }

    return;
}

void do_9338(zgPtr game, DngObj *doPtr)
{
    do_B0AC(doPtr);

    while(doPtr->arg1 > 0)
    {
        do_9895(game, doPtr);

        doPtr->mapAddr += 4;
        doPtr->arg1--;
    }

    return;
}

void do_9347(zgPtr game, DngObj *doPtr)
{   
    u32 y = doPtr->mapAddr;

    do_B0AC(doPtr);

    while(doPtr->arg1 > 0)
    {
        do_B2AF(game, doPtr);

        doPtr->mapAddr += 0x100;
        doPtr->arg1--;
    }

    return;
}

void do_9357(zgPtr game, DngObj *doPtr)
{
    do_B0AC(doPtr);

    u32 mem_0C = doPtr->tileNum;

    while(doPtr->arg1 > 0)
    {
        doPtr->tileNum = mem_0C;

        do_97F0(game, doPtr, 2);

        doPtr->mapAddr += 0x2FC;

        doPtr->arg1--;
    }

    return;
}

void do_936F(zgPtr game, DngObj *doPtr)
{
    do_B0AC(doPtr);

    u32 mem_0C = doPtr->tileNum;
    u32 i = doPtr->arg1;
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;

    while(i > 0)
    {
        doPtr->tileNum = mem_0C;
        do_97F0(game, doPtr, 2);
        
        doPtr->mapAddr += 0x08;
        i--;
    }

    doPtr->tileNum = x;
    doPtr->mapAddr = y;

    return;
}

void do_9387(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;
    u32 index = 0;
        
    do_B0AC(doPtr);

    while(doPtr->arg1 > 0)
    {
        doPtr->tileNum = x;
        index = 4;

        do_8D80(game, doPtr, index);

        doPtr->mapAddr += 8;
        doPtr->arg1--;
    }

    return;
}

void do_939F(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;

    do_B0AC(doPtr);
    
    while(doPtr->arg1 > 0)
    {
        doPtr->tileNum = x;

        do_97F0(game, doPtr, 3);

        doPtr->mapAddr += 0x03FA;
        doPtr->arg1--;
    }

    return;
}

void do_93B7(zgPtr game, DngObj *doPtr)
{
    u32 mem_08 = doPtr->mapAddr;
    do_B0AC(doPtr);

    while(doPtr->arg1 > 0)
    {
        doPtr->tileNum = 0x08CA;
        do_B2AF(game, doPtr);

        doPtr->mapAddr += 0x200;
        doPtr->tileNum += 0x08;

        do_B2AF(game, doPtr);

        mem_08 += 0x08; doPtr->mapAddr = mem_08;
        doPtr->arg1--;
    }

    return;
}

void do_93DC(zgPtr game, DngObj *doPtr)
{
    u32 mem_08 = doPtr->mapAddr;

    doPtr->arg1++;
    doPtr->arg2 = (doPtr->arg2 << 1) + 1;

    do_93FF(game, doPtr);
    mem_08 += 0x80;
    doPtr->mapAddr = mem_08;

    doPtr->tileNum += 0x08;

    while(doPtr->arg2 > 0)
    {
        do_93FF(game, doPtr);

        mem_08 += 0x80;
        doPtr->mapAddr = mem_08;
        doPtr->arg2--;
    }

    doPtr->tileNum += 0x08;
    do_93FF(game, doPtr);
    mem_08 += 0x80;
    doPtr->mapAddr = mem_08;

    doPtr->tileNum += 0x08;
    do_93FF(game, doPtr);
    mem_08 += 0x80;
    doPtr->mapAddr = mem_08;

    return;
}


void do_93FF(zgPtr game, DngObj *doPtr)
{
    u32 i = doPtr->arg1;
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;

    bufPtr rom = game->image;

    RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x));

    while(i > 0)
    {
        RegObjTile(game, doPtr, 0x002 + y, Get2Bytes(rom, 0x1B54 + x));
        RegObjTile(game, doPtr, 0x004 + y, Get2Bytes(rom, 0x1B56 + x));

        y += 4; i--;
    }

    RegObjTile(game, doPtr, 0x002 + y, Get2Bytes(rom, 0x1B58 + x));

    doPtr->tileNum = x;
    doPtr->mapAddr = y;

    return;
}

void do_9429(zgPtr game, DngObj *doPtr)
{
    u32 i = 0;
    u32 y = doPtr->mapAddr;

    doPtr->arg1++; doPtr->arg2++;

    while(doPtr->arg2 > 0)
    {
        i = doPtr->arg1;

        while(i > 0)
        {
            do_9895(game, doPtr);

            i--;
        }

        y += 0x0100;
        doPtr->mapAddr = y;
        doPtr->arg2--;
    }

    return;
}

void do_9446(zgPtr game, DngObj *doPtr)
{
    do_B0AC(doPtr);

    while(doPtr->arg1 > 0)
    {
        do_B2AF(game, doPtr);

        doPtr->mapAddr += 0x0600; doPtr->arg1--;
    }

    return;
}

void do_9456(zgPtr game, DngObj *doPtr)
{
    do_B0AC(doPtr);

    while(doPtr->arg1 > 0)
    {
        do_B2AF(game, doPtr);
 
        doPtr->mapAddr += 0xFF1C;
        doPtr->mapAddr &= 0xFFFF;
        doPtr->arg1--;
    }    

    return;
}

void do_9466(zgPtr game, DngObj *doPtr)
{
    do_B0AC(doPtr);

    doPtr->arg1 <<= 1;

    do_B279(game, doPtr);

    doPtr->tileNum += 0x0A; doPtr->mapAddr += 2;

    while(doPtr->arg1 > 0)
    {
        do_B279(game, doPtr);

        doPtr->mapAddr += 2; doPtr->arg1--;
    }

    doPtr->tileNum += 0x0A;

    do_B279(game, doPtr);

    return;
}

void do_9488(zgPtr game, DngObj *doPtr)
{
    u32 x = 0;
    u32 y = 0;
    u32 i = 0;

    bufPtr rom = game->image;
    
    do_B0AC(doPtr);

    i = doPtr->arg1 << 1;

    do_8D80(game, doPtr, 1);

    x = doPtr->tileNum; y = doPtr->mapAddr;

    while(i > 0)
    {
        RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x));        
        RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B54 + x));
        RegObjTile(game, doPtr, 0x100 + y, Get2Bytes(rom, 0x1B56 + x));
        
        y += 2; i--;
    }
    
    doPtr->tileNum = x + 0x06; doPtr->mapAddr = y;

    do_8D80(game, doPtr, 1);

    return;
}

void do_94B4(zgPtr game, DngObj *doPtr)
{
    do_B0AC(doPtr);
    do_B220(game, doPtr, 0x08);

    return;
}

void do_94BD(zgPtr game, DngObj *doPtr)
{
    do_B0AC(doPtr);

    u32 i = doPtr->arg1 << 1;

    do_B2F6(game, doPtr);

    doPtr->mapAddr += 2; doPtr->tileNum += 0x06;

    while(i > 0)
    {
        do_B2F6(game, doPtr);

        doPtr->mapAddr += 2; i--;
    }

    doPtr->tileNum += 6;

    do_B2F6(game, doPtr);

    return;
}

void do_94DF(zgPtr game, DngObj *doPtr)
{
    do_B0AC(doPtr);
    do_97F0(game, doPtr, 1);

    u32 i = doPtr->arg1;

    while(i > 0)
    {   
        do_97F0(game, doPtr, 2);
        
        doPtr->tileNum -= 0x10; i--;
    }

    doPtr->tileNum += 0x10;

    do_B2E1(game, doPtr);

    return;
}

void do_9501(zgPtr game, DngObj *doPtr)
{


    return;
}

void do_95EF(zgPtr game, DngObj *doPtr)
{


    return;
}

void do_96DC(zgPtr game, DngObj *doPtr)
{
    //!

    do_B0AC(doPtr);
    doPtr->arg1++;
    do_B2CE(game, doPtr);

    return;
}

void do_96E4(zgPtr game, DngObj *doPtr)
{   
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;
    u32 i = 0;

    bufPtr rom = game->image;

    do_B0AC(doPtr);
    i = doPtr->arg1 + 1;

    while(i > 0)
    {
        RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x));    

        y += 0x80; i--;
    }

    return;
}

void do_96F9(zgPtr game, DngObj *doPtr)
{
    u32 index = 0x0018;

    do_B0AC(doPtr);
    do_B220(game, doPtr, index);

    return;
}

void do_9702(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;
    u32 i = 0;

    do_B0AC(doPtr);

    i = doPtr->arg1;

    while(i > 0)
    {   
        doPtr->tileNum = x;

        do_97F0(game, doPtr, 2);

        doPtr->mapAddr += 0x05FC;
        i--;    
    }

    return;
}

void do_971B(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;
    u32 i = doPtr->arg1;

    do_B0AC(doPtr);

    while(i > 0)
    {
        doPtr->tileNum = x;

        do_97F0(game, doPtr, 3);

        doPtr->mapAddr += 0x2FA; i--;
    }

    return;
}

void do_9733(zgPtr game, DngObj *doPtr)
{
    //!

    doPtr->tileNum = doPtr->mapAddr;
    doPtr->mapAddr = 0x0AB4;
    
    doPtr->arg1++;
    doPtr->arg2 = (doPtr->arg2 << 1) + 5;

    while(doPtr->arg2 > 0)
    {
        do_975C(game, doPtr);

        doPtr->arg2--;
    }

    doPtr->mapAddr += 2;

    do_975C(game, doPtr);

    doPtr->mapAddr += 2;

    do_975C(game, doPtr);

    return;
}

void do_975C(zgPtr game, DngObj *doPtr)
{
    u32 tempX = doPtr->tileNum;
    u32 mem_0A = doPtr->arg1;

    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;
    u32 value = 0;
    bufPtr rom = game->image;

    RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y));

    value = Get2Bytes(rom, 0x1B58 + y);

    while(mem_0A > 0)
    {
        RegObjTile(game, doPtr, 0x002 + x, value);

        x += 2;
        mem_0A--;
    }

    RegObjTile(game, doPtr, 0x002 + x, Get2Bytes(rom, 0x1B5E + y));

    value = Get2Bytes(rom, 0x1B64 + y);
    RegObjTile(game, doPtr, 0x004 + x, value);
    RegObjTile(game, doPtr, 0x006 + x, value);
    RegObjTile(game, doPtr, 0x008 + x, value);
    RegObjTile(game, doPtr, 0x00A + x, value);

    mem_0A = doPtr->arg1;

    RegObjTile(game, doPtr, 0x00C + x, Get2Bytes(rom, 0x1B6A + y));

    value = Get2Bytes(rom, 0x1B70 + y);

    while(mem_0A > 0)
    {
        RegObjTile(game, doPtr, 0x00E + x, value);

        x += 2;
        mem_0A--;
    }

    RegObjTile(game, doPtr, 0x00E + x, Get2Bytes(rom, 0x1B76 + y));

    doPtr->tileNum = (tempX + 0x80);

    return;
}

void do_97B5(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;
    u32 i = 0;

    bufPtr rom = game->image;

    do_B0AF(doPtr, 0x02);

    i = doPtr->arg1 << 1;

    RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x));
    RegObjTile(game, doPtr, 0x002 + y, Get2Bytes(rom, 0x1B54 + x));

    while(i > 0)
    {
        RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B56 + x));
        RegObjTile(game, doPtr, 0x082 + y, Get2Bytes(rom, 0x1B58 + x));

        y += 0x80; i--;
    }

    return;
}

void do_97DC(zgPtr game, DngObj *doPtr)
{
    u32 i = 0;

    do_B0AC(doPtr);

    i = doPtr->arg1;

    while(i > 0)
    {
        doPtr->tileNum = 0x0B16;
        
        do_97F0(game, doPtr, 0x02);
        
        i--;
    }

    return;
}

void sh_97ED(zgPtr game, DngObj *doPtr)
{
    // the sh stands for "shell" b/c if I didn't do this I would break the object
    // framework. It's a little bit of a hack, i must admit.

    do_97ED(game, doPtr);

    return;
}

void do_97ED(zgPtr game, DngObj *doPtr)
{
    do_97F0(game, doPtr, 0x0004);

    return;
}

void do_97F0(zgPtr game, DngObj *doPtr, u32 index)
{
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];
    bufPtr rom = game->image;

    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    while( index > 0)
    {
        RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x)); 
        RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B54 + x));
        RegObjTile(game, doPtr, 0x100 + y, Get2Bytes(rom, 0x1B56 + x));
        RegObjTile(game, doPtr, 0x180 + y, Get2Bytes(rom, 0x1B58 + x));

        x += 8;
        y += 2;
        index--;
    }
    
    doPtr->tileNum = x;
    doPtr->mapAddr = y;

    return;
}

void do_9813(zgPtr game, DngObj *doPtr)
{
    bufPtr rom = game->image;

    u32 a = 0;
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;
    u32 index = 0;
    u32 mapNumTemp = doPtr->mapNum;

    a = x;
    x = y;
    y = a;

    index = 4;

    while(index > 0)
    {
        doPtr->mapNum = 0;

        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));

        doPtr->mapNum = 1;
      
        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));

        y += 8;
        x += 2;
        
        index--;
    }

    doPtr->mapNum = mapNumTemp;

    return; 
}

void do_9854(zgPtr game, DngObj *doPtr)
{
    bufPtr BG_0 = game->BG_Map[0];
    bufPtr BG_1 = game->BG_Map[1];
    bufPtr rom = game->image;

    u32 a = 0;
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;
    u32 index = 0;
    u32 mapNumTemp = doPtr->mapNum;

    a = x;
    x = y;
    y = a;

    index = 3;

    while(index > 0)
    {
        doPtr->mapNum = 0;

        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));
      
        doPtr->mapNum = 1;

        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));

        y += 8;
        x += 2;
        
        index--;
    }

    doPtr->mapNum = mapNumTemp;

    return;
}

void do_985C(zgPtr game, DngObj *doPtr)
{
    u32 a = doPtr->tileNum;
    u32 x = doPtr->mapAddr;
    u32 y = a;
    u32 i = 0x04;
    bufPtr rom = game->image;

    DngObj swap = *doPtr;
    swap.mapNum = 1;
    doPtr->mapNum = 0;

    while( i > 0 )
    {
        a = Get2Bytes(rom, 0x1B52 + y);
        RegObjTile(game, doPtr, 0x000 + x, a); 
        RegObjTile(game, &swap, 0x000 + x, a);

        a = Get2Bytes(rom, 0x1B54 + y);
        RegObjTile(game, doPtr, 0x080 + x, a); 
        RegObjTile(game, &swap, 0x080 + x, a);

        a = Get2Bytes(rom, 0x1B56 + y);
        RegObjTile(game, doPtr, 0x100 + x, a); 
        RegObjTile(game, &swap, 0x100 + x, a);
        
        y += 0x06; x += 0x02; i--;
    }
    
    return;
}

void do_9892(zgPtr game, DngObj *doPtr)
{
    // inc $045A <- not sure how this is relevant yet

    sh_9895(game, doPtr);

    return;
}

void sh_9895(zgPtr game, DngObj *doPtr)
{
    do_9895(game, doPtr);

    return;
}


void do_9895(zgPtr game, DngObj *doPtr)
{
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];
    bufPtr rom = game->image;

    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x)); 
    RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B54 + x));
    RegObjTile(game, doPtr, 0x002 + y, Get2Bytes(rom, 0x1B56 + x));
    RegObjTile(game, doPtr, 0x082 + y, Get2Bytes(rom, 0x1B58 + x));

    y += 4;
    doPtr->mapAddr = y;

    return;
}

void do_98AE(zgPtr game, DngObj *doPtr)
{
    u32 *n = &(game->dngObjs->numBigKeyLocks);

    if(*n >= 0x06)
        return;

    game->dngObjs->chestLocs[*n] = doPtr->mapAddr;

    (*n)++;
    
    doPtr->tileNum = 0x1494;
    
    do_9895(game, doPtr);

    return;
}

void do_98D0(zgPtr game, DngObj *doPtr)
{
    DngObjStruct *doStruct = game->dngObjs;

    u32 opened = 0;
    u32 *n = &(doStruct->numChests);
    u32 *m = &(doStruct->numBigKeyLocks);

    if(*n >= 0x06 || *m >= 0x06)
        return;

    doPtr->tileNum = 0x149C;
    
    if(opened)
        doPtr->tileNum = 0x14A4;

    doStruct->chestLocs[*n] = doPtr->mapAddr;

    if(doPtr->mapNum == 0)
        doStruct->chestLocs[*n] |= 0x2000;

    (*n)++;
    (*m)++;

    do_9895(game, doPtr);    

    return;
}

void do_99BB(zgPtr game, DngObj *doPtr)
{
    // Big chest object
    // Type 1 Subtype 3

    u32 *n = &(game->dngObjs->numChests);
    u32 *m = &(game->dngObjs->numBigKeyLocks);

    if(*n >= 0x06 || *m >= 0x06)
        return;

    doPtr->tileNum = 0x14AC;
    doPtr->arg2 = 0x0004;

    game->dngObjs->chestLocs[*n] = doPtr->mapAddr | 0x8000;

    if(doPtr->mapNum == 0)
        game->dngObjs->chestLocs[*n] |= 0xA000;

    (*n)++;
    (*m)++;

    do_99E6(game, doPtr);

    return;
}

void do_99E6(zgPtr game, DngObj *doPtr)
{
    do_8D80(game, doPtr, 0x04);
}

void do_99EC(zgPtr game, DngObj *doPtr)
{ 
    do_97F0(game, doPtr, 0x03);

    return;
}

void do_9A00(zgPtr game, DngObj *doPtr)
{ 
    do_8D80(game, doPtr, 0x04);

    return;
}

void do_9A06(zgPtr game, DngObj *doPtr)
{
    do_8D80(game, doPtr, 0x0008);

    return;
}

void do_9A0C(zgPtr game, DngObj *doPtr)
{
    do_8D80(game, doPtr, 0x0006);

    return;
}

void do_9A12(zgPtr game, DngObj *doPtr)
{
    u32 i = 8;
    u32 x = doPtr->mapAddr;
    u32 y = doPtr->tileNum; // not a mistake

    bufPtr rom = game->image;

    while( i > 0 )
    {
        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y)); 
        RegObjTile(game, doPtr, 0x100 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y)); 
        RegObjTile(game, doPtr, 0x200 + x, Get2Bytes(rom, 0x1B5A + y)); 
        RegObjTile(game, doPtr, 0x280 + x, Get2Bytes(rom, 0x1B5C + y));
        RegObjTile(game, doPtr, 0x300 + x, Get2Bytes(rom, 0x1B5E + y)); 

        y += 0x0E; x += 0x02; i--;
    }

    return;
}

void do_9A66(zgPtr game, DngObj *doPtr)
{
    doPtr->tileNum = doPtr->mapAddr;
    doPtr->mapAddr = 0x1F92;

    do_9DA2(game, doPtr, 0x06);

    return;
}

void do_9A6F(zgPtr game, DngObj *doPtr)
{
    game->dngObjs->numStars++;    

    do_B2AF(game, doPtr);

    return;
}

void do_9A90(zgPtr game, DngObj *doPtr)
{
    u32 mem_08 = doPtr->mapAddr;

    do_8D80(game, doPtr, 0x04);
    
    doPtr->mapAddr = mem_08 + 0x0180;

    do_8D80(game, doPtr, 0x04);

    return;
}

void do_9AA3(zgPtr game, DngObj *doPtr)
{
    do_97F0(game, doPtr, 0x06);

    return;
}

void do_9AA9(zgPtr game, DngObj *doPtr)
{
    u32 i = 3;
    u32 y = 0x1DD6;
    u32 x = doPtr->mapAddr;
    u32 index = 0;

    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];
    bufPtr rom = game->image;

    while(i > 0)
    {
        index = Get2Bytes(rom, 0x1B52 + y);
        RegObjTile(game, doPtr, 0x000 + x, index); 
        RegObjTile(game, doPtr, 0x180 + x, index); 
        RegObjTile(game, doPtr, 0x300 + x, index);

        index = Get2Bytes(rom, 0x1B54 + y);
        RegObjTile(game, doPtr, 0x080 + x, index); 
        RegObjTile(game, doPtr, 0x200 + x, index); 
        RegObjTile(game, doPtr, 0x380 + x, index); 

        x += 4;
        i--;
    }

    return;
}

void do_9AEE(zgPtr game, DngObj *doPtr)
{
    u32 index = 5;
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    bufPtr rom = game->image;
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];

    while(index > 0)
    {
        RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x)); 
        RegObjTile(game, doPtr, 0x002 + y, Get2Bytes(rom, 0x1B54 + x));
        RegObjTile(game, doPtr, 0x004 + y, Get2Bytes(rom, 0x1B56 + x));
        RegObjTile(game, doPtr, 0x006 + y, Get2Bytes(rom, 0x1B58 + x));

        x += 0x08;
        y += 0x80;
        index--;
    }

    return;
}

void do_9B1E(zgPtr game, DngObj *doPtr)
{
    if(game->bm_dung_header.tag_2 != 0x001B)
        goto water_twin;

    do_A3AE(game, doPtr);

    return;

water_twin:

    // some other shit

    do_9B48(game, doPtr);

    return;
}

void do_9B48(zgPtr game, DngObj *doPtr)
{
    //!

    doPtr->arg1 = 0x01;    
    do_B220(game, doPtr, doPtr->arg1);

    return;
}

void do_9B50(zgPtr game, DngObj *doPtr)
{
    do_8D80(game, doPtr, 0x06);

    return;
}

void do_9B56(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->mapAddr;
    u32 y = doPtr->tileNum; // note x and y criss cross in this routine
    u32 i = 6;
    u32 value = 0;
    bufPtr rom = game->image;

    while(i > 0)
    {
        value = Get2Bytes(rom, 0x1B52 + y);
        RegObjTile(game, doPtr, 0x000 + x, value); 
        RegObjTile(game, doPtr, 0x008 + x, value);
        RegObjTile(game, doPtr, 0x010 + x, value);  
        RegObjTile(game, doPtr, 0x01C + x, value); 
        RegObjTile(game, doPtr, 0x024 + x, value);  
        RegObjTile(game, doPtr, 0x02C + x, value);  

        value |= 0x4000;     
        RegObjTile(game, doPtr, 0x002 + x, value);
        RegObjTile(game, doPtr, 0x00A + x, value);
        RegObjTile(game, doPtr, 0x012 + x, value);
        RegObjTile(game, doPtr, 0x01E + x, value);
        RegObjTile(game, doPtr, 0x026 + x, value);
        RegObjTile(game, doPtr, 0x02E + x, value);
        
        value = Get2Bytes(rom, 0x1B5E + y);
        RegObjTile(game, doPtr, 0x004 + x, value);
        RegObjTile(game, doPtr, 0x00C + x, value);
        RegObjTile(game, doPtr, 0x020 + x, value);
        RegObjTile(game, doPtr, 0x028 + x, value);
                                          
        value |= 0x4000;                  
        RegObjTile(game, doPtr, 0x006 + x, value); 
        RegObjTile(game, doPtr, 0x00E + x, value); 
        RegObjTile(game, doPtr, 0x022 + x, value);
        RegObjTile(game, doPtr, 0x02A + x, value);
             
        y += 2;
        x += 0x0080;                     
        i--;                             
    }

    doPtr->tileNum = y + 0x000C;
    doPtr->mapAddr += 0x0014;

    do_8D80(game, doPtr, 4);

    return;
}

void do_9BD9(zgPtr game, DngObj *doPtr)
{
    // checks out

    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;
    bufPtr rom = game->image;

    RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x)); 
    RegObjTile(game, doPtr, 0x002 + y, Get2Bytes(rom, 0x1B54 + x));
    RegObjTile(game, doPtr, 0x004 + y, Get2Bytes(rom, 0x1B56 + x));
    RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B58 + x)); 
    RegObjTile(game, doPtr, 0x082 + y, Get2Bytes(rom, 0x1B5A + x));
    RegObjTile(game, doPtr, 0x084 + y, Get2Bytes(rom, 0x1B5C + x));

    return;
}

void do_9BF8(zgPtr game, DngObj *doPtr)
{
    

    // if some shit, watergate is open
    do_97F0(game, doPtr, 0x0A);

    // if( some other shit)
    // doPtr->tileNum = 0x13E8;
    // do_97F0(game, doPtr, 0x0A);

}

void do_9C3B(zgPtr game, DngObj *doPtr)
{
    // INC $03F4, but not sure if it's relevant to this program

    do_9C3E(game, doPtr);

    return;
}

void do_9C3E(zgPtr game, DngObj *doPtr)
{
    RegObjTile(game, 
               doPtr, 
               doPtr->mapAddr, 
               Get2Bytes(game->image, 0x1B52 + doPtr->tileNum) );

    return;
}

void do_9C44(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->mapAddr;
    u32 y = 0x1488;
    u32 pushX = x;
    u32 i = 5;
    u32 value = 0;
    bufPtr rom = game->image;

    while(i > 0)
    {
        value = Get2Bytes(rom, 0x1B54 + y);
        RegObjTile(game, doPtr, 0x004 + x, value);
        RegObjTile(game, doPtr, 0x012 + x, value);

        value = Get2Bytes(rom, 0x1B56 + y);
        RegObjTile(game, doPtr, 0x084 + x, value);
        value |= 0x4000;
        RegObjTile(game, doPtr, 0x092 + x, value);

        value = Get2Bytes(rom, 0x1B5A + y);
        RegObjTile(game, doPtr, 0x104 + x, value);
        value |= 0x4000;
        RegObjTile(game, doPtr, 0x112 + x, value);

        value = Get2Bytes(rom, 0x1B5C + y);
        RegObjTile(game, doPtr, 0x184 + x, value);
        value |= 0x4000;
        RegObjTile(game, doPtr, 0x192 + x, value);

        x += 2; i--;
    }

    x = pushX;

    value = Get2Bytes(rom, 0x1B52 + y);
    RegObjTile(game, doPtr, 0x000 + x, value);
    value |= 0x4000;
    RegObjTile(game, doPtr, 0x01E + x, value);

    value = Get2Bytes(rom, 0x1B54 + y);
    RegObjTile(game, doPtr, 0x002 + x, value);
    RegObjTile(game, doPtr, 0x00E + x, value);
    RegObjTile(game, doPtr, 0x010 + x, value);
    RegObjTile(game, doPtr, 0x01C + x, value);

    value = Get2Bytes(rom, 0x1B58 + y);
    RegObjTile(game, doPtr, 0x102 + x, value);
    value |= 0x4000;
    RegObjTile(game, doPtr, 0x11C + x, value);


    return;
}

void do_9CC6(zgPtr game, DngObj *doPtr)
{
    u32 pushX = 0;
    u32 i = 0;

    do_B0AC(doPtr);
    do_8D80(game, doPtr, 0x02);

    i = doPtr->arg1 - 1; // caution... underflow for 32-bit ints is different than shorts

    if(i)
    {
        while(i > 0)
        {
            pushX = doPtr->tileNum;

            do_8D80(game, doPtr, 0x02);

            doPtr->tileNum = pushX; i--;
        }
    }

    doPtr->tileNum += 0x0C;
    
    do_8D80(game, doPtr, 0x02);

    return;
}

void do_9CEB(zgPtr game, DngObj *doPtr)
{
    u32 pushX = 0;
    u32 i = 0;

    do_B0AC(doPtr);
    do_9D04(game, doPtr);

    i = doPtr->arg1 - 1;

    if(i)
    {
        while(i > 0)
        {
            pushX = doPtr->tileNum;
            
            do_9D04(game, doPtr);

            doPtr->tileNum = pushX; i--;
        }
    }

    doPtr->tileNum += 0x000C;

    do_9D04(game, doPtr);

    return;
}

void do_9D04(zgPtr game, DngObj *doPtr)
{
    u32 i = 2;
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;
    bufPtr rom = game->image;
    
    while(i > 0)
    {   
        RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x)); 
        RegObjTile(game, doPtr, 0x002 + y, Get2Bytes(rom, 0x1B54 + x));
        RegObjTile(game, doPtr, 0x004 + y, Get2Bytes(rom, 0x1B56 + x));

        x += 6;y += 0x80; i--;
    }

    doPtr->tileNum = x; doPtr->mapAddr = y;

    return;
}

void do_9D29(zgPtr game, DngObj *doPtr)
{
    do_9D6F(game, doPtr, 0x03);

    return;
}

void do_9D5D(zgPtr game, DngObj *doPtr)
{  
    doPtr->tileNum = 0x162C;
    do_9D6F(game, doPtr, 0x05);
    
    return;
}

void do_9D67(zgPtr game, DngObj *doPtr)
{
    do_9D6F(game, doPtr, 0x07);

    return;
}

void do_9D6C(zgPtr game, DngObj *doPtr)
{
    do_9D6F(game, doPtr, 0x02);

    return;
}

void do_9D6F(zgPtr game, DngObj *doPtr, u32 index)
{
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;

    bufPtr rom = game->image;
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];

    while( index > 0)
    {
        RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x)); 
        RegObjTile(game, doPtr, 0x002 + y, Get2Bytes(rom, 0x1B54 + x));
        RegObjTile(game, doPtr, 0x004 + y, Get2Bytes(rom, 0x1B56 + x));
        RegObjTile(game, doPtr, 0x006 + y, Get2Bytes(rom, 0x1B58 + x));

        x += 0x08;
        y += 0x80;
        index--;
    }   

    doPtr->tileNum = x;
    doPtr->mapAddr = y;

    return;
}

void do_9D96(zgPtr game, DngObj *doPtr)
{
    // check if Trinexx is defeated
    // if defeated return;

    doPtr->tileNum = doPtr->mapAddr; 
    // ^ in this case I'm thinking ahead
    // do_9DA2 rapes the value at $08 if you don't do this.
    doPtr->mapAddr = 0x1DFA;
    do_9DA2(game, doPtr, 0x0A);

    return;
}

void do_9DA2(zgPtr game, DngObj *doPtr, u32 index)
{
    u32 i = index; // i is $0E, index is $0A
    u32 j = 8; // j is $0C
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;
    u32 mem_08 = doPtr->tileNum; // for later use.
    bufPtr rom = game->image;

    while(j > 0)
    {
        i = index; x = mem_08;

        while(i > 0)
        {
            RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y));

            y += 2; x += 2; i--;
        }

        mem_08 += 0x80; j--;
    }

    return;
}

void do_9DD9(zgPtr game, DngObj *doPtr)
{
    // also maybe check if Trinexx has been defeated?

    u32 mem_08 = doPtr->mapAddr;
    doPtr->mapAddr = doPtr->tileNum;
    doPtr->tileNum = mem_08;

    do_9DA2(game, doPtr, 0x0A);

    return;
}

void do_9DE5(zgPtr game, DngObj *doPtr)
{   
    u32 value = 0x0514;
    doPtr->mapAddr = 0x16DC;        

    do_9E06(game, doPtr, value);

    value = 0x0554;
    doPtr->mapAddr = 0x17F6;  

    do_9E06(game, doPtr, value);

    value = 0x1514;
    doPtr->mapAddr = 0x1914;  

    do_9E06(game, doPtr, value);

    value = 0x1554;
    doPtr->mapAddr = 0x1A2A;  

    do_9E06(game, doPtr, value);

    return;
}

void do_9E06(zgPtr game, DngObj *doPtr, u32 index)
{
    u32 i = index;
    u32 j = 0x000C;
    u32 k = 0x000C;
    bufPtr rom = game->image;

    doPtr->mapNum = 0; // must be on BG0
    // this is the torch light, it only works there

    while(j > 0)
    {
        k = 0x000C; doPtr->tileNum = i;

        while(k > 0)
        {
            RegObjTile(game, 
                       doPtr, 
                       0x000 + doPtr->tileNum,
                       Get2Bytes(rom, 0x1B52 + doPtr->mapAddr));


            doPtr->tileNum += 2; doPtr->mapAddr += 2; k--;
        }

        i += 0x80; j--;
    }

    return;
}

void do_9E30(zgPtr game, DngObj *doPtr)
{
    u32 i = 0x0E;
    u32 x = doPtr->mapAddr;
    u32 y = 0x1B4A;
    u32 value = 0;
    bufPtr rom = game->image;

    while(i > 0)
    {
        value = Get2Bytes(rom, 0x1B52 + y); // step 1.
        RegObjTile(game, doPtr, 0x000 + x, value);
        value |= 0x4000;
        RegObjTile(game, doPtr, 0x01A + x, value);

        value = Get2Bytes(rom, 0x1B6E + y); // step 2.
        RegObjTile(game, doPtr, 0x002 + x, value);
        RegObjTile(game, doPtr, 0x004 + x, value);

        value ^= 0x4000;
        RegObjTile(game, doPtr, 0x016 + x, value);
        RegObjTile(game, doPtr, 0x018 + x, value);

        value = Get2Bytes(rom, 0x1B8A + y); // step 3.
        RegObjTile(game, doPtr, 0x006 + x, value);
 
        value ^= 0x4000;
        RegObjTile(game, doPtr, 0x014 + x, value);

        value = Get2Bytes(rom, 0x1BA6 + y); // step 4.
        RegObjTile(game, doPtr, 0x008 + x, value);
 
        value ^= 0x4000;
        RegObjTile(game, doPtr, 0x012 + x, value);

        value = Get2Bytes(rom, 0x1BC2 + y); // step 5.
        RegObjTile(game, doPtr, 0x00A + x, value);
 
        value ^= 0x4000;
        RegObjTile(game, doPtr, 0x010 + x, value);

        value = Get2Bytes(rom, 0x1BDE + y); // step 6.
        RegObjTile(game, doPtr, 0x00C + x, value);
 
        value ^= 0x4000;
        RegObjTile(game, doPtr, 0x00E + x, value);  

        x += 0x0080;
        y += 0x0002;
        i--;
    }   

    return;
}

void do_9EA3(zgPtr game, DngObj *doPtr)
{
    u32 i = 6;
    u32 x = doPtr->mapAddr;
    u32 y = 0x1BF2;
    u32 value = 0;
    bufPtr rom = game->image;
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];
  
    while(i > 0)
    {
        value = Get2Bytes(rom, 0x1B52 + y);
        RegObjTile(game, doPtr, 0x20E + x, value);   
        RegObjTile(game, doPtr, 0x21A + x, value);   
        RegObjTile(game, doPtr, 0x226 + x, value);     
         
        value = Get2Bytes(rom, 0x1B54 + y);
        RegObjTile(game, doPtr, 0x28E + x, value);   
        RegObjTile(game, doPtr, 0x29A + x, value);   
        RegObjTile(game, doPtr, 0x2A6 + x, value);       

        value = Get2Bytes(rom, 0x1B56 + y);
        RegObjTile(game, doPtr, 0x30E + x, value);   
        RegObjTile(game, doPtr, 0x31A + x, value);   
        RegObjTile(game, doPtr, 0x326 + x, value);   

        value = Get2Bytes(rom, 0x1B58 + y);
        RegObjTile(game, doPtr, 0x38E + x, value);   
        RegObjTile(game, doPtr, 0x39A + x, value);   
        RegObjTile(game, doPtr, 0x3A6 + x, value);   

        y += 0x08; x += 0x02; i--;
    }

    i = 5; y = 0x1C22; x = doPtr->mapAddr;

    while(i > 0)
    {       
        value = Get2Bytes(rom, 0x1B52 + y);
        RegObjTile(game, doPtr, 0x504 + x, value);   
        RegObjTile(game, doPtr, 0x486 + x, value);  
        RegObjTile(game, doPtr, 0x408 + x, value);  
        RegObjTile(game, doPtr, 0x38A + x, value);  
        RegObjTile(game, doPtr, 0x30C + x, value);  
        RegObjTile(game, doPtr, 0x28E + x, value);  
        RegObjTile(game, doPtr, 0x210 + x, value);

        value |= 0x4000;
        RegObjTile(game, doPtr, 0x22E + x, value);   
        RegObjTile(game, doPtr, 0x2B0 + x, value);  
        RegObjTile(game, doPtr, 0x332 + x, value);  
        RegObjTile(game, doPtr, 0x3B4 + x, value);  
        RegObjTile(game, doPtr, 0x436 + x, value);  
        RegObjTile(game, doPtr, 0x4B8 + x, value);  
        RegObjTile(game, doPtr, 0x53A + x, value);

        y += 2; x += 0x80; i--;
    }

    i = 6; y = 0x1C2C; x = doPtr->mapAddr;

    while(i > 0)
    {
        value = Get2Bytes(rom, 0x1B52 + y);
        RegObjTile(game, doPtr, 0x584 + x, value);   
        RegObjTile(game, doPtr, 0x884 + x, value);  
        RegObjTile(game, doPtr, 0xB84 + x, value);  
        value |= 0x4000;
        RegObjTile(game, doPtr, 0x5BA + x, value);   
        RegObjTile(game, doPtr, 0x8BA + x, value);  
        RegObjTile(game, doPtr, 0xBBA + x, value);

        value = Get2Bytes(rom, 0x1B54 + y);
        RegObjTile(game, doPtr, 0x586 + x, value);   
        RegObjTile(game, doPtr, 0x886 + x, value);  
        RegObjTile(game, doPtr, 0xB86 + x, value);  
        value |= 0x4000;
        RegObjTile(game, doPtr, 0x5B8 + x, value);   
        RegObjTile(game, doPtr, 0x8B8 + x, value);  
        RegObjTile(game, doPtr, 0xBB8 + x, value);

        value = Get2Bytes(rom, 0x1B56 + y);
        RegObjTile(game, doPtr, 0x588 + x, value);   
        RegObjTile(game, doPtr, 0x888 + x, value);  
        RegObjTile(game, doPtr, 0xB88 + x, value);  
        value |= 0x4000;
        RegObjTile(game, doPtr, 0x5B6 + x, value);   
        RegObjTile(game, doPtr, 0x8B6 + x, value);  
        RegObjTile(game, doPtr, 0xBB6 + x, value);

        value = Get2Bytes(rom, 0x1B58 + y);
        RegObjTile(game, doPtr, 0x58A + x, value);   
        RegObjTile(game, doPtr, 0x88A + x, value);  
        RegObjTile(game, doPtr, 0xB8A + x, value);  
        value |= 0x4000;
        RegObjTile(game, doPtr, 0x5B4 + x, value);   
        RegObjTile(game, doPtr, 0x8B4 + x, value);  
        RegObjTile(game, doPtr, 0xBB4 + x, value);

        y += 0x08; x += 0x80; i--;
    }

    i = 6; y = 0x1C5C; x = doPtr->mapAddr;

    while(i > 0)
    {
        value = Get2Bytes(rom, 0x1B52 + y);
        RegObjTile(game, doPtr, 0x498 + x, value);  
        RegObjTile(game, doPtr, 0x4A4 + x, value);

        value = Get2Bytes(rom, 0x1B5E + y);
        RegObjTile(game, doPtr, 0x518 + x, value);  
        RegObjTile(game, doPtr, 0x524 + x, value);

        y += 0x02; x += 0x02; i--;
    }

    i = 6; y = 0x1C74; x = doPtr->mapAddr;

    // loop 5
    while(i > 0)
    {
        value = Get2Bytes(rom, 0x1B52 + y);
        RegObjTile(game, doPtr, 0x70E + x, value);  
        RegObjTile(game, doPtr, 0xA0E + x, value);

        value = Get2Bytes(rom, 0x1B54 + y);
        RegObjTile(game, doPtr, 0x710 + x, value);  
        RegObjTile(game, doPtr, 0xA10 + x, value);

        y += 0x04; x += 0x80; i--;
    }

    i = 5; y = 0x1C8C; x = doPtr->mapAddr;

    while(i > 0)
    {
        RegObjTile(game, doPtr, 0x48E + x, Get2Bytes(rom, 0x1B52 + y));  
        RegObjTile(game, doPtr, 0x50E + x, Get2Bytes(rom, 0x1B54 + y)); 
        RegObjTile(game, doPtr, 0x58E + x, Get2Bytes(rom, 0x1B56 + y)); 
        RegObjTile(game, doPtr, 0x60E + x, Get2Bytes(rom, 0x1B58 + y)); 
        RegObjTile(game, doPtr, 0x68E + x, Get2Bytes(rom, 0x1B5A + y)); 

        y += 0x0A; x += 0x02; i--;
    }

    i = 4; x = doPtr->mapAddr;
    
    while(i > 0)
    {
        value = Get2Bytes(BG_Map, 0xE1C + x);
        value |= 0x2000; // priority
        RegObjTile(game, doPtr, 0xE1C + x, value);
          
        value = Get2Bytes(BG_Map, 0xE9C + x);
        value |= 0x2000; // priority
        RegObjTile(game, doPtr, 0xE9C + x, value);
      
        x += 0x02; i--;
    }
        

    return;
}

void do_A095(zgPtr game, DngObj *doPtr)
{
    u32 i = 0x06;
    u32 x = doPtr->mapAddr;
    u32 y = 0x202E;
    u32 value = 0;
    bufPtr rom = game->image;

    while(i > 0)
    {
        value = Get2Bytes(rom, 0x1B52 + y);
        RegObjTile(game, doPtr, 0x002 + x, value);
        RegObjTile(game, doPtr, 0x004 + x, value);
        RegObjTile(game, doPtr, 0x082 + x, value);        
        RegObjTile(game, doPtr, 0x084 + x, value);

        value = Get2Bytes(rom, 0x1B54 + y);
        RegObjTile(game, doPtr, 0x102 + x, value);
        value |= 0x4000;
        RegObjTile(game, doPtr, 0x104 + x, value);

        x += 0x04; i--;
    }

    i = 0x03; x = doPtr->mapAddr;

    while(i > 0)
    {
        value = Get2Bytes(rom, 0x1B56 + y);
        RegObjTile(game, doPtr, 0x180 + x, value);
        RegObjTile(game, doPtr, 0x184 + x, value);
        RegObjTile(game, doPtr, 0x194 + x, value);        
        RegObjTile(game, doPtr, 0x198 + x, value);

        value |= 0x4000;
        RegObjTile(game, doPtr, 0x182 + x, value);
        RegObjTile(game, doPtr, 0x186 + x, value);
        RegObjTile(game, doPtr, 0x196 + x, value);        
        RegObjTile(game, doPtr, 0x19A + x, value);

        value = Get2Bytes(rom, 0x1B5C + y);
        RegObjTile(game, doPtr, 0x188 + x, value);
        RegObjTile(game, doPtr, 0x18C + x, value);
        RegObjTile(game, doPtr, 0x190 + x, value);        

        value |= 0x4000;
        RegObjTile(game, doPtr, 0x18A + x, value);
        RegObjTile(game, doPtr, 0x18E + x, value);
        RegObjTile(game, doPtr, 0x192 + x, value);        

        y += 2; x += 0x80; i--;
    }

    x = doPtr->mapAddr;

    value = Get2Bytes(rom, 0x1B5C + y);
    RegObjTile(game, doPtr, 0x000 + x, value);
    RegObjTile(game, doPtr, 0x080 + x, value);

    value |= 0x4000;
    RegObjTile(game, doPtr, 0x01A + x, value);
    RegObjTile(game, doPtr, 0x09A + x, value);  

    value = Get2Bytes(rom, 0x1B5E + y);
    RegObjTile(game, doPtr, 0x100 + x, value);

    value |= 0x4000;
    RegObjTile(game, doPtr, 0x11A + x, value);

    i = 0x04; y = 0x202E; x = doPtr->mapAddr;

    while(i > 0)
    {
        value = Get2Bytes(rom, 0x1B66 + y);
        RegObjTile(game, doPtr, 0x506 + x, value);

        value ^= 0x4000;
        RegObjTile(game, doPtr, 0x514 + x, value);
        
        value = Get2Bytes(rom, 0x1B6E + y);
        RegObjTile(game, doPtr, 0x508 + x, value);

        value ^= 0x4000;
        RegObjTile(game, doPtr, 0x512 + x, value);

        value = Get2Bytes(rom, 0x1B76 + y);
        RegObjTile(game, doPtr, 0x50A + x, value);

        value ^= 0x4000;
        RegObjTile(game, doPtr, 0x510 + x, value);

        value = Get2Bytes(rom, 0x1B7E + y);
        RegObjTile(game, doPtr, 0x50C + x, value);

        value ^= 0x4000;
        RegObjTile(game, doPtr, 0x50E + x, value);

        y += 2; x += 0x80; i--;
    }

    return;
}

void do_A194(zgPtr game, DngObj *doPtr)
{
    u32 i = 0x03;
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;
    bufPtr rom = game->image;

    RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x));
    RegObjTile(game, doPtr, 0x002 + y, Get2Bytes(rom, 0x1B54 + x));
    RegObjTile(game, doPtr, 0x004 + y, Get2Bytes(rom, 0x1B56 + x));

    while(i > 0)
    {
        RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B58 + x));
        RegObjTile(game, doPtr, 0x082 + y, Get2Bytes(rom, 0x1B5A + x));
        RegObjTile(game, doPtr, 0x084 + y, Get2Bytes(rom, 0x1B5C + x));

        y += 0x80; i--;
    }

    RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B5E + x));
    RegObjTile(game, doPtr, 0x082 + y, Get2Bytes(rom, 0x1B60 + x));
    RegObjTile(game, doPtr, 0x084 + y, Get2Bytes(rom, 0x1B62 + x));

    return;
}

void do_A1D1(zgPtr game, DngObj *doPtr)
{
    u32 i = 0x16;
    u32 x = doPtr->mapAddr;
    u32 y = 0x20F6;
    doPtr->mapNum = 0;
    bufPtr rom = game->image;
    
    while(i > 0)
    {
        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y));
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));
        RegObjTile(game, doPtr, 0x200 + x, Get2Bytes(rom, 0x1B5A + y));
        RegObjTile(game, doPtr, 0x280 + x, Get2Bytes(rom, 0x1B5C + y));
        RegObjTile(game, doPtr, 0x300 + x, Get2Bytes(rom, 0x1B5E + y));
        RegObjTile(game, doPtr, 0x380 + x, Get2Bytes(rom, 0x1B60 + y));
        RegObjTile(game, doPtr, 0x400 + x, Get2Bytes(rom, 0x1B62 + y));
        RegObjTile(game, doPtr, 0x480 + x, Get2Bytes(rom, 0x1B64 + y));
        RegObjTile(game, doPtr, 0x500 + x, Get2Bytes(rom, 0x1B66 + y));

        y += 0x16; x += 2; i--;
    }

    y = 0x22DA; x = doPtr->mapAddr; i = 0x03;

    while(i > 0)
    {
        RegObjTile(game, doPtr, 0x592 + y, Get2Bytes(rom, 0x1B52 + x));
        RegObjTile(game, doPtr, 0x612 + y, Get2Bytes(rom, 0x1B58 + x));
        
        y += 0x02; x += 0x02; i--;
    }


    return;
}

void do_A255(zgPtr game, DngObj *doPtr)
{
    doPtr->tileNum = 0xE0;

    do_8A1F(game, doPtr, 0); 

    return;
}

void do_A25D(zgPtr game, DngObj *doPtr)
{
    // not entirely accurate but for now it works.
    do_A26D(game, doPtr);

    return;
}

void do_A26D(zgPtr game, DngObj *doPtr)
{
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;
    u32 tempX = doPtr->tileNum;
    u32 index = 4;
    u32 mapNumTemp = doPtr->mapNum;
    bufPtr rom = game->image;

    x = doPtr->mapAddr;
    y = tempX;

    while(index > 0)
    {

        doPtr->mapNum = 0;
        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 +  x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));

        doPtr->mapNum = 1;
        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));

        y += 0x08;
        x += 0x02;

    index--;
    }

    doPtr->mapNum = mapNumTemp;

    return;
}

void do_A2C7(zgPtr game, DngObj *doPtr)
{
    // do other shit

    do_97ED(game, doPtr);

    return;
}

void do_A2DF(zgPtr game, DngObj *doPtr)
{
    // do other shit

    doPtr->tileNum = 0x10C8;
    do_97ED(game, doPtr);

    return;
}

void do_A30C(zgPtr game, DngObj *doPtr)
{
    // add up number of similar staircases

    do_A32A(game, doPtr);

    return;
}

void do_A31C(zgPtr game, DngObj *doPtr)
{
    // Add up number of similar staircases

    do_A32A(game, doPtr);

    return;
}

void do_A32A(zgPtr game, DngObj *doPtr)
{
    u32 i = 4;
    u32 y = doPtr->tileNum; // Not a typo, x and y swap for this object
    u32 x = doPtr->mapAddr;
    u32 mapNumTemp = doPtr->mapNum;
    bufPtr rom = game->image;

    while( i > 0)
    {
        doPtr->mapNum = 0;
        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 +  x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));

        doPtr->mapNum = 1;
        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));

        y += 0x08;
        x += 0x2;

        i--;
    }

    doPtr->mapNum = mapNumTemp;

    return;
}

void do_A36E(zgPtr game, DngObj *doPtr)
{
    do_97ED(game, doPtr);

    return;
}

void do_A380(zgPtr game, DngObj *doPtr)
{
    // add code later to check for water twin (0x1B) 

    do_97ED(game, doPtr);
    
    return;
}

void do_A3AE(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->mapAddr;
    u32 y = 0x1108;
    u32 value = 0;
    bufPtr rom = game->image;

    DngObj temp = *doPtr;
    DngObj *doPtr2 = &temp;
    doPtr->mapNum = 0;
    doPtr2->mapNum = 1;

    value = Get2Bytes(rom, 0x1B52 + y);
    RegObjTile(game, doPtr,  0x000 + x, value); 
    RegObjTile(game, doPtr2, 0x000 + x, value);
 
    value = Get2Bytes(rom, 0x1B54 + y);
    RegObjTile(game, doPtr,  0x002 + x, value); 
    RegObjTile(game, doPtr2, 0x002 + x, value);

    value = Get2Bytes(rom, 0x1B56 + y);
    RegObjTile(game, doPtr,  0x004 + x, value); 
    RegObjTile(game, doPtr2, 0x004 + x, value);

    value = Get2Bytes(rom, 0x1B58 + y);
    RegObjTile(game, doPtr,  0x006 + x, value); 
    RegObjTile(game, doPtr2, 0x006 + x, value);

    value = Get2Bytes(rom, 0x1B5A + y);
    RegObjTile(game, doPtr,  0x080 + x, value); 
    RegObjTile(game, doPtr2, 0x080 + x, value);
 
    value = Get2Bytes(rom, 0x1B5C + y);
    RegObjTile(game, doPtr,  0x082 + x, value); 
    RegObjTile(game, doPtr2, 0x082 + x, value);

    value = Get2Bytes(rom, 0x1B5E + y);
    RegObjTile(game, doPtr,  0x084 + x, value); 
    RegObjTile(game, doPtr2, 0x084 + x, value);

    value = Get2Bytes(rom, 0x1B60 + y);
    RegObjTile(game, doPtr,  0x086 + x, value); 
    RegObjTile(game, doPtr2, 0x086 + x, value);

    return;
}

void do_A41B(zgPtr game, DngObj *doPtr)
{
    

    doPtr->tileNum = 0x1088;
    do_97ED(game, doPtr);

    return;
}

void do_A458(zgPtr game, DngObj *doPtr)
{


    // Do other shit

    doPtr->tileNum = 0x10A8;
    do_97ED(game, doPtr);

    return;
}

void do_A486(zgPtr game, DngObj *doPtr)
{
    

    // Do other shit

    doPtr->tileNum = 0x10A8;
    do_97ED(game, doPtr);

    return;
}

void do_A4B4(zgPtr game, DngObj *doPtr)
{
    // Do other shit

    doPtr->tileNum = 0x1148;
    do_8D80(game, doPtr, 4);
    

    return;
}

void do_A4F5(zgPtr game, DngObj *doPtr)
{
    doPtr->tileNum = 0x1178;
    do_8D80(game, doPtr, 4);

    return;
}

void do_A533(zgPtr game, DngObj *doPtr)
{
    doPtr->tileNum = 0x1160;
    
    do_8D80(game, doPtr, 4);

    //doPtr->mapAddr = x;
    //doPtr->mapAddr -= 2;

    return;
}

void do_A584(zgPtr game, DngObj *doPtr)
{
    doPtr->tileNum = 0x1190;
 
    do_8D80(game, doPtr, 4);

    return;
}

void do_A5D2(zgPtr game, DngObj *doPtr)
{
    do_A634(game, doPtr);

    return;
}

void do_A5F4(zgPtr game, DngObj *doPtr)
{
    do_A634(game, doPtr);

    return;
}

void do_A607(zgPtr game, DngObj *doPtr)
{
    do_A634(game, doPtr);

    return;
}

void do_A626(zgPtr game, DngObj *doPtr)
{
    do_A634(game, doPtr);

    return;
}



void do_A634(zgPtr game, DngObj *doPtr)
{
    // in the documentation this is branch alpha of do_A5D2
    u32 x = doPtr->mapAddr;
    u32 y = doPtr->tileNum;
    u32 i = 4;
    bufPtr rom = game->image;
   
    while(i > 0)
    {
        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y)); 

        x += 2; y += 8; i--;
    }

    doPtr->mapAddr = x;
    doPtr->tileNum = y;

    return;
}

void do_A664(zgPtr game, DngObj *doPtr)
{
    // increment number of staircase types
   
    do_A6B5(game, doPtr);

    return;
}

void do_A695(zgPtr game, DngObj *doPtr)
{   
    // increment number of staircase types

    do_A6B5(game, doPtr);

    return;
}

void do_A6B5(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->mapAddr;
    u32 y = doPtr->tileNum;
    u32 i = 0x04;

    bufPtr rom = game->image;

    while(i > 0)
    {
        doPtr->mapNum = 1;
        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        doPtr->mapNum = 0;
        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y));

        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));

        y += 8; x += 2; i--;
    }

    doPtr->tileNum = doPtr->mapAddr - 0x200;

    do_A6EE(game, doPtr);

    return;
}

void do_A6EE(zgPtr game, DngObj *doPtr)
{
    u32 a = 0;
    u32 x = doPtr->tileNum;
    bufPtr bg = game->BG_Map[1];

    a = Get2Bytes(bg, 0x000 + x) | 0x2000;
    Put2Bytes(bg,     0x000 + x, a);

    a = Get2Bytes(bg, 0x080 + x) | 0x2000;
    Put2Bytes(bg,     0x080 + x, a);

    a = Get2Bytes(bg, 0x100 + x) | 0x2000;
    Put2Bytes(bg,     0x100 + x, a);

    a = Get2Bytes(bg, 0x180 + x) | 0x2000;
    Put2Bytes(bg,     0x180 + x, a);

    return;
}

void do_A71C(zgPtr game, DngObj *doPtr)
{
    do_A767(game, doPtr);

    return;
}

void do_A74A(zgPtr game, DngObj *doPtr)
{
    do_A767(game, doPtr);

    return;
}

void do_A767(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->mapAddr;
    u32 y = doPtr->tileNum;
    u32 i = 0x04;

    bufPtr rom = game->image;

    while(i > 0)
    {
        doPtr->mapNum = 0;
        RegObjTile(game, doPtr, 0x000 + x, Get2Bytes(rom, 0x1B52 + y)); 
        RegObjTile(game, doPtr, 0x080 + x, Get2Bytes(rom, 0x1B54 + y));
        RegObjTile(game, doPtr, 0x100 + x, Get2Bytes(rom, 0x1B56 + y));
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));

        doPtr->mapNum = 1;
        RegObjTile(game, doPtr, 0x180 + x, Get2Bytes(rom, 0x1B58 + y));

        y += 8; x += 2; i--;
    }

    doPtr->tileNum = doPtr->mapAddr + 0x200;

    do_A6EE(game, doPtr);

    return;
}

void do_A7A3(zgPtr game, DngObj *doPtr)
{   
    u32 mem_08 = doPtr->mapAddr;

    do_8D80(game, doPtr, 0x03);

    doPtr->mapAddr = mem_08 + 0x0180;

    do_8D80(game, doPtr, 0x03);

    return;
}

void do_A7B6(zgPtr game, DngObj *doPtr)
{   
    u32 mem_08 = doPtr->mapAddr;

    do_97ED(game, doPtr);

    doPtr->mapAddr = mem_08 + 0x0100;
    doPtr->tileNum = 0x2376;

    do_97ED(game, doPtr);

    doPtr->mapAddr = mem_08 + 0x0300;
    doPtr->tileNum = 0x2396;

    do_97ED(game, doPtr);

    return;
}

void do_A7D3(zgPtr game, DngObj *doPtr)
{
    // check if it's been bombed open

    do_A7DC(game, doPtr);

    return;
}

void do_A7DC(zgPtr game, DngObj *doPtr)
{
    u32 y = doPtr->mapAddr;

    do_97ED(game, doPtr);
    do_97ED(game, doPtr);

    doPtr->mapAddr = y + 0x200;

    do_97ED(game, doPtr);
    do_97ED(game, doPtr);

    return;
}

void do_A7F0(zgPtr game, DngObj *doPtr)
{
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;

    do_97ED(game, doPtr);

    doPtr->mapAddr = y + 0x1FC;
  
    x = doPtr->tileNum; // Save tileNum's value

    do_97ED(game, doPtr);

    doPtr->tileNum = x;
    doPtr->mapAddr = y + 0x204;

    do_97ED(game, doPtr);

    return;
}

void do_A809(zgPtr game, DngObj *doPtr)
{
    u32 mem_08 = doPtr->mapAddr;

    do_8A44(game, doPtr, 0x05);

    doPtr->mapAddr = mem_08 + 0x200;

    do_8A44(game, doPtr, 0x05);

    return;
}

void do_A81C(zgPtr game, DngObj *doPtr)
{
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];
    bufPtr rom = game->image;

    u32 x = doPtr->tileNum;
    u32 y = Get2Bytes(rom, 0x197E + x);
    u32 mem_08 = y;
    u32 a = doPtr->arg1;
    u32 i = 0;

    switch(a)
    {

    case 0x02:

        a = y & 0xF07F;
        do_B1A4(game, doPtr, &x, &y, &a);
        goto default_door;

        break;

    case 0x06:
    


        break;

    case 0x08:

        goto default_door;

goBack:



        break;

    case 0x12:

        // door to overworld (only up or down)
        // LDX $19E0
        // TYA : STA $19E2, X
        // INX : INX : STX $19E0

        break;

    case 0x14:
        
        // TYA : SEC : SBC #$00FE :
        // LDX $0450 : LSR A : STA $06D0, X : INX : INX
        //STX $0450

        break;

    case 0x20:
    case 0x22:
    case 0x24:
    case 0x26:

        if(a < 0x24)
            goto lower_half;

        x = 0; a = 0;

        do_B0DA(game, doPtr, &x, &y, &a);

        y = Get2Bytes(rom, 0x4D9E + y); x = mem_08; i = 0x04;
        doPtr->mapNum = 0;

        while(i > 0)
        {
            a = Get2Bytes(rom, 0x1B52 + y);
            RegObjTile(game, doPtr, 0x000 + x, a);

            a = Get2Bytes(rom, 0x1B54 + y);
            RegObjTile(game, doPtr, 0x080 + x, a);

            a = Get2Bytes(rom, 0x1B56 + y);
            RegObjTile(game, doPtr, 0x100 + x, a);
    
            x += 0x02; y += 0x06; i--;
        }

        break;
    
    case 0x30:

        // Those exploding passages, most notably in Skull Woods when you pull a lever and a path to the big chest opens up.


        break;

    case 0x32:

        // Door that you have to swing sword to open

        // $B0DA : 

        break;

    case 0x16:

        a = y - 0x00FE;
 
        /*
        LDX $044E;
        LSR A;
        STA $06C0, X;
        INX;
        INX:
        STX $044E;*/

        break;

    default:
    default_door:

        x = doPtr->tileNum;

        if(x >= 0x40)
            return;
        
        x = doPtr->tileNum;

        if(x >= 0x0C)
        {
            do_A984(game, doPtr);
        }

    lower_half:
        
        mem_08 = y; x = 0; a = 0;

        do_B0DA(game, doPtr, &x, &y, &a);

        x = Get2Bytes(rom, 0x4D9E + y); y = mem_08; i = 4;

        while(i > 0)
        {
            a = Get2Bytes(rom, 0x1B52 + x);
            RegObjTile(game, doPtr, 0x000 + y, a);

            a = Get2Bytes(rom, 0x1B54 + x);
            RegObjTile(game, doPtr, 0x080 + y, a);

            a = Get2Bytes(rom, 0x1B56 + x);
            RegObjTile(game, doPtr, 0x100 + y, a);

            x += 6; y += 2; i--;
        }

        if(doPtr->arg2 == 0x08)
            goto goBack;

        break;
    }

    return;
}

void do_A984(zgPtr game, DngObj *doPtr)
{  
    bufPtr rom = game->image;

    u32 a = doPtr->arg1;
    u32 x = doPtr->tileNum;
    u32 y = Get2Bytes(rom, 0x1996 + x);
    u32 mem_08 = y;
    u32 i = 0;

    if(doPtr->funcNum == 0)
    {
        y = Get2Bytes(rom, 0x198A + x);

        if( a != 0x02 )
            if( a != 0x08)
                goto default_door;
    }

    switch(a)
    {

    case 0x02:

        a = y + 0x200;

        do_B1A4(game, doPtr, &x, &y, &a);
        goto default_door;

        break;

    case 0x04:

        break;

    case 0x06:

        a = a;

        break;

    case 0x08:

        break;

    case 0x0A:

        mem_08 -= 0x0206;

        doPtr->mapAddr = 0x2656;
        doPtr->tileNum = mem_08;
        do_9DA2(game, doPtr, 0x0A);

        break;

    case 0x0C:

        doPtr->mapNum = 0;

        mem_08 -= 0x0206;

        doPtr->mapAddr = 0x2656;
        doPtr->tileNum = mem_08;
        do_9DA2(game, doPtr, 0x0A);

        doPtr->mapNum = 1; i = 0x0A;

        x = mem_08 - 0x0080;

        while(i > 0)
        {
            a = Get2Bytes(game->BG_Map[0], x) | 0x2000;
            RegObjTile(game, doPtr, x, a);
            
            x += 2; i--;
        }

        break;

    case 0x0E:
    case 0x10:

        a = y + 0x200;

        x = 0; a = 1;

        do_B0DA(game, doPtr, &x, &y, &a);

        doPtr->mapAddr = mem_08; doPtr->tileNum = 0x26F6; 

        do_97ED(game, doPtr);

        break;

    case 0x12:

        break;

    case 0x14:


        break;

    case 0x16:

        

        break;


    case 0x40:


        break;

    default:
    default_door:

        mem_08 = y; x = 0; a = 0x01;

        do_B0DA(game, doPtr, &x, &y, &a);

        x = Get2Bytes(rom, 0x4E06 + y);

        y = mem_08; i = 0x04; 

        while(i > 0)
        {
            a = Get2Bytes(rom, 0x1B52 + x);
            RegObjTile(game, doPtr, 0x080 + y, a);

            a = Get2Bytes(rom, 0x1B54 + x);
            RegObjTile(game, doPtr, 0x100 + y, a);

            a = Get2Bytes(rom, 0x1B56 + x);
            RegObjTile(game, doPtr, 0x180 + y, a);

            x += 6; y += 2; i--;
        }

        break;
    }

    return;
}

void do_AAD7(zgPtr game, DngObj* doPtr)
{
    //!

    bufPtr rom = game->image;

    u32 a = doPtr->arg1;
    u32 x = doPtr->tileNum;
    u32 y = Get2Bytes(rom, 0x19AE + x);
    u32 mem_08 = y;
    u32 i = 0;
    u32 pushY = 0, pushA = 0;

    switch(a)
    {
    case 0x06:

        a = a;

        break;

    default:

        if(a >= 0x40)
        {
            do_AE40(game, doPtr, &x, &y, &a);

            return;

        }

        x = doPtr->tileNum;

        if(x >= 0x0C)
        {
            pushA = a; pushY = y; 
            y = Get2Bytes(rom, 0x19BA + x);
            a = doPtr->arg1;

            do_AB99(game, doPtr);

            a = pushA; y = pushY;
            doPtr->arg2 = doPtr->arg1; // just going by the book
        }

        x = 0; a = 3;

        do_B0DA(game, doPtr, &x, &y, &a);

        x = Get2Bytes(rom, 0x4E66 + y); y = mem_08; i = 0x03;

        while(i > 0)
        {
            a = Get2Bytes(rom, 0x1B52 + x);
            RegObjTile(game, doPtr, 0x000 + y, a);

            a = Get2Bytes(rom, 0x1B54 + x);
            RegObjTile(game, doPtr, 0x080 + y, a);

            a = Get2Bytes(rom, 0x1B56 + x);
            RegObjTile(game, doPtr, 0x100 + y, a);

            a = Get2Bytes(rom, 0x1B58 + x);
            RegObjTile(game, doPtr, 0x180 + y, a);

            x += 8; y += 2; i--;
        }

        break;

    }
  
    return;
}

void do_AB99(zgPtr game, DngObj* doPtr)
{
    bufPtr rom = game->image;

    u32 a = doPtr->arg1;
    u32 x = doPtr->tileNum;
    u32 y = Get2Bytes(rom, 0x19C6 + x);
    u32 mem_08 = y;
    u32 i = 0;

    if(doPtr->funcNum == 2)
        y = Get2Bytes(rom, 0x19BA + x);

    switch(a)
    {
    case 0x06:

        a = a;

        break;

    default:

        mem_08 = y; x = 0; a = 3;

        do_B0DA(game, doPtr, &x, &y, &a);

        x = Get2Bytes(rom, 0x4EC6 + y); y = mem_08 + 2; i = 0x03;

        while(i > 0)
        {
            a = Get2Bytes(rom, 0x1B52 + x);
            RegObjTile(game, doPtr, 0x000 + y, a);

            a = Get2Bytes(rom, 0x1B54 + x);
            RegObjTile(game, doPtr, 0x080 + y, a);

            a = Get2Bytes(rom, 0x1B56 + x);
            RegObjTile(game, doPtr, 0x100 + y, a);

            a = Get2Bytes(rom, 0x1B58 + x);
            RegObjTile(game, doPtr, 0x180 + y, a);

            x += 8; y += 2; i--;
        }

        break;

    }
    


    return;
}

void do_AE40(zgPtr game, DngObj *doPtr, u32* x1, u32 *y1, u32 *a1)
{   
    u32 x = *x1, y = *y1, a = *a1;
    u32 mem_08 = 0;
    u32 i = 0;

    bufPtr rom = game->image;

    x = doPtr->tileNum;

    if(x >= 0x0C)
    {
        mem_08 = y; x = 0x10; 
        
        do_B0DA(game, doPtr, &x, &y, &a);

        a = 0x40;
        
        if(y == 0x48)
            goto special_a;

        a = 0x40;

        if(a != 0x4A)
            goto not_special_a;

special_a:


not_special_a:

        y = Get2Bytes(rom, 0x4EC6 + y);

        x = mem_08; i = 0x02; doPtr->mapNum = 0;

        while(i > 0)
        {
            a = Get2Bytes(rom, 0x1B52 + y);
            RegObjTile(game, doPtr, 0x002 + x, a);

            a = Get2Bytes(rom, 0x1B54 + y);
            RegObjTile(game, doPtr, 0x082 + x, a);

            a = Get2Bytes(rom, 0x1B56 + y);
            RegObjTile(game, doPtr, 0x102 + x, a);

            a = Get2Bytes(rom, 0x1B58 + y);
            RegObjTile(game, doPtr, 0x182 + x, a);

            y += 0x08; x += 0x02; i--;
        }

        doPtr->mapNum = 1;

        a = Get2Bytes(rom, 0x1B52 + y);
        RegObjTile(game, doPtr, 0x002 + x, a);

        a = Get2Bytes(rom, 0x1B54 + y);
        RegObjTile(game, doPtr, 0x082 + x, a);

        a = Get2Bytes(rom, 0x1B56 + y);
        RegObjTile(game, doPtr, 0x102 + x, a);

        a = Get2Bytes(rom, 0x1B58 + y);
        RegObjTile(game, doPtr, 0x182 + x, a);
    }

    mem_08 = y; x = 0; a = 0x02;

    do_B0DA(game, doPtr, &x, &y, &a);

    a = 0x44;

    if(y == 0x48)
        goto special;

    a = 0x40;

    if(a != 0x4A)
        goto not_special;

special:

      

not_special:

    y = Get2Bytes(rom, 0x4E66 + y); x = mem_08;

    doPtr->mapNum = 1;

    a = Get2Bytes(rom, 0x1B52 + y);
    RegObjTile(game, doPtr, 0x000 + x, a);

    a = Get2Bytes(rom, 0x1B54 + y);
    RegObjTile(game, doPtr, 0x080 + x, a);

    a = Get2Bytes(rom, 0x1B56 + y);
    RegObjTile(game, doPtr, 0x100 + x, a);

    a = Get2Bytes(rom, 0x1B58 + y);
    RegObjTile(game, doPtr, 0x180 + x, a);

    y += 0x08; x += 0x02; i = 2; doPtr->mapNum = 0;

    while(i > 0)
    {
        a = Get2Bytes(rom, 0x1B52 + y);
        RegObjTile(game, doPtr, 0x000 + x, a);

        a = Get2Bytes(rom, 0x1B54 + y);
        RegObjTile(game, doPtr, 0x080 + x, a);

        a = Get2Bytes(rom, 0x1B56 + y);
        RegObjTile(game, doPtr, 0x100 + x, a);

        a = Get2Bytes(rom, 0x1B58 + y);
        RegObjTile(game, doPtr, 0x180 + x, a);

        y += 0x08; x += 0x02; i--;
    }

    a = mem_08;

    return;
}   

void do_B0AC(DngObj *doPtr)
{
    u32 index = 1;

    do_B0AF(doPtr, index);

    return;
}

void do_B0AF(DngObj *doPtr, u32 index)
{
    //!

    u32 a = doPtr->arg1, b = doPtr->arg2;

    a = ((a << 2) | b) + index;
    b = 0;

    doPtr->arg1 = a; doPtr->arg2 = b;

    return;
}

void do_B0BE(DngObj *doPtr)
{
    //!
    // simulation of $B0BE in rom
  
    u32 a = doPtr->arg1;
    u32 b = doPtr->arg2;

    a = (a << 2) | b;

    if(!a)
        a = 0x1A;

    doPtr->arg1 = a;

    return;
}

void do_B0CC(DngObj *doPtr)
{
    //!

    u32 a = doPtr->arg1; u32 b = doPtr->arg2;

    a = (a << 2) | b;

    if(!a)
        a = 0x0020;

    doPtr->arg1 = a;

    return;
}

bool do_B0DA(zgPtr game, DngObj *doPtr, u32 *x, u32 *y, u32 *a)
{
    u32 x1 = *x, y1 = *y, a1 = *a;

    y1 = x1 & 0x000F;

    if(y1 < 0x08)
    {

        

    }

    y1 = doPtr->arg2;
    x1 += 2;

    if(y1 != 0x1A)
        goto success;

success:

    *x = x1; *y = y1; *a = a1;

    return true;

failure:

    *x = x1; *y = y1; *a = a1;

    return false;

}

void do_B1A4(zgPtr game, DngObj *doPtr, u32 *x, u32 *y, u32 *a)
{
    u32 x1 = *x, y1 = *y, a1 = *a;
    u32 i = 0x07;

    bufPtr bg = game->BG_Map[1];
    bufPtr rom = game->image;

    x1 = a1;

    while(i > 0)
    {
        a1 = Get2Bytes(bg, 0x000 + x1) | 0x2000;
        RegObjTile(game, doPtr, 0x000 + x1, a1);

        a1 = Get2Bytes(bg, 0x002 + x1) | 0x2000;
        RegObjTile(game, doPtr, 0x002 + x1, a1);

        a1 = Get2Bytes(bg, 0x004 + x1) | 0x2000;
        RegObjTile(game, doPtr, 0x004 + x1, a1);

        a1 = Get2Bytes(bg, 0x006 + x1) | 0x2000;
        RegObjTile(game, doPtr, 0x006 + x1, a1);

        x1 += 0x80; i--;
    }

    *x = x1, *y = y1, *a = a1;

    return;
}

void do_B220(zgPtr game, DngObj *doPtr, u32 index)
{
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];
    bufPtr rom = game->image;

    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;
    u32 i = doPtr->arg1;

    while(i > 0)
    {
        RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x)); 
        RegObjTile(game, doPtr, 0x002 + y, Get2Bytes(rom, 0x1B54 + x));
        RegObjTile(game, doPtr, 0x004 + y, Get2Bytes(rom, 0x1B56 + x));
        RegObjTile(game, doPtr, 0x006 + y, Get2Bytes(rom, 0x1B58 + x));
        RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B5A + x)); 
        RegObjTile(game, doPtr, 0x082 + y, Get2Bytes(rom, 0x1B5C + x));
        RegObjTile(game, doPtr, 0x084 + y, Get2Bytes(rom, 0x1B5E + x));
        RegObjTile(game, doPtr, 0x086 + y, Get2Bytes(rom, 0x1B60 + x));

        y += index;
        i--;
    }  

    doPtr->mapAddr = y;

    return;
}

void do_B279(zgPtr game, DngObj *doPtr)
{
    bufPtr rom = game->image;
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];

    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x)); 
    RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B54 + x));
    RegObjTile(game, doPtr, 0x100 + y, Get2Bytes(rom, 0x1B56 + x));
    RegObjTile(game, doPtr, 0x180 + y, Get2Bytes(rom, 0x1B58 + x));
    RegObjTile(game, doPtr, 0x200 + y, Get2Bytes(rom, 0x1B5A + x));

    return;
}

void do_B29C(zgPtr game, DngObj *doPtr)
{
    u32 i = doPtr->arg1 - 1;

    while(i > 0)
    {
        do_B279(game, doPtr);

        doPtr->mapAddr += 0x82; i--;
    }

    return;
}

void do_B2AA(zgPtr game, DngObj *doPtr)
{
    u32 i = doPtr->arg1 - 1;

    while(i > 0)
    {
        do_B279(game, doPtr);

        doPtr->mapAddr -= 0x7E;

        i--;
    }

    return;
}

void do_B2AF(zgPtr game, DngObj *doPtr) 
{
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];
    bufPtr rom = game->image;

    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;
    
    RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x)); 
    RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B54 + x));
    RegObjTile(game, doPtr, 0x002 + y, Get2Bytes(rom, 0x1B56 + x));
    RegObjTile(game, doPtr, 0x082 + y, Get2Bytes(rom, 0x1B58 + x));

    y += 0x100;

    doPtr->mapAddr = y;

    return;
}

void do_B2CA(zgPtr game, DngObj *doPtr)
{
    doPtr->tileNum += 2;
    doPtr->mapAddr += 2;

    do_B2CE(game, doPtr);

    return;
}

void do_B2CE(zgPtr game, DngObj *doPtr)
{
    do_B2D0(game, doPtr, doPtr->arg1);

    return;
}

void do_B2D0(zgPtr game, DngObj *doPtr, u32 index)
{
    u32 i = index;
    u32 x = doPtr->tileNum;
    u32 y = doPtr->mapAddr;

    bufPtr rom = game->image;
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];

    u32 j = Get2Bytes(rom, 0x1B52 + x);

    while(i > 0)
    {
        RegObjTile(game, doPtr, y, j);
    
        y += 2;
        i--;
    }

    doPtr->mapAddr = y;
    doPtr->tileNum = x;

    return;
}

void do_B2E1(zgPtr game, DngObj *doPtr)
{
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;
    bufPtr rom = game->image;

    RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x)); 
    RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B54 + x)); 
    RegObjTile(game, doPtr, 0x100 + y, Get2Bytes(rom, 0x1B56 + x));
    RegObjTile(game, doPtr, 0x180 + y, Get2Bytes(rom, 0x1B58 + x));
    
    return;
}

void do_B2F6(zgPtr game, DngObj *doPtr)
{
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;
    bufPtr rom = game->image;

    RegObjTile(game, doPtr, 0x000 + y, Get2Bytes(rom, 0x1B52 + x)); 
    RegObjTile(game, doPtr, 0x080 + y, Get2Bytes(rom, 0x1B54 + x));
    RegObjTile(game, doPtr, 0x100 + y, Get2Bytes(rom, 0x1B56 + x));

    return;
}

void do_B306(zgPtr game, DngObj *doPtr)
{
    do_B33A(game, doPtr, 0x1010);

    return;
}

void do_B30B(zgPtr game, DngObj *doPtr)
{
    do_B33A(game, doPtr, 0x1212);

    return;
}

void do_B310(zgPtr game, DngObj *doPtr)
{
    u32 y = doPtr->mapAddr;

    doPtr->tileNum = 0x0E62;
    do_B33A(game, doPtr, 0x2020);

    doPtr->tileNum = 0x0E6A;
    do_B33A(game, doPtr, 0x2121);

    doPtr->mapAddr = y + 0x100;

    doPtr->tileNum = 0x0E72;
    do_B33A(game, doPtr, 0x2222);

    doPtr->tileNum = 0x0E7A;
    do_B33A(game, doPtr, 0x2323);

    return;
}

void do_B33A(zgPtr game, DngObj *doPtr, u32 index)
{
    DngObjStruct *doStruct = game->dngObjs;
    u32 *n = &doStruct->numTwoStateObjs;

    if(*n > 0x10)
        return;

    doStruct->twoStateLocs[*n] = doPtr->mapAddr;

    if(doPtr->mapNum == 0)
        doStruct->twoStateLocs[*n] |= 0x2000;

    (*n)++;
 
    do_9895(game, doPtr);
}

void do_B376(zgPtr game, DngObj *doPtr)
{
    u32 i = doPtr->arg1;

    do_B0AC(doPtr);

    while(i > 0)
    {
        do_B395(game, doPtr);
        
        i--;        
    }

    return;
}

void do_B381(zgPtr game, DngObj *doPtr)
{
    u32 i = doPtr->arg1;
    u32 mem_08 = doPtr->mapAddr;

    do_B0AC(doPtr);

    while(i > 0)
    {
        do_B395(game, doPtr);
        
        mem_08 += 0x0100; doPtr->mapAddr = mem_08; i--;        
    }

    return;
}

void do_B395(zgPtr game, DngObj *doPtr)
{
    // Single pot objects (1.3.0x2F)

    DngObjStruct *doStruct = game->dngObjs;

    bool darkWorld = doStruct->darkWorld;
    u32 *n = &(doStruct->numTwoStateObjs);

    if(*n >= 0x10)
        return;

    doStruct->twoStateLocs[*n] = doPtr->mapAddr;

    if(doPtr->mapNum == 0)
        doStruct->twoStateLocs[*n] |= 0x2000;
    
    if(darkWorld)
        doPtr->tileNum = 0x0E92;

    do_9895(game, doPtr);

    (*n)++;

    return;
}

void do_B3E1(zgPtr game, DngObj *doPtr)
{
    u32 y = doPtr->mapAddr;
    u32 x = doPtr->tileNum;

    if(game->currentDungRoom == 0x65)
        return;

    x = 0x0220;
    do_9895(game, doPtr);

    x = 0x0228;
    do_9895(game, doPtr);

    doPtr->mapAddr = y + 0x100;
    x = 0x0230;
    do_9895(game, doPtr);

    x = 0x0238;
    do_9895(game, doPtr);

    return;
}

void do_B474(zgPtr game, DngObj *doPtr)
{
    u32 i = doPtr->arg1;

    do_B0AC(doPtr);

    while(i > 0)
    {
        do_B493(game, doPtr);
        
        i--;        
    }
    
    return;
}

void do_B47F(zgPtr game, DngObj *doPtr)
{
    // vertical line of moles... unused?

    u32 mem_08 = doPtr->mapAddr;
    u32 i = doPtr->arg1;

    do_B0AC(doPtr);

    while(i > 0)
    {
        do_B493(game, doPtr);
        
        mem_08 += 0x0100; doPtr->mapAddr = mem_08; i--;        
    }

    return;
}

void do_B493(zgPtr game, DngObj *doPtr)
{
    DngObjStruct *doStruct = game->dngObjs;
    u32 *n = &doStruct->numTwoStateObjs;

    if(*n >= 0x10)
        return;

    doStruct->twoStateLocs[*n] = doPtr->mapAddr;

    if(doPtr->mapNum == 0)
        doStruct->twoStateLocs[*n] |= 0x2000;
    
    (*n)++;

    do_9895(game, doPtr);

    return;
}

void do_null(zgPtr game, DngObj *doPtr)
{
    doPtr->arg1 = 0;

    return;
}

void do_free(zgPtr game, DngObj *doPtr)
{
    // This is so I can keep track of objects that lead to an RTS
    // Could use for custom code later though

    //doPtr->arg1 = 0;

    return;
}

DngObj* GetDngObj(DngObjList *doList, u32 index)
{
    if(!doList)
        return 0;

    if(index > doList->numObj)
        return 0;

    if( ( (index + 1) * sizeof(DngObj)) > doList->data->length)
        return 0;

    return &(( (DngObj*) doList->data->contents)[index]);
}

DngObj* GetDngObj(DngObjStruct *d, u32 type, u32 layer, u32 index)
{
    if(!d)
        return 0;

    // use the first definition
    return GetDngObj(GetDoList(d, type, layer), index);
}

DngObj* GetDngObj(DngObjStruct *d, u32 value, u32 layer)
{
    u32 type = (value & 0x4000) ? 2 : 1;

    value &= 0x3FFF;

    // use the second definition
    return GetDngObj(d, type, layer, value); 
}

void RegObjTile(zgPtr game, DngObj *doPtr, u32 offset, u32 value)
{
    /* Structure of the registry:
    Two bytes per entry
    ftiiiiii iiiiiiii

    f = flag indicating the object is registered. prevents false positives
    t = object type (either 0 or 1), but in this case 0 means 1, and 1 means 2
    i = 14 bit index of the object in its respect layer subsection
    */

    u32 index = doPtr->index & 0x7FFF;
    u32 info = 0x8000 | index;
    u32 minX, minY, maxX, maxY, offX, offY;
    int diff = 0; // needs to be signed
    
    bufPtr regN = game->dngObjs->scrapReg[doPtr->mapNum];
    bufPtr BG_Map = game->BG_Map[doPtr->mapNum];

    if(doPtr->mapNum == 1)
        doPtr->isOnBG1 = true;
    else
        doPtr->isOnBG0 = true;

    Put2Bytes(regN, offset, info);
    Put2Bytes(BG_Map, offset, value);

    // If doPtr->doMin is in fact zero, this could mean we haven't
    // registered any tiles yet at all!
    if(!doPtr->doMin)
        doPtr->doMin = offset;

    if(!doPtr->doMax)
        doPtr->doMax = offset;

    if(doPtr->funcNum == 0xDB)
        minX = 0;

    minX = doPtr->doMin & 0x7F; minY = doPtr->doMin & 0x1F80;
    maxX = doPtr->doMax & 0x7F; maxY = doPtr->doMax & 0x1F80;

    offX = offset & 0x7F; offY = offset & 0x1F80;

    minX = min(minX, offX); minY = min(minY, offY);
    maxX = max(maxX, offX); maxY = max(maxY, offY);

    doPtr->doMin = minX | minY;
    doPtr->doMax = maxX | maxY;


    return;
}

bool SearchForObj(DngObjStruct *d, u32 type, u32 subType, u32 funcNum, u32 argNum)
{   
    u32 a = 0;
    u32 b = 0;
    u32 c = 0;

    DngObjList* doList[6];
    DngObj *doPtr;

    // set the object lists we're interested in
    for(a = 0; a < 3; a++)
        doList[a] = &d->type1[a], doList[a+3] = &d->type2[a];

    for(a = 0; a < 3; a++)
    {
        b = 0;

        c = a << 1;

        // if type 2, only look at type 2 objects (and if type 1 only look at type 1)
        if(type == 2)
            c++;

        while(b < doList[c]->numObj)
        {
            doPtr = GetDngObj(doList[c], b);

            if(!doPtr)
                continue;

            if(doPtr->type == type)
            {    if(doPtr->subType == subType)
                    if(doPtr->funcNum == funcNum)
                    {
                        if(argNum != -1)
                        {
                            if(doPtr->arg1 != argNum)
                            { 
                                b++;                             
                                continue;
                            }
                        }
                        
                        // if argNum was not specified return true b/c it passed 
                        // all parameters requested
                        return true;

                    }
            }

            b++;
        }
    }

    return false;
}

DngObjStruct* CreateDngObjStruct()
{
    u32 i = 0;

    DngObjStruct *doStruct = (DngObjStruct*) malloc(sizeof(DngObjStruct));

    if(!doStruct) // failsafe
        return 0;

    for(i = 0; i < 2; i++)
    {
       doStruct->scrapReg[i] = CreateBuffer(0x2000);
       doStruct->BG_Attr[i] = CreateBuffer(0x1000);
    }

    for(i = 0; i < 4; i++)
    {
        doStruct->BG0Registry[i] = CreateBuffer(0x2000);
        doStruct->BG1Registry[i] = CreateBuffer(0x2000);
    }

    doStruct->masterAttrTbl = CreateBuffer(0x200);

    // using a size of zero b/c it will be resized later
    for(i = 0; i < 4; i++)
    {
        doStruct->type1[i].data = CreateBuffer(0);
        doStruct->type1[i].numObj = 0;
        doStruct->type2[i].data = CreateBuffer(0);
        doStruct->type2[i].numObj = 0;
    }



    
    return doStruct;   
}

void InitDngObjStruct(DngObjStruct *doStruct)
{
    u32 i = 0;

    // Initialize all the variables for the structure (but don't mallocate!)

    doStruct->darkWorld = false;
    doStruct->numChests = 0;
    doStruct->numStars = 0;
    doStruct->numBigKeyLocks = 0;
    doStruct->numTwoStateObjs = 0;

    for(i = 0; i < 2; i++)
    {
       ZeroBuffer(doStruct->scrapReg[i]);
       ZeroBuffer(doStruct->BG_Attr[i]);
    }

    for(i = 0; i < 4; i++)
    {
        ZeroBuffer(doStruct->BG0Registry[i]);
        ZeroBuffer(doStruct->BG1Registry[i]);
    }

    ZeroBuffer(doStruct->masterAttrTbl);


    for(i = 0; i < 0x06; i++)
        doStruct->chestObjs[i] = 0;

    for(i = 0; i < 0x10; i++)
        doStruct->twoStateObjs[i] = 0;

    // Scale our object lists back down to zero size
    for(i = 0; i < 4; i++)
    {
        DiscardDngObjs(&doStruct->type1[i]); // dicards the 
        ResizeBuffer(doStruct->type1[i].data, 0);
        doStruct->type1[i].numObj = 0;
 
        DiscardDngObjs(&doStruct->type2[i]);
        ResizeBuffer(doStruct->type2[i].data, 0);
        doStruct->type2[i].numObj = 0;
    }

    //
    memset( &doStruct->sprites, 0, sizeof(SprStruct));
    memset( &(doStruct->sprIndices[0]), 0, sizeof(u32));
    doStruct->numSprites = 0;

    memset(&doStruct->overlords, 0, sizeof(SprStruct));
    memset( &(doStruct->overIndices[0]), 0, sizeof(u32));
    doStruct->numOverlords = 0;

    return;
}

void DiscardDngObjs(DngObjList *doList)
{
    u32 i = 0;
    DngObj *doPtr;

    for(i = 0; i < doList->numObj; i++)
    {
        doPtr = GetDngObj(doList, i);

        if(!doPtr)
            continue;

        DeallocBuffer(doPtr->chr0);
        DeallocBuffer(doPtr->chr1);
        DeallocBuffer(doPtr->below0);
        DeallocBuffer(doPtr->below1);
        DeallocBuffer(doPtr->above0);
        DeallocBuffer(doPtr->above1);
    }

    return;
}

void RegDngObj(zgPtr game, DngObjStruct *d, DngObj *doPtr)
{
    u32 i = 0;
    u32 j = 0;
    u32 h = doPtr->width << 1, v = doPtr->height; // height and width vars for the DngObj
    u32 layer = doPtr->layer;    

    u32 bg = 0;
    u32 value = 0;
    u32 regValue = 0;
    u32 doType = 0;
    bool hasActualTile;
    bool isOnBGs[2] = { doPtr->isOnBG0, doPtr->isOnBG1 };
    bufPtr regs[2] = { d->BG0Registry[layer], d->BG1Registry[layer] };
    bufPtr belows[2] = { doPtr->below0, doPtr->below1 };
    bufPtr chrs[2] = { doPtr->chr0, doPtr->chr1 };
    
    bufPtr reg = 0;

    for(bg = 0; bg < 2; bg++)
    {
        if( !isOnBGs[bg])
            continue;

        for(j = 0; j < (h * v); j += 2)
        {  
            i = doToWin(doPtr, j);

            reg = regs[bg];
                
                // Check if anything's registered already at this coordinate
            if( RegIsPermitted(reg, doPtr, i) )
            {
                regValue = 0x8000 | (doPtr->index & 0x3FFF);
                hasActualTile = Get2Bytes(belows[bg], j) ? true : false;
                doType = doPtr->type == 1 ? 0 : 0x4000;
                regValue |= doType;

                if(DrawIsPermitted(d, doPtr, bg, i) && hasActualTile)
                {
                    value = Get2Bytes(reg, i);
                    value = (value) & 0x8000 ? value : regValue;
                    Put2Bytes(belows[bg], j, value);
                    Put2Bytes(reg, i, regValue);

                    value = Get2Bytes(chrs[bg], j);
                    Put2Bytes(game->BG_Map[bg], i , value);
                }
            } 

        }// end of inner for loop    
        
    }// end of outer for loop

    return;
}

bool RegIsPermitted(bufPtr registry, DngObj *doPtr, u32 i)
{
    bool regSuccess = false;
    u32 value = 0;
    u32 targType = 0;

    value = Get2Bytes(registry, i);
    targType = (value & 0x4000) ? 2 : 1;

    if( !(value & 0x8000) )
        regSuccess = true;
    else if(doPtr->type >= targType)
    {
        if(doPtr->index >= (value & 0x3FFF))
            regSuccess = true;
    }
    else
        regSuccess = false;
 
    return regSuccess;
}

bool DrawIsPermitted(DngObjStruct *d, DngObj *doPtr, u32 bg, u32 i)
{
    u32 j = 0;
    u32 layer = doPtr->layer;
    u32 value = 0;

    bufPtr reg;

    for(j = layer + 1; j <= 3; j++)
    {
        // check layers above the current layer because they might have tiles already registerd and drawn :(
        reg = bg ? d->BG1Registry[j] : d->BG0Registry[j];

        value = Get2Bytes(reg, i);

        // if object is registered there and drawn, we can't draw there
        if( value & 0x8000)
            return (false);
        else
            continue;
    }

    return (true);
}

void DeleteDngObj(zgPtr game, DngObjList *doList, u32 index)
{
    u32 i = 0;
    u32 j = 0;
    u32 size = 0;
    u32 hasTile = 0;
    u32 targType = 0;
    u32 value = 0;
    
    DngObjStruct *d = game->dngObjs;
    DngObj *doPtr = GetDngObj(doList, index);
    DngObj *newDoPtr = 0;

    if(!doPtr)
        return;

    size = doPtr->height * (doPtr->width << 1);

    for(j = 0; j < size; j += 2)
    {
        i = doToWin(doPtr, j);

        if(doPtr->isOnBG0)
        {
            hasTile = Get2Bytes(doPtr->below0, j);
            
            if(hasTile)
                Put2Bytes(d->BG0Registry[1], i, 0);
        }

        if(doPtr->isOnBG1)
        {
            hasTile = Get2Bytes(doPtr->below1, j);

            if(!(hasTile & 0x8000))
                continue;

            targType = hasTile & 0x4000 ? 2 : 1;

            // In this case, nothing is below this object on this layer so we have to search deeper
            if( (targType == doPtr->type) && (doPtr->index == (hasTile & 0x3FFF) ) )
            {
                Put2Bytes(d->BG1Registry[1], i, 0);
                FindNewTile(game, doPtr, 1, i);
            }
            else
            {
             
                Put2Bytes(d->BG1Registry[1], i, hasTile);
                FindNewTile(game, doPtr, 1, i);         

            }

        }




    }// end of for loop


    
}

u32 doToWin(DngObj *doPtr, u32 coord)
{
    //input is a (word aligned) position in a dngObj's internal buffer
    //output is the coordinate in terms of the tilemap

    u32 x = coord % (doPtr->width << 1);
    u32 y = (coord / (doPtr->width << 1)) << 7; // the << 7 is to multiply by 0x80

    return (x + y + doPtr->doMin);
}

u32 WinToDo(DngObj *doPtr, u32 coord)
{
    //in put is coordinate in terms of the tile map
    // output is the (word aligned) position in a dngObj's internal buffer
    
    // tm stands for tilemap
    u32 tm_x = coord % 0x80;
    u32 tm_y = (coord / 0x80);

    //dn stands for doPtr->doMin
    u32 dn_x = doPtr->doMin % 0x80;
    u32 dn_y = doPtr->doMin / 0x80;

    //dx stands for doPtr->doMax
    u32 dx_x = doPtr->doMax % 0x80;
    u32 dx_y = doPtr->doMax / 0x80;

    if(tm_x >= dn_x && tm_x <= dx_x)
        if(tm_y >= dn_y && tm_y <= dx_y)
        {
            tm_x -= dn_x;
            tm_y -= dn_y;

            return (tm_x + (tm_y * doPtr->width << 1));

        }

    // error return value;
    return -1;
}

void FindNewTile(zgPtr game, DngObj *doPtr, u32 bg, u32 i)
{
    int j = 0;
    u32 layer = doPtr->layer;
    u32 value = 0;
    u32 type = 0;

    DngObj *newDoPtr = 0;
    DngObjStruct *d = game->dngObjs;

    bufPtr reg;

    for(j = layer - 1; j >= 0; j--)
    {
        // check layers above the current layer because they might have tiles already registerd and drawn :(
        reg = bg ? d->BG1Registry[j] : d->BG0Registry[j];

        value = Get2Bytes(reg, i);
     
        if( value & 0x8000)
        {
            newDoPtr = GetDngObj(d, value, j);            

            if(!newDoPtr)
                continue;
                
            value = GetFloorTile(game, bg, i);
            //value = GetDoChrVal(newDoPtr, bg, i);
            Put2Bytes(game->BG_Map[bg], i, value);    
           
        }
    }

    //make it so we draw a floor tile from the appropriate layer

    return;
}

DngObjList* GetDoList(DngObjStruct *d, u32 type, u32 layer)
{
    int a = (int) layer;
    DngObjList *doList = 0;

    if(!d)
        return 0;

    if(type != 1 && type != 2)
        return 0;

    if( a < 0 || a > 3)
        return 0;

    if(type == 1)
        doList = & d->type1[layer];
    else
        doList= &d->type2[layer];

    return doList;            
}

bufPtr GetDoChr(DngObj *doPtr, u32 which)
{
    return (which ? doPtr->chr1 : doPtr->chr0);
}

u32 GetDoChrVal(DngObj *doPtr, u32 which, u32 tmCoord)
{
    u32 j = WinToDo(doPtr, tmCoord);

    if(j == -1)
        return 0;
    
    bufPtr chr = GetDoChr(doPtr, which);

    return Get2Bytes(chr, j);
}

u32 GetFloorTile(zgPtr game, u32 bg, u32 coord)
{    
    // ensuring safe input
    bg &= 0x01;
    coord &= 0x1FFF;

    // We want X to range from 0 to 6 and always be even, and we want y to be 0 or 0x08
    // x %= 0x80 (I think this is the same)
    u32 x = coord & 0x7; 
    
    // divide by 0x80 and determine if even or odd
    u32 y = (coord & 0x80) >> 4;

    u32 index = game->dngObjs->floor[bg] + x + y;
    
    return Get2Bytes(game->image, 0x1B52 + index);
}

//******************************************

void SyncGUI(HWND dlg, SprStruct *spr)
{
    u32 i = 0;

    bufPtr digits = CreateBuffer(0x08);

    HWND keyCombo = GetDlgItem(dlg, IDC_DROPITEM);
    HWND sprCombo = GetDlgItem(dlg, IDC_SPRITENAME);
    HWND subTypeEdit = GetDlgItem(dlg, IDE_SUBTYPE);

    if(!spr)
        return;

    if(!spr->flag)
    {
        // clear the combo box
        SendMessage(sprCombo, CB_RESETCONTENT, 0, 0);

        // add in all the necessary strings
        for(i = 0; strlen(SprStrings[i]) > 0; i++)
            SendMessage(sprCombo, CB_ADDSTRING, 0, (LPARAM) SprStrings[i]);

        // Make the checkbox reflect the sprite's state
        CheckDlgButton(dlg, IDC_OVERLORD, BST_UNCHECKED);
    
        EnableWindow(keyCombo, TRUE);
        EnableWindow(subTypeEdit, TRUE);

        SendMessage(GetDlgItem(dlg, IDC_DROPITEM), CB_SETCURSEL, (WPARAM) spr->key % 3, 0);
        SendMessage(sprCombo, CB_SETCURSEL, (WPARAM) spr->type % 0xF3, 0);

        makeHexStr(digits, spr->subType);
        SetDlgItemText(dlg, IDE_SUBTYPE, (LPCSTR) digits->contents);
    }
    else
    {
        SendMessage(sprCombo, CB_RESETCONTENT, 0, 0);

        for(i = 0; strlen(OverStrings[i]) > 0; i++)
            SendMessage(sprCombo, CB_ADDSTRING, 0, (LPARAM) OverStrings[i]);
        
        CheckDlgButton(dlg, IDC_OVERLORD, BST_CHECKED);
        
        EnableWindow(keyCombo, FALSE);
        EnableWindow(subTypeEdit, FALSE);

        SendMessage(sprCombo, CB_SETCURSEL, (WPARAM) spr->type % 0x1B, 0);
    }    

    makeHexStr(digits, spr->type);
    SetDlgItemText(dlg, IDE_TYPE, (LPCSTR) digits->contents);

    CheckRadioButton(dlg, IDR_FLOOR1, IDR_FLOOR2, spr->floor ? IDR_FLOOR2 : IDR_FLOOR1);

    return;
}

//******************************************

int CALLBACK toolDlgProc(HWND dlg, UINT msg, WPARAM wp, LPARAM lp)
{
    HWND keyCombo = GetDlgItem(dlg, IDC_DROPITEM);
    HWND sprCombo = GetDlgItem(dlg, IDC_SPRITENAME);
    HBITMAP sprBM = NULL;
    HBITMAP oldBM = NULL;
    HDC dlgDC = NULL;
    HDC memDC = NULL;

    zgPtr game = (zgPtr) GetWindowLong(GetParent(dlg), 0);

    SprStruct *spr = (SprStruct*) GetWindowLong(dlg, DWL_USER);

    u32 a = 0, i = 0;
    char dummy[0x10];
    bufPtr digits = NULL;

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

            u32 x = (u32) wp << 3;
            u32 y = (u32) lp << 3;
            x %= 0x200; y %= 0x200; // boundary checking

            spr->x = x & 0x1F0;
            spr->y = y & 0x1F0;       
        }
 
            break;

        case SPRITE_STOPPED:

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

                wp = wp;

                break;

            case CBN_SELCHANGE:

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

            case 3:

                break;

            default:

                wp = wp;
                lp = lp;

                break;
            }

            break;

        case WM_SETFONT:

            return 0;

            break;
    }

    return FALSE;
}

//**********************************************

SprStruct* SpriteClick(DngObjStruct *d, u32 x, u32 y)
{
    u32 i = 0;

    u32 *s = &d->sprIndices[0];
    SprStruct *spr = &d->sprites[0];

    for(i = 16; i != -1; i--)
    {
        if( s[i] == -1 )
            continue;

        if( spr[i].x == (x & 0x1F0) )
            if( spr[i].y == (y & 0x1F0) )
                return &spr[i];
    }

    s = &d->overIndices[0];
    spr = &d->overlords[0];

    for(i = 8; i != -1; i--)
    {
        if( s[i] == -1 )
            continue;

        if( spr[i].x == (x & 0x1F0) )
            if( spr[i].y == (y & 0x1F0) )
                return &spr[i];
    }

    return NULL;
}

//******************************************

    SprStruct* DngSprContextMenu(HWND hwnd, zgPtr game, u32 input, u32 x, u32 y)
    {
        SprStruct *spr  = (SprStruct*) GetWindowLong(game->toolDlg, DWL_USER);
        SprStruct *s    = &game->dngObjs->sprites[0];
        u32 *indicS     = &game->dngObjs->sprIndices[0];
        u32 *numS       = &game->dngObjs->numSprites;
    
        u32 i = 0, newIndex = -1;

        switch(input)
        {
        case ID_ADDSPRITE:

            for(i = 0; i < 16; i++)
            {
                if(indicS[i] == -1)
                {
                    newIndex = i;
                    break;
                }
            }

            if(newIndex == -1)
            {
                MessageBox(hwnd, "Only 16 sprites per room are allowed.", "sorry", MB_OK);
                return NULL;
            }
            else
            {
                indicS[i] = newIndex;
                s[i].x = x & 0x1F0; s[i].y = y & 0x1F0;
                s[i].floor = 0; s[i].key = 0;
                s[i].subType = 0; s[i].type = 0;
                s[i].selected = 0; s[i].flag = 0;

                (*numS)++;
            }

            return &s[i];

            break;

        case ID_ADDOVERLORD:

            s = &game->dngObjs->overlords[0];
            indicS = &game->dngObjs->overIndices[0];
            numS = &game->dngObjs->numOverlords;

            for(i = 0; i < 8; i++)
            {
                if(indicS[i] == -1)
                {
                    newIndex = i;
                    break;
                }
            }

            if(newIndex == -1)
            {
                MessageBox(hwnd, "Only 8 overlords per room are allowed.", "sorry", MB_OK);
                return NULL;
            }
            else
            {
                indicS[i] = newIndex;
                s[i].x = x & 0x1F0; s[i].y = y & 0x1F0;
                s[i].floor = 0; s[i].key = 0;
                s[i].subType = 0; s[i].type = 0;
                s[i].selected = 0; s[i].flag = 1;

                (*numS)++;
            }

            return &s[i];

            break;

        case ID_DELETESPRITE:

            if(!spr)
                break;

            for(i = 0; i < 16; i++)
            {
                if(&s[indicS[i]] == spr)
                    break;
            }

            if(i == 16)
                goto delOverlord;

            indicS[i] = -1;

            if(*numS > 16)
            {
                MessageBox(hwnd, "Error in deleting sprite.\nCould not find sprite to delete!", "error", MB_OK);
                return NULL;
            }

            (*numS)--;
            SetWindowLong(game->toolDlg, DWL_USER, 0);

            return NULL;

            break;

        delOverlord:

            s = &game->dngObjs->overlords[0];
            indicS = &game->dngObjs->overIndices[0];
            numS = &game->dngObjs->numOverlords;

            for(i = 0; i < 8; i++)
            {
                if(&s[indicS[i]] == spr)
                    break;
            }

            if(i == 8)
                break;

            indicS[i] = -1;

            if(*numS > 8)
            {
                MessageBox(hwnd, "Error in deleting overlord.\nCould not find overlord to delete!", "error", MB_OK);
                break;
            }

            (*numS)--;
            SetWindowLong(game->toolDlg, DWL_USER, 0);

            break;

        case ID_CLONESPRITE:
 
            if(!spr)
                break;

            if(spr->flag)
                goto cloneOverlord;

            for(i = 0; i < 16; i++)
            {
                if(indicS[i] == -1)
                {
                    newIndex = i;
                    break;
                }
            }

            if(newIndex == -1)
            {
                MessageBox(hwnd, "Only 16 sprites per room are allowed.", "sorry", MB_OK);
                return NULL;
            }
            else
            {
                indicS[i] = newIndex;
                s[i].x = x & 0x1F0; s[i].y = y & 0x1F0;
                s[i].floor = spr->floor; s[i].key = spr->key;
                s[i].subType = spr->subType; s[i].type = spr->type;
                s[i].selected = 0; s[i].flag = spr->flag;

                (*numS)++;
            }

            return &s[i];

            break;

        cloneOverlord:

            s = &game->dngObjs->overlords[0];
            indicS = &game->dngObjs->overIndices[0];
            numS = &game->dngObjs->numOverlords;


            for(i = 0; i < 8; i++)
            {
                if(indicS[i] == -1)
                {
                    newIndex = i;
                    break;
                }
            }

            if(newIndex == -1)
            {
                MessageBox(hwnd, "Only 8 sprites per room are allowed.", "sorry", MB_OK);
                return NULL;
            }
            else
            {
                indicS[i] = newIndex;
                s[i].x = x & 0x1F0; s[i].y = y & 0x1F0;
                s[i].floor = spr->floor; s[i].key = spr->key;
                s[i].subType = spr->subType; s[i].type = spr->type;
                s[i].selected = 0; s[i].flag = spr->flag;

                (*numS)++;
            }

            return &s[i];

        default:

            return NULL;

            break;

        }

        return NULL;
    }

//*************************************************************
