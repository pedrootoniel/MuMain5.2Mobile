// InvasionManager.cpp: implementation of the CInvasionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InvasionManager.h"
#include "GameServer.h"
#include "Map.h"
#include "MapManager.h"
#include "ReadFile.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInvasionManager::CInvasionManager() // OK
{
	for(int n=0;n < MAX_INVASION;n++)
	{
		INVASION_INFO* lpInfo = &this->m_InvasionInfo[n];

		lpInfo->Index = n;
		lpInfo->State = INVASION_STATE_BLANK;
		lpInfo->RemainTime = 0;
		lpInfo->TargetTime = 0;
		lpInfo->TickCount = GetTickCount();

		this->CleanMonster(lpInfo);
	}
}

CInvasionManager::~CInvasionManager() // OK
{

}

void CInvasionManager::Init() // OK
{
	for(int n=0;n < MAX_INVASION;n++)
	{
		if(gServerInfo->m_InvasionManagerSwitch == 0)
		{
			this->SetState(&this->m_InvasionInfo[n],INVASION_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_InvasionInfo[n],INVASION_STATE_EMPTY);
		}
	}
}

void CInvasionManager::Load(char* path) // OK
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

	for(int n=0;n < MAX_INVASION;n++)
	{
		EditMenuLabel(2,2,3000+n,"<EMPTY>",0);
		this->m_InvasionInfo[n].RespawnMessage = -1;
		this->m_InvasionInfo[n].DespawnMessage = -1;
		this->m_InvasionInfo[n].BossIndex = -1;
		memset(this->m_InvasionInfo[n].BossCount,0,sizeof(this->m_InvasionInfo[n].BossCount));
		this->m_InvasionInfo[n].BossMessage = -1;
		this->m_InvasionInfo[n].InvasionTime = 0;
		this->m_InvasionInfo[n].StartTime.clear();

		for(int i=0;i<MAX_INVASION_RESPAWN_GROUP;i++)
		{
			this->m_InvasionInfo[n].RespawnInfo[i].clear();
		}

		this->m_InvasionInfo[n].MonsterInfo.clear();
	}

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

					INVASION_START_TIME info;

					int index = lpReadFile->GetNumber();

					info.Year = lpReadFile->GetAsNumber();

					info.Month = lpReadFile->GetAsNumber();

					info.Day = lpReadFile->GetAsNumber();

					info.DayOfWeek = lpReadFile->GetAsNumber();

					info.Hour = lpReadFile->GetAsNumber();

					info.Minute = lpReadFile->GetAsNumber();

					info.Second = lpReadFile->GetAsNumber();

					this->m_InvasionInfo[index].StartTime.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpReadFile->GetAsString()) == 0)
					{
						break;
					}

					int index = lpReadFile->GetNumber();

					this->m_InvasionInfo[index].RespawnMessage = lpReadFile->GetAsNumber();

					this->m_InvasionInfo[index].DespawnMessage = lpReadFile->GetAsNumber();

					this->m_InvasionInfo[index].BossIndex = lpReadFile->GetAsNumber();

					this->m_InvasionInfo[index].BossMessage = lpReadFile->GetAsNumber();

					this->m_InvasionInfo[index].InvasionTime = lpReadFile->GetAsNumber();

					this->m_InvasionInfo[index].Effect = lpReadFile->GetAsNumber();

					strcpy_s(this->m_InvasionInfo[index].Name,lpReadFile->GetAsString());

					EditMenuLabel(2,2,3000+index,this->m_InvasionInfo[index].Name,1);
				}
				else if(section == 2)
				{
					if(strcmp("end",lpReadFile->GetAsString()) == 0)
					{
						break;
					}

					INVASION_RESPWAN_INFO info;

					int index = lpReadFile->GetNumber();

					info.Group = lpReadFile->GetAsNumber();

					info.Map = lpReadFile->GetAsNumber();

					info.Stage = lpReadFile->GetAsNumber();

					this->m_InvasionInfo[index].RespawnInfo[info.Group].push_back(info);
				}
				else if(section == 3)
				{
					if(strcmp("end",lpReadFile->GetAsString()) == 0)
					{
						break;
					}

					INVASION_MONSTER_INFO info;

					int index = lpReadFile->GetNumber();

					info.Group = lpReadFile->GetAsNumber();

					info.MonsterClass = lpReadFile->GetAsNumber();

					info.Range = lpReadFile->GetAsNumber();

					info.Map = lpReadFile->GetAsNumber();

					info.X = lpReadFile->GetAsNumber();

					info.Y = lpReadFile->GetAsNumber();

					info.TX = lpReadFile->GetAsNumber();

					info.TY = lpReadFile->GetAsNumber();

					info.Count = lpReadFile->GetAsNumber();

					info.Stage = lpReadFile->GetAsNumber();

					info.RegenType = lpReadFile->GetAsNumber();

					info.RegenTime = lpReadFile->GetAsNumber();

					this->m_InvasionInfo[index].MonsterInfo.push_back(info);
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

void CInvasionManager::MainProc() // OK
{
	if(gServerInfo->m_InvasionManagerSwitch == 0)
	{
		return;
	}

	for(int n=0;n < MAX_INVASION;n++)
	{
		INVASION_INFO* lpInfo = &this->m_InvasionInfo[n];

		if((GetTickCount()-lpInfo->TickCount) >= 1000)
		{
			lpInfo->TickCount = GetTickCount();

			lpInfo->RemainTime = (int)difftime(lpInfo->TargetTime,time(0));

			switch(lpInfo->State)
			{
				case INVASION_STATE_BLANK:
					this->ProcState_BLANK(lpInfo);
					break;
				case INVASION_STATE_EMPTY:
					this->ProcState_EMPTY(lpInfo);
					break;
				case INVASION_STATE_START:
					this->ProcState_START(lpInfo);
					break;
			}
		}
	}
}

void CInvasionManager::ProcState_BLANK(INVASION_INFO* lpInfo) // OK
{

}

void CInvasionManager::ProcState_EMPTY(INVASION_INFO* lpInfo) // OK
{
	if(lpInfo->RemainTime <= 0)
	{
		LogAdd(LOG_BLACK,"[Invasion Manager] (%d) SetState START",lpInfo->Index);
		if(lpInfo->RespawnMessage != -1){gNotice->GCNoticeSendToAll(0,0,0,0,0,0,gMessage->GetMessage(lpInfo->RespawnMessage));}
		this->SetState(lpInfo,INVASION_STATE_START);
	}
}

void CInvasionManager::ProcState_START(INVASION_INFO* lpInfo) // OK
{
	if(lpInfo->RemainTime <= 0)
	{
		LogAdd(LOG_BLACK,"[Invasion Manager] (%d) SetState EMPTY",lpInfo->Index);
		if(lpInfo->DespawnMessage != -1){gNotice->GCNoticeSendToAll(0,0,0,0,0,0,gMessage->GetMessage(lpInfo->DespawnMessage));}
		this->SetState(lpInfo,INVASION_STATE_EMPTY);
	}
}

void CInvasionManager::SetState(INVASION_INFO* lpInfo,int state) // OK
{
	if(gServerInfo->m_InvasionManagerSwitch == 0)
	{
		return;
	}

	lpInfo->State = state;

	switch(lpInfo->State)
	{
		case INVASION_STATE_BLANK:
			this->SetState_BLANK(lpInfo);
			break;
		case INVASION_STATE_EMPTY:
			this->SetState_EMPTY(lpInfo);
			break;
		case INVASION_STATE_START:
			this->SetState_START(lpInfo);
			break;
	}
}

void CInvasionManager::SetState_BLANK(INVASION_INFO* lpInfo) // OK
{

}

void CInvasionManager::SetState_EMPTY(INVASION_INFO* lpInfo) // OK
{
	this->ClearMonster(lpInfo);

	this->CheckSync(lpInfo);

	memset(lpInfo->BossCount,0,sizeof(lpInfo->BossCount));

	if(lpInfo->Effect != 0)
	{
		GCEventStateSendToAll(-1,0,lpInfo->Effect);
	}
}

void CInvasionManager::SetState_START(INVASION_INFO* lpInfo) // OK
{
	memset(lpInfo->BossCount,0,sizeof(lpInfo->BossCount));

	for(int n=0;n < MAX_INVASION_RESPAWN_GROUP;n++)
	{
		if(lpInfo->RespawnInfo[n].empty() == 0)
		{
			INVASION_RESPWAN_INFO* lpRespawnInfo = &lpInfo->RespawnInfo[n][(GetLargeRand()%lpInfo->RespawnInfo[n].size())];

			for(std::vector<INVASION_MONSTER_INFO>::iterator it=lpInfo->MonsterInfo.begin();it != lpInfo->MonsterInfo.end();it++)
			{
				if(it->Group != lpRespawnInfo->Group || it->Map != lpRespawnInfo->Map || it->Stage != lpRespawnInfo->Stage)
				{
					continue;
				}

				for(int n=0;n < it->Count;n++)
				{
					int index = gMonster->AddMonster(it->Map);

					if(OBJECT_RANGE(index) == 0)
					{
						continue;
					}

					LPOBJ lpObj = &gObj[index];

					int px = it->X;
					int py = it->Y;

					if(gObjGetRandomMonsterLocation(it->Map,&px,&py,(it->TX - it->X),(it->TY - it->Y),50) == 0)
					{
						px = it->X;
						py = it->Y;
					}

					lpObj->PosNum = -1;
					lpObj->X = px;
					lpObj->Y = py;
					lpObj->TX = px;
					lpObj->TY = py;
					lpObj->OldX = px;
					lpObj->OldY = py;
					lpObj->StartX = px;
					lpObj->StartY = py;
					lpObj->Dir = GetLargeRand() % 8;
					lpObj->Map = it->Map;
					lpObj->MoveRange = it->Range;
					lpObj->InvasionIndex = lpInfo->Index;

					if(gMonster->SetMonster(index,it->MonsterClass) == 0)
					{
						gObjDel(index);
						continue;
					}

					lpObj->DieRegen = 0;
					lpObj->RegenTime = 1;
					lpObj->Attribute = ATTRIBUTE_INVASION;
					lpObj->MaxRegenTime = ((it->RegenType == 0) ? (lpInfo->InvasionTime*1000) : it->RegenTime);
					lpObj->LastCheckTick = GetTickCount();

					if(this->AddMonster(lpInfo,index) == 0)
					{
						gObjDel(index);
						continue;
					}

					if(lpInfo->BossIndex == it->MonsterClass)
					{
						lpInfo->BossCount[it->Map]++;

						if(lpInfo->Effect != 0)
						{
							GCEventStateSendToAll(it->Map,1,lpInfo->Effect);
						}
					}

					LogAdd(LOG_BLACK,"[Invasion Manager] (%d) Spawn Monster [%d] Map:%d X:%d Y:%d",lpInfo->Index,it->MonsterClass,lpObj->Map,lpObj->X,lpObj->Y);
				}
			}
		}
	}

	lpInfo->RemainTime = lpInfo->InvasionTime;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void CInvasionManager::CheckSync(INVASION_INFO* lpInfo) // OK
{
	if(lpInfo->StartTime.empty() != 0)
	{
		this->SetState(lpInfo,INVASION_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<INVASION_START_TIME>::iterator it=lpInfo->StartTime.begin();it != lpInfo->StartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpInfo,INVASION_STATE_BLANK);
		return;
	}

	lpInfo->RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	lpInfo->TargetTime = (int)ScheduleTime.GetTime();

	int sec = lpInfo->RemainTime;

	int day = sec/86400;
	sec %= 86400;

	int hr = sec/3600;
	sec %= 3600;

	int min = sec/60;

	if(day > 0)
	{
		LogAdd(LOG_BLACK,"[Invasion Manager] (%d) Sync Start Time. [%d] day [%d] hour [%d] min remain",lpInfo->Index,day,hr,min);
	}
	else if(hr > 0)
	{
		LogAdd(LOG_BLACK,"[Invasion Manager] (%d) Sync Start Time. [%d] hour [%d] min remain",lpInfo->Index,hr,min);
	}
	else
	{
		LogAdd(LOG_BLACK,"[Invasion Manager] (%d) Sync Start Time. [%d] min remain",lpInfo->Index,min);
	}
}

int CInvasionManager::GetState(int index) // OK
{
	if(index < 0 || index >= MAX_INVASION)
	{
		return INVASION_STATE_BLANK;
	}

	return this->m_InvasionInfo[index].State;
}

int CInvasionManager::GetRemainTime(int index) // OK
{
	if(index < 0 || index >= MAX_INVASION)
	{
		return 0;
	}

	INVASION_INFO* lpInfo = &this->m_InvasionInfo[index];

	if(lpInfo->StartTime.empty() != 0)
	{
		return 0;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<INVASION_START_TIME>::iterator it=lpInfo->StartTime.begin();it != lpInfo->StartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		return 0;
	}

	int RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	return (((RemainTime%60)==0)?(RemainTime/60):((RemainTime/60)+1));
}

bool CInvasionManager::AddMonster(INVASION_INFO* lpInfo,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if(this->GetMonster(lpInfo,aIndex) != 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_INVASION_MONSTER;n++)
	{
		if(OBJECT_RANGE(lpInfo->MonsterIndex[n]) == 0)
		{
			lpInfo->MonsterIndex[n] = aIndex;
			return 1;
		}
	}

	return 0;
}

bool CInvasionManager::DelMonster(INVASION_INFO* lpInfo,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	int* index = this->GetMonster(lpInfo,aIndex);

	if(index == 0)
	{
		return 0;
	}

	(*index) = -1;
	return 1;
}

int* CInvasionManager::GetMonster(INVASION_INFO* lpInfo,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_INVASION_MONSTER;n++)
	{
		if(lpInfo->MonsterIndex[n] == aIndex)
		{
			return &lpInfo->MonsterIndex[n];
		}
	}

	return 0;
}

void CInvasionManager::CleanMonster(INVASION_INFO* lpInfo) // OK
{
	for(int n=0;n < MAX_INVASION_MONSTER;n++)
	{
		lpInfo->MonsterIndex[n] = -1;
	}
}

void CInvasionManager::ClearMonster(INVASION_INFO* lpInfo) // OK
{
	for(int n=0;n < MAX_INVASION_MONSTER;n++)
	{
		if(OBJECT_RANGE(lpInfo->MonsterIndex[n]) != 0)
		{
			gObjDel(lpInfo->MonsterIndex[n]);
			lpInfo->MonsterIndex[n] = -1;
		}
	}
}

int CInvasionManager::GetMonsterCount(INVASION_INFO* lpInfo) // OK
{
	int count = 0;

	for(int n=0;n < MAX_INVASION_MONSTER;n++)
	{
		if(OBJECT_RANGE(lpInfo->MonsterIndex[n]) != 0)
		{
			count++;
		}
	}

	return count;
}

void CInvasionManager::MonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	int aIndex = gMonster->GetTopHitDamageUser(lpObj);

	if(OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	if(this->GetState(lpObj->InvasionIndex) != INVASION_STATE_START)
	{
		return;
	}

	INVASION_INFO* lpInfo = &this->m_InvasionInfo[lpObj->InvasionIndex];

	if(this->GetMonster(lpInfo,lpObj->Index) == 0)
	{
		return;
	}
		
	if(lpObj->Class == lpInfo->BossIndex)
	{
		if((--lpInfo->BossCount[lpTarget->Map]) <= 0)
		{
			if(lpInfo->Effect != 0)
			{
				GCEventStateSendToAll(lpTarget->Map,0,lpInfo->Effect);
			}
		}

		if(lpInfo->BossMessage != -1)
		{
			gNotice->GCNoticeSendToAll(0,0,0,0,0,0,gMessage->GetMessage(lpInfo->BossMessage),lpTarget->Name,gMapManager->GetMapName(lpTarget->Map));
		}

		LogAdd(LOG_BLACK,"[Invasion Manager] (%d) [%s][%s] Kill Boss [%d] Map:%d X:%d Y:%d",lpInfo->Index,lpTarget->Account,lpTarget->Name,lpInfo->BossIndex,lpObj->Map,lpObj->X,lpObj->Y);
	}
}

void CInvasionManager::CheckEvent(LPOBJ lpObj) // OK
{
	for(int n=0;n<MAX_INVASION;n++)
	{
		if(this->GetState(n) != INVASION_STATE_START)
		{
			continue;
		}

		if(this->m_InvasionInfo[n].BossCount[lpObj->Map] <= 0)
		{
			continue;
		}

		if(this->m_InvasionInfo[n].Effect == 0)
		{
			continue;
		}

		GCEventStateSend(lpObj->Index,1,this->m_InvasionInfo[n].Effect);
		return;
	}

	GCEventStateSend(lpObj->Index,0,0);
}

void CInvasionManager::ForceStart(int index) // OK
{
	if(gServerInfo->m_InvasionManagerSwitch == 0)
	{
		return;
	}

	if(index < 0 || index >= MAX_INVASION)
	{
		return;
	}

	INVASION_INFO* lpInfo = &this->m_InvasionInfo[index];

	if(this->GetState(index) == INVASION_STATE_BLANK)
	{
		return;
	}

	SYSTEMTIME SystemTime;

	GetLocalTime(&SystemTime);

	INVASION_START_TIME info;

	info.Year = SystemTime.wYear;

	info.Month = SystemTime.wMonth;

	info.Day = SystemTime.wDay;

	info.DayOfWeek = -1;

	info.Hour = SystemTime.wHour;

	info.Minute = SystemTime.wMinute;

	info.Second = SystemTime.wSecond;

	lpInfo->StartTime.push_back(info);

	this->SetState(lpInfo,INVASION_STATE_EMPTY);
}