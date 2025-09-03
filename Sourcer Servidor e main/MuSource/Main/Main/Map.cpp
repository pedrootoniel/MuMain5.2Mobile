#include "stdafx.h"
#include "Map.h"
#include "CustomMap.h"
#include "Offset.h"
#include "Util.h"

void InitMap()  // OK
{
	SetByte(0x005D7764,0xEB); // Fix Check .map files

	SetByte(0x005D776A,0xEB); // Fix Check .map files

	SetByte(0x005D79BB,0xEB); // Fix Check .att files

	SetByte(0x005D79C1,0xEB); // Fix Check .att files

	SetByte(0x005D7A5E,0xEB); // Fix Check .obj files

	SetByte(0x005D7A64,0xEB); // Fix Check .obj files

	SetByte(0x005D7763,0x69); // Increase terrain

	SetByte(0x005D79BA,0x69); // Increase terrain

	SetByte(0x005D7A5D,0x69); // Increase terrain

	SetCompleteHook(0xE8,0x006011C8,&LoadMapName);

	SetCompleteHook(0xE8,0x00604597,&LoadMapName);

	SetCompleteHook(0xE8,0x007420FC,&LoadMapName);

	SetCompleteHook(0xE8,0x0045DFB9,&LoadMapTitle);
}

char* LoadMapName(int index) //OK
{
	CUSTOM_MAP_INFO* lpInfo = gCustomMap.GetInfoByNumber(index);

	if(lpInfo != 0)
	{
		return lpInfo->MapName;
	}

	return ((char*(*)(int))0x00597780)(index);
}

int LoadMapTitle(char* path,int index,int c,int d,int e,int f) // OK
{
	CUSTOM_MAP_INFO* lpInfo = gCustomMap.GetInfoByNumber(*(DWORD*)MAIN_CURRENT_MAP);

	if(lpInfo != 0)
	{
		path = lpInfo->TitlePath;
	}

	return pLoadImage(path,index,c,d,e,f);
}