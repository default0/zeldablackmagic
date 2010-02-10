
#ifndef PATH_NAME_H

    #define PATH_NAME_H

    class PathName
    {
        private:

            bool unixStyle;
            bool onNetwork;
        
            char separator;
        
            char drive[512];
            char dir[512];
            char name[512];
            char ext[512];
            char fullPath[513];
            
        public:
        
            PathName(const char *pathName);

            void Init(const char *pathName);

            char*   ParseDrive(char *pathName);
            char*   ParseDir(char *pathName);
            char*   ParseName(char *pathName);

            void    SetNetwork(bool onNetwork);
            void    SetUnix(bool onUnix);
            void    SetSeparator(char newSeparator);

            char*   GetDrive();
            void    SetDrive(const char *newDrive);

            char*   GetDir();
            void    SetDir(const char *newDir);

            char*   GetName();
            void    SetName(const char *newName);

            char*   GetExt();
            void    SetExt(const char *newExt);

            void    MakeDirectory();
            void    MakePath();
            
            char*   GetDirectory();
            char*   GetFullPath();

            PathName* Duplicate();
    };

#endif