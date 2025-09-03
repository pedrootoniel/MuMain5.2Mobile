// NewUICommandWindow.cpp: implementation of the CNewUICommandWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NewUICommandWindow.h"
#include "NewUISystem.h"
#include "wsclientinline.h"
#include "ZzzInterface.h"
#include "PersonalShopTitleImp.h"
#include "UIGuildInfo.h"
#include "ZzzAI.h"
#include "w_CursedTemple.h"
#include "MapManager.h"

using namespace SEASON3B;

SEASON3B::CNewUICommandWindow::CNewUICommandWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
	m_iCurSelectCommand = COMMAND_NONE;
	m_iCurMouseCursor = CURSOR_NORMAL;
	m_bSelectedChar = false;
	m_bCanCommand = false;
}

SEASON3B::CNewUICommandWindow::~CNewUICommandWindow()
{
	Release();
}

bool SEASON3B::CNewUICommandWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_COMMAND, this);

	SetPos(x, y);

	LoadImages();

	Show(false);

	return true;
}

void SEASON3B::CNewUICommandWindow::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUICommandWindow::OpenningProcess()
{
	m_iCurSelectCommand = COMMAND_NONE;
	m_iCurMouseCursor = CURSOR_NORMAL;
}

void SEASON3B::CNewUICommandWindow::ClosingProcess()
{
	m_iCurSelectCommand = COMMAND_NONE;
	m_iCurMouseCursor = CURSOR_NORMAL;
}

bool SEASON3B::CNewUICommandWindow::BtnProcess()
{
	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24, 24) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Hide(SEASON3B::INTERFACE_COMMAND);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	for (int i = COMMAND_TRADE; i < COMMAND_END; i++)
	{
		if (SEASON3B::CheckMouseIn(m_Pos.x + (COMMAND_WINDOW_WIDTH / 2 - 100 / 2), (m_Pos.y + 50) + (i * 28), 100, 25) == true && MouseLButtonPush)
		{
			if (i == COMMAND_PCBANG && Hero->SafeZone)
			{
				return CommandPCBangPoint();
			}

			if (g_CursedTemple->GetInterfaceState(static_cast<int>(SEASON3B::INTERFACE_COMMAND), i))
			{
				m_iCurSelectCommand = i;
			}

			PlayBuffer(SOUND_CLICK01);
		}
	}

	return false;
}

bool SEASON3B::CNewUICommandWindow::UpdateMouseEvent()
{
	if (true == BtnProcess())
	{
		return false;
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, COMMAND_WINDOW_WIDTH, COMMAND_WINDOW_HEIGHT))
	{
		SetMouseCursor(CURSOR_NORMAL);
		return false;
	}
	else
	{
		if (m_iCurSelectCommand != COMMAND_NONE)
		{
			SetMouseCursor(CURSOR_IDSELECT);
		}
	}

	return true;
}

bool SEASON3B::CNewUICommandWindow::UpdateKeyEvent()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_COMMAND) == true)
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_COMMAND);
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}
	return true;
}

bool SEASON3B::CNewUICommandWindow::Update()
{
	if (IsVisible())
	{
		SelectCommand();
		RunCommand();
	}

	return true;
}

void CreateCheckBox(int X, int Y, int Check)
{
	DisableAlphaBlend();

	glColor4ub(146, 134, 121, 255);

	RenderColor(X, Y, 9.0, 1.0);

	RenderColor(X, (Y + 8), 9.0, 1.0);

	RenderColor(X, Y, 1.0, 9.0);

	RenderColor((X + 8), Y, 1.0, 9.0);

	EndRenderColor();

	EnableAlphaTest(1);

	if (Check == 1)
	{
		RenderBitmap((BITMAP_INTERFACE_EX + 13), (X + 2), (Y + 2), 5.0, 5.0, 0, 0, 0.625, 0.625, 1, 1, 0.0);
	}
}

bool SEASON3B::CNewUICommandWindow::Render()
{
	EnableAlphaTest();

	char szBuffer[256] = { 0 };

	glColor4f(1.f, 1.f, 1.f, 1.f);

	RenderBaseWindow();

	int iGlobalText[] = { 943, 1124, 944, 945, 946, 1352, 1321, 1322, 947, 948, 949, 2325 };

	for (int i = COMMAND_TRADE; i < COMMAND_END; i++)
	{
		RenderBitmap(IMAGE_COMMAND_BTN, m_Pos.x + (COMMAND_WINDOW_WIDTH / 2 - 100 / 2), (m_Pos.y + 50) + (i * 28), 100.0, 25.0, 0, 0, ((double)213 / 256.0), 1.0, 1, 1, 0.0);
	
		if (SEASON3B::CheckMouseIn(m_Pos.x + (COMMAND_WINDOW_WIDTH / 2 - 100 / 2), (m_Pos.y + 50) + (i * 28), 100, 25) == true || m_iCurSelectCommand == i)
		{
			g_pRenderText->SetFont(g_hFontBold);
		}
		else
		{
			g_pRenderText->SetFont(g_hFont);
		}

		g_pRenderText->SetBgColor(0);

		if (i == COMMAND_PCBANG)
		{
			if (Hero->SafeZone)
			{
				g_pRenderText->SetTextColor(255, 128, 255, 255);
			}
			else
			{
				g_pRenderText->SetTextColor(128, 64, 128, 255);			
			}
		}
		else
		{
			g_pRenderText->SetTextColor(255, 255, 255, 255);
		}

		if (m_iCurSelectCommand == i)
		{
			if (i != COMMAND_PCBANG)
			{
				CreateCheckBox(m_Pos.x + (COMMAND_WINDOW_WIDTH / 2 - 100 / 2) - 20, (m_Pos.y + 50) + (i * 28) + 8, 1);
			}

			glColor3f(0.69999999, 0.69999999, 0.69999999);
			
			RenderBitmap(IMAGE_COMMAND_BTN, m_Pos.x + (COMMAND_WINDOW_WIDTH / 2 - 100 / 2), (m_Pos.y + 50) + (i * 28), 100.0, 25.0, 0, 0, ((double)213 / 256.0), 1.0, 1, 1, 0.0);
			
			glColor3f(1.0, 1.0, 1.0);
		}
		else
		{
			if (i != COMMAND_PCBANG)
			{
				CreateCheckBox(m_Pos.x + (COMMAND_WINDOW_WIDTH / 2 - 100 / 2) - 20, (m_Pos.y + 50) + (i * 28) + 8, 0);
			}

			if (i == COMMAND_PCBANG && !Hero->SafeZone)
			{
				glColor3f(0.69999999, 0.69999999, 0.69999999);

				RenderBitmap(IMAGE_COMMAND_BTN, m_Pos.x + (COMMAND_WINDOW_WIDTH / 2 - 100 / 2), (m_Pos.y + 50) + (i * 28), 100.0, 25.0, 0, 0, ((double)213 / 256.0), 1.0, 1, 1, 0.0);

				glColor3f(1.0, 1.0, 1.0);
			}
		}

		g_pRenderText->RenderText(m_Pos.x + (COMMAND_WINDOW_WIDTH / 2 - 100 / 2) + 15, (m_Pos.y + 50) + (i * 28) + 8, GlobalText[iGlobalText[i]], 72, 0, RT3_SORT_CENTER);
	}

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(m_Pos.x + 60, m_Pos.y + 12, GlobalText[938], 72, 0, RT3_SORT_CENTER);

	if ((m_iCurMouseCursor == CURSOR_IDSELECT) && (m_bSelectedChar == true))
	{
		CHARACTER* c = &CharactersClient[SelectedCharacter];

		if (c != NULL && c->Object.Kind == KIND_PLAYER && c != Hero && (c->Object.Type == MODEL_PLAYER || c->Change))
		{
			int Width = 128; int Height = 32; int x = (float)MouseX + 5; int y = (float)MouseY + 5;

			RenderBitmap(BITMAP_COMMAND_WINDOW_BEGIN, x, y, Width, Height);

			g_pRenderText->SetFont(g_hFontBig);

			if (m_bCanCommand)
			{
				g_pRenderText->SetTextColor(255, 255, 255, 255);
			}
			else
			{
				g_pRenderText->SetTextColor(255, 0, 0, 255);
			}

			g_pRenderText->SetBgColor(20, 20, 20, 0);

			g_pRenderText->RenderText(MouseX + 5 + 64, MouseY + 12, c->ID, 0, 0, RT3_WRITE_CENTER);

			g_pRenderText->SetFont(g_hFont);
		}
	}

	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0) == true)
	{
		sprintf(szBuffer, "Close");

		RenderTipText(m_Pos.x + 25, m_Pos.y + 380, szBuffer);
	}

	RenderBitmap(IMAGE_COMMAND_BASE_WINDOW_BTN_EXIT, m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);

	DisableAlphaBlend();

	return true;
}

void SEASON3B::CNewUICommandWindow::RenderBaseWindow()
{
	RenderBitmap(IMAGE_COMMAND_BASE_WINDOW_TOP, m_Pos.x, m_Pos.y, 190.0, 256.0, 0, 0, 0.7421875, 1.0, 1, 1, 0.0);
	RenderBitmap(IMAGE_COMMAND_BASE_WINDOW_LOW, m_Pos.x, m_Pos.y + 256.0, 190.0, 177.0, 0, 0, 0.7421875, 0.69140625, 1, 1, 0.0);
}

void SEASON3B::CNewUICommandWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

float SEASON3B::CNewUICommandWindow::GetLayerDepth()
{
	return 4.6f;
}

void SEASON3B::CNewUICommandWindow::RunCommand()
{
	if (MouseLButtonPush && m_iCurMouseCursor != CURSOR_IDSELECT)
	{
		SetMouseCursor(CURSOR_PUSH);
	}

	if (m_iCurSelectCommand == COMMAND_NONE)
	{
		return;
	}

	int Selectindex = -1;

	CHARACTER* pSelectedCha = NULL;

	int distX, distY;

	m_bSelectedChar = false;

	m_bCanCommand = false;

	if (SelectedCharacter >= 0)
	{
		pSelectedCha = &CharactersClient[SelectedCharacter];

		m_bSelectedChar = true;

		if (pSelectedCha != NULL)
		{
			distX = abs((pSelectedCha->PositionX) - (Hero->PositionX));
			distY = abs((pSelectedCha->PositionY) - (Hero->PositionY));

			if (pSelectedCha->Object.Kind == KIND_PLAYER && pSelectedCha != Hero && (pSelectedCha->Object.Type == MODEL_PLAYER || pSelectedCha->Change) && (distX <= MAX_DISTANCE_TILE && distY <= MAX_DISTANCE_TILE))
			{
				if ((pSelectedCha->Object.SubType != MODEL_XMAS_EVENT_CHA_DEER) && (pSelectedCha->Object.SubType != MODEL_XMAS_EVENT_CHA_SNOWMAN) && (pSelectedCha->Object.SubType != MODEL_XMAS_EVENT_CHA_SSANTA))
				{
					Selectindex = SelectedCharacter;
					m_bCanCommand = true;
				}
			}
		}
	}

	if (MouseRButtonPush)
	{
		MouseRButtonPush = false;
		MouseRButton = false;

		SetMouseCursor(CURSOR_NORMAL);

		if (Selectindex >= 0)
		{
			switch (m_iCurSelectCommand)
			{

			case COMMAND_TRADE:
			{
				CommandTrade(pSelectedCha);
			}break;

			case COMMAND_PURCHASE:
			{
				CommandPurchase(pSelectedCha);
			}break;

			case COMMAND_PARTY:
			{
				CommandParty(pSelectedCha->Key);
			}break;

			case COMMAND_WHISPER:
			{
				CommandWhisper(pSelectedCha);
			}break;

			case COMMAND_GUILD:
			{
				CommandGuild(pSelectedCha);
			}break;

			case COMMAND_GUILDUNION:
			{
				CommandGuildUnion(pSelectedCha);
			}break;

			case COMMAND_RIVAL:
			{
				CommandGuildRival(pSelectedCha);
			}break;

			case COMMAND_RIVALOFF:
			{
				CommandCancelGuildRival(pSelectedCha);
			}break;

			case COMMAND_ADD_FRIEND:
			{
				CommandAddFriend(pSelectedCha);
			}break;

			case COMMAND_FOLLOW:
			{
				CommandFollow(Selectindex);
			}break;

			case COMMAND_BATTLE:
			{
				CommandDual(pSelectedCha);
			}break;
			case COMMAND_PCBANG:
			{
				CommandPCBangPoint();
			}break;
			}
		}
		
		m_iCurSelectCommand = COMMAND_NONE;
	}
}

void SEASON3B::CNewUICommandWindow::SelectCommand()
{

}

int SEASON3B::CNewUICommandWindow::GetCurCommandType()
{
	return m_iCurSelectCommand;
}

void SEASON3B::CNewUICommandWindow::SetMouseCursor(int iCursorType)
{
	m_iCurMouseCursor = iCursorType;
}

int SEASON3B::CNewUICommandWindow::GetMouseCursor()
{
	return m_iCurMouseCursor;
}

void SEASON3B::CNewUICommandWindow::LoadImages()
{
	LoadBitmap("Common\\Interface\\ui_item_backtop.jpg", IMAGE_COMMAND_BASE_WINDOW_TOP);
	LoadBitmap("Common\\Interface\\ui_item_backlow.jpg", IMAGE_COMMAND_BASE_WINDOW_LOW);

	LoadBitmap("Common\\Interface\\ui_menu_exit.jpg", IMAGE_COMMAND_BASE_WINDOW_BTN_EXIT);

	LoadBitmap("Common\\Interface\\ui_message_box.jpg", IMAGE_COMMAND_BTN);

	LoadBitmap("Common\\Interface\\ui_cursorid_wnd.jpg", IMAGE_COMMAND_SELECTID_BG);
}

void SEASON3B::CNewUICommandWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_COMMAND_BASE_WINDOW_TOP);
	DeleteBitmap(IMAGE_COMMAND_BASE_WINDOW_LOW);

	DeleteBitmap(IMAGE_COMMAND_BASE_WINDOW_BTN_EXIT);
	DeleteBitmap(IMAGE_COMMAND_BTN);
	DeleteBitmap(IMAGE_COMMAND_SELECTID_BG);
}

bool SEASON3B::CNewUICommandWindow::CommandTrade(CHARACTER* pSelectedCha)
{
	if (pSelectedCha == NULL)
		return false;

	int level = CharacterAttribute->Level;

	if (level < TRADELIMITLEVEL)
	{
		g_pChatListBox->AddText("", GlobalText[478], SEASON3B::TYPE_SYSTEM_MESSAGE);
		return false;
	}
	if (IsShopInViewport(pSelectedCha))
	{
		g_pChatListBox->AddText("", GlobalText[493], SEASON3B::TYPE_ERROR_MESSAGE);
		return false;
	}

	SendRequestTrade(pSelectedCha->Key);

	return true;
}

bool SEASON3B::CNewUICommandWindow::CommandPurchase(CHARACTER* pSelectedCha)
{
	if (pSelectedCha == NULL)
		return false;

	SendRequestOpenPersonalShop(pSelectedCha->Key, pSelectedCha->ID);

	return true;
}

bool SEASON3B::CNewUICommandWindow::CommandParty(SHORT iChaKey)
{
	if (PartyNumber > 0 && strcmp(Party[0].Name, Hero->ID) != NULL)
	{
		g_pChatListBox->AddText("", GlobalText[257], SEASON3B::TYPE_ERROR_MESSAGE);
		return false;
	}

	SendRequestParty(iChaKey);

	return true;
}

bool SEASON3B::CNewUICommandWindow::CommandWhisper(CHARACTER* pSelectedCha)
{
	g_pChatInputBox->SetWhsprID(pSelectedCha->ID);

	return true;
}

bool SEASON3B::CNewUICommandWindow::CommandGuild(CHARACTER* pSelectedChar)
{
	if (Hero->GuildStatus != G_NONE)
	{
		g_pChatListBox->AddText("", GlobalText[255], SEASON3B::TYPE_SYSTEM_MESSAGE);
		return false;
	}
	if ((pSelectedChar->GuildMarkIndex < 0) || (pSelectedChar->GuildStatus != G_MASTER))
	{
		g_pChatListBox->AddText("", GlobalText[507], SEASON3B::TYPE_SYSTEM_MESSAGE);
		return false;
	}

	SendRequestGuild(pSelectedChar->Key);

	return true;
}

bool SEASON3B::CNewUICommandWindow::CommandGuildUnion(CHARACTER* pSelectedCha)
{
	if (Hero->GuildStatus != G_MASTER)
	{
		g_pChatListBox->AddText("", GlobalText[1320], SEASON3B::TYPE_SYSTEM_MESSAGE);
		return false;
	}
	if (pSelectedCha->GuildStatus == G_NONE)
	{
		g_pChatListBox->AddText("", GlobalText[1385], SEASON3B::TYPE_SYSTEM_MESSAGE);
		return false;
	}
	if (pSelectedCha->GuildStatus != G_MASTER)
	{
		g_pChatListBox->AddText("", GlobalText[507], SEASON3B::TYPE_SYSTEM_MESSAGE);
		return false;
	}
	if (pSelectedCha->GuildStatus == G_MASTER)
	{
		SendRequestGuildRelationShip(0x01, 0x01, HIBYTE(pSelectedCha->Key), LOBYTE(pSelectedCha->Key));
		return true;
	}

	return false;
}

bool SEASON3B::CNewUICommandWindow::CommandGuildRival(CHARACTER* pSelectedCha)
{
	if (Hero->GuildStatus != G_MASTER)
	{
		g_pChatListBox->AddText("", GlobalText[1320], SEASON3B::TYPE_SYSTEM_MESSAGE);
		return false;
	}

	if (pSelectedCha->GuildStatus != G_MASTER)
	{
		g_pChatListBox->AddText("", GlobalText[507], SEASON3B::TYPE_SYSTEM_MESSAGE);
		return false;
	}

	SendRequestGuildRelationShip(0x02, 0x01, HIBYTE(pSelectedCha->Key), LOBYTE(pSelectedCha->Key));

	return true;
}

bool SEASON3B::CNewUICommandWindow::CommandCancelGuildRival(CHARACTER* pSelectedCha)
{
	if (Hero->GuildStatus != G_MASTER)
	{
		g_pChatListBox->AddText("", GlobalText[1320], SEASON3B::TYPE_SYSTEM_MESSAGE);
		return false;
	}
	if (pSelectedCha->GuildStatus != G_MASTER)
	{
		g_pChatListBox->AddText("", GlobalText[507], SEASON3B::TYPE_SYSTEM_MESSAGE);
		return false;
	}

	SetAction(&Hero->Object, PLAYER_RESPECT1);
	SendRequestAction(AT_RESPECT1, ((BYTE)((Hero->Object.Angle[2] + 22.5f) / 360.f * 8.f + 1.f) % 8));
	SendRequestGuildRelationShip(0x02, 0x02, HIBYTE(pSelectedCha->Key), LOBYTE(pSelectedCha->Key));

	return true;
}

bool SEASON3B::CNewUICommandWindow::CommandAddFriend(CHARACTER* pSelectedCha)
{
	if (g_pWindowMgr->IsServerEnable() == TRUE)
	{
		SendRequestAddFriend(pSelectedCha->ID);
		return true;
	}

	return false;
}

bool SEASON3B::CNewUICommandWindow::CommandFollow(int iSelectedChaIndex)
{
	if (iSelectedChaIndex < 0)
	{
		return false;
	}

	g_iFollowCharacter = iSelectedChaIndex;

	return true;
}

int SEASON3B::CNewUICommandWindow::CommandDual(CHARACTER* pSelectedCha)
{
	if (!g_bEnableDuel)
	{
		SendRequestDuelStart(pSelectedCha->Key, pSelectedCha->ID);
		return 1;
	}
	else if (g_bEnableDuel == true && strcmp(pSelectedCha->ID, g_szDuelPlayerID) == NULL)
	{
		SendRequestDuelEnd();
		return 2;
	}
	else
	{
		g_pChatListBox->AddText("", GlobalText[915], SEASON3B::TYPE_ERROR_MESSAGE);
		return 3;
	}

	return 0;
}

bool SEASON3B::CNewUICommandWindow::CommandPCBangPoint()
{
	PlayBuffer(SOUND_CLICK01);

	PlayBuffer(SOUND_INTERFACE01);

	MouseUpdateTime = 0;
	MouseUpdateTimeMax = 6;

	SendRequestPCRoomPointShopOpen();
	return true;
}