#include "Strings.h"
#include "Globals.h"

#ifndef MUSIC_H
#define MUSIC_H

#define headPC          0x0025
#define headA           0x0027
#define headX           0x0028
#define headY           0x0029
#define headPSW         0x002A
#define headSP          0x002B
#define headReserve     0x002C
#define headID666       0x002E // probably won't be using this

#define end_flag        0x0001
#define loop_flag       0x0002



void WriteSPCHeader(bufPtr header);
void ExportSpc(zgPtr game);

#endif