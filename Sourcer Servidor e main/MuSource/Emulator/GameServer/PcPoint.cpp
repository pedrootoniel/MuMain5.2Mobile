// PcPoint.cpp: implementation of the CPcPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PcPoint.h"
#include "BonusManager.h"
#include "EffectManager.h"
#include "GameMain.h"
#include "Map.h"
#include "ReadFile.h"
#include "ServerInfo.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPcPoint::CPcPoint() // OK
{
	#if(GAMESERVER_SHOP==1)

	this->Init();

	#endif
}

CPcPoint::~CPcPoint() // OK
{

}

void CPcPoint::Load(char* path) // OK
{
	#if(GAMESERVER_SHOP==1)

	CReadFile* lpReadFile = new CReadFile;

	if(lpReadFile == 0)
	{
		ErrorMessageBox(READ_FILE_ALLOC_ERROR,path);
		return;
	}

	if(lpReadFile->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpReadFile->GetLastError());
		delete lpReadFile;
		return;
	}

	this->Init();

	try
	{
		while(true)
		{
			if(lpReadFile->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpReadFile->GetString()) == 0)
			{
				break;
			}

			int ItemIndex,ItemLevel,ItemDurability,ItemOption1,ItemOption2,ItemOption3,ItemNewOption,ItemSet,ItemValue = 0;

			ItemIndex = SafeGetItem(GET_ITEM(lpReadFile->GetNumber(),lpReadFile->GetAsNumber()));

			ItemLevel = lpReadFile->GetAsNumber();

			ItemDurability = lpReadFile->GetAsNumber();

			ItemOption1 = lpReadFile->GetAsNumber();

			ItemOption2 = lpReadFile->GetAsNumber();

			ItemOption3 = lpReadFile->GetAsNumber();

			ItemNewOption = lpReadFile->GetAsNumber();

			ItemSet = lpReadFile->GetAsNumber();

			ItemValue = lpReadFile->GetAsNumber();

			this->InsertItem(ItemIndex,ItemLevel,ItemDurability,ItemOption1,ItemOption2,ItemOption3,ItemNewOption,ItemSet,0,ItemValue);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpReadFile->GetLastError());
	}

	delete lpReadFile;

	#endif
}

void CPcPoint::ReloadShopInterface() // OK
{
	#if(GAMESERVER_SHOP==1)

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			if(gObj[n].Interface.use != 0 && gObj[n].Interface.type == INTERFACE_PC_SHOP)
			{
				this->GCShopItemListSend(n);

				this->GCShopItemPriceSend(n);
			}
		}
	}

	#endif
}

void CPcPoint::CGPcPointItemBuyRecv(PMSG_PC_POINT_ITEM_BUY_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_SHOP==1)

	if(gServerInfo->m_PcPointSwitch == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(((lpObj->PcPointTransaction[1]==0)?(lpObj->PcPointTransaction[1]++):lpObj->PcPointTransaction[1]) != 0)
	{
		return;
	}

	SDHP_PC_POINT_ITEM_BUY_SEND pMsg;

	pMsg.header.set(0x19,0x01,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	pMsg.slot = lpMsg->slot;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CPcPoint::CGPcPointOpenRecv(PMSG_PC_POINT_OPEN_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_SHOP==1)

	if(gServerInfo->m_PcPointSwitch == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Permission[10] != 0)
	{
		return;
	}

	PMSG_PC_POINT_OPEN_SEND pMsg;

	pMsg.header.set(0xD0,0x06,sizeof(pMsg));

	pMsg.result = 0;

	if(lpObj->Interface.use != 0)
	{
		pMsg.result = 3;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(this->GetItemCount() == 0)
	{
		pMsg.result = 2;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) == 0)
	{
		pMsg.result = 1;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	lpObj->Interface.use = 1;

	lpObj->Interface.type = INTERFACE_PC_SHOP;

	lpObj->Interface.state = 0;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	this->GCShopItemListSend(aIndex);

	this->GCShopItemPriceSend(aIndex);

	#endif
}

void CPcPoint::DGPcPointPointRecv(SDHP_PC_POINT_POINT_RECV* lpMsg) // OK
{
	#if(GAMESERVER_SHOP==1)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGPcPointPointRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->PcPointTransaction[0] = 0;

	lpObj->Coin1 = lpMsg->PcPoint;
	
	lpObj->Coin2 = 0;
	
	lpObj->Coin3 = 0;

	PMSG_PC_POINT_POINT_SEND pMsg;

	pMsg.header.set(0xD0,0x04,sizeof(pMsg));

	pMsg.MinPcPoint = GET_MAX_WORD_VALUE(lpMsg->PcPoint);

	pMsg.MaxPcPoint = GET_MAX_WORD_VALUE(gServerInfo->m_PcPointMaxPoint);

	pMsg.PcPointType = 0;

	#if(GAMESERVER_EXTRA==1)

	pMsg.ViewCurPoint = lpMsg->PcPoint;

	pMsg.ViewMaxPoint = gServerInfo->m_PcPointMaxPoint;

	#endif

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CPcPoint::DGPcPointItemBuyRecv(SDHP_PC_POINT_ITEM_BUY_RECV* lpMsg) // OK
{
	#if(GAMESERVER_SHOP==1)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGPcPointItemBuyRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->PcPointTransaction[1] = 0;

	PMSG_PC_POINT_ITEM_BUY_SEND pMsg;

	pMsg.header.set(0xD0,0x05,sizeof(pMsg));

	pMsg.result = 0;

	pMsg.ItemSlot = 0xFF;

	if(lpMsg->result != 0)
	{
		pMsg.result = 7;
		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_PC_SHOP)
	{
		pMsg.result = 5;
		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	gObjFixInventoryPointer(lpObj->Index);

	if(lpObj->Transaction == 1)
	{
		pMsg.result = 6;
		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	CItem item;

	if(this->GetItem(&item,lpMsg->slot) == 0)
	{
		pMsg.result = 8;
		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpMsg->PcPoint < ((DWORD)item.m_PcPointValue))
	{
		pMsg.result = 9;
		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	EFFECT_INFO* lpInfo = gEffectManager->GetInfoByItem(item.m_Index);

	if(lpInfo == 0)
	{
		if(gItemManager->InventoryInsertItemStack(lpObj,&item) == 0)
		{
			if((pMsg.ItemSlot=gItemManager->InventoryInsertItem(lpObj->Index,item)) == 0xFF)
			{
				pMsg.result = 3;
				DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
				return;
			}

			this->GDPcPointSubPointSaveSend(lpObj->Index,item.m_PcPointValue);

			gItemManager->ItemByteConvert(pMsg.ItemInfo,item);

			DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
		}
		else
		{
			this->GDPcPointSubPointSaveSend(lpObj->Index,item.m_PcPointValue);

			DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
		}
	}
	else
	{
		if(gEffectManager->GetEffect(lpObj,lpInfo->Index) != 0)
		{
			pMsg.result = 3;
			DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
			return;
		}

		gEffectManager->AddEffect(lpObj,0,lpInfo->Index,0,0,0,0,0);

		this->GDPcPointSubPointSaveSend(lpObj->Index,item.m_PcPointValue);

		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
	}

	#endif
}

void CPcPoint::DGPcPointRecievePointRecv(SDHP_PC_POINT_RECIEVE_POINT_RECV* lpMsg) // OK
{
	#if(GAMESERVER_SHOP==1)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGPcPointRecievePointRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	((void(*)(LPOBJ,DWORD,DWORD,DWORD,DWORD,DWORD))lpMsg->CallbackFunc)(lpObj,lpMsg->CallbackArg1,lpMsg->CallbackArg2,0,0,lpMsg->PcPoint);

	#endif
}

void CPcPoint::GDPcPointPointSend(int aIndex) // OK
{
	#if(GAMESERVER_SHOP==1)

	if(gServerInfo->m_PcPointSwitch == 0)
	{
		return;
	}

	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	if(((gObj[aIndex].PcPointTransaction[0]==0)?(gObj[aIndex].PcPointTransaction[0]++):gObj[aIndex].PcPointTransaction[0]) != 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->Coin1 = 0;

	lpObj->Coin2 = 0;

	lpObj->Coin3 = 0;

	SDHP_PC_POINT_POINT_SEND pMsg;

	pMsg.header.set(0x19,0x00,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CPcPoint::GDPcPointRecievePointSend(int aIndex,DWORD CallbackFunc,DWORD CallbackArg1,DWORD CallbackArg2) // OK
{
	#if(GAMESERVER_SHOP==1)

	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	SDHP_PC_POINT_RECIEVE_POINT_SEND pMsg;

	pMsg.header.set(0x19,0x02,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	pMsg.CallbackFunc = CallbackFunc;

	pMsg.CallbackArg1 = CallbackArg1;

	pMsg.CallbackArg2 = CallbackArg2;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CPcPoint::GDPcPointAddPointSaveSend(int aIndex,DWORD AddPcPoint) // OK
{
	#if(GAMESERVER_SHOP==1)

	SDHP_PC_POINT_ADD_POINT_SAVE_SEND pMsg;

	pMsg.header.set(0x19,0x30,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	pMsg.AddPcPoint = AddPcPoint;

	pMsg.MaxPcPoint = gServerInfo->m_PcPointMaxPoint;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));

	this->GDPcPointPointSend(aIndex);

	#endif
}

void CPcPoint::GDPcPointSubPointSaveSend(int aIndex,DWORD SubPcPoint) // OK
{
	#if(GAMESERVER_SHOP==1)

	SDHP_PC_POINT_SUB_POINT_SAVE_SEND pMsg;

	pMsg.header.set(0x19,0x31,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	pMsg.SubPcPoint = SubPcPoint;

	pMsg.MaxPcPoint = gServerInfo->m_PcPointMaxPoint;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));

	this->GDPcPointPointSend(aIndex);

	#endif
}