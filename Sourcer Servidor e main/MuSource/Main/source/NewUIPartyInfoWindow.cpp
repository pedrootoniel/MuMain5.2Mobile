// NewUIPartyInfo.cpp: implementation of the CNewUIPartyInfo class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NewUIPartyInfoWindow.h"
#include "NewUISystem.h"
#include "wsclientinline.h"
#include "GMBattleCastle.h"
#include "CSChaosCastle.h"
#include "GMHellas.h"
#include "GM3rdChangeUp.h"
#include "w_CursedTemple.h"
#include "MapManager.h"


using namespace SEASON3B;

CNewUIPartyInfoWindow::CNewUIPartyInfoWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
	m_bParty = false;
}

CNewUIPartyInfoWindow::~CNewUIPartyInfoWindow()
{
	Release();
}

bool CNewUIPartyInfoWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_PARTY, this);

	SetPos(x, y);

	LoadImages();

	InitButtons();

	Show(false);

	return true;
}

void CNewUIPartyInfoWindow::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void CNewUIPartyInfoWindow::InitButtons()
{
	
}

void CNewUIPartyInfoWindow::OpenningProcess()
{
	SendRequestPartyList();
}

void CNewUIPartyInfoWindow::ClosingProcess()
{

}

bool CNewUIPartyInfoWindow::BtnProcess()
{
	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24, 24) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Hide(SEASON3B::INTERFACE_PARTY);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	if (IsVisible())
	{
		for (int i = 0; i < PartyNumber; i++)
		{
			int iVal = i * 31;

			if (!strcmp(Party[0].Name, Hero->ID) || !strcmp(Party[i].Name, Hero->ID))
			{
				if (SEASON3B::CheckMouseIn(m_Pos.x + 143, m_Pos.y + 60 + iVal, 24, 24) == true && MouseLButtonPush)
				{
					MouseLButtonPush = false;

					MouseUpdateTime = 0;
					MouseUpdateTimeMax = 6;

					LeaveParty(i);

					PlayBuffer(SOUND_CLICK01);

					return true;
				}
			}
		}
	}


	return false;
}

bool CNewUIPartyInfoWindow::UpdateMouseEvent()
{
	if (true == BtnProcess())
		return false;

	if (CheckMouseIn(m_Pos.x, m_Pos.y, PARTY_INFO_WINDOW_WIDTH, PARTY_INFO_WINDOW_HEIGHT))
		return false;

	return true;
}

bool CNewUIPartyInfoWindow::UpdateKeyEvent()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_PARTY) == true)
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_PARTY);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}

	return true;
}

bool CNewUIPartyInfoWindow::Update()
{
	if (IsVisible())
	{
		if (PartyNumber > 0)
			SetParty(true);
		else
			SetParty(false);
	}


	return true;
}

bool CNewUIPartyInfoWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(0xFFFFFFFF);

	DWORD dwPreBGColor = g_pRenderText->GetBgColor();

	g_pRenderText->SetBgColor(RGBA(0, 0, 0, 0));

	RenderBitmap(IMAGE_PARTY_BASE_WINDOW_TOP, m_Pos.x, m_Pos.y, 190.0, 256.0, 0, 0, 0.7421875, 1.0, 1, 1, 0.0);
	RenderBitmap(IMAGE_PARTY_BASE_WINDOW_LOW, m_Pos.x, m_Pos.y + 256.0, 190.0, 177.0, 0, 0, 0.7421875, 0.69140625, 1, 1, 0.0);

	/* */

	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0) == true)
	{
		RenderTipText(m_Pos.x + 25, m_Pos.y + 380, GlobalText[221]);
	}

	RenderBitmap(IMAGE_PARTY_BASE_WINDOW_BTN_EXIT, m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);

	/* */

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->RenderText(m_Pos.x + 60, m_Pos.y + 12, GlobalText[190], 72, 0, RT3_SORT_CENTER);

	g_pRenderText->SetFont(g_hFont);

	if (m_bParty)
	{
		for (int i = 0; i < PartyNumber; i++)
		{
			PARTY_t* pMember = &Party[i];

			bool bExitBtnRender = false;

			if (!strcmp(Party[0].Name, Hero->ID) || !strcmp(Party[i].Name, Hero->ID))
			{
				bExitBtnRender = true;
			}

			RenderMemberStatue(i, pMember);

			if (bExitBtnRender)
			{
				int iVal = i * 31;

				RenderBitmap(IMAGE_PARTY_BASE_WINDOW_BTN_EXIT, m_Pos.x + 143, m_Pos.y + 60 + iVal, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
			}		
		}
	}
	else
	{
		int iStartHeight = 60;
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iStartHeight, GlobalText[191], 0, 0, RT3_SORT_CENTER);
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iStartHeight + 15, GlobalText[192], 0, 0, RT3_SORT_CENTER);
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iStartHeight + 30, GlobalText[193], 0, 0, RT3_SORT_CENTER);
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iStartHeight + 50, GlobalText[194], 0, 0, RT3_SORT_CENTER);
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iStartHeight + 65, GlobalText[195], 0, 0, RT3_SORT_CENTER);
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iStartHeight + 80, GlobalText[196], 0, 0, RT3_SORT_CENTER);
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iStartHeight + 95, GlobalText[197], 0, 0, RT3_SORT_CENTER);
	}

	g_pRenderText->SetBgColor(dwPreBGColor);

	DisableAlphaBlend();

	return true;
}

void CNewUIPartyInfoWindow::RenderMemberStatue(int iIndex, PARTY_t* pMember)
{
	unicode::t_char szText[256] = { 0, };

	int iVal = iIndex * 31;
	int iPosX = m_Pos.x + 10;
	int iPosY = m_Pos.y + 30 + iVal;

	g_pRenderText->SetFont(g_hFontBold);

	if (iIndex == 0)
	{
		g_pRenderText->SetFont(g_hFontBold);

		g_pRenderText->SetTextColor(255, 255, 255, 255);

		g_pRenderText->SetBgColor(40, 42, 143, 255);

		g_pRenderText->RenderText(iPosX + 10, iPosY + 23, pMember->Name, 15, 0, RT3_SORT_LEFT);
	}
	else
	{
		g_pRenderText->SetFont(g_hFontBold);

		g_pRenderText->SetBgColor(0, 0, 0, 0);

		g_pRenderText->SetTextColor(255, 255, 255, 255);

		g_pRenderText->RenderText(iPosX + 10, iPosY + 23, pMember->Name, 15, 0, RT3_SORT_LEFT);
	}

	g_pRenderText->SetFont(g_hFont);

	g_pRenderText->SetBgColor(0, 0, 0, 0);

	g_pRenderText->SetTextColor(RGBA(255, 255, 255, 255));

	g_pRenderText->RenderText(iPosX + 10, iPosY + 30, gMapManager.GetMapName(pMember->Map), 70, 0, RT3_SORT_LEFT);

	unicode::_sprintf(szText, "%d,%d", pMember->x, pMember->y);
	g_pRenderText->RenderText(iPosX + 96, iPosY + 30, szText, 60, 0, RT3_SORT_LEFT);

	EnableAlphaTest();

	glColor4f(0.f, 0.f, 0.f, 0.5f);

	RenderColor(float(iPosX + 8), float(iPosY + 40), float(111), float(6));

	glColor3f(250.f / 255.f, 10 / 255.f, 0.f);

	int iHP = (pMember->currHP * 107) / pMember->maxHP;

	RenderColor(float(iPosX + 10), float(iPosY + 42), float(iHP), float(3));

	EndRenderColor();

	unicode::_sprintf(szText, "( %d / %d )", pMember->currHP, pMember->maxHP);
	g_pRenderText->RenderText(iPosX + 48, iPosY + 46, szText, 70, 0, RT3_SORT_RIGHT);
}

bool CNewUIPartyInfoWindow::LeaveParty(const int iIndex)
{
	if (!gMapManager.IsCursedTemple())
	{
		PlayBuffer(SOUND_CLICK01);
		SendRequestPartyLeave(Party[iIndex].Number);
	}

	SetParty(false);

	return true;
}

void CNewUIPartyInfoWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

void CNewUIPartyInfoWindow::SetParty(bool bParty)
{
	m_bParty = bParty;
}

float CNewUIPartyInfoWindow::GetLayerDepth()
{
	return 2.4f;
}

void CNewUIPartyInfoWindow::LoadImages()
{
	LoadBitmap("Common\\Interface\\ui_item_backtop.jpg", IMAGE_PARTY_BASE_WINDOW_TOP);
	LoadBitmap("Common\\Interface\\ui_item_backlow.jpg", IMAGE_PARTY_BASE_WINDOW_LOW);

	LoadBitmap("Common\\Interface\\ui_menu_exit.jpg", IMAGE_PARTY_BASE_WINDOW_BTN_EXIT);

	LoadBitmap("Interface\\newui_party_lifebar01.jpg", IMAGE_PARTY_HPBAR_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_party_lifebar02.jpg", IMAGE_PARTY_HPBAR, GL_LINEAR);
	LoadBitmap("Interface\\newui_party_flag.tga", IMAGE_PARTY_FLAG, GL_LINEAR);
	LoadBitmap("Interface\\newui_party_x.tga", IMAGE_PARTY_EXIT, GL_LINEAR);
}

void CNewUIPartyInfoWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_PARTY_BASE_WINDOW_TOP);
	DeleteBitmap(IMAGE_PARTY_BASE_WINDOW_LOW);

	DeleteBitmap(IMAGE_PARTY_BASE_WINDOW_BTN_EXIT);

	DeleteBitmap(IMAGE_PARTY_HPBAR_BACK);
	DeleteBitmap(IMAGE_PARTY_HPBAR);
	DeleteBitmap(IMAGE_PARTY_FLAG);
	DeleteBitmap(IMAGE_PARTY_EXIT);
}
