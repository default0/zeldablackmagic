
    #include "List.h"

// ===============================================================

    List::List()
    {
        Init();
    }

// ===============================================================

    List::~List()
    {

    }

// ===============================================================
    
    void List::Init()
    {
        this->head = this;
        this->tail = this;

        this->prev = NULL;
        this->next = NULL;

        this->size = 1;
        this->sizePtr = &this->size;
    }

// ===============================================================

    void List::SetSize()
    {
        u32 size = 0;

        List *iter = this->head;

        this->head->sizePtr = &this->head->size;

        while(iter)
        {
            size += 1;



            iter->sizePtr = iter->head->sizePtr;
            iter = iter->next;
        }

        this->head->size = size;
    }

// ===============================================================

    u32 List::GetSize()
    {
        return *(this->head->sizePtr);
    }

// ===============================================================

    List* List::Add(List *list, List *entry)
    {
        List *t;

        if(!entry)
            return list;

        if(!list)
        {
            list = entry;
            return list;
        }

        entry->prev = list->tail;
        list->tail->next = entry;

        for(t = list->head; t != NULL; t = t->next)
        {
            t->head = list->head;
            t->tail = entry->tail;
        }

        list->SetSize();

        return list->head;
    }

// ===============================================================

    List* List::Insert(u32 index)
    {


        return this;
    }

// ===============================================================

    // Extracts a list entry from a linked list and patches up the existing linked list
    List* List::Extract(List **remainder)
    {   
        List *t         = NULL;
        List *t2        = NULL;

        if(this->GetSize() == 1)
        {
            (*remainder) = this;
            return this;
        }

        // reassign the head of the list if necessary
        if(this->head == this)
        {
            if(this->next)
            {
                for(t = this->next; t != NULL; t = t->next)
                    t->head = this->next;
            }
        }

        // reassign the tail for the remainder of the list
        // if we're at the end of the list
        if(this->tail == this)
        {
            if(this->prev)
            {
                for(t = this->prev; t != NULL; t = t->prev)
                    t->tail = this->prev;
            }
        }

        // close the gap in the linked list resulting from "this" leaving the list.
        if(this->prev)
        {
            this->prev->next = this->next;
            t2 = this->prev;
        }

        if(this->next)
        {
            this->next->prev = this->prev;
            t2 = this->next;
        }

        t2->SetSize();

        // Set "this"'s values to default for a single element list
        Init();

        if(remainder)
            (*remainder) = t2->head;

        return this;
    }

// ===============================================================

    List* List::Delete(List **s)
    {
        List *t = NULL;
        List *ret = NULL;

        if(!s)
            return NULL;

        if(*s)
        {    
            t = this->Extract(s);

            if(t == (*s))
                ret = NULL;
            else
                ret = (*s);

            delete t;
        }

        return ret;
    }

// ===============================================================

    bool ListUnitTest()
    {
        List *t = new List();
        List *s = NULL;
        List *u = NULL;

        List::Add(t, new List());
        List::Add(t, new List());
        List::Add(t, new List());

        t->Delete(&t);
        t->Delete(&t);
        t->Delete(&t);

        List::Add(t, new List());
        List::Add(t, new List());

        s = t->head->Extract();
        u = t->tail->Extract();

        return true;
    }

// ===============================================================