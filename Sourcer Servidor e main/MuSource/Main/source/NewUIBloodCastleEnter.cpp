// NewUIBloodCastleEnter.cpp: implementation of the CNewUIPartyInfo class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NewUIBloodCastleEnter.h"
#include "NewUISystem.h"
#include "wsclientinline.h"
#include "NewUICommonMessageBox.h"
#include "CharacterManager.h"

using namespace SEASON3B;

CNewUIEnterBloodCastle::CNewUIEnterBloodCastle()
{
	m_pNewUIMng = NULL;
	memset( &m_Pos, 0, sizeof(POINT) );
	memset( &m_EnterUITextPos, 0, sizeof(POINT) );
	
	m_iNumActiveBtn = 1;
	m_BtnEnterStartPos.x = m_BtnEnterStartPos.y = 0;
	m_dwBtnTextColor[0] = RGBA( 150, 150, 150, 255 );
	m_dwBtnTextColor[1] = RGBA( 255, 255 ,255, 255 );

	m_iBloodCastleLimitLevel[0][0] = 15;  m_iBloodCastleLimitLevel[0][1] = 80;
	m_iBloodCastleLimitLevel[1][0] = 81;  m_iBloodCastleLimitLevel[1][1] = 130;
	m_iBloodCastleLimitLevel[2][0] = 131; m_iBloodCastleLimitLevel[2][1] = 180;
	m_iBloodCastleLimitLevel[3][0] = 181; m_iBloodCastleLimitLevel[3][1] = 230;
	m_iBloodCastleLimitLevel[4][0] = 231; m_iBloodCastleLimitLevel[4][1] = 280;
	m_iBloodCastleLimitLevel[5][0] = 281; m_iBloodCastleLimitLevel[5][1] = 330;
	m_iBloodCastleLimitLevel[6][0] = 331; m_iBloodCastleLimitLevel[6][1] = 400;
	m_iBloodCastleLimitLevel[7][0] = 0;   m_iBloodCastleLimitLevel[7][1] = 0;


	m_iBloodCastleLimitLevel[8][0]  = 10;  m_iBloodCastleLimitLevel[8][1]  = 60;
	m_iBloodCastleLimitLevel[9][0]  = 61;  m_iBloodCastleLimitLevel[9][1]  = 110;
	m_iBloodCastleLimitLevel[10][0] = 111; m_iBloodCastleLimitLevel[10][1] = 160;
	m_iBloodCastleLimitLevel[11][0] = 161; m_iBloodCastleLimitLevel[11][1] = 210;
	m_iBloodCastleLimitLevel[12][0] = 211; m_iBloodCastleLimitLevel[12][1] = 260;
	m_iBloodCastleLimitLevel[13][0] = 261; m_iBloodCastleLimitLevel[13][1] = 310;
	m_iBloodCastleLimitLevel[14][0] = 311; m_iBloodCastleLimitLevel[14][1] = 400;
	m_iBloodCastleLimitLevel[15][0] = 0;   m_iBloodCastleLimitLevel[15][1] = 0;
}

CNewUIEnterBloodCastle::~CNewUIEnterBloodCastle()
{
	Release();
}

//---------------------------------------------------------------------------------------------
// Create
bool CNewUIEnterBloodCastle::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if( NULL == pNewUIMng )
		return false;
	
	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj( SEASON3B::INTERFACE_BLOODCASTLE, this );
	
	SetPos(x, y);
	
	LoadImages();
	
	Show( false );
	
	return true;
}

//---------------------------------------------------------------------------------------------
// Release
void CNewUIEnterBloodCastle::Release()
{
	UnloadImages();
	
	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj( this );
		m_pNewUIMng = NULL;
	}
}

//---------------------------------------------------------------------------------------------
// SetPos
void CNewUIEnterBloodCastle::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	m_EnterUITextPos.x = m_Pos.x + 3;
	m_EnterUITextPos.y = m_Pos.y + 55;

	SetBtnPos(m_Pos.x - 7, m_Pos.y + 90);
}

//---------------------------------------------------------------------------------------------
// SetBtnPos
void CNewUIEnterBloodCastle::SetBtnPos(int x, int y)
{
	m_BtnEnterStartPos.x = x;
	m_BtnEnterStartPos.y = y;
}

//---------------------------------------------------------------------------------------------
// UpdateMouseEvent
bool CNewUIEnterBloodCastle::UpdateMouseEvent()
{
	if( true == BtnProcess() )
		return false;
	
	if( CheckMouseIn(m_Pos.x, m_Pos.y, ENTERBC_BASE_WINDOW_WIDTH, ENTERBC_BASE_WINDOW_HEIGHT) )
		return false;

	return true;
}

//---------------------------------------------------------------------------------------------
// UpdateKeyEvent
bool CNewUIEnterBloodCastle::UpdateKeyEvent()
{
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_BLOODCASTLE) == true)
	{
		if(SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_BLOODCASTLE);
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}

	return true;
}

int CNewUIEnterBloodCastle::CheckLimitLV( int iIndex )
{
	int	iVal = 0;
	int iRet = 0;

	if (iIndex == 1)
	{
		iVal = 8;
	}

	int iLevel = CharacterAttribute->Level;

	if (gCharacterManager.IsThirdClass(CharacterAttribute->Class) == false)
	{
		for (int iCastleLV = 0; iCastleLV < MAX_ENTER_GRADE - 1; ++iCastleLV)
		{
			if (iLevel >= m_iBloodCastleLimitLevel[iVal + iCastleLV][0] && iLevel <= m_iBloodCastleLimitLevel[iVal + iCastleLV][1])
			{
				iRet = iCastleLV;
				break;
			}
		}
	}
	else
	{
		iRet = MAX_ENTER_GRADE - 1;
	}

	return iRet;
}

bool CNewUIEnterBloodCastle::Update()
{
	if( !IsVisible() )
		return true;
	
	return true;
}

bool CNewUIEnterBloodCastle::Render()
{
	EnableAlphaTest();

	glColor4f(1.f, 1.f, 1.f, 1.f);
	
	RenderBitmap(IMAGE_ENTERBC_BASE_WINDOW_TOP, m_Pos.x, m_Pos.y, 190.0, 256.0, 0, 0, 0.7421875, 1.0, 1, 1, 0.0);
	RenderBitmap(IMAGE_ENTERBC_BASE_WINDOW_LOW, m_Pos.x, m_Pos.y + 256.0, 190.0, 177.0, 0, 0, 0.7421875, 0.69140625, 1, 1, 0.0);
	
	g_pRenderText->SetFont(g_hFontBold);

	g_pRenderText->SetTextColor(0xFFFFFFFF);

	g_pRenderText->SetBgColor(0x00000000);

	g_pRenderText->RenderText(m_Pos.x + 47, m_Pos.y + 12, GlobalText[846], 100, 0, RT3_SORT_CENTER);

	g_pRenderText->SetFont(g_hFont);

	/* */

	char txtline[NUM_LINE_CMB][MAX_LENGTH_CMB];

	int tl = SeparateTextIntoLines(GlobalText[832], txtline[0], NUM_LINE_CMB, MAX_LENGTH_CMB);

	for (int j = 0; j < tl; ++j)
	{
		g_pRenderText->RenderText(m_EnterUITextPos.x, m_EnterUITextPos.y + j * 20, txtline[j], 190, 0, RT3_SORT_CENTER);
	}

	/* */

	for (int i = 0; i < MAX_ENTER_GRADE; i++)
	{
		if (m_iNumActiveBtn != i)
		{
			glColor3f(0.69999999, 0.69999999, 0.69999999);
		}

		RenderBitmap(IMAGE_ENTERBC_BASE_WINDOW_BTN_ENTER, m_BtnEnterStartPos.x + (190 / 2 - 100 / 2), (m_BtnEnterStartPos.y + 60) + (i * 30), 120.0, 22.0, 0, 0, ((double)213 / 256.0), 1.0, 1, 1, 0.0);
		
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
			unicode::_sprintf(sztext, GlobalText[1779], 8);
		}
		else
		{
			unicode::_sprintf(sztext, GlobalText[847], i + 1, m_iBloodCastleLimitLevel[(iLimitLVIndex * MAX_ENTER_GRADE) + i][0], m_iBloodCastleLimitLevel[(iLimitLVIndex * MAX_ENTER_GRADE) + i][1]);
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

	RenderBitmap(IMAGE_ENTERBC_BASE_WINDOW_BTN_EXIT, m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);

	/* */

	DisableAlphaBlend();

	return true;
}

bool CNewUIEnterBloodCastle::BtnProcess()
{
	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24, 24) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Hide(SEASON3B::INTERFACE_BLOODCASTLE);

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

				iItemIndex = g_pMyInventory->FindItemIndex(ITEM_HELPER + 18, m_iNumActiveBtn + 1);

				if (iItemIndex == -1)
				{
					iItemIndex = g_pMyInventory->FindItemIndex(ITEM_HELPER + 47, -1);
				}

				if (iItemIndex > -1)
				{
					SendRequestMoveToEventMatch(m_iNumActiveBtn + 1, MAX_EQUIPMENT + iItemIndex);
				}
				else
				{
					SEASON3B::CreateOkMessageBox(GlobalText[854]);
				}

				g_pNewUISystem->Hide(SEASON3B::INTERFACE_BLOODCASTLE);

				PlayBuffer(SOUND_CLICK01);
			}
		}
	}
	
	return false;
}

float CNewUIEnterBloodCastle::GetLayerDepth()
{
	return 4.1f;
}

void CNewUIEnterBloodCastle::OpenningProcess()
{
	SendExitInventory();
}

void CNewUIEnterBloodCastle::ClosingProcess()
{
	SendExitInventory();
}

void CNewUIEnterBloodCastle::LoadImages()
{
	LoadBitmap("Common\\Interface\\ui_item_backtop.jpg", IMAGE_ENTERBC_BASE_WINDOW_TOP);
	LoadBitmap("Common\\Interface\\ui_item_backlow.jpg", IMAGE_ENTERBC_BASE_WINDOW_LOW);
	LoadBitmap("Common\\Interface\\ui_menu_exit.jpg", IMAGE_ENTERBC_BASE_WINDOW_BTN_EXIT);
	LoadBitmap("Common\\Interface\\ui_message_box.jpg", IMAGE_ENTERBC_BASE_WINDOW_BTN_ENTER);
}

void CNewUIEnterBloodCastle::UnloadImages()
{
	DeleteBitmap(IMAGE_ENTERBC_BASE_WINDOW_TOP);
	DeleteBitmap(IMAGE_ENTERBC_BASE_WINDOW_LOW);
	DeleteBitmap(IMAGE_ENTERBC_BASE_WINDOW_BTN_EXIT);
	DeleteBitmap(IMAGE_ENTERBC_BASE_WINDOW_BTN_ENTER);
}
