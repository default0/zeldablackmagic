
    #include "OverData.h"

// ===============================================================

    OverArea::OverArea()
    {

        




    }

// ===============================================================

    OverArea::~OverArea()
    {



    }

// ===============================================================

    bufPtr OverArea::LoadMap32(u32 mapNum)
    {
        u8 a         = 0;
        u8 b         = 0;

        u32 i        = 0;
        u32 tableLoc = 0;
        u32 offset   = 0;

        bufPtr data1 = NULL;
        bufPtr data2 = NULL;

        // map32 in Zelda should contain 0x200 bytes of data
        bufPtr map32Buf = CreateBuffer(0x10, 0x10, 2);

        // ---------------------------------------------

        tableLoc = CpuToRomAddr(Get3Bytes(this->rom, asm_overmap_ptrs));
        offset   = Get3Bytes(this->rom, tableLoc + (mapNum * 3));
        data1    = DecompressBank02(this->rom, CpuToRomAddr(offset) );

        tableLoc = CpuToRomAddr(Get3Bytes(this->rom, asm_overmap_ptrs2));
        offset   = Get3Bytes(this->rom, tableLoc + (mapNum * 3));
        data2    = DecompressBank02(this->rom, CpuToRomAddr(offset) );

        for(i = 0; i < 0x100; i++)
        {
            // notice how this data is interlaced from two separate sources
            // strange storage method... maybe it compresses better that way
            a = GetByte(data1, i);
            b = GetByte(data2, i);
            PutByte(map32Buf, (i*2)+1, a);
            PutByte(map32Buf, (i*2),   b);
        }

        DeallocBuffer(data1);
        DeallocBuffer(data2);

        // signal that there's a problem if the map32 data is not of the correct size
        if(map32Buf->length != 0x200)
            map32Buf->error = 1;

        return map32Buf;
    }

// ===============================================================

