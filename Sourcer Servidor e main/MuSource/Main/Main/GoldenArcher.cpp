#include "stdafx.h"
#include "GoldenArcher.h"
#include "CustomMessage.h"
#include "ItemMove.h"
#include "Offset.h"
#include "Protect.h"
#include "Shop.h"
#include "Util.h"
#include "WindowTime.h"

int RegisterRenaCount = 0;
int AvailableRenaCount = 0;
bool ButtonStatus[6] = {false,false,false,false,false,false};
DWORD TickCount = GetTickCount();

void InitGoldenArcher() // OK
{
	if(gProtect.m_MainInfo.GoldenArcherRenewalSwitch == 0)
	{
		return;
	}

	SetCompleteHook(0xE9,0x00708E8A,&DrawGoldenArcher);

	SetCompleteHook(0xE9,0x00708555,&ControlGoldenArcher);
}

bool DrawGoldenArcher() // OK
{
	EnableAlphaTest(true);

	glColor4f(1.0f,1.0f,1.0f,1.0f);

	AvailableRenaCount =InventoryItemCount(InventoryThis(pWindowThis()),GET_ITEM(14,21));

	if(GetTickCount() >= TickCount)
	{
		ButtonStatus[0] = ((RegisterRenaCount >= 10)?true:false);
			
		ButtonStatus[1] = ((RegisterRenaCount >= 20)?true:false);
			
		ButtonStatus[2] = ((RegisterRenaCount >= 30)?true:false);
			
		ButtonStatus[3] = ((RegisterRenaCount >= 50)?true:false);
			
		ButtonStatus[4] = ((RegisterRenaCount >= 100)?true:false);
			
		ButtonStatus[5] = ((AvailableRenaCount >= 1)?true:false);
	}

	DrawInterface(31312, WINDOW_POSX, WINDOW_POSY, 190.f, 429.f);
		
	DrawInterface(31340, WINDOW_POSX, WINDOW_POSY, 190.f, 64.f);
		
	DrawInterface(31342, WINDOW_POSX, WINDOW_POSY+64, 21.f, 320.f);
		
	DrawInterface(31343, WINDOW_POSX+190-21, WINDOW_POSY+64, 21.f, 320.f);
		
	DrawInterface(31344, WINDOW_POSX, WINDOW_POSY+384, 190.f, 45.f);

	WindowRenderText((int)WINDOW_POSX + 95, 15, pFontBold, 8, 0, gCustomMessage.GetMessage(42));

	for(int n = 0; n < 6; n++)
	{
		if(!ButtonStatus[n])
		{ 
			glColor3f(0.4f, 0.4f, 0.4f); 
		}

		float PosX = WINDOW_POSX + 41.0f;

		float PosY = WINDOW_POSY + 60.0f + (float)(n * 30) + ((n == 5) ? 30.0f : 0);

		float ButtonWidth = 108.0f;

		float ButtonHeight = 29.0f;

		if(pMouseOnZone((int)PosX, (int)PosY, (int)ButtonWidth, (int)ButtonHeight, 1) && ButtonStatus[n])
		{
			if(MouseLButton)
				pDrawButton(31316, PosX, PosY, ButtonWidth, ButtonHeight, 0.0f, ButtonHeight*2);
			else
				pDrawButton(31316, PosX, PosY, ButtonWidth, ButtonHeight, 0.0f, ButtonHeight);
		}
		else
		{
			pDrawButton(31316, PosX, PosY, ButtonWidth, ButtonHeight, 0.0f, 0.0f);
		}
			
		WindowRenderText(WINDOW_POSX + 95, WINDOW_POSY + 72 + (n * 30) + ((n == 5) ? 30 : 0), pFontNormal, 8, 0, gCustomMessage.GetMessage(43+n));
			
		glColor3f(1.f, 1.f, 1.f);
	}

	GoldenArcherRenderItem3D();

	WindowRenderText(WINDOW_POSX + 50, 306, pFontBold, 0, 0, gCustomMessage.GetMessage(49),RegisterRenaCount);

	WindowRenderText(WINDOW_POSX + 50, 342, pFontBold, 0, 0, gCustomMessage.GetMessage(50),AvailableRenaCount);

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

	return true;
}

bool ControlGoldenArcher() // OK
{
	if(CheckWindow(INTERFACE_GOLD_BOWMAN))
	{
		if(pMouseOnZone(WINDOW_POSX, WINDOW_POSY, 190, 433, 1))
		{
			MouseOnWindow = true;

			if(pMouseOnZone(WINDOW_POSX + 35, WINDOW_POSY + 60, 120, 24, 1) && IsRelease(VK_LBUTTON) && ButtonStatus[0])
			{
				PMSG_GOLDEN_ARCHER_REWARD_SEND pMsg;

				pMsg.header.set(0x96,sizeof(pMsg));

				pMsg.type = 0;

				DataSend((BYTE*)&pMsg,pMsg.header.size);

				ButtonStatus[0] = false;

				GoldenArcherButtonPush();

				return 0;
			}
			else if(pMouseOnZone(WINDOW_POSX + 35, WINDOW_POSY + 90, 120, 24, 1) && IsRelease(VK_LBUTTON) && ButtonStatus[1])
			{
				PMSG_GOLDEN_ARCHER_REWARD_SEND pMsg;

				pMsg.header.set(0x96,sizeof(pMsg));

				pMsg.type = 1;

				DataSend((BYTE*)&pMsg,pMsg.header.size);

				ButtonStatus[1] = false;

				GoldenArcherButtonPush();

				return 0;
			}
			else if(pMouseOnZone(WINDOW_POSX + 35, WINDOW_POSY + 120, 120, 24, 1) && IsRelease(VK_LBUTTON) && ButtonStatus[2])
			{
				PMSG_GOLDEN_ARCHER_REWARD_SEND pMsg;

				pMsg.header.set(0x96,sizeof(pMsg));

				pMsg.type = 2;

				DataSend((BYTE*)&pMsg,pMsg.header.size);

				ButtonStatus[2] = false;

				GoldenArcherButtonPush();

				return 0;
			}
			else if(pMouseOnZone(WINDOW_POSX + 35, WINDOW_POSY + 150, 120, 24, 1) && IsRelease(VK_LBUTTON) && ButtonStatus[3])
			{
				PMSG_GOLDEN_ARCHER_REWARD_SEND pMsg;

				pMsg.header.set(0x96,sizeof(pMsg));

				pMsg.type = 3;

				DataSend((BYTE*)&pMsg,pMsg.header.size);

				ButtonStatus[3] = false;

				GoldenArcherButtonPush();

				return 0;
			}
			else if(pMouseOnZone(WINDOW_POSX + 35, WINDOW_POSY + 180, 120, 24, 1) && IsRelease(VK_LBUTTON) && ButtonStatus[4])
			{
				PMSG_GOLDEN_ARCHER_REWARD_SEND pMsg;

				pMsg.header.set(0x96,sizeof(pMsg));

				pMsg.type = 4;

				DataSend((BYTE*)&pMsg,pMsg.header.size);

				ButtonStatus[4] = false;

				GoldenArcherButtonPush();

				return 0;
			}
			else if(pMouseOnZone(WINDOW_POSX + 35, WINDOW_POSY + 240, 120, 24, 1) && IsRelease(VK_LBUTTON) && ButtonStatus[5])
			{
				ButtonStatus[5] = false;

				GoldenArcherButtonPush();

				PBMSG_HEAD pMsg;

				pMsg.set(0x95,sizeof(pMsg));

				DataSend((BYTE*)&pMsg,pMsg.size);

				return 0;
			}
			else if(pMouseOnZone(463, 392, 36, 29, 1) && IsRelease(VK_LBUTTON))
			{
				CloseWindow(INTERFACE_GOLD_BOWMAN);

				pPlayBuffer(25,0,0);

				return 0;
			}

			else if(pMouseOnZone(619, 7, 13, 12, 1) && IsPress(VK_LBUTTON))
			{
				CloseWindow(INTERFACE_GOLD_BOWMAN);

				pPlayBuffer(25,0,0);

				return 0;
			}
		}
		else
		{
			if(MouseLButtonPush)
			{
				CloseWindow(INTERFACE_GOLD_BOWMAN);

				pPlayBuffer(25,0,0);

				return 0;
			}
		}
	}

	return 1;
}

void GoldenArcherRenderItem3D() // OK
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
 
	pglViewport2(0,0,*(DWORD*)MAIN_RESOLUTION_X,*(DWORD*)MAIN_RESOLUTION_Y);
	pgluPerspective2(1.f,(float)(*(float*)MAIN_RESOLUTION_X / *(float*)MAIN_RESOLUTION_Y),20.f,2000.f);
 
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

	pGetOpenGLMatrix(pCameraMatrix);
	pEnableDepthTest();
	pEnableDepthMask();

	glClear(GL_DEPTH_BUFFER_BIT);

	pRenderItem3D(480.f,302.f,10.f,10.f,GET_ITEM(14,21),0,0,0,0);

	pRenderItem3D(480.f,336.f,10.f,10.f,GET_ITEM(14,21),0,0,0,0);

	pUpdateMousePosition();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	pBeginBitmap();

	glColor3f(1,1,1);

	EnableAlphaTest(true);
}

void GoldenArcherButtonPush() // OK
{
	TickCount = GetTickCount() + 100;

	MouseLButtonPush = 0;
	MouseUpdateTime = 0;
	
	MouseUpdateTimeMax = 6;

	pPlayBuffer(25, 0, 0);
}

void GoldenArcherCountRecv(PMSG_GOLDEN_ARCHER_COUNT_RECV* lpMsg) // OK
{
	HideAllWindow;

	RegisterRenaCount = lpMsg->count;
	
	OpenWindow(INTERFACE_GOLD_BOWMAN);
	
	EventChipDialogEnable = 0;
}