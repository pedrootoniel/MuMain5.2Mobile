#pragma once

#define INVENTORY_WEAR_SIZE 12

#define GetChangeUp(x) (((x>>4)&0x01)?3:(((x>>3)&0x01)?2:1))

void InitItemMove();
bool EquipmentWindowProcess(LPVOID WindowThis);
bool UpdateMouseEvent(DWORD* WindowThis);
void RightClickMoveItem();
void RightClickDeleteItem(BYTE result);
BYTE GetInventoryWearSlot(int ItemIndex);