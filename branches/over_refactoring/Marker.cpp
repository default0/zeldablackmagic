
    #include "Marker.h"

// ===============================================================

    Marker::Marker()
    {
        selected = false;
        dragging = false;
        
        strcpy(label, "durp"); // default

        grid = 1; // default is 1 x/y coordinate = 1 pixel

        SetPos(0, 0);
    }

// ===============================================================

    Marker::Marker(u16 x, u16 y, u16 grid)
    {
        this->selected = false;
        this->dragging = false;

        this->grid = grid;
        
        SetPos(x, y);
    }

// ===============================================================

    void Marker::SetPos(u16 x, u16 y)
    {
        // snaps the object's coordinates to a grid

        this->x = x - (x % this->grid);
        this->y = y - (y % this->grid);

        SetRect();
    }

// ===============================================================

    void Marker::SetRect()
    {
        r.left      = this->x;
        r.right     = this->x + 15;
        r.top       = this->y;
        r.bottom    = this->y + 15; 
    }

// ===============================================================
    
    // returns true only if the inputted coordinates
    // would resolve to the same position as the Marker's.

    bool Marker::IsPos(u16 x, u16 y)
    {
        if(this->y == (y - (y % this->grid)))
            if(this->x == (x - (x % this->grid)))
                return true;

        return false;
    }

// ===============================================================

    bool Marker::IsClick(u16 x, u16 y)
    {
        POINT pt = { x, y };

        return PtInRect( (const RECT*) this->GetRect(), pt) == TRUE ? true : false;
    }

// ===============================================================

    RECT* Marker::GetRect()
    {
        return &this->r;
    }
    
// ===============================================================

    bool Marker::Select(Marker *m)
    {
        if(m)
        {
            m->selected = true;
            m->dragging = true;

            return true;
        }
        else
            return false;
    }            
    
// ===============================================================

    bool Marker::Unselect(Marker *m)
    {
        if(m)
        {
            m->selected = false;
            m->dragging = false;

            return true;
        }
        else
            return false;
    }            
            
// ===============================================================
