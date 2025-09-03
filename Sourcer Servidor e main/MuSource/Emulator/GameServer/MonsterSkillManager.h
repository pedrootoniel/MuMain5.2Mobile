// MonsterSkillManager.h: interface for the CMonsterSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MonsterManager.h"
#include "MonsterSkillInfo.h"
#include "MonsterSkillUnitInfo.h"
#include "Skill.h"
#include "User.h"

#define MAX_MONSTER_SKILL_INFO MAX_MONSTER_INFO

class CMonsterSkillManager
{
	CMonsterSkillManager();
	virtual ~CMonsterSkillManager();
	SingletonInstance(CMonsterSkillManager)
public:
	void Load(char* path);
	void Init();
	bool CheckMonsterSkill(int MonsterClass,int MonsterSkillUnitType);
	CMonsterSkillUnitInfo* FindMonsterSkillUnitInfo(int aIndex,int MonsterSkillUnitType);
	void UseMonsterSkill(int aIndex,int bIndex,int MonsterSkillUnitType,int MonsterSkillUnit,CSkill* lpSkill);
	void SpecialMonsterSkillAttack(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,CMonsterSkillUnitInfo* lpMonsterSkillUnitInfo);
	CMonsterSkillInfo m_MonsterSkillInfoArray[MAX_MONSTER_SKILL_INFO];
};

#define gMonsterSkillManager SingNull(CMonsterSkillManager)