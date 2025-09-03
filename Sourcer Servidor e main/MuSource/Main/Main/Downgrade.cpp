#include "stdafx.h"
#include "Downgrade.h"
#include "Offset.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "Util.h"
#include "WindowTime.h"

void InitDowngrade() // OK
{
	if (gProtect.m_MainInfo.UseOldMainFrameBar == 0)
	{
		return;
	}

	MemorySet(0x0071C431, 0x90, 0x26); // UpdateKeyEvent (Disable R)

	SetByte(0x00710F1E, 0xC3); // Hide Position Info on top

	SetByte(0x00719AA9, 0xC3); // Disable NewUI Buttons Function

	SetCompleteHook(0xE9, 0x00719775, &LoadImages);

	SetCompleteHook(0xE9, 0x0071BD23, &RenderButtons);

	SetCompleteHook(0xE9, 0x00719FEB, &RenderFrame);

	SetCompleteHook(0xE9, 0x0071A0DB, &RenderLifeMana);

	SetCompleteHook(0xE9, 0x0071AA1E, &RenderGuageSD);

	SetCompleteHook(0xE9, 0x0071A77A, &RenderGuageAG);

	SetCompleteHook(0xE9, 0x0071ACD0, &RenderExperience);

	SetCompleteHook(0xE9, 0x0071CC5B, &RenderItems);

	SetCompleteHook(0xE9, 0x0071CD13, &RenderItemCount);

	SetCompleteHook(0xE9, 0x0071CDAD, &UseItemRButton);

	SetCompleteHook(0xE9, 0x0071E322, &RenderCurrentSkill);

	SetCompleteHook(0xE9, 0x0071E5E3, &RenderSkillList);

	SetCompleteHook(0xE9, 0x0071D2D1, &SkillListMouseEvent);

	SetCompleteHook(0xE8, 0x0071FD44, &RenderSkillNumber);
}

void LoadImages() // OK
{
	pLoadImage("Interface\\Menu01_new.jpg", IMAGE_MENU_1, GL_NEAREST, GL_CLAMP, 0, 1);

	pLoadImage("Interface\\Menu02.jpg", IMAGE_MENU_2, GL_NEAREST, GL_CLAMP, 0, 1);

	pLoadImage("Interface\\Menu03_new.jpg", IMAGE_MENU_3, GL_NEAREST, GL_CLAMP, 0, 1);

	pLoadImage("Interface\\Menu04.tga", IMAGE_MENU_4, GL_NEAREST, GL_CLAMP, 0, 1);

	pLoadImage("Interface\\Menu_Blue.jpg", IMAGE_GAUGE_BLUE, GL_NEAREST, GL_CLAMP, 0, 1);

	pLoadImage("Interface\\Menu_Green.jpg", IMAGE_GAUGE_GREEN, GL_NEAREST, GL_CLAMP, 0, 1);

	pLoadImage("Interface\\Menu_Red.jpg", IMAGE_GAUGE_RED, GL_NEAREST, GL_CLAMP, 0, 1);

	pLoadImage("Interface\\Menu03_new_AG.jpg", IMAGE_GAUGE_AG, GL_NEAREST, GL_CLAMP, 0, 1);

	pLoadImage("Interface\\menu01_new2_SD.jpg", IMAGE_GAUGE_SD, GL_NEAREST, GL_CLAMP, 0, 1);

	pLoadImage("Interface\\menu01_new2.jpg", IMAGE_MENU_BTN_COMMAND, GL_NEAREST, GL_CLAMP, 0, 1);

	pLoadImage("Interface\\Menu_Party.jpg", IMAGE_MENU_BTN_PARTY, GL_NEAREST, GL_CLAMP, 0, 1);

	pLoadImage("Interface\\Menu_Character.jpg", IMAGE_MENU_BTN_CHAINFO, GL_NEAREST, GL_CLAMP, 0, 1);

	pLoadImage("Interface\\Menu_Inventory.jpg", IMAGE_MENU_BTN_MYINVEN, GL_NEAREST, GL_CLAMP, 0, 1);

	pLoadImage("Interface\\win_push.jpg", IMAGE_MENU_BTN_FRIEND, GL_NEAREST, GL_CLAMP, 0, 1);

	pLoadImage("Interface\\Guild.jpg", IMAGE_MENU_BTN_GUILD, GL_NEAREST, GL_CLAMP, 0, 1);

	pLoadImage("Interface\\SkillBox.jpg", IMAGE_SKILLBOX, GL_NEAREST, GL_CLAMP, 0, 1);
}

void RenderButtons() // OK
{
	// Render CommandMenu and Tip Text

	if (CheckWindow(INTERFACE_COMMAND))
	{
		DrawInterface(IMAGE_MENU_BTN_COMMAND, 6, 435, 53, 19);
	}

	if (pMouseOnZone(6, 435, 53, 19, 1) == true)
	{
		pRenderTipText(6 - 3, 435 - 5, pGetTextLine(939));

		if (IsPress(VK_LBUTTON))
		{
			pPlayBuffer(25, 0, 0);

			ToggleWindow(INTERFACE_COMMAND);

			ResetMouseLButton();
		}
	}

	// Render Party and Tip Text

	if (CheckWindow(INTERFACE_PARTY))
	{
		DrawInterface(IMAGE_MENU_BTN_PARTY, 348, 452, 24, 24);
	}

	if (pMouseOnZone(348, 452, 24, 24, 1) == true)
	{
		pRenderTipText(348 - 3, 452 - 5, pGetTextLine(361));

		if (IsPress(VK_LBUTTON))
		{
			pPlayBuffer(25, 0, 0);

			ToggleWindow(INTERFACE_PARTY);

			ResetMouseLButton();
		}
	}

	// Render CharacterInfo and Tip Text

	if (CheckWindow(INTERFACE_CHARACTER))
	{
		DrawInterface(IMAGE_MENU_BTN_CHAINFO, 379, 452, 24, 24);
	}

	if (pMouseOnZone(379, 452, 24, 24, 1) == true)
	{
		pRenderTipText(379 - 3, 452 - 5, pGetTextLine(362));

		if (IsPress(VK_LBUTTON))
		{
			pPlayBuffer(25, 0, 0);

			ToggleWindow(INTERFACE_CHARACTER);

			ResetMouseLButton();
		}
	}

	// Render Inventory and Tip Text

	if (CheckWindow(INTERFACE_INVENTORY))
	{
		DrawInterface(IMAGE_MENU_BTN_MYINVEN, 410, 452, 24, 24);
	}

	if (pMouseOnZone(410, 452, 24, 24, 1) == true)
	{
		pRenderTipText(410 - 3, 452 - 5, pGetTextLine(363));

		if (IsPress(VK_LBUTTON))
		{
			pPlayBuffer(25, 0, 0);

			ToggleWindow(INTERFACE_INVENTORY);

			ResetMouseLButton();
		}
	}

	// Render Friends and Tip Text

	if (CheckWindow(INTERFACE_FRIEND))
	{
		DrawInterface(IMAGE_MENU_BTN_FRIEND, 581, 435, 52, 18);
	}

	if (pMouseOnZone(581, 435, 52, 18, 1) == true)
	{
		pRenderTipText(581 - 3, 435 - 5, pGetTextLine(1043));

		if (IsPress(VK_LBUTTON))
		{
			pPlayBuffer(25, 0, 0);

			if (ViewLevel >= 6)
			{
				ToggleWindow(INTERFACE_FRIEND);
			}

			ResetMouseLButton();
		}
	}

	// Render Guild and Tip Text

	if (CheckWindow(INTERFACE_GUILDINFO))
	{
		DrawInterface(IMAGE_MENU_BTN_GUILD, 581, 459, 52, 18);
	}

	if (pMouseOnZone(581, 459, 52, 18, 1) == true)
	{
		pRenderTipText(581 - 3, 459 - 5, pGetTextLine(364));

		if (IsPress(VK_LBUTTON))
		{
			pPlayBuffer(25, 0, 0);

			ToggleWindow(INTERFACE_GUILDINFO);

			ResetMouseLButton();
		}
	}

	glColor3f(1.0f, 1.0f, 1.0f);
}

void RenderFrame() // OK
{
	// Render the MainFrame frame first
	float width = 256.0f;

	float height = 48.0f;

	float x = 0.0f;

	float y = 480.0f - height;

	DrawInterface(IMAGE_MENU_1, x, y, width, height);

	width = 128.0f;

	x = 256.0f;

	DrawInterface(IMAGE_MENU_2, x, y, width, height);

	width = 256.0f;

	x = 256.0f + 128.0f;

	DrawInterface(IMAGE_MENU_3, x, y, width, height);

	// Render coordinates

	glColor3f(0.6f, 0.6f, 0.6f);

	RenderNumber2D(24.0f, 462.0f, *(BYTE*)(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+132), 9.0f, 10.0f);

	RenderNumber2D(48.0f, 462.0f, *(BYTE*)(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+136), 9.0f, 10.0f);

	glColor3f(1.0f, 1.0f, 1.0f);

	// DRAGONS
	x = 0.0f;

	y = 480.0f - height - 45.0f;

	width = 108.0f;

	height = 45.0f;

	if (CheckWindow(INTERFACE_MASTER_LEVEL))
	{
		return;
	}

	if (!CheckWindow(INTERFACE_CHATINPUTBOX))
	{
		pDrawImage(IMAGE_MENU_4, x, y, width, height, 0.0f, 0.0f, 0.84375f, 0.703125f, 1, 1, 0.0);
	}

	// Interfaces Open
	if (WindowGetScreenWidth() != 640)
	{
		return;
	}

	// Menu letter U ~ Guild War ~ Duel ~ Minimap
	if (CheckWindow(INTERFACE_WINDOW_MENU) || CheckWindow(INTERFACE_BATTLE_SOCCER_SCORE) || CheckWindow(INTERFACE_DUEL_WINDOW) || CheckWindow(INTERFACE_SIEGEWARFARE))
	{
		return;
	}

	// Interface of Events (BC, CC ,KANTURU, IT)
	if (CheckWindow(INTERFACE_BLOODCASTLE_TIME) || CheckWindow(INTERFACE_CHAOSCASTLE_TIME) || CheckWindow(INTERFACE_KANTURU_INFO) || CheckWindow(INTERFACE_CURSEDTEMPLE_GAMESYSTEM))
	{
		return;
	}

	// Crywolf
	if (m_CrywolfState >= 3 && m_CrywolfState < 6)
	{
		return;
	}

	x = 256.0f + 256.0f + 20.0f;

	pDrawImage(IMAGE_MENU_4, x, y, width, height, 0.84375f, 0.0f, -0.84375f, 0.703125f, 1, 1, 0.0);
}

void RenderLifeMana() // OK
{
	DWORD wLifeMax = ViewMaxHP;

	DWORD wLife = min(max((WORD)0, ViewCurHP), wLifeMax);

	DWORD wManaMax = ViewMaxMP;

	DWORD wMana = min(max((WORD)0, ViewCurMP), wManaMax);

	// Sense of making a heartbeat when the health value is lower than 20%
	if (wLifeMax > 0)
	{
		if (wLife > 0 && (wLife / (float)wLifeMax) < 0.2f)
		{
			pPlayBuffer(34, 0, 0);
		}
	}

	// handle division exception
	float fLife = 0.0f;

	float fMana = 0.0f;

	if (wLifeMax > 0)
	{
		fLife = (wLifeMax - wLife) / (float)wLifeMax;
	}

	if (wManaMax > 0)
	{
		fMana = (wManaMax - wMana) / (float)wManaMax;
	}

	float fY, fH, fV;

	// life
	float width = 53.0f;

	float height = 48.0f;

	float x = 97.0f;

	float y = 480.0f - height;

	// If the poison attribute is met, green energy is drawn, otherwise red energy is drawn.
	fY = y + (fLife * height);

	fH = height - (fLife * height);

	fV = fLife;

	if (g_isCharacterBuff(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+1096, 55))
	{
		pDrawImage(IMAGE_GAUGE_GREEN, x, fY, width, fH, 0.0f, fV * height / 64.0f, width / 64.0f, (1.0f - fV) * height / 64.0f, 1, 1, 0.0);
	}
	else
	{
		pDrawImage(IMAGE_GAUGE_RED, x, fY, width, fH, 0.0f, fV * height / 64.0f, width / 64.0f, (1.0f - fV) * height / 64.0f, 1, 1, 0.0);
	}

	RenderNumber2D(x + 43.0f, 480.0f - 13.0f, wLife, 9.0f, 10.0f);

	// life description
	char strTipText[256];

	if (pMouseOnZone((int)x, (int)y, (int)width, (int)height, 1) == true)
	{
		sprintf_s(strTipText, pGetTextLine(358), wLife, wLifeMax);

		pRenderTipText((int)x, (int)419, strTipText);

		glColor3f(1.0f, 1.0f, 1.0f);
	}

	// mana
	width = 53.0f;

	height = 48.0f;

	x = 256.0f + 128.0f + 105.0f;

	y = 480.0f - height;

	fY = y + (fMana * height);

	fH = height - (fMana * height);

	fV = fMana;

	pDrawImage(IMAGE_GAUGE_BLUE, x, fY, width, fH, 0.0f, fV * height / 64.0f, width / 64.0f, (1.0f - fV) * height / 64.0f, 1, 1, 0.0);

	RenderNumber2D(x + 36.0f, 480.0f - 13.0f, wMana, 9.0f, 10.0f);

	// mana description
	if (pMouseOnZone((int)x, (int)y, (int)width, (int)height, 1) == true)
	{
		sprintf_s(strTipText, pGetTextLine(359), wMana, wManaMax);

		pRenderTipText((int)x, (int)419, strTipText);

		glColor3f(1.0f, 1.0f, 1.0f);
	}
}

void RenderGuageSD() // OK
{
	DWORD wMaxShield = max((WORD)1, ViewMaxSD);

	DWORD wShield = min(wMaxShield, ViewCurSD);

	// handle division exception
	float fShield = 0.0f;

	if (wMaxShield > 0)
	{
		fShield = (wMaxShield - wShield) / (float)wMaxShield;
	}

	float width = 15.0f;

	float height = 36.0f;

	float x = 73.0f;

	float y = 474.0f - height;

	float fY = y + (fShield * height);

	float fH = height - (fShield * height);

	float fV = fShield;

	pDrawImage(IMAGE_GAUGE_SD, x, fY, width, fH, 0.0f, fV * height / 64.0f, width / 16.0f, (1.0f - fV) * height / 64.0f, 1, 1, 0.0);

	RenderNumber2D(x + 17.0f, 480.0f - 13.0f, wShield, 9.0f, 10.0f);

	if (pMouseOnZone((int)x, (int)y, (int)width, (int)height, 1) == true)
	{
		char strTipText[256];

		// 2037 " S D : %d / %d"
		sprintf_s(strTipText, pGetTextLine(2037), wShield, wMaxShield);

		pRenderTipText((int)x - 5, (int)419, strTipText);
	}
}

void RenderGuageAG() // OK
{
	DWORD dwMaxSkillMana = max((WORD)1, ViewMaxBP);

	DWORD dwSkillMana = min(dwMaxSkillMana, ViewCurBP);

	// handle division exception
	float fSkillMana = 0.0f;

	if (dwMaxSkillMana > 0)
	{
		fSkillMana = (dwMaxSkillMana - dwSkillMana) / (float)dwMaxSkillMana;
	}

	float width = 15.0f;

	float height = 36.0f;

	float x = 256.0f + 256.0f + 39.0f;

	float y = 474.0f - height;

	float fY = y + (fSkillMana * height);

	float fH = height - (fSkillMana * height);

	float fV = fSkillMana;

	pDrawImage(IMAGE_GAUGE_AG, x, fY, width, fH, 0.0f, fV * height / 64.0f, width / 16.0f, (1.0f - fV) * height / 64.0f, 1, 1, 0.0);

	RenderNumber2D(x + 20.0f, 480.0f - 13.0f, dwSkillMana, 9.0f, 10.0f);

	if (pMouseOnZone((int)x, (int)y, (int)width, (int)height, 1) == true)
	{
		char strTipText[256];

		sprintf_s(strTipText, pGetTextLine(214), dwSkillMana, dwMaxSkillMana);

		pRenderTipText((int)x - 5, (int)419, strTipText);
	}
}

void RenderExperience() // OK
{
	QWORD dwNexExperience = 0; // next level up experience

	QWORD dwExperience = 0; // current experience

	QWORD dwPriorExperience = 0;

	if (IsMasterLevel())
	{
		dwNexExperience = *(QWORD*)0x07FE8D20;

		dwExperience = *(QWORD*)0x07FE8D18;

		QWORD iTotalLevel = *(WORD*)0x07FE8D10 + 400;

		QWORD iTOverLevel = iTotalLevel - 255;

		QWORD iData_Master = (((9 + iTotalLevel) * iTotalLevel * iTotalLevel * 10) + ((9 + iTOverLevel) * iTOverLevel * iTOverLevel * 1000));

		dwPriorExperience = (iData_Master - 3892250000) / 2;
	}
	else
	{
		dwNexExperience = ViewNextExperience;

		dwExperience = ViewExperience;

		WORD wPriorLevel = (WORD)(ViewLevel - 1);

		if (wPriorLevel > 0)
		{
			dwPriorExperience = (((wPriorLevel + 9) * wPriorLevel) * wPriorLevel) * CalcConstA;

			if (wPriorLevel > 255)
			{
				int iLevelOverN = wPriorLevel - 255;

				dwPriorExperience += (((iLevelOverN + 9) * iLevelOverN) * iLevelOverN) * CalcConstB;
			}
		}
	}

	int ExpNumber = 0;

	float Experience = (float)(dwExperience - dwPriorExperience);

	float NextExperience = (float)(dwNexExperience - dwPriorExperience);

	float width = 0.0f;

	if(Experience > 0 && NextExperience > 0 && NextExperience > Experience)
	{
		width = ((Experience/NextExperience)*10.0f);

		width -= (int)width;

		width *= 198.0f;

		ExpNumber = (int)((Experience/NextExperience)*10.0f);
	}

	float height = 4.0f;

	float x = 221.0f;

	float y = 439.0f;

	glColor3f(0.91f, 0.81f, 0.3f);

	pDrawBarForm(x, y, width, height);

	pTextureBlind();

	EnableAlphaTest(true);

	glColor3f(0.91f, 0.81f, 0.6f);

	RenderNumber2D(425.0f, 434.0f, ExpNumber, 9.0f, 10.0f);

	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);

	width = 198.0f;

	if (pMouseOnZone((int)x, (int)y, (int)width, (int)height, 1) == true)
	{
		char strTipText[256];

		// 1748 "Experience: %I64d / %I64d"
		sprintf_s(strTipText, pGetTextLine(1748), dwExperience, dwNexExperience);

		pRenderTipText((int)x + 2, (int)y - 15, strTipText);
	}
}

void RenderItems() // OK
{
	float width = 20.0f;

	float height = 20.0f;

	float x = 209.0f;

	float y = 453.0f;

	DWORD NewUIItemHotKey = GetUI_NewUIItemHotKey(CNewUISystem__GetInstance());

	for (int i = 0; i < 3; ++i)
	{
		int iIndex = GetHotKeyItemIndex(NewUIItemHotKey + 40, i, 0);

		if (iIndex != -1)
		{
			DWORD pItem = FindItem(InventoryThis(pWindowThis()), iIndex);

			if (pItem)
			{
				x = 209.0f + (i * 31);

				pRenderItem3D(x, y, width, height, *(WORD*)pItem, *(DWORD*)(pItem + 2), 0, 0, 0);
			}
		}
	}
}

void RenderItemCount() // OK
{
	float x = 226.0f;

	float y = 447.0f;

	EnableAlphaTest(true);

	glColor3f(1.0f, 0.95f, 0.75f);

	DWORD NewUIItemHotKey = GetUI_NewUIItemHotKey(CNewUISystem__GetInstance());

	for (int i = 0; i < 3; ++i)
	{
		int iCount = GetHotKeyItemIndex(NewUIItemHotKey + 40, i, true);

		if (iCount > 0)
		{
			x = 226.0f + (i * 31.0f);

			RenderNumber2D(x, y, iCount, 9.0f, 10.0f);
		}
	}
}

void UseItemRButton() // OK
{
	float width = 20.0f;

	float height = 20.0f;

	float x = 209.0f;

	float y = 453.0f;

	DWORD NewUIItemHotKey = GetUI_NewUIItemHotKey(CNewUISystem__GetInstance());

	for (int i = 0; i < 3; ++i)
	{
		x = 209.0f + (i * 31);

		if (pMouseOnZone((int)x, (int)y, (int)width, (int)height, 1) == true)
		{
			if (MouseRButtonPush)
			{
				MouseRButtonPush = false;

				int iIndex = GetHotKeyItemIndex(NewUIItemHotKey + 40, i, 0);

				if (iIndex != -1)
				{
					/*PMSG_ITEM_USE_SEND pMsg;

					pMsg.header.set(0x26,sizeof(pMsg));

					pMsg.SourceSlot = iIndex;

					pMsg.TargetSlot = 0xFF;

					pMsg.type = 0;

					DataSend((BYTE*)&pMsg,pMsg.header.size);*/

					return;
				}
			}
		}
	}
}

void RenderCurrentSkill() // OK
{
	glColor3f(1.0f, 0.9f, 0.8f);

	float x = 310.0f;

	float y = 448.0f;

	float width = 32.0f;

	float height = 38.0f;

	// use lock
	BYTE bySkillNumber = *(BYTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x76);

	// If the number of skills is 1 or more
	if (bySkillNumber > 0)
	{
		DWORD NewUISkillList = GetUI_NewUISkillList(CNewUISystem__GetInstance());

		DrawInterface(IMAGE_SKILLBOX, x - 6, y - 5, width, height);

		// Render the currently used skill icon
		RenderSkillIcon(NewUISkillList, *(BYTE*)(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+50), x, y, 20.0f, 28.0f);
	}
}

bool RenderSkillList() // OK
{
	DWORD This = GetUI_NewUISkillList(CNewUISystem__GetInstance());

	// use lock
	BYTE bySkillNumber = *(BYTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+118);

	// If the number of skills is 1 or more
	if (bySkillNumber > 0)
	{
		// render the skill list
		if (*(bool*)(This + 64) == true)
		{
			float x = 304.0f;

			float y = 390.0f;

			float width = 32.0f;

			float height = 38.0f;

			float fOrigX = 304.0f;

			int iSkillType = 0;

			int iSkillCount = 0;

			for (int i = 0; i < 64; ++i)
			{
				// use lock
				iSkillType = *(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+2 * i + 120);

				if (iSkillType != 0 && (iSkillType < 67 || iSkillType > 72))
				{
					// use lock
					BYTE bySkillUseType = *(BYTE *)(*(DWORD*)0x079FAE54 + 80 * iSkillType + 55);

					if (bySkillUseType == 1 || bySkillUseType == 3)
					{
						continue;
					}

					iSkillCount++;
				}
			}

			if ((iSkillCount % 2) == 0 && iSkillCount < 13)
			{
				x += (width / 2);

				fOrigX += (width / 2);
			}

			iSkillCount = 0;

			for (int i = 0; i < 64; ++i)
			{
				// use lock
				iSkillType = *(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+2 * i + 120);

				if (iSkillType != 0 && (iSkillType < 67 || iSkillType > 72))
				{
					// use lock
					BYTE bySkillUseType = *(BYTE *)(*(DWORD*)0x079FAE54 + 80 * iSkillType + 55);

					if (bySkillUseType == 1 || bySkillUseType == 3)
					{
						continue;
					}

					if (iSkillCount == 13)
					{
						y -= height;
					}

					if (iSkillCount < 13)
					{
						int iRemainder = iSkillCount % 2;
						int iQuotient = iSkillCount / 2;

						if (iRemainder == 0)
						{
							x = fOrigX + iQuotient * width;
						}
						else
						{
							x = fOrigX - (iQuotient + 1) * width;
						}
					}
					else
					{
						x = fOrigX - (6 * width) + ((iSkillCount - 13) * width);
					}

					iSkillCount++;

					glColor3f(1.0f, 0.9f, 0.8f);

					DrawInterface(IMAGE_SKILLBOX, x, y, width, height);

					if (i != *(BYTE*)(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+50))
					{
						glColor3f(0.6f, 0.7f, 0.8f);
					}

					RenderSkillIcon(This, i, x + 6, y + 5, 20.0f, 28.0f);
				}
			}

			// Pet
			if (*(DWORD*)(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+592))
			{
				x = 256;

				y = 352;

				for (int i = 120; i < 124; ++i)
				{
					glColor3f(1.0f, 0.9f, 0.8f);

					DrawInterface(IMAGE_SKILLBOX, x, y, width, height);

					if (i != *(BYTE*)(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+50))
					{
						glColor3f(0.6f, 0.7f, 0.8f);
					}

					RenderSkillIcon(This, i, x + 6, y + 5, 20.0f, 28.0f);

					x += width;
				}
			}
		}
	}

	// Render skill information
	if (*(BYTE *)(This + 65) == 1 && *(DWORD *)(This + 16))
	{
		*(int*)(This + 76) -= 20;

		RenderSkillInfo(This);

		*(BYTE *)(This + 65) = 0;
	}

	return true;
}

bool SkillListMouseEvent() // OK
{
	DWORD This = GetUI_NewUISkillList(CNewUISystem__GetInstance());

	// use lock
	BYTE bySkillNumber = *(BYTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+118); // number of skills

	*(bool*)(This + 65) = false; // m_bRenderSkillInfo

	if (bySkillNumber <= 0)
	{
		return true;
	}

	// currently used skill
	float x = 310.0f;

	float y = 448.0f;

	float width = 20.0f;

	float height = 28.0f;

	if (*(EVENT_STATE*)(This + 80) == EVENT_NONE && MouseLButtonPush == false && pMouseOnZone((int)x, (int)y, (int)width, (int)height, 1) == true)
	{
		*(EVENT_STATE*)(This + 80) = EVENT_BTN_HOVER_CURRENTSKILL;

		return true;
	}

	if (*(EVENT_STATE*)(This + 80) == EVENT_BTN_HOVER_CURRENTSKILL && MouseLButtonPush == false && pMouseOnZone((int)x, (int)y, (int)width, (int)height, 1) == false)
	{
		*(EVENT_STATE*)(This + 80) = EVENT_NONE;

		return true;
	}

	if (*(EVENT_STATE*)(This + 80) == EVENT_BTN_HOVER_CURRENTSKILL && (MouseLButtonPush == true) && pMouseOnZone((int)x, (int)y, (int)width, (int)height, 1) == true)
	{
		*(EVENT_STATE*)(This + 80) = EVENT_BTN_DOWN_CURRENTSKILL;

		return false;
	}

	if (*(EVENT_STATE*)(This + 80) == EVENT_BTN_DOWN_CURRENTSKILL)
	{
		if (MouseLButtonPush == false)
		{
			if (pMouseOnZone((int)x, (int)y, (int)width, (int)height, 1) == true)
			{
				*(bool*)(This + 64) ^= true;

				pPlayBuffer(25, 0, 0);

				*(EVENT_STATE*)(This + 80) = EVENT_NONE;

				return false;
			}

			*(EVENT_STATE*)(This + 80) = EVENT_NONE;

			return true;
		}
	}

	if (*(EVENT_STATE*)(This + 80) == EVENT_BTN_HOVER_CURRENTSKILL)
	{
		*(bool*)(This + 65) = true;

		*(int*)(This + 68) = *(BYTE*)(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+50);

		*(int*)(This + 72) = (int)(x - 5);

		*(int*)(This + 76) = (int)y;
		return false;
	}
	else if (*(EVENT_STATE*)(This + 80) == EVENT_BTN_DOWN_CURRENTSKILL)
	{
		return false;
	}

	// list of skills
	if (*(bool*)(This + 64) == false) //// From below, only processing if there is a skill list ////
	{
		return true;
	}

	WORD bySkillType = 0;

	int iSkillCount = 0;

	bool bMouseOnSkillList = false;

	x = 304.0f;;

	y = 390.0f;

	width = 32.0f;;

	height = 38.0f;;

	float fOrigX = 304.0f;

	EVENT_STATE PrevEventState = *(EVENT_STATE*)(This + 80);

	for (int i = 0; i < 64; ++i)
	{
		// use lock
		bySkillType = *(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+2 * i + 120);

		if (bySkillType != 0 && (bySkillType < 67 || bySkillType > 72))
		{
			// use lock
			BYTE bySkillUseType = *(BYTE *)(*(DWORD*)0x079FAE54 + 80 * bySkillType + 55);

			if (bySkillUseType == 1 || bySkillUseType == 3)
			{
				continue;
			}

			iSkillCount++;
		}
	}

	if ((iSkillCount % 2) == 0 && iSkillCount < 13)
	{
		x += (width / 2);

		fOrigX += (width / 2);
	}

	iSkillCount = 0;

	for (int i = 0; i < 64; ++i)
	{
		// If there is a skill type except for siege skills
		bySkillType = *(WORD *)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+2 * i + 120);

		if (bySkillType == 0 || (bySkillType >= 67 && bySkillType <= 72))
		{
			continue;
		}

		BYTE bySkillUseType = *(BYTE *)(*(DWORD*)(0x079FAE54) + 80 * bySkillType + 55);

		if (bySkillUseType == 3)
		{
			continue;
		}

		if (iSkillCount == 13)
		{
			y -= height;
		}

		if (iSkillCount < 13)
		{
			int iRemainder = iSkillCount % 2;
			int iQuotient = iSkillCount / 2;

			if (iRemainder == 0)
			{
				x = fOrigX + iQuotient * width;
			}
			else
			{
				x = fOrigX - (iQuotient + 1) * width;
			}
		}
		else
		{
			x = fOrigX - (6 * width) + ((iSkillCount - 13) * width);
		}

		iSkillCount++;

		if (pMouseOnZone((int)x, (int)y, (int)width, (int)height, 1) == true)
		{
			bMouseOnSkillList = true;

			if (*(EVENT_STATE*)(This + 80) == EVENT_NONE && MouseLButtonPush == false)
			{
				*(EVENT_STATE*)(This + 80) = EVENT_BTN_HOVER_SKILLLIST;

				break;
			}
		}

		if (*(EVENT_STATE*)(This + 80) == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == true && pMouseOnZone((int)x, (int)y, (int)width, (int)height, 1) == true)
		{
			*(EVENT_STATE*)(This + 80) = EVENT_BTN_DOWN_SKILLLIST;

			break;
		}

		if (*(EVENT_STATE*)(This + 80) == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == false && pMouseOnZone((int)x, (int)y, (int)width, (int)height, 1) == true)
		{
			*(bool*)(This + 65) = true;

			*(int*)(This + 68) = i;

			*(int*)(This + 72) = (int)x;

			*(int*)(This + 76) = (int)y;
		}

		if (*(EVENT_STATE*)(This + 80) == EVENT_BTN_DOWN_SKILLLIST && MouseLButtonPush == false && *(int*)(This + 68) == i && pMouseOnZone((int)x, (int)y, (int)width, (int)height, 1) == true)
		{
			*(EVENT_STATE*)(This + 80) = EVENT_NONE;

			*(BYTE *)(This + 84) = *(BYTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+2 * *(BYTE *)(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+50) + 120);

			*(BYTE*)(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+50) = i;

			*(bool*)(This + 64) = false;

			pPlayBuffer(25, 0, 0);

			return false;
		}
	}

	if (PrevEventState != *(EVENT_STATE*)(This + 80))
	{
		if (*(EVENT_STATE*)(This + 80) == EVENT_NONE || *(EVENT_STATE*)(This + 80) == EVENT_BTN_HOVER_SKILLLIST)
		{
			return true;
		}

		return false;
	}

	// pet skill
	if (*(DWORD *)(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+592) != 0)
	{
		x = 256.0f;

		y = 352.0f;

		for (int i = 120; i < 124; ++i)
		{
			if (pMouseOnZone((int)x, (int)y, (int)width, (int)height, 1) == true)
			{
				bMouseOnSkillList = true;

				if (*(EVENT_STATE*)(This + 80) == EVENT_NONE && MouseLButtonPush == false)
				{
					*(EVENT_STATE*)(This + 80) = EVENT_BTN_HOVER_SKILLLIST;

					return true;
				}

				if (*(EVENT_STATE*)(This + 80) == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == true)
				{
					*(EVENT_STATE*)(This + 80) = EVENT_BTN_DOWN_SKILLLIST;

					return false;
				}

				if (*(EVENT_STATE*)(This + 80) == EVENT_BTN_HOVER_SKILLLIST)
				{
					*(bool*)(This + 65) = true;

					*(int*)(This + 68) = i;

					*(int*)(This + 72) = (int)x;

					*(int*)(This + 76) = (int)y;
				}

				if (*(EVENT_STATE*)(This + 80) == EVENT_BTN_DOWN_SKILLLIST && MouseLButtonPush == false
					&& *(int*)(This + 68) == i)
				{
					*(EVENT_STATE*)(This + 80) = EVENT_NONE;

					*(BYTE *)(This + 84) = *(BYTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+2 * *(BYTE *)(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+50) + 120);

					*(BYTE*)(*(DWORD*)(MAIN_VIEWPORT_STRUCT)+50) = i;

					*(bool*)(This + 64) = false;

					pPlayBuffer(25, 0, 0);

					return false;
				}
			}

			x += width + 3;
		}
	}

	if (bMouseOnSkillList == false && *(EVENT_STATE*)(This + 80) == EVENT_BTN_HOVER_SKILLLIST)
	{
		*(EVENT_STATE*)(This + 80) = EVENT_NONE;

		return true;
	}

	if (bMouseOnSkillList == false && MouseLButtonPush == false && *(EVENT_STATE*)(This + 80) == EVENT_BTN_DOWN_SKILLLIST)
	{
		*(EVENT_STATE*)(This + 80) = EVENT_NONE;

		return false;
	}

	if (*(EVENT_STATE*)(This + 80) == EVENT_BTN_DOWN_SKILLLIST)
	{
		if (MouseLButtonPush == false)
		{
			*(EVENT_STATE*)(This + 80) = EVENT_NONE;

			return true;
		}

		return false;
	}

	return true;
}

float RenderSkillNumber(float x, float y, int num, float scale) // OK
{
	return RenderNumber2D(x - 19.0f, y - 2.0f, num, 9.0f, 10.0f);
}

bool IsMasterLevel() // OK
{
	return ((((*(BYTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0B) >> 4) & 1) != 0) && (ViewLevel >= CharacterMaxLevel));
}