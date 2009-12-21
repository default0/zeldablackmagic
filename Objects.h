

#include "Globals.h"
#include "Strings.h"

#ifndef OBJECTS_H
#define OBJECTS_H

typedef void (*object)(zgPtr, DngObj*); 

extern object Type1_1Object[0xF8];
extern object Type1_2Object[0x40];
extern object Type1_3Object[0x80];
extern object Type2_Object[0x04];

void do_8A1F(zgPtr, DngObj*, u32);
void do_8A44(zgPtr, DngObj*, u32);
void do_8A89(zgPtr, DngObj*);
void do_8A92(zgPtr, DngObj*);
void do_free(zgPtr, DngObj*);
void do_8AA4(zgPtr, DngObj*);

void do_8B74(zgPtr, DngObj*);
void do_8B79(zgPtr, DngObj*);
void do_8B7E(zgPtr, DngObj*);
void do_8B0D(zgPtr, DngObj*);
void do_8B89(zgPtr, DngObj*);
void do_8B94(zgPtr, DngObj*);
void do_8BE0(zgPtr, DngObj*);
void do_8BF4(zgPtr, DngObj*);

void do_8C0E(zgPtr, DngObj*);
void do_8C22(zgPtr, DngObj*);
void do_8C4F(zgPtr, DngObj*);
void do_8C58(zgPtr, DngObj*);
void do_8C61(zgPtr, DngObj*);
void do_8C6A(zgPtr, DngObj*, u32);
void do_8C73(zgPtr, DngObj*);
void do_8CC7(zgPtr, DngObj*);

void do_8D47(zgPtr, DngObj*, u32);
void do_8D5D(zgPtr, DngObj*);
void do_8D80(zgPtr, DngObj*, u32);
void do_8D9E(zgPtr, DngObj*);
void do_8DDC(zgPtr, DngObj*);

void do_8E67(zgPtr, DngObj*);
void do_8E7B(zgPtr, DngObj*);
void do_8E95(zgPtr, DngObj*);
void do_8EA9(zgPtr, DngObj*);
void do_8EC3(zgPtr, DngObj*);
void do_8EF0(zgPtr, DngObj*);

void do_8F0C(zgPtr, DngObj*);
void do_8F30(zgPtr, DngObj*);
void do_8F36(zgPtr, DngObj*);
void do_8F62(zgPtr, DngObj*);
void do_8F8A(zgPtr, DngObj*);
void do_8F9D(zgPtr, DngObj*);
void do_8FA2(zgPtr, DngObj*);
void do_8FA5(zgPtr, DngObj*);
void do_8FBD(zgPtr, DngObj*);
void do_8FEC(zgPtr, DngObj*, u32);

void do_9001(zgPtr, DngObj*);
void do_9030(zgPtr, DngObj*, u32);
void do_9045(zgPtr, DngObj*);
void do_9078(zgPtr, DngObj*, u32);
void do_908F(zgPtr, DngObj*);
void do_90C2(zgPtr, DngObj*, u32);
void do_90E2(zgPtr, DngObj*);
void do_90D9(zgPtr, DngObj*);
void do_90F9(zgPtr, DngObj*);

void do_9111(zgPtr, DngObj*);
void do_9120(zgPtr, DngObj*);
void do_9136(zgPtr, DngObj*);

void do_92FB(zgPtr, DngObj*);

void do_930E(zgPtr, DngObj*);
void do_9323(zgPtr, DngObj*);
void do_9338(zgPtr, DngObj*);
void do_9347(zgPtr, DngObj*);
void do_9357(zgPtr, DngObj*);
void do_936F(zgPtr, DngObj*);
void do_9387(zgPtr, DngObj*);
void do_939F(zgPtr, DngObj*);
void do_93B7(zgPtr, DngObj*);
void do_93DC(zgPtr, DngObj*);
void do_93FF(zgPtr, DngObj*);

void do_9429(zgPtr, DngObj*);
void do_9446(zgPtr, DngObj*);
void do_9456(zgPtr, DngObj*);
void do_9466(zgPtr, DngObj*);
void do_9488(zgPtr, DngObj*);
void do_94B4(zgPtr, DngObj*);
void do_94BD(zgPtr, DngObj*);
void do_94DF(zgPtr, DngObj*);

void do_9501(zgPtr, DngObj*);
void do_95EF(zgPtr, DngObj*);

void do_96DC(zgPtr, DngObj*);
void do_96E4(zgPtr, DngObj*);
void do_96F9(zgPtr, DngObj*);

void do_9702(zgPtr, DngObj*);
void do_971B(zgPtr, DngObj*);
void do_9733(zgPtr, DngObj*);
void do_975C(zgPtr, DngObj*);
void do_97B5(zgPtr, DngObj*);
void do_97DC(zgPtr, DngObj*);
void do_97ED(zgPtr, DngObj*);
void do_97F0(zgPtr, DngObj*, u32);

void do_9813(zgPtr, DngObj*);
void do_9854(zgPtr, DngObj*);
void do_985C(zgPtr, DngObj*);
void do_9892(zgPtr, DngObj*);
void do_9895(zgPtr, DngObj*);
void do_98AE(zgPtr, DngObj*);
void do_98D0(zgPtr, DngObj*);

void do_99BB(zgPtr, DngObj*);
void do_99E6(zgPtr, DngObj*);
void do_99EC(zgPtr, DngObj*);

void do_9A00(zgPtr, DngObj*);
void do_9A06(zgPtr, DngObj*);
void do_9A0C(zgPtr, DngObj*);
void do_9A12(zgPtr, DngObj*);
void do_9A66(zgPtr, DngObj*);
void do_9A6F(zgPtr, DngObj*);
void do_9A90(zgPtr, DngObj*);
void do_9AA3(zgPtr, DngObj*);
void do_9AA9(zgPtr, DngObj*);
void do_9AEE(zgPtr, DngObj*);

void do_9B1E(zgPtr, DngObj*);
void do_9B48(zgPtr, DngObj*);
void do_9B50(zgPtr, DngObj*);
void do_9B56(zgPtr, DngObj*);
void do_9BF8(zgPtr, DngObj*);
void do_9BD9(zgPtr, DngObj*);

void do_9C3B(zgPtr, DngObj*);
void do_9C3E(zgPtr, DngObj*);
void do_9C44(zgPtr, DngObj*);
void do_9CC6(zgPtr, DngObj*);
void do_9CEB(zgPtr, DngObj*);

void do_9D04(zgPtr, DngObj*);
void do_9D29(zgPtr, DngObj*);
void do_9D5D(zgPtr, DngObj*);
void do_9D67(zgPtr, DngObj*);
void do_9D6C(zgPtr, DngObj*);
void do_9D6F(zgPtr, DngObj*, u32);
void do_9D96(zgPtr, DngObj*);
void do_9DA2(zgPtr, DngObj*, u32);
void do_9DD9(zgPtr, DngObj*);
void do_9DE5(zgPtr, DngObj*);

void do_9E06(zgPtr, DngObj*, u32);
void do_9E30(zgPtr, DngObj*);
void do_9EA3(zgPtr, DngObj*);

void do_A095(zgPtr, DngObj*);

void do_A194(zgPtr, DngObj*);
void do_A1D1(zgPtr, DngObj*);

void do_A255(zgPtr, DngObj*);
void do_A25D(zgPtr, DngObj*);
void do_A26D(zgPtr, DngObj*);
void do_A2C7(zgPtr, DngObj*);
void do_A2DF(zgPtr, DngObj*);

void do_A30C(zgPtr, DngObj*);
void do_A31C(zgPtr, DngObj*);
void do_A32A(zgPtr, DngObj*);
void do_A36E(zgPtr, DngObj*);
void do_A380(zgPtr, DngObj*);
void do_A3AE(zgPtr, DngObj*);

void do_A41B(zgPtr, DngObj*);
void do_A458(zgPtr, DngObj*);
void do_A486(zgPtr, DngObj*);
void do_A4B4(zgPtr, DngObj*);
void do_A4F5(zgPtr, DngObj*);
void do_A533(zgPtr, DngObj*);
void do_A584(zgPtr, DngObj*);
void do_A5D2(zgPtr, DngObj*);
void do_A5F4(zgPtr, DngObj*);
void do_A607(zgPtr, DngObj*);
void do_A626(zgPtr, DngObj*);
void do_A634(zgPtr, DngObj*);
void do_A664(zgPtr, DngObj*);
void do_A695(zgPtr, DngObj*);
void do_A6B5(zgPtr, DngObj*);
void do_A6EE(zgPtr, DngObj*);

void do_A71C(zgPtr, DngObj*);
void do_A74A(zgPtr, DngObj*);
void do_A767(zgPtr, DngObj*);
void do_A7A3(zgPtr, DngObj*);
void do_A7B6(zgPtr, DngObj*);
void do_A7D3(zgPtr, DngObj*);
void do_A7DC(zgPtr, DngObj*);
void do_A7F0(zgPtr, DngObj*);

void do_A809(zgPtr, DngObj*);
void do_A81C(zgPtr, DngObj*);
void do_A984(zgPtr, DngObj*);

void do_AAD7(zgPtr, DngObj*);
void do_AB99(zgPtr, DngObj*);

void do_AE40(zgPtr, DngObj*, u32*, u32*, u32*);

void do_B0AC(DngObj*);
void do_B0AF(DngObj*, u32);
void do_B0BE(DngObj*);
void do_B0CC(DngObj*);
bool do_B0DA(zgPtr, DngObj*, u32*, u32*, u32*);

void do_B1A4(zgPtr, DngObj*, u32*, u32*, u32*);

void do_B220(zgPtr, DngObj*, u32);
void do_B279(zgPtr, DngObj*);
void do_B29C(zgPtr, DngObj*);
void do_B2AA(zgPtr, DngObj*);
void do_B2AF(zgPtr, DngObj*);
void do_B2CA(zgPtr, DngObj*);
void do_B2CE(zgPtr, DngObj*);
void do_B2D0(zgPtr, DngObj*, u32);
void do_B2E1(zgPtr, DngObj*);
void do_B2F6(zgPtr, DngObj*);

void do_B306(zgPtr, DngObj*);
void do_B30B(zgPtr, DngObj*);
void do_B310(zgPtr, DngObj*);
void do_B33A(zgPtr, DngObj*, u32);
void do_B376(zgPtr, DngObj*);
void do_B381(zgPtr, DngObj*);
void do_B395(zgPtr, DngObj*);
void do_B3E1(zgPtr, DngObj*);

void do_B474(zgPtr, DngObj*);
void do_B47F(zgPtr, DngObj*);
void do_B493(zgPtr, DngObj*);

void sh_8C6A(zgPtr, DngObj*);
void sh_8CB9(zgPtr, DngObj*);
void sh_8EBE(zgPtr, DngObj*);
void sh_8EC3(zgPtr, DngObj*);
void sh_8EEB(zgPtr, DngObj*);
void sh_8EF0(zgPtr, DngObj*);
void sh_9895(zgPtr, DngObj*);
void sh_97ED(zgPtr, DngObj*);

void do_null(zgPtr, DngObj*);
void do_free(zgPtr, DngObj*);

void RegObjTile(zgPtr, DngObj*, u32, u32);
DngObj* GetDngObj(DngObjList*, u32);
DngObj* GetDngObj(DngObjStruct*, u32, u32, u32);
DngObjList* GetDoList(DngObjStruct *d, u32 type, u32 layer);

bool SearchForObj(DngObjStruct*, u32, u32, u32, u32 argNum = -1);

DngObjStruct* CreateDngObjStruct();
void InitDngObjStruct(DngObjStruct *doStruct);
void DiscardDngObjs(DngObjList* doList);
void RegDngObj(zgPtr game, DngObjStruct *d, DngObj *doPtr);
bool RegIsPermitted(bufPtr registry, DngObj *doPtr, u32 i);
bool DrawIsPermitted(DngObjStruct *d, DngObj *doPtr, u32 bg, u32 i);
void DeleteDngObj(zgPtr game, DngObjList *doList, u32 index);
void FindNewTile(zgPtr, DngObj*, u32, u32);
void LoadSprites(zgPtr);
SprStruct* SpriteClick(DngObjStruct *d, u32 x, u32 y);
int CALLBACK toolDlgProc(HWND, UINT, WPARAM, LPARAM);
SprStruct* DngSprContextMenu(HWND hwnd, zgPtr game, u32 input, u32 x, u32 y);

u32 doToWin(DngObj *doPtr, u32 coord);
u32 WinToDo(DngObj *doPtr, u32 coord);
u32 GetDoChrVal(DngObj *doPtr, u32 which, u32 tmCoord);
u32 GetFloorTile(zgPtr game, u32 bg, u32 coord);











#endif