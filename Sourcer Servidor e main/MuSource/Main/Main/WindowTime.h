#pragma once

#include "Protocol.h"

#define WINDOW_POSX 450
#define WINDOW_POSY 0

struct PMSG_EVENT_TIME_LIST_RECV
{
	PSWMSG_HEAD header; // C2:F3:EF
	BYTE count;
};

struct EVENT_TIME_INFO
{
	char name[32];
	BYTE status;
	DWORD time;
};

enum eEventTimeState
{
	EVENT_STATE_BLANK = 0,
	EVENT_STATE_STAND = 1,
	EVENT_STATE_OPEN = 2,
	EVENT_STATE_START = 3,
};

void InitWindowTime();
int WindowGetScreenWidth();
void WindowTimeClose();
void WindowButtonToggle();
void DrawWindowTime();
void WindowRenderText(int x,int y,HFONT font,int align,int color,char* text,...);
int GetEventRenderColor(int status,int time);
char* GetEventRemainTime(int status,int time);
void GCEventTimeListRecv(PMSG_EVENT_TIME_LIST_RECV* lpMsg);