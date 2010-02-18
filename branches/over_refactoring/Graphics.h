
    #include "Strings.h"

#ifndef GRAPHICS_H

    #define GRAPHICS_H

    // structure types
    typedef struct
    {
        u32 stride;
        u32 bottom;

        bufPtr memory;
        bufPtr pixels;
        BITMAPINFO* structure;
        BITMAPINFOHEADER* header;

    } ManagedBitmap;

#endif