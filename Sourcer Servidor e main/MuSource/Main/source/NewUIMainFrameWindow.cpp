//////////////////////////////////////////////////////////////////////
// NewUIMainFrameWindow.cpp: implementation of the CNewUIMainFrameWindow class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NewUIMainFrameWindow.h"	// self
#include "NewUIOptionWindow.h"
#include "NewUISystem.h"
#include "UIBaseDef.h"
#include "DSPlaySound.h"
#include "ZzzInfomation.h"
#include "ZzzBMD.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "ZzzInventory.h"
#include "wsclientinline.h"
#include "CSItemOption.h"
#include "CSChaosCastle.h"
#include "MapManager.h"
#include "CharacterManager.h"
#include "SkillManager.h"
#include "GMDoppelGanger1.h"
#include "GMDoppelGanger2.h"
#include "GMDoppelGanger3.h"
#include "GMDoppelGanger4.h"
#include "./Time/CTimCheck.h"

extern float g_fScreenRate_x;
extern float g_fScreenRate_y;
extern int  MouseUpdateTime;
extern int  MouseUpdateTimeMax;
extern int SelectedCharacter;
extern int Attacking;

SEASON3B::CNewUIMainFrameWindow::CNewUIMainFrameWindow()
{
	m_bExpEffect = false;
	m_dwExpEffectTime = 0;
	m_dwPreExp = 0;
	m_dwGetExp = 0;
}

SEASON3B::CNewUIMainFrameWindow::~CNewUIMainFrameWindow()
{
	Release();
}

void SEASON3B::CNewUIMainFrameWindow::LoadImages()
{
	LoadBitmap("Common\\Interface\\ui_menu_left.jpg", IMAGE_MENU_1);
	LoadBitmap("Common\\Interface\\ui_menu_middle.jpg", IMAGE_MENU_2);
	LoadBitmap("Common\\Interface\\ui_menu_right.jpg", IMAGE_MENU_3);
	LoadBitmap("Common\\Interface\\ui_menu_dragon.tga", IMAGE_MENU_DRAGON);
	LoadBitmap("Common\\Interface\\ui_menu_mana.jpg", IMAGE_GAUGE_BLUE);
	LoadBitmap("Common\\Interface\\ui_menu_poison.jpg", IMAGE_GAUGE_GREEN);
	LoadBitmap("Common\\Interface\\ui_menu_health.jpg", IMAGE_GAUGE_RED);
	LoadBitmap("Common\\Interface\\ui_menu_ag.jpg", IMAGE_GAUGE_AG);
	LoadBitmap("Common\\Interface\\ui_menu_sd.jpg", IMAGE_GAUGE_SD);
	LoadBitmap("Common\\Interface\\ui_menu_btn_party.jpg", IMAGE_MENU_BTN_PARTY);
	LoadBitmap("Common\\Interface\\ui_menu_btn_character.jpg", IMAGE_MENU_BTN_CHAINFO);
	LoadBitmap("Common\\Interface\\ui_menu_btn_inventory.jpg", IMAGE_MENU_BTN_MYINVEN);
	LoadBitmap("Common\\Interface\\ui_menu_btn_friend.jpg", IMAGE_MENU_BTN_FRIEND);
	LoadBitmap("Common\\Interface\\ui_menu_btn_guild.jpg", IMAGE_MENU_BTN_GUILD);
	LoadBitmap("Common\\Interface\\ui_menu_btn_command.jpg", IMAGE_MENU_BTN_COMMAND);
}

void SEASON3B::CNewUIMainFrameWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_MENU_1);
	DeleteBitmap(IMAGE_MENU_2);
	DeleteBitmap(IMAGE_MENU_3);
	DeleteBitmap(IMAGE_MENU_DRAGON);
	DeleteBitmap(IMAGE_GAUGE_BLUE);
	DeleteBitmap(IMAGE_GAUGE_GREEN);
	DeleteBitmap(IMAGE_GAUGE_RED);
	DeleteBitmap(IMAGE_GAUGE_AG);
	DeleteBitmap(IMAGE_GAUGE_SD);

	DeleteBitmap(IMAGE_MENU_BTN_PARTY);
	DeleteBitmap(IMAGE_MENU_BTN_CHAINFO);
	DeleteBitmap(IMAGE_MENU_BTN_MYINVEN);
	DeleteBitmap(IMAGE_MENU_BTN_FRIEND);
	DeleteBitmap(IMAGE_MENU_BTN_GUILD);
	DeleteBitmap(IMAGE_MENU_BTN_COMMAND);
}

bool SEASON3B::CNewUIMainFrameWindow::Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng)
{
	if (NULL == pNewUIMng || NULL == pNewUI3DRenderMng)
	{
		return false;
	}

	m_pNewUIMng = pNewUIMng;

	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_MAINFRAME, this);

	m_pNewUI3DRenderMng = pNewUI3DRenderMng;

	m_pNewUI3DRenderMng->Add3DRenderObj(this, ITEMHOTKEYNUMBER_CAMERA_Z_ORDER);

	LoadImages();

	Show(true);

	return true;
}

void SEASON3B::CNewUIMainFrameWindow::Release()
{
	UnloadImages();

	if (m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->Remove3DRenderObj(this);
		m_pNewUI3DRenderMng = NULL;
	}

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

bool SEASON3B::CNewUIMainFrameWindow::Render()
{
	EnableAlphaTest();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	this->RenderFrame();

	this->RenderDragonLeft();
	this->RenderDragonRight();

	this->m_pNewUI3DRenderMng->RenderUI2DEffect(ITEMHOTKEYNUMBER_CAMERA_Z_ORDER, UI2DEffectCallback, this, 0, 0);

	g_pSkillList->RenderCurrentSkillAndHotSkillList();

	this->RenderLifeMana();
	this->RenderGuageAG();
	this->RenderGuageSD();
	this->RenderExperience();

	this->RenderButtonInfo();

	DisableAlphaBlend();

	return true;
}

void SEASON3B::CNewUIMainFrameWindow::Render3D()
{
	m_ItemHotKey.RenderItems();
}

void SEASON3B::CNewUIMainFrameWindow::UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB)
{
	g_pMainFrame->RenderHotKeyItemCount();

}

bool SEASON3B::CNewUIMainFrameWindow::IsVisible() const
{
	return CNewUIObj::IsVisible();
}

void SEASON3B::CNewUIMainFrameWindow::RenderFrame()
{
	RenderBitmap(IMAGE_MENU_1, 0.0, 432.0, 256.0, 48.0, 0, 0, 1.0, 0.75, 1, 1, 0.0);
	RenderBitmap(IMAGE_MENU_2, 256.0, 432.0, 128.0, 48.0, 0, 0, 1.0, 0.75, 1, 1, 0.0);
	RenderBitmap(IMAGE_MENU_3, 384.0, 432.0, 256.0, 48.0, 0, 0, 1.0, 0.75, 1, 1, 0.0);

	glColor3f(0.60000002, 0.60000002, 0.60000002);
	SEASON3B::RenderOldNumber(24.0f, 462.0f, Hero->PositionX, 8.0f, 9.0f);

	glColor3f(0.60000002, 0.60000002, 0.60000002);
	SEASON3B::RenderOldNumber(48.0f, 462.0f, Hero->PositionY, 8.0f, 9.0f);
}

void SEASON3B::CNewUIMainFrameWindow::RenderDragonLeft()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MINI_MAP) == true)
	{
		return;
	}

	RenderBitmap(IMAGE_MENU_DRAGON, 0.0, 387.0, 108.0, 45.0, 0, 0, 0.84375, 0.703125, 1, 1, 0.0);
}

void SEASON3B::CNewUIMainFrameWindow::RenderDragonRight()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_FRIEND) == true ||
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_PARTY) == true ||
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MYQUEST) == true ||
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GUILDINFO) == true ||
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_COMMAND) == true ||
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY) == true ||
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER) == true ||
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_WINDOW_MENU) == true ||
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INGAMESHOP) == true ||
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MINI_MAP) == true ||
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_BLOODCASTLE_TIME) == true ||
		g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_DUEL_WINDOW) == true)
	{
		return;
	}

	RenderBitmap(IMAGE_MENU_DRAGON, 532.0, 387.0, 108.0, 45.0, 0.84, 0, -0.84375, 0.703125, 1, 1, 0.0);
}

void SEASON3B::CNewUIMainFrameWindow::RenderLifeMana()
{
	DWORD wLifeMax, wLife, wManaMax, wMana;

	wLifeMax = CharacterAttribute->LifeMax;
	wLife = min(max(0, CharacterAttribute->Life), wLifeMax);

	wManaMax = CharacterAttribute->ManaMax;
	wMana = min(max(0, CharacterAttribute->Mana), wManaMax);

	if (wLifeMax > 0)
	{
		if (wLife > 0 && (wLife / (float)wLifeMax) < 0.2f)
		{
			PlayBuffer(SOUND_HEART);
		}
	}

	float fLife = 0.f;
	float fMana = 0.f;

	if (wLifeMax > 0)
	{
		fLife = (wLifeMax - wLife) / (float)wLifeMax;
	}
	if (wManaMax > 0)
	{
		fMana = (wManaMax - wMana) / (float)wManaMax;
	}

	float width, height;
	float x, y;
	float fY, fH, fV;

	// life
	width = 52.f;
	x = 98.f;
	height = 51.f;
	y = 432.f;

	fY = y + (fLife * height);
	fH = height - (fLife * height);
	fV = fLife;

	if (g_isCharacterBuff((&Hero->Object), eDeBuff_Poison))
	{
		RenderBitmap(IMAGE_GAUGE_GREEN, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.0f - fV) * height / 64.f);
	}
	else
	{
		RenderBitmap(IMAGE_GAUGE_RED, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.0f - fV) * height / 64.f);
	}

	glColor3f(1.0, 0.94999999, 0.75);

	SEASON3B::RenderOldNumber(x + 35.0f, y + 35.0f, wLife, 9.0f, 10.0f);

	char strTipText[256];

	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		sprintf(strTipText, GlobalText[358], wLife, wLifeMax);

		RenderTipText((int)x, (int)418, strTipText);
	}

	// mana
	width = 52.f;
	x = 489.f;
	height = 51.0f;
	y = 432.0f;

	fY = y + (fMana * height);
	fH = height - (fMana * height);
	fV = fMana;

	RenderBitmap(IMAGE_GAUGE_BLUE, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.0f - fV) * height / 64.f);

	glColor3f(1.0, 0.94999999, 0.75);

	SEASON3B::RenderOldNumber(x + 31.0f, y + 35.0f, wMana, 9.0f, 10.0f);

	// mana
	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		sprintf(strTipText, GlobalText[359], wMana, wManaMax);
		RenderTipText((int)x, (int)418, strTipText);
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderGuageAG()
{
	float x, y, width, height;
	float fY, fH, fV;

	DWORD dwMaxSkillMana, dwSkillMana;

	dwMaxSkillMana = max(1, CharacterAttribute->SkillManaMax);
	dwSkillMana = min(dwMaxSkillMana, CharacterAttribute->SkillMana);

	float fSkillMana = 0.0f;

	if (dwMaxSkillMana > 0)
	{
		fSkillMana = (dwMaxSkillMana - dwSkillMana) / (float)dwMaxSkillMana;
	}

	width = 15.0f, height = 36.0f;
	x = 551.0f; y = 438.7f;
	fY = y + (fSkillMana * height);
	fH = height - (fSkillMana * height);
	fV = fSkillMana;

	RenderBitmap(IMAGE_GAUGE_AG, x, fY, width, fH, 0.f, fV * height / 64.f, width / 16.f, (1.0f - fV) * height / 64.f);

	glColor3f(1.0, 0.94999999, 0.75);

	SEASON3B::RenderOldNumber(x + 12.0f, y + 29.7f, (int)dwSkillMana, 9.0f, 10.0f);

	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		char strTipText[256];

		sprintf(strTipText, GlobalText[214], dwSkillMana, dwMaxSkillMana);

		RenderTipText((int)x - 20, (int)418, strTipText);
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderGuageSD()
{
	float x, y, width, height;
	float fY, fH, fV;
	
	DWORD wMaxShield, wShield;

	wMaxShield = max(1, CharacterAttribute->ShieldMax);
	wShield = min(wMaxShield, CharacterAttribute->Shield);

	float fShield = 0.0f;

	if (wMaxShield > 0)
	{
		fShield = (wMaxShield - wShield) / (float)wMaxShield;
	}

	width = 15.0f, height = 36.0f;
	x = 73.0f; y = 438.7f;
	fY = y + (fShield * height);
	fH = height - (fShield * height);
	fV = fShield;

	RenderBitmap(IMAGE_GAUGE_SD, x, fY, width, fH, 0.f, fV * height / 64.f, width / 16.f, (1.0f - fV) * height / 64.f);

	glColor3f(1.0, 0.94999999, 0.75);

	SEASON3B::RenderOldNumber(x + 12.0f, y + 29.7f, (int)wShield, 9.0f, 10.0f);

	height = 39.0f;
	y = 431.0f;

	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		char strTipText[256];

		sprintf(strTipText, GlobalText[2037], wShield, wMaxShield);
		RenderTipText((int)x - 20, (int)418, strTipText);
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderExperience()
{
	__int64 wLevel;
	__int64 dwNexExperience;
	__int64 dwExperience;
	double x, y, width, height;

	wLevel = CharacterAttribute->Level;
	dwNexExperience = CharacterAttribute->NextExperince;
	dwExperience = CharacterAttribute->Experience;

	glColor3f(0.91000003, 0.81, 0.60000002);

	x = 0; y = 198; width = 6; height = 4;

	WORD wPriorLevel = wLevel - 1;
	DWORD dwPriorExperience = 0;

	if (wPriorLevel > 0)
	{
		dwPriorExperience = (9 + wPriorLevel) * wPriorLevel * wPriorLevel * 10;

		if (wPriorLevel > 255)
		{
			int iLevelOverN = wPriorLevel - 255;

			dwPriorExperience += (9 + iLevelOverN) * iLevelOverN * iLevelOverN * 1000;
		}
	}

	float fNeedExp = dwNexExperience - dwPriorExperience;

	float fExp = dwExperience - dwPriorExperience;

	if (dwExperience < dwPriorExperience)
	{
		fExp = 0.f;
	}

	float fExpBarNum = 0.f;

	if (fExp > 0.f && fNeedExp > 0)
	{
		fExpBarNum = (fExp / fNeedExp) * 10.f;
	}

	float fProgress = fExpBarNum;

	fProgress = fProgress - (int)fProgress;

	x = 220.f; y = 439.f; width = fProgress * 198.f; height = 4.7f;

	RenderColor(x, y, width, height);

	EndRenderColor();

	int iExp = (int)fExpBarNum;

	x = 425.f; y = 433.5f;

	SEASON3B::RenderOldNumber(x, y, (int)iExp, 9.0f, 10.0f);

	x = 220.f; y = 438.f; width = 198.f; height = 5.8f;

	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		char strTipText[256];

		sprintf(strTipText, GlobalText[1748], dwExperience, dwNexExperience);
		RenderTipText(280, 418, strTipText);
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderHotKeyItemCount()
{
	m_ItemHotKey.RenderItemCount();
}

void SEASON3B::CNewUIMainFrameWindow::RenderButtons()
{
	m_BtnChaInfo.Render();

	m_BtnMyInven.Render();

	RenderFriendButton();

	m_BtnWindow.Render();
}

void SEASON3B::CNewUIMainFrameWindow::RenderFriendButton()
{
	m_BtnFriend.Render();

	int iBlinkTemp = g_pFriendMenu->GetBlinkTemp();

	BOOL bIsAlertTime = (iBlinkTemp % 24 < 12);

	if (g_pFriendMenu->IsNewChatAlert() && bIsAlertTime)
	{
		RenderFriendButtonState();
	}
	if (g_pFriendMenu->IsNewMailAlert())
	{
		if (bIsAlertTime)
		{
			RenderFriendButtonState();

			if (iBlinkTemp % 24 == 11)
			{
				g_pFriendMenu->IncreaseLetterBlink();
			}
		}
	}
	else if (g_pLetterList->CheckNoReadLetter())
	{
		RenderFriendButtonState();
	}

	g_pFriendMenu->IncreaseBlinkTemp();
}

void SEASON3B::CNewUIMainFrameWindow::RenderFriendButtonState()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_FRIEND) == true)
	{
		RenderImage(IMAGE_MENU_BTN_FRIEND, 488 + 76, 480 - 51, 38, 42, 0.0f, 126.f);
	}
	else
	{
		RenderImage(IMAGE_MENU_BTN_FRIEND, 488 + 76, 480 - 51, 38, 42, 0.0f, 42.f);
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderButtonInfo()
{
	char szBuffer[256] = { 0 };

	int X, Y, Width, Height;

	X = 348, Y = 449, Width = 25, Height = 25;

	if (SEASON3B::CheckMouseIn(X, Y, Width, Height) == true)
	{
		sprintf(szBuffer, "Party (P)");

		RenderTipText(X - 3, Y - 29, szBuffer);
	}

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_PARTY) == true)
	{
		RenderBitmap(IMAGE_MENU_BTN_PARTY, (float)X, 452.0, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
	}

	X = 379, Y = 449, Width = 25, Height = 25;

	if (SEASON3B::CheckMouseIn(X, Y, Width, Height) == true)
	{
		sprintf(szBuffer, "Character (C)");

		RenderTipText(X - 4, Y - 29, szBuffer);
	}

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER) == true)
	{
		RenderBitmap(IMAGE_MENU_BTN_CHAINFO, (float)X, 452.0, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
	}

	X = 409, Y = 449, Width = 25, Height = 25;

	if (SEASON3B::CheckMouseIn(X, Y, Width, Height) == true)
	{
		sprintf(szBuffer, "Inventory (I,V)");

		RenderTipText(X - 4, Y - 29, szBuffer);
	}

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY) == true)
	{
		RenderBitmap(IMAGE_MENU_BTN_MYINVEN, 410.0, 452.0, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
	}

	X = 581, Y = 432, Width = 52, Height = 18;

	if (SEASON3B::CheckMouseIn(X, Y, Width, Height) == true)
	{
		sprintf(szBuffer, "Friends (F)");

		RenderTipText(X - 1, Y - 12, szBuffer);
	}

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_FRIEND) == true)
	{
		RenderBitmap(IMAGE_MENU_BTN_FRIEND, 581.0, 435.0, 52.0, 18.0, 0, 0, 0.8125, 0.5625, 1, 1, 0.0);
	}

	X = 581, Y = 454, Width = 52, Height = 18;

	if (SEASON3B::CheckMouseIn(X, Y, Width, Height) == true)
	{
		sprintf(szBuffer, "Guild (G)");

		RenderTipText(X - 1, Y - 10, szBuffer);
	}

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GUILDINFO) == true)
	{
		RenderBitmap(IMAGE_MENU_BTN_GUILD, 581.0, 458.0, 52.0, 18.0, 0, 0, 0.8125, 0.5625, 1, 1, 0.0);
	}

	X = 5, Y = 432, Width = 52, Height = 18;

	if (SEASON3B::CheckMouseIn(X, Y, Width, Height) == true)
	{
		sprintf(szBuffer, "Commands (D)");

		RenderTipText(X + 8, Y - 12, szBuffer);
	}

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_COMMAND) == true)
	{
		RenderBitmap(IMAGE_MENU_BTN_COMMAND, 6.0, 435.0, 53.0, 19, 0, 0, (53 / 64.0), (19 / 32.0), 1, 1, 0.0);
	}
}

bool SEASON3B::CNewUIMainFrameWindow::UpdateMouseEvent()
{
	if (g_pNewUIHotKey->IsStateGameOver() == true)
	{
		return true;
	}

	if (BtnProcess() == true)
	{
		return false;
	}

	return true;
}

bool SEASON3B::CNewUIMainFrameWindow::BtnProcess()
{
	int X, Y, Width, Height;

	X = 348, Y = 449, Width = 25, Height = 25;

	if(SEASON3B::CheckMouseIn(X, Y, Width, Height) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_PARTY);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	X = 379, Y = 449, Width = 25, Height = 25;

	if (SEASON3B::CheckMouseIn(X, Y, Width, Height) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_CHARACTER);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	X = 409, Y = 449, Width = 25, Height = 25;

	if (SEASON3B::CheckMouseIn(X, Y, Width, Height) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_INVENTORY);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	X = 581, Y = 432, Width = 52, Height = 18;

	if (SEASON3B::CheckMouseIn(X, Y, Width, Height) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		int iLevel = CharacterAttribute->Level;

		if (iLevel < 6)
		{
			g_pChatListBox->AddText("", GlobalText[1067], SEASON3B::TYPE_SYSTEM_MESSAGE);
		}
		else
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_FRIEND);
		}

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	X = 581, Y = 454, Width = 52, Height = 18;

	if (SEASON3B::CheckMouseIn(X, Y, Width, Height) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_GUILDINFO);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	X = 5, Y = 432, Width = 52, Height = 18;

	if (SEASON3B::CheckMouseIn(X, Y, Width, Height) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_COMMAND);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	return false;
}

bool SEASON3B::CNewUIMainFrameWindow::UpdateKeyEvent()
{
	if (m_ItemHotKey.UpdateKeyEvent() == false)
	{
		return false;
	}
	return true;
}

bool SEASON3B::CNewUIMainFrameWindow::Update()
{
	if (m_bExpEffect == true)
	{
		if (timeGetTime() - m_dwExpEffectTime > 2000)
		{
			m_bExpEffect = false;
			m_dwExpEffectTime = 0;
			m_dwGetExp = 0;
		}
	}

	return true;
}

float SEASON3B::CNewUIMainFrameWindow::GetLayerDepth()
{
	return 10.6f;
}

float SEASON3B::CNewUIMainFrameWindow::GetKeyEventOrder()
{
	return 2.9f;
}

void SEASON3B::CNewUIMainFrameWindow::SetItemHotKey(int iHotKey, int iItemType, int iItemLevel)
{
	m_ItemHotKey.SetHotKey(iHotKey, iItemType, iItemLevel);
}

int SEASON3B::CNewUIMainFrameWindow::GetItemHotKey(int iHotKey)
{
	return m_ItemHotKey.GetHotKey(iHotKey);
}

int SEASON3B::CNewUIMainFrameWindow::GetItemHotKeyLevel(int iHotKey)
{
	return m_ItemHotKey.GetHotKeyLevel(iHotKey);
}

void SEASON3B::CNewUIMainFrameWindow::UseHotKeyItemRButton()
{
	m_ItemHotKey.UseItemRButton();
}

void SEASON3B::CNewUIMainFrameWindow::UpdateItemHotKey()
{
	m_ItemHotKey.UpdateKeyEvent();
}

void SEASON3B::CNewUIMainFrameWindow::ResetSkillHotKey()
{
	g_pSkillList->Reset();
}

void SEASON3B::CNewUIMainFrameWindow::SetSkillHotKey(int iHotKey, int iSkillType)
{
	g_pSkillList->SetHotKey(iHotKey, iSkillType);
}

int SEASON3B::CNewUIMainFrameWindow::GetSkillHotKey(int iHotKey)
{
	return g_pSkillList->GetHotKey(iHotKey);
}

int SEASON3B::CNewUIMainFrameWindow::GetSkillHotKeyIndex(int iSkillType)
{
	return g_pSkillList->GetSkillIndex(iSkillType);
}

SEASON3B::CNewUIItemHotKey::CNewUIItemHotKey()
{
	for (int i = 0; i < HOTKEY_COUNT; ++i)
	{
		m_iHotKeyItemType[i] = -1;
		m_iHotKeyItemLevel[i] = 0;
	}
}

SEASON3B::CNewUIItemHotKey::~CNewUIItemHotKey()
{

}

bool SEASON3B::CNewUIItemHotKey::UpdateKeyEvent()
{
	int iIndex = -1;

	if (SEASON3B::IsPress('Q') == true)
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_Q);
	}
	else if (SEASON3B::IsPress('W') == true)
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_W);
	}
	else if (SEASON3B::IsPress('E') == true)
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_E);
	}
	else if (SEASON3B::IsPress('R') == true)
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_R);
	}

	if (iIndex != -1)
	{
		ITEM* pItem = NULL;
		pItem = g_pMyInventory->FindItem(iIndex);
		if ((pItem->Type >= ITEM_POTION + 78 && pItem->Type <= ITEM_POTION + 82))
		{
			std::list<eBuffState> secretPotionbufflist;
			secretPotionbufflist.push_back(eBuff_SecretPotion1);
			secretPotionbufflist.push_back(eBuff_SecretPotion2);
			secretPotionbufflist.push_back(eBuff_SecretPotion3);
			secretPotionbufflist.push_back(eBuff_SecretPotion4);
			secretPotionbufflist.push_back(eBuff_SecretPotion5);

			if (g_isCharacterBufflist((&Hero->Object), secretPotionbufflist) != eBuffNone) {
				SEASON3B::CreateOkMessageBox(GlobalText[2530], RGBA(255, 30, 0, 255));
			}
			else {
				SendRequestUse(iIndex, 0);
			}
		}
		else

		{
			SendRequestUse(iIndex, 0);
		}
		return false;
	}

	return true;
}

int SEASON3B::CNewUIItemHotKey::GetHotKeyItemIndex(int iType, bool bItemCount)
{
	int iStartItemType = 0, iEndItemType = 0;
	int i, j;

	switch (iType)
	{
	case HOTKEY_Q:
		if (GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{
			if (m_iHotKeyItemType[iType] >= ITEM_POTION + 4 && m_iHotKeyItemType[iType] <= ITEM_POTION + 6)
			{
				iStartItemType = ITEM_POTION + 6; iEndItemType = ITEM_POTION + 4;
			}
			else
			{
				iStartItemType = ITEM_POTION + 3; iEndItemType = ITEM_POTION + 0;
			}
		}
		break;
	case HOTKEY_W:
		if (GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{
			if (m_iHotKeyItemType[iType] >= ITEM_POTION + 0 && m_iHotKeyItemType[iType] <= ITEM_POTION + 3)
			{
				iStartItemType = ITEM_POTION + 3; iEndItemType = ITEM_POTION + 0;
			}
			else
			{
				iStartItemType = ITEM_POTION + 6; iEndItemType = ITEM_POTION + 4;
			}
		}
		break;
	case HOTKEY_E:
		if (GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{
			if (m_iHotKeyItemType[iType] >= ITEM_POTION + 0 && m_iHotKeyItemType[iType] <= ITEM_POTION + 3)
			{
				iStartItemType = ITEM_POTION + 3; iEndItemType = ITEM_POTION + 0;
			}
			else if (m_iHotKeyItemType[iType] >= ITEM_POTION + 4 && m_iHotKeyItemType[iType] <= ITEM_POTION + 6)
			{
				iStartItemType = ITEM_POTION + 6; iEndItemType = ITEM_POTION + 4;
			}
			else
			{
				iStartItemType = ITEM_POTION + 8; iEndItemType = ITEM_POTION + 8;
			}
		}
		break;
	case HOTKEY_R:
		if (GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{
			if (m_iHotKeyItemType[iType] >= ITEM_POTION + 0 && m_iHotKeyItemType[iType] <= ITEM_POTION + 3)
			{
				iStartItemType = ITEM_POTION + 3; iEndItemType = ITEM_POTION + 0;
			}
			else if (m_iHotKeyItemType[iType] >= ITEM_POTION + 4 && m_iHotKeyItemType[iType] <= ITEM_POTION + 6)
			{
				iStartItemType = ITEM_POTION + 6; iEndItemType = ITEM_POTION + 4;
			}
			else
			{
				iStartItemType = ITEM_POTION + 37; iEndItemType = ITEM_POTION + 35;
			}
		}
		break;
	}

	int iItemCount = 0;
	ITEM* pItem = NULL;

	int iNumberofItems = g_pMyInventory->GetInventoryCtrl()->GetNumberOfItems();
	for (i = iStartItemType; i >= iEndItemType; --i)
	{
		if (bItemCount)
		{
			for (j = 0; j < iNumberofItems; ++j)
			{
				pItem = g_pMyInventory->GetInventoryCtrl()->GetItem(j);
				if (pItem == NULL)
				{
					continue;
				}

				if (
					(pItem->Type == i && ((pItem->Level >> 3) & 15) == m_iHotKeyItemLevel[iType])
					|| (pItem->Type == i && (pItem->Type >= ITEM_POTION + 0 && pItem->Type <= ITEM_POTION + 3))
					)
				{
					if (pItem->Type == ITEM_POTION + 9
						|| pItem->Type == ITEM_POTION + 10
						|| pItem->Type == ITEM_POTION + 20
						)
					{
						iItemCount++;
					}
					else
					{
						iItemCount += pItem->Durability;
					}
				}
			}
		}
		else
		{
			int iIndex = -1;
			if (i >= ITEM_POTION + 0 && i <= ITEM_POTION + 3)
			{
				iIndex = g_pMyInventory->FindItemReverseIndex(i);
			}
			else
			{
				iIndex = g_pMyInventory->FindItemReverseIndex(i, m_iHotKeyItemLevel[iType]);
			}

			if (-1 != iIndex)
			{
				pItem = g_pMyInventory->FindItem(iIndex);
				if ((pItem->Type != ITEM_POTION + 7
					&& pItem->Type != ITEM_POTION + 10
					&& pItem->Type != ITEM_POTION + 20)
					|| ((pItem->Level >> 3) & 15) == m_iHotKeyItemLevel[iType]
					)
				{
					return iIndex;
				}
			}
		}
	}

	if (bItemCount == true)
	{
		return iItemCount;
	}

	return -1;
}

bool SEASON3B::CNewUIItemHotKey::GetHotKeyCommonItem(IN int iHotKey, OUT int& iStart, OUT int& iEnd)
{
	switch (m_iHotKeyItemType[iHotKey])
	{
	case ITEM_POTION + 7:
	case ITEM_POTION + 8:
	case ITEM_POTION + 9:
	case ITEM_POTION + 10:
	case ITEM_POTION + 20:
	case ITEM_POTION + 46:
	case ITEM_POTION + 47:
	case ITEM_POTION + 48:
	case ITEM_POTION + 49:
	case ITEM_POTION + 50:
	case ITEM_POTION + 70:
	case ITEM_POTION + 71:
	case ITEM_POTION + 78:
	case ITEM_POTION + 79:
	case ITEM_POTION + 80:
	case ITEM_POTION + 81:
	case ITEM_POTION + 82:
	case ITEM_POTION + 94:
	case ITEM_POTION + 85:
	case ITEM_POTION + 86:
	case ITEM_POTION + 87:
	case ITEM_POTION + 133:
		if (m_iHotKeyItemType[iHotKey] != ITEM_POTION + 20 || m_iHotKeyItemLevel[iHotKey] == 0)
		{
			iStart = iEnd = m_iHotKeyItemType[iHotKey];
			return true;
		}
		break;
	default:
		if (m_iHotKeyItemType[iHotKey] >= ITEM_POTION + 35 && m_iHotKeyItemType[iHotKey] <= ITEM_POTION + 37)
		{
			iStart = ITEM_POTION + 37; iEnd = ITEM_POTION + 35;
			return true;
		}
		else if (m_iHotKeyItemType[iHotKey] >= ITEM_POTION + 38 && m_iHotKeyItemType[iHotKey] <= ITEM_POTION + 40)
		{
			iStart = ITEM_POTION + 40; iEnd = ITEM_POTION + 38;
			return true;
		}
		break;
	}
	return false;
}

int SEASON3B::CNewUIItemHotKey::GetHotKeyItemCount(int iType)
{
	return 0;
}

void SEASON3B::CNewUIItemHotKey::SetHotKey(int iHotKey, int iItemType, int iItemLevel)
{
	if (iHotKey != -1 && CNewUIMyInventory::CanRegisterItemHotKey(iItemType) == true
		)
	{
		m_iHotKeyItemType[iHotKey] = iItemType;
		m_iHotKeyItemLevel[iHotKey] = iItemLevel;
	}
	else
	{
		m_iHotKeyItemType[iHotKey] = -1;
		m_iHotKeyItemLevel[iHotKey] = 0;
	}
}

int SEASON3B::CNewUIItemHotKey::GetHotKey(int iHotKey)
{
	if (iHotKey != -1)
	{
		return m_iHotKeyItemType[iHotKey];
	}

	return -1;
}

int SEASON3B::CNewUIItemHotKey::GetHotKeyLevel(int iHotKey)
{
	if (iHotKey != -1)
	{
		return m_iHotKeyItemLevel[iHotKey];
	}

	return 0;
}

void SEASON3B::CNewUIItemHotKey::RenderItems()
{
	float x, y, width, height;

	for (int i = 0; i < HOTKEY_COUNT; ++i)
	{
		int iIndex = GetHotKeyItemIndex(i);

		if (iIndex != -1)
		{
			ITEM* pItem = g_pMyInventory->FindItem(iIndex);

			if (pItem)
			{
				x = 177 + (i * 32); y = 455.0f; width = 18; height = 18;
				RenderItem3D(x, y, width, height, pItem->Type, pItem->Level, 0, 0);
			}
		}
	}
}

void SEASON3B::CNewUIItemHotKey::RenderItemCount()
{
	float x, y, width, height;

	glColor3f(1.0, 1.0, 1.0);

	for (int i = 0; i < HOTKEY_COUNT; ++i)
	{
		int iCount = GetHotKeyItemIndex(i, true);

		if (iCount > 0)
		{
			x = 195 + (i * 31); y = 446.0f; width = 8.0f; height = 10.0f;

			glColor3f(1.0, 0.94999999, 0.75);

			SEASON3B::RenderOldNumber(x, y, iCount, width, height);
		}
	}
}

void SEASON3B::CNewUIItemHotKey::UseItemRButton()
{
	int x, y, width, height;

	for (int i = 0; i < HOTKEY_COUNT; ++i)
	{
		x = 10 + (i * 38); y = 445; width = 20; height = 20;
		if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			if (MouseRButtonPush)
			{
				MouseRButtonPush = false;
				int iIndex = GetHotKeyItemIndex(i);
				if (iIndex != -1)
				{
					SendRequestUse(iIndex, 0);
					break;
				}
			}
		}
	}
}

SEASON3B::CNewUISkillList::CNewUISkillList()
{
	m_pNewUIMng = NULL;
	Reset();
}

SEASON3B::CNewUISkillList::~CNewUISkillList()
{
	Release();
}

bool SEASON3B::CNewUISkillList::Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_SKILL_LIST, this);

	m_pNewUI3DRenderMng = pNewUI3DRenderMng;

	LoadImages();

	Show(true);

	return true;
}

void SEASON3B::CNewUISkillList::Release()
{
	if (m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->DeleteUI2DEffectObject(UI2DEffectCallback);
	}

	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUISkillList::Reset()
{
	m_bSkillList = false;
	m_bHotKeySkillListUp = false;

	m_bRenderSkillInfo = false;
	m_iRenderSkillInfoType = 0;
	m_iRenderSkillInfoPosX = 0;
	m_iRenderSkillInfoPosY = 0;

	for (int i = 0; i < SKILLHOTKEY_COUNT; ++i)
	{
		m_iHotKeySkillType[i] = -1;
	}

	m_EventState = EVENT_NONE;
}

void SEASON3B::CNewUISkillList::LoadImages()
{
	LoadBitmap("Common\\Interface\\ui_skill_list.jpg", IMAGE_SKILL1);
	LoadBitmap("Common\\Interface\\ui_skill_list2.jpg", IMAGE_SKILL2);
	LoadBitmap("Common\\Interface\\ui_skill_command.jpg", IMAGE_COMMAND);
	LoadBitmap("Common\\Interface\\ui_skill_box.jpg", IMAGE_SKILLBOX);
}

void SEASON3B::CNewUISkillList::UnloadImages()
{
	DeleteBitmap(IMAGE_SKILL1);
	DeleteBitmap(IMAGE_SKILL2);
	DeleteBitmap(IMAGE_COMMAND);
	DeleteBitmap(IMAGE_SKILLBOX);
}

bool SEASON3B::CNewUISkillList::UpdateMouseEvent()
{
	if (g_isCharacterBuff((&Hero->Object), eBuff_DuelWatch))
	{
		m_bSkillList = false;
		return true;
	}

	BYTE bySkillNumber = CharacterAttribute->SkillNumber;
	BYTE bySkillMasterNumber = CharacterAttribute->SkillMasterNumber;

	float x, y, width, height;

	m_bRenderSkillInfo = false;

	if (bySkillNumber <= 0)
	{
		return true;
	}
	
	x = 310.0f; y = 445.0f; width = 25.0f; height = 32.0f;

	if (m_EventState == EVENT_NONE && MouseLButtonPush == false && SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		m_EventState = EVENT_BTN_HOVER_CURRENTSKILL;
		return true;
	}

	if (m_EventState == EVENT_BTN_HOVER_CURRENTSKILL && MouseLButtonPush == false && SEASON3B::CheckMouseIn(x, y, width, height) == false)
	{
		m_EventState = EVENT_NONE;
		return true;
	}

	if (m_EventState == EVENT_BTN_HOVER_CURRENTSKILL && (MouseLButtonPush == true || MouseLButtonDBClick == true) && SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		m_EventState = EVENT_BTN_DOWN_CURRENTSKILL;
		return false;
	}

	if (m_EventState == EVENT_BTN_DOWN_CURRENTSKILL)
	{
		if (MouseLButtonPush == false && MouseLButtonDBClick == false)
		{
			if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
			{
				m_bSkillList = !m_bSkillList;

				PlayBuffer(SOUND_CLICK01);

				m_EventState = EVENT_NONE;
				return false;
			}

			m_EventState = EVENT_NONE;
			return true;
		}
	}

	if (m_EventState == EVENT_BTN_HOVER_CURRENTSKILL)
	{
		m_bRenderSkillInfo = true;
		m_iRenderSkillInfoType = Hero->CurrentSkill;
		m_iRenderSkillInfoPosX = x - 5;
		m_iRenderSkillInfoPosY = y - 12;

		return false;
	}
	else if (m_EventState == EVENT_BTN_DOWN_CURRENTSKILL)
	{
		return false;
	}

	x = 330.0f; y = 480.0f; width = 100.0f; height = 20.0f;

	if (m_EventState == EVENT_NONE && MouseLButtonPush == false && SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		m_EventState = EVENT_BTN_HOVER_SKILLHOTKEY;
		return true;
	}

	if (m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY && MouseLButtonPush == false && SEASON3B::CheckMouseIn(x, y, width, height) == false)
	{
		m_EventState = EVENT_NONE;
		return true;
	}

	if (m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY && MouseLButtonPush == true	&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		m_EventState = EVENT_BTN_DOWN_SKILLHOTKEY;
		return false;
	}

	x = 222.f; y = 431.f; width = 32.f * 5.f; height = 38.f;

	if (m_EventState == EVENT_BTN_DOWN_SKILLHOTKEY)
	{
		if (MouseLButtonPush == false && SEASON3B::CheckMouseIn(x, y, width, height) == false)
		{
			m_EventState = EVENT_NONE;
			return true;
		}

		return false;
	}

	if (m_bSkillList == false)
		return true;

	WORD bySkillType = 0;

	int iSkillCount = 0;
	bool bMouseOnSkillList = false;

	x = 302.0f; y = 375.0f; width = 32; height = 38;

	float fOrigX = 320.f;

	EVENT_STATE PrevEventState = m_EventState;

	for (int i = 0; i < MAX_MAGIC; ++i)
	{
		bySkillType = CharacterAttribute->Skill[i];

		if (bySkillType == 0 || (bySkillType >= AT_SKILL_STUN && bySkillType <= AT_SKILL_REMOVAL_BUFF))
		{
			continue;
		}

		BYTE bySkillUseType = SkillAttribute[bySkillType].SkillUseType;

		if (bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
		{
			continue;
		}

		if (iSkillCount == 14)
		{
			y -= height;
		}

		if (iSkillCount < 14)
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
		else if (iSkillCount >= 14 && iSkillCount < 14)
		{
			x = fOrigX - (8 * width) - ((iSkillCount - 14) * width);
		}
		else
		{
			x = 239 + fOrigX - (12 * width) + ((iSkillCount - 13) * width);
		}

		iSkillCount++;

		if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			bMouseOnSkillList = true;

			if (m_EventState == EVENT_NONE && MouseLButtonPush == false)
			{
				m_EventState = EVENT_BTN_HOVER_SKILLLIST;
				break;
			}
		}

		if (m_EventState == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == true && SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			m_EventState = EVENT_BTN_DOWN_SKILLLIST;
			break;
		}

		if (m_EventState == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == false && SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			m_bRenderSkillInfo = true;
			m_iRenderSkillInfoType = i;
			m_iRenderSkillInfoPosX = x;
			m_iRenderSkillInfoPosY = y;
		}

		if (m_EventState == EVENT_BTN_DOWN_SKILLLIST && MouseLButtonPush == false && m_iRenderSkillInfoType == i && SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			m_EventState = EVENT_NONE;

			m_wHeroPriorSkill = CharacterAttribute->Skill[Hero->CurrentSkill];

			Hero->CurrentSkill = i;
			m_bSkillList = false;

			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}

	if (PrevEventState != m_EventState)
	{
		if (m_EventState == EVENT_NONE || m_EventState == EVENT_BTN_HOVER_SKILLLIST)
			return true;
		return false;
	}

	if (Hero->m_pPet != NULL)
	{
		x = 255.f; y = 337; width = 32; height = 38;

		for (int i = AT_PET_COMMAND_DEFAULT; i < AT_PET_COMMAND_END; ++i)
		{
			if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
			{
				bMouseOnSkillList = true;

				if (m_EventState == EVENT_NONE && MouseLButtonPush == false)
				{
					m_EventState = EVENT_BTN_HOVER_SKILLLIST;
					return true;
				}
				if (m_EventState == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == true)
				{
					m_EventState = EVENT_BTN_DOWN_SKILLLIST;
					return false;
				}

				if (m_EventState == EVENT_BTN_HOVER_SKILLLIST)
				{
					m_bRenderSkillInfo = true;
					m_iRenderSkillInfoType = i;
					m_iRenderSkillInfoPosX = x;
					m_iRenderSkillInfoPosY = y;
				}
				if (m_EventState == EVENT_BTN_DOWN_SKILLLIST && MouseLButtonPush == false
					&& m_iRenderSkillInfoType == i)
				{
					m_EventState = EVENT_NONE;

					m_wHeroPriorSkill = CharacterAttribute->Skill[Hero->CurrentSkill];

					Hero->CurrentSkill = i;
					m_bSkillList = false;
					PlayBuffer(SOUND_CLICK01);
					return false;
				}
			}
			x += width;
		}
	}

	if (bMouseOnSkillList == false && m_EventState == EVENT_BTN_HOVER_SKILLLIST)
	{
		m_EventState = EVENT_NONE;
		return true;
	}
	if (bMouseOnSkillList == false && MouseLButtonPush == false
		&& m_EventState == EVENT_BTN_DOWN_SKILLLIST)
	{
		m_EventState = EVENT_NONE;
		return false;
	}
	if (m_EventState == EVENT_BTN_DOWN_SKILLLIST)
	{
		if (MouseLButtonPush == false)
		{
			m_EventState = EVENT_NONE;
			return true;
		}
		return false;
	}

	return true;
}

bool SEASON3B::CNewUISkillList::UpdateKeyEvent()
{
	for (int i = 0; i < 9; ++i)
	{
		if (SEASON3B::IsPress('1' + i))
		{
			UseHotKey(i + 1);
		}
	}

	if (SEASON3B::IsPress('0'))
	{
		UseHotKey(0);
	}

	if (m_EventState == EVENT_BTN_HOVER_SKILLLIST)
	{
		if (SEASON3B::IsRepeat(VK_CONTROL))
		{
			for (int i = 0; i < 9; ++i)
			{
				if (SEASON3B::IsPress('1' + i))
				{
					SetHotKey(i + 1, m_iRenderSkillInfoType);

					return false;
				}
			}

			if (SEASON3B::IsPress('0'))
			{
				SetHotKey(0, m_iRenderSkillInfoType);

				return false;
			}
		}
	}

	if (SEASON3B::IsRepeat(VK_SHIFT))
	{
		for (int i = 0; i < 4; ++i)
		{
			if (SEASON3B::IsPress('1' + i))
			{
				Hero->CurrentSkill = AT_PET_COMMAND_DEFAULT + i;
				return false;
			}
		}
	}

	return true;
}

bool SEASON3B::CNewUISkillList::IsArrayUp(BYTE bySkill)
{
	for (int i = 0; i < SKILLHOTKEY_COUNT; ++i)
	{
		if (m_iHotKeySkillType[i] == bySkill)
		{
			if (i == 0 || i > 5)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

bool SEASON3B::CNewUISkillList::IsArrayIn(BYTE bySkill)
{
	for (int i = 0; i < SKILLHOTKEY_COUNT; ++i)
	{
		if (m_iHotKeySkillType[i] == bySkill)
		{
			return true;
		}
	}

	return false;
}

void SEASON3B::CNewUISkillList::SetHotKey(int iHotKey, int iSkillType)
{
	for (int i = 0; i < SKILLHOTKEY_COUNT; ++i)
	{
		if (m_iHotKeySkillType[i] == iSkillType)
		{
			m_iHotKeySkillType[i] = -1;
			break;
		}
	}

	m_iHotKeySkillType[iHotKey] = iSkillType;
}

int SEASON3B::CNewUISkillList::GetHotKey(int iHotKey)
{
	return m_iHotKeySkillType[iHotKey];
}

int SEASON3B::CNewUISkillList::GetSkillIndex(int iSkillType)
{
	int iReturn = -1;
	for (int i = 0; i < MAX_MAGIC; ++i)
	{
		if (CharacterAttribute->Skill[i] == iSkillType)
		{
			iReturn = i;
			break;
		}
	}

	return iReturn;
}

void SEASON3B::CNewUISkillList::UseHotKey(int iHotKey)
{
	if (m_iHotKeySkillType[iHotKey] != -1)
	{
		if (m_iHotKeySkillType[iHotKey] >= AT_PET_COMMAND_DEFAULT && m_iHotKeySkillType[iHotKey] < AT_PET_COMMAND_END)
		{
			if (Hero->m_pPet == NULL)
			{
				return;
			}
		}

		WORD wHotKeySkill = CharacterAttribute->Skill[m_iHotKeySkillType[iHotKey]];

		if (wHotKeySkill == 0)
		{
			return;
		}

		m_wHeroPriorSkill = CharacterAttribute->Skill[Hero->CurrentSkill];

		Hero->CurrentSkill = m_iHotKeySkillType[iHotKey];

		WORD bySkill = CharacterAttribute->Skill[Hero->CurrentSkill];


		if (
			g_pOption->IsAutoAttack() == true
			&& gMapManager.WorldActive != WD_6STADIUM
			&& gMapManager.InChaosCastle() == false
			&& (bySkill == AT_SKILL_TELEPORT || bySkill == AT_SKILL_TELEPORT_B))
		{
			SelectedCharacter = -1;
			Attacking = -1;
		}
	}
}

bool SEASON3B::CNewUISkillList::Update()
{
	if (IsArrayIn(Hero->CurrentSkill) == true)
	{
		if (IsArrayUp(Hero->CurrentSkill) == true)
		{
			m_bHotKeySkillListUp = true;
		}
		else
		{
			m_bHotKeySkillListUp = false;
		}
	}

	if (Hero->m_pPet == NULL)
	{
		if (Hero->CurrentSkill >= AT_PET_COMMAND_DEFAULT && Hero->CurrentSkill < AT_PET_COMMAND_END)
		{
			Hero->CurrentSkill = 0;
		}
	}

	return true;
}

void SEASON3B::CNewUISkillList::RenderCurrentSkillAndHotSkillList()
{
	float x, y, width, height;

	BYTE bySkillNumber = CharacterAttribute->SkillNumber;

	if (bySkillNumber > 0)
	{
		int iStartSkillIndex = 1;

		if (m_bHotKeySkillListUp)
		{
			iStartSkillIndex = 6;
		}

		x = 304.5f; y = 443.5f; width = 32; height = 38;

		SEASON3B::RenderImage(IMAGE_SKILLBOX, x, y + 2, width, height);

		x = 310.0f; y = 447.0f; width = 20; height = 28;

		RenderSkillIcon(Hero->CurrentSkill, x + 0.5f, y + 2, width, height);

		if (m_EventState == EVENT_BTN_DOWN_CURRENTSKILL)
		{
			EnableAlphaBlend();

			glColor4f(0.15f, 0.15f, 0.15f, 0.5f);

			RenderColor(x, y, width, height);

			DisableAlphaBlend();

			EnableAlphaTest();

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
}

bool SEASON3B::CNewUISkillList::Render()
{
	float x, y, width, height;

	BYTE bySkillNumber = CharacterAttribute->SkillNumber;

	if (bySkillNumber > 0)
	{
		if (m_bSkillList == true)
		{
			x = 302.0f; y = 375.0f; width = 32; height = 38;

			float fOrigX = 320.f;

			int iSkillType = 0;
			int iSkillCount = 0;

			for (int i = 0; i < MAX_MAGIC; ++i)
			{
				iSkillType = CharacterAttribute->Skill[i];

				if (iSkillType != 0 && (iSkillType < AT_SKILL_STUN || iSkillType > AT_SKILL_REMOVAL_BUFF))
				{
					BYTE bySkillUseType = SkillAttribute[iSkillType].SkillUseType;

					if (bySkillUseType == SKILL_USE_TYPE_MASTER || bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
					{
						continue;
					}

					if (iSkillCount == 14)
					{
						y -= height;
					}

					if (iSkillCount < 14)
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
					else if (iSkillCount >= 14 && iSkillCount < 14)
					{
						x = fOrigX - (8 * width) - ((iSkillCount - 14) * width);
					}
					else
					{
						x = 239 + fOrigX - (12 * width) + ((iSkillCount - 13) * width);
					}

					iSkillCount++;

					if (i == Hero->CurrentSkill)
					{
						glColor3f(1.f, 0.9f, 0.8f);

						SEASON3B::RenderImage(IMAGE_SKILLBOX, x, y + 2, width, height); // -> BOX USE
					}
					else
					{
						glColor3f(1.f, 0.9f, 0.8f);

						SEASON3B::RenderImage(IMAGE_SKILLBOX, x, y + 2, width, height);

						glColor3f(0.6f, 0.7f, 0.8f);
					}

					RenderSkillIcon(i, x + 6, y + 6, 20, 28);

					if (m_EventState == EVENT_BTN_DOWN_SKILLLIST && m_iRenderSkillInfoType == i)
					{
						EnableAlphaBlend();

						glColor4f(0.15f, 0.15f, 0.15f, 0.5f);

						RenderColor(x + 6, y + 6, 20, 28);

						DisableAlphaBlend();

						EnableAlphaTest();

						glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					}
				}
			}

			RenderPetSkill();
		}
	}

	if (m_bRenderSkillInfo == true && m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->RenderUI2DEffect(INVENTORY_CAMERA_Z_ORDER, UI2DEffectCallback, this, 0, 0);

		m_bRenderSkillInfo = false;
	}

	return true;
}

void SEASON3B::CNewUISkillList::RenderSkillInfo()
{
	::RenderSkillInfo(m_iRenderSkillInfoPosX + 15, m_iRenderSkillInfoPosY - 10, m_iRenderSkillInfoType);
}

float SEASON3B::CNewUISkillList::GetLayerDepth()
{
	return 5.2f;
}

WORD SEASON3B::CNewUISkillList::GetHeroPriorSkill()
{
	return m_wHeroPriorSkill;
}

void SEASON3B::CNewUISkillList::SetHeroPriorSkill(BYTE bySkill)
{
	m_wHeroPriorSkill = bySkill;
}

void SEASON3B::CNewUISkillList::RenderPetSkill()
{
	if (Hero->m_pPet == NULL)
	{
		return;
	}

	float x, y, width, height;

	x = 255.0; y = 337; width = 32; height = 38;

	for (int i = AT_PET_COMMAND_DEFAULT; i < AT_PET_COMMAND_END; ++i)
	{
		if (i == Hero->CurrentSkill)
		{
			glColor3f(1.f, 0.9f, 0.8f);

			SEASON3B::RenderImage(IMAGE_SKILLBOX, x, y, width, height); // -> BOX USE
		}
		else
		{
			glColor3f(1.f, 0.9f, 0.8f);

			SEASON3B::RenderImage(IMAGE_SKILLBOX, x, y, width, height);

			glColor3f(0.6f, 0.7f, 0.8f);
		}

		RenderSkillIcon(i, x + 6, y + 6, 20, 28);

		if (m_EventState == EVENT_BTN_DOWN_SKILLLIST && m_iRenderSkillInfoType == i)
		{
			EnableAlphaBlend();

			glColor4f(0.15f, 0.15f, 0.15f, 0.5f);

			RenderColor(x + 6, y + 6, 20, 28);

			DisableAlphaBlend();

			EnableAlphaTest();

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		}

		x += width;
	}
}

void SEASON3B::CNewUISkillList::RenderSkillIcon(int iIndex, float x, float y, float width, float height)
{
	WORD bySkillType = CharacterAttribute->Skill[iIndex];

	if (bySkillType == 0)
	{
		return;
	}

	if (iIndex >= AT_PET_COMMAND_DEFAULT)
	{
		bySkillType = iIndex;
	}

	BYTE bySkillUseType = SkillAttribute[bySkillType].SkillUseType;

	int Skill_Icon = SkillAttribute[bySkillType].Magic_Icon;

	if (!gSkillManager.DemendConditionCheckSkill(bySkillType))
	{
		glColor3f(1.f, 0.5f, 0.5f);
	}

	if (IsCanBCSkill(bySkillType) == false)
	{
		glColor3f(1.f, 0.5f, 0.5f);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_AddSkill) && bySkillUseType == SKILL_USE_TYPE_BRAND)
	{
		glColor3f(1.f, 0.5f, 0.5f);
	}

	if (bySkillType == AT_SKILL_SPEAR && (Hero->Helper.Type<MODEL_HELPER + 2 || Hero->Helper.Type>MODEL_HELPER + 3) && Hero->Helper.Type != MODEL_HELPER + 37)
	{
		glColor3f(1.f, 0.5f, 0.5f);
	}

	if (bySkillType == AT_SKILL_SPEAR && (Hero->Helper.Type == MODEL_HELPER + 2 || Hero->Helper.Type == MODEL_HELPER + 3 || Hero->Helper.Type == MODEL_HELPER + 37))
	{
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;

		if ((iTypeL < ITEM_SPEAR || iTypeL >= ITEM_BOW) && (iTypeR < ITEM_SPEAR || iTypeR >= ITEM_BOW))
		{
			glColor3f(1.f, 0.5f, 0.5f);
		}
	}

	if (bySkillType >= AT_SKILL_BLOCKING && bySkillType <= AT_SKILL_SWORD5 && (Hero->Helper.Type == MODEL_HELPER + 2 || Hero->Helper.Type == MODEL_HELPER + 3 || Hero->Helper.Type == MODEL_HELPER + 37))
	{
		glColor3f(1.f, 0.5f, 0.5f);
	}

	if ((bySkillType == AT_SKILL_ICE_BLADE || (AT_SKILL_POWER_SLASH_UP <= bySkillType && AT_SKILL_POWER_SLASH_UP + 4 >= bySkillType)) && (Hero->Helper.Type == MODEL_HELPER + 2 || Hero->Helper.Type == MODEL_HELPER + 3 || Hero->Helper.Type == MODEL_HELPER + 37))
	{
		glColor3f(1.f, 0.5f, 0.5f);
	}

	int iEnergy = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;

	if (g_csItemOption.IsDisableSkill(bySkillType, iEnergy))
	{
		glColor3f(1.f, 0.5f, 0.5f);
	}

	if (bySkillType == AT_SKILL_PARTY_TELEPORT && PartyNumber <= 0)
	{
		glColor3f(1.f, 0.5f, 0.5f);
	}

	if (bySkillType == AT_SKILL_PARTY_TELEPORT && (IsDoppelGanger1() || IsDoppelGanger2() || IsDoppelGanger3() || IsDoppelGanger4()))
	{
		glColor3f(1.f, 0.5f, 0.5f);
	}

	if (bySkillType == AT_SKILL_DARK_HORSE || (AT_SKILL_ASHAKE_UP <= bySkillType && bySkillType <= AT_SKILL_ASHAKE_UP + 4))
	{
		BYTE byDarkHorseLife = 0;

		byDarkHorseLife = CharacterMachine->Equipment[EQUIPMENT_HELPER].Durability;

		if (byDarkHorseLife == 0 || Hero->Helper.Type != MODEL_HELPER + 4)
		{
			glColor3f(1.f, 0.5f, 0.5f);
		}
	}

	if ((bySkillType == AT_SKILL_INFINITY_ARROW) || (bySkillType == AT_SKILL_SWELL_OF_MAGICPOWER))
	{
		if (g_csItemOption.IsDisableSkill(bySkillType, iEnergy))
		{
			glColor3f(1.f, 0.5f, 0.5f);
		}

		if ((g_isCharacterBuff((&Hero->Object), eBuff_InfinityArrow)) || (g_isCharacterBuff((&Hero->Object), eBuff_SwellOfMagicPower)))
		{
			glColor3f(1.f, 0.5f, 0.5f);
		}
	}

	if (bySkillType == AT_SKILL_REDUCEDEFENSE || (AT_SKILL_BLOOD_ATT_UP <= bySkillType && bySkillType <= AT_SKILL_BLOOD_ATT_UP + 4))
	{
		WORD Strength;

		const WORD wRequireStrength = 596;

		Strength = CharacterAttribute->Strength + CharacterAttribute->AddStrength;

		if (Strength < wRequireStrength)
		{
			glColor3f(1.f, 0.5f, 0.5f);
		}

		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;

		if (!(iTypeR != -1 && (iTypeR < ITEM_STAFF || iTypeR >= ITEM_STAFF + MAX_ITEM_INDEX) && (iTypeL < ITEM_STAFF || iTypeL >= ITEM_STAFF + MAX_ITEM_INDEX)))
		{
			glColor3f(1.f, 0.5f, 0.5f);
		}
	}

	if (bySkillType == AT_SKILL_PARALYZE)
	{
		WORD  Dexterity;

		const WORD wRequireDexterity = 646;

		Dexterity = CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;

		if (Dexterity < wRequireDexterity)
		{
			glColor3f(1.f, 0.5f, 0.5f);
		}
	}

	if (bySkillType == AT_SKILL_WHEEL || (AT_SKILL_TORNADO_SWORDA_UP <= bySkillType && bySkillType <= AT_SKILL_TORNADO_SWORDA_UP + 4) || (AT_SKILL_TORNADO_SWORDB_UP <= bySkillType && bySkillType <= AT_SKILL_TORNADO_SWORDB_UP + 4))
	{
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;

		if (!(iTypeR != -1 && (iTypeR < ITEM_STAFF || iTypeR >= ITEM_STAFF + MAX_ITEM_INDEX) && (iTypeL < ITEM_STAFF || iTypeL >= ITEM_STAFF + MAX_ITEM_INDEX)))
		{
			glColor3f(1.f, 0.5f, 0.5f);
		}
	}

	if (gMapManager.InChaosCastle() == true)
	{
		if (bySkillType == AT_SKILL_DARK_HORSE || bySkillType == AT_SKILL_RIDER || (bySkillType >= AT_PET_COMMAND_DEFAULT && bySkillType <= AT_PET_COMMAND_TARGET) || (AT_SKILL_ASHAKE_UP <= bySkillType && bySkillType <= AT_SKILL_ASHAKE_UP + 4))
		{
			glColor3f(1.f, 0.5f, 0.5f);
		}
	}
	else
	{
		if (bySkillType == AT_SKILL_DARK_HORSE || (AT_SKILL_ASHAKE_UP <= bySkillType && bySkillType <= AT_SKILL_ASHAKE_UP + 4))
		{
			BYTE byDarkHorseLife = 0;

			byDarkHorseLife = CharacterMachine->Equipment[EQUIPMENT_HELPER].Durability;

			if (byDarkHorseLife == 0)
			{
				glColor3f(1.f, 0.5f, 0.5f);
			}
		}
	}

	int iCharisma = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;

	if (g_csItemOption.IsDisableSkill(bySkillType, iEnergy, iCharisma))
	{
		glColor3f(1.f, 0.5f, 0.5f);
	}

	float fU, fV;
	int iKindofSkill = 0;

	if (g_csItemOption.Special_Option_Check() == false && (bySkillType == AT_SKILL_ICE_BLADE || (AT_SKILL_POWER_SLASH_UP <= bySkillType && AT_SKILL_POWER_SLASH_UP + 4 >= bySkillType)))
	{
		glColor3f(1.f, 0.5f, 0.5f);
	}

	if (g_csItemOption.Special_Option_Check(1) == false && (bySkillType == AT_SKILL_CROSSBOW || (AT_SKILL_MANY_ARROW_UP <= bySkillType && AT_SKILL_MANY_ARROW_UP + 4 >= bySkillType)))
	{
		glColor3f(1.f, 0.5f, 0.5f);
	}

	if (bySkillType >= AT_PET_COMMAND_DEFAULT && bySkillType <= AT_PET_COMMAND_END)
	{
		fU = ((bySkillType - AT_PET_COMMAND_DEFAULT) % 8) * width / 256.f;
		fV = ((bySkillType - AT_PET_COMMAND_DEFAULT) / 8) * height / 256.f;
		RenderBitmap(IMAGE_COMMAND, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType == AT_SKILL_PLASMA_STORM_FENRIR)
	{
		fU = 4 * width / 256.f;
		fV = 0.f;
		RenderBitmap(IMAGE_COMMAND, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType >= AT_SKILL_ALICE_DRAINLIFE && bySkillType <= AT_SKILL_ALICE_THORNS)
	{
		fU = ((bySkillType - AT_SKILL_ALICE_DRAINLIFE) % 8) * width / 256.f;
		fV = 3 * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType >= AT_SKILL_ALICE_SLEEP && bySkillType <= AT_SKILL_ALICE_BLIND)
	{
		fU = ((bySkillType - AT_SKILL_ALICE_SLEEP + 4) % 8) * width / 256.f;
		fV = 3 * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType == AT_SKILL_ALICE_BERSERKER)
	{
		fU = 10 * width / 256.f;
		fV = 3 * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType >= AT_SKILL_ALICE_WEAKNESS && bySkillType <= AT_SKILL_ALICE_ENERVATION)
	{
		fU = (bySkillType - AT_SKILL_ALICE_WEAKNESS + 8) * width / 256.f;
		fV = 3 * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType >= AT_SKILL_SUMMON_EXPLOSION && bySkillType <= AT_SKILL_SUMMON_REQUIEM)
	{
		fU = ((bySkillType - AT_SKILL_SUMMON_EXPLOSION + 6) % 8) * width / 256.f;
		fV = 3 * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType == AT_SKILL_SUMMON_POLLUTION)
	{
		fU = 11 * width / 256.f;
		fV = 3 * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType == AT_SKILL_BLOW_OF_DESTRUCTION)
	{
		fU = 7 * width / 256.f;
		fV = 2 * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType == AT_SKILL_GAOTIC)
	{
		fU = 3 * width / 256.f;
		fV = 8 * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType == AT_SKILL_RECOVER)
	{
		fU = 9 * width / 256.f;
		fV = 2 * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType == AT_SKILL_MULTI_SHOT)
	{
		if (gCharacterManager.GetEquipedBowType_Skill() == BOWTYPE_NONE)
			glColor3f(1.f, 0.5f, 0.5f);

		fU = 0 * width / 256.f;
		fV = 8 * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType == AT_SKILL_FLAME_STRIKE)
	{
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;

		if (!(iTypeR != -1 &&
			(iTypeR < ITEM_STAFF || iTypeR >= ITEM_STAFF + MAX_ITEM_INDEX) &&
			(iTypeL < ITEM_STAFF || iTypeL >= ITEM_STAFF + MAX_ITEM_INDEX)))
			glColor3f(1.f, 0.5f, 0.5f);

		fU = 1 * width / 256.f;
		fV = 8 * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType == AT_SKILL_GIGANTIC_STORM)
	{
		fU = 2 * width / 256.f;
		fV = 8 * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType == AT_SKILL_LIGHTNING_SHOCK)
	{
		fU = 2 * width / 256.f;
		fV = 3 * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (AT_SKILL_LIGHTNING_SHOCK_UP <= bySkillType && bySkillType <= AT_SKILL_LIGHTNING_SHOCK_UP + 4)
	{
		fU = 6 * width / 256.f;
		fV = 8 * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType == AT_SKILL_SWELL_OF_MAGICPOWER)
	{
		fU = 8 * width / 256.f;
		fV = 2 * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillUseType == 4)
	{
		fU = (width / 256.f) * (Skill_Icon % 12);
		fV = (height / 256.f) * ((Skill_Icon / 12) + 4);
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else if (bySkillType >= 57)
	{
		fU = ((bySkillType - 57) % 8) * width / 256.f;
		fV = ((bySkillType - 57) / 8) * height / 256.f;
		RenderBitmap(IMAGE_SKILL2, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}
	else
	{
		fU = ((bySkillType - 1) % 8) * width / 256.f;
		fV = ((bySkillType - 1) / 8) * height / 256.f;
		RenderBitmap(IMAGE_SKILL1, x, y, width, height, fU, fV, width / 256.f, height / 256.f);
	}

	int iHotKey = -1;
	for (int i = 0; i < SKILLHOTKEY_COUNT; ++i)
	{
		if (m_iHotKeySkillType[i] == iIndex)
		{
			iHotKey = i;
			break;
		}
	}

	if (iHotKey != -1)
	{
		glColor3f(1.f, 0.9f, 0.8f);
		SEASON3B::RenderNumber(x + 20, y + 20, iHotKey);
	}

	if ((bySkillType != AT_SKILL_INFINITY_ARROW) && (bySkillType != AT_SKILL_SWELL_OF_MAGICPOWER))
	{
		RenderSkillDelay(iIndex, x, y, width, height);
	}

	glColor3f(1.f, 1.f, 1.f);
}

void SEASON3B::CNewUISkillList::RenderSkillDelay(int iIndex, float x, float y, float width, float height)
{
	int iSkillDelay = CharacterAttribute->SkillDelay[iIndex];
	if (iSkillDelay > 0)
	{
		int iSkillType = CharacterAttribute->Skill[iIndex];

		if (iSkillType == AT_SKILL_PLASMA_STORM_FENRIR)
		{
			if (!CheckAttack())
			{
				return;
			}
		}

		int iSkillMaxDelay = SkillAttribute[iSkillType].Delay;

		float fPersent = (float)(iSkillDelay / (float)iSkillMaxDelay);

		EnableAlphaTest();
		glColor4f(1.f, 0.5f, 0.5f, 0.5f);
		float fdeltaH = height * fPersent;
		RenderColor(x, y + height - fdeltaH, width, fdeltaH);
		EndRenderColor();
	}
}

bool SEASON3B::CNewUISkillList::IsSkillListUp()
{
	return m_bHotKeySkillListUp;
}

void SEASON3B::CNewUISkillList::ResetMouseLButton()
{
	MouseLButton = false;
	MouseLButtonPop = false;
	MouseLButtonPush = false;
}

void SEASON3B::CNewUISkillList::UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB)
{
	if (pClass)
	{
		CNewUISkillList* pSkillList = (CNewUISkillList*)(pClass);
		pSkillList->RenderSkillInfo();
	}
}

void SEASON3B::CNewUIMainFrameWindow::SetPreExp_Wide(__int64 dwPreExp)
{
	m_loPreExp = dwPreExp;
}

void SEASON3B::CNewUIMainFrameWindow::SetGetExp_Wide(__int64 dwGetExp)
{
	m_loGetExp = dwGetExp;

	if (m_loGetExp > 0)
	{
		m_bExpEffect = true;
		m_dwExpEffectTime = timeGetTime();
	}
}

void SEASON3B::CNewUIMainFrameWindow::SetPreExp(DWORD dwPreExp)
{
	m_dwPreExp = dwPreExp;
}

void SEASON3B::CNewUIMainFrameWindow::SetGetExp(DWORD dwGetExp)
{
	m_dwGetExp = dwGetExp;

	if (m_dwGetExp > 0)
	{
		m_bExpEffect = true;
		m_dwExpEffectTime = timeGetTime();
	}
}