#include "stdafx.h"
#include "MiniMap.h"
#include "Offset.h"
#include "Util.h"

bool MiniMapTable[MAX_MINI_MAP];

void InitMiniMap() // OK
{
	memset(MiniMapTable,0,sizeof(MiniMapTable));

	MemorySet(0x0060139B,0x90,0x02);

	MemorySet(0x006C7F22,0x90,0x1E);

	SetCompleteHook(0xFF,0x005D768A,&MiniMapCore);

	SetCompleteHook(0xE8,0x006C7F22,&MiniMapLoad);

	SetCompleteHook(0xFF,0x006C5FCA,&MiniMapCheck);

	SetCompleteHook(0xFF,0x0071198A,&MiniMapCheck);
}

void MiniMapCore() // OK
{
	((void(*)())0x005D2F16)();

	MiniMapLoad();
}

void MiniMapLoad() // OK
{
	char buff[64];

	wsprintf(buff,"World%d\\Map1.jpg",(*(DWORD*)MAIN_CURRENT_MAP)+1);

	if(MiniMapFileCheck(*(DWORD*)MAIN_CURRENT_MAP) != 0)
	{
		MiniMapTable[*(DWORD*)MAIN_CURRENT_MAP]=1;
		pLoadImage(buff,0x7B69,GL_LINEAR,GL_CLAMP,0,1);
	}
}

bool MiniMapCheck(int map) // OK
{
	if(MiniMapTable[map] != 0)
	{
		return 1;
	}
	else
	{
		CloseWindow(INTERFACE_SIEGEWARFARE);
		return 0;
	}
}

bool MiniMapFileCheck(int map) // OK
{
	char buff[64];

	wsprintf(buff,".\\Data\\World%d\\Map1.ozj",(map+1));

	FILE* file;

	if(fopen_s(&file,buff,"r") != 0)
	{
		return 0;
	}
	else
	{
		fclose(file);
		return 1;
	}
}
