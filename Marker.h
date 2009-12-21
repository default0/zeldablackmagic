
    #include "Globals.h"

#ifndef MARKER_H

    #define MARKER_H

    class Marker
    {
        public:

            bool selected;
            bool dragging;

            char label[0x50];

            u16 x;
            u16 y;

            u16 xAnchor;
            u16 yAnchor;

            u16 grid;

            RECT r;

        public:

            Marker();
            Marker(u16 x, u16 y, u16 grid);

            virtual void SetPos(u16 x, u16 y);
            virtual void SetRect();
            
            bool IsPos(u16 x, u16 y);
            bool IsClick(u16 x, u16 y);

            RECT* GetRect();

            static bool Select(Marker *m);
            static bool Unselect(Marker *m);
    };

#endif