// CustomAttack.cpp: implementation of the CCustomAttack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomAttack.h"
#include "CommandManager.h"
#include "EffectManager.h"
#include "GameMaster.h"
#include "Map.h"
#include "MapManager.h"
#include "MasterSkillTree.h"
#include "ReadFile.h"
#include "Message.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "Party.h"
#include "Reconnect.h"
#include "SkillManager.h"
#include "SocketManager.h"
#include "Viewport.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomAttack::CCustomAttack() // OK
{
	memset(this->m_CustomAttackMapList,0,sizeof(this->m_CustomAttackMapList));

	this->m_CustomAttackDelayInfo.clear();

	this->m_CustomAttackSkillInfo.clear();
}

CCustomAttack::~CCustomAttack() // OK
{

}

void CCustomAttack::MainProc(LPOBJ lpObj) // OK
{
	if(lpObj->Attack.Started == 0)
	{
		return;
	}

	if(lpObj->Attack.Offline != 0)
	{
		if(lpObj->Attack.AutoBuff == 0 && lpObj->Attack.SkillIndex == 0)
		{
			this->AttackOfflineClose(lpObj);
			return;
		}

		if(this->m_CustomAttackOfflineMaxTimeLimit[lpObj->AccountLevel] > 0)
		{
			if(((DWORD)time(0)) >= ((DWORD)lpObj->Attack.OnlineTime+this->m_CustomAttackOfflineMaxTimeLimit[lpObj->AccountLevel]))
			{
				this->AttackOfflineClose(lpObj);
				return;
			}
		}

		if(this->m_CustomAttackOfflineKeepEnable[lpObj->AccountLevel] == 0)
		{
			if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
			{
				this->AttackOfflineClose(lpObj);
				return;
			}

			if(lpObj->SkillSummonPartyTime > 0 && gViewport->CheckViewportObjectPosition(lpObj->Index,lpObj->Attack.StartMap,lpObj->Attack.StartX,lpObj->Attack.StartY,10) == 0)
			{
				this->AttackOfflineClose(lpObj);
				return;
			}

			if((this->m_CustomAttackMapZone != -1 && this->m_CustomAttackMapList[lpObj->Map] == 0) || gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
			{
				this->AttackOfflineClose(lpObj);
				return;
			}
		}
	}
	else
	{
		if(lpObj->Attack.AutoBuff == 0 && lpObj->Attack.SkillIndex == 0)
		{
			this->CustomAttackClose(lpObj->Index);
			return;
		}

		if(this->m_CustomAttackMaxTimeLimit[lpObj->AccountLevel] > 0)
		{
			if(((DWORD)time(0)) >= ((DWORD)lpObj->Attack.OnlineTime+this->m_CustomAttackMaxTimeLimit[lpObj->AccountLevel]))
			{
				GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(578));
				lpObj->Attack.Started = 0;
				this->CustomAttackSend(lpObj->Index);
				return;
			}
		}

		if((this->m_CustomAttackMapZone != -1 && this->m_CustomAttackMapList[lpObj->Map] == 0) || gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
		{
			this->CustomAttackClose(lpObj->Index);
			return;
		}

		if(gViewport->CheckViewportObjectPosition(lpObj->Index,lpObj->Attack.StartMap,lpObj->Attack.StartX,lpObj->Attack.StartY,5) == 0)
		{
			this->CustomAttackClose(lpObj->Index);
			return;
		}
	}

	this->CustomAttackAutoPotion(lpObj);

	this->CustomAttackAutoRepair(lpObj);

	this->CustomAttackAutoReload(lpObj);

	this->CustomAttackAutoBuff(lpObj);

	this->CustomAttackAutoAttack(lpObj);
}

void CCustomAttack::SecondProc(LPOBJ lpObj) // OK
{
	if(lpObj->Attack.Offline != 0)
	{
		lpObj->ConnectTickCount = GetTickCount();

		lpObj->OnlineRewardTime = ((this->m_CustomAttackOfflineCoinGain[lpObj->AccountLevel]==0)?GetTickCount():lpObj->OnlineRewardTime);
	}
}

void CCustomAttack::AttackOfflineClose(LPOBJ lpObj) // OK
{
	if(lpObj->Attack.Offline != 0)
	{
		gReconnect->GDReconnectInfoRemoveSend(lpObj->Name);
		gObjDel(lpObj->Index);
		lpObj->Attack.Started = 0;
		lpObj->Attack.Offline = 0;
	}
}

void CCustomAttack::ReadCustomAttackInfo(char* section,char* path) // OK
{
	this->m_CustomAttackSwitch = GetPrivateProfileInt(section,"CustomAttackSwitch",0,path);

	this->m_CustomAttackMapZone = GetPrivateProfileInt(section,"CustomAttackMapZone",0,path);

	if(this->m_CustomAttackMapZone != -1)
	{
		memset(this->m_CustomAttackMapList,0,sizeof(this->m_CustomAttackMapList));

		char buff[256] = {0};

		GetPrivateProfileString(section,"CustomAttackMapZone","",buff,sizeof(buff),path);

		char* next_token;
		char* token = strtok_s(buff,",",&next_token);
	
		while(token != 0)
		{
			if(gMapManager->IsValidMap(atoi(token)) != 0)
			{
				this->m_CustomAttackMapList[atoi(token)] = 1;
			}

			token = strtok_s(0,",",&next_token);
		}
	}

	this->m_CustomAttackBuffEnable[0] = GetPrivateProfileInt(section,"CustomAttackBuffEnable_AL0",0,path);
	this->m_CustomAttackBuffEnable[1] = GetPrivateProfileInt(section,"CustomAttackBuffEnable_AL1",0,path);
	this->m_CustomAttackBuffEnable[2] = GetPrivateProfileInt(section,"CustomAttackBuffEnable_AL2",0,path);
	this->m_CustomAttackBuffEnable[3] = GetPrivateProfileInt(section,"CustomAttackBuffEnable_AL3",0,path);

	this->m_CustomAttackMaxTimeLimit[0] = GetPrivateProfileInt(section,"CustomAttackMaxTimeLimit_AL0",0,path);
	this->m_CustomAttackMaxTimeLimit[1] = GetPrivateProfileInt(section,"CustomAttackMaxTimeLimit_AL1",0,path);
	this->m_CustomAttackMaxTimeLimit[2] = GetPrivateProfileInt(section,"CustomAttackMaxTimeLimit_AL2",0,path);
	this->m_CustomAttackMaxTimeLimit[3] = GetPrivateProfileInt(section,"CustomAttackMaxTimeLimit_AL3",0,path);

	this->m_CustomAttackPotionEnable[0] = GetPrivateProfileInt(section,"CustomAttackPotion_AL0",0,path);
	this->m_CustomAttackPotionEnable[1] = GetPrivateProfileInt(section,"CustomAttackPotion_AL1",0,path);
	this->m_CustomAttackPotionEnable[2] = GetPrivateProfileInt(section,"CustomAttackPotion_AL2",0,path);
	this->m_CustomAttackPotionEnable[3] = GetPrivateProfileInt(section,"CustomAttackPotion_AL3",0,path);

	this->m_CustomAttackPotionDelay[0] = GetPrivateProfileInt(section,"CustomAttackPotionDelay_AL0",0,path);
	this->m_CustomAttackPotionDelay[1] = GetPrivateProfileInt(section,"CustomAttackPotionDelay_AL1",0,path);
	this->m_CustomAttackPotionDelay[2] = GetPrivateProfileInt(section,"CustomAttackPotionDelay_AL2",0,path);
	this->m_CustomAttackPotionDelay[3] = GetPrivateProfileInt(section,"CustomAttackPotionDelay_AL3",0,path);

	this->m_CustomAttackPotionRate1 = GetPrivateProfileInt(section,"CustomAttackPotionRate1",0,path);
	this->m_CustomAttackPotionRate2 = GetPrivateProfileInt(section,"CustomAttackPotionRate2",0,path);

	this->m_CustomAttackRepair[0] = GetPrivateProfileInt(section, "CustomAttackRepair_AL0", 0, path);
	this->m_CustomAttackRepair[1] = GetPrivateProfileInt(section, "CustomAttackRepair_AL1", 0, path);
	this->m_CustomAttackRepair[2] = GetPrivateProfileInt(section, "CustomAttackRepair_AL2", 0, path);
	this->m_CustomAttackRepair[3] = GetPrivateProfileInt(section, "CustomAttackRepair_AL3", 0, path);

	this->m_CustomAttackOfflineSwitch = GetPrivateProfileInt(section,"CustomAttackOfflineSwitch",0,path);

	#if(GAMESERVER_SHOP>=1)

	this->m_CustomAttackOfflineCoinGain[0] = GetPrivateProfileInt(section,"CustomAttackOfflineCoinGain_AL0",0,path);
	this->m_CustomAttackOfflineCoinGain[1] = GetPrivateProfileInt(section,"CustomAttackOfflineCoinGain_AL1",0,path);
	this->m_CustomAttackOfflineCoinGain[2] = GetPrivateProfileInt(section,"CustomAttackOfflineCoinGain_AL2",0,path);
	this->m_CustomAttackOfflineCoinGain[3] = GetPrivateProfileInt(section,"CustomAttackOfflineCoinGain_AL3",0,path);

	#endif

	this->m_CustomAttackOfflineBuffEnable[0] = GetPrivateProfileInt(section,"CustomAttackOfflineBuffEnable_AL0",0,path);
	this->m_CustomAttackOfflineBuffEnable[1] = GetPrivateProfileInt(section,"CustomAttackOfflineBuffEnable_AL1",0,path);
	this->m_CustomAttackOfflineBuffEnable[2] = GetPrivateProfileInt(section,"CustomAttackOfflineBuffEnable_AL2",0,path);
	this->m_CustomAttackOfflineBuffEnable[3] = GetPrivateProfileInt(section,"CustomAttackOfflineBuffEnable_AL3",0,path);

	this->m_CustomAttackOfflineKeepEnable[0] = GetPrivateProfileInt(section,"CustomAttackOfflineKeepEnable_AL0",0,path);
	this->m_CustomAttackOfflineKeepEnable[1] = GetPrivateProfileInt(section,"CustomAttackOfflineKeepEnable_AL1",0,path);
	this->m_CustomAttackOfflineKeepEnable[2] = GetPrivateProfileInt(section,"CustomAttackOfflineKeepEnable_AL2",0,path);
	this->m_CustomAttackOfflineKeepEnable[3] = GetPrivateProfileInt(section,"CustomAttackOfflineKeepEnable_AL3",0,path);

	this->m_CustomAttackOfflineMaxTimeLimit[0] = GetPrivateProfileInt(section,"CustomAttackOfflineMaxTimeLimit_AL0",0,path);
	this->m_CustomAttackOfflineMaxTimeLimit[1] = GetPrivateProfileInt(section,"CustomAttackOfflineMaxTimeLimit_AL1",0,path);
	this->m_CustomAttackOfflineMaxTimeLimit[2] = GetPrivateProfileInt(section,"CustomAttackOfflineMaxTimeLimit_AL2",0,path);
	this->m_CustomAttackOfflineMaxTimeLimit[3] = GetPrivateProfileInt(section,"CustomAttackOfflineMaxTimeLimit_AL3",0,path);
}

void CCustomAttack::Load(char* path) // OK
{
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

	this->m_CustomAttackDelayInfo.clear();

	this->m_CustomAttackSkillInfo.clear();

	try
	{
		while(true)
		{
			if(lpReadFile->GetToken() == TOKEN_END)
			{
				break;
			}
		
			int section = lpReadFile->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpReadFile->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_ATTACK_DELAY_INFO info;

					info.Index = lpReadFile->GetNumber();
					
					info.MinDexterity = lpReadFile->GetAsNumber();
					
					info.MaxDexterity = lpReadFile->GetAsNumber();
					
					info.MinCharSpeed = lpReadFile->GetAsNumber();
					
					info.MaxCharSpeed = lpReadFile->GetAsNumber();
					
					info.AttackDelay = lpReadFile->GetAsNumber();

					info.BaseSpeed = lpReadFile->GetAsNumber();

					info.MainSpeed = lpReadFile->GetAsNumber();

					this->m_CustomAttackDelayInfo.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpReadFile->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_ATTACK_SKILL_INFO info;

					info.Index = lpReadFile->GetNumber();

					info.Group = lpReadFile->GetAsNumber();

					info.Value = lpReadFile->GetAsNumber();

					for(int n=0;n < MAX_CLASS;n++){info.RequireClass[n] = lpReadFile->GetAsNumber();}

					this->SetInfo(info);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpReadFile->GetLastError());
	}

	delete lpReadFile;
}

void CCustomAttack::SetInfo(CUSTOM_ATTACK_SKILL_INFO info) // OK
{
	if(CHECK_RANGE(info.Index,MAX_SKILL) == 0)
	{
		return;
	}

	std::map<int,CUSTOM_ATTACK_SKILL_INFO>::iterator it = this->m_CustomAttackSkillInfo.find(info.Index);

	if(it == this->m_CustomAttackSkillInfo.end())
	{
		this->m_CustomAttackSkillInfo.insert(std::pair<int,CUSTOM_ATTACK_SKILL_INFO>(info.Index,info));
	}
}

DWORD CCustomAttack::GetAttackDelay(LPOBJ lpObj,int skill) // OK
{
	int CharSpeed = ((lpObj->Class==CLASS_DW||lpObj->Class==CLASS_SU)?lpObj->MagicSpeed:lpObj->PhysiSpeed);

	for(std::vector<CUSTOM_ATTACK_DELAY_INFO>::iterator it=this->m_CustomAttackDelayInfo.begin(); it != this->m_CustomAttackDelayInfo.end(); it++)
	{
		if(it->Index != -1 && it->Index != skill)
		{
			continue;
		}

		if(it->MinDexterity != -1 && it->MinDexterity > lpObj->Dexterity)
		{
			continue;
		}

		if(it->MaxDexterity != -1 && it->MaxDexterity < lpObj->Dexterity)
		{
			continue;
		}

		if(it->MinCharSpeed != -1 && it->MinCharSpeed > CharSpeed)
		{
			continue;
		}

		if(it->MaxCharSpeed != -1 && it->MaxCharSpeed < CharSpeed)
		{
			continue;
		}

		return (it->AttackDelay==-1)?(it->BaseSpeed-(CharSpeed*it->MainSpeed)):it->AttackDelay;
	}

	return (1500-(CharSpeed*4));
}

CUSTOM_ATTACK_SKILL_INFO* CCustomAttack::GetInfo(int Class,int skill,bool helper) // OK
{
	std::map<int,CUSTOM_ATTACK_SKILL_INFO>::iterator it = this->m_CustomAttackSkillInfo.find(skill);

	if(it == this->m_CustomAttackSkillInfo.end())
	{
		return 0;
	}

	if(helper == 0)
	{
		if(it->second.Group == 0)
		{
			return 0;
		}
	}

	if(it->second.RequireClass[Class] == 0)
	{
		return 0;
	}

	return &it->second;
}

int CCustomAttack::GetSkilLAuto(LPOBJ lpObj) // OK
{
	for(std::map<int,CUSTOM_ATTACK_SKILL_INFO>::iterator it=this->m_CustomAttackSkillInfo.begin();it != this->m_CustomAttackSkillInfo.end();it++)
	{
		if(it->second.Group == 0 || it->second.RequireClass[lpObj->Class] == 0)
		{
			return 0;
		}

		CSkill* lpSkill = this->CustomAttackFindSkill(lpObj,it->second.Index);

		if(lpSkill == 0)
		{
			continue;
		}

		return it->second.Index;
	}

	return 0;
}

void CCustomAttack::CustomAttackSend(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_CUSTOM_ATTACK_STATUS_SEND pMsg;

	pMsg.header.set(0xF3,0xEB,sizeof(pMsg));

	pMsg.Started = lpObj->Attack.Started;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
}

void CCustomAttack::CustomAttackClose(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Attack.Started == 0)
	{
		return;
	}

	GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(572));

	lpObj->Attack.Started = 0;

	this->CustomAttackSend(lpObj->Index);
}

void CCustomAttack::CommandCustomAttack(LPOBJ lpObj,char* arg) // OK
{
	if(this->m_CustomAttackSwitch == 0)
	{
		return;
	}
	
	if(lpObj->Helper.Started != 0)
	{
		GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(573));
		return;
	}

	if((this->m_CustomAttackMapZone != -1 && this->m_CustomAttackMapList[lpObj->Map] == 0) || gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
	{
		GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(574));
		return;
	}

	if(lpObj->Attack.Started != 0)
	{
		lpObj->Attack.Started = 0;
	}
	else
	{
		if(strcmp(arg,"") != 0)
		{
			lpObj->Attack.AutoBuff = gCommandManager->GetNumber(arg,0);

			lpObj->Attack.SkillIndex = gCommandManager->GetNumber(arg,1);
		}
		else
		{
			lpObj->Attack.AutoBuff = this->m_CustomAttackBuffEnable[lpObj->AccountLevel];

			lpObj->Attack.SkillIndex = this->GetSkilLAuto(lpObj);
		}

		if(lpObj->Attack.AutoBuff == 0 && lpObj->Attack.SkillIndex == 0)
		{
			GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(575));
			return;
		}

		if(lpObj->Attack.SkillIndex != 0)
		{
			if(this->GetInfo(lpObj->Class,lpObj->Attack.SkillIndex,0) == 0)
			{
				GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(575));
				return;
			}

			if(this->CustomAttackFindSkill(lpObj,lpObj->Attack.SkillIndex) == 0)
			{
				GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(575));
				return;
			}

			if(gSkillManager->CheckSkillRequireWeapon(lpObj,lpObj->Attack.SkillIndex) == 0)
			{
				GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(575));
				return;
			}

			if(lpObj->Attack.SkillIndex == SKILL_TWISTING_SLASH || lpObj->Attack.SkillIndex == SKILL_RAGEFUL_BLOW || lpObj->Attack.SkillIndex == SKILL_DEATH_STAB)
			{
				if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].IsItem() == 0)
				{
					GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(575));
					return;
				}
			}

			if(lpObj->Attack.SkillIndex == SKILL_TRIPLE_SHOT || lpObj->Attack.SkillIndex == SKILL_ICE_ARROW || lpObj->Attack.SkillIndex == SKILL_PENETRATION)
			{
				if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].IsItem() == 0 || lpObj->Inventory[INVENTORY_SLOT_WEAPON2].IsItem() == 0)
				{
					GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(575));
					return;
				}

				if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(4,0) || lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(5,0))
				{
					GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(575));
					return;
				}

				if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index < GET_ITEM(4,0) || lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index >= GET_ITEM(5,0))
				{
					GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(575));
					return;
				}
			}
		}

		lpObj->Attack.Started = 1;

		lpObj->Attack.StartMap = lpObj->Map;

		lpObj->Attack.StartX = (BYTE)lpObj->X;

		lpObj->Attack.StartY = (BYTE)lpObj->Y;

		lpObj->Attack.AttackTime = GetTickCount();

		lpObj->Attack.PotionTime = GetTickCount();

		lpObj->Attack.OnlineTime = ((DWORD)time(0));

		lpObj->Attack.TargetIndex = -1;
	}

	this->CustomAttackSend(lpObj->Index);
}

void CCustomAttack::CommandCustomAttackOffline(LPOBJ lpObj) // OK
{
	if(this->m_CustomAttackOfflineSwitch == 0)
	{
		return;
	}

	if(lpObj->Attack.Started == 0)
	{
		GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(577));
		return;
	}

	lpObj->Socket = INVALID_SOCKET;

	lpObj->Attack.Offline = 1;

	lpObj->Attack.OnlineTime = ((DWORD)time(0));

	closesocket(lpObj->PerSocketContext->Socket);

	gReconnect->SetReconnectInfo(lpObj);
}

void CCustomAttack::CustomAttackAutoPotion(LPOBJ lpObj) // OK
{
	if(this->m_CustomAttackPotionEnable[lpObj->AccountLevel] == 0)
	{
		return;
	}

	if(GetTickCount() < lpObj->Attack.PotionTime)
	{
		return;
	}

	PMSG_ITEM_USE_RECV pMsg;

	pMsg.header.set(0x26,sizeof(pMsg));

	pMsg.SourceSlot = 0xFF;

	pMsg.TargetSlot = 0xFF;

	pMsg.type = 0;

	for(int n=0;n < 10;n++)
	{
		if(((lpObj->Life*100)/(lpObj->MaxLife+lpObj->AddLife)) < this->m_CustomAttackPotionRate1)
		{
			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager->GetInventoryItemSlot(lpObj,GET_ITEM(14,3),-1):pMsg.SourceSlot);

			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager->GetInventoryItemSlot(lpObj,GET_ITEM(14,2),-1):pMsg.SourceSlot);

			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager->GetInventoryItemSlot(lpObj,GET_ITEM(14,1),-1):pMsg.SourceSlot);

			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager->GetInventoryItemSlot(lpObj,GET_ITEM(14,0),-1):pMsg.SourceSlot);

			if(INVENTORY_FULL_RANGE(pMsg.SourceSlot) != 0)
			{
				gItemManager->CGItemUseRecv(&pMsg,lpObj->Index);

				lpObj->Attack.PotionTime = GetTickCount()+this->m_CustomAttackPotionDelay[lpObj->AccountLevel];
			}
		}

		if(((lpObj->Mana*100)/(lpObj->MaxMana+lpObj->AddMana)) < this->m_CustomAttackPotionRate2)
		{
			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager->GetInventoryItemSlot(lpObj,GET_ITEM(14,6),-1):pMsg.SourceSlot);

			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager->GetInventoryItemSlot(lpObj,GET_ITEM(14,5),-1):pMsg.SourceSlot);

			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager->GetInventoryItemSlot(lpObj,GET_ITEM(14,4),-1):pMsg.SourceSlot);

			if(INVENTORY_FULL_RANGE(pMsg.SourceSlot) != 0)
			{
				gItemManager->CGItemUseRecv(&pMsg,lpObj->Index);

				lpObj->Attack.PotionTime = GetTickCount()+this->m_CustomAttackPotionDelay[lpObj->AccountLevel];
			}
		}
	}
}

void CCustomAttack::CustomAttackAutoRepair(LPOBJ lpObj) // OK
{
	if(this->m_CustomAttackRepair[lpObj->AccountLevel] == 0)
	{
		return;
	}

	PMSG_ITEM_REPAIR_SEND pMsg;

	pMsg.header.set(0x34,sizeof(pMsg));

	pMsg.money = 0;

	for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
	{
		if(lpObj->Inventory[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->Inventory[n].m_Durability >= lpObj->Inventory[n].m_BaseDurability)
		{
			continue;
		}

		if(gItemManager->RepairItem(lpObj,&lpObj->Inventory[n],n,0,&pMsg.money) != 0)
		{
			DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
			
			gObjectManager->CharacterCalcAttribute(lpObj->Index);
		}
	}
}

void CCustomAttack::CustomAttackAutoReload(LPOBJ lpObj) // OK
{
	if(gEffectManager->CheckEffect(lpObj,EFFECT_INFINITY_ARROW) != 0 || gEffectManager->CheckEffect(lpObj,EFFECT_INFINITY_ARROW_IMPROVED) != 0)
	{
		return;
	}

	if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(4,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index != GET_ITEM(4,15) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Slot == 0)
	{
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index != GET_ITEM(4,7) || lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Durability < 1)
		{
			if(gItemManager->GetInventoryItemCount(lpObj,GET_ITEM(4,7),-1) > 0)
			{
				PMSG_ITEM_MOVE_RECV pMsg;

				pMsg.header.set(0x24,sizeof(pMsg));

				pMsg.SourceFlag = 0;

				pMsg.SourceSlot = gItemManager->GetInventoryItemSlot(lpObj,GET_ITEM(4,7),-1);

				pMsg.TargetFlag = 0;

				pMsg.TargetSlot = 1;

				if(INVENTORY_FULL_RANGE(pMsg.SourceSlot) != 0) 
				{
					gItemManager->CGItemMoveRecv(&pMsg,lpObj->Index);
					gItemManager->InventoryDelItem(lpObj->Index,pMsg.SourceSlot);
					gItemManager->GCItemDeleteSend(lpObj->Index,pMsg.SourceSlot,1);
				}
			}
		}
	}

	if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index >= GET_ITEM(4,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index < GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index != GET_ITEM(4,7) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Slot == 1)
	{
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index != GET_ITEM(4,15) || lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Durability < 1)
		{
			if(gItemManager->GetInventoryItemCount(lpObj,GET_ITEM(4,15),-1) > 0)
			{
				PMSG_ITEM_MOVE_RECV pMsg;

				pMsg.header.set(0x24,sizeof(pMsg));

				pMsg.SourceFlag = 0;

				pMsg.SourceSlot = gItemManager->GetInventoryItemSlot(lpObj,GET_ITEM(4,15),-1);

				pMsg.TargetFlag = 0;

				pMsg.TargetSlot = 0;

				if(INVENTORY_FULL_RANGE(pMsg.SourceSlot) != 0)
				{ 
					gItemManager->CGItemMoveRecv(&pMsg,lpObj->Index);
					gItemManager->InventoryDelItem(lpObj->Index,pMsg.SourceSlot);
					gItemManager->GCItemDeleteSend(lpObj->Index,pMsg.SourceSlot,1);
				}
			}
		}
	}
}

void CCustomAttack::CustomAttackAutoBuff(LPOBJ lpObj) // OK
{
	if(lpObj->Attack.Offline == 0 && this->m_CustomAttackBuffEnable[lpObj->AccountLevel] == 0)
	{
		return;
	}

	if(lpObj->Attack.Offline != 0 && this->m_CustomAttackOfflineBuffEnable[lpObj->AccountLevel] == 0)
	{
		return;
	}

	if(lpObj->Attack.AutoBuff == 0)
	{ 
		return;
	}

	int BuffCount = 0;

	for(std::map<int,CUSTOM_ATTACK_SKILL_INFO>::iterator it=this->m_CustomAttackSkillInfo.begin();it != this->m_CustomAttackSkillInfo.end();it++)
	{
		if(it->second.Group != 0)
		{
			continue;
		}

		if(it->second.RequireClass[lpObj->Class] == 0)
		{
			continue;
		}
		
		CSkill* lpSkill = gSkillManager->GetSkill(lpObj,it->second.Index);

		if(lpSkill == 0)
		{
			continue;
		}

		BuffCount++;

		if(gSkillManager->CheckSkillMana(lpObj,lpSkill->m_index) == 0)
		{
			continue;
		}

		if(gSkillManager->CheckSkillBP(lpObj,lpSkill->m_index) == 0)
		{
			continue;
		}

		if(it->second.Value == 0)
		{
			if(gEffectManager->CheckEffect(lpObj,gSkillManager->GetSkillEffect(lpSkill->m_index)) == 0)
			{
				this->CustomAttackUseSkill(lpObj,lpObj,&it->second,lpSkill);
				continue;
			}
		}
		else
		{
			if(OBJECT_RANGE(lpObj->PartyNumber) == 0)
			{
				if(gEffectManager->CheckEffect(lpObj,gSkillManager->GetSkillEffect(lpSkill->m_index)) == 0)
				{
					this->CustomAttackUseSkill(lpObj,lpObj,&it->second,lpSkill);
					continue;
				}
			}
			else
			{
				for(int n=0;n < MAX_PARTY_USER;n++)
				{
					int bIndex = gParty->m_PartyInfo[lpObj->PartyNumber].Index[n];

					if(OBJECT_RANGE(bIndex) == 0)
					{
						continue;
					}

					LPOBJ lpTarget = &gObj[bIndex];

					if(lpTarget->Live == 0 || lpTarget->State != OBJECT_PLAYING || lpTarget->Map != lpObj->Map)
					{
						continue;
					}

					if(gObjCalcDistance(lpObj,lpTarget) > gSkillManager->GetSkillRange(lpSkill->m_skill))
					{
						continue;
					}

					if(gEffectManager->CheckEffect(lpTarget,gSkillManager->GetSkillEffect(lpSkill->m_index)) == 0)
					{
						this->CustomAttackUseSkill(lpObj,lpTarget,&it->second,lpSkill);
						continue;
					}
				}
			}
		}
	}

	if(BuffCount == 0)
	{
		lpObj->Attack.AutoBuff = 0;
	}
}

void CCustomAttack::CustomAttackAutoAttack(LPOBJ lpObj) // OK
{
	if(lpObj->Attack.SkillIndex == 0)
	{
		return;
	}

	CUSTOM_ATTACK_SKILL_INFO* lpInfo = this->GetInfo(lpObj->Class,lpObj->Attack.SkillIndex,0);

	if(lpInfo == 0)
	{
		lpObj->Attack.SkillIndex = 0;
		return;
	}

	CSkill* lpSkill = this->CustomAttackFindSkill(lpObj,lpObj->Attack.SkillIndex);

	if(lpSkill == 0)
	{
		lpObj->Attack.SkillIndex = 0;
		return;
	}

	if(GetTickCount() < lpObj->Attack.AttackTime)
	{
		return;
	}

	if(gSkillManager->CheckSkillMana(lpObj,lpSkill->m_index) == 0)
	{
		return;
	}

	if(gSkillManager->CheckSkillBP(lpObj,lpSkill->m_index) == 0)
	{
		return;
	}

	if(this->CustomAttackFindTarget(lpObj,lpSkill->m_skill) != 0)
	{
		this->CustomAttackUseSkill(lpObj,&gObj[lpObj->Attack.TargetIndex],lpInfo,lpSkill);
	}
}

int CCustomAttack::CustomAttackFindTarget(LPOBJ lpObj,int skill) // OK
{
	int NearestDistance = 100;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0 || lpObj->VpPlayer2[n].type != OBJECT_MONSTER)
		{
			continue;
		}

		if(gSkillManager->CheckSkillTarget(lpObj,lpObj->VpPlayer2[n].index,-1,lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		int DistanceDifference = gObjCalcDistance(lpObj,&gObj[lpObj->VpPlayer2[n].index]);

		if(DistanceDifference >= NearestDistance)
		{
			continue;
		}

		if(gSkillManager->CheckSkillRange(lpObj,skill,lpObj->X,lpObj->Y,gObj[lpObj->VpPlayer2[n].index].X,gObj[lpObj->VpPlayer2[n].index].Y) != 0)
		{
			lpObj->Attack.TargetIndex = lpObj->VpPlayer2[n].index;
			NearestDistance = DistanceDifference;
			continue;
		}

		if(gSkillManager->CheckSkillRadio(skill,lpObj->X,lpObj->Y,gObj[lpObj->VpPlayer2[n].index].X,gObj[lpObj->VpPlayer2[n].index].Y) != 0)
		{
			lpObj->Attack.TargetIndex = lpObj->VpPlayer2[n].index;
			NearestDistance = DistanceDifference;
			continue;
		}
	}

	return ((NearestDistance==100)?0:1);
}

CSkill* CCustomAttack::CustomAttackFindSkill(LPOBJ lpObj,int skill) // OK
{
	CSkill* lpSkill = gSkillManager->GetMasterSkill(lpObj,skill);

	if(lpSkill != 0)
	{
		return lpSkill;
	}

	lpSkill = gSkillManager->GetSkill(lpObj,skill);

	if(lpSkill != 0)
	{
		return lpSkill;
	}

	return 0;
}

void CCustomAttack::CustomAttackUseSkill(LPOBJ lpObj,LPOBJ lpTarget,CUSTOM_ATTACK_SKILL_INFO* lpInfo,CSkill* lpSkill) // OK
{
	if(lpInfo == 0 || lpSkill == 0)
	{
		return;
	}

	if(lpSkill->m_skill == SKILL_HEAL && ((lpTarget->Life*100)/(lpTarget->MaxLife+lpTarget->AddLife)) > 50)
	{
		return;
	}

	if(GetTickCount() < lpObj->Attack.AttackTime)
	{
		return;
	}

	if(gSkillManager->CheckSkillMana(lpObj,lpSkill->m_index) == 0)
	{
		return;
	}

	if(gSkillManager->CheckSkillBP(lpObj,lpSkill->m_index) == 0)
	{
		return;
	}

	switch(lpInfo->Group)
	{
		case 2:
			this->CustomAttackMultilAttack(lpObj,lpTarget->Index,lpInfo->Index);
			break;
		case 3:
			this->CustomAttackDurationlAttack(lpObj,lpTarget->Index,lpInfo->Index);
			break;
		case 4:
			this->CustomAttackRageAttack(lpObj,lpTarget->Index,lpInfo->Index);
			break;
		default:
			this->CustomAttackSkillAttack(lpObj,lpTarget->Index,lpInfo->Index);
			break;
	}

	DWORD SkillDelay = gSkillManager->GetSkillDelay(lpSkill->m_skill);

	if(SkillDelay == 0)
	{
		SkillDelay = this->GetAttackDelay(lpObj,lpInfo->Index);
	}

	lpObj->Attack.AttackTime = (GetTickCount()+SkillDelay);
}

void CCustomAttack::CustomAttackSkillAttack(LPOBJ lpObj,int bIndex,int skill) // OK
{
	PMSG_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(0x19,sizeof(pMsg));

	#if(GAMESERVER_UPDATE>=701)

	pMsg.skillH = SET_NUMBERHB(skill);

	pMsg.skillL = SET_NUMBERLB(skill);

	pMsg.indexH = SET_NUMBERHB(bIndex);

	pMsg.indexL = SET_NUMBERLB(bIndex);

	#else

	#if(GAMESERVER_UPDATE>=401)

	pMsg.skill[0] = SET_NUMBERHB(skill);

	pMsg.skill[1] = SET_NUMBERLB(skill);

	#else

	pMsg.skill = skill;

	#endif

	pMsg.index[0] = SET_NUMBERHB(bIndex);

	pMsg.index[1] = SET_NUMBERLB(bIndex);

	#endif

	pMsg.dis = 0;

	gSkillManager->CGSkillAttackRecv(&pMsg,lpObj->Index);
}

void CCustomAttack::CustomAttackMultilAttack(LPOBJ lpObj,int bIndex,int skill) // OK
{
	this->CustomAttackDurationlAttack(lpObj,bIndex,skill);

	BYTE send[256];

	PMSG_MULTI_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(PROTOCOL_CODE4,sizeof(pMsg));

	int size = sizeof(pMsg);

	#if(GAMESERVER_UPDATE>=701)

	pMsg.skillH = SET_NUMBERHB(skill);

	pMsg.skillL = SET_NUMBERLB(skill);

	#elif(GAMESERVER_UPDATE>=401)

	pMsg.skill[0] = SET_NUMBERHB(skill);

	pMsg.skill[1] = SET_NUMBERLB(skill);

	#else

	pMsg.skill = skill;

	#endif

	pMsg.x = (BYTE)lpObj->X;

	pMsg.y = (BYTE)lpObj->Y;

	pMsg.serial = 0;

	pMsg.count = 0;

	PMSG_MULTI_SKILL_ATTACK info;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0 || lpObj->VpPlayer2[n].type != OBJECT_MONSTER)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if(gSkillManager->CheckSkillTarget(lpObj,index,bIndex,lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if(gSkillManager->CheckSkillRadio(skill,lpObj->X,lpObj->Y,gObj[index].X,gObj[index].Y) == 0)
		{
			continue;
		}

		#if(GAMESERVER_UPDATE>=701)

		info.indexH = SET_NUMBERHB(index);

		info.indexL = SET_NUMBERLB(index);

		#else

		info.index[0] = SET_NUMBERHB(index);

		info.index[1] = SET_NUMBERLB(index);

		#endif

		info.MagicKey = 0;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		if(CHECK_SKILL_ATTACK_COUNT(pMsg.count) == 0)
		{
			break;
		}
	}

	pMsg.header.size = size;

	memcpy(send,&pMsg,sizeof(pMsg));

	gSkillManager->CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)send,lpObj->Index,0);
}

void CCustomAttack::CustomAttackDurationlAttack(LPOBJ lpObj,int bIndex,int skill) // OK
{
	PMSG_DURATION_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(0x1E,sizeof(pMsg));

	#if(GAMESERVER_UPDATE>=701)

	pMsg.skillH = SET_NUMBERHB(skill);

	pMsg.skillL = SET_NUMBERLB(skill);

	#elif(GAMESERVER_UPDATE>=401)

	pMsg.skill[0] = SET_NUMBERHB(skill);

	pMsg.skill[1] = SET_NUMBERLB(skill);

	#else

	pMsg.skill = skill;

	#endif

	pMsg.x = (BYTE)gObj[bIndex].X;

	pMsg.y = (BYTE)gObj[bIndex].Y;

	pMsg.dir = (gSkillManager->GetSkillAngle(gObj[bIndex].X,gObj[bIndex].Y,lpObj->X,lpObj->Y)*255)/360;

	pMsg.dis = 0;

	pMsg.angle = (gSkillManager->GetSkillAngle(lpObj->X,lpObj->Y,gObj[bIndex].X,gObj[bIndex].Y)*255)/360;

	#if(GAMESERVER_UPDATE>=803)

	pMsg.indexH = SET_NUMBERHB(bIndex);

	pMsg.indexL = SET_NUMBERLB(bIndex);

	#else

	pMsg.index[0] = SET_NUMBERHB(bIndex);

	pMsg.index[1] = SET_NUMBERLB(bIndex);

	#endif

	pMsg.MagicKey = 0;

	gSkillManager->CGDurationSkillAttackRecv(&pMsg,lpObj->Index);
}

void CCustomAttack::CustomAttackRageAttack(LPOBJ lpObj,int bIndex,int skill) // OK
{
	if(skill == SKILL_DARK_SIDE)
	{
		PMSG_SKILL_DARK_SIDE_RECV pMsg;

		pMsg.header.set(0x4B,sizeof(pMsg));

		pMsg.skill[0] = SET_NUMBERHB(skill);

		pMsg.skill[1] = SET_NUMBERLB(skill);

		pMsg.index[0] = SET_NUMBERHB(bIndex);

		pMsg.index[1] = SET_NUMBERLB(bIndex);

		gSkillManager->CGSkillDarkSideRecv(&pMsg,lpObj->Index);
	}
	
	PMSG_RAGE_FIGHTER_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(0x4A,sizeof(pMsg));

	#if(GAMESERVER_UPDATE>=701)

	pMsg.indexH = SET_NUMBERHB(bIndex);

	pMsg.indexL = SET_NUMBERLB(bIndex);

	pMsg.skillH = SET_NUMBERHB(skill);

	pMsg.skillL = SET_NUMBERLB(skill);

	#else

	pMsg.skill[0] = SET_NUMBERHB(skill);

	pMsg.skill[1] = SET_NUMBERLB(skill);

	pMsg.index[0] = SET_NUMBERHB(bIndex);

	pMsg.index[1] = SET_NUMBERLB(bIndex);

	#endif

	pMsg.dis = 0;

	gSkillManager->CGRageFighterSkillAttackRecv(&pMsg,lpObj->Index);

	this->CustomAttackSkillAttack(lpObj,bIndex,skill);
}

void CCustomAttack::CGCustomAttackToggleRecv(PMSG_CUSTOM_ATTACK_TOGGLE_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(this->m_CustomAttackSwitch == 0)
	{
		return;
	}

	if(lpObj->Helper.Started != 0)
	{
		GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(573));
		return;
	}

	if((this->m_CustomAttackMapZone != -1 && this->m_CustomAttackMapList[lpObj->Map] == 0) || gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
	{
		return;
	}

	COMMAND_LIST_INFO lpInfo;

	if(gCommandManager->GetCommandCode(COMMAND_CUSTOM_ATTACK,&lpInfo) == 0)
	{
		return;
	}

	if(lpInfo.Switch[lpObj->AccountLevel] == 0)
	{
		return;
	}

	if(lpObj->TradeMoney != 0)
	{
		return;
	}

	DWORD TickCount = (GetTickCount()-lpObj->CommandCheckTime[lpInfo.Index])/1000;

	if(TickCount < lpInfo.Delay)
	{
		gNotice->GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage->GetMessage(300),(lpInfo.Delay-TickCount),lpInfo.Command);
		return;
	}

	if(lpInfo.GMLevel != -1 && lpInfo.GMLevel > gGameMaster->GetGameMasterLevel(lpObj))
	{
		gNotice->GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage->GetMessage(301),lpInfo.Command);
		return;
	}

	if(lpInfo.MinLevel != -1 && lpInfo.MinLevel > lpObj->Level)
	{
		gNotice->GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage->GetMessage(302),lpInfo.MinLevel,lpInfo.Command);
		return;
	}

	if(lpInfo.MaxLevel != -1 && lpInfo.MaxLevel < lpObj->Level)
	{
		gNotice->GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage->GetMessage(303),lpInfo.MaxLevel,lpInfo.Command);
		return;
	}

	if(lpInfo.MinReset != -1 && lpInfo.MinReset > lpObj->Reset)
	{
		gNotice->GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage->GetMessage(304),lpInfo.MinReset,lpInfo.Command);
		return;
	}

	if(lpInfo.MaxReset != -1 && lpInfo.MaxReset < lpObj->Reset)
	{
		gNotice->GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage->GetMessage(305),lpInfo.MaxReset,lpInfo.Command);
		return;
	}

	if(lpInfo.Money[lpObj->AccountLevel] > lpObj->Money)
	{
		gNotice->GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage->GetMessage(306),lpInfo.Money[lpObj->AccountLevel],lpInfo.Command);
		return;
	}

	if(lpObj->Coin1 < lpInfo.Coin1[lpObj->AccountLevel])
	{
		gNotice->GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage->GetMessage(307),lpInfo.Coin1[lpObj->AccountLevel],lpInfo.Command);
		return;
	}

	if(lpObj->Coin2 < lpInfo.Coin2[lpObj->AccountLevel])
	{
		gNotice->GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage->GetMessage(308),lpInfo.Coin2[lpObj->AccountLevel],lpInfo.Command);
		return;
	}

	if(lpObj->Coin3 < lpInfo.Coin3[lpObj->AccountLevel])
	{
		gNotice->GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage->GetMessage(309),lpInfo.Coin3[lpObj->AccountLevel],lpInfo.Command);
		return;
	}

	#if(GAMESERVER_UPDATE>=401)
	
	if(lpMsg->index >= 300)
	{
		int index = gMasterSkillTree->GetMasterSkillRelated(lpMsg->index);

		lpMsg->index = ((index==0)?lpMsg->index:index);
	}
		
	#endif

	if(lpObj->Attack.Started != 0)
	{
		lpObj->Attack.Started = 0;
	}
	else
	{
		lpObj->Attack.AutoBuff = this->m_CustomAttackBuffEnable[lpObj->AccountLevel];

		lpObj->Attack.SkillIndex = lpMsg->index;

		if(lpObj->Attack.AutoBuff == 0 && lpObj->Attack.SkillIndex == 0)
		{
			GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(575));
			return;
		}

		if(lpObj->Attack.SkillIndex != 0)
		{
			if(this->GetInfo(lpObj->Class,lpObj->Attack.SkillIndex,0) == 0)
			{
				GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(575));
				return;
			}

			if(this->CustomAttackFindSkill(lpObj,lpObj->Attack.SkillIndex) == 0)
			{
				GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(575));
				return;
			}

			if(gSkillManager->CheckSkillRequireWeapon(lpObj,lpObj->Attack.SkillIndex) == 0)
			{
				GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(575));
				return;
			}

			if(lpObj->Attack.SkillIndex == SKILL_TWISTING_SLASH && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].IsItem() == 0)
			{
				GCNewMessageSend(lpObj->Index,1,2,gMessage->GetMessage(575));
				return;
			}
		}

		lpObj->Attack.Started = 1;

		lpObj->Attack.StartMap = lpObj->Map;

		lpObj->Attack.StartX = (BYTE)lpObj->X;

		lpObj->Attack.StartY = (BYTE)lpObj->Y;

		lpObj->Attack.AttackTime = GetTickCount();

		lpObj->Attack.PotionTime = GetTickCount();

		lpObj->Attack.OnlineTime = ((DWORD)time(0));

		lpObj->Attack.TargetIndex = -1;

		gCommandManager->DiscountRequirement(lpObj,COMMAND_CUSTOM_ATTACK);
	}

	this->CustomAttackSend(lpObj->Index);
}