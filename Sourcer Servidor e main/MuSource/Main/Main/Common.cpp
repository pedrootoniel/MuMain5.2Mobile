#include "stdafx.h"
#include "Attack.h"
#include "Common.h"
#include "Notice.h"
#include "Offset.h"
#include "Protect.h"
#include "Protocol.h"
#include "Util.h"
#include "WindowTime.h"

BYTE CastleSiegeMapUI = 0;
char WindowName[128];

void InitCommon() // OK
{
	SetCompleteHook(0xE8,0x005ECA1F,&CalcFPS);

	SetCompleteHook(0xE9,0x004036F3,&CharacterCreationLevel);

	SetCompleteHook(0xFF,0x004BDB34,&CheckMasterLevel); // C1:16

	SetCompleteHook(0xFF,0x004BDB95,&CheckMasterLevel); // C1:16

	SetCompleteHook(0xFF,0x0061FE6F,&CheckMasterLevel); // C1:48

	SetCompleteHook(0xFF,0x0060D415,&CheckMasterLevel); // C1:9C

	SetCompleteHook(0xFF,0x0060D4C7,&CheckMasterLevel); // C1:9C

	SetCompleteHook(0xFF,0x0060095B,&CheckMasterLevel); // C1:F3:03

	SetCompleteHook(0xFF,0x00600A59,&CheckMasterLevel); // C1:F3:03

	SetCompleteHook(0xFF,0x0060151F,&CheckMasterLevel); // C1:F3:04

	SetCompleteHook(0xFF,0x0060FF37,&CheckMasterLevel); // LevelUp

	SetCompleteHook(0xFF,0x006D5E30,&CheckMasterLevel); // Print Level

	SetCompleteHook(0xFF,0x0071ACEA,&CheckMasterLevel); // Experience Bar

	SetCompleteHook(0xFF,0x0071AD6F,&CheckMasterLevel); // Experience Bar

	SetCompleteHook(0xE9,0x005ED21F,&CheckTickCount);
}

void CalcFPS() // OK
{
	if (gProtect.m_MainInfo.IncreaseFPSSwitch != 0)
	{
		*(int*)0x00853588 += 8;
	}

	((void(*)())0x004C4559)();

	if(*(DWORD*)(MAIN_SCREEN_STATE) == 5)
	{
		if(*(DWORD*)MAIN_CURRENT_MAP == 30)
		{
			if(CastleSiegeMapUI == 0)
			{
				SiegeWarfare(SiegeInitMiniMapUI(pWindowThis()));
				CastleSiegeMapUI = 1;
			}
		}
		else
		{
			if(CastleSiegeMapUI == 1)
			{
				CastleSiegeMapUI = 0;
			}
		}

		if(WindowName[0] == 0)
		{
			SetWindowText(*(HWND*)(MAIN_WINDOW),gProtect.m_MainInfo.WindowName);
		}
		else
		{
			SetWindowText(*(HWND*)(MAIN_WINDOW),WindowName);
		}

		if(CustomAttack != 0)
		{
			if((Attacking != -1 || MouseRButton || MouseRButtonPush) && pMouseOnZone(0, 0, WindowGetScreenWidth(), 429, 1))
			{
				CustomAttack = 0;

				PBMSG_HEAD pMsg;

				pMsg.set(0x04,sizeof(pMsg));

				DataSend((BYTE*)&pMsg,pMsg.size);
			}
		}
	}
	else
	{
		//ClearNotice();
		CustomAttack = 0;
		WindowName[0] = 0;
		SetWindowText(*(HWND*)(MAIN_WINDOW),gProtect.m_MainInfo.WindowName);
	}
}

__declspec(naked) void CharacterCreationLevel() // OK
{
	static DWORD CheckCreateClassAddress1 = 0x00403777;
	static DWORD CheckCreateClassAddress2 = 0x004047D0;
	
	_asm
	{
		Push 0x01
		Mov Eax,[Ebp-0x10]
		Lea Ecx,[Eax+0xE0*0+0x270]
		Call[CheckCreateClassAddress2]
		Push 0x01
		Mov Eax,[Ebp-0x10]
		Lea Ecx,[Eax+0xE0*1+0x270]
		Call[CheckCreateClassAddress2]
		Push 0x01
		Mov Eax,[Ebp-0x10]
		Lea Ecx,[Eax+0xE0*2+0x270]
		Call[CheckCreateClassAddress2]
		Movzx Edx,Byte Ptr Ds:[0x07FE4A38]
		And Edx,0x04
		Sar Edx,0x02
		Push Edx
		Mov Eax,[Ebp-0x10]
		Lea Ecx,[Eax+0xE0*3+0x270]
		Call[CheckCreateClassAddress2]
		Movzx Edx,Byte Ptr Ds:[0x07FE4A38]
		And Edx,0x02
		Sar Edx,0x01
		Push Edx
		Mov Eax,[Ebp-0x10]
		Lea Ecx,[Eax+0xE0*4+0x270]
		Call[CheckCreateClassAddress2]
		Movzx Edx,Byte Ptr Ds:[0x07FE4A38]
		And Edx,0x01
		Push Edx
		Mov Eax,[Ebp-0x10]
		Lea Ecx,[Eax+0xE0*5+0x270]
		Call[CheckCreateClassAddress2]
		Jmp[CheckCreateClassAddress1]
	}
}

__declspec(naked) void CheckMasterLevel() // OK
{
	static DWORD CheckMasterLevelAddress1 = 0x004FD158;

	_asm
	{
		Push Ebp
		Mov Ebp,Esp
		Mov Eax,Dword Ptr Ds:[MAIN_CHARACTER_STRUCT]
		Xor Ecx,Ecx
		Mov Cx,Word Ptr Ds:[Eax+0x0E]
		Cmp Ecx,CharacterMaxLevel
		Je NEXT
		Xor Eax,Eax
		Jmp EXIT
		NEXT:
		Xor Edx,Edx
		Mov Dl,Byte Ptr Ss:[Ebp+0x08]
		Push Edx
		Call [CheckMasterLevelAddress1]
		Add Esp,0x04
		EXIT:
		Pop Ebp
		Retn
	}
}

__declspec(naked) void CheckTickCount() // OK
{
	static DWORD CheckTickCountAddress1 = 0x005ED235;

	static DWORD TimeRender = ((gProtect.m_MainInfo.IncreaseFPSSwitch==0)?40:32);

	_asm
	{
		Call Ebx
		Mov Esi,Dword Ptr Ss:[Esp+0x28]
		Sub Eax,Esi
		Cmp Eax,TimeRender
		Jge EXIT
		NEXT:
		Push 1
		Call Dword Ptr Ds:[Sleep]
		Call Ebx
		Sub Eax,Esi
		Cmp Eax,TimeRender
		Jl NEXT
		EXIT:
		Jmp [CheckTickCountAddress1]
	}
}