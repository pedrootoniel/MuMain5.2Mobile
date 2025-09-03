//////////////////////////////////////////////////////////////////////
// NewUIGuildInfoWindow.cpp: implementation of the CNewUIGuildInfoWindow class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIGuildInfoWindow.h"
#include "NewUISystem.h"
#include "NewUICommonMessageBox.h"
#include "NewUICustomMessageBox.h"
#include "DSPlaySound.h"
#include "UIGuildInfo.h"
#include "UIControls.h"
#include "UIPopup.h"
#include "ZzzInterface.h"
#include "ZzzInventory.h"
#include "ZzzInfomation.h"
#include "wsclientinline.h"
#include "CharacterManager.h"
#include "GuildManager.h"
#include "..\\InterfaceManager.h"

int	DeleteIndex = 0;
int AppointStatus = 0;
char DeleteID[100];

extern CUIPopup* g_pUIPopup;
extern MARK_t GuildMark[MAX_MARKS];

using namespace SEASON3B;

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

int SEASON3B::CNewUIGuildInfoWindow::GetGuildMemberIndex( char* szName )
{
	for( int i=0 ; i<g_nGuildMemberCount ; ++i )
	{
		if( GuildList[i].Name && !strcmp( GuildList[i].Name, szName ) )
			return i;
	}

	return -1;
}

SEASON3B::CNewUIGuildInfoWindow::CNewUIGuildInfoWindow() : m_Button(NULL)
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
	m_nCurrentTab = 1;
	m_EventState = EVENT_NONE;
	m_Loc = 0;
	m_BackUp = 0;
	m_CurrentListPos = 0;
	m_Loc_Bk = 0;
	m_Tot_Notice = 0;
	m_dwPopupID = 0;

	m_bRequestUnionList = false;
}

SEASON3B::CNewUIGuildInfoWindow::~CNewUIGuildInfoWindow() 
{ 
	Release(); 
}

bool SEASON3B::CNewUIGuildInfoWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if(NULL == pNewUIMng || NULL == g_pNewUI3DRenderMng || NULL == g_pNewItemMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_GUILDINFO, this);

	SetPos(x, y);

	LoadImages();
	
	Show(false);

	return true;
}

void SEASON3B::CNewUIGuildInfoWindow::OpenningProcess()
{
	m_nCurrentTab = 1;
	
	SendRequestGuildList();
}

void SEASON3B::CNewUIGuildInfoWindow::ClosingProcess()
{
	m_bRequestUnionList = false;
}

void SEASON3B::CNewUIGuildInfoWindow::Release()
{
	UnloadImages();

	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIGuildInfoWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool SEASON3B::CNewUIGuildInfoWindow::UpdateMouseEvent()
{
	Check_Btn();

	if(m_nCurrentTab == 0)
	{
		m_GuildNotice.DoAction();
	}
	else if(m_nCurrentTab == 1)
	{
		m_GuildMember.DoAction();
	}
	else if(m_nCurrentTab == 2)
	{
		m_UnionListBox.DoAction();
	}

	if(CheckMouseIn(m_Pos.x, m_Pos.y, GUILDINFO_WIDTH, GUILDINFO_HEIGHT))
	{
		return false;
	}

	return true;
}

bool SEASON3B::CNewUIGuildInfoWindow::Check_Btn()
{
	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24, 24) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Hide(SEASON3B::INTERFACE_GUILDINFO);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	/* */

	const int numTabs = 3;

	const int baseX = m_Pos.x + 20;
	const int baseY = m_Pos.y + 68;

	const int tabSpacing = 50;

	for (int i = 0; i < numTabs; i++)
	{
		if (SEASON3B::CheckMouseIn(baseX + (i * tabSpacing), baseY, 49, 18) == true && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;
			MouseUpdateTimeMax = 6;

			this->m_nCurrentTab = i;

			switch (this->m_nCurrentTab)
			{
			case 0:
				break;
			case 1:
				SendRequestGuildList();
				break;
			case 2:
				{
					if (m_bRequestUnionList == false && GuildMark[Hero->GuildMarkIndex].UnionName[0] != NULL)
					{
						SendRequestUnionList();

						m_bRequestUnionList = true;
					}
				}
				break;
			}

			PlayBuffer(SOUND_CLICK01);

			return true;
		}
	}

	if(m_nCurrentTab == 0)
	{
		if (SEASON3B::CheckMouseIn(m_Pos.x + 70, m_Pos.y + 370, 49, 16) == true && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;
			MouseUpdateTimeMax = 6;

			if (Hero->GuildStatus == G_MASTER)
			{
				if (!strcmp(GuildMark[Hero->GuildMarkIndex].GuildName, GuildMark[Hero->GuildMarkIndex].UnionName))
				{
					SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CGuildOutPerson));
				}
				else
				{
					DeleteIndex = GetGuildMemberIndex(Hero->ID);
					SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CGuildBreakMsgBoxLayout));
				}
			}
			else
			{
				DeleteIndex = GetGuildMemberIndex(Hero->ID);
				SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CGuildBreakPasswordMsgBoxLayout));
			}

			PlayBuffer(SOUND_CLICK01);

			return true;
		}
	}
	else if(m_nCurrentTab == 1)
	{
		if (SEASON3B::CheckMouseIn(m_Pos.x + 20, m_Pos.y + 365, 49, 16) == true && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;
			MouseUpdateTimeMax = 6;

			if (GUILDLIST_TEXT* pText = m_GuildMember.GetSelectedText())
			{
				if (Hero->GuildStatus == G_MASTER)
				{
					if (pText->m_GuildStatus != G_MASTER)
					{
						if (GUILDLIST_TEXT* pText = m_GuildMember.GetSelectedText())
						{
							AppointStatus = (GUILD_STATUS)pText->m_GuildStatus;
							DeleteIndex = GetGuildMemberIndex(pText->m_szID);
							SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(CGuild_ToPerson_PositionLayout));
						}
					}
				}
			}

			PlayBuffer(SOUND_CLICK01);

			return true;		
		}
		else if (SEASON3B::CheckMouseIn(m_Pos.x + 70, m_Pos.y + 365, 49, 16) == true && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;
			MouseUpdateTimeMax = 6;

			if (GUILDLIST_TEXT* pText = m_GuildMember.GetSelectedText())
			{
				if (pText->m_GuildStatus == G_SUB_MASTER || pText->m_GuildStatus == G_BATTLE_MASTER)
				{
					if (GUILDLIST_TEXT* pText = m_GuildMember.GetSelectedText())
					{
						AppointStatus = (GUILD_STATUS)pText->m_GuildStatus;
						DeleteIndex = GetGuildMemberIndex(pText->m_szID);
						CNewUICommonMessageBox* pMsgBox = NULL;
						SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(CGuildPerson_Cancel_Position_MsgBoxLayout), &pMsgBox);
						if (pMsgBox != NULL)
						{
							unicode::t_char strText[256];
							unicode::_sprintf(strText, GlobalText[1367], pText->m_szID);
							pMsgBox->AddMsg(strText);
							pMsgBox->AddMsg(GlobalText[1368]);
						}
					}
				}
			}

			PlayBuffer(SOUND_CLICK01);

			return true;
		}
		else if (SEASON3B::CheckMouseIn(m_Pos.x + 120, m_Pos.y + 365, 49, 16) == true && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;
			MouseUpdateTimeMax = 6;

			if (Hero->GuildStatus == G_MASTER)
			{
				if (GUILDLIST_TEXT* pText = m_GuildMember.GetSelectedText())
				{
					if (pText->m_GuildStatus != G_MASTER)
					{
						if (GUILDLIST_TEXT* pText = m_GuildMember.GetSelectedText())
						{
							DeleteIndex = GetGuildMemberIndex(pText->m_szID);
							SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CGuildPerson_Get_Out));
						}
					}
				}
			}

			PlayBuffer(SOUND_CLICK01);

			return true;
		}
	}
	else if(m_nCurrentTab == 2)
	{
		/*if(m_Button[BUTTON_UNION_CREATE].UpdateMouseEvent())
		{
			if( Hero->GuildStatus == G_MASTER )
			{
				if( UNIONGUILD_TEXT* pText = m_UnionListBox.GetSelectedText() )
				{
					if( strcmp( pText->szName, GuildMark[Hero->GuildMarkIndex].GuildName ) )
					{
						strcpy( DeleteID, pText->szName );
						SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(CUnionGuild_Break_MsgBoxLayout));
					}
				}
			}
		}
		else if(m_Button[BUTTON_UNION_OUT].UpdateMouseEvent())
		{
			if( Hero->GuildStatus == G_MASTER )
			{
				if( !strcmp( GuildMark[Hero->GuildMarkIndex].GuildName, GuildMark[Hero->GuildMarkIndex].UnionName ) )
				{
					SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(CUnionGuild_Out_MsgBoxLayout));
				}
				else
				{
					SendRequestGuildRelationShip( 0x01, 0x02, HIBYTE(Hero->Key), LOBYTE(Hero->Key) );
				}
			}
		}*/
	}
	return true;
}

bool SEASON3B::CNewUIGuildInfoWindow::UpdateKeyEvent()
{
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GUILDINFO) == true)
	{
		if(SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_GUILDINFO);
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}

	return true;
}
bool SEASON3B::CNewUIGuildInfoWindow::Update()
{
	return true;
}

bool SEASON3B::CNewUIGuildInfoWindow::Render()
{
	EnableAlphaTest();

	glColor4f(1.f, 1.f, 1.f, 1.f);

	RenderFrame();

	if( Hero->GuildStatus == G_NONE )
	{
		RenderNoneGuild();
	}
	else
	{
		RenderTabButton();

		switch(m_nCurrentTab)
		{
		case 0:
			Render_Guild_History();
			break;
		case 1:
			Render_Guild_Enum();
			break;
		case 2:
			Render_Guild_Info();
			break;
		}

		Render_Text();

		/* */

		if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0) == true)
		{
			RenderTipText(m_Pos.x + 20, m_Pos.y + 380, GlobalText[1002]);
		}

		RenderBitmap(IMAGE_GUILDINFO_EXIT_BTN, m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
	}

	DisableAlphaBlend();

	return true;
}

void SEASON3B::CNewUIGuildInfoWindow::RenderFrame()
{
	RenderBitmap(IMAGE_GUILDINFO_TOP, m_Pos.x, m_Pos.y, 190.0, 256.0, 0, 0, 0.7421875, 1.0, 1, 1, 0.0);
	RenderBitmap(IMAGE_GUILDINFO_LOW, m_Pos.x, m_Pos.y + 256.0, 190.0, 177.0, 0, 0, 0.7421875, 0.69140625, 1, 1, 0.0);
}

void SEASON3B::CNewUIGuildInfoWindow::RenderNoneGuild()
{
	POINT ptOrigin = { m_Pos.x+15, m_Pos.y+38 };
	ptOrigin.x += 10;
	ptOrigin.y += 8;

	unicode::t_char Text[128];
	memset(&Text, 0, sizeof(unicode::t_char)*128);
	sprintf(Text, GlobalText[180]);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y+15, Text, 190, 0, RT3_SORT_CENTER );

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->RenderText(ptOrigin.x, ptOrigin.y, GlobalText[185]);
	ptOrigin.y += 15;
	g_pRenderText->RenderText(ptOrigin.x, ptOrigin.y, GlobalText[186]);
	ptOrigin.y += 15;
	g_pRenderText->RenderText(ptOrigin.x, ptOrigin.y, GlobalText[187]);
	
	m_BtnExit.Render();	
}

void SEASON3B::CNewUIGuildInfoWindow::RenderTabButton()
{
	/*RenderImage(IMAGE_GUILDINFO_TAB_LIST,m_Pos.x+12,m_Pos.y+68,166.f,22.f);
	int Tab_Pos = m_nCurrentTab * 55;
	RenderImage(IMAGE_GUILDINFO_TAB_POINT,m_Pos.x+12+Tab_Pos,m_Pos.y+68,56.f,22.f);*/
}

void SEASON3B::CNewUIGuildInfoWindow::Render_Text()
{
	char Text[300];

	POINT ptOrigin;

	sprintf(Text, GlobalText[180]);

	RenderText(Text, m_Pos.x, m_Pos.y + 12, 190, 0, 0xFFFFFFFF, 0x00000000, RT3_SORT_CENTER);

	ptOrigin.x = m_Pos.x + 35;
	ptOrigin.y = m_Pos.y + 48;

	g_pRenderText->SetFont(g_hFontBold);

	g_pRenderText->SetTextColor(200, 255, 100, 255);

	sprintf(Text, "%s ( Score: %d )", GuildMark[Hero->GuildMarkIndex].GuildName, GuildTotalScore);

	g_pRenderText->RenderText(ptOrigin.x, ptOrigin.y, Text, 120, 0, RT3_SORT_CENTER);

	/* */

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	/* */

	const int numTabs = 3;

	const int baseX = m_Pos.x + 20;
	const int baseY = m_Pos.y + 68;

	const int tabSpacing = 50;

	int textIndices[numTabs] = { 180, 1330, 1352 };

	for (int i = 0; i < numTabs; i++) 
	{
		if (SEASON3B::CheckMouseIn(baseX + (i * tabSpacing), baseY, 49, 18) || this->m_nCurrentTab == i) 
		{
			glColor3f(1.0, 1.0, 1.0);
		}
		else 
		{
			glColor3f(0.7, 0.7, 0.7);
		}

		RenderBitmap((BITMAP_INTERFACE_EX + 9), float(baseX + (i * tabSpacing)), float(baseY), float(49), float(18), 0, 0, 0.765625f, 1.0, 1, 1, 0.0);

		g_pRenderText->RenderText(baseX + (i * tabSpacing) - 12, m_Pos.y + 73, GlobalText[textIndices[i]], 74, 0, RT3_SORT_CENTER);

		glColor3f(1.0, 1.0, 1.0);
	}

	/* */

	if (m_nCurrentTab == 0)
	{
		int MsgGuildButtonOutId = -1;

		if (Hero->GuildStatus == G_MASTER)
		{
			MsgGuildButtonOutId = 188;
		}
		else
		{
			MsgGuildButtonOutId = 189;
		}

		if (SEASON3B::CheckMouseIn(m_Pos.x + 70, m_Pos.y + 370, 49, 16) || this->m_nCurrentTab == 2)
		{
			glColor3f(1.0, 1.0, 1.0);
		}
		else
		{
			glColor3f(0.7, 0.7, 0.7);
		}

		RenderBitmap((BITMAP_INTERFACE_EX + 9), float(m_Pos.x + 70), float(m_Pos.y + 370), float(49), float(16), 0, 0, 0.765625f, 1.0, 1, 1, 0.0);

		g_pRenderText->RenderText(m_Pos.x + 80, m_Pos.y + 373, GlobalText[MsgGuildButtonOutId], 74, 0, RT3_SORT_LEFT);

		glColor3f(1.0, 1.0, 1.0);

		/* */

		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->SetTextColor(255, 255, 255, 255);
		g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 253, GlobalText[1323], 74, 0, RT3_SORT_LEFT);

		m_GuildNotice.SetSize(140, 80);
		m_GuildNotice.SetPosition(m_Pos.x + 25, m_Pos.y + 264 + m_GuildNotice.GetHeight());
		m_GuildNotice.Render();

		gInterfaceManager.RenderBoxGoldRect(m_Pos.x + 24, m_Pos.y + 250, 140.f, 93.f);

		gInterfaceManager.RenderBoxGoldRect(m_Pos.x + 24, m_Pos.y + 161, 140.f, 60.f);

		int Nm_Loc = m_Pos.y + 169;
		sprintf(Text, "%s :", GlobalText[1332]);
		RenderText(Text, m_Pos.x + 30, Nm_Loc, 40, 0, 0xFFFFFFFF, 0x00000000, RT3_SORT_LEFT);
		Nm_Loc += 13;

		sprintf(Text, GlobalText[1256], GuildTotalScore);
		RenderText(Text, m_Pos.x + 30, Nm_Loc, 80, 0, 0xFFFFFFFF, 0x00000000, RT3_SORT_LEFT);

		Nm_Loc += 13;
		if (Hero->GuildStatus == G_MASTER)
		{
			int Class = gCharacterManager.GetBaseClass(CharacterAttribute->Class);
			if (Class == CLASS_DARK_LORD)
			{
				int nCount = CharacterAttribute->Level / 10 + CharacterAttribute->Charisma / 10;
				if (nCount > 80)
					nCount = 80;

				sprintf(Text, GlobalText[1362], g_nGuildMemberCount, nCount);
			}
			else
			{
				sprintf(Text, GlobalText[1362], g_nGuildMemberCount, CharacterAttribute->Level / 10);
			}
		}
		else
		{
			sprintf(Text, GlobalText[1310], g_nGuildMemberCount);
		}

		RenderText(Text, m_Pos.x + 30, Nm_Loc, 80, 0, 0xFFFFFFFF, 0x00000000, RT3_SORT_LEFT);

		Nm_Loc += 13;
		sprintf(Text, "%s : %s", GlobalText[1321], m_RivalGuildName[0] ? m_RivalGuildName : GlobalText[1361]);
		RenderText(Text, m_Pos.x + 30, Nm_Loc, 0, 0, 0xFFFFFFFF, 0x00000000, RT3_SORT_LEFT);
	}
	else if (m_nCurrentTab == 1)
	{
		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->SetBgColor(0);
		g_pRenderText->SetTextColor(255, 255, 255, 255);

		g_pRenderText->RenderText(m_Pos.x + 24, m_Pos.y + 112, GlobalText[1389], 40, 0, RT3_SORT_LEFT);
		g_pRenderText->RenderText(m_Pos.x + 89, m_Pos.y + 112, GlobalText[1307], 40, 0, RT3_SORT_LEFT);
		g_pRenderText->RenderText(m_Pos.x + 126, m_Pos.y + 112, GlobalText[1022], 40, 0, RT3_SORT_LEFT);		

		m_GuildMember.SetSize(157, 235);
		m_GuildMember.SetPosition(m_Pos.x + 13, m_Pos.y + 123 + m_GuildMember.GetHeight());
		m_GuildMember.Render();
	}
	else if (m_nCurrentTab == 2)
	{
		glColor4f(1.f, 1.f, 1.f, 1.f);

		if (GuildMark[Hero->GuildMarkIndex].UnionName[0] != NULL)
		{
			RenderText((char*)GlobalText[182], m_Pos.x + 34, m_Pos.y + 115, 40, 0, 0xFFFFFFFF, 0x00000000, RT3_SORT_LEFT);
			RenderText((char*)GlobalText[1330], m_Pos.x + 140, m_Pos.y + 115, 40, 0, 0xFFFFFFFF, 0x00000000, RT3_SORT_LEFT);
		}
	}
}

void SEASON3B::CNewUIGuildInfoWindow::Render_Guild_History()
{
	CreateGuildMark(Hero->GuildMarkIndex);

	RenderBitmap(BITMAP_GUILD, m_Pos.x + 74, m_Pos.y + 106, 39, 39);

	gInterfaceManager.RenderBoxGoldRect(m_Pos.x + 65, m_Pos.y + 100, 53.f, 53.f);
}

void SEASON3B::CNewUIGuildInfoWindow::Render_Guild_Info()
{
	POINT ptOrigin = { m_Pos.x+15, m_Pos.y+98 };

	if( GuildMark[Hero->GuildMarkIndex].UnionName[0] == NULL )
	{
		ptOrigin.x += 10;
		ptOrigin.y += 8;

		g_pRenderText->SetTextColor(255, 255, 255, 255);
		g_pRenderText->SetBgColor(0);
		g_pRenderText->RenderText(ptOrigin.x, ptOrigin.y, GlobalText[1257]);
		ptOrigin.y += 15;
		g_pRenderText->RenderText(ptOrigin.x, ptOrigin.y, GlobalText[1258]);
		ptOrigin.y += 15;
		g_pRenderText->RenderText(ptOrigin.x, ptOrigin.y, GlobalText[1259]);
		ptOrigin.y += 15;
		g_pRenderText->RenderText(ptOrigin.x, ptOrigin.y, GlobalText[1260]);
		ptOrigin.y += 15;
		g_pRenderText->RenderText(ptOrigin.x, ptOrigin.y, GlobalText[1261]);

		ptOrigin.y += 25;
		g_pRenderText->RenderText(ptOrigin.x, ptOrigin.y, GlobalText[1262]);
		ptOrigin.y += 15;
		g_pRenderText->RenderText(ptOrigin.x, ptOrigin.y, GlobalText[1263]);
		ptOrigin.y += 15;
		g_pRenderText->RenderText(ptOrigin.x, ptOrigin.y, GlobalText[1264]);
		ptOrigin.y += 20;
		g_pRenderText->RenderText(ptOrigin.x, ptOrigin.y, GlobalText[1265]);
		ptOrigin.y += 15;
		g_pRenderText->RenderText(ptOrigin.x, ptOrigin.y, GlobalText[1266]);
		ptOrigin.y += 15;
		g_pRenderText->RenderText(ptOrigin.x, ptOrigin.y, GlobalText[1267]);

		g_pRenderText->SetTextColor(0xFFFFFFFF);
	}
	else
	{
		glColor4ub(21, 21, 21, 150);
		RenderColor(m_Pos.x+12, m_Pos.y+12+113, 165, 85);

		glColor4ub(59, 59, 59, 150);
		RenderColor(m_Pos.x+12, m_Pos.y+12+98, 165, 15);
		EndRenderColor();

		EndRenderColor();

		m_UnionListBox.SetSize(160, 80);
		m_UnionListBox.SetPosition(m_Pos.x + 15, m_Pos.y + 210);
		m_UnionListBox.Render();

		EnableAlphaTest();
	}
}

void SEASON3B::CNewUIGuildInfoWindow::Render_Guild_Enum()
{
	if(gGuildManager.IsGuildMaster())
	{
		const int numTabs = 3;

		const int baseX = m_Pos.x + 20;
		const int baseY = m_Pos.y + 365;

		const int tabSpacing = 50;

		int textIndices[numTabs] = { 1307, 1308, 1309 };

		for (int i = 0; i < numTabs; i++)
		{
			if (SEASON3B::CheckMouseIn(baseX + (i * tabSpacing), baseY, 49, 16))
			{
				glColor3f(1.0, 1.0, 1.0);
			}
			else
			{
				glColor3f(0.7, 0.7, 0.7);
			}

			RenderBitmap((BITMAP_INTERFACE_EX + 9), float(baseX + (i * tabSpacing)), float(baseY), float(49), float(16), 0, 0, 0.765625f, 1.0, 1, 1, 0.0);

			g_pRenderText->SetFont(g_hFontBold);
			g_pRenderText->SetBgColor(0);
			g_pRenderText->SetTextColor(255, 255, 255, 255);
			g_pRenderText->RenderText(baseX + (i * tabSpacing) - 12, m_Pos.y + 368, GlobalText[textIndices[i]], 74, 0, RT3_SORT_CENTER);

			glColor3f(1.0, 1.0, 1.0);
		}
	}
}

float SEASON3B::CNewUIGuildInfoWindow::GetLayerDepth()
{ 
	return 4.5f; 
}

void SEASON3B::CNewUIGuildInfoWindow::LoadImages()
{
	LoadBitmap("Common\\Interface\\ui_item_backtop.jpg", IMAGE_GUILDINFO_TOP);
	LoadBitmap("Common\\Interface\\ui_item_backlow.jpg", IMAGE_GUILDINFO_LOW);

	LoadBitmap("Interface\\newui_guild_tab01.tga", IMAGE_GUILDINFO_TAB_LIST, GL_LINEAR);
	LoadBitmap("Interface\\newui_guild_tab02.tga", IMAGE_GUILDINFO_TAB_POINT, GL_LINEAR);
	LoadBitmap("Interface\\newui_guild_tab03.tga", IMAGE_GUILDINFO_TAB_HEAD, GL_LINEAR);
	
	LoadBitmap("Common\\Interface\\ui_menu_exit.jpg", IMAGE_GUILDINFO_EXIT_BTN);

	LoadBitmap("Interface\\newui_btn_empty_small.tga", IMAGE_GUILDINFO_BUTTON, GL_LINEAR);

	LoadBitmap("Common\\Interface\\ui_msgbox_line.jpg",		IMAGE_GUILDINFO_SCROLL_TOP);
	
	LoadBitmap("Interface\\newui_scrollbar_m.tga",		IMAGE_GUILDINFO_SCROLL_MIDDLE);
	LoadBitmap("Interface\\newui_scrollbar_down.tga",	IMAGE_GUILDINFO_SCROLL_BOTTOM);
	LoadBitmap("Interface\\newui_scroll_on.tga",		IMAGE_GUILDINFO_SCROLLBAR_ON, GL_LINEAR);
	LoadBitmap("Interface\\newui_scroll_off.tga",		IMAGE_GUILDINFO_SCROLLBAR_OFF, GL_LINEAR);
	LoadBitmap("Interface\\newui_scrollbar_stretch.jpg",IMAGE_GUILDINFO_DRAG_BTN, GL_LINEAR);
}

void SEASON3B::CNewUIGuildInfoWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_GUILDINFO_TOP);
	DeleteBitmap(IMAGE_GUILDINFO_LOW);

	DeleteBitmap(IMAGE_GUILDINFO_TAB_LIST);
	DeleteBitmap(IMAGE_GUILDINFO_TAB_POINT);
	DeleteBitmap(IMAGE_GUILDINFO_TAB_HEAD);
	DeleteBitmap(IMAGE_GUILDINFO_BUTTON);
	
	DeleteBitmap(IMAGE_GUILDINFO_EXIT_BTN);

	DeleteBitmap(IMAGE_GUILDINFO_SCROLL_TOP);
	DeleteBitmap(IMAGE_GUILDINFO_SCROLL_MIDDLE);
	DeleteBitmap(IMAGE_GUILDINFO_SCROLL_BOTTOM);
	DeleteBitmap(IMAGE_GUILDINFO_SCROLLBAR_ON);
	DeleteBitmap(IMAGE_GUILDINFO_SCROLLBAR_OFF);
	DeleteBitmap(IMAGE_GUILDINFO_DRAG_BTN);
}

void SEASON3B::CNewUIGuildInfoWindow::AddGuildNotice(char* szText)
{
	char szTemp[5][MAX_TEXT_LENGTH + 1] = { {0}, {0}, {0}, {0}, {0} } ;
	CutText3( szText, szTemp[0], 110, 5, MAX_TEXT_LENGTH + 1);

	for( int i=0 ; i<5 ; ++i )
	{
		if( szTemp[i][0] )
		{
			m_GuildNotice.AddText( szTemp[i] );
			m_Tot_Notice++;
		}
	}
	m_GuildNotice.Scrolling( m_GuildNotice.GetLineNum()-m_GuildNotice.GetBoxSize() );
}

void SEASON3B::CNewUIGuildInfoWindow::AddGuildMember(GUILD_LIST_t* pInfo)
{
	m_GuildMember.AddText( pInfo->Name, pInfo->Number, pInfo->Server, pInfo->GuildStatus );
	m_GuildMember.Scrolling( -m_GuildMember.GetBoxSize() );
}

void SEASON3B::CNewUIGuildInfoWindow::GuildClear()
{
	m_GuildMember.Clear();
}

void SEASON3B::CNewUIGuildInfoWindow::UnionGuildClear()
{
	m_UnionListBox.Clear();
}

void SEASON3B::CNewUIGuildInfoWindow::NoticeClear()
{
	m_GuildNotice.Clear();
}

void SEASON3B::CNewUIGuildInfoWindow::SetRivalGuildName(char* szName)
{
	memcpy(m_RivalGuildName, szName, sizeof(char)*MAX_GUILDNAME);
	m_RivalGuildName[MAX_GUILDNAME] = NULL;
}

void SEASON3B::CNewUIGuildInfoWindow::AddUnionList(BYTE* pGuildMark, char* szGuildName, int nMemberCount)
{
	m_UnionListBox.AddText( pGuildMark, szGuildName, nMemberCount );
	m_bRequestUnionList = false;
}

int SEASON3B::CNewUIGuildInfoWindow::GetUnionCount()
{
	return m_UnionListBox.GetTextCount();
}

void SEASON3B::CNewUIGuildInfoWindow::ReceiveGuildRelationShip( BYTE byRelationShipType, BYTE byRequestType, 
							   BYTE  byTargetUserIndexH, BYTE byTargetUserIndexL)
{
	if( !g_MessageBox->IsEmpty() )
	{
		SendRequestGuildRelationShipResult( byRelationShipType, byRequestType, 0x00, byTargetUserIndexH, byTargetUserIndexL );
	}
	else
	{
		m_MessageInfo.s_byRelationShipType = byRelationShipType;
		m_MessageInfo.s_byRelationShipRequestType = byRequestType;
		m_MessageInfo.s_byTargetUserIndexH = byTargetUserIndexH;
		m_MessageInfo.s_byTargetUserIndexL = byTargetUserIndexL;

		int nCharKey = MAKEWORD(m_MessageInfo.s_byTargetUserIndexL, m_MessageInfo.s_byTargetUserIndexH);
		int nIndex = FindCharacterIndex(nCharKey);
		if( nIndex < 0 || nIndex >= MAX_CHARACTERS_CLIENT )
			return;
		CHARACTER* pPlayer = &CharactersClient[nIndex];

		unicode::t_char szText[3][64];
		ZeroMemory(szText, sizeof(szText));

		if( m_MessageInfo.s_byRelationShipType == 0x01 )			// Union
		{
			if( m_MessageInfo.s_byRelationShipRequestType == 0x01 )	// Join
			{
				sprintf( szText[0], GlobalText[1280], pPlayer->ID );
				sprintf( szText[1], GlobalText[1281] );	
				sprintf( szText[2], GlobalText[1283] );	
			}
			else										// Break Off
			{
				sprintf( szText[0], GlobalText[1280], pPlayer->ID );
				sprintf( szText[1], GlobalText[1282] );
				sprintf( szText[2], GlobalText[1283] );
			}
		}
		else if( m_MessageInfo.s_byRelationShipType == 0x02 )		// Rival
		{
			if( m_MessageInfo.s_byRelationShipRequestType == 0x01 )	// Join
			{
				sprintf( szText[0], GlobalText[1284], pPlayer->ID );
				sprintf( szText[1], GlobalText[1286] );
				sprintf( szText[2], GlobalText[1283] );
			}
			else
			{
				sprintf( szText[0], GlobalText[1284], pPlayer->ID );
				sprintf( szText[1], GlobalText[1285] );
				sprintf( szText[2], GlobalText[1283] );
			}
		}

		SEASON3B::CNewUICommonMessageBox* pMsgBox = NULL;
		SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CGuildRelationShipMsgBoxLayout), &pMsgBox);
		if(pMsgBox)
		{
			pMsgBox->AddMsg(szText[0]);
			pMsgBox->AddMsg(szText[1]);
			pMsgBox->AddMsg(szText[2]);
		}
	}
}
