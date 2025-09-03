#include "stdafx.h"
#include "Attack.h"
#include "CustomMessage.h"
#include "Offset.h"
#include "OptionsMenu.h"
#include "Protocol.h"

int CustomAttack;
DWORD LastTickCount;

void AutoAttackToggle() // OK
{
	if(*(BYTE*)(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+14) != 0)
	{
		return;
	}

	if((GetTickCount()-LastTickCount) < 1000)
	{
		return;
	}

	int index = *(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT) + 2 * *(BYTE*)(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+50) + 120);

	if(index > 0)
	{
		PMSG_CUSTOM_ATTACK_TOGGLE_SEND pMsg;

		pMsg.header.set(0xF3,0x0A,sizeof(pMsg));

		pMsg.index = index;

		DataSend((BYTE*)&pMsg, pMsg.header.size);
	}
}

void AttackDrawInterface() // OK
{
	if(CustomAttack == 0)
	{
		return;
	}

	if(gOptionsMenu.DeleteAttackBar != 0)
	{
		return;
	}

	EnableAlphaTest(true);
	
	glColor4f(0.0f,0.0f,0.0f,0.6f);
	pDrawBarForm(0,80,640,25.0f);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);

	EnableAlphaTest(false);

	EnableAlphaTest(true);

	pTextSetFont(pTextThis(),pFontBig);

	pSetBGTextColor(pTextThis(),0,0,0,0);

	pSetTextColor(pTextThis(),255,210,40,255);

	pDrawText(pTextThis(),320, 85, gCustomMessage.GetMessage(0), 0, 0, 8, 0);

	DisableAlphaBlend();
}

void GCCustomAttackStatusRecv(PMSG_CUSTOM_ATTACK_STATUS_RECV* lpMsg) // OK
{
	if(lpMsg->Started != 0)
	{
		*(BYTE*)(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+80) = 0;

		pSetPlayerStop(*(DWORD*)(MAIN_VIEWPORT_STRUCT));

		Attacking = -1;
	}

	CustomAttack = lpMsg->Started;

	LastTickCount = GetTickCount();
}