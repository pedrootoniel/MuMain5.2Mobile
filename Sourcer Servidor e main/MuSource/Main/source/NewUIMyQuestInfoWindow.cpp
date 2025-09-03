// NewUIMyQuestInfoWindow.cpp: implementation of the CNewUIMyQuestInfoWindow class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIMyQuestInfoWindow.h"
#include "wsclientinline.h"
#include "csQuest.h"

using namespace SEASON3B;

extern int g_iNumLineMessageBoxCustom;
extern int g_iNumAnswer;
extern char g_lpszMessageBoxCustom[NUM_LINE_CMB][MAX_LENGTH_CMB];

SEASON3B::CNewUIMyQuestInfoWindow::CNewUIMyQuestInfoWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;

	this->m_iNumCurOpenTab = TAB_QUEST;
}

SEASON3B::CNewUIMyQuestInfoWindow::~CNewUIMyQuestInfoWindow()
{
	Release(); 
}

bool SEASON3B::CNewUIMyQuestInfoWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if(NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;

	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_MYQUEST, this);

	SetPos(x, y);

	LoadImages();

	SetButtonInfo();

	Show(false);

	return true;
}

void SEASON3B::CNewUIMyQuestInfoWindow::Release()
{
	UnloadImages();

	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}
		
void SEASON3B::CNewUIMyQuestInfoWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	m_BtnTab.ChangeRadioButtonInfo(true, m_Pos.x + 12.f, m_Pos.y + 202.f, 56, 22);
}
		
bool SEASON3B::CNewUIMyQuestInfoWindow::UpdateMouseEvent()
{
	if(BtnProcess() == true)
	{
		return false;
	}

	if(CheckMouseIn(m_Pos.x, m_Pos.y, MYQUESTINFO_WINDOW_WIDTH, MYQUESTINFO_WINDOW_HEIGHT))
	{
		return false;
	}
	
	return true;
}

bool SEASON3B::CNewUIMyQuestInfoWindow::BtnProcess()
{
	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24, 24) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Hide(SEASON3B::INTERFACE_MYQUEST);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	if (SEASON3B::CheckMouseIn(m_Pos.x + 20, m_Pos.y + 242, 49, 16) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		this->m_iNumCurOpenTab = TAB_QUEST;

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	/*if (SEASON3B::CheckMouseIn(m_Pos.x + 70, m_Pos.y + 242, 49, 16) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		this->m_iNumCurOpenTab = TAB_CASTLE;

		SendRequestEventCount(2);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	if (SEASON3B::CheckMouseIn(m_Pos.x + 120, m_Pos.y + 242, 49, 16) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		this->m_iNumCurOpenTab = TAB_TEMPLE;

		SendRequestEventCount(3);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}*/

	return false;
}

bool SEASON3B::CNewUIMyQuestInfoWindow::UpdateKeyEvent()
{
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MYQUEST) == true)
	{
		if(SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_MYQUEST);

			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}

	return true;
}

bool SEASON3B::CNewUIMyQuestInfoWindow::Update()
{
	return true;
}

bool SEASON3B::CNewUIMyQuestInfoWindow::Render()
{
	EnableAlphaTest();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	RenderFrame();
	
	RenderSubjectTexts();

	RenderQuestTexts();

	if (this->m_iNumCurOpenTab == TAB_QUEST)
	{
		RenderTabQuest();
	}
	else if (this->m_iNumCurOpenTab == TAB_CASTLE)
	{
		RenderTabCastle();
	}
	else if (this->m_iNumCurOpenTab == TAB_TEMPLE)
	{
		RenderTabTemple();
	}

	RenderButtons();

	DisableAlphaBlend();
	
	return true;
}
		
float SEASON3B::CNewUIMyQuestInfoWindow::GetLayerDepth()
{
	return 3.3f;
}

void SEASON3B::CNewUIMyQuestInfoWindow::LoadImages()
{
	LoadBitmap("Common\\Interface\\ui_item_backtop.jpg", IMAGE_MYQUEST_TOP);
	LoadBitmap("Common\\Interface\\ui_item_backlow.jpg", IMAGE_MYQUEST_LOW);

	LoadBitmap("Common\\Interface\\ui_menu_exit.jpg", IMAGE_MYQUEST_BTN_EXIT);
	
	LoadBitmap("Common\\Interface\\ui_trading_line.jpg", IMAGE_MYQUEST_LINE);
}

void SEASON3B::CNewUIMyQuestInfoWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_MYQUEST_LINE);
	DeleteBitmap(IMAGE_MYQUEST_BTN_EXIT);
	DeleteBitmap(IMAGE_MYQUEST_TOP);
	DeleteBitmap(IMAGE_MYQUEST_LOW);
}

void SEASON3B::CNewUIMyQuestInfoWindow::RenderFrame()
{
	RenderBitmap(IMAGE_MYQUEST_TOP, m_Pos.x, m_Pos.y, 190.0, 256.0, 0, 0, 0.7421875, 1.0, 1, 1, 0.0);
	RenderBitmap(IMAGE_MYQUEST_LOW, m_Pos.x, m_Pos.y + 256.0, 190.0, 177.0, 0, 0, 0.7421875, 0.69140625, 1, 1, 0.0);
}

void SEASON3B::CNewUIMyQuestInfoWindow::RenderSubjectTexts()
{
	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(230, 230, 230, 255);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y+12, "Quest", 190, 0, RT3_SORT_CENTER);

	g_pRenderText->SetTextColor(36, 242, 252, 255);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 20, g_csQuest.getQuestTitleWindow(), 190, 0, RT3_SORT_CENTER);
}

void SEASON3B::CNewUIMyQuestInfoWindow::RenderQuestTexts()
{
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0);

	int iY = m_Pos.y + 70;
	for (int i = 0; i < g_iNumLineMessageBoxCustom; ++i)
	{
		g_pRenderText->RenderText(m_Pos.x, iY, g_lpszMessageBoxCustom[i], 190.f, 0.f, RT3_SORT_CENTER);
		iY += 18;
	}
}

void SEASON3B::CNewUIMyQuestInfoWindow::RenderTabQuest()
{
	g_pRenderText->SetFont(g_hFontBig);
	g_pRenderText->SetTextColor(223, 191, 103, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 0);

	BYTE byState = g_csQuest.getCurrQuestState();

	if (byState == QUEST_NONE || byState == QUEST_NO || QUEST_ERROR)
	{
		g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 310, GlobalText[930], 190, 0, RT3_SORT_CENTER);
	}
	else if (byState == QUEST_ING)
	{
		g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 310, GlobalText[931], 190, 0, RT3_SORT_CENTER);
	}
	else if (byState == QUEST_END)
	{
		g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 310, GlobalText[932], 190, 0, RT3_SORT_CENTER);
	}
}

void SEASON3B::CNewUIMyQuestInfoWindow::RenderTabCastle()
{
	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(255, 255, 0, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 0);

	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 260, GlobalText[56], 190, 0, RT3_SORT_CENTER);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 0);

	unicode::t_char strText[256];
	unicode::_sprintf(strText, GlobalText[868], g_csQuest.GetEventCount(2));
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 280, strText, 190, 0, RT3_SORT_CENTER);
	
	unicode::_sprintf(strText, GlobalText[829], 6);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 300, strText, 190, 0, RT3_SORT_CENTER);
}

void SEASON3B::CNewUIMyQuestInfoWindow::RenderTabTemple()
{
	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(255, 255, 0, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 0);

	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 260, GlobalText[2369], 190, 0, RT3_SORT_CENTER);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 0);

	unicode::t_char strText[256];
	unicode::_sprintf(strText, GlobalText[868], g_csQuest.GetEventCount(3));
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 280, strText, 190, 0, RT3_SORT_CENTER);
	
	unicode::_sprintf(strText, GlobalText[829], 6);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 300, strText, 190, 0, RT3_SORT_CENTER);
}

void SEASON3B::CNewUIMyQuestInfoWindow::RenderButtons()
{
	RenderBitmap(IMAGE_MYQUEST_LINE, m_Pos.x, m_Pos.y + 230, 190.0, 10.0, 0, 0, 0.7421875, 0.625, 1, 1, 0.0);

	/* */

	if ((SEASON3B::CheckMouseIn(m_Pos.x + 20, m_Pos.y + 242, 49, 16) == true) || (this->m_iNumCurOpenTab == TAB_QUEST))
	{
		glColor3f(1.0, 1.0, 1.0);
	}
	else
	{
		glColor3f(0.69999999, 0.69999999, 0.69999999);
	}

	RenderBitmap(BITMAP_INTERFACE_EX + 9, m_Pos.x + 20, m_Pos.y + 242, 49.0f, 16.f, 0, 0, 0.765625, 1.0, 1, 1, 0.0);

	glColor3f(1.0, 1.0, 1.0);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(0xFFFFFFFF);
	g_pRenderText->SetBgColor(0);

	g_pRenderText->RenderText(m_Pos.x + 35, m_Pos.y + 245, GlobalText[1140], 100, 0, RT3_SORT_LEFT);

	/* */

	glColor3f(1.0, 0.30000001, 0.2);

	RenderBitmap(BITMAP_INTERFACE_EX + 9, m_Pos.x + 70, m_Pos.y + 242, 49.0f, 16.f, 0, 0, 0.765625, 1.0, 1, 1, 0.0);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(0);

	g_pRenderText->RenderText(m_Pos.x + 85, m_Pos.y + 245, GlobalText[1142], 100, 0, RT3_SORT_LEFT);

	glColor3f(1.0, 1.0, 1.0);

	/* */

	glColor3f(1.0, 0.30000001, 0.2);

	RenderBitmap(BITMAP_INTERFACE_EX + 9, m_Pos.x + 120, m_Pos.y + 242, 49.0f, 16.f, 0, 0, 0.765625, 1.0, 1, 1, 0.0);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(0);

	g_pRenderText->RenderText(m_Pos.x + 125, m_Pos.y + 245, GlobalText[2369], 100, 0, RT3_SORT_LEFT);

	glColor3f(1.0, 1.0, 1.0);

	/* */

	RenderBitmap(IMAGE_MYQUEST_LINE, m_Pos.x, m_Pos.y + 260, 190.0, 10.0, 0, 0, 0.7421875, 0.625, 1, 1, 0.0);

	/* */

	RenderBitmap(IMAGE_MYQUEST_LINE, m_Pos.x, m_Pos.y + 383, 190.0, 10.0, 0, 0, 0.7421875, 0.625, 1, 1, 0.0);

	/* */

	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24, 24) == true)
	{
		RenderTipText(m_Pos.x + 20, m_Pos.y + 380, GlobalText[1002]);
	}

	RenderBitmap(IMAGE_MYQUEST_BTN_EXIT, m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
}

void SEASON3B::CNewUIMyQuestInfoWindow::OpenningProcess()
{
	g_csQuest.ShowQuestPreviewWindow(-1);
}

void SEASON3B::CNewUIMyQuestInfoWindow::ClosingProcess()
{
	SendExitInventory();

	PlayBuffer(SOUND_CLICK01);
}

void SEASON3B::CNewUIMyQuestInfoWindow::SetButtonInfo()
{
	std::list<unicode::t_string> TextList;

	TextList.push_back(GlobalText[1140]);

	TextList.push_back(GlobalText[1142]);

	TextList.push_back(GlobalText[2369]);

	/*m_BtnTab.CreateRadioGroup(3, IMAGE_MYQUEST_TAB_BUTTON);
	m_BtnTab.ChangeRadioText(TextList);
	m_BtnTab.ChangeRadioButtonInfo(true, m_Pos.x + 12.f, m_Pos.y + 202.f, 56, 22);
	m_BtnTab.ChangeFrame(TAB_QUEST);*/
}