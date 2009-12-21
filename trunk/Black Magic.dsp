# Microsoft Developer Studio Project File - Name="Black Magic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Black Magic - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Black Magic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Black Magic.mak" CFG="Black Magic - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Black Magic - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Black Magic - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Black Magic - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\Compresch\src\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msimg32.lib re_lib_vs06.lib compresch_vs6.lib /nologo /subsystem:windows /map /machine:I386 /libpath:"../re_lib_vs06/release" /libpath:"..\Compresch_vs6\Release\\"

!ELSEIF  "$(CFG)" == "Black Magic - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\Compresch\src\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 comctl32.lib re_lib_vs06.lib msimg32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib compresch_vs6.lib /nologo /subsystem:windows /map /debug /machine:I386 /libpath:"../re_lib_vs06/debug" /libpath:"..\Compresch_vs6\Release\\"
# SUBTRACT LINK32 /profile /pdb:none /incremental:no /nodefaultlib /force

!ENDIF 

# Begin Target

# Name "Black Magic - Win32 Release"
# Name "Black Magic - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=".\Black Magic.cpp"

!IF  "$(CFG)" == "Black Magic - Win32 Release"

# ADD CPP /FR

!ELSEIF  "$(CFG)" == "Black Magic - Win32 Debug"

# ADD CPP /Ze /FR

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\constants.cpp
# End Source File
# Begin Source File

SOURCE=.\Globals.cpp
# End Source File
# Begin Source File

SOURCE=.\List.cpp
# End Source File
# Begin Source File

SOURCE=.\Marker.cpp
# End Source File
# Begin Source File

SOURCE=.\PathName.cpp
# End Source File
# Begin Source File

SOURCE=.\Strings.cpp

!IF  "$(CFG)" == "Black Magic - Win32 Release"

!ELSEIF  "$(CFG)" == "Black Magic - Win32 Debug"

# ADD CPP /Ze

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=".\Black Magic.h"
# End Source File
# Begin Source File

SOURCE=.\constants.h
# End Source File
# Begin Source File

SOURCE=.\Globals.h
# End Source File
# Begin Source File

SOURCE=.\List.h
# End Source File
# Begin Source File

SOURCE=.\Marker.h
# End Source File
# Begin Source File

SOURCE=.\PathName.h
# End Source File
# Begin Source File

SOURCE=.\Strings.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Group "Bitmaps"

# PROP Default_Filter "*.bmp;*.ico;"
# Begin Source File

SOURCE=.\bigkey1.bmp
# End Source File
# Begin Source File

SOURCE=.\bigkey2.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\bush.bmp
# End Source File
# Begin Source File

SOURCE=.\bush256.bmp
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\key1.bmp
# End Source File
# Begin Source File

SOURCE=.\key2.bmp
# End Source File
# Begin Source File

SOURCE=.\soldier.bmp
# End Source File
# Begin Source File

SOURCE=.\sprite_icon8.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\BM_resource.rc
# End Source File
# Begin Source File

SOURCE=.\planning.c

!IF  "$(CFG)" == "Black Magic - Win32 Release"

!ELSEIF  "$(CFG)" == "Black Magic - Win32 Debug"

# ADD CPP /Ze

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Overworld"

# PROP Default_Filter "*.*"
# Begin Source File

SOURCE=.\OverData.cpp
# End Source File
# Begin Source File

SOURCE=.\OverData.h
# End Source File
# Begin Source File

SOURCE=.\OverHole.cpp
# End Source File
# Begin Source File

SOURCE=.\OverHole.h
# End Source File
# Begin Source File

SOURCE=.\OverMode.cpp
# End Source File
# Begin Source File

SOURCE=.\OverMode.h
# End Source File
# Begin Source File

SOURCE=.\OverModeGui.cpp
# End Source File
# Begin Source File

SOURCE=.\OverModeGui.h
# End Source File
# Begin Source File

SOURCE=.\OverTypes.h
# End Source File
# End Group
# Begin Group "Dungeons"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DungMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DungMode.h
# End Source File
# Begin Source File

SOURCE=.\DungTypes.h
# End Source File
# Begin Source File

SOURCE=.\Objects.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects.h
# End Source File
# End Group
# Begin Group "Other Modes"

# PROP Default_Filter "*.cpp;*.h"
# Begin Source File

SOURCE=.\Music.cpp
# End Source File
# Begin Source File

SOURCE=.\Music.h
# End Source File
# Begin Source File

SOURCE=.\Sprites.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprites.h
# End Source File
# Begin Source File

SOURCE=.\TextMode.cpp
# End Source File
# End Group
# Begin Group "Graphics"

# PROP Default_Filter "*.c;*.cpp;*.h"
# Begin Source File

SOURCE=.\Core.cpp

!IF  "$(CFG)" == "Black Magic - Win32 Release"

!ELSEIF  "$(CFG)" == "Black Magic - Win32 Debug"

# ADD CPP /Ze

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Core.h
# End Source File
# Begin Source File

SOURCE=.\Graphics.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\hooks.asm
# End Source File
# End Target
# End Project
