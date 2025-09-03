#include "stdafx.h"
#include "PeriodicItem.h"
#include "CustomMessage.h"
#include "Offset.h"
#include "ItemMove.h"
#include "ItemToolTip.h"
#include "Util.h"
#include <ctime>

DWORD m_PeriodicInfo[76];

void RenderPeriodicTipText() // OK
{
	if(pMouseOnZone(450, 0, 190, 433, 1))
	{
		int SourceSlot = *(DWORD*)((DWORD)InventoryThis(pWindowThis()) + 276);

		DWORD lpItem = 0;

		if(SourceSlot != -1)
		{
			lpItem = GetItemEquiped(SourceSlot);
		}
		else
		{
			lpItem = (DWORD)InventoryFindItem(InventoryPointer(InventoryThis(pWindowThis())),pCursorX,pCursorY);

			if(lpItem)
			{
				int x = *(BYTE*)(lpItem + 59);

				int y = *(BYTE*)(lpItem + 60);

				SourceSlot = ((y*8)+x)+INVENTORY_WEAR_SIZE;
			}
		}

		if(SourceSlot != -1)
		{
			if(m_PeriodicInfo[SourceSlot] > 0)
			{
				tm ExpireTime;
	
				_tzset();

				time_t msgTime = m_PeriodicInfo[SourceSlot];

				if(localtime_s(&ExpireTime,&msgTime) == 0) 
				{
					char buff[32] = {0};
					strftime(buff,sizeof(buff),gCustomMessage.GetMessage(24),&ExpireTime);
					AddTipText(gCustomMessage.GetMessage(25),3,1);
					AddTipText(buff,1,0);
				}
			}
		}
	}
}

void GCPeriodicItemInit() // OK
{
	for(int n = 0;n < 76; n++)
	{
		m_PeriodicInfo[n] = 0;
	}
}

void GCPeriodicItemRecv(PMSG_PERIODIC_ITEM_RECV* lpMsg) // OK
{
	if(lpMsg->sourceslot != 0xFFFF)
	{
		m_PeriodicInfo[lpMsg->sourceslot] = 0;
	}

	m_PeriodicInfo[lpMsg->targetslot] = lpMsg->time;
}

void GCPeriodicItemDeleteRecv(PMSG_PERIODIC_ITEM_DELETE_RECV* lpMsg) // OK
{
	m_PeriodicInfo[lpMsg->slot] = 0;
}