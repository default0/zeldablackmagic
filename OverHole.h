
#include "List.h"

#ifndef ENTRANCE_H
    
    #define ENTRANCE_H

    // Entrance class definition

    class Entrance;
    class OverItem;
    class OverSpr;

// =========================================================================
    
    class MarkerList : public List, public Marker
    {
        public:

        operator Entrance*() { return (Entrance*) this; }
        operator OverItem*() { return (OverItem*) this; }
        operator OverSpr*()  { return (OverSpr*)  this; }

        static MarkerList* Select(MarkerList *list, u16 x, u16 y)
        {
            MarkerList *m = NULL;

            for(m = MarkerList::GetHead(list); m; m = m->GetNext() )
                Marker::Unselect( (Marker*) m );

            m = GetClicked(list, x, y);

            if(m)
                Marker::Select( (Marker*) m );

            return m;
            }

        // ********************************

        static MarkerList* GetSelected(MarkerList *m)
        {
            // ----------------------
            
            if(m)
            {
                for(m = MarkerList::GetHead(m); m != NULL; m = m->GetNext())
                    if(m->selected)
                        break;
            }

            return m;
        }

        // ********************************

        static MarkerList* GetDragging(MarkerList *list)
        {
            MarkerList *m = GetSelected(list);

            // -----------------------

            // marker must be selected in addition to being dragged.
            if(m)
            {
                if(m->dragging == false) 
                    m = NULL;
            }

            return m;
        }
    
    // ********************************

        static MarkerList* GetClicked(MarkerList *list, u16 x, u16 y)
        {
            MarkerList *m = NULL;
            
            // ------------------------

            for(m = MarkerList::GetTail(list); m != NULL; m = m->GetPrev())
            {
                if(m->IsClick(x, y))
                    break;
            }
            
            return m;
        }

    // ********************************
       
        MarkerList* GetAt(u16 x, u16 y)
        {
            MarkerList *m = NULL;
            
            // ------------------------

            for(m = MarkerList::GetHead(this); m != NULL; m = m->GetNext())
            {
                if(m->IsPos(x, y))
                    break;
            }
            
            return m;
        }
    
        // wrapper functions to get the most of List functionality
        MarkerList* GetNext() {   return (MarkerList*) this->next;     }
        MarkerList* GetPrev() {   return (MarkerList*) this->prev;     }
        static MarkerList* GetHead(MarkerList *exit) { return exit ? (MarkerList*) exit->head : exit;    }
        static MarkerList* GetTail(MarkerList *exit) { return exit ? (MarkerList*) exit->tail : exit;    }
        static MarkerList* Add(MarkerList *list, MarkerList *entry) { return (MarkerList*) List::Add(list, entry); }
    };

// =========================================================================

    class Entrance : public MarkerList
    {
        public:

            u16 area;
            u16 entrance;
            
        // operators 

            operator MarkerList*() { return (MarkerList*) this; }

        // ********************************

        public:

            // constructor
            Entrance(u16 x = 0, u16 y = 0, u16 entrance = 0, u16 area = 0)
            {
                this->entrance = entrance;
                this->grid     = 16;
                this->area     = area;

                SetPos(x, y);
            }

        // ********************************
    };

// =========================================================================

    typedef enum 
    {
        over_exit_down,
        over_exit_up,
        over_exit_wooden,
        over_exit_bombable,
        over_exit_sanctuary,
        over_exit_palace

    } over_exit_type;

    class OverExit : public List, public Marker
    {
        public:

            u8 area;
            u16 room;

            u16 doorType;

            Marker *door;

        // ********************************

        public:

            // constructor
            OverExit(u16 x, u16 y, u16 area, u16 room, over_exit_type type)
            {
                this->x     = x;
                this->y     = y;
                this->grid  = 1;
                this->room  = room;
                this->area  = area;
                this->doorType = type;

                door = new Marker(0, 0, 16);
    
                SetPos(x, y);
            }

        // ********************************

            void SetRect()
            {
                Marker::SetRect();

                r.right += 16;
            }

        // ********************************

            OverExit* GetExit(u32 index)
            {
                u32 i = 0;
                OverExit *exit = NULL;

                // ------------------------
                           
                for(exit = OverExit::GetHead(this); exit != NULL; exit = exit->GetNext(), i += 1)
                {
                    if(i == index)  
                        break;
                }

                return exit;    
            }

        // ********************************

            static OverExit* GetSelectedExit(OverExit *exit)
            {
                // ----------------------

                if(exit)
                {
                    for(exit = OverExit::GetHead(exit); exit != NULL; exit = exit->GetNext())
                        if(exit->selected)
                            break;
                }

                return exit;
            }
    
        // ********************************

            static OverExit* GetDraggingExit(OverExit *exit)
            {
                OverExit *h = GetSelectedExit(exit);

                // -----------------------

                // exit must be selected in addition to being dragged.
                if(h)
                {
                    if(h->dragging == false) 
                        h = NULL;
                }

                return h;
            }

        // ********************************

            OverExit* GetClicked(u16 x, u16 y)
            {
                OverExit *exit = NULL;
                
                // ------------------------

                for(exit = OverExit::GetTail(this); exit != NULL; exit = exit->GetPrev())
                {
                    if(exit->IsClick(x, y))
                        break;
                }
                
                return exit;
            }

        // ********************************

            OverExit* GetAt(u16 x, u16 y)
            {
                OverExit *exit = NULL;
                
                // ------------------------

                for(exit = OverExit::GetHead(this); exit != NULL; exit = exit->GetNext())
                {
                    if(exit->IsPos(x, y))
                        break;
                }
                
                return exit;
            }

            // wrapper functions to get the most of List functionality
            OverExit* GetNext() {   return (OverExit*) this->next;     }
            OverExit* GetPrev() {   return (OverExit*) this->prev;     }
            static OverExit* GetHead(OverExit *exit) { return exit ? (OverExit*) exit->head : exit;    }
            static OverExit* GetTail(OverExit *exit) { return exit ? (OverExit*) exit->tail : exit;    }
            static OverExit* Add(OverExit *list, OverExit *entry) { return (OverExit*) List::Add(list, entry); }
    };

// =========================================================================
    
    class OverSpr : public MarkerList
    {
    public:
        
        operator MarkerList*() { return (MarkerList*) this; }

        bool overlord;

        u8 exData1;
        u8 exData2;
        u8 exData3;

        u16 spriteNum;

        // ------------------

    public:

        OverSpr(u16 spriteNum = 0, u16 x = 0, u16 y = 0, bool overlord = false, u8 exData1 = 0, u8 exData2 = 0, u8 exData3 = 0)
        {
            this->overlord  = overlord;
            this->spriteNum = 0;
            this->exData1   = exData1;
            this->exData2   = exData2;
            this->exData3   = exData3;

            this->grid      = 16;

            SetPos(x, y);
            SetType(spriteNum);

        }

        bool SetType(u16 value)
        {
            bool retVal = false;

            if(value <= 0x00F3)
            {
                spriteNum = value;
                retVal    = true;
            }

            return retVal;
        }
    };
    
// =========================================================================
    
    class OverItem : public MarkerList
    {
    public:
        
        operator MarkerList*() { return (MarkerList*) this; }

        u8 exData1;
        u8 exData2;
        u8 exData3;

        u16 itemNum;

        // ------------------
        
    public:

        OverItem(u16 itemNum = 0, u16 x = 0, u16 y = 0, u8 exData1 = 0, u8 exData2 = 0, u8 exData3 = 0)
        {
            this->itemNum   = 0;

            this->exData1   = exData1;
            this->exData2   = exData2;
            this->exData3   = exData3;
    
            this->grid      = 16;

            SetPos(x, y);
            SetType(itemNum);
        }

        bool SetType(u16 value)
        {
            bool retVal = false;

            if(value < 16)
            {
                itemNum = value;
                retVal  = true;
            }

            switch(value)
            {
                case 0x80:
                case 0x82:
                case 0x84:
                case 0x86:
                case 0x88:

                    itemNum = value;
                    retVal  = true;

                    break;

                default:

                    break;
            }

            return retVal;
        }
    };
    
// =========================================================================

    class OverMisc : public MarkerList
    {
    public:

        u16 eventType;
        u16 eventData;

        operator MarkerList*() { return (MarkerList*) this; }

    public:



    };

// =========================================================================

#endif

    