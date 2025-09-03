// NewUIPetInformation.cpp: imp0lementation of the CNewUIPetInfoWindow class.
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NewUIPetInfoWindow.h"
#include "NewUISystem.h"
#include "wsclientinline.h"
#include "GIPetManager.h"
#include "CharacterManager.h"
#include "SkillManager.h"

using namespace SEASON3B;

CNewUIPetInfoWindow::CNewUIPetInfoWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
	m_iNumCurOpenTab = TAB_TYPE_DARKHORSE;
	CalcDamage(m_iNumCurOpenTab);
	m_EventState = EVENT_NONE;

}

CNewUIPetInfoWindow::~CNewUIPetInfoWindow()
{
	Release();
}

bool CNewUIPetInfoWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_PET, this);

	SetPos(x, y);

	LoadImages();

	Show(false);

	return true;
}

void CNewUIPetInfoWindow::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void CNewUIPetInfoWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool CNewUIPetInfoWindow::UpdateMouseEvent()
{
	if (BtnProcess() == true)
	{
		return false;
	}

	/* */

	if (SEASON3B::CheckMouseIn(m_Pos.x + 20, m_Pos.y + 48, 49, 16) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		CalcDamage(TAB_TYPE_DARKHORSE);

		this->m_iNumCurOpenTab = TAB_TYPE_DARKHORSE;

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	/* */

	if (SEASON3B::CheckMouseIn(m_Pos.x + 75, m_Pos.y + 48, 49, 16) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		CalcDamage(TAB_TYPE_DARKSPIRIT);

		this->m_iNumCurOpenTab = TAB_TYPE_DARKSPIRIT;

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	/* */

	if (CheckMouseIn(m_Pos.x, m_Pos.y, PETINFOWINDOW_WIDTH, PETINFOWINDOW_HEIGHT))
	{
		return false;
	}

	return true;
}

bool CNewUIPetInfoWindow::UpdateKeyEvent()
{
	return true;
}

bool CNewUIPetInfoWindow::Update()
{
	if (IsVisible() == false)
	{
		return true;
	}

	return true;
}

bool CNewUIPetInfoWindow::Render()
{
	EnableAlphaTest();

	/* */

	glColor4f(1.f, 1.f, 1.f, 1.f);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(0xFFFFFFFF);

	/* */

	RenderBitmap(IMAGE_PETINFO_TOP, m_Pos.x, m_Pos.y, 190.0, 256.0, 0, 0, 0.7421875, 1.0, 1, 1, 0.0);
	RenderBitmap(IMAGE_PETINFO_LOW, m_Pos.x, m_Pos.y + 256.0, 190.0, 177.0, 0, 0, 0.7421875, 0.69140625, 1, 1, 0.0);
	
	/* */

	if ((SEASON3B::CheckMouseIn(m_Pos.x + 20, m_Pos.y + 48, 49, 16) == true) || (this->m_iNumCurOpenTab == TAB_TYPE_DARKHORSE))
	{
		glColor3f(1.0, 1.0, 1.0);
	}
	else
	{
		glColor3f(0.69999999, 0.69999999, 0.69999999);
	}

	RenderBitmap(BITMAP_INTERFACE_EX + 9, m_Pos.x + 20, m_Pos.y + 48, 49.0f, 16.f, 0, 0, 0.765625, 1.0, 1, 1, 0.0);

	glColor3f(1.0, 1.0, 1.0);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(0xFFFFFFFF);
	g_pRenderText->SetBgColor(0);

	g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 51, GlobalText[1187], 120, 0, RT3_SORT_LEFT);

	/* */

	if ((SEASON3B::CheckMouseIn(m_Pos.x + 75, m_Pos.y + 48, 49, 16) == true) || (this->m_iNumCurOpenTab == TAB_TYPE_DARKSPIRIT))
	{
		glColor3f(1.0, 1.0, 1.0);
	}
	else
	{
		glColor3f(0.69999999, 0.69999999, 0.69999999);
	}

	RenderBitmap(BITMAP_INTERFACE_EX + 9, m_Pos.x + 75, m_Pos.y + 48, 49.0f, 16.f, 0, 0, 0.765625, 1.0, 1, 1, 0.0);

	glColor3f(1.0, 1.0, 1.0);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(0xFFFFFFFF);
	g_pRenderText->SetBgColor(0);

	g_pRenderText->RenderText(m_Pos.x + 83, m_Pos.y + 51, GlobalText[1214], 120, 0, RT3_SORT_LEFT);

	/* */

	RenderBitmap(IMAGE_PETINFO_LINE, m_Pos.x, m_Pos.y + 70, 190.0, 10.0, 0, 0, 0.7421875, 0.625, 1, 1, 0.0);
	RenderBitmap(IMAGE_PETINFO_LINE, m_Pos.x, m_Pos.y + 383, 190.0, 10.0, 0, 0, 0.7421875, 0.625, 1, 1, 0.0);

	/* */

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetBgColor(0);

	g_pRenderText->RenderText(m_Pos.x + 36, m_Pos.y + 12, GlobalText[1217], 120, 0, RT3_SORT_CENTER);

	/* */

	PET_INFO* pPetInfo = NULL;

	switch (m_iNumCurOpenTab)
	{
	case TAB_TYPE_DARKHORSE:
	{
		pPetInfo = Hero->GetEquipedPetInfo(PET_TYPE_DARK_HORSE);

		if (pPetInfo->m_dwPetType == PET_TYPE_NONE)
		{
			g_pRenderText->SetTextColor(0xFF0000FF);

			g_pRenderText->SetFont(g_hFontBold);

			unicode::t_char szText[256] = { 0, };

			unicode::_sprintf(szText, GlobalText[1233], GlobalText[1187]);

			g_pRenderText->RenderText(m_Pos.x + 15, m_Pos.y + 98, szText, 160, 30, RT3_SORT_CENTER);
		}
		else
		{
			RenderDarkHorseInfo(pPetInfo);
		}
	}
	break;
	case TAB_TYPE_DARKSPIRIT:
	{
		pPetInfo = Hero->GetEquipedPetInfo(PET_TYPE_DARK_SPIRIT);

		if (pPetInfo->m_dwPetType == PET_TYPE_NONE)
		{
			g_pRenderText->SetTextColor(0xFF0000FF);

			g_pRenderText->SetFont(g_hFontBold);

			g_pRenderText->RenderText(m_Pos.x + 15, m_Pos.y + 98, GlobalText[1169], 160, 30, RT3_SORT_CENTER);
		}
		else
		{
			RenderDarkSpiritInfo(pPetInfo);
		}
	}
	break;
	}

	/* */

	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24, 24) == true)
	{
		RenderTipText(m_Pos.x + 20, m_Pos.y + 380, GlobalText[1002]);
	}

	RenderBitmap(IMAGE_PETINFO_BTN_EXIT, m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);

	/* */

	DisableAlphaBlend();

	return true;
}

bool CNewUIPetInfoWindow::RenderDarkHorseInfo(PET_INFO* pPetInfo)
{
	unicode::t_char szText[256] = { 0, };

	g_pRenderText->SetFont(g_hFontBold);

	g_pRenderText->SetTextColor(255, 255, 255, 255);

	g_pRenderText->RenderText(m_Pos.x + 60, m_Pos.y + 22, GlobalText[1187], 70, 0, RT3_SORT_CENTER);

	int iPosX = m_Pos.x + 15;
	int iPosY = m_Pos.y + 85;

	g_pRenderText->SetTextColor(223, 191, 103, 255);

	RenderBitmap(IMAGE_PETINFO_LEVEL_BOX, iPosX - 2, iPosY + 2, 75.0, 21.0, 0, 0, 0.5859375, 0.65625, 1, 1, 0.0);

	unicode::_sprintf(szText, GlobalText[200], pPetInfo->m_wLevel);
	g_pRenderText->RenderText(iPosX + 2, iPosY + 8, szText, 70 - 14, 0, RT3_SORT_CENTER);

	g_pRenderText->SetBgColor(32, 32, 32, 180);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	unicode::_sprintf(szText, GlobalText[358], pPetInfo->m_wLife, 255);

	g_pRenderText->RenderText(iPosX + 10, iPosY + 28, szText, 145, 0, RT3_SORT_LEFT);

	EnableAlphaTest();

	glColor4f(0.f, 0.f, 0.f, 0.5f);

	RenderColor(float(iPosX + 9), float(iPosY + 42), float(146), float(10));

	glColor3f(250.f / 255.f, 10 / 255.f, 0.f);

	int iHP = (min(pPetInfo->m_wLife, 255) * 142) / 255;

	RenderColor(float(iPosX + 11), float(iPosY + 44), float(iHP), float(6));

	EndRenderColor();

	unicode::_sprintf(szText, GlobalText[357], pPetInfo->m_dwExp1, pPetInfo->m_dwExp2);
	g_pRenderText->RenderText(iPosX + 10, iPosY + 59, szText, 145, 0, RT3_SORT_LEFT);

	unicode::_sprintf(szText, GlobalText[203], m_aiDamage[0], m_aiDamage[1], pPetInfo->m_wAttackSuccess);
	g_pRenderText->RenderText(iPosX + 10, iPosY + 72, szText, 145, 0, RT3_SORT_LEFT);

	unicode::_sprintf(szText, GlobalText[64], pPetInfo->m_wAttackSpeed);
	g_pRenderText->RenderText(iPosX + 10, iPosY + 85, szText, 145, 0, RT3_SORT_LEFT);
	return true;
}

bool CNewUIPetInfoWindow::RenderDarkSpiritInfo(PET_INFO* pPetInfo)
{
	unicode::t_char szText[256] = { 0, };

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(m_Pos.x + 60, m_Pos.y + 22, GlobalText[1214], 70, 0, RT3_SORT_CENTER);

	int iGBox1PosX = m_Pos.x + 15;
	int iGBox1PosY = m_Pos.y + 85;
	int iGBox2PosX = m_Pos.x + 12;

	int iGBox2PosY = m_Pos.y + 196;

	RenderBitmap(IMAGE_PETINFO_LEVEL_BOX, iGBox1PosX - 2, iGBox1PosY + 2, 75.0, 21.0, 0, 0, 0.5859375, 0.65625, 1, 1, 0.0);
	RenderBitmap(IMAGE_PETINFO_LEVEL_BOX, iGBox1PosX - 2, iGBox2PosY + 2, 75.0, 21.0, 0, 0, 0.5859375, 0.65625, 1, 1, 0.0);

	g_pRenderText->SetTextColor(223, 191, 103, 255);

	unicode::_sprintf(szText, GlobalText[200], pPetInfo->m_wLevel);
	g_pRenderText->RenderText(iGBox1PosX + 2, iGBox1PosY + 8, szText, 70 - 14, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(iGBox2PosX + 2, iGBox2PosY + 8, GlobalText[1218], 70 - 14, 0, RT3_SORT_CENTER);

	g_pRenderText->SetBgColor(32, 32, 32, 180);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	unicode::_sprintf(szText, GlobalText[358], pPetInfo->m_wLife, 255);

	g_pRenderText->RenderText(iGBox1PosX + 10, iGBox1PosY + 28, szText, 145, 0, RT3_SORT_LEFT);

	EnableAlphaTest();

	glColor4f(0.f, 0.f, 0.f, 0.5f);

	RenderColor(float(iGBox1PosX + 9), float(iGBox1PosY + 42), float(146), float(10));

	glColor3f(250.f / 255.f, 10 / 255.f, 0.f);

	int iHP = (min(pPetInfo->m_wLife, 255) * 142) / 255;

	RenderColor(float(iGBox1PosX + 11), float(iGBox1PosY + 44), float(iHP), float(6));

	EndRenderColor();

	unicode::_sprintf(szText, GlobalText[357], pPetInfo->m_dwExp1, pPetInfo->m_dwExp2);
	g_pRenderText->RenderText(iGBox1PosX + 10, iGBox1PosY + 59, szText, 145, 0, RT3_SORT_LEFT);

	unicode::_sprintf(szText, GlobalText[203], m_aiDamage[0], m_aiDamage[1], pPetInfo->m_wAttackSuccess);
	g_pRenderText->RenderText(iGBox1PosX + 10, iGBox1PosY + 72, szText, 145, 0, RT3_SORT_LEFT);

	unicode::_sprintf(szText, GlobalText[64], pPetInfo->m_wAttackSpeed);
	g_pRenderText->RenderText(iGBox1PosX + 10, iGBox1PosY + 85, szText, 145, 0, RT3_SORT_LEFT);

	unicode::_sprintf(szText, GlobalText[698], (185 + (pPetInfo->m_wLevel * 15)));
	g_pRenderText->RenderText(iGBox1PosX + 10, iGBox1PosY + 98, szText, 145, 0, RT3_SORT_LEFT);

	// SkillBox
	RenderImage(IMAGE_PETINFO_SKILLBOX, iGBox2PosX + 10, iGBox2PosY + 28, 32.f, 38.f);
	RenderImage(IMAGE_PETINFO_SKILLBOX, iGBox2PosX + 10, iGBox2PosY + 68, 32.f, 38.f);
	RenderImage(IMAGE_PETINFO_SKILLBOX, iGBox2PosX + 10, iGBox2PosY + 108, 32.f, 38.f);
	RenderImage(IMAGE_PETINFO_SKILLBOX, iGBox2PosX + 10, iGBox2PosY + 148, 32.f, 38.f);
	// Skill	(x = SKillbox.x + 7, y = SkillBox.y + 6)
	RenderImage(IMAGE_PETINFO_SKILL, iGBox2PosX + 17, iGBox2PosY + 34, 19.f, 27.f, 1.f, 0.f);
	RenderImage(IMAGE_PETINFO_SKILL, iGBox2PosX + 17, iGBox2PosY + 74, 19.f, 27.f, 21.f, 0.f);
	RenderImage(IMAGE_PETINFO_SKILL, iGBox2PosX + 17, iGBox2PosY + 114, 19.f, 27.f, 41.f, 0.f);
	RenderImage(IMAGE_PETINFO_SKILL, iGBox2PosX + 17, iGBox2PosY + 154, 19.f, 27.f, 61.f, 0.f);

	g_pRenderText->RenderText(iGBox2PosX + 52, iGBox2PosY + 45, GlobalText[1219], 0, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(iGBox2PosX + 52, iGBox2PosY + 85, GlobalText[1220], 0, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(iGBox2PosX + 52, iGBox2PosY + 125, GlobalText[1221], 0, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(iGBox2PosX + 52, iGBox2PosY + 165, GlobalText[1222], 0, 0, RT3_SORT_CENTER);
	return true;
}

void CNewUIPetInfoWindow::RenderGroupBox(int iPosX, int iPosY, int iWidth, int iHeight, int iTitleWidth/* =60 */, int iTitleHeight/* =20  */)
{
	EnableAlphaTest();

	glColor4f(0.f, 0.f, 0.f, 0.9f);
	RenderColor(float(iPosX + 3), float(iPosY + 2), float(iTitleWidth - 8), float(iTitleHeight));
	glColor4f(0.f, 0.f, 0.f, 0.6f);
	RenderColor(float(iPosX + 3), float(iPosY + 2 + iTitleHeight), float(iWidth - 7), float(iHeight - iTitleHeight - 7));

	EndRenderColor();
}

float CNewUIPetInfoWindow::GetLayerDepth()
{
	return 2.3f;
}


bool CNewUIPetInfoWindow::BtnProcess()
{
	POINT ptExitBtn1 = { m_Pos.x + 169, m_Pos.y + 7 };

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 13, 12))
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_PET);
		return true;
	}

	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24, 24) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Hide(SEASON3B::INTERFACE_PET);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	return false;
}

void CNewUIPetInfoWindow::CalcDamage(int iNumTapButton)
{
	PET_INFO* pPetInfo = NULL;

	switch (iNumTapButton)
	{
	case TAB_TYPE_DARKHORSE:
	{
		int iSkillDamage[2];
		gCharacterManager.GetSkillDamage(AT_SKILL_DARK_HORSE, &iSkillDamage[0], &iSkillDamage[1]);

		pPetInfo = Hero->GetEquipedPetInfo(PET_TYPE_DARK_HORSE);
		m_aiDamage[0] = pPetInfo->m_wDamageMin + iSkillDamage[0];
		m_aiDamage[1] = pPetInfo->m_wDamageMax + iSkillDamage[1];
	}
	break;
	case TAB_TYPE_DARKSPIRIT:
	{
		m_fAddDamagePercent = 0.f;
		m_aiDamage[0] = 0;
		m_aiDamage[1] = 0;

		if (CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type != -1)
		{
			m_fAddDamagePercent = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].MagicPower;
		}

		pPetInfo = Hero->GetEquipedPetInfo(PET_TYPE_DARK_SPIRIT);

		m_fAddDamagePercent /= 100.f;
		m_aiDamage[0] = pPetInfo->m_wDamageMin + (int)(pPetInfo->m_wDamageMin * m_fAddDamagePercent);
		m_aiDamage[1] = pPetInfo->m_wDamageMax + (int)(pPetInfo->m_wDamageMax * m_fAddDamagePercent);
	}
	break;
	}
}

void CNewUIPetInfoWindow::LoadImages()
{
	LoadBitmap("Common\\Interface\\ui_item_backtop.jpg", IMAGE_PETINFO_TOP);
	LoadBitmap("Common\\Interface\\ui_item_backlow.jpg", IMAGE_PETINFO_LOW);

	LoadBitmap("Common\\Interface\\ui_menu_exit.jpg", IMAGE_PETINFO_BTN_EXIT);

	LoadBitmap("Interface\\newui_guild_tab04.tga", IMAGE_PETINFO_TAB_BUTTON, GL_LINEAR);

	// Skill
	LoadBitmap("Common\\Interface\\ui_skill_command.jpg", IMAGE_PETINFO_SKILL);
	LoadBitmap("Common\\Interface\\ui_skill_box.jpg", IMAGE_PETINFO_SKILLBOX);
}

void CNewUIPetInfoWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_PETINFO_TOP);
	DeleteBitmap(IMAGE_PETINFO_LOW);

	DeleteBitmap(IMAGE_PETINFO_BTN_EXIT);
	DeleteBitmap(IMAGE_PETINFO_TAB_BUTTON);

	DeleteBitmap(IMAGE_PETINFO_SKILL);
	DeleteBitmap(IMAGE_PETINFO_SKILLBOX);
}

void CNewUIPetInfoWindow::OpenningProcess()
{
	CalcDamage(m_iNumCurOpenTab);
}

void CNewUIPetInfoWindow::ClosingProcess()
{

}
