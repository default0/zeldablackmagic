    #include "stdio.h"
    #include "string.h"
    #include "PathName.h"

// ==========================================================

    #define GetLastChar(x)      (x) ? (strchr((x), '\0')) : NULL
    #define NullTerminate(x)    if(x) { x[i] = '\0'; }

// ==========================================================

    PathName::PathName(const char *pathName)
    {
        // Initialize all the internal settings first
        Init(pathName);
    }

// ==========================================================

    void PathName::Init(const char *pathName)
    {
        char *pos = (char*) pathName;

        // -------------------------
        
        // initialize basic flags and settings
        unixStyle = ((pathName[0]) == '/');

        // windows networking shares start with two backslashes
        // and function similarly to a drive name on Linux.
        onNetwork = (strstr(pathName, "\\\\") == pos);

        separator = unixStyle ? '/' : '\\';
        
        // initialize all the component strings
        strcpy(drive, ""); 
        strcpy(dir, "");
        strcpy(name, "");
        strcpy(ext, "");
        
        pos = ParseDrive(pos);
        pos = ParseDir(pos);
        pos = ParseName(pos);
        
        MakePath();
    }
    
// ==========================================================
    
    char* PathName::ParseDrive(char *pathName)
    {
        char *endPoint = NULL;
        char *pos      = pathName;
        char *lastChar = GetLastChar(pathName);

        unsigned int i = 0;

        // ---------------------------
        
        if(!pathName) { return NULL; }
        
        // figure out whether it contains a drive name
        // unix style drives are of the form "/drivename/"
        if(unixStyle)
            pos += 1;
        else if(onNetwork)
            pos += 2;
    
        endPoint = strchr(pos, separator);

        // we found a drive name
        if(endPoint && (endPoint < lastChar) )
        {
            for(i = 0; (pos + i) < endPoint; ++i)
                drive[i] = pos[i];
             
            NullTerminate(drive);

            pos = ++endPoint;
        }
        else if(unixStyle)
        {
            // This happens when one finds the root of the unix drive but with
            // nothing else after it.
            for(i = 1; (pos + i) < lastChar; ++i)
                drive[i] = pos[i];
        
            NullTerminate(drive);
            
            pos = NULL;
        }

        // When there's no drive specified at all, we'll just assume that there's a
        // directory + filename + extension but no drive expressed.
    
        return pos;
    }
    
// ==========================================================
    
    char* PathName::ParseDir(char *pathName)
    {
        char *endPoint  = pathName;
        char *pos       = pathName;
        char *temp      = pathName;
        char *lastChar  = GetLastChar(pathName);
 
        unsigned int i = 0;

        // ---------------------------

        if(!pathName) { return NULL; }
        
        // next, we'll locate the last separator in the path
        temp     = pos;
        
        // advance to the last separator character
        while(temp && (endPoint < lastChar) )
        {
            temp = strchr(endPoint + 1, separator);
            
            if(temp) { endPoint = temp; }
        }
        
        if( (endPoint) && (endPoint > pos) && (endPoint < lastChar))
        {
            // if true, it means there's an intermediate directory name
            for(i = 0; (pos + i) < endPoint; i++)
                dir[i] = pos[i];
            
            NullTerminate(dir);

            pos = ++endPoint;
        }
        else if(endPoint)
        {
            pos = NULL;
        }
        
        return pos;
    }
    
    char* PathName::ParseName(char *pathName)
    {          
        char *endPoint  = pathName;
        char *pos       = pathName;
        char *temp      = pathName;
        char *lastChar  = GetLastChar(pathName);
 
        unsigned int i = 0;

        // ---------------------------

        if(!pathName) { return NULL; }

        while(temp && (endPoint < lastChar))
        {
            temp = strchr(endPoint + 1, '.');

            if(temp) { endPoint = temp; }
        }

        if( (endPoint > pos) && (endPoint < lastChar))
        {
            for(i = 0; (pos + i) < endPoint; ++i)
                name[i] = pos[i];
                
            NullTerminate(name);
        
            pos = ++endPoint;
        }
        else if(endPoint == pos)
        {
            // in this case there is no extension
            for(i = 0; (pos + i) < lastChar; i++)
                name[i] = pos[i];

            name[i] = '\0';

            return NULL;
        }
        
        // the remaining characters just get dumped as the extension
        for(i = 0; pos + i < lastChar; i++)
            ext[i] = pos[i];
        
        NullTerminate(ext);

        return NULL;
    }

// ==========================================================
    
    void PathName::SetNetwork(bool onNetwork)
    {
        this->onNetwork = onNetwork;

        MakePath();
    }

    void PathName::SetUnix(bool onUnix)
    {
        this->unixStyle = onUnix;

        MakePath();
    }

    void PathName::SetSeparator(char newSeparator)
    {
        this->separator = newSeparator;

        MakePath();
    }

// ==========================================================
    
    char* PathName::GetDrive()
    {
        return drive;
    }

    void PathName::SetDrive(const char *newDrive)
    {
        strcpy(drive, newDrive);

        MakePath();
    }

// ==========================================================
    
    char* PathName::GetDir()
    {
        return dir;            
    }

    void PathName::SetDir(const char *newDir)
    {
        strcpy(dir, newDir);

        MakePath();
    }

// ==========================================================
    
    char* PathName::GetName()
    {
        return name;
    }

    void PathName::SetName(const char *newName)
    {
        strcpy(name, newName);

        MakePath();
    }

// ==========================================================
    
    char* PathName::GetExt()
    {
        return ext;
    }

    void PathName::SetExt(const char *newExt)
    {
        strcpy(ext, newExt);

        MakePath();
    }

// ==========================================================
    
    void PathName::MakePath()
    {
        char c = separator;

        if(unixStyle)
            sprintf(fullPath, "%c%s%c%s%c%s.%s", c, drive, c, dir, c, name, ext);
        else if(onNetwork)
            sprintf(fullPath, "%c%c%s%c%s%c%s.%s", c, c, drive, c, dir, c, name, ext);
        else
            sprintf(fullPath, "%s%c%s%c%s.%s", drive, c, dir, c, name, ext);
    }
    
// ==========================================================
    
    char* PathName::GetFullPath()
    {
        MakePath();
    
        return fullPath;
    }

// ==========================================================
    
    PathName* PathName::Duplicate()
    {
        return new PathName( this->GetFullPath() );
    }

// ==========================================================
