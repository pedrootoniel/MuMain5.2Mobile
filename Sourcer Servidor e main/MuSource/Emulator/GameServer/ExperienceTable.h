// ExperienceTable.h: interface for the CExperienceTable class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct EXPERIENCE_TABLE_INFO
{
	int MinLevel;
	int MaxLevel;
	int MinMasterLevel;
	int MaxMasterLevel;
	int MinReset;
	int MaxReset;
	int MinMasterReset;
	int MaxMasterReset;
	int ExperienceRate;
};

class CExperienceTable
{
	CExperienceTable();
	virtual ~CExperienceTable();
	SingletonInstance(CExperienceTable)
public:
	void Load(char* path);
	int GetExperienceRate(LPOBJ lpObj);
	int GetMasterExperienceRate(LPOBJ lpObj);
private:
	std::vector<EXPERIENCE_TABLE_INFO> m_ExperienceTableInfo;
};

#define gExperienceTable SingNull(CExperienceTable)