#include "stdafx.h"
#include "Shop.h"
#include "CustomJewel.h"
#include "CustomMessage.h"
#include "CustomWing.h"
#include "Item.h"
#include "Offset.h"
#include "Util.h"

std::vector<ITEM_STACK_INFO> m_ItemStackInfo;

std::vector<ITEM_VALUE_INFO> m_ItemValueInfo;

std::map<int,SHOP_INFO> m_PcPointInfo;

std::map<int,SHOP_INFO> m_ShopInfo;

DWORD ShopValueType = 0;

DWORD ShopPcPoint = 0;

void InitShop() // OK
{
	SetCompleteHook(0xE9,0x005842E0,&GetItemDurability);

	SetCompleteHook(0xE9,0x00583E70,&GetItemRepairMoney);

	SetCompleteHook(0xE9,0x005056D2,&GetItemValue);

	SetCompleteHook(0xFF,0x005865A4,&ShowBuyPrice);

	SetCompleteHook(0xE9,0x005866DE,&ColorBuyPrice);
}

WORD GetItemDurability(DWORD address,ITEM_INFO* lpItemInfo,int level) // OK
{
	if(*(WORD*)address == GET_ITEM(14,21) || *(WORD*)address == GET_ITEM(14,29)) // Rena, Symbol of Kundun
	{
		return 1;
	}

	if(*(WORD*)address == GET_ITEM(13,18) || *(WORD*)address == GET_ITEM(13,29) || *(WORD*)address == GET_ITEM(13,51) || *(WORD*)address == GET_ITEM(14,19)) // Invisibility Cloak, Armor of Guardsman,  Scroll of Blood, Devil's Invitation
	{
		return 1;
	}

	int dur = 0;

	if(level >= 5)
	{
		if(level == 10)
		{
			dur = lpItemInfo->Durability+((level*2)-3);
		}
		else if(level == 11)
		{
			dur = lpItemInfo->Durability+((level*2)-1);
		}
		else if(level == 12)
		{
			dur = lpItemInfo->Durability+((level*2)+2);
		}
		else if(level == 13)
		{
			dur = lpItemInfo->Durability+((level*2)+6);
		}
		else
		{
			dur = lpItemInfo->Durability+((level*2)-4);
		}
	}
	else
	{
		dur = lpItemInfo->Durability+level;
	}

	if(*(WORD*)address != GET_ITEM(0,19) && *(WORD*)address != GET_ITEM(2,13) && *(WORD*)address != GET_ITEM(4,18) && *(WORD*)address != GET_ITEM(5,10) && *(WORD*)address != GET_ITEM(5,36) && lpItemInfo->Slot != 7) // Sword of Archangel, Scepter of Archangel, Crossbow of Archangel, Staff of Archangel, Stick of Archangel
	{
		if((*(BYTE *)(address + 24) % 4) != 0) // Ancient
		{
			dur += 20;
		}
		else if((*(BYTE *)(address + 23) & 63) != 0) // Excellent
		{
			dur += 15;
		}
	}

	return ((dur>255)?255:dur);
}

DWORD GetItemRepairMoney(int money,int dur,int maxdur,WORD index,char* TextOut) // OK
{
	int cost = 0;

	if(index == GET_ITEM(13,4) || index == GET_ITEM(13,5)) // Dark Horse, Dark Reaven
	{
		cost = money/1;
	}
	else
	{
		cost = money/3;
	}

	cost = ((cost>MAX_MONEY)?MAX_MONEY:cost);

	cost = ((cost>=100)?((cost/10)*10):cost);

	cost = ((cost>=1000)?((cost/100)*100):cost);

	float sq1 = sqrt((float)cost);
	float sq2 = sqrt(sq1);

	float value = (float)((((3.0*sq1)*sq2)*(1-((float)dur/(float)maxdur)))+1);

	if(dur <= 0)
	{
		if(index == GET_ITEM(13,4) || index == GET_ITEM(13,5)) // Dark Horse, Dark Reaven
		{
			value *= (float)2.0f;
		}
		else
		{
			value *= (float)1.4f;
		}
	}

	cost = (int)value;

	cost = ((cost>=100)?((cost/10)*10):cost);

	cost = ((cost>=1000)?((cost/100)*100):cost);

	if(cost < 1000000000)
	{
		if(cost < 1000000)
		{
			if(cost < 1000)
			{
				wsprintf(TextOut,"%u",cost%1000);
			}
			else
			{
				wsprintf(TextOut,"%u,%03u",cost%1000000/1000,cost%1000);
			}
		}
		else
		{
			wsprintf(TextOut,"%u,%03u,%03u",cost%1000000000/1000000,cost%1000000/1000,cost%1000);
		}
	}
	else
	{
		wsprintf(TextOut,"%u,%03u,%03u,%03u",cost/1000000000,cost%1000000000/1000000,cost%1000000/1000,cost%1000);
	}

	return cost;
}

DWORD GetItemValue(DWORD address,int type) // OK
{
	if(*(WORD*)address == -1)
	{
		return 0;
	}

	int m_ItemLevel = (*(DWORD*)(address + 2) >> 3) & 15;

	int m_ItemOption = ((*(DWORD*)(address + 2)) & 3)+((*(BYTE*)(address + 23)) & 64) / 64 * 4;

	DWORD m_BuyMoney = 0;

	DWORD m_SellMoney = 0;

	if(ShopValueType != 0)
	{
		int x = *(BYTE*)(address + 59);

		int y = *(BYTE*)(address + 60);

		std::map<int,SHOP_INFO>::iterator it = m_ShopInfo.find(y*8+x);

		if(it != m_ShopInfo.end())
		{
			return it->second.money;
		}
	
		return 0;
	}

	if(ShopPcPoint != 0)
	{
		int x = *(BYTE*)(address + 59);

		int y = *(BYTE*)(address + 60);

		std::map<int,SHOP_INFO>::iterator it = m_PcPointInfo.find(y*8+x);

		if(it != m_PcPointInfo.end())
		{
			return it->second.money;
		}
	
		return 0;
	}

	ITEM_INFO* lpItemInfo = GetItemInfo(*(WORD*)address);

	int value = 0;

	if(ItemValue(*(WORD*)address,m_ItemLevel,(*(BYTE*)(address + 23) & 63),*(BYTE*)(address + 22),&value) != 0)
	{
		m_BuyMoney = value;

		m_BuyMoney = ((m_BuyMoney>=100)?((m_BuyMoney/10)*10):m_BuyMoney);

		m_BuyMoney = ((m_BuyMoney>=1000)?((m_BuyMoney/100)*100):m_BuyMoney);

		m_SellMoney = value/3;

		m_SellMoney = ((m_SellMoney>=100)?((m_SellMoney/10)*10):m_SellMoney);

		m_SellMoney = ((m_SellMoney>=1000)?((m_SellMoney/100)*100):m_SellMoney);

		return ((type==1)?m_SellMoney:m_BuyMoney);
	}

	if(lpItemInfo->BuyMoney != 0)
	{
		m_BuyMoney = lpItemInfo->BuyMoney;

		m_BuyMoney = ((m_BuyMoney>=100)?((m_BuyMoney/10)*10):m_BuyMoney);

		m_BuyMoney = ((m_BuyMoney>=1000)?((m_BuyMoney/100)*100):m_BuyMoney);

		m_SellMoney = lpItemInfo->BuyMoney/3;

		m_SellMoney = ((m_SellMoney>=100)?((m_SellMoney/10)*10):m_SellMoney);

		m_SellMoney = ((m_SellMoney>=1000)?((m_SellMoney/100)*100):m_SellMoney);

		return ((type==1)?m_SellMoney:m_BuyMoney);
	}

	QWORD price = 0;

	if(gCustomJewel.CheckCustomJewelByItem(*(WORD*)address) != 0)
	{
		price = gCustomJewel.GetCustomJewelSalePrice(*(WORD*)address);
	}
	else if(lpItemInfo->Value > 0)
	{
		price = ((lpItemInfo->Value*lpItemInfo->Value)*10)/12;

		if(*(WORD*)address >= GET_ITEM(14,0) && *(WORD*)address <= GET_ITEM(14,8))
		{
			if(*(WORD*)address == GET_ITEM(14,3) || *(WORD*)address == GET_ITEM(14,6))
			{
				price *= 2;
			}

			price *= (QWORD)(1<<m_ItemLevel);

			price *= (QWORD)*(BYTE*)(address + 22);

			price = ((price>MAX_MONEY)?MAX_MONEY:price);

			m_BuyMoney = (DWORD)price;

			m_BuyMoney = ((m_BuyMoney>=10)?((m_BuyMoney/10)*10):m_BuyMoney);

			m_SellMoney = (DWORD)(price/3);

			m_SellMoney = ((m_SellMoney>=10)?((m_SellMoney/10)*10):m_SellMoney);

			return ((type==1)?m_SellMoney:m_BuyMoney);
		}
	}
	else
	{
		int ItemLevel = lpItemInfo->Level+(m_ItemLevel*3);

		if((*(BYTE *)(address + 23) & 63) != 0 && *(WORD*)address < GET_ITEM(12,0))
		{
			ItemLevel += 25;
		}

		if(((*(WORD*)address/512) == 12 && ((*(WORD*)address > GET_ITEM(12,6) && *(WORD*)address < GET_ITEM(12,36)) || *(WORD*)address > GET_ITEM(12,43)) && *(WORD*)address != GET_ITEM(12,50) && (*(WORD*)address < GET_ITEM(12,262) || *(WORD*)address > GET_ITEM(12,265)) && gCustomWing.CheckCustomWingByItem(*(WORD*)address) == 0) || (*(WORD*)address/512) == 13 || (*(WORD*)address/512) == 15)
		{
			price = ((ItemLevel*ItemLevel)*ItemLevel)+100;
			price = ((m_ItemOption>0)?(price+(price*m_ItemOption)):price);

			for(int n=0;n < 6;n++)
			{
				if((*(BYTE*)(address + 23) & (1 << n)) != 0)
				{
					price += ((*(WORD*)address < GET_ITEM(12,0))?((price*100)/100):((price*25)/100));
				}
			}
		}
		else
		{
			switch(m_ItemLevel)
			{
				case 5:
					ItemLevel += 4;
					break;
				case 6:
					ItemLevel += 10;
					break;
				case 7:
					ItemLevel += 25;
					break;
				case 8:
					ItemLevel += 45;
					break;
				case 9:
					ItemLevel += 65;
					break;
				case 10:
					ItemLevel += 95;
					break;
				case 11:
					ItemLevel += 135;
					break;
				case 12:
					ItemLevel += 185;
					break;
				case 13:
					ItemLevel += 245;
					break;
			}

			if((*(WORD*)address >= GET_ITEM(12,0) && *(WORD*)address <= GET_ITEM(12,6)) || (*(WORD*)address >= GET_ITEM(12,36) && *(WORD*)address <= GET_ITEM(12,43)) || *(WORD*)address == GET_ITEM(12,50) || (*(WORD*)address >= GET_ITEM(12,262) && *(WORD*)address <= GET_ITEM(12,265)) || gCustomWing.CheckCustomWingByItem(*(WORD*)address) != 0) // Wings
			{
				price = ((((ItemLevel+40)*ItemLevel)*ItemLevel)*11)+40000000;
			}
			else
			{
				price = ((((ItemLevel+40)*ItemLevel)*ItemLevel)/8)+100;
			}

			if(*(WORD*)address >= GET_ITEM(0,0) && *(WORD*)address < GET_ITEM(6,0))
			{
				if(lpItemInfo->TwoHand == 0)
				{
					price = (price*80)/100;
				}
			}

			if((*(DWORD*)(address + 2) >> 2) & 1)
			{
				price += (price*25)/100;
			}

			if(m_ItemOption != 0)
			{
				price += (((price*(60*m_ItemOption))/100));
			}

			for(int n=0;n < 6;n++)
			{
				if((*(BYTE*)(address + 23) & (1 << n)) != 0)
				{
					price += ((*(WORD*)address < GET_ITEM(12,0))?((price*100)/100):((price*25)/100));
				}
			}
		}
	}

	if(*(BYTE*)(address + 65) != 0)
	{
		price += (price*16)/100;
	}

	int slot = 0;

	int SocketMoney = 0;

	for(int n=0;n < 5;n++)
	{
		if(*(BYTE*)(address + 66 + n) != 255)
		{
			slot++;
		}

		if(*(BYTE*)(address + 66 + n) != 255 && *(BYTE*)(address + 66 + n) != 254)
		{
			int ItemOptionType = *(BYTE*)(address + 66 + n) % 50;

			int ItemOptionLevel = *(BYTE*)(address + 66 + n) / 50;

			int lpInfoType = 0;

			if(ItemOptionType >= 0 && ItemOptionType <= 5)
			{
				lpInfoType = 1;
			}
			else if(ItemOptionType >= 10 && ItemOptionType <= 14)
			{
				lpInfoType = 2;
			}
			else if(ItemOptionType >= 16 && ItemOptionType <= 20)
			{
				lpInfoType = 3;
			}
			else if(ItemOptionType >= 21 && ItemOptionType <= 26)
			{
				lpInfoType = 4;
			}
			else if(ItemOptionType >= 29 && ItemOptionType <= 32)
			{
				lpInfoType = 5;
			}
			else if(ItemOptionType == 36)
			{
				lpInfoType = 6;
			}
			else
			{
				continue;
			}

			int ItemIndex = (GET_ITEM(12,100)+(ItemOptionLevel*6)+(lpInfoType-1));

			ITEM_INFO* lpSocketItem = GetItemInfo(ItemIndex);

			SocketMoney += lpSocketItem->BuyMoney;
		}
	}

	if(slot > 0)
	{
		price += ((price*slot)*80)/100;
	}

	if(SocketMoney > 0)
	{
		price += SocketMoney;
	}

	price = ((price>MAX_MONEY)?MAX_MONEY:price);

	m_BuyMoney = (DWORD)price;

	m_BuyMoney = ((m_BuyMoney>=100)?((m_BuyMoney/10)*10):m_BuyMoney);

	m_BuyMoney = ((m_BuyMoney>=1000)?((m_BuyMoney/100)*100):m_BuyMoney);

	m_SellMoney = (DWORD)(price/3);

	float m_BaseDurability = (float)GetItemDurability(address,lpItemInfo,m_ItemLevel);

	m_SellMoney = ((lpItemInfo->Slot >= 0 && lpItemInfo->Slot <= 11) ? (m_SellMoney-(DWORD)((m_SellMoney*0.6)*(1-(*(BYTE*)(address + 22)/m_BaseDurability)))):m_SellMoney);

	m_SellMoney = ((m_SellMoney>=100)?((m_SellMoney/10)*10):m_SellMoney);

	m_SellMoney = ((m_SellMoney>=1000)?((m_SellMoney/100)*100):m_SellMoney);

	return ((type==1)?m_SellMoney:m_BuyMoney);
}

void ShowBuyPrice(char* TextOut,char* text,char* param) // OK
{
	if(ShopValueType==0)
	{
		wsprintf(TextOut,text,param);
	}
	else
	{
		wsprintf(TextOut,gCustomMessage.GetMessage(30),param);
	}
}

__declspec(naked) void ColorBuyPrice() // OK
{
	static DWORD ExitAddress1 = 0x005866E3;
	static DWORD ExitAddress2 = 0x005866E7;

	_asm
	{
		Mov Eax,Dword Ptr Ds:[0x07B553F4]
		Cmp ShopValueType,0
		Jnz Next
		Jmp[ExitAddress1]
	Next:
		Mov Ecx,8
		Jmp[ExitAddress2]
	}
}

void GCShopPriceListRecv(PMSG_SHOP_PRICE_LIST_RECV* lpMsg) // OK
{
	m_ShopInfo.clear();

	ShopValueType = lpMsg->type;

	for (int n = 0; n<lpMsg->count; n++)
	{
		PMSG_SHOP_PRICE_RECV* lpInfo = (PMSG_SHOP_PRICE_RECV*)(((BYTE*)lpMsg) + sizeof(PMSG_SHOP_PRICE_LIST_RECV) + (sizeof(PMSG_SHOP_PRICE_RECV)*n));

		SHOP_INFO info;

		info.money = lpInfo->money;

		m_ShopInfo.insert(std::pair<int,SHOP_INFO>(lpInfo->slot,info));
	}
}

void GCPcPointPriceListRecv(PMSG_PC_POINT_PRICE_LIST_RECV* lpMsg) // OK
{
	m_PcPointInfo.clear();

	for (int n = 0; n<lpMsg->count; n++)
	{
		PMSG_PC_POINT_PRICE_RECV* lpInfo = (PMSG_PC_POINT_PRICE_RECV*)(((BYTE*)lpMsg) + sizeof(PMSG_PC_POINT_PRICE_LIST_RECV) + (sizeof(PMSG_PC_POINT_PRICE_RECV)*n));

		SHOP_INFO info;

		info.money = lpInfo->money;

		m_PcPointInfo.insert(std::pair<int,SHOP_INFO>(lpInfo->slot,info));
	}

	ShopPcPoint = 1;
}

int ItemMaxStack(int index,int level) // OK
{
	for each(ITEM_STACK_INFO lpInfo in m_ItemStackInfo)
	{
		if(lpInfo.Index != index)
		{
			continue;
		}

		if(lpInfo.Level != 0xFF && lpInfo.Level != level)
		{
			continue;
		}

		return lpInfo.MaxStack;
	}

	return 0;
}

bool ItemValue(int index,int level,int grade,int dur,int* value) // OK
{
	for each(ITEM_VALUE_INFO lpInfo in m_ItemValueInfo)
	{
		if(lpInfo.Index == index)
		{
			if(lpInfo.Level == 0xFF || lpInfo.Level == level)
			{
				if(lpInfo.Grade == 0xFF || lpInfo.Grade == grade)
				{
					if(ItemMaxStack(lpInfo.Index,lpInfo.Level) == 0 || lpInfo.Index == GET_ITEM(4,7) || lpInfo.Index == GET_ITEM(4,15))
					{
						(*value) = lpInfo.Value;
					}
					else
					{
						(*value) = (int)(lpInfo.Value*dur);
					}

					return 1;
				}
			}
		}
	}

	return 0;
}

void GCItemStackListRecv(PMSG_ITEM_LIST_INFO_RECV* lpMsg) // OK
{
	m_ItemStackInfo.clear();

	for(int n=0;n < lpMsg->count;n++)
	{
		ITEM_STACK_INFO* lpInfo = (ITEM_STACK_INFO*)(((BYTE*)lpMsg)+sizeof(PMSG_ITEM_LIST_INFO_RECV)+(sizeof(ITEM_STACK_INFO)*n));

		ITEM_STACK_INFO info;

		info.Index = lpInfo->Index;

		info.Level = lpInfo->Level;

		info.MaxStack = lpInfo->MaxStack;

		m_ItemStackInfo.push_back(info);
	}
}

void GCItemValueListRecv(PMSG_ITEM_LIST_INFO_RECV* lpMsg) // OK
{
	m_ItemValueInfo.clear();

	for(int n=0;n < lpMsg->count;n++)
	{
		ITEM_VALUE_INFO* lpInfo = (ITEM_VALUE_INFO*)(((BYTE*)lpMsg)+sizeof(PMSG_ITEM_LIST_INFO_RECV)+(sizeof(ITEM_VALUE_INFO)*n));

		ITEM_VALUE_INFO info;

		info.Index = lpInfo->Index;

		info.Level = lpInfo->Level;

		info.Grade = lpInfo->Grade;

		info.Value = lpInfo->Value;

		m_ItemValueInfo.push_back(info);
	}
}