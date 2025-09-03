#pragma once

#define TextNum *(int*)(0x07B553F4)
#define TextListColor (&*(int*)0x07B23BB4)
#define TextBold (&*(int*)0x07B52C30)
#define TextList(x) (char*)(0x07B22A50+(100*x))

void InitItemToolTip();
void RenderItemInfo(int sx,int sy,DWORD ip,bool sell,int type,bool box);
void RenderTipTextList(int x,int y,int count,int tab,int sort,int point,BOOL usebg);
void AddTipText(char* text,int color,int bold);
void AddTipText(char* text,int color,int bold,int slot);