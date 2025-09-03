#include "stdafx.h"
#include "ItemMove.h"
#include "Offset.h"
#include "Protect.h"
#include "Item.h"
#include "Shop.h"
#include "Util.h"

bool ItemMove = false;
int SourceFlag = -1;
int SourceSlot = -1;
int TargetSlot = -1;

void InitItemMove() // OK
{
	if(gProtect.m_MainInfo.ItemMoveRightClicSwitch == 0)
	{
		return;
	}

	SetCompleteHook(0xE8,0x0073342B,&EquipmentWindowProcess);

	SetCompleteHook(0xE8,0x0074FCDB,&UpdateMouseEvent);
}

bool EquipmentWindowProcess(LPVOID WindowThis) // OK
{
	if(ItemMove)
	{
		return true;
	}

	return ((bool(__thiscall*)(LPVOID))0x00735DB4)(WindowThis);
}

bool UpdateMouseEvent(DWORD* WindowThis) // OK
{
	RightClickMoveItem();

	return ((bool(__thiscall*)(DWORD))0x007206DE)(*(DWORD*)WindowThis);
}

void RightClickMoveItem() // OK
{
	if(!IsPress(VK_RBUTTON) || PickedItem || ItemMove)
	{
		return;
	}

	if(!CheckWindow(INTERFACE_INVENTORY))
	{
		return;
	}

	ItemMove = false;

	SourceFlag = -1;

	TargetSlot = -1;

	SourceSlot = *(DWORD*)((DWORD)InventoryThis(pWindowThis())+276);

	if(SourceSlot >= 0 && SourceSlot < INVENTORY_WEAR_SIZE)
	{
		if(*(DWORD*)MAIN_CURRENT_MAP == 10 || *(DWORD*)MAIN_CURRENT_MAP == 39)
		{
			if(SourceSlot == 7 && GetItemEquipedIndex(8) != GET_ITEM(13,3) && GetItemEquipedIndex(8) != GET_ITEM(13,4) && GetItemEquipedIndex(8) != GET_ITEM(13,37))
			{
				return;
			}

			if(SourceSlot == 8 && (GetItemEquipedIndex(8) == GET_ITEM(13,3) || GetItemEquipedIndex(8) == GET_ITEM(13,4) || GetItemEquipedIndex(8) == GET_ITEM(13,37)) && GetItemEquipedIndex(7) == 0xFFFF)
			{
				return;
			}
		}

		DWORD ItemStruct = GetItemEquiped(SourceSlot);

		if(ItemStruct && GetItemEquipedIndex(SourceSlot) != 0xFFFF)
		{
			ITEM_INFO* lpItemInfo = GetItemInfo(*(WORD*)ItemStruct);

			TargetSlot = InventoryFindEmptySlot(InventoryThis(pWindowThis()),lpItemInfo->Width,lpItemInfo->Height);

			if(TargetSlot != -1)
			{
				SendRequestEquipmentItem(0,SourceSlot,ItemStruct,0,INVENTORY_WEAR_SIZE+TargetSlot);
				ResetMouseRButton();
				ItemMove = true;
				SourceFlag = 0;
			}
		}

		return;
	}

	SourceSlot = InventoryFindItemSlot(MixGetMyInventoryCtrl(MixInventoryThis(pWindowThis())),pCursorX,pCursorY);

	if(SourceSlot != -1)
	{
		DWORD MixItemStruct = (DWORD)InventoryFindItem(MixGetMyInventoryCtrl(MixInventoryThis(pWindowThis())),pCursorX,pCursorY);

		if(MixItemStruct)
		{
			ITEM_INFO* lpItemInfo = GetItemInfo(*(WORD*)MixItemStruct);

			TargetSlot = InventoryFindEmptySlot(InventoryThis(pWindowThis()),lpItemInfo->Width,lpItemInfo->Height);

			if(SourceSlot != -1 && TargetSlot != -1)
			{
				SendRequestEquipmentItem(3,SourceSlot,MixItemStruct,0,INVENTORY_WEAR_SIZE+TargetSlot);
				ResetMouseRButton();
				ItemMove = true;
				SourceFlag = 1;
			}
		}

		return;
	}

	int InventoryMain = InventoryFindItemSlot(InventoryPointer(InventoryThis(pWindowThis())),pCursorX,pCursorY);

	if(InventoryMain != -1)
	{
		if(CheckWindow(INTERFACE_MIXINVENTORY))
		{
			DWORD ItemStruct = (DWORD)InventoryFindItem(InventoryPointer(InventoryThis(pWindowThis())),pCursorX,pCursorY);

			if(ItemStruct)
			{
				ITEM_INFO* lpItemInfo = GetItemInfo(*(WORD*)ItemStruct);

				SourceSlot = INVENTORY_WEAR_SIZE + InventoryMain;

				TargetSlot = InventoryMixFindEmptySlot(MixGetMyInventoryCtrl(MixInventoryThis(pWindowThis())),lpItemInfo->Width,lpItemInfo->Height);

				if(SourceSlot != -1 && TargetSlot != -1)
				{
					SendRequestEquipmentItem(0,SourceSlot,ItemStruct,3,TargetSlot);
					ResetMouseRButton();
					ItemMove = true;
					SourceFlag = 0;
				}
			}
		}
		else
		{
			if(CheckWindow(INTERFACE_TRADE) || CheckWindow(INTERFACE_STORAGE))
			{
				return;
			}

			DWORD ItemStruct = (DWORD)InventoryFindItem(InventoryPointer(InventoryThis(pWindowThis())),pCursorX,pCursorY);

			if(ItemStruct)
			{
				int x = *(BYTE*)(ItemStruct + 59);

				int y = *(BYTE*)(ItemStruct + 60);

				SourceSlot = ((y*8)+x)+INVENTORY_WEAR_SIZE;

				TargetSlot = GetInventoryWearSlot(*(WORD*)ItemStruct);

				if(TargetSlot != 0xFF)
				{
					SendRequestEquipmentItem(0,SourceSlot,ItemStruct,0,TargetSlot);
					ResetMouseRButton();
					ItemMove = true;
					SourceFlag = 0;
				}
			}	
		}
	}
}

void RightClickDeleteItem(BYTE result) // OK
{
	if(ItemMove == true)
	{
		if(result != 0xFF)
		{
			if(SourceFlag == 0)
			{
				if(SourceSlot >= 0 && SourceSlot < INVENTORY_WEAR_SIZE)
				{
					InventoryUnequipItem(InventoryThis(pWindowThis()),SourceSlot);
				}
				else if(SourceSlot >= INVENTORY_WEAR_SIZE)
				{
					InventoryDeleteItem(InventoryThis(pWindowThis()),SourceSlot-INVENTORY_WEAR_SIZE);
				}
			}
			else
			{
				InventoryMixDeleteItem(MixInventoryThis(pWindowThis()),SourceSlot);
			}
		}

		ItemMove = false;
	}
}

BYTE GetInventoryWearSlot(int ItemIndex) // OK
{
	int slot = 0xFF;

	ITEM_INFO* lpItemInfo = GetItemInfo(ItemIndex);

	BYTE Class = ((*(BYTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0B)) & 7);

	BYTE ChangeUp = GetChangeUp(*(BYTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0B));

	WORD ItemR = GetItemEquipedIndex(0);
	WORD ItemL = GetItemEquipedIndex(1);
	WORD RingR = GetItemEquipedIndex(10);
	WORD RingL = GetItemEquipedIndex(11);

	if(lpItemInfo->Slot >= 0 && lpItemInfo->Slot < INVENTORY_WEAR_SIZE)
	{
		if(lpItemInfo->TwoHand == 0)
		{
			if(lpItemInfo->Slot == 0 && ItemR != 0xFFFF && ItemL == 0xFFFF)
			{
				ITEM_INFO* lpItemRight = GetItemInfo(ItemR);

				if(lpItemRight->TwoHand == 0)
				{
					slot = 1;
				}
			}
			else if(lpItemInfo->Slot == 1 && ItemL != 0xFFFF && ItemR == 0xFFFF)
			{
				ITEM_INFO* lpItemLeft = GetItemInfo(ItemL);

				if(lpItemLeft->TwoHand == 0)
				{
					slot = 1;
				}
			}
			else if(lpItemInfo->Slot == 10 && RingR != 0xFFFF && RingL == 0xFFFF)
			{
				slot = 11;
			}
		}

		if(GetItemEquipedIndex(lpItemInfo->Slot) == 0xFFFF)
		{
			slot = lpItemInfo->Slot;
		}
	}

	bool bEquipable = false;

	if(lpItemInfo->RequireClass[Class])
	{
		bEquipable = true;
	}
	else if(Class == 3 && lpItemInfo->RequireClass[0] && lpItemInfo->RequireClass[1])
	{
		bEquipable = true;
	}

	if(lpItemInfo->RequireClass[Class] > ChangeUp)
	{
		bEquipable = false;
	}

	if(bEquipable == true)
	{
		bEquipable = false;

		if(lpItemInfo->Slot == slot)
		{
			bEquipable = true;
		}
		else if(lpItemInfo->Slot == 0 && slot == 1)
		{
			if((Class == 1 || Class == 3) && lpItemInfo->TwoHand == 0)
			{
				bEquipable = true;
			}
			else if(Class == 5 && !(ItemIndex >= GET_ITEM(5,0) && ItemIndex < GET_ITEM(6,0)))
			{
				bEquipable = true;
			}
		}
		else if(lpItemInfo->Slot == 10 && slot == 11)
		{
			bEquipable = true;
		}

		if(Class == 2)
		{
			if(lpItemInfo->Slot == 0 && ItemL != GET_ITEM(4,7) && (ItemL >= GET_ITEM(4,0) && ItemL < GET_ITEM(5,0)) && ItemIndex != GET_ITEM(4,15))
			{
				bEquipable = false;
			}
		}
	
		if(Class == 3 && lpItemInfo->Slot == 2)
		{
			bEquipable = false;
		}
	}

	return ((bEquipable) ? slot : 0xFF);
}