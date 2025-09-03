// NewUIGoldBowmanLena.cpp: implementation of the NewUIGoldBowmanLena class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIGoldBowmanLena.h"
#include "NewUISystem.h"
#include "wsclientinline.h"
#include "MixMgr.h"

namespace 
{
	void RenderText( char* text, int x, int y, int sx, int sy, DWORD color, DWORD backcolor, int sort )
	{
		g_pRenderText->SetFont(g_hFont);
		
		DWORD backuptextcolor = g_pRenderText->GetTextColor();
		DWORD backuptextbackcolor = g_pRenderText->GetBgColor();
		
		g_pRenderText->SetTextColor(color);
		g_pRenderText->SetBgColor(backcolor);
		g_pRenderText->RenderText(x, y, text, sx, sy, sort);
		
		g_pRenderText->SetTextColor(backuptextcolor);
		g_pRenderText->SetBgColor(backuptextbackcolor);
	}
};

using namespace SEASON3B;

CNewUIGoldBowmanLena::CNewUIGoldBowmanLena()
{

}

CNewUIGoldBowmanLena::~CNewUIGoldBowmanLena()
{
	Release();
}

bool CNewUIGoldBowmanLena::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng) {
		return false;
	}

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_GOLD_BOWMAN_LENA, this);
	
	SetPos(x, y);

	LoadImages();

	Show(false);

	return true;
}

void CNewUIGoldBowmanLena::Release()
{
	UnloadImages();
}

void CNewUIGoldBowmanLena::LoadImages()
{
	LoadBitmap("Common\\Interface\\ui_item_backtop.jpg", IMAGE_GBL_TOP);
	LoadBitmap("Common\\Interface\\ui_item_backlow.jpg", IMAGE_GBL_LOW);

	LoadBitmap("Common\\Interface\\ui_message_box.jpg", IMAGE_GBL_EXCHANGEBTN);
	LoadBitmap("Common\\Interface\\ui_message_box.jpg", IMAGE_GBL_BTN_SERIAL);

	LoadBitmap("Common\\Interface\\ui_menu_exit.jpg", IMAGE_GBL_BTN_EXIT);
}

void CNewUIGoldBowmanLena::UnloadImages()
{
	DeleteBitmap(IMAGE_GBL_BTN_EXIT);
	DeleteBitmap(IMAGE_GBL_BTN_SERIAL);
	DeleteBitmap(IMAGE_GBL_EXCHANGEBTN);

	DeleteBitmap(IMAGE_GBL_TOP);
	DeleteBitmap(IMAGE_GBL_LOW);
}

void CNewUIGoldBowmanLena::OpeningProcess()
{
	
}

void CNewUIGoldBowmanLena::ClosingProcess()
{
	g_bEventChipDialogEnable = 0;
	g_shEventChipCount = 0;
	SendRequestEventChipExit();
}

bool CNewUIGoldBowmanLena::UpdateMouseEvent()
{
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GOLD_BOWMAN_LENA) == false) 
	{
		return true;
	}

	if (SEASON3B::CheckMouseIn(m_Pos.x + 45, m_Pos.y + 285, 100, 25) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		int registerItem = g_pMyInventory->GetInventoryCtrl()->GetItemCount(ITEM_POTION + 21, 0);

		if (registerItem != 0)
		{
			int index = g_pMyInventory->GetInventoryCtrl()->FindItemIndex(ITEM_POTION + 21, 0);

			if (index != -1)
			{
				SendRequestEventChip(0, index);
			}
		}

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24, 24) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Hide(SEASON3B::INTERFACE_GOLD_BOWMAN_LENA);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	if(CheckMouseIn(m_Pos.x, m_Pos.y, INVENTORY_WIDTH, INVENTORY_HEIGHT)) 
	{
		if(SEASON3B::IsPress(VK_RBUTTON)) 
		{
			MouseRButton = false;
			MouseRButtonPop = false;
			MouseRButtonPush = false;
			return false;
		}
		
		if(SEASON3B::IsNone(VK_LBUTTON) == false) 
		{
			return false;
		}

		return false;
	}
	else
	{
		if(SEASON3B::IsNone(VK_LBUTTON) == false) 
		{
			return false;
		}

		return false;
	}

	return true;
}

bool CNewUIGoldBowmanLena::UpdateKeyEvent()
{
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GOLD_BOWMAN_LENA) == false) 
	{
		return true;
	}
	
	if(SEASON3B::IsPress(VK_ESCAPE) == true) 
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_GOLD_BOWMAN_LENA);
		return false;
	}

	return true;
}

bool CNewUIGoldBowmanLena::Update()
{
	return true;
}

bool CNewUIGoldBowmanLena::Render()
{
	EnableAlphaTest();

	glColor4f(1.f, 1.f, 1.f, 1.f);

	RenderFrame();
	
	RenderTexts();

	RendeerButton();

	DisableAlphaBlend();

	Render3D();

	return true;
}

void CNewUIGoldBowmanLena::RenderFrame()
{
	RenderBitmap(IMAGE_GBL_TOP, m_Pos.x, m_Pos.y, 190.0, 256.0, 0, 0, 0.7421875, 1.0, 1, 1, 0.0);
	RenderBitmap(IMAGE_GBL_LOW, m_Pos.x, m_Pos.y + 256.0, 190.0, 177.0, 0, 0, 0.7421875, 0.69140625, 1, 1, 0.0);
}

void CNewUIGoldBowmanLena::RenderTexts()
{
	char Text[100];
	
	memset(&Text, 0, sizeof(char)*100);
	sprintf( Text, getMonsterName(236) );
	RenderText( Text, m_Pos.x, m_Pos.y+15, 190, 0, 0xFFFFFFFF, 0x00000000, RT3_SORT_CENTER );
	
	for( int i = 0; i < 3; ++i ) {
		memset(&Text, 0, sizeof(char)*100);
		sprintf( Text, GlobalText[700+i] );
		RenderText( Text, m_Pos.x, m_Pos.y+100+(i*15), 190, 0, 0xFFFFFFFF, 0x00000000, RT3_SORT_CENTER );
	}

	int registerItem = g_pMyInventory->GetInventoryCtrl()->GetItemCount( ITEM_POTION+21, 0 );

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(0xFF47DFFA);
	g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 180, GlobalText[245], 120, 0, RT3_SORT_CENTER);

	memset(&Text, 0, sizeof(char)*100);
	sprintf( Text, "    X    %d", registerItem);
	RenderText( Text, m_Pos.x+5, m_Pos.y+202, 190, 0, 0xFFFFFFFF, 0x00000000, RT3_SORT_CENTER );

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(0xFF47DFFA);
	g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 225, GlobalText[246], 120, 0, RT3_SORT_CENTER);

	memset(&Text, 0, sizeof(char)*100);
	sprintf( Text, "    X    %d", g_shEventChipCount);
	RenderText( Text, m_Pos.x+5, m_Pos.y+245, 190, 0, 0xFFFFFFFF, 0x00000000, RT3_SORT_CENTER );
}

void CNewUIGoldBowmanLena::RendeerButton()
{
	if (SEASON3B::CheckMouseIn(m_Pos.x + 45, m_Pos.y + 285, 100.0, 25.0) == true)
	{
		RenderTipText(m_Pos.x + 42, m_Pos.y + 270, GlobalText[243]);

		glColor3f(1.0, 1.0, 1.0);
	}
	else
	{
		glColor3f(0.69999999, 0.69999999, 0.69999999);
	}

	RenderBitmap(IMAGE_GBL_EXCHANGEBTN, m_Pos.x + 45, m_Pos.y + 285, 100.0, 25.0, 0, 0, ((double)213 / 256.0), 1.0, 1, 1, 0.0);

	glColor3f(1.0, 1.0, 1.0);

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->RenderText(m_Pos.x + 60, m_Pos.y + 293, GlobalText[243], 72, 0, RT3_SORT_CENTER);

	/* */

	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0) == true)
	{
		RenderTipText(m_Pos.x + 20, m_Pos.y + 380, GlobalText[1002]);
	}

	RenderBitmap(IMAGE_GBL_BTN_EXIT, m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
}

float CNewUIGoldBowmanLena::GetLayerDepth()	// 3.4f
{
	return 3.4f;
}

void CNewUIGoldBowmanLena::Render3D()
{
	EndBitmap();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport2(0, 0, WindowWidth, WindowHeight);
	gluPerspective2(1.f, (float)(WindowWidth) / (float)(WindowHeight), RENDER_ITEMVIEW_NEAR, RENDER_ITEMVIEW_FAR);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	GetOpenGLMatrix(CameraMatrix);
	EnableDepthTest();
	EnableDepthMask();

	int Type = ITEM_POTION + 21;
	int Level = 0;
	float x = 640.f - 120.f;
	float y = 200.f;
	float Width = (float)ItemAttribute[Type].Width * INVENTORY_SCALE;
	float Height = (float)ItemAttribute[Type].Height * INVENTORY_SCALE;
	RenderItem3D(x, y, Width, Height, Type, Level, 0, 0, false);
	RenderItem3D(x, y + 42, Width, Height, Type, Level, 0, 0, false);

	UpdateMousePositionn();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	BeginBitmap();
}