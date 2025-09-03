#include "stdafx.h"
#include "Monster.h"
#include "CustomMonster.h"
#include "Offset.h"
#include "Util.h"

void InitMonster() // OK
{
	SetByte(0x004D8BFD,0xFF); // Monster Kill

	SetByte(0x004D8BFE,0xFF); // Monster Kill

	SetCompleteHook(0xE8,0x0048A316,&CreateMonster);
	
	SetCompleteHook(0xE8,0x004FCC07,&CreateMonster);
	
	SetCompleteHook(0xE8,0x00507660,&CreateMonster);
	
	SetCompleteHook(0xE8,0x00577BE1,&CreateMonster);
	
	SetCompleteHook(0xE8,0x00605890,&CreateMonster);
	
	SetCompleteHook(0xE8,0x00606165,&CreateMonster);
	
	SetCompleteHook(0xE8,0x00606570,&CreateMonster);
	
	SetCompleteHook(0xE8,0x006295B0,&CreateMonster);
	
	SetCompleteHook(0xE8,0x00794673,&CreateMonster);
	
	SetCompleteHook(0xE8,0x0079468D,&CreateMonster);
	
	SetCompleteHook(0xE8,0x007946A7,&CreateMonster);
	
	SetCompleteHook(0xE8,0x007946C1,&CreateMonster);
}

DWORD CreateMonster(int index,int x,int y,int key) // OK
{
	CUSTOM_MONSTER_INFO* lpInfo = gCustomMonster.GetInfoByIndex(index);

	if(lpInfo != 0)
	{
		index += MONSTER_BASE_MODEL;

		DWORD o = *(DWORD*)0x058E47B4 + 244 * index; // OK

		if(lpInfo->Type == 0 || lpInfo->Type == 2)
		{
			if(*(short*)(o + 38) <= 0)
			{
				char path[MAX_PATH] = {0};

				wsprintf(path,"Data\\%s",lpInfo->FolderPath);

				pLoadItemModel(index,path,lpInfo->ModelName,-1);

				for(int n = 0;n < *(short*)(o + 38);++n)
				{
					*(float*)(*(DWORD*)(o + 48) + 16 * n + 4) = 0.25f;
				}
			}
		}
		else
		{
			if(*(short*)(o + 36) <= 0  && *(short*)(o + 38) <= 0)
			{
				char path[MAX_PATH] = {0};

				wsprintf(path,"Data\\%s",lpInfo->FolderPath);

				pLoadItemModel(index,path,lpInfo->ModelName,-1);

				if(*(short*)(o + 36) > 0)
				{
					*(float*)(*(DWORD*)(o + 48) + 4) = 0.25f;
					*(float*)(*(DWORD*)(o + 48) + 20) = 0.2f;
					*(float*)(*(DWORD*)(o + 48) + 36) = 0.34f;
					*(float*)(*(DWORD*)(o + 48) + 52) = 0.33f;
					*(float*)(*(DWORD*)(o + 48) + 68) = 0.33f;
					*(float*)(*(DWORD*)(o + 48) + 84) = 0.5f;
					*(float*)(*(DWORD*)(o + 48) + 100) = 0.55f;
					*(bool*)(*(DWORD*)(o + 48) + 96) = true;
				}
			}
		}

		if(*(short*)(o + 36) > 0)
		{
			pLoadItemTexture(index,lpInfo->FolderPath,GL_REPEAT,GL_NEAREST,GL_TRUE);
		}

		DWORD pCharacter = ((DWORD(*)(int,int,BYTE,BYTE,float))0x004F6397)(key,((lpInfo->Type>1)?811:index),x,y,0.0f);

		if(pCharacter)
		{
			memcpy((DWORD*)(pCharacter + 56),lpInfo->Name,sizeof(lpInfo->Name));

			*(DWORD*)(pCharacter + 92) = ((lpInfo->Effect!=-1)?lpInfo->Effect:index);

			*(BYTE*)(pCharacter + 634) = ((lpInfo->Type == 0 || lpInfo->Type == 2)?4:2);

			*(float*)(pCharacter + 708) = lpInfo->Scale;

			*(BYTE*)(pCharacter + 632) = 0;

			*(short*)(pCharacter + 88) = *(short*)(0x07FE8DD4);

			if(lpInfo->Type >= 2)
			{
				*(BYTE*)(pCharacter + 19) = lpInfo->NpcClass;

				*(BYTE*)(pCharacter + 31) = lpInfo->PKLevel;

				for(int n=0;n < 5;n++)
				{
					*(WORD*)(pCharacter + 224 + 32 * n) = (lpInfo->NpcClass & 7) + 9021 + 8 * n;
				}

				for(int n=0;n < MAX_SKIN_SLOT;n++)
				{
					if(lpInfo->SkinInfo[n].Use == true)
					{
						*(WORD*)(pCharacter + 224 + 32 * n) = lpInfo->SkinInfo[n].ItemIndex+ITEM_BASE_MODEL;
						*(BYTE*)(pCharacter + 226 + 32 * n) = lpInfo->SkinInfo[n].ItemLevel;
						*(BYTE*)(pCharacter + 227 + 32 * n) = lpInfo->SkinInfo[n].SetOption;
						*(BYTE*)(pCharacter + 228 + 32 * n) = lpInfo->SkinInfo[n].NewOption;
					}
				}

				((void(*)(DWORD))0x004F64F0)(pCharacter);
			}

			return pCharacter;
		}
	}

	return ((DWORD(*)(int,int,int,int))0x004F7F80)(index,x,y,key);
}