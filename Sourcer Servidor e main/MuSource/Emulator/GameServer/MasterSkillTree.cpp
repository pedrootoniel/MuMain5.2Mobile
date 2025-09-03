// MasterSkillTree.cpp: implementation of the CMasterSkillTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MasterSkillTree.h"
#include "GameMain.h"
#include "ItemManager.h"
#include "ReadFile.h"
#include "ObjectManager.h"
#include "ServerInfo.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMasterSkillTree::CMasterSkillTree() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->m_MasterSkillTreeInfo.clear();

	#endif
}

CMasterSkillTree::~CMasterSkillTree() // OK
{

}

void CMasterSkillTree::Load(char* path) // OK
{
	#if(GAMESERVER_UPDATE>=401)

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

	this->m_MasterSkillTreeInfo.clear();

	try
	{
		while (true)
		{
			if(lpReadFile->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpReadFile->GetString()) == 0)
			{
				break;
			}

			MASTER_SKILL_TREE_INFO info;

			info.Index = lpReadFile->GetNumber();

			info.Group = lpReadFile->GetAsNumber();

			info.Rank = lpReadFile->GetAsNumber();

			info.MinLevel = lpReadFile->GetAsNumber();

			info.MaxLevel = lpReadFile->GetAsNumber();

			info.MainValue = lpReadFile->GetAsNumber();

			info.RelatedSkill = lpReadFile->GetAsNumber();

			info.ReplaceSkill = lpReadFile->GetAsNumber();

			info.RequireSkill[0] = lpReadFile->GetAsNumber();

			info.RequireSkill[1] = lpReadFile->GetAsNumber();

			this->m_MasterSkillTreeInfo.insert(std::pair<int,MASTER_SKILL_TREE_INFO>(info.Index,info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadFile->GetLastError());
	}

	delete lpReadFile;

#endif
}

bool CMasterSkillTree::GetInfo(int index,MASTER_SKILL_TREE_INFO* lpInfo) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	std::map<int,MASTER_SKILL_TREE_INFO>::iterator it = this->m_MasterSkillTreeInfo.find(index);

	if(it != this->m_MasterSkillTreeInfo.end())
	{
		(*lpInfo) = it->second;
		return 1;
	}

	#endif

	return 0;
}

int CMasterSkillTree::GetMasterSkillRelated(int index) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	std::map<int,MASTER_SKILL_TREE_INFO>::iterator it = this->m_MasterSkillTreeInfo.find(index);

	if(it != this->m_MasterSkillTreeInfo.end())
	{
		return it->second.RelatedSkill;
	}

	#endif

	return 0;
}

float CMasterSkillTree::GetMasterSkillValue(int index,int level) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	std::map<int,MASTER_SKILL_TREE_INFO>::iterator it = this->m_MasterSkillTreeInfo.find(index);

	if(it != this->m_MasterSkillTreeInfo.end())
	{
		return (float)it->second.MainValue;
	}

	#endif

	return 0;
}

int CMasterSkillTree::GetMasterSkillValue(LPOBJ lpObj,int index) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	#if(GAMESERVER_UPDATE>=602)

	CSkill* lpMasterSkill = ((lpObj->Type == OBJECT_USER) ? gSkillManager->GetMasterSkill(lpObj,index) : 0);

	if(lpMasterSkill != 0)
	{
		return (int)GetRoundValue(this->GetMasterSkillValue(lpMasterSkill->m_index,lpMasterSkill->m_level));
	}

	#else

	CSkill* lpMasterSkill = ((lpObj->Type == OBJECT_USER) ? gSkillManager->GetSkill(lpObj,index) : 0);

	if(lpMasterSkill != 0)
	{
		return (int)this->GetMasterSkillValue(lpMasterSkill->m_index,lpMasterSkill->m_level);
	}

	#endif

	#endif

	return 0;
}

int CMasterSkillTree::GetMasterSkillLevel(LPOBJ lpObj,int index) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	#if(GAMESERVER_UPDATE>=602)

	CSkill* lpMasterSkill = ((lpObj->Type == OBJECT_USER) ? gSkillManager->GetMasterSkill(lpObj,index) : 0);

	if(lpMasterSkill != 0)
	{
		return (lpMasterSkill->m_level+1);
	}

	#else

	CSkill* lpMasterSkill = ((lpObj->Type == OBJECT_USER) ? gSkillManager->GetSkill(lpObj,index) : 0);

	if(lpMasterSkill != 0)
	{
		return ((lpMasterSkill->m_index%MAX_SKILL_TREE_LEVEL)+1);
	}

	#endif

	#endif

	return 0;
}

int CMasterSkillTree::GetMasterSkillWeapon(LPOBJ lpObj,int index) // OK
{
	return index;
}

int CMasterSkillTree::GetMasterSkillDamageMin(LPOBJ lpObj,int index) // OK
{
	return 0;
}

int CMasterSkillTree::GetMasterSkillDamageMax(LPOBJ lpObj,int index) // OK
{
	return 0;
}

bool CMasterSkillTree::CheckMasterLevel(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(gServerInfo->m_MasterSkillTree != 0 && lpObj->Level >= gServerInfo->m_MaxLevel && lpObj->ChangeUp >= 2)
	{
		return 1;
	}

	#endif
	
	return 0;
}

bool CMasterSkillTree::CheckMasterReplaceSkill(LPOBJ lpObj,int index) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	CSkill* lpSkill = gSkillManager->GetSkill(lpObj,index);

	if(index != 0 && lpSkill == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}

	#endif

	return 0;
}

bool CMasterSkillTree::CheckMasterRequireSkill(LPOBJ lpObj,int index) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	#if(GAMESERVER_UPDATE>=602)

	CSkill* lpMasterSkill = gSkillManager->GetMasterSkill(lpObj,index);

	if(index != 0 && (lpMasterSkill == 0 || (lpMasterSkill->IsMasterSkill() != 0 && (lpMasterSkill->m_level+1) < MIN_SKILL_TREE_LEVEL)))
	{
		return 0;
	}
	else
	{
		return 1;
	}

	#else

	CSkill* lpMasterSkill = gSkillManager->GetSkill(lpObj,index);

	if(index != 0 && (lpMasterSkill == 0 || (lpMasterSkill->IsMasterSkill() != 0 && ((lpMasterSkill->m_index%MAX_SKILL_TREE_LEVEL)+1) < MIN_SKILL_TREE_LEVEL)))
	{
		return 0;
	}
	else
	{
		return 1;
	}

	#endif

	#endif

	return 0;
}

bool CMasterSkillTree::CheckMasterRequireGroup(LPOBJ lpObj,int group,int rank) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	#if(GAMESERVER_UPDATE>=602)

	for(int n = 0; n < MAX_MASTER_SKILL_LIST; n++)
	{
		if(lpObj->MasterSkill[n].IsMasterSkill() != 0)
		{
			MASTER_SKILL_TREE_INFO MasterSkillTreeInfo;

			if(this->GetInfo(lpObj->MasterSkill[n].m_index,&MasterSkillTreeInfo) != 0)
			{
				if(MasterSkillTreeInfo.Group == group && MasterSkillTreeInfo.Rank == rank && (lpObj->MasterSkill[n].m_level+1) >= MIN_SKILL_TREE_LEVEL)
				{
					return 1;
				}
			}
		}
	}

	#else

	for(int n = 0; n < MAX_SKILL_LIST; n++)
	{
		if(lpObj->Skill[n].IsMasterSkill() != 0)
		{
			MASTER_SKILL_TREE_INFO MasterSkillTreeInfo;

			if(this->GetInfo(lpObj->Skill[n].m_index,&MasterSkillTreeInfo) != 0)
			{
				if(MasterSkillTreeInfo.Group == group && MasterSkillTreeInfo.Rank == rank && ((lpObj->Skill[n].m_index%MAX_SKILL_TREE_LEVEL)+1) >= MIN_SKILL_TREE_LEVEL)
				{
					return 1;
				}
			}
		}
	}

	#endif

	#endif

	return 0;	
}

void CMasterSkillTree::SetMasterLevelExperienceTable() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->m_MasterLevelExperienceTable[0] = 0;

	QWORD var1 = 0,var2 = 0,var3 = 0;

	for(int n=1;n < (MAX_CHARACTER_MASTER_LEVEL+1);n++)
	{
		var1 = n+400;

		var3 = ((((var1+9)*var1)*var1)*10);

		var2 = var1-255;

		var3 += ((((var2+9)*var2)*var2)*1000);

		var3 = (var3-3892250000)/2;

		this->m_MasterLevelExperienceTable[n] = var3;
	}
	
	#endif
}

void CMasterSkillTree::CalcMasterLevelNextExperience(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	lpObj->MasterExperience = ((lpObj->MasterExperience<this->m_MasterLevelExperienceTable[lpObj->MasterLevel]) ? this->m_MasterLevelExperienceTable[lpObj->MasterLevel] : lpObj->MasterExperience);

	lpObj->MasterNextExperience = this->m_MasterLevelExperienceTable[((lpObj->MasterLevel >= MAX_CHARACTER_MASTER_LEVEL) ? MAX_CHARACTER_MASTER_LEVEL : (lpObj->MasterLevel+1))];

	#endif
}

void CMasterSkillTree::CalcMasterSkillTreeOption(LPOBJ lpObj,bool flag) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	#if(GAMESERVER_UPDATE>=602)

	for(int n = 0; n < MAX_MASTER_SKILL_LIST; n++)
	{
		if(lpObj->MasterSkill[n].IsMasterSkill() != 0)
		{
			MASTER_SKILL_TREE_INFO MasterSkillTreeInfo;

			if(this->GetInfo(lpObj->MasterSkill[n].m_index,&MasterSkillTreeInfo) != 0)
			{
				this->InsertOption(lpObj,MasterSkillTreeInfo.Index,(int)GetRoundValue(this->GetMasterSkillValue(MasterSkillTreeInfo.Index,(((lpObj->MasterSkill[n].m_level+0) >= MasterSkillTreeInfo.MaxLevel) ? (MasterSkillTreeInfo.MaxLevel - 1) : (lpObj->MasterSkill[n].m_level+0)))),flag);
			}
		}
	}

	#else

	for(int n = 0; n < MAX_SKILL_LIST; n++)
	{
		if(lpObj->Skill[n].IsMasterSkill() != 0)
		{
			MASTER_SKILL_TREE_INFO MasterSkillTreeInfo;

			if(this->GetInfo(lpObj->Skill[n].m_index,&MasterSkillTreeInfo) != 0)
			{
				this->InsertOption(lpObj,MasterSkillTreeInfo.Index,(int)this->GetMasterSkillValue(MasterSkillTreeInfo.Index,lpObj->Skill[n].m_level),flag);
			}
		}
	}

	#endif

	#endif
}

void CMasterSkillTree::InsertOption(LPOBJ lpObj,int index,int value,bool flag) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	#if(GAMESERVER_UPDATE>=602)

	if(flag == 0)
	{
		if(index == MASTER_SKILL_ADD_ENERGY1 || index == MASTER_SKILL_ADD_VITALITY1 || index == MASTER_SKILL_ADD_DEXTERITY1 || index == MASTER_SKILL_ADD_STRENGTH1 || index == MASTER_SKILL_ADD_LEADERSHIP || index == MASTER_SKILL_ADD_ENERGY2 || index == MASTER_SKILL_ADD_VITALITY2 || index == MASTER_SKILL_ADD_DEXTERITY2 || index == MASTER_SKILL_ADD_STRENGTH2)
		{
			return;
		}
	}
	else
	{
		if(index != MASTER_SKILL_ADD_ENERGY1 && index != MASTER_SKILL_ADD_VITALITY1 && index != MASTER_SKILL_ADD_DEXTERITY1 && index != MASTER_SKILL_ADD_STRENGTH1 && index != MASTER_SKILL_ADD_LEADERSHIP && index != MASTER_SKILL_ADD_ENERGY2 && index != MASTER_SKILL_ADD_VITALITY2 && index != MASTER_SKILL_ADD_DEXTERITY2 && index != MASTER_SKILL_ADD_STRENGTH2)
		{
			return;
		}
	}

	switch (index)
	{
	case MASTER_SKILL_ADD_ITEM_DURABILITY_RATE1:
		lpObj->WeaponDurabilityRate += value;
		lpObj->ArmorDurabilityRate += value;
		break;
	case MASTER_SKILL_ADD_DEFENSE_SUCCESS_RATE_PVP1:
		lpObj->DefenseSuccessRatePvP += value;
		break;
	case MASTER_SKILL_ADD_MAX_SD1:
		lpObj->AddShield += value;
		break;
	case MASTER_SKILL_ADD_MP_RECOVERY_RATE1:
		lpObj->MPRecoveryRate += value;
		break;
	case MASTER_SKILL_ADD_POISON_RESISTANCE1:
		lpObj->Resistance[1] += value;
		break;
	case MASTER_SKILL_ADD_JEWELRY_DURABILITY_RATE1:
		lpObj->PendantDurabilityRate += value;
		lpObj->RingDurabilityRate += value;
		break;
	case MASTER_SKILL_ADD_SD_RECOVERY_RATE1:
		lpObj->SDRecoveryRate += value;
		break;
	case MASTER_SKILL_ADD_HP_RECOVERY_RATE1:
		lpObj->HPRecoveryRate += value;
		break;
	case MASTER_SKILL_ADD_LIGHTNING_RESISTANCE1:
		lpObj->Resistance[2] += value;
		break;
	case MASTER_SKILL_ADD_DEFENSE1:
		lpObj->Defense += value;
		break;
	case MASTER_SKILL_ADD_BP_RECOVERY_RATE1:
		lpObj->BPRecoveryRate += value;
		break;
	case MASTER_SKILL_ADD_ICE_RESISTANCE1:
		lpObj->Resistance[0] += value;
		break;
	case MASTER_SKILL_ADD_GUARDIAN_DURABILITY_RATE1:
		lpObj->GuardianDurabilityRate += value;
		break;
	case MASTER_SKILL_ADD_DEFENSE_SUCCESS_RATE1:
		lpObj->DefenseSuccessRate += (lpObj->DefenseSuccessRate*value)/100;
		break;
	case MASTER_SKILL_ADD_ARMOR_SET_BONUS1:
		if(lpObj->ArmorSetBonus != 0)
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_REFLECT_DAMAGE1:
		lpObj->FullDamageReflectRate += value;
		break;
	case MASTER_SKILL_ADD_ENERGY1:
		lpObj->AddEnergy += value;
		break;
	case MASTER_SKILL_ADD_VITALITY1:
		lpObj->AddVitality += value;
		break;
	case MASTER_SKILL_ADD_DEXTERITY1:
		lpObj->AddDexterity += value;
		break;
	case MASTER_SKILL_ADD_STRENGTH1:
		lpObj->AddStrength += value;
		break;
	case MASTER_SKILL_ADD_DK_WING_DEFENSE:
		if(lpObj->Inventory[INVENTORY_SLOT_WING].m_Index == GET_ITEM(12,36))
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_DK_WING_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WING].m_Index == GET_ITEM(12,36))
		{
			lpObj->PhysiDamageMinLeft += value;
			lpObj->PhysiDamageMinRight += value;
			lpObj->PhysiDamageMaxLeft += value;
			lpObj->PhysiDamageMaxRight += value;
		}
		break;
	case MASTER_SKILL_ADD_ATTACK_SUCCESS_RATE1:
		lpObj->AttackSuccessRate += value;
		break;
	case MASTER_SKILL_ADD_MAX_HP1:
		lpObj->AddLife += value;
		break;
	case MASTER_SKILL_ADD_WEAPON_DAMAGE1:
		lpObj->PhysiDamageMinLeft += value;
		lpObj->PhysiDamageMinRight += value;
		lpObj->PhysiDamageMaxLeft += value;
		lpObj->PhysiDamageMaxRight += value;
		break;
	case MASTER_SKILL_ADD_MAX_MP1:
		lpObj->AddMana += value;
		break;
	case MASTER_SKILL_ADD_MAX_BP1:
		lpObj->AddBP += value;
		break;
	case MASTER_SKILL_ADD_BLOOD_STORM:
		if((value = gSkillManager->AddSkill(lpObj,SKILL_BLOOD_STORM,0)) >= 0)
		{
			gSkillManager->GCSkillAddSend(lpObj->Index,value,SKILL_BLOOD_STORM,0,0);
		}
		break;
	case MASTER_SKILL_ADD_ATTACK_SUCCESS_RATE_PVP1:
		lpObj->AttackSuccessRatePvP += value;
		break;
	case MASTER_SKILL_ADD_TWO_HAND_SWORD_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(0,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(1,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_TwoHand != 0)
		{
			lpObj->PhysiDamageMinLeft += value;
			lpObj->PhysiDamageMinRight += value;
			lpObj->PhysiDamageMaxLeft += value;
			lpObj->PhysiDamageMaxRight += value;
		}
		break;
	case MASTER_SKILL_ADD_ONE_HAND_SWORD_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(0,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(1,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_TwoHand == 0)
		{
			lpObj->PhysiDamageMinLeft += value;
			lpObj->PhysiDamageMinRight += value;
			lpObj->PhysiDamageMaxLeft += value;
			lpObj->PhysiDamageMaxRight += value;
		}
		break;
	case MASTER_SKILL_ADD_MACE_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(2,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(3,0))
		{
			lpObj->PhysiDamageMinLeft += value;
			lpObj->PhysiDamageMinRight += value;
			lpObj->PhysiDamageMaxLeft += value;
			lpObj->PhysiDamageMaxRight += value;
		}
		break;
	case MASTER_SKILL_ADD_SPEAR_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(3,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(4,0))
		{
			lpObj->PhysiDamageMinLeft += value;
			lpObj->PhysiDamageMinRight += value;
			lpObj->PhysiDamageMaxLeft += value;
			lpObj->PhysiDamageMaxRight += value;
		}
		break;
	case MASTER_SKILL_ADD_TWO_HAND_SWORD_MASTERY:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(0,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(1,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_TwoHand != 0)
		{
			lpObj->DamagePvP += value;
		}
		break;
	case MASTER_SKILL_ADD_ONE_HAND_SWORD_MASTERY:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(0,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(1,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_TwoHand == 0)
		{
			lpObj->PhysiSpeed += value;
			lpObj->MagicSpeed += value;
		}
		break;
	case MASTER_SKILL_ADD_SPEAR_MASTERY:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(3,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(4,0))
		{
			lpObj->DoubleDamageRate += value;
		}
		break;
	case MASTER_SKILL_ADD_MP_CONSUMPTION_RATE1:
		lpObj->MPConsumptionRate -= value;
		break;
	case MASTER_SKILL_ADD_HUNT_SD1:
		lpObj->HuntSD += ((value == 0) ? 0 : (100/value));
		break;
	case MASTER_SKILL_ADD_HUNT_HP1:
		lpObj->HuntHP += ((value == 0) ? 0 : (100/value));
		break;
	case MASTER_SKILL_ADD_MIN_PHYSI_DAMAGE1:
		lpObj->PhysiDamageMinLeft += value;
		lpObj->PhysiDamageMinRight += value;
		break;
	case MASTER_SKILL_ADD_HUNT_MP1:
		lpObj->HuntMP += ((value == 0) ? 0 : (100/value));
		break;
	case MASTER_SKILL_ADD_MAX_PHYSI_DAMAGE1:
		lpObj->PhysiDamageMaxLeft += value;
		lpObj->PhysiDamageMaxRight += value;
		break;
	case MASTER_SKILL_ADD_CRITICAL_DAMAGE_RATE1:
		lpObj->CriticalDamageRate += value;
		break;
	case MASTER_SKILL_ADD_RESTORE_MP_RATE1:
		lpObj->OffensiveFullMPRestoreRate += value;
		break;
	case MASTER_SKILL_ADD_RESTORE_HP_RATE1:
		lpObj->OffensiveFullHPRestoreRate += value;
		break;
	case MASTER_SKILL_ADD_EXCELLENT_DAMAGE_RATE1:
		lpObj->ExcellentDamageRate += value;
		break;
	case MASTER_SKILL_ADD_DOUBLE_DAMAGE_RATE1:
		lpObj->DoubleDamageRate += value;
		break;
	case MASTER_SKILL_ADD_IGNORE_DEFENSE_RATE1:
		lpObj->IgnoreDefenseRate += value;
		break;
	case MASTER_SKILL_ADD_RESTORE_SD_RATE1:
		lpObj->OffensiveFullSDRestoreRate += value;
		break;
	case MASTER_SKILL_ADD_DW_WING_DEFENSE:
		if(lpObj->Inventory[INVENTORY_SLOT_WING].m_Index == GET_ITEM(12,37))
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_DW_WING_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WING].m_Index == GET_ITEM(12,37))
		{
			lpObj->MagicDamageMin += value;
			lpObj->MagicDamageMax += value;
		}
		break;
	case MASTER_SKILL_ADD_MAGIC_DAMAGE1:
		lpObj->MagicDamageMin += value;
		lpObj->MagicDamageMax += value;
		break;
	case MASTER_SKILL_ADD_ONE_HAND_STAFF_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(6,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_TwoHand == 0)
		{
			lpObj->MagicDamageMin += value;
			lpObj->MagicDamageMax += value;
		}
		break;
	case MASTER_SKILL_ADD_TWO_HAND_STAFF_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(6,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_TwoHand != 0)
		{
			lpObj->MagicDamageMin += value;
			lpObj->MagicDamageMax += value;
		}
		break;
	case MASTER_SKILL_ADD_SHIELD_DEFENSE1:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index >= GET_ITEM(6,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index < GET_ITEM(7,0))
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_ONE_HAND_STAFF_MASTERY:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(6,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_TwoHand == 0)
		{
			lpObj->PhysiSpeed += value;
			lpObj->MagicSpeed += value;
		}
		break;
	case MASTER_SKILL_ADD_TWO_HAND_STAFF_MASTERY:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(6,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_TwoHand != 0)
		{
			lpObj->DamagePvP += value;
		}
		break;
	case MASTER_SKILL_ADD_SHIELD_MASTERY1:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index >= GET_ITEM(6,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index < GET_ITEM(7,0))
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_MIN_MAGIC_DAMAGE1:
		lpObj->MagicDamageMin += value;
		break;
	case MASTER_SKILL_ADD_MAX_MAGIC_DAMAGE1:
		lpObj->MagicDamageMax += value;
		break;
	case MASTER_SKILL_ADD_FE_WING_DEFENSE:
		if(lpObj->Inventory[INVENTORY_SLOT_WING].m_Index == GET_ITEM(12,38))
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_FE_WING_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WING].m_Index == GET_ITEM(12,38))
		{
			lpObj->PhysiDamageMinLeft += value;
			lpObj->PhysiDamageMinRight += value;
			lpObj->PhysiDamageMaxLeft += value;
			lpObj->PhysiDamageMaxRight += value;
		}
		break;
	case MASTER_SKILL_ADD_WEAPON_DAMAGE2:
		lpObj->PhysiDamageMinLeft += value;
		lpObj->PhysiDamageMinRight += value;
		lpObj->PhysiDamageMaxLeft += value;
		lpObj->PhysiDamageMaxRight += value;
		break;
	case MASTER_SKILL_ADD_CURE:
		if((value = gSkillManager->AddSkill(lpObj,SKILL_CURE,0)) >= 0)
		{
			gSkillManager->GCSkillAddSend(lpObj->Index,value,SKILL_CURE,0,0);
		}
		break;
	case MASTER_SKILL_ADD_PARTY_HEAL:
		if((value = gSkillManager->AddSkill(lpObj,SKILL_PARTY_HEAL,0)) >= 0)
		{
			gSkillManager->GCSkillAddSend(lpObj->Index,value,SKILL_PARTY_HEAL,0,0);
		}
		break;
	case MASTER_SKILL_ADD_POISON_ARROW:
		if((value = gSkillManager->AddSkill(lpObj,SKILL_POISON_ARROW,0)) >= 0)
		{
			gSkillManager->GCSkillAddSend(lpObj->Index,value,SKILL_POISON_ARROW,0,0);
		}
		break;
	case MASTER_SKILL_ADD_BLESS:
		if((value = gSkillManager->AddSkill(lpObj,SKILL_BLESS,0)) >= 0)
		{
			gSkillManager->GCSkillAddSend(lpObj->Index,value,SKILL_BLESS,0,0);
		}
		break;
	case MASTER_SKILL_ADD_SUMMON_SATYROS:
		if((value = gSkillManager->AddSkill(lpObj,SKILL_SUMMON_SATYROS,0)) >= 0)
		{
			gSkillManager->GCSkillAddSend(lpObj->Index,value,SKILL_SUMMON_SATYROS,0,0);
		}
		break;
	case MASTER_SKILL_ADD_BOW_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index >= GET_ITEM(4,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index < GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index != GET_ITEM(4,7) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Slot == 1)
		{
			lpObj->PhysiDamageMinLeft += value;
			lpObj->PhysiDamageMinRight += value;
			lpObj->PhysiDamageMaxLeft += value;
			lpObj->PhysiDamageMaxRight += value;
		}
		break;
	case MASTER_SKILL_ADD_CROSS_BOW_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(4,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index != GET_ITEM(4,15) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Slot == 0)
		{
			lpObj->PhysiDamageMinLeft += value;
			lpObj->PhysiDamageMinRight += value;
			lpObj->PhysiDamageMaxLeft += value;
			lpObj->PhysiDamageMaxRight += value;
		}
		break;
	case MASTER_SKILL_ADD_SHIELD_DEFENSE2:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index >= GET_ITEM(6,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index < GET_ITEM(7,0))
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_BOW_MASTERY:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index >= GET_ITEM(4,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index < GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index != GET_ITEM(4,7) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Slot == 1)
		{
			lpObj->PhysiSpeed += value;
			lpObj->MagicSpeed += value;
		}
		break;
	case MASTER_SKILL_ADD_CROSS_BOW_MASTERY:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(4,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index != GET_ITEM(4,15) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Slot == 0)
		{
			lpObj->DamagePvP += value;
		}
		break;
	case MASTER_SKILL_ADD_SHIELD_MASTERY2:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index >= GET_ITEM(6,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index < GET_ITEM(7,0))
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_MIN_PHYSI_DAMAGE2:
		lpObj->PhysiDamageMinLeft += value;
		lpObj->PhysiDamageMinRight += value;
		break;
	case MASTER_SKILL_ADD_MAX_PHYSI_DAMAGE2:
		lpObj->PhysiDamageMaxLeft += value;
		lpObj->PhysiDamageMaxRight += value;
		break;
	case MASTER_SKILL_ADD_SU_WING_DEFENSE:
		if(lpObj->Inventory[INVENTORY_SLOT_WING].m_Index == GET_ITEM(12,43))
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_SU_WING_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WING].m_Index == GET_ITEM(12,43))
		{
			lpObj->MagicDamageMin += value;
			lpObj->MagicDamageMax += value;
		}
		break;
	case MASTER_SKILL_ADD_MAGIC_DAMAGE2:
		lpObj->MagicDamageMin += value;
		lpObj->MagicDamageMax += value;
		break;
	case MASTER_SKILL_ADD_BLIND:
		if((value = gSkillManager->AddSkill(lpObj,SKILL_BLIND,0)) >= 0)
		{
			gSkillManager->GCSkillAddSend(lpObj->Index,value,SKILL_BLIND,0,0);
		}
		break;
	case MASTER_SKILL_ADD_STICK_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(6,0))
		{
			lpObj->MagicDamageMin += value;
			lpObj->MagicDamageMax += value;
		}
		break;
	case MASTER_SKILL_ADD_BOOK_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index >= GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index < GET_ITEM(6,0))
		{
			lpObj->CurseDamageMin += value;
			lpObj->CurseDamageMax += value;
		}
		break;
	case MASTER_SKILL_ADD_STICK_MASTERY:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(6,0))
		{
			lpObj->DamagePvP += value;
		}
		break;
	case MASTER_SKILL_ADD_BOOK_MASTERY:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index >= GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index < GET_ITEM(6,0))
		{
			lpObj->PhysiSpeed += value;
			lpObj->MagicSpeed += value;
		}
		break;
	case MASTER_SKILL_ADD_MIN_MAGIC_DAMAGE2:
		lpObj->MagicDamageMin += value;
		lpObj->CurseDamageMin += value;
		break;
	case MASTER_SKILL_ADD_MAX_MAGIC_DAMAGE2:
		lpObj->MagicDamageMax += value;
		lpObj->CurseDamageMax += value;
		break;
	case MASTER_SKILL_ADD_MG_WING_DEFENSE:
		if(lpObj->Inventory[INVENTORY_SLOT_WING].m_Index == GET_ITEM(12,39))
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_MG_WING_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WING].m_Index == GET_ITEM(12,39))
		{
			lpObj->MagicDamageMin += value;
			lpObj->MagicDamageMax += value;
			lpObj->PhysiDamageMinLeft += value;
			lpObj->PhysiDamageMinRight += value;
			lpObj->PhysiDamageMaxLeft += value;
			lpObj->PhysiDamageMaxRight += value;
		}
		break;
	case MASTER_SKILL_ADD_WEAPON_DAMAGE3:
		lpObj->PhysiDamageMinLeft += value;
		lpObj->PhysiDamageMinRight += value;
		lpObj->PhysiDamageMaxLeft += value;
		lpObj->PhysiDamageMaxRight += value;
		break;
	case MASTER_SKILL_ADD_MAGIC_DAMAGE3:
		lpObj->MagicDamageMin += value;
		lpObj->MagicDamageMax += value;
		break;
	case MASTER_SKILL_ADD_EARTH_PRISON:
		if((value = gSkillManager->AddSkill(lpObj,SKILL_EARTH_PRISON,0)) >= 0)
		{
			gSkillManager->GCSkillAddSend(lpObj->Index,value,SKILL_EARTH_PRISON,0,0);
		}
		break;
	case MASTER_SKILL_ADD_DL_WING_DEFENSE:
		if(lpObj->Inventory[INVENTORY_SLOT_WING].m_Index == GET_ITEM(12,40))
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_LEADERSHIP:
		lpObj->AddLeadership += value;
		break;
	case MASTER_SKILL_ADD_DL_WING_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WING].m_Index == GET_ITEM(12,40))
		{
			lpObj->PhysiDamageMinLeft += value;
			lpObj->PhysiDamageMinRight += value;
			lpObj->PhysiDamageMaxLeft += value;
			lpObj->PhysiDamageMaxRight += value;
		}
		break;
	case MASTER_SKILL_ADD_DARK_HORSE_DEFENSE:
		if(lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,4))
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_WEAPON_DAMAGE4:
		lpObj->PhysiDamageMinLeft += value;
		lpObj->PhysiDamageMinRight += value;
		lpObj->PhysiDamageMaxLeft += value;
		lpObj->PhysiDamageMaxRight += value;
		break;
	case MASTER_SKILL_ADD_IRON_DEFENSE:
		if((value = gSkillManager->AddSkill(lpObj,SKILL_IRON_DEFENSE,0)) >= 0)
		{
			gSkillManager->GCSkillAddSend(lpObj->Index,value,SKILL_IRON_DEFENSE,0,0);
		}
		break;
	case MASTER_SKILL_ADD_SCEPTER_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(2,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(3,0))
		{
			lpObj->PhysiDamageMinLeft += value;
			lpObj->PhysiDamageMinRight += value;
			lpObj->PhysiDamageMaxLeft += value;
			lpObj->PhysiDamageMaxRight += value;
		}
		break;
	case MASTER_SKILL_ADD_SHIELD_DEFENSE3:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index >= GET_ITEM(6,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index < GET_ITEM(7,0))
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_SCEPTER_MASTERY:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(2,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(3,0))
		{
			lpObj->DamagePvP += value;
		}
		break;
	case MASTER_SKILL_ADD_SHIELD_MASTERY3:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index >= GET_ITEM(6,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index < GET_ITEM(7,0))
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_DEFENSE_BY_LEADERSHIP:
		lpObj->Defense += (lpObj->Leadership+lpObj->AddLeadership)/value;
		break;
	case MASTER_SKILL_ADD_PET_DURABILITY_RATE:
		lpObj->PetDurabilityRate += value;
		break;
	case MASTER_SKILL_ADD_RF_WING_DEFENSE:
		if(lpObj->Inventory[INVENTORY_SLOT_WING].m_Index == GET_ITEM(12,50))
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_RF_WING_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WING].m_Index == GET_ITEM(12,50))
		{
			lpObj->PhysiDamageMinLeft += value;
			lpObj->PhysiDamageMinRight += value;
			lpObj->PhysiDamageMaxLeft += value;
			lpObj->PhysiDamageMaxRight += value;
		}
		break;
	case MASTER_SKILL_ADD_DEFENSE_SUCCESS_RATE2:
		lpObj->DefenseSuccessRate += (lpObj->DefenseSuccessRate*value)/100;
		break;
	case MASTER_SKILL_ADD_WEAPON_DAMAGE5:
		lpObj->PhysiDamageMinLeft += value;
		lpObj->PhysiDamageMinRight += value;
		lpObj->PhysiDamageMaxLeft += value;
		lpObj->PhysiDamageMaxRight += value;
		break;
	case MASTER_SKILL_ADD_BLOOD_HOWLING:
		if((value = gSkillManager->AddSkill(lpObj,SKILL_BLOOD_HOWLING,0)) >= 0)
		{
			gSkillManager->GCSkillAddSend(lpObj->Index,value,SKILL_BLOOD_HOWLING,0,0);
		}
		break;
	case MASTER_SKILL_ADD_GLOVE_DAMAGE:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(0,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(1,0))
		{
			lpObj->PhysiDamageMinLeft += value;
			lpObj->PhysiDamageMinRight += value;
			lpObj->PhysiDamageMaxLeft += value;
			lpObj->PhysiDamageMaxRight += value;
		}
		break;
	case MASTER_SKILL_ADD_GLOVE_MASTERY:
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(0,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(1,0))
		{
			lpObj->DoubleDamageRate += value;
		}
		break;
	case MASTER_SKILL_ADD_ITEM_DURABILITY_RATE2:
		lpObj->WeaponDurabilityRate += value;
		lpObj->ArmorDurabilityRate += value;
		break;
	case MASTER_SKILL_ADD_DEFENSE_SUCCESS_RATE_PVP2:
		lpObj->DefenseSuccessRatePvP += value;
		break;
	case MASTER_SKILL_ADD_MAX_SD2:
		lpObj->AddShield += value;
		break;
	case MASTER_SKILL_ADD_MP_RECOVERY_RATE2:
		lpObj->MPRecoveryRate += value;
		break;
	case MASTER_SKILL_ADD_POISON_RESISTANCE2:
		lpObj->Resistance[1] += value;
		break;
	case MASTER_SKILL_ADD_JEWELRY_DURABILITY_RATE2:
		lpObj->PendantDurabilityRate += value;
		lpObj->RingDurabilityRate += value;
		break;
	case MASTER_SKILL_ADD_SD_RECOVERY_RATE2:
		lpObj->SDRecoveryRate += value;
		break;
	case MASTER_SKILL_ADD_HP_RECOVERY_RATE2:
		lpObj->HPRecoveryRate += value;
		break;
	case MASTER_SKILL_ADD_LIGHTNING_RESISTANCE2:
		lpObj->Resistance[2] += value;
		break;
	case MASTER_SKILL_ADD_DEFENSE2:
		lpObj->Defense += value;
		break;
	case MASTER_SKILL_ADD_BP_RECOVERY_RATE2:
		lpObj->BPRecoveryRate += value;
		break;
	case MASTER_SKILL_ADD_ICE_RESISTANCE2:
		lpObj->Resistance[0] += value;
		break;
	case MASTER_SKILL_ADD_GUARDIAN_DURABILITY_RATE2:
		lpObj->GuardianDurabilityRate += value;
		break;
	case MASTER_SKILL_ADD_DEFENSE_SUCCESS_RATE3:
		lpObj->DefenseSuccessRate += (lpObj->DefenseSuccessRate*value)/100;
		break;
	case MASTER_SKILL_ADD_ARMOR_SET_BONUS2:
		if(lpObj->ArmorSetBonus != 0)
		{
			lpObj->Defense += value;
		}
		break;
	case MASTER_SKILL_ADD_REFLECT_DAMAGE2:
		lpObj->FullDamageReflectRate += value;
		break;
	case MASTER_SKILL_ADD_ENERGY2:
		lpObj->AddEnergy += value;
		break;
	case MASTER_SKILL_ADD_VITALITY2:
		lpObj->AddVitality += value;
		break;
	case MASTER_SKILL_ADD_DEXTERITY2:
		lpObj->AddDexterity += value;
		break;
	case MASTER_SKILL_ADD_STRENGTH2:
		lpObj->AddStrength += value;
		break;
	case MASTER_SKILL_ADD_ATTACK_SUCCESS_RATE2:
		lpObj->AttackSuccessRate += value;
		break;
	case MASTER_SKILL_ADD_MAX_HP2:
		lpObj->AddLife += value;
		break;
	case MASTER_SKILL_ADD_MAX_MP2:
		lpObj->AddMana += value;
		break;
	case MASTER_SKILL_ADD_ATTACK_SUCCESS_RATE_PVP2:
		lpObj->AttackSuccessRatePvP += value;
		break;
	case MASTER_SKILL_ADD_MP_CONSUMPTION_RATE2:
		lpObj->MPConsumptionRate -= value;
		break;
	case MASTER_SKILL_ADD_HUNT_SD2:
		lpObj->HuntSD += ((value == 0) ? 0 : (100/value));
		break;
	case MASTER_SKILL_ADD_HUNT_HP2:
		lpObj->HuntHP += ((value == 0) ? 0 : (100/value));
		break;
	case MASTER_SKILL_ADD_MIN_PHYSI_DAMAGE3:
		lpObj->PhysiDamageMinLeft += value;
		lpObj->PhysiDamageMinRight += value;
		break;
	case MASTER_SKILL_ADD_HUNT_MP2:
		lpObj->HuntMP += ((value == 0) ? 0 : (100/value));
		break;
	case MASTER_SKILL_ADD_MAX_PHYSI_DAMAGE3:
		lpObj->PhysiDamageMaxLeft += value;
		lpObj->PhysiDamageMaxRight += value;
		break;
	case MASTER_SKILL_ADD_CRITICAL_DAMAGE_RATE2:
		lpObj->CriticalDamageRate += value;
		break;
	case MASTER_SKILL_ADD_RESTORE_MP_RATE2:
		lpObj->DefensiveFullMPRestoreRate += value;
		break;
	case MASTER_SKILL_ADD_RESTORE_HP_RATE2:
		lpObj->DefensiveFullHPRestoreRate += value;
		break;
	case MASTER_SKILL_ADD_EXCELLENT_DAMAGE_RATE2:
		lpObj->ExcellentDamageRate += value;
		break;
	case MASTER_SKILL_ADD_DOUBLE_DAMAGE_RATE2:
		lpObj->DoubleDamageRate += value;
		break;
	case MASTER_SKILL_ADD_IGNORE_DEFENSE_RATE2:
		lpObj->IgnoreDefenseRate += value;
		break;
	case MASTER_SKILL_ADD_RESTORE_SD_RATE2:
		lpObj->OffensiveFullSDRestoreRate += value;
		break;
	case MASTER_SKILL_ADD_TRIPLE_DAMAGE_RATE:
		lpObj->TripleDamageRate += value;
		break;
	}

	#else

	if(flag != 0)
	{
		return;
	}

	switch (index)
	{
	case MASTER_SKILL_ADD_ATTACK_SUCCESS_RATE_1:
	case MASTER_SKILL_ADD_ATTACK_SUCCESS_RATE_2:
	case MASTER_SKILL_ADD_ATTACK_SUCCESS_RATE_3:
	case MASTER_SKILL_ADD_ATTACK_SUCCESS_RATE_4:
	case MASTER_SKILL_ADD_ATTACK_SUCCESS_RATE_5:
		lpObj->AttackSuccessRate += value;
		break;
	case MASTER_SKILL_ADD_ATTACK_SUCCESS_RATE_PVP_1:
	case MASTER_SKILL_ADD_ATTACK_SUCCESS_RATE_PVP_2:
	case MASTER_SKILL_ADD_ATTACK_SUCCESS_RATE_PVP_3:
	case MASTER_SKILL_ADD_ATTACK_SUCCESS_RATE_PVP_4:
	case MASTER_SKILL_ADD_ATTACK_SUCCESS_RATE_PVP_5:
		lpObj->AttackSuccessRatePvP += value;
		break;
	case MASTER_SKILL_ADD_DEFENSE_SUCCESS_RATE_PVP_1:
	case MASTER_SKILL_ADD_DEFENSE_SUCCESS_RATE_PVP_2:
	case MASTER_SKILL_ADD_DEFENSE_SUCCESS_RATE_PVP_3:
	case MASTER_SKILL_ADD_DEFENSE_SUCCESS_RATE_PVP_4:
	case MASTER_SKILL_ADD_DEFENSE_SUCCESS_RATE_PVP_5:
		lpObj->DefenseSuccessRatePvP += value;
		break;
	case MASTER_SKILL_ADD_ITEM_DURABILITY_RATE_1:
	case MASTER_SKILL_ADD_ITEM_DURABILITY_RATE_2:
	case MASTER_SKILL_ADD_ITEM_DURABILITY_RATE_3:
	case MASTER_SKILL_ADD_ITEM_DURABILITY_RATE_4:
	case MASTER_SKILL_ADD_ITEM_DURABILITY_RATE_5:
		lpObj->WeaponDurabilityRate += value;
		lpObj->ArmorDurabilityRate += value;
		break;
	case MASTER_SKILL_ADD_JEWELRY_DURABILITY_RATE_1:
	case MASTER_SKILL_ADD_JEWELRY_DURABILITY_RATE_2:
	case MASTER_SKILL_ADD_JEWELRY_DURABILITY_RATE_3:
	case MASTER_SKILL_ADD_JEWELRY_DURABILITY_RATE_4:
	case MASTER_SKILL_ADD_JEWELRY_DURABILITY_RATE_5:
		lpObj->PendantDurabilityRate += value;
		lpObj->RingDurabilityRate += value;
		break;
	case MASTER_SKILL_ADD_POISON_RESISTANCE_1:
	case MASTER_SKILL_ADD_POISON_RESISTANCE_2:
	case MASTER_SKILL_ADD_POISON_RESISTANCE_3:
	case MASTER_SKILL_ADD_POISON_RESISTANCE_4:
	case MASTER_SKILL_ADD_POISON_RESISTANCE_5:
		lpObj->Resistance[1] += value;
		break;
	case MASTER_SKILL_ADD_LIGHTNING_RESISTANCE_1:
	case MASTER_SKILL_ADD_LIGHTNING_RESISTANCE_2:
	case MASTER_SKILL_ADD_LIGHTNING_RESISTANCE_3:
	case MASTER_SKILL_ADD_LIGHTNING_RESISTANCE_4:
	case MASTER_SKILL_ADD_LIGHTNING_RESISTANCE_5:
		lpObj->Resistance[2] += value;
		break;
	case MASTER_SKILL_ADD_ICE_RESISTANCE_1:
	case MASTER_SKILL_ADD_ICE_RESISTANCE_2:
	case MASTER_SKILL_ADD_ICE_RESISTANCE_3:
	case MASTER_SKILL_ADD_ICE_RESISTANCE_4:
	case MASTER_SKILL_ADD_ICE_RESISTANCE_5:
		lpObj->Resistance[0] += value;
		break;
	case MASTER_SKILL_ADD_HP_RECOVERY_RATE_1:
	case MASTER_SKILL_ADD_HP_RECOVERY_RATE_2:
	case MASTER_SKILL_ADD_HP_RECOVERY_RATE_3:
	case MASTER_SKILL_ADD_HP_RECOVERY_RATE_4:
	case MASTER_SKILL_ADD_HP_RECOVERY_RATE_5:
		lpObj->HPRecoveryRate += value;
		break;
	case MASTER_SKILL_ADD_MONEY_AMOUNT_DROP_RATE_1:
	case MASTER_SKILL_ADD_MONEY_AMOUNT_DROP_RATE_2:
	case MASTER_SKILL_ADD_MONEY_AMOUNT_DROP_RATE_3:
	case MASTER_SKILL_ADD_MONEY_AMOUNT_DROP_RATE_4:
	case MASTER_SKILL_ADD_MONEY_AMOUNT_DROP_RATE_5:
		lpObj->MoneyAmountDropRate += value;
		break;
	case MASTER_SKILL_ADD_DEFENSE_1:
	case MASTER_SKILL_ADD_DEFENSE_2:
	case MASTER_SKILL_ADD_DEFENSE_3:
	case MASTER_SKILL_ADD_DEFENSE_4:
	case MASTER_SKILL_ADD_DEFENSE_5:
		lpObj->Defense += value;
		break;
	case MASTER_SKILL_ADD_MAX_HP_1:
	case MASTER_SKILL_ADD_MAX_HP_2:
	case MASTER_SKILL_ADD_MAX_HP_3:
	case MASTER_SKILL_ADD_MAX_HP_4:
	case MASTER_SKILL_ADD_MAX_HP_5:
		lpObj->AddLife += value;
		break;
	case MASTER_SKILL_ADD_MAX_BP_1:
	case MASTER_SKILL_ADD_MAX_BP_2:
	case MASTER_SKILL_ADD_MAX_BP_3:
	case MASTER_SKILL_ADD_MAX_BP_4:
	case MASTER_SKILL_ADD_MAX_BP_5:
		lpObj->AddBP += value;
		break;
	case MASTER_SKILL_ADD_HUNT_MP_1:
	case MASTER_SKILL_ADD_HUNT_MP_2:
	case MASTER_SKILL_ADD_HUNT_MP_3:
	case MASTER_SKILL_ADD_HUNT_MP_4:
	case MASTER_SKILL_ADD_HUNT_MP_5:
		lpObj->HuntMP += ((value == 0) ? 0 : (100/value));
		break;
	case MASTER_SKILL_ADD_HUNT_HP_1:
	case MASTER_SKILL_ADD_HUNT_HP_2:
	case MASTER_SKILL_ADD_HUNT_HP_3:
	case MASTER_SKILL_ADD_HUNT_HP_4:
	case MASTER_SKILL_ADD_HUNT_HP_5:
		lpObj->HuntHP += ((value == 0) ? 0 : (100/value));
		break;
	case MASTER_SKILL_ADD_HUNT_SD_1:
	case MASTER_SKILL_ADD_HUNT_SD_2:
	case MASTER_SKILL_ADD_HUNT_SD_3:
	case MASTER_SKILL_ADD_HUNT_SD_4:
	case MASTER_SKILL_ADD_HUNT_SD_5:
		lpObj->HuntSD += ((value == 0) ? 0 : (100/value));
		break;
	case MASTER_SKILL_ADD_EXPERIENCE_RATE_1:
	case MASTER_SKILL_ADD_EXPERIENCE_RATE_2:
	case MASTER_SKILL_ADD_EXPERIENCE_RATE_3:
	case MASTER_SKILL_ADD_EXPERIENCE_RATE_4:
	case MASTER_SKILL_ADD_EXPERIENCE_RATE_5:
		lpObj->ExperienceRate += value;
		break;
	case MASTER_SKILL_ADD_MAX_SD_1:
	case MASTER_SKILL_ADD_MAX_SD_2:
	case MASTER_SKILL_ADD_MAX_SD_3:
	case MASTER_SKILL_ADD_MAX_SD_4:
	case MASTER_SKILL_ADD_MAX_SD_5:
		lpObj->AddShield += value;
		break;
	case MASTER_SKILL_ADD_SD_RECOVERY_RATE_1:
	case MASTER_SKILL_ADD_SD_RECOVERY_RATE_2:
	case MASTER_SKILL_ADD_SD_RECOVERY_RATE_3:
	case MASTER_SKILL_ADD_SD_RECOVERY_RATE_4:
	case MASTER_SKILL_ADD_SD_RECOVERY_RATE_5:
		lpObj->SDRecoveryRate += value;
		break;
	case MASTER_SKILL_ADD_MAX_PHYSI_DAMAGE_1:
	case MASTER_SKILL_ADD_MAX_PHYSI_DAMAGE_2:
	case MASTER_SKILL_ADD_MAX_PHYSI_DAMAGE_3:
	case MASTER_SKILL_ADD_MAX_PHYSI_DAMAGE_4:
	case MASTER_SKILL_ADD_MAX_PHYSI_DAMAGE_5:
		lpObj->PhysiDamageMaxLeft += value;
		lpObj->PhysiDamageMaxRight += value;
		break;
	case MASTER_SKILL_ADD_MIN_PHYSI_DAMAGE_1:
	case MASTER_SKILL_ADD_MIN_PHYSI_DAMAGE_2:
	case MASTER_SKILL_ADD_MIN_PHYSI_DAMAGE_3:
	case MASTER_SKILL_ADD_MIN_PHYSI_DAMAGE_4:
	case MASTER_SKILL_ADD_MIN_PHYSI_DAMAGE_5:
		lpObj->PhysiDamageMinLeft += value;
		lpObj->PhysiDamageMinRight += value;
		break;
	case MASTER_SKILL_ADD_MP_CONSUMPTION_RATE_1:
	case MASTER_SKILL_ADD_MP_CONSUMPTION_RATE_2:
	case MASTER_SKILL_ADD_MP_CONSUMPTION_RATE_3:
	case MASTER_SKILL_ADD_MP_CONSUMPTION_RATE_4:
	case MASTER_SKILL_ADD_MP_CONSUMPTION_RATE_5:
		lpObj->MPConsumptionRate -= value;
		break;
	case MASTER_SKILL_ADD_MAX_MAGIC_DAMAGE_1:
	case MASTER_SKILL_ADD_MAX_MAGIC_DAMAGE_2:
	case MASTER_SKILL_ADD_MAX_MAGIC_DAMAGE_3:
	case MASTER_SKILL_ADD_MAX_MAGIC_DAMAGE_4:
	case MASTER_SKILL_ADD_MAX_MAGIC_DAMAGE_5:
		lpObj->MagicDamageMax += value;
		break;
	case MASTER_SKILL_ADD_MIN_MAGIC_DAMAGE_1:
	case MASTER_SKILL_ADD_MIN_MAGIC_DAMAGE_2:
	case MASTER_SKILL_ADD_MIN_MAGIC_DAMAGE_3:
	case MASTER_SKILL_ADD_MIN_MAGIC_DAMAGE_4:
	case MASTER_SKILL_ADD_MIN_MAGIC_DAMAGE_5:
		lpObj->MagicDamageMin += value;
		break;
	case MASTER_SKILL_ADD_PET_DURABILITY_RATE_1:
	case MASTER_SKILL_ADD_PET_DURABILITY_RATE_2:
	case MASTER_SKILL_ADD_PET_DURABILITY_RATE_3:
	case MASTER_SKILL_ADD_PET_DURABILITY_RATE_4:
	case MASTER_SKILL_ADD_PET_DURABILITY_RATE_5:
		lpObj->PetDurabilityRate += value;
		break;
	case MASTER_SKILL_ADD_MAX_DAMAGE_1:
	case MASTER_SKILL_ADD_MAX_DAMAGE_2:
	case MASTER_SKILL_ADD_MAX_DAMAGE_3:
	case MASTER_SKILL_ADD_MAX_DAMAGE_4:
	case MASTER_SKILL_ADD_MAX_DAMAGE_5:
		lpObj->PhysiDamageMaxLeft += value;
		lpObj->PhysiDamageMaxRight += value;
		lpObj->MagicDamageMax += value;
		break;
	case MASTER_SKILL_ADD_MIN_DAMAGE_1:
	case MASTER_SKILL_ADD_MIN_DAMAGE_2:
	case MASTER_SKILL_ADD_MIN_DAMAGE_3:
	case MASTER_SKILL_ADD_MIN_DAMAGE_4:
	case MASTER_SKILL_ADD_MIN_DAMAGE_5:
		lpObj->PhysiDamageMinLeft += value;
		lpObj->PhysiDamageMinRight += value;
		lpObj->MagicDamageMin += value;
		break;
	}

	#endif

	#endif
}

bool CMasterSkillTree::ReplaceMasterSkill(LPOBJ lpObj,int index,int skill,int level) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	CSkill* lpSkill = gSkillManager->GetSkill(lpObj,index);

	if(lpSkill == 0)
	{
		return ((index>0) ? ((index == skill) ? 1 : 0) : 1);
	}
	else
	{
		return lpSkill->Set(skill,level);
	}

	#else

	return 0;

	#endif
}

bool CMasterSkillTree::ResetMasterSkill(LPOBJ lpObj,int index) // OK
{
	
	return 0;

}

void CMasterSkillTree::CGMasterSkillRecv(PMSG_MASTER_SKILL_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=602)

	if(gServerInfo->m_MasterSkillTree == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	MASTER_SKILL_TREE_INFO MasterSkillTreeInfo;

	if(this->GetInfo(lpMsg->MasterSkill,&MasterSkillTreeInfo) == 0)
	{
		return;
	}

	if(lpObj->MasterPoint < MasterSkillTreeInfo.MinLevel)
	{
		return;
	}

	CSkill* lpMasterSkill = gSkillManager->GetMasterSkill(lpObj,MasterSkillTreeInfo.Index);

	if(lpMasterSkill == 0)
	{
		if(this->CheckMasterReplaceSkill(lpObj,MasterSkillTreeInfo.ReplaceSkill) == 0)
		{
			return;
		}

		if(this->CheckMasterRequireSkill(lpObj,MasterSkillTreeInfo.RequireSkill[0]) == 0)
		{
			return;
		}

		if(this->CheckMasterRequireSkill(lpObj,MasterSkillTreeInfo.RequireSkill[1]) == 0)
		{
			return;
		}

		if(MasterSkillTreeInfo.Rank > 1 && this->CheckMasterRequireGroup(lpObj,MasterSkillTreeInfo.Group,(MasterSkillTreeInfo.Rank - 1)) == 0)
		{
			return;
		}

		if(this->ReplaceMasterSkill(lpObj,MasterSkillTreeInfo.ReplaceSkill,MasterSkillTreeInfo.Index,(MasterSkillTreeInfo.MinLevel - 1)) == 0)
		{
			return;
		}

		gSkillManager->AddMasterSkill(lpObj,MasterSkillTreeInfo.Index,(MasterSkillTreeInfo.MinLevel - 1));
	}
	else
	{
		if((lpMasterSkill->m_level+1) >= MasterSkillTreeInfo.MaxLevel)
		{
			return;
		}

		if(this->ReplaceMasterSkill(lpObj,MasterSkillTreeInfo.Index,MasterSkillTreeInfo.Index,lpMasterSkill->m_level) == 0)
		{
			return;
		}

		lpMasterSkill->Set(MasterSkillTreeInfo.Index,(lpMasterSkill->m_level+1));
	}

	this->GCMasterSkillListSend(aIndex);

	gSkillManager->GCSkillListSend(lpObj,0);

	lpObj->MasterPoint -= MasterSkillTreeInfo.MinLevel;

	gObjectManager->CharacterCalcAttribute(aIndex);

	this->GCMasterSkillSend(aIndex,0,0,lpObj->MasterPoint,lpMsg->MasterSkill,lpMsg->MasterEmpty);

	#else

	if(gServerInfo->m_MasterSkillTree == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	MASTER_SKILL_TREE_INFO MasterSkillTreeInfo;

	if(this->GetInfo(lpMsg->MasterSkill,&MasterSkillTreeInfo) == 0)
	{
		return;
	}

	if(lpObj->MasterPoint < MasterSkillTreeInfo.MinLevel)
	{
		return;
	}

	if((MasterSkillTreeInfo.Index%MAX_SKILL_TREE_LEVEL) == 0)
	{
		if(this->CheckMasterReplaceSkill(lpObj,MasterSkillTreeInfo.ReplaceSkill) == 0)
		{
			return;
		}

		if(this->CheckMasterRequireSkill(lpObj,MasterSkillTreeInfo.RequireSkill[0]) == 0)
		{
			return;
		}

		if(this->CheckMasterRequireSkill(lpObj,MasterSkillTreeInfo.RequireSkill[1]) == 0)
		{
			return;
		}

		if(MasterSkillTreeInfo.Rank > 1 && this->CheckMasterRequireGroup(lpObj,MasterSkillTreeInfo.Group,(MasterSkillTreeInfo.Rank - 1)) == 0)
		{
			return;
		}

		if(this->ReplaceMasterSkill(lpObj,MasterSkillTreeInfo.ReplaceSkill,MasterSkillTreeInfo.Index,0) == 0)
		{
			return;
		}

		gSkillManager->AddSkill(lpObj,MasterSkillTreeInfo.Index,0);
	}
	else
	{
		if(this->CheckMasterReplaceSkill(lpObj,MasterSkillTreeInfo.ReplaceSkill) == 0)
		{
			return;
		}

		if(((MasterSkillTreeInfo.Index%MAX_SKILL_TREE_LEVEL)+1) > MasterSkillTreeInfo.MaxLevel)
		{
			return;
		}

		if(this->ReplaceMasterSkill(lpObj,MasterSkillTreeInfo.ReplaceSkill,MasterSkillTreeInfo.Index,0) == 0)
		{
			return;
		}
	}

	lpObj->MasterPoint -= MasterSkillTreeInfo.MinLevel;

	gObjectManager->CharacterCalcAttribute(aIndex);

	this->GCMasterSkillSend(aIndex,1,0,lpObj->MasterPoint,lpMsg->MasterSkill,lpMsg->MasterEmpty);

	#endif
}

void CMasterSkillTree::GCMasterInfoSend(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	PMSG_MASTER_INFO_SEND pMsg;

	pMsg.header.set(0xF3,0x50,sizeof(pMsg));

	pMsg.MasterLevel = lpObj->MasterLevel;

	pMsg.Experience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterExperience)));
	pMsg.Experience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterExperience)));
	pMsg.Experience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterExperience)));
	pMsg.Experience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterExperience)));
	pMsg.Experience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterExperience)));
	pMsg.Experience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterExperience)));
	pMsg.Experience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterExperience)));
	pMsg.Experience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterExperience)));

	pMsg.NextExperience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
	pMsg.NextExperience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
	pMsg.NextExperience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
	pMsg.NextExperience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
	pMsg.NextExperience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
	pMsg.NextExperience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
	pMsg.NextExperience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
	pMsg.NextExperience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterNextExperience)));

	pMsg.MasterPoint = lpObj->MasterPoint;

	pMsg.MaxLife = GET_MAX_WORD_VALUE((lpObj->MaxLife+lpObj->AddLife));

	pMsg.MaxMana = GET_MAX_WORD_VALUE((lpObj->MaxMana+lpObj->AddMana));

	pMsg.MaxShield = GET_MAX_WORD_VALUE((lpObj->MaxShield+lpObj->AddShield));

	pMsg.MaxBP = GET_MAX_WORD_VALUE((lpObj->MaxBP+lpObj->AddBP));

	#if(GAMESERVER_EXTRA==1)
	pMsg.ViewMaxHP = (DWORD)(lpObj->MaxLife+lpObj->AddLife);
	pMsg.ViewMaxMP = (DWORD)(lpObj->MaxMana+lpObj->AddMana);
	pMsg.ViewMaxBP = (DWORD)(lpObj->MaxBP+lpObj->AddBP);
	pMsg.ViewMaxSD = (DWORD)(lpObj->MaxShield+lpObj->AddShield);
	#endif

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CMasterSkillTree::GCMasterLevelUpSend(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	PMSG_MASTER_LEVEL_UP_SEND pMsg;

	pMsg.header.set(0xF3,0x51,sizeof(pMsg));

	pMsg.MasterLevel = lpObj->MasterLevel;

	pMsg.MinMasterLevel = 1;

	pMsg.MasterPoint = lpObj->MasterPoint;

	pMsg.MaxMasterLevel = gServerInfo->m_MasterSkillTreeMaxLevel;

	pMsg.MaxLife = GET_MAX_WORD_VALUE((lpObj->MaxLife+lpObj->AddLife));

	pMsg.MaxMana = GET_MAX_WORD_VALUE((lpObj->MaxMana+lpObj->AddMana));

	pMsg.MaxShield = GET_MAX_WORD_VALUE((lpObj->MaxShield+lpObj->AddShield));

	pMsg.MaxBP = GET_MAX_WORD_VALUE((lpObj->MaxBP+lpObj->AddBP));

	#if(GAMESERVER_EXTRA==1)
	pMsg.ViewMaxHP = (DWORD)(lpObj->MaxLife+lpObj->AddLife);
	pMsg.ViewMaxMP = (DWORD)(lpObj->MaxMana+lpObj->AddMana);
	pMsg.ViewMaxBP = (DWORD)(lpObj->MaxBP+lpObj->AddBP);
	pMsg.ViewMaxSD = (DWORD)(lpObj->MaxShield+lpObj->AddShield);
	pMsg.ViewMasterExperience = lpObj->MasterExperience;
	pMsg.ViewMasterNextExperience = lpObj->MasterNextExperience;
	#endif

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	GCEffectInfoSend(lpObj->Index,16);

	#endif
}

void CMasterSkillTree::GCMasterSkillSend(int aIndex,BYTE type,BYTE flag,int MasterPoint,int MasterSkill,int MasterEmpty) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	PMSG_MASTER_SKILL_SEND pMsg;

	pMsg.header.set(0xF3,0x52,sizeof(pMsg));

	pMsg.type = type;

	pMsg.flag = flag;

	pMsg.MasterPoint = MasterPoint;

	#if(GAMESERVER_UPDATE<=406)

	pMsg.MasterSkill = MasterSkill;

	pMsg.MasterEmpty = MasterEmpty;

	#endif

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CMasterSkillTree::GCMasterSkillListSend(int aIndex) // OK
{
	
}

void CMasterSkillTree::DGMasterSkillTreeRecv(SDHP_MASTER_SKILL_TREE_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGMasterSkillTreeRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if(lpObj->LoadMasterLevel != 0)
	{
		return;
	}

	lpObj->LoadMasterLevel = 1;

	lpObj->MasterLevel = lpMsg->MasterLevel;

	lpObj->MasterPoint = lpMsg->MasterPoint;

	lpObj->MasterExperience = lpMsg->MasterExperience;

	gObjectManager->CharacterCalcAttribute(lpObj->Index);

	this->GCMasterInfoSend(lpObj);

	#endif
}

void CMasterSkillTree::GDMasterSkillTreeSend(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	if(gObj[aIndex].LoadMasterLevel != 0)
	{
		return;
	}

	SDHP_MASTER_SKILL_TREE_SEND pMsg;

	pMsg.header.set(0x0D,0x00,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CMasterSkillTree::GDMasterSkillTreeSaveSend(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->LoadMasterLevel == 0)
	{
		return;
	}

	SDHP_MASTER_SKILL_TREE_SAVE_SEND pMsg;

	pMsg.header.set(0x0D,0x30,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.MasterLevel = lpObj->MasterLevel;

	pMsg.MasterPoint = lpObj->MasterPoint;

	pMsg.MasterExperience = lpObj->MasterExperience;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));

	#endif
}