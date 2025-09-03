#include "stdafx.h"
#include "WindowTime.h"
#include "CustomMessage.h"
#include "Offset.h"
#include "Protect.h"
#include "Util.h"

bool m_OtherClosed = false;
bool m_WindowOpen = false;

std::vector<EVENT_TIME_INFO> m_EventTimeInfo;

void InitWindowTime() // OK
{
	if(gProtect.m_MainInfo.WindowEventTimeSwitch == 0)
	{
		return;
	}

	SetCompleteHook(0xE9,0x005906B0,&WindowGetScreenWidth);
}

int WindowGetScreenWidth() //OK
{
	int Width = 640;

	if(CheckWindow(INTERFACE_INVENTORY)
		&& (CheckWindow(INTERFACE_CHARACTER)
			|| CheckWindow(INTERFACE_NPCSHOP)
			|| CheckWindow(INTERFACE_STORAGE)
			|| CheckWindow(INTERFACE_MIXINVENTORY)
			|| CheckWindow(INTERFACE_TRADE)
			|| CheckWindow(INTERFACE_MYSHOP_INVENTORY)
			|| CheckWindow(INTERFACE_PURCHASESHOP_INVENTORY)
			|| CheckWindow(INTERFACE_LUCKYCOIN_REGISTRATION)))
	{
		Width = 260;
	}
	else if(CheckWindow(INTERFACE_CHARACTER) && (CheckWindow(INTERFACE_MYQUEST) || CheckWindow(INTERFACE_PET)))
	{
		Width = 260;
	}
	else if(CheckWindow(INTERFACE_REFINERY))
	{
		Width = 260;
	}
	else if(CheckWindow(INTERFACE_INVENTORY)
		|| CheckWindow(INTERFACE_CHARACTER) 
		|| CheckWindow(INTERFACE_PARTY)
		|| CheckWindow(INTERFACE_NPCGUILDMASTER)
		|| CheckWindow(INTERFACE_GUILDINFO)
		|| CheckWindow(INTERFACE_GUARDSMAN)
		|| CheckWindow(INTERFACE_SENATUS)
		|| CheckWindow(INTERFACE_GATEKEEPER)
		|| CheckWindow(INTERFACE_MYQUEST)
        || CheckWindow(INTERFACE_SERVERDIVISION)
        || CheckWindow(INTERFACE_COMMAND)
        || CheckWindow(INTERFACE_NPCQUEST)
		|| CheckWindow(INTERFACE_GATESWITCH)
		|| CheckWindow(INTERFACE_CATAPULT)
		|| CheckWindow(INTERFACE_DEVILSQUARE)
		|| CheckWindow(INTERFACE_BLOODCASTLE)
		|| CheckWindow(INTERFACE_GOLD_BOWMAN))
	{
		Width = 450;
	}
	else
	{
		Width = 640;
	}

	if(Width == 640)
	{
		m_OtherClosed = true;

		if(m_WindowOpen)
		{
			if(pMouseOnZone(WINDOW_POSX,WINDOW_POSY,190,433,1))
			{
				MouseOnWindow = true;
			}

			Width = 450;
		}
	}
	else
	{
		m_OtherClosed = false;

		if(m_WindowOpen)
		{
			WindowTimeClose();
		}
	}

	return Width;
}

void WindowTimeClose() // OK
{
	m_WindowOpen = false;

	pPlayBuffer(25,0,0);
}

void WindowButtonToggle() // OK
{
	if(gProtect.m_MainInfo.WindowEventTimeSwitch == 0)
	{
		return;
	}

	if(CheckWindow(INTERFACE_CHATINPUTBOX) || CheckWindow(INTERFACE_MASTER_LEVEL) || IsInputEnable())
	{
		return;
	}

	if(m_OtherClosed == false)
	{
		HideAllWindow;
	}

	if(m_WindowOpen)
	{
		WindowTimeClose();
	}
	else
	{
		m_WindowOpen = true;

		pPlayBuffer(25,0,0);

		PBMSG_HEAD pMsg;

		pMsg.set(0x06,sizeof(pMsg));

		DataSend((BYTE*)&pMsg,pMsg.size);
	}
}

void DrawWindowTime() // OK
{
	if(m_WindowOpen)
	{
		if(pMouseOnZone(WINDOW_POSX,WINDOW_POSY,190,433,1))
		{
			if(pMouseOnZone(463, 392, 36, 29, 1) && IsRelease(VK_LBUTTON))
			{
				WindowTimeClose();

				return;
			}
			else if(pMouseOnZone(619, 7, 13, 12, 1)  && IsPress(VK_LBUTTON))
			{
				WindowTimeClose();

				return;
			}
		}
		else
		{
			if(MouseLButton || MouseRButton || MouseLButtonPush || MouseRButtonPush || CheckWindow(INTERFACE_MASTER_LEVEL))
			{
				WindowTimeClose();

				return;
			}
		}

		EnableAlphaTest(true);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		DrawInterface(31312, WINDOW_POSX, WINDOW_POSY, 190.f, 429.f);
		
		DrawInterface(31340, WINDOW_POSX, WINDOW_POSY, 190.f, 64.f);
		
		DrawInterface(31342, WINDOW_POSX, WINDOW_POSY+64, 21.f, 320.f);
		
		DrawInterface(31343, WINDOW_POSX+190-21, WINDOW_POSY+64, 21.f, 320.f);
		
		DrawInterface(31344, WINDOW_POSX, WINDOW_POSY+384, 190.f, 45.f);

		WindowRenderText((int)WINDOW_POSX + 95, 15, pFontBold, 8, 0, gCustomMessage.GetMessage(33));

		int count = 0;

		for each(EVENT_TIME_INFO lpInfo in m_EventTimeInfo)
		{
			EnableAlphaTest(true);

			glColor4f(0.0f, 0.0f, 0.0f, 0.55f);

			pDrawBarForm((float)WINDOW_POSX + 19, (float)WINDOW_POSY + 48 + (10 * count), 152, 8);

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			pTextureBlind();

			DisableAlphaBlend();

			WindowRenderText(WINDOW_POSX + 20, WINDOW_POSY + 52 + (10 * count), pFontBold, 0, 0, "%s", lpInfo.name);

			WindowRenderText(WINDOW_POSX + 170, WINDOW_POSY + 52 + (10 * count), pFontNormal, 7, GetEventRenderColor(lpInfo.status, lpInfo.time), "%s", GetEventRemainTime(lpInfo.status, lpInfo.time));

			count++;
		}

		if(pMouseOnZone(WINDOW_POSX + 13, WINDOW_POSY + 392, 36, 29, 1))
		{
			pRenderTipText(WINDOW_POSX + 15, WINDOW_POSY + 380, gCustomMessage.GetMessage(39));

			if(MouseLButton)
			{
				pDrawButton(31357, WINDOW_POSX + 13, WINDOW_POSY + 392, 36.f, 29.f, 0.0f, 29.0f);
			}
			else
			{
				pDrawButton(31357, WINDOW_POSX + 13, WINDOW_POSY + 392, 36.f, 29.f, 0.0f, 0.0f);
			}
		}
		else
		{
			pDrawButton(31357, WINDOW_POSX + 13, WINDOW_POSY + 392, 36.f, 29.f, 0.0f, 0.0f);
		}

		DisableAlphaBlend();
	}
}

void WindowRenderText(int x,int y,HFONT font,int align,int color,char* text,...) // OK
{
	char buff[256];

	memset(buff, 0, sizeof(buff));

	va_list arg;
	va_start(arg, text);
	vsprintf_s(buff, text, arg);
	va_end(arg);

	SIZE sz;

	GetTextExtentPoint(pGetFontDC(pTextThis()), buff, strlen(buff), &sz);

	int py = (y - (480 * sz.cy / *(DWORD*)MAIN_RESOLUTION_Y >> 1));

	EnableAlphaTest(true);

	pTextSetFont(pTextThis(),font);
	pSetBGTextColor(pTextThis(), 0, 0, 0, 0);

	if(color == 0)
	{
		pSetTextColor(pTextThis(), 220, 220, 220, 255);
	}
	else if(color == 1)
	{
		pSetTextColor(pTextThis(), 223, 191, 103, 255);
	}
	else if(color == 2)
	{
		pSetTextColor(pTextThis(), 230, 5, 5, 255);
	}
	else if(color == 3)
	{
		pSetTextColor(pTextThis(), 73, 183, 0, 255);
	}
	else if(color == 4)
	{
		pSetTextColor(pTextThis(), 100, 150, 255, 255);
	}

	pDrawText(pTextThis(), x, py, buff, 0, 0, align, 0);

	pTextSetFont(pTextThis(),pFontNormal);
}

int GetEventRenderColor(int status, int time) // OK
{
	int color = 1;

	if(status == EVENT_STATE_BLANK)
	{
		color = 2;
	}
	else if(status == EVENT_STATE_STAND)
	{
		if(time >= 0 && time <= 300)
		{
			color = 2;
		}
	}
	else if(status == EVENT_STATE_OPEN)
	{
		color = 3;
	}
	else if(status == EVENT_STATE_START)
	{
		color = 4;
	}

	return color;
}

char* GetEventRemainTime(int status,int time) // OK
{
	static char buff[256];

	memset(buff, 0, sizeof(buff));

	if(status == EVENT_STATE_BLANK)
	{
		sprintf_s(buff, gCustomMessage.GetMessage(34));
	}
	else if(status == EVENT_STATE_STAND)
	{
		int day = time/86400;
		time %= 86400;

		int hr = time/3600;
		time %= 3600;

		int min = time/60;
		time %= 60;

		if(day > 0)
		{
			sprintf_s(buff, gCustomMessage.GetMessage(35), day);
		}
		else
		{
			sprintf_s(buff, gCustomMessage.GetMessage(36), hr, min, time);
		}
	}
	else if(status == EVENT_STATE_OPEN)
	{
		sprintf_s(buff, gCustomMessage.GetMessage(37));
	}
	else if(status == EVENT_STATE_START)
	{
		sprintf_s(buff, gCustomMessage.GetMessage(38));
	}

	return buff;
}

void GCEventTimeListRecv(PMSG_EVENT_TIME_LIST_RECV* lpMsg) // OK
{
	m_EventTimeInfo.clear();

	if(m_WindowOpen)
	{
		for(int n = 0; n < lpMsg->count; n++)
		{
			EVENT_TIME_INFO* lpInfo = (EVENT_TIME_INFO*)(((BYTE*)lpMsg) + sizeof(PMSG_EVENT_TIME_LIST_RECV) + (sizeof(EVENT_TIME_INFO)*n));

			EVENT_TIME_INFO info;

			strcpy_s(info.name, lpInfo->name);

			info.status = lpInfo->status;

			info.time = lpInfo->time;

			m_EventTimeInfo.push_back(info);
		}
	}
}