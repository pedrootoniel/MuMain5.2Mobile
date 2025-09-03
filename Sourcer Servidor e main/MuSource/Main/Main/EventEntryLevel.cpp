#include "stdafx.h"
#include "EventEntryLevel.h"
#include "Offset.h"
#include "Protect.h"
#include "Util.h"

void InitEventEntryLevel() // OK
{
	for(int n=0;n < 7;n++)
	{
		SetDword(0x006B40A7+(0x1A*n),gProtect.m_MainInfo.m_BloodCastleEntryLevelCommon[n][0]);
		SetDword(0x006B40B4+(0x1A*n),gProtect.m_MainInfo.m_BloodCastleEntryLevelCommon[n][1]);
		SetDword(0x006B4177+(0x1A*n),gProtect.m_MainInfo.m_BloodCastleEntryLevelSpecial[n][0]);
		SetDword(0x006B4184+(0x1A*n),gProtect.m_MainInfo.m_BloodCastleEntryLevelSpecial[n][1]);
	}

	for(int n=0;n < 6;n++)
	{
		SetDword(0x006B79E7+(0x1A*n),gProtect.m_MainInfo.m_DevilSquareEntryLevelCommon[n][0]);
		SetDword(0x006B79F4+(0x1A*n),gProtect.m_MainInfo.m_DevilSquareEntryLevelCommon[n][1]);
		SetDword(0x006B7A9D+(0x1A*n),gProtect.m_MainInfo.m_DevilSquareEntryLevelSpecial[n][0]);
		SetDword(0x006B7AAA+(0x1A*n),gProtect.m_MainInfo.m_DevilSquareEntryLevelSpecial[n][1]);
	}

	MemoryCpy(0x00814948,gProtect.m_MainInfo.m_ChaosCastleEntryLevelCommon,sizeof(gProtect.m_MainInfo.m_ChaosCastleEntryLevelCommon));

	MemoryCpy(0x00814978,gProtect.m_MainInfo.m_ChaosCastleEntryLevelSpecial,sizeof(gProtect.m_MainInfo.m_ChaosCastleEntryLevelSpecial));

	MemoryCpy(0x008166D4,gProtect.m_MainInfo.m_KalimaEntryLevelCommon,sizeof(gProtect.m_MainInfo.m_KalimaEntryLevelCommon));

	MemoryCpy(0x0081670C,gProtect.m_MainInfo.m_KalimaEntryLevelSpecial,sizeof(gProtect.m_MainInfo.m_KalimaEntryLevelSpecial));

	MemoryCpy(0x00815238,gProtect.m_MainInfo.m_IllusionTempleEntryLevelMin,sizeof(gProtect.m_MainInfo.m_IllusionTempleEntryLevelMin));

	MemoryCpy(0x0081524C,gProtect.m_MainInfo.m_IllusionTempleEntryLevelMax,sizeof(gProtect.m_MainInfo.m_IllusionTempleEntryLevelMax));

	SetDword(0x0058A8CA,0x270F); // Chaos Castle MaxLevel

	SetDword(0x0078779F,0x270F); // Kalima MaxLevel
}