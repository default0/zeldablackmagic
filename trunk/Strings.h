// Declarations of functions and variables for String.cpp

//#include "Globals.cpp"

// #defines
#include "Globals.h"
#include "resource.h"

#ifndef STRINGS_H

    #define STRINGS_H

    // Variables

    // function prototypes *************************************

    // Strings.cpp functions

    bufPtr  CreateBuffer(int length);
    bufPtr  CreateBuffer(int width, int length, int unit);

    int     InitBuffer(int length, bufPtr bufArg);

    bufPtr  DuplicateBuffer(bufPtr bufArg);

    void    DestroyBuffer(bufPtr bufArg);
    int     ConcatBuffer(bufPtr first, bufPtr second);

    int     Get4Bytes(bufPtr source, unsigned int offset);
    int     Get4Bytes(bufPtr source, u32 x, u32 y);

    int     Get3Bytes(bufPtr source, int offset);
    int     Get3Bytes(bufPtr source, u32 x, u32 y);

    int     Get2Bytes(bufPtr source, int offset);
    int     Get2Bytes(bufPtr source, u32 x, u32 y);

    int     GetByte(bufPtr source, int offset);
    int     GetByte(bufPtr source, u32 x, u32 y);

    bufPtr  AllocBuffer(void);
    u32     DeallocBuffer(bufPtr);

    bool    FromFile(bufPtr target, char* pathName, u32 length = 0, u32 offset = 0);
    bool    ToFile(bufPtr source,   char* pathName, u32 length = 0, u32 offset = 0);

    bufPtr  GetSubBuffer(bufPtr source, u32 offset, u32 newLength = 0);
    u32     GetBit(bufPtr source, u32 bit);
    u32     PutBit(bufPtr target, u32 value, u32 bit);

    int     PutByte(bufPtr target, int value);
    int     PutByte(bufPtr target, int offset, int value);
    int     PutByte(bufPtr target, u32 x, u32 y, u32 value);

    u32     Put2Bytes(bufPtr target, u32 *offset, u32 value);
    u32     Put2Bytes(bufPtr target, u32 value);
    int     Put2Bytes(bufPtr target, int offset, int value);
    int     Put2Bytes(bufPtr target, u32 x, u32 y, u32 value);

    int     Put3Bytes(bufPtr target, int offset, int value);
    int     Put3Bytes(bufPtr target, u32 x, u32 y, u32 value);

    int     Put4Bytes(bufPtr target, int offset, int value);
    int     Put4Bytes(bufPtr target, u32 x, u32 y, u32 value);

    int     ResizeBuffer(bufPtr bufArg, int newSize);

    int     CopyBuffer(bufPtr target, bufPtr src, u32 numBytes);
    int     CopyBuffer(bufPtr target, bufPtr src, unsigned int tOffset, unsigned int sOffset, unsigned int numBytes);

    int     CopyBufferWB(bufPtr target, bufPtr src, unsigned int *tPtr, unsigned int *sPtr, unsigned int numBytes);

    int     ZeroBuffer(bufPtr arg);

    bool    CompareBuffer(bufPtr a, bufPtr b);

    bufPtr  FromString(char *contents, int length);
    bufPtr  FromString(char *contents, int width, int height, int unit);

    unsigned char* ToString(bufPtr input);

    unsigned int GetBank(unsigned int cpuAddr);
    int     IsBankChange(unsigned int cpuAddr, int size);



    // Black Magic.cpp routines

    void    MainLoop();

    void    RegisterBMClasses();

    void    MakeNewEditWindow();

    long _stdcall MainProc(HWND, UINT, WPARAM, LPARAM);

    int IsDuplicateRom(char compareBuf[MAX_PATH]);

    int GetFile(OPENFILENAME *ofn, HWND win);

    void MakeNewGamePtr();

    int SaveGame(HWND win);

    //void LoadDataFromRom();

    void DeletePointer();

    void DrawBitmap(HWND win);

    int CALLBACK ExpandProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    int DlgCommand(HWND dlg, WPARAM wParam, LPARAM lParam);

    void ExpandRom(int *newSize, int force);

    void OffsetWindowIds(WindowElement *we);

    void AssignCurrentWES(WindowElement *we);

    void PopulateCombobox(HWND combobox, char **stringList);

    int CALLBACK NewEditProc(HWND edit, UINT msg, WPARAM wParam, LPARAM lParam);

    void ActivateCurrentWES(WEStruct *currentWES, HWND parent);

    u32 hextodec(u8 *input, u32 length);

    void makeHexStr(bufPtr input, u32 value);

    int GetHexEditNum(HWND win);

    int dung(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    long _stdcall PictureProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    BOOL CALLBACK MoveChildren(HWND hwndChild, LPARAM lParam); 

#endif

