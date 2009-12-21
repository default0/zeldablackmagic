
    #include "Globals.h"

// A linked list class

#ifndef LIST_H
    
    #define LIST_H

    bool ListUnitTest();

    class List
    {
        protected:

            int  size;
            int *sizePtr;

        public:
        
            List *prev;
            List *next;
            List *head;
            List *tail;

        public:

            List();
            ~List();

            void    Init();
            void    SetSize();
            u32     GetSize();
           
            static  List*   Add(List *list, List *entry);
            virtual List*   Insert(u32 index);
            virtual List*   Extract(List **extracted = NULL);
            virtual List*   Delete(List **s = NULL);
    };

#endif