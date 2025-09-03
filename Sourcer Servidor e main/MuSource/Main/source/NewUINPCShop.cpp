// NewUINPCShop.cpp: implementation of the CNewUINPCShop class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUINPCShop.h"
#include "NewUISystem.h"
#include "NewUICommonMessageBox.h"
#include "ZzzInventory.h"
#include "wsclientinline.h"
#include "GambleSystem.h"
#include "PCRoom.h"

using namespace SEASON3B;

SEASON3B::CNewUINPCShop::CNewUINPCShop() 
{ 
	Init(); 
}

SEASON3B::CNewUINPCShop::~CNewUINPCShop() 
{ 
	Release(); 
}

void SEASON3B::CNewUINPCShop::Init()
{
	m_pNewUIMng = NULL;
	m_pNewInventoryCtrl = NULL;
	m_Pos.x = m_Pos.y = 0;
	m_dwShopState = SHOP_STATE_BUYNSELL;
	m_iTaxRate = 0;
	m_bRepairShop = false;
	m_bIsNPCShopOpen = false;
	m_dwStandbyItemKey = 0;
	m_bSellingItem = false;
}

bool SEASON3B::CNewUINPCShop::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng || NULL == g_pNewItemMng)
		return false;

	m_pNewUIMng = pNewUIMng;

	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_NPCSHOP, this);

	m_pNewInventoryCtrl = new CNewUIInventoryCtrl;

	if (false == m_pNewInventoryCtrl->Create(g_pNewUI3DRenderMng, g_pNewItemMng, this, x + 15, y + 50, 8, 15))
	{
		SAFE_DELETE(m_pNewInventoryCtrl);
		return false;
	}

	if (m_pNewInventoryCtrl)
	{
		m_pNewInventoryCtrl->LockInventory();
		m_pNewInventoryCtrl->SetToolTipType(TOOLTIP_TYPE_NPC_SHOP);
	}

	SetPos(x, y);

	LoadImages();

	Show(false);

	return true;
}

void SEASON3B::CNewUINPCShop::Release()
{
	UnloadImages();

	SAFE_DELETE(m_pNewInventoryCtrl);

	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUINPCShop::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool SEASON3B::CNewUINPCShop::UpdateMouseEvent()
{
	if(m_pNewInventoryCtrl)
	{	
		if(false == m_pNewInventoryCtrl->UpdateMouseEvent())
		{
			return false;
		}
		
		if(InventoryProcess() == true)
		{
			return false;
		}

		if(m_pNewInventoryCtrl->CheckPtInRect(MouseX, MouseY) == true)
		{
			ITEM* pItem = m_pNewInventoryCtrl->FindItemAtPt(MouseX, MouseY);

			if((m_bIsNPCShopOpen == true) && (pItem) && (SEASON3B::IsRelease(VK_LBUTTON)))
			{
				int iIndex = (pItem->y * m_pNewInventoryCtrl->GetNumberOfColumn()) + pItem->x;

				GambleSystem& _gambleSys = GambleSystem::Instance();

				CPCRoomPtSys& rPCRoomPtSys = CPCRoomPtSys::Instance();

				if (rPCRoomPtSys.IsPCRoomPointShopMode())
				{
					rPCRoomPtSys.SetBuyItemPos(iIndex);

					g_pNPCShop->SetStandbyItemKey(pItem->Key);

					SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CPCRoomPointItemBuyMsgBoxLayout));

					return false;
				}
				else
				{
					if (_gambleSys.IsGambleShop())
					{
						_gambleSys.SetBuyItemInfo(iIndex, ItemValue(pItem, 0));

						g_pNPCShop->SetStandbyItemKey(pItem->Key);

						SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CGambleBuyMsgBoxLayout));

						return false;
					}
					else
					{
						if (BuyCost == 0)
						{
							SendRequestBuy(iIndex, ItemValue(pItem, 0));
						}

						return false;
					}
				}				
			}
			else if(SEASON3B::IsRelease(VK_LBUTTON))
			{
				m_bIsNPCShopOpen = true;
				return false;
			}
			else if(SEASON3B::IsPress(VK_LBUTTON))
			{
				return false;
			}
		}
	}

	if(BtnProcess() == true)
	{
		return false;
	}

	if(CheckMouseIn(m_Pos.x, m_Pos.y, NPCSHOP_WIDTH, NPCSHOP_HEIGHT))
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
	}

	return true;
}

bool SEASON3B::CNewUINPCShop::UpdateKeyEvent()
{
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_NPCSHOP) == false)
	{
		return true;
	}

	if(SEASON3B::IsRepeat(VK_SHIFT) && SEASON3B::IsPress('L'))
	{
		SendRequestRepair(255, 0);
		return false;
	}
	else if(SEASON3B::IsPress('L'))
    {
		if(m_bRepairShop && CNewUIInventoryCtrl::GetPickedItem() == NULL )	
        {
			ToggleState();
			return false;
        }
    }
	
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_NPCSHOP) == true)
	{
		if(SEASON3B::IsPress(VK_ESCAPE) == true	&& m_bSellingItem == false)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_NPCSHOP);

			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}

	return true;
}

bool SEASON3B::CNewUINPCShop::Update()
{
	if(m_bRepairShop)
	{
		RepairAllGold();
	}

	if(m_pNewInventoryCtrl && false == m_pNewInventoryCtrl->Update())
	{
		return false;
	}

	return true;
}

bool SEASON3B::CNewUINPCShop::Render()
{
	EnableAlphaTest();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	RenderFrame();

	RenderTexts();

	RenderButton();

	RenderRepairMoney();

	if(m_pNewInventoryCtrl)
	{
		m_pNewInventoryCtrl->Render();
	}

	DisableAlphaBlend();

	return true;
}

void SEASON3B::CNewUINPCShop::RenderFrame()
{
	RenderBitmap(IMAGE_NPCSHOP_TOP, m_Pos.x, m_Pos.y, 190.0, 256.0, 0, 0, 0.7421875, 1.0, 1, 1, 0.0);
	RenderBitmap(IMAGE_NPCSHOP_LOW, m_Pos.x, m_Pos.y + 256.0, 190.0, 177.0, 0, 0, 0.7421875, 0.69140625, 1, 1, 0.0);
}

void SEASON3B::CNewUINPCShop::RenderTexts()
{
	if (CPCRoomPtSys::Instance().IsPCRoomPointShopMode())	// PC포인트 상점인가?
	{
		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->SetBgColor(0);
		g_pRenderText->SetTextColor(255, 128, 255, 255);

		g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 12, GlobalText[2325], NPCSHOP_WIDTH, 0, RT3_SORT_CENTER);

		const short c_nLineMax = 2;
		const short c_nLowMax = 36;

		unicode::t_char aszNotice[c_nLineMax][c_nLowMax];

		const short c_nNoticeMax = 3;

		const unicode::t_char* c_apszGlobalText[c_nNoticeMax] = { GlobalText[2327], GlobalText[2328], GlobalText[2332] };

		int i, j;

		for (i = 0; i < c_nNoticeMax; ++i)
		{
			::memset(aszNotice[0], 0, sizeof(unicode::t_char) * c_nLineMax * c_nLowMax);

			::SeparateTextIntoLines(c_apszGlobalText[i], aszNotice[0], c_nLineMax, c_nLowMax);

			for (j = 0; j < c_nLineMax; ++j)
			{
				if (0 == unicode::_strlen(aszNotice[j])) continue;

				g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 356 + (i * 22) + (j * 11),	aszNotice[j], 190, 0, RT3_SORT_CENTER);
			}
		}

		g_pRenderText->SetFont(g_hFont);

		return;
	}

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(220, 220, 220, 255);

	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 8, GlobalText[230], NPCSHOP_WIDTH, 0, RT3_SORT_CENTER);
	
	unicode::t_char strText[256];

	unicode::_sprintf(strText, GlobalText[1623], m_iTaxRate);

	g_pRenderText->RenderText(m_Pos.x,m_Pos.y + 20, strText, NPCSHOP_WIDTH, 0, RT3_SORT_CENTER);
}

void SEASON3B::CNewUINPCShop::RenderButton()
{
	if(m_bRepairShop)
	{
		if (SEASON3B::CheckMouseIn(m_Pos.x + 54, m_Pos.y + 365, 24, 24) == true)
		{
			RenderTipText(m_Pos.x + 54, m_Pos.y + 350, GlobalText[233]);
		}

		if (g_pMyInventory->GetRepairMode() == SEASON3B::CNewUIMyInventory::REPAIR_MODE_OFF)
		{
			RenderBitmap(IMAGE_NPCSHOP_BTN_REPAIR, m_Pos.x + 54.0, m_Pos.y + 365.0, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
		}
		else
		{
			RenderBitmap(IMAGE_NPCSHOP_BTN_REPAIR_CLICK, m_Pos.x + 54.0, m_Pos.y + 365.0, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
		}

		if (SEASON3B::CheckMouseIn(m_Pos.x + 98, m_Pos.y + 365, 24, 24) == true)
		{
			RenderTipText(m_Pos.x + 98, m_Pos.y + 350, GlobalText[237]);
		}

		glColor3f(0.5, 0.69999999, 1.0);

		RenderBitmap(IMAGE_NPCSHOP_BTN_REPAIR, m_Pos.x + 98.0, m_Pos.y + 365.0, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);

		glColor3f(1.0, 1.0, 1.0);
	}
}

void SEASON3B::CNewUINPCShop::RenderRepairMoney()
{
	if(m_bRepairShop)
	{
		RenderBitmap(IMAGE_NPCSHOP_REPAIR_MONEY, m_Pos.x + 55, m_Pos.y + 395, 110.0, 18.0, 0, 0, 0.859375, 0.5625, 1, 1, 0.0);

		g_pRenderText->SetBgColor(30, 30, 30, 255);

		g_pRenderText->SetTextColor(255, 220, 150, 255);

		unicode::t_char strText[256];

        ConvertGold(AllRepairGold, strText);

		g_pRenderText->SetFont(g_hFontBold);

		g_pRenderText->RenderText(m_Pos.x + 17, m_Pos.y + 400, GlobalText[239]);

        g_pRenderText->SetTextColor(getGoldColor(AllRepairGold));

		g_pRenderText->RenderText(m_Pos.x + 65, m_Pos.y + 400, strText, 0, 0, RT3_SORT_RIGHT);
	}
}

float SEASON3B::CNewUINPCShop::GetLayerDepth()
{ 
	return 4.55;	
}

void SEASON3B::CNewUINPCShop::LoadImages()
{
	LoadBitmap("Common\\Interface\\ui_item_backtop.jpg", IMAGE_NPCSHOP_TOP);
	LoadBitmap("Common\\Interface\\ui_item_backlow.jpg", IMAGE_NPCSHOP_LOW);

	LoadBitmap("Common\\Interface\\ui_menu_repair.jpg", IMAGE_NPCSHOP_BTN_REPAIR);
	LoadBitmap("Common\\Interface\\ui_menu_repair_click.jpg", IMAGE_NPCSHOP_BTN_REPAIR_CLICK);

	LoadBitmap("Common\\Interface\\ui_item_money.jpg", IMAGE_NPCSHOP_REPAIR_MONEY);
}

void SEASON3B::CNewUINPCShop::UnloadImages()
{
	DeleteBitmap(IMAGE_NPCSHOP_TOP);
	DeleteBitmap(IMAGE_NPCSHOP_LOW);

	DeleteBitmap(IMAGE_NPCSHOP_BTN_REPAIR);
	DeleteBitmap(IMAGE_NPCSHOP_BTN_REPAIR_CLICK);

	DeleteBitmap(IMAGE_NPCSHOP_REPAIR_MONEY);
}

void SEASON3B::CNewUINPCShop::SetTaxRate(int iTaxRate)
{
	m_iTaxRate = iTaxRate;
}

int SEASON3B::CNewUINPCShop::GetTaxRate()
{	
	return m_iTaxRate;
}

bool SEASON3B::CNewUINPCShop::InsertItem(int iIndex, BYTE* pbyItemPacket)
{
	if(m_pNewInventoryCtrl)
	{
		return m_pNewInventoryCtrl->AddItem(iIndex, pbyItemPacket);
	}
	
	return false;
}

bool SEASON3B::CNewUINPCShop::InventoryProcess()
{
	CNewUIPickedItem* pPickedItem = CNewUIInventoryCtrl::GetPickedItem();

	if( !m_pNewInventoryCtrl )	return false;
	if( !pPickedItem )			return false;

	ITEM* pItem = pPickedItem->GetItem();

	if(	SEASON3B::IsRelease(VK_LBUTTON) == true	&& m_pNewInventoryCtrl->CheckPtInRect(MouseX, MouseY) == true && m_bSellingItem == false)
	{
		if (CPCRoomPtSys::Instance().IsPCRoomPointShopMode())
		{
			g_pChatListBox->AddText("", GlobalText[2329], SEASON3B::TYPE_ERROR_MESSAGE);

			return true;
		}

		if (CharacterMachine->Gold + ItemValue(pItem) > 2000000000)
		{
			g_pChatListBox->AddText("", GlobalText[3148], SEASON3B::TYPE_SYSTEM_MESSAGE);
			
			return true;
		}

		if( pItem && pItem->Jewel_Of_Harmony_Option != 0 )
		{
			g_pChatListBox->AddText("", GlobalText[2211], SEASON3B::TYPE_ERROR_MESSAGE);

			return true;
		}
		else if(pItem && IsSellingBan(pItem) == true)
		{
			g_pChatListBox->AddText("", GlobalText[668], SEASON3B::TYPE_ERROR_MESSAGE);
			m_pNewInventoryCtrl->BackupPickedItem();
			
			return true;
		}
		else if(pItem && IsHighValueItem(pItem) == true)
		{
			SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CHighValueItemCheckMsgBoxLayout));
			pPickedItem->HidePickedItem();
			
			return true;
		}

		if (pPickedItem->GetOwnerInventory() == g_pMyInventory->GetInventoryCtrl())
		{
			int iSourceIndex = pPickedItem->GetSourceLinealPos() + MAX_EQUIPMENT_INDEX;
			SendRequestSell(iSourceIndex);

			return true;
		}
		else if(pPickedItem->GetOwnerInventory() == NULL)
		{
			int iSourceIndex = pPickedItem->GetSourceLinealPos();
			SendRequestSell(iSourceIndex);
			
			return true;
		}
	}

	return false;
}

bool SEASON3B::CNewUINPCShop::BtnProcess()
{
	if(m_bRepairShop)
	{
		if (SEASON3B::CheckMouseIn(m_Pos.x + 54, m_Pos.y + 365, 24, 24) == true && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;
			MouseUpdateTimeMax = 6;

			ToggleState();
			
			PlayBuffer(SOUND_CLICK01);

			return true;
		}

		if (SEASON3B::CheckMouseIn(m_Pos.x + 98, m_Pos.y + 365, 24, 24) == true && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;
			MouseUpdateTimeMax = 6;

			SendRequestRepair(255, 0);
			
			PlayBuffer(SOUND_CLICK01);

			return true;
		}
	}

	return false;
}

void SEASON3B::CNewUINPCShop::OpenningProcess()
{
	if( SEASON3B::IsRepeat(VK_LBUTTON))
	{
		m_bIsNPCShopOpen = false;
	}
	else
	{
		m_bIsNPCShopOpen = true;
	}
}

void SEASON3B::CNewUINPCShop::ClosingProcess()
{
	SendExitInventory();

	m_dwShopState = SHOP_STATE_BUYNSELL;
	m_iTaxRate = 0;
	m_bRepairShop = false;
	m_dwStandbyItemKey = 0;

	m_bIsNPCShopOpen = false;

	if(m_pNewInventoryCtrl)
	{
		m_pNewInventoryCtrl->RemoveAllItems();
	}

	CPCRoomPtSys::Instance().SetPCRoomPointShopMode(false);

	GambleSystem::Instance().SetGambleShop(false);

	m_bSellingItem = false;
}

void SEASON3B::CNewUINPCShop::SetRepairShop(bool bRepair)
{
	m_bRepairShop = bRepair;
}

bool SEASON3B::CNewUINPCShop::IsRepairShop()
{
	return m_bRepairShop;
}

void SEASON3B::CNewUINPCShop::ToggleState()
{
	if(m_dwShopState == SHOP_STATE_BUYNSELL)
	{
		m_dwShopState = SHOP_STATE_REPAIR;
		
		g_pMyInventory->SetRepairMode(true);
	}
	else
	{
		m_dwShopState = SHOP_STATE_BUYNSELL;
		g_pMyInventory->SetRepairMode(false);
	}
}

DWORD SEASON3B::CNewUINPCShop::GetShopState()
{	
	return m_dwShopState;
}


int SEASON3B::CNewUINPCShop::GetPointedItemIndex()
{
	return m_pNewInventoryCtrl->GetPointedSquareIndex();
}

void SEASON3B::CNewUINPCShop::SetStandbyItemKey(DWORD dwItemKey)
{ 
	m_dwStandbyItemKey = dwItemKey; 
}

DWORD SEASON3B::CNewUINPCShop::GetStandbyItemKey() const
{ 
	return m_dwStandbyItemKey; 
}

int SEASON3B::CNewUINPCShop::GetStandbyItemIndex()
{
	ITEM* pItem = GetStandbyItem();
	if(pItem)
		return pItem->y*m_pNewInventoryCtrl->GetNumberOfColumn()+pItem->x;
	return -1;
}

ITEM* SEASON3B::CNewUINPCShop::GetStandbyItem()
{
	if(m_pNewInventoryCtrl)
		return m_pNewInventoryCtrl->FindItemByKey(m_dwStandbyItemKey);
	return NULL;
}

void SEASON3B::CNewUINPCShop::SetSellingItem(bool bFlag)
{
	m_bSellingItem = bFlag;
}

bool SEASON3B::CNewUINPCShop::IsSellingItem()
{
	return m_bSellingItem;
}
