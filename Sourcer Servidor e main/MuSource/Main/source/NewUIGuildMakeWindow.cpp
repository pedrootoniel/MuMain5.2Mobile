// NewUIGuildMakeWindow.cpp: implementation of the CNewUIGuildMakeWindow class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIGuildMakeWindow.h"
#include "NewUIManager.h"
#include "NewUICommonMessageBox.h"
#include "DSPlaySound.h"
#include "wsclientinline.h"
#include "ZzzInterface.h"
#include "ZzzInventory.h"
#include "Local.h"
#include "..\InterfaceManager.h"

extern MARK_t		GuildMark[MAX_MARKS];
extern int			SelectMarkColor;

namespace
{
	BOOL IsGuildName( const char* szName )
	{
		if( strlen( szName ) >= 4 )
			return TRUE;
		else
			return FALSE;
	}
	
	BOOL IsGuildMark()
	{
		BOOL bDraw = FALSE;

		for( int i=0;i<64;i++ )
		{
			if( GuildMark[MARK_EDIT].Mark[i] != 0 )
				return TRUE;
		}
		
		return FALSE;
	}

	void UpdateEditGuildMark( int iPos_x, int iPos_y )
	{
		int i, j;
		float x, y;
		Hero->Object.Angle[2] = 90.f + 22.5f;
		for( i=0 ; i<8 ; ++i )
		{
			for( j=0; j<8 ; ++j )
			{
				x = iPos_x + j*15 + 50;
				y = iPos_y + i*15 + 100;
				if( MouseX>=x && MouseX<x+15 && MouseY>=y && MouseY<y+15 )
				{
					if( MouseLButton )
						GuildMark[MARK_EDIT].Mark[i*8+j] = SelectMarkColor;
					if( MouseRButton )
						GuildMark[MARK_EDIT].Mark[i*8+j] = 0;
				}
			}
		}
		for( i=0 ; i<2 ; ++i )
		{
			for( j=0 ; j<8 ; ++j )
			{
				x = iPos_x + j*20 + 15;
				y = iPos_y + i*20 + 260;
				if( MouseX>=x && MouseX<x+20 && MouseY>=y && MouseY<y+20 )
				{
					if( MouseLButtonPush )
					{
						MouseLButtonPush = FALSE;
						PlayBuffer( SOUND_CLICK01 );
						SelectMarkColor = i*8 + j;
					}
				}
			}
		}
	}

	void RenderEditGuildMark( int iPos_x, int iPos_y )
	{
		int i, j;
		float x, y;
		for( i=0 ; i<8 ; ++i )
		{
			for( j=0 ; j<8 ; ++j )
			{
				x = (float)iPos_x + j*15 + 50;
				y = (float)iPos_y + i*15 + 100;
				RenderGuildColor( x+1, y+1, 13, 13, GuildMark[MARK_EDIT].Mark[i*8+j] );
			}
		}
		for( i=0 ; i<2 ; ++i )
		{
			for( j=0 ; j<8 ; ++j )
			{
				x = (float)iPos_x + j*20 + 15;
				y = (float)iPos_y + i*20 + 260;
				RenderGuildColor( x+1, y+1, 18, 18, i*8+j );
			}
		}
		x = (float)iPos_x + 15;
		y = (float)iPos_y + 230;
		RenderGuildColor( x+1, y+1, 23, 23, SelectMarkColor );

		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->SetTextColor(230, 230, 230, 255);
		g_pRenderText->SetBgColor(0, 0, 0, 0);
		g_pRenderText->RenderText(iPos_x+50, iPos_y+230, GlobalText[183]);
		g_pRenderText->RenderText(iPos_x+50, iPos_y+245, GlobalText[184]);
	}

	void RenderGoldRect(float x, float y, float sx, float sy, int fill = 0)
	{
		switch (fill)
		{
		case 1:
			glColor4ub(146, 144, 141, 200);
			RenderColor(x, y, sx, sy);
			EndRenderColor();
			break;
		};

		RenderBitmap(BITMAP_INVENTORY+19, x, y, sx, 2,10/256.f, 5/16.f, 170.f/256.f, 2.f/16.f);
		RenderBitmap(BITMAP_INVENTORY+19, x, y + sy, sx + 1, 2,	10/256.f, 5/16.f, 170.f/256.f, 2.f/16.f);
		RenderBitmap(BITMAP_INVENTORY, x, y, 2, sy,	1.f/256.f, 5/16.f, 2.f/256.f, 125.f/256.f);
		RenderBitmap(BITMAP_INVENTORY, x + sx, y, 2, sy, 1.f/256.f, 5/16.f, 2.f/256.f, 125.f/256.f);
	}

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

CNewUIGuildMakeWindow::CNewUIGuildMakeWindow() : m_pNewUIMng(NULL), m_EditBox(NULL), m_Button(NULL),
 m_GuildMakeState( GUILDMAKE_INFO )
{

}

CNewUIGuildMakeWindow::~CNewUIGuildMakeWindow()
{
	Release();
}

bool CNewUIGuildMakeWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if(NULL == pNewUIMng)
		return false;

	LoadImages();
	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_NPCGUILDMASTER, this);
	SetPos(x, y);

	m_EditBox = new CUITextInputBox;
	m_EditBox->Init(g_hWnd, 200, 14, MAXGUILDNAME);
	m_EditBox->SetPosition(m_Pos.x+60, m_Pos.y+66);
	m_EditBox->SetTextColor(255, 255, 230, 210);
	m_EditBox->SetBackColor(0, 0, 0, 25);
	m_EditBox->SetFont(g_hFont);
	m_EditBox->SetState(UISTATE_NORMAL);
	m_EditBox->SetOption(UIOPTION_NOLOCALIZEDCHARACTERS);
	m_Button = new CNewUIButton[GUILDMAKEBUTTON_COUNT];

	for( int i = 0; i < GUILDMAKEBUTTON_COUNT; ++i )
	{
		if( i == 0 )
		{
			m_Button[i].ChangeButtonImgState( true, IMAGE_GUILDMAKE_MAKEBUTTON, true );
			m_Button[i].ChangeButtonInfo( 0, 0, 108, 29 );
		}
		else
		{
			m_Button[i].ChangeButtonImgState( true, IMAGE_GUILDMAKE_NEXTBUTTON, true );
			m_Button[i].ChangeButtonInfo( 0, 0, 64, 29 );
		}
	}
	
	Show(false);

	return true;
}

void CNewUIGuildMakeWindow::Release()
{
	SAFE_DELETE_ARRAY(m_Button);
	SAFE_DELETE(m_EditBox);

	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}

	UnloadImages();
}

void CNewUIGuildMakeWindow::LoadImages()
{
	LoadBitmap("Common\\Interface\\ui_item_backtop.jpg", IMAGE_GUILDMAKE_BACK_TOP);
	LoadBitmap("Common\\Interface\\ui_item_backlow.jpg", IMAGE_GUILDMAKE_BACK_LOW);
	
	LoadBitmap("Common\\Interface\\ui_message_box.jpg", IMAGE_GUILDMAKE_MAKEBUTTON);
	LoadBitmap("Common\\Interface\\ui_message_box.jpg", IMAGE_GUILDMAKE_NEXTBUTTON);

	LoadBitmap("Common\\Interface\\ui_menu_exit.jpg", IMAGE_GUILDMAKE_BTN_EXIT);

	LoadBitmap("Common\\Interface\\ui_item_money.jpg", IMAGE_GUILDMAKE_EDITBOX);
}

void CNewUIGuildMakeWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_GUILDMAKE_EDITBOX);
	DeleteBitmap(IMAGE_GUILDMAKE_BTN_EXIT);
	DeleteBitmap(IMAGE_GUILDMAKE_NEXTBUTTON);
	DeleteBitmap(IMAGE_GUILDMAKE_MAKEBUTTON);
	DeleteBitmap(IMAGE_GUILDMAKE_BACK_TOP);
	DeleteBitmap(IMAGE_GUILDMAKE_BACK_LOW);
}

float CNewUIGuildMakeWindow::GetLayerDepth()
{
	return 4.3f;
}

void CNewUIGuildMakeWindow::ClosingProcess()
{
	ChangeWindowState(GUILDMAKE_INFO);
	ChangeEditBox(UISTATE_HIDE);

	SendRequestGuildMaster( FALSE );
}

void CNewUIGuildMakeWindow::ChangeWindowState(const GUILDMAKE_STATE state)
{
	m_GuildMakeState = state;
}

void CNewUIGuildMakeWindow::ChangeEditBox(const UISTATES type)
{
	m_EditBox->SetState(type);

	if( type == UISTATE_NORMAL )
	{
		m_EditBox->GiveFocus();
	}

	m_EditBox->SetText(NULL);
}

bool CNewUIGuildMakeWindow::UpdateGMInfo()
{
	if (SEASON3B::CheckMouseIn(m_Pos.x + 47, m_Pos.y + 80, 100, 18) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		SendRequestGuildMaster(TRUE);

		ChangeWindowState(GUILDMAKE_MARK);

		ChangeEditBox(UISTATE_NORMAL);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24, 24) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Hide(SEASON3B::INTERFACE_NPCGUILDMASTER);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	return false;
}

bool CNewUIGuildMakeWindow::UpdateGMMark()
{
	m_EditBox->DoAction();

	UpdateEditGuildMark(m_Pos.x, m_Pos.y);
	
	if (SEASON3B::CheckMouseIn(m_Pos.x + 40, m_Pos.y + 379, 49, 16) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		ChangeWindowState(GUILDMAKE_INFO);

		ChangeEditBox(UISTATE_HIDE);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	if (SEASON3B::CheckMouseIn(m_Pos.x + 100, m_Pos.y + 379, 49, 16) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		char tempText[100];

		memset(&tempText, 0, sizeof(char) * 100);

		m_EditBox->GetText(tempText);

		PlayBuffer(SOUND_CLICK01);

		if (CheckSpecialText(tempText) == true)
		{
			SEASON3B::CreateOkMessageBox(GlobalText[391]);
		}
		else if (IsGuildName(tempText) == FALSE)
		{
			CreateOkMessageBox(GlobalText[390]);
		}
		else if (IsGuildMark() == FALSE)
		{
			CreateOkMessageBox(GlobalText[426]);
		}
		else
		{
			strcpy(GuildMark[MARK_EDIT].GuildName, tempText);

			ChangeWindowState(GUILDMAKE_RESULTINFO);

			ChangeEditBox(UISTATE_HIDE);

			return true;
		}		
	}

	return false;
}

bool CNewUIGuildMakeWindow::UpdateGMResultInfo()
{
	if (SEASON3B::CheckMouseIn(m_Pos.x + 40, m_Pos.y + 379, 49, 16) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		ChangeWindowState(GUILDMAKE_MARK);

		ChangeEditBox(UISTATE_NORMAL);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	if (SEASON3B::CheckMouseIn(m_Pos.x + 100, m_Pos.y + 379, 49, 16) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		BYTE Mark[32];

		for (int i = 0; i < 64; i++)
		{
			if (i % 2 == 0)
				Mark[i / 2] = GuildMark[MARK_EDIT].Mark[i] << 4;
			else
				Mark[i / 2] += GuildMark[MARK_EDIT].Mark[i];
		}

		SendRequestCreateGuild(0, (BYTE*)GuildMark[MARK_EDIT].GuildName, Mark);

		g_pNewUISystem->Hide(SEASON3B::INTERFACE_NPCGUILDMASTER);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	return false;
}

void CNewUIGuildMakeWindow::RenderGMInfo()
{
	char Text[100];

	memset(&Text, 0, sizeof(char) * 100);

	sprintf(Text, GlobalText[181]);

	RenderText(Text, m_Pos.x, m_Pos.y + 50, 190, 0, 0xFFFFFFFF, 0x00000000, RT3_SORT_CENTER);

	if (SEASON3B::CheckMouseIn(m_Pos.x + 47, m_Pos.y + 80, 100, 18) == true)
	{
		glColor3f(1.0, 1.0, 1.0);
	}
	else
	{
		glColor3f(0.69999999, 0.69999999, 0.69999999);	
	}

	RenderBitmap(BITMAP_INTERFACE_EX + 9, m_Pos.x + 47, m_Pos.y + 80, 100.f, 18.f, 0, 0, 0.765625, 1.0, 1, 1, 0.0);

	glColor3f(1.0, 1.0, 1.0);

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x + 75, m_Pos.y + 84, GlobalText[1303], 74, 0, RT3_SORT_LEFT);

	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0) == true)
	{
		RenderTipText(m_Pos.x + 20, m_Pos.y + 380, GlobalText[1002]);
	}

	RenderBitmap(IMAGE_GUILDMAKE_BTN_EXIT, m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
}

void CNewUIGuildMakeWindow::RenderGMMark()
{
	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x + 25, m_Pos.y + 66, GlobalText[182], 74, 0, RT3_SORT_LEFT);

	RenderBitmap(IMAGE_GUILDMAKE_EDITBOX, m_Pos.x + 55, m_Pos.y + 60, 110.0, 18.0, 0, 0, 0.859375, 0.5625, 1, 1, 0.0);

	m_EditBox->Render();

	RenderGoldRect(m_Pos.x + 45, m_Pos.y + 95, 130.f, 130.f);

	CreateGuildMark(MARK_EDIT);

	RenderEditGuildMark(m_Pos.x, m_Pos.y);

	g_pRenderText->SetFont(g_hFontBold);

	g_pRenderText->SetBgColor(0);

	/*if (SEASON3B::CheckMouseIn(m_Pos.x + 40, m_Pos.y + 379, 49, 16) == true)
	{
		glColor3f(1.0, 1.0, 1.0);
	}
	else
	{
		glColor3f(0.69999999, 0.69999999, 0.69999999);
	}

	RenderBitmap(BITMAP_INTERFACE_EX + 9, m_Pos.x + 40, m_Pos.y + 379, 49.0f, 16.f, 0, 0, 0.765625, 1.0, 1, 1, 0.0);
	
	glColor3f(1.0, 1.0, 1.0);

	g_pRenderText->RenderText(m_Pos.x + 55, m_Pos.y + 382, GlobalText[1306], 74, 0, RT3_SORT_LEFT);*/

	gInterfaceManager.RenderButton(BITMAP_INTERFACE_EX + 9, m_Pos.x + 40, m_Pos.y + 379, 49.0f, 16.f, 0.765625, 1.0, 1306);

	/*if (SEASON3B::CheckMouseIn(m_Pos.x + 100, m_Pos.y + 379, 49, 16) == true)
	{
		glColor3f(1.0, 1.0, 1.0);
	}
	else
	{
		glColor3f(0.69999999, 0.69999999, 0.69999999);
	}

	RenderBitmap(BITMAP_INTERFACE_EX + 9, m_Pos.x + 100, m_Pos.y + 379, 49.0f, 16.f, 0, 0, 0.765625, 1.0, 1, 1, 0.0);

	g_pRenderText->RenderText(m_Pos.x + 115, m_Pos.y + 382, GlobalText[1305], 74, 0, RT3_SORT_LEFT);

	glColor3f(1.0, 1.0, 1.0);*/

	gInterfaceManager.RenderButton(BITMAP_INTERFACE_EX + 9, m_Pos.x + 100, m_Pos.y + 379, 49.0f, 16.f, 0.765625, 1.0, 1305);
}

void CNewUIGuildMakeWindow::RenderGMResultInfo()
{
	RenderGoldRect(m_Pos.x + 72, m_Pos.y + 70, 53.f, 53.f);

	CreateGuildMark(MARK_EDIT);

	RenderBitmap(BITMAP_GUILD, m_Pos.x + 72, m_Pos.y + 74, 48, 48);

	char Text[100];

	memset(&Text, 0, sizeof(char) * 100);

	sprintf(Text, "%s : %s", GlobalText[182], GuildMark[MARK_EDIT].GuildName);

	RenderText(Text, m_Pos.x, m_Pos.y + 140, 190, 0, 0xFF49B0FF, 0x00000000, RT3_SORT_CENTER);

	g_pRenderText->SetFont(g_hFontBold);

	g_pRenderText->SetBgColor(0);

	gInterfaceManager.RenderButton(BITMAP_INTERFACE_EX + 9, m_Pos.x + 40, m_Pos.y + 379, 49.0f, 16.f, 0.765625, 1.0, 1306);

	/*RenderBitmap(BITMAP_INTERFACE_EX + 9, m_Pos.x + 40, m_Pos.y + 379, 49.0f, 16.f, 0, 0, 0.765625, 1.0, 1, 1, 0.0);

	g_pRenderText->RenderText(m_Pos.x + 55, m_Pos.y + 382, GlobalText[1306], 74, 0, RT3_SORT_LEFT);

	RenderBitmap(BITMAP_INTERFACE_EX + 9, m_Pos.x + 100, m_Pos.y + 379, 49.0f, 16.f, 0, 0, 0.765625, 1.0, 1, 1, 0.0);

	g_pRenderText->RenderText(m_Pos.x + 115, m_Pos.y + 382, GlobalText[1305], 74, 0, RT3_SORT_LEFT);*/

	gInterfaceManager.RenderButton(BITMAP_INTERFACE_EX + 9, m_Pos.x + 100, m_Pos.y + 379, 49.0f, 16.f, 0.765625, 1.0, 1305);
}

void CNewUIGuildMakeWindow::RenderFrame()
{
	RenderBitmap(IMAGE_GUILDMAKE_BACK_TOP, 450.0, 0, 190.0, 256.0, 0, 0, 0.7421875, 1.0, 1, 1, 0.0);
	RenderBitmap(IMAGE_GUILDMAKE_BACK_LOW, 450.0, 256.0, 190.0, 177.0, 0, 0, 0.7421875, 0.69140625, 1, 1, 0.0);

	char Text[100];

	memset(&Text, 0, sizeof(char) * 100);

	sprintf(Text, GlobalText[180]);

	RenderText(Text, m_Pos.x, m_Pos.y + 15, 190, 0, 0xFF49B0FF, 0x00000000, RT3_SORT_CENTER);
}

bool CNewUIGuildMakeWindow::UpdateKeyEvent()
{
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_NPCGUILDMASTER) == true)
	{
		if(SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_NPCGUILDMASTER);
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}

	return true;
}

bool CNewUIGuildMakeWindow::Update()
{
	return true;
}

bool CNewUIGuildMakeWindow::UpdateMouseEvent()
{
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_NPCGUILDMASTER) == false)
	{
		return true;
	}

	POINT ptExitBtn1 = { m_Pos.x+169, m_Pos.y+7 };

	if(SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 13, 12))
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_NPCGUILDMASTER);
		return false;
	}

	bool bResult = false;

	switch( m_GuildMakeState )
	{
	case GUILDMAKE_INFO: bResult = UpdateGMInfo(); 
		break;
	case GUILDMAKE_MARK: bResult = UpdateGMMark(); 
		break;
	case GUILDMAKE_RESULTINFO: bResult = UpdateGMResultInfo(); 
		break;
	}

	if(bResult == true)
	{
		return false;
	}
	
	if(CheckMouseIn(m_Pos.x, m_Pos.y, GUILDMAKE_WIDTH, GUILDMAKE_HEIGHT))
	{
		if(SEASON3B::IsPress(VK_RBUTTON))
		{
			MouseRButton = false;
			MouseRButtonPop = false;
			MouseRButtonPush = false;
		}

		return false;
	}

	return true;
}

bool CNewUIGuildMakeWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	RenderFrame();

	switch( m_GuildMakeState )
	{
	case GUILDMAKE_INFO: RenderGMInfo(); break;
	case GUILDMAKE_MARK: RenderGMMark(); break;
	case GUILDMAKE_RESULTINFO: RenderGMResultInfo(); break;
	}

	DisableAlphaBlend();

	return true;
}
