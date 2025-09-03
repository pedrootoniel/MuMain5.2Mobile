// NewUIEnterBloodCastle.cpp: implementation of the CNewUIPartyInfo class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NewUIEnterDevilSquare.h"
#include "NewUISystem.h"
#include "NewUICommonMessageBox.h"
#include "wsclientinline.h"
#include "ZzzCharacter.h"
#include "CharacterManager.h"

using namespace SEASON3B;

CNewUIEnterDevilSquare::CNewUIEnterDevilSquare()
{
	m_pNewUIMng = NULL;
	memset(&m_Pos, 0, sizeof(POINT));
	memset(&m_EnterUITextPos, 0, sizeof(POINT));

	m_iNumActiveBtn = 1;
	m_BtnEnterStartPos.x = m_BtnEnterStartPos.y = 0;
	m_dwBtnTextColor[0] = RGBA(150, 150, 150, 255);
	m_dwBtnTextColor[1] = RGBA(255, 255, 255, 255);

	m_iDevilSquareLimitLevel[0][0] = 15; m_iDevilSquareLimitLevel[0][1] = 130;
	m_iDevilSquareLimitLevel[1][0] = 131; m_iDevilSquareLimitLevel[1][1] = 180;
	m_iDevilSquareLimitLevel[2][0] = 181; m_iDevilSquareLimitLevel[2][1] = 230;
	m_iDevilSquareLimitLevel[3][0] = 231; m_iDevilSquareLimitLevel[3][1] = 280;
	m_iDevilSquareLimitLevel[4][0] = 281; m_iDevilSquareLimitLevel[4][1] = 330;
	m_iDevilSquareLimitLevel[5][0] = 331; m_iDevilSquareLimitLevel[5][1] = 400;
	m_iDevilSquareLimitLevel[6][0] = 0; m_iDevilSquareLimitLevel[6][1] = 0;

	m_iDevilSquareLimitLevel[7][0] = 15; m_iDevilSquareLimitLevel[7][1] = 110;
	m_iDevilSquareLimitLevel[8][0] = 111; m_iDevilSquareLimitLevel[8][1] = 160;
	m_iDevilSquareLimitLevel[9][0] = 161; m_iDevilSquareLimitLevel[9][1] = 210;
	m_iDevilSquareLimitLevel[10][0] = 211; m_iDevilSquareLimitLevel[10][1] = 260;
	m_iDevilSquareLimitLevel[11][0] = 261; m_iDevilSquareLimitLevel[11][1] = 310;
	m_iDevilSquareLimitLevel[12][0] = 311; m_iDevilSquareLimitLevel[12][1] = 400;
	m_iDevilSquareLimitLevel[13][0] = 0; m_iDevilSquareLimitLevel[13][1] = 0;
}

CNewUIEnterDevilSquare::~CNewUIEnterDevilSquare()
{
	Release();
}

bool CNewUIEnterDevilSquare::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_DEVILSQUARE, this);

	SetPos(x, y);

	LoadImages();

	Show(false);

	return true;
}

void CNewUIEnterDevilSquare::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void CNewUIEnterDevilSquare::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	m_EnterUITextPos.x = m_Pos.x + 3;
	m_EnterUITextPos.y = m_Pos.y + 55;

	SetBtnPos(m_Pos.x - 7, m_Pos.y + 90);
}

void CNewUIEnterDevilSquare::SetBtnPos(int x, int y)
{
	m_BtnEnterStartPos.x = x;
	m_BtnEnterStartPos.y = y;
}

bool CNewUIEnterDevilSquare::UpdateMouseEvent()
{
	if (true == BtnProcess())
		return false;

	if (CheckMouseIn(m_Pos.x, m_Pos.y, ENTERDS_BASE_WINDOW_WIDTH, ENTERDS_BASE_WINDOW_HEIGHT))
		return false;

	return true;
}

bool CNewUIEnterDevilSquare::UpdateKeyEvent()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_DEVILSQUARE) == true)
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_DEVILSQUARE);
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}

	return true;
}

bool CNewUIEnterDevilSquare::Update()
{
	if (!IsVisible())
		return true;

	return true;
}

bool CNewUIEnterDevilSquare::Render()
{
	EnableAlphaTest();

	RenderBitmap(IMAGE_ENTERDS_BASE_WINDOW_TOP, m_Pos.x, m_Pos.y, 190.0, 256.0, 0, 0, 0.7421875, 1.0, 1, 1, 0.0);
	RenderBitmap(IMAGE_ENTERDS_BASE_WINDOW_LOW, m_Pos.x, m_Pos.y + 256.0, 190.0, 177.0, 0, 0, 0.7421875, 0.69140625, 1, 1, 0.0);

	/* */

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(0xFFFFFFFF);
	g_pRenderText->SetBgColor(0x00000000);
	g_pRenderText->RenderText(m_Pos.x + 60, m_Pos.y + 12, GlobalText[39], 72, 0, RT3_SORT_CENTER);
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->RenderText(m_EnterUITextPos.x, m_EnterUITextPos.y, GlobalText[670], 190, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(m_EnterUITextPos.x, m_EnterUITextPos.y + 15, GlobalText[671], 190, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(m_EnterUITextPos.x, m_EnterUITextPos.y + 30, GlobalText[672], 190, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(m_EnterUITextPos.x, m_EnterUITextPos.y + 45, GlobalText[673], 190, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(m_EnterUITextPos.x, m_EnterUITextPos.y + 60, GlobalText[674], 190, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(m_EnterUITextPos.x, m_EnterUITextPos.y + 75, GlobalText[675], 190, 0, RT3_SORT_CENTER);

	/* */

	for (int i = 0; i < MAX_ENTER_GRADE; i++)
	{
		if (m_iNumActiveBtn != i)
		{
			glColor3f(0.69999999, 0.69999999, 0.69999999);
		}

		RenderBitmap(IMAGE_ENTERDS_BASE_WINDOW_BTN_ENTER, m_BtnEnterStartPos.x + (190 / 2 - 100 / 2), (m_BtnEnterStartPos.y + 60) + (i * 30), 120.0, 22.0, 0, 0, ((double)213 / 256.0), 1.0, 1, 1, 0.0);

		glColor3f(1.f, 1.f, 1.f);
	}

	/* */

	int iLimitLVIndex = 0;

	if (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_DARK || gCharacterManager.GetBaseClass(Hero->Class) == CLASS_DARK_LORD)
	{
		iLimitLVIndex = 1;
	}

	m_iNumActiveBtn = CheckLimitLV(iLimitLVIndex);

	unicode::t_char sztext[255] = { 0, };

	for (int i = 0; i < MAX_ENTER_GRADE; i++)
	{
		if (m_iNumActiveBtn != i)
		{
			glColor3f(0.69999999, 0.69999999, 0.69999999);
		}

		if (i == MAX_ENTER_GRADE - 1)
		{
			unicode::_sprintf(sztext, GlobalText[1778], 7);
		}
		else
		{
			unicode::_sprintf(sztext, GlobalText[645], i + 1, m_iDevilSquareLimitLevel[(iLimitLVIndex * (MAX_ENTER_GRADE)) + i][0], m_iDevilSquareLimitLevel[(iLimitLVIndex * (MAX_ENTER_GRADE)) + i][1]);
		}

		g_pRenderText->SetFont(g_hFontBold);

		g_pRenderText->RenderText(m_BtnEnterStartPos.x + 10, (m_BtnEnterStartPos.y + 60) + (i * 30) + 8, sztext, 190, 0, RT3_SORT_CENTER);

		glColor3f(1.f, 1.f, 1.f);
	}

	/* */

	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0) == true)
	{
		RenderTipText(m_Pos.x + 20, m_Pos.y + 380, GlobalText[1002]);
	}

	RenderBitmap(IMAGE_ENTERDS_BASE_WINDOW_BTN_EXIT, m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);

	/* */

	DisableAlphaBlend();

	return true;
}

//---------------------------------------------------------------------------------------------
// BtnProcess
bool CNewUIEnterDevilSquare::BtnProcess()
{
	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24, 24) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Hide(SEASON3B::INTERFACE_DEVILSQUARE);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	int iLimitLVIndex = 0;

	if (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_DARK || gCharacterManager.GetBaseClass(Hero->Class) == CLASS_DARK_LORD)
	{
		iLimitLVIndex = 1;
	}

	m_iNumActiveBtn = CheckLimitLV(iLimitLVIndex);

	for (int i = 0; i < MAX_ENTER_GRADE; i++)
	{
		if (this->m_iNumActiveBtn == i)
		{
			if ((m_iNumActiveBtn != -1) && SEASON3B::CheckMouseIn(m_BtnEnterStartPos.x + (190 / 2 - 100 / 2), (m_BtnEnterStartPos.y + 60) + (i * 30), 120, 22) == true && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;
				MouseUpdateTimeMax = 6;

				int iItemIndex = -1;

				iItemIndex = g_pMyInventory->FindItemIndex(ITEM_POTION + 19, 0);

				if (iItemIndex == -1)
				{
					iItemIndex = g_pMyInventory->FindItemIndex(ITEM_POTION + 19, m_iNumActiveBtn + 1);
				}

				if (iItemIndex == -1)
				{
					iItemIndex = g_pMyInventory->FindItemIndex(ITEM_HELPER + 46, -1);
				}

				if (iItemIndex > -1)
				{
					SendRequestMoveToDevilSquare(m_iNumActiveBtn, MAX_EQUIPMENT + iItemIndex);
				}
				else
				{
					SEASON3B::CreateOkMessageBox(GlobalText[677]);
				}

				g_pNewUISystem->Hide(SEASON3B::INTERFACE_DEVILSQUARE);

				PlayBuffer(SOUND_CLICK01);
			}
		}
	}

	return false;
}

float CNewUIEnterDevilSquare::GetLayerDepth()
{
	return 4.0f;
}

int CNewUIEnterDevilSquare::CheckLimitLV(int iIndex)
{
	int	iVal = 0;
	int iRet = 0;

	if (iIndex == 1)
	{
		iVal = 7;
	}

	int iLevel;
	
	iLevel = CharacterAttribute->Level;

	if (gCharacterManager.IsThirdClass(CharacterAttribute->Class) == false)
	{
		for (int iCastleLV = 0; iCastleLV < MAX_ENTER_GRADE - 1; ++iCastleLV)
		{
			if (iLevel >= m_iDevilSquareLimitLevel[iVal + iCastleLV][0]
				&& iLevel <= m_iDevilSquareLimitLevel[iVal + iCastleLV][1])
			{
				iRet = iCastleLV;
				break;
			}
		}
	}
	else
		iRet = MAX_ENTER_GRADE - 1;


	return iRet;
}

void CNewUIEnterDevilSquare::OpenningProcess()
{
	SendExitInventory();
}

void CNewUIEnterDevilSquare::ClosingProcess()
{
	SendExitInventory();
}

void CNewUIEnterDevilSquare::LoadImages()
{
	LoadBitmap("Common\\Interface\\ui_item_backtop.jpg", IMAGE_ENTERDS_BASE_WINDOW_TOP);
	LoadBitmap("Common\\Interface\\ui_item_backlow.jpg", IMAGE_ENTERDS_BASE_WINDOW_LOW);
	LoadBitmap("Common\\Interface\\ui_menu_exit.jpg", IMAGE_ENTERDS_BASE_WINDOW_BTN_EXIT);
	LoadBitmap("Common\\Interface\\ui_message_box.jpg", IMAGE_ENTERDS_BASE_WINDOW_BTN_ENTER);
}

void CNewUIEnterDevilSquare::UnloadImages()
{
	DeleteBitmap(IMAGE_ENTERDS_BASE_WINDOW_TOP);
	DeleteBitmap(IMAGE_ENTERDS_BASE_WINDOW_LOW);

	DeleteBitmap(IMAGE_ENTERDS_BASE_WINDOW_BTN_EXIT);		// Exit Button
	DeleteBitmap(IMAGE_ENTERDS_BASE_WINDOW_BTN_ENTER);	// Enter Button
}
