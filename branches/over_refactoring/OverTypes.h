    
#ifndef OVERTYPES_H

    #define OVERTYPES_H

    typedef enum
    {
        fromMap8  = 0x08,
        fromMap16 = 0x10,
        fromMap32 = 0x20
    } objMapType;

    class OverObj
    {
    public:

        bool forming;
        bool active;
        bool dragging;
        
        u32 x;
        u32 y;
        
        u32 width;
        u32 height;
       
        u32 *mapData;

        int id;
        
        objMapType mapType;

        ManagedBitmap* bData;

        RECT r;

        POINT anchor;

        HBITMAP hData;

        // **************************************************
        
    public:

        OverObj(u32 *mapData = NULL, u32 width = 0, u32 height = 0, int id = NULL, objMapType mapType = fromMap8)
        {
            // parameters that default to basic null values.
            forming  = false;
            active   = false;
            dragging = false;

            x = 0;
            y = 0;
            
            anchor.x = anchor.y = 0;

            bData = NULL;
            hData = NULL;

            // parameters that can be set with the constructor
            this->width     = width;
            this->height    = height;
            this->mapData    = mapData;
            this->id         = id;
            this->mapType    = mapType;
        }

    // **************************************************

        void SetPos(u16 x, u16 y)
        {
            this->x = x & ~(mapType - 1);
            this->y = y & ~(mapType - 1);
        }

    // **************************************************

        RECT* GetRect()
        {
            r.left   = x;
            r.right  = x + width;
            r.top    = y;
            r.bottom = y + height;

            return &r;
        }

    // **************************************************

        bool HFlip()
        {
            u32 i = 0, j = 0;

            u32 dx = width / mapType, dy = height / mapType;

            u32 tileLeft = 0, tileRight = 0;

            // ----------------------------------

            if(mapType != fromMap8)
                return false;

            if(mapData)
            {
                for(i = 0; i < (dx / 2) + 1; ++i)
                {
                    if((dx % 2) == 0)
                        if(i >= (dx / 2))
                            continue;

                    for(j = 0; j < dy; ++j)
                    {
                        tileLeft  = mapData[i + (j * dx)];
                        tileRight = mapData[ (dx - 1 - i) + (j * dx) ];

                        tileLeft  ^= 0x4000;
                        tileRight ^= 0x4000;

                        mapData[i + (j * dx)]               = tileRight;
                        mapData[ (dx - 1 - i) + (j * dx) ]  = tileLeft;
                    }
                }

                return true;
            }

            return false;
        }

    // **************************************************

        bool VFlip()
        {
            u32 i = 0, j = 0;

            u32 dx = width / mapType, dy = height / mapType;

            u32 tileHigh = 0, tileLow = 0;

            // ----------------------------------

            if(mapType != fromMap8)
                return false;

            if(mapData)
            {
                for(i = 0; i < dx; ++i)
                {
                    for(j = 0; j < (dy / 2) + 1; ++j)
                    {
                        if((dy % 2) == 0)
                            if(j >= (dy / 2))
                                continue;

                        tileHigh  = mapData[i + (j * dx)];
                        tileLow   = mapData[i + ( (dy - 1 - j) * dx) ];

                        tileHigh  ^= 0x8000;
                        tileLow   ^= 0x8000;

                        mapData[i + (j * dx)]               = tileLow;
                        mapData[i + ( (dy - 1 - j) * dx) ]  = tileHigh;
                    }
                }

                return true;
            }

            return false;
        }

    // **************************************************

        bool SetPriority(bool high)
        {
            u16 tile = 0;

            u16 priority = high ? 0x2000 : 0x0000;

            u32 i = 0, j = 0;

            u32 dx = width / mapType, dy = height / mapType;

            // ----------------------------------

            if(mapType != fromMap8)
                return false;

            if(mapData)
            {
                for(i = 0; i < dx; ++i)
                {
                    for(j = 0; j < dy; ++j)
                    {
                        tile = mapData[i + (j * dx)];
                        tile = (tile & ~0x2000) | priority;
                        mapData[i + (j * dx)] = tile;
                    }
                }

                return true;
            }

            return false;
        }

    // **************************************************

        bool SetPalette(u16 palette)
        {
            u16 tile = 0;

            u16 mask = ~0x1C00;

            u32 i = 0, j = 0;

            u32 dx = width / mapType, dy = height / mapType;

            // ----------------------------------

            palette = (palette & 0x07) << 10;

            if(mapType != fromMap8)
                return false;

            if(mapData)
            {
                for(i = 0; i < dx; ++i)
                {
                    for(j = 0; j < dy; ++j)
                    {
                        tile = mapData[i + (j * dx)];
                        tile = (tile & mask) | palette;
                        mapData[i + (j * dx)] = tile;
                    }
                }

                return true;
            }

            return false;
        }


    };

#endif