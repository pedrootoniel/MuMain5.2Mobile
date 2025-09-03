// CastleDeep.cpp: implementation of the CCastleDeep class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CastleDeep.h"
#include "DSProtocol.h"
#include "Map.h"
#include "MapServerManager.h"
#include "ReadFile.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCastleDeep::CCastleDeep() // OK
{
	this->m_State = CD_STATE_EMPTY;
	this->m_RemainTime = 0;
	this->m_TargetTime = 0;
	this->m_TickCount = GetTickCount();
	this->m_TimeNotify = 0;
	this->m_Stage = 0;
	this->m_Group = 0;

	this->m_CastleDeepStartTime.clear();

	this->m_CastleDeepGroup.clear();

	this->m_CastleDeepSpawn.clear();

	this->m_CastleDeepMonster.clear();
}

CCastleDeep::~CCastleDeep() // OK
{

}

void CCastleDeep::Init() // OK
{
	if(gServerInfo->m_CastleDeepEvent == 0)
	{
		this->SetState(CD_STATE_BLANK);
	}
	else
	{
		this->SetState(CD_STATE_EMPTY);
	}
}

void CCastleDeep::Load(char* path) // OK
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

	this->m_CastleDeepStartTime.clear();

	this->m_CastleDeepGroup.clear();

	this->m_CastleDeepSpawn.clear();

	this->m_CastleDeepMonster.clear();

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

					CASTLE_DEEP_START_TIME info;

					info.Year = lpReadFile->GetNumber();

					info.Month = lpReadFile->GetAsNumber();

					info.Day = lpReadFile->GetAsNumber();

					info.DayOfWeek = lpReadFile->GetAsNumber();

					info.Hour = lpReadFile->GetAsNumber();

					info.Minute = lpReadFile->GetAsNumber();

					info.Second = lpReadFile->GetAsNumber();

					this->m_CastleDeepStartTime.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpReadFile->GetAsString()) == 0)
					{
						break;
					}

					CASTLE_DEEP_GROUP info;

					info.Index = lpReadFile->GetNumber();

					info.Rate = lpReadFile->GetAsNumber();

					this->m_CastleDeepGroup.push_back(info);
				}
				else if(section == 2)
				{
					if(strcmp("end",lpReadFile->GetAsString()) == 0)
					{
						break;
					}

					CASTLE_DEEP_SPAWN info;

					info.Index = lpReadFile->GetNumber();

					info.Group = lpReadFile->GetAsNumber();

					info.Minute = lpReadFile->GetAsNumber();

					this->m_CastleDeepSpawn.push_back(info);
				}
				else if(section == 3)
				{
					if(strcmp("end",lpReadFile->GetAsString()) == 0)
					{
						break;
					}

					CASTLE_DEEP_MONSTER info;

					info.Index = lpReadFile->GetNumber();

					info.Group = lpReadFile->GetAsNumber();

					info.Class = lpReadFile->GetAsNumber();

					info.Count = lpReadFile->GetAsNumber();

					info.X = lpReadFile->GetAsNumber();

					info.Y = lpReadFile->GetAsNumber();

					info.TX = lpReadFile->GetAsNumber();

					info.TY = lpReadFile->GetAsNumber();

					this->m_CastleDeepMonster.push_back(info);
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

void CCastleDeep::MainProc() // OK
{
	if(gServerInfo->m_CastleDeepEvent == 0)
	{
		return;
	}

	if((GetTickCount()-this->m_TickCount) >= 1000)
	{
		this->m_TickCount = GetTickCount();

		this->m_RemainTime = (int)difftime(this->m_TargetTime,time(0));

		switch(this->m_State)
		{
			case CD_STATE_BLANK:
				this->ProcState_BLANK();
				break;
			case CD_STATE_EMPTY:
				this->ProcState_EMPTY();
				break;
			case CD_STATE_START:
				this->ProcState_START();
				break;
		}
	}
}

void CCastleDeep::ProcState_BLANK() // OK
{

}

void CCastleDeep::ProcState_EMPTY() // OK
{
	if(this->m_RemainTime > 0 && this->m_RemainTime <= 300 && this->m_TimeNotify == 0)
	{
		this->m_TimeNotify = 1;

		GDGlobalNoticeSend(gMapServerManager->GetMapServerGroup(),0,0,0,0,0,0,gMessage->GetMessage(52));
	}

	if(this->m_RemainTime <= 0)
	{
		GDGlobalNoticeSend(gMapServerManager->GetMapServerGroup(),0,0,0,0,0,0,gMessage->GetMessage(53));

		this->SetState(CD_STATE_START);
	}
}

void CCastleDeep::ProcState_START() // OK
{
	for(std::vector<CASTLE_DEEP_SPAWN>::iterator it=this->m_CastleDeepSpawn.begin();it != this->m_CastleDeepSpawn.end();it++)
	{
		if(this->m_Group == it->Index && this->m_Stage == it->Group && (((gServerInfo->m_CastleDeepEventTime*60)-this->m_RemainTime)/60) == it->Minute)
		{
			this->AddMonster(it->Index,it->Group);
			this->m_Stage++;
			break;
		}
	}

	if(this->m_RemainTime <= 0)
	{
		gNotice->GCNoticeSendToAll(0,0,0,0,0,0,gMessage->GetMessage(54));

		this->SetState(CD_STATE_EMPTY);
	}
}

void CCastleDeep::SetState(int state) // OK
{
	if(gServerInfo->m_CastleDeepEvent == 0)
	{
		return;
	}

	this->m_State = state;

	switch(this->m_State)
	{
		case CD_STATE_BLANK:
			this->SetState_BLANK();
			break;
		case CD_STATE_EMPTY:
			this->SetState_EMPTY();
			break;
		case CD_STATE_START:
			this->SetState_START();
			break;
	}
}

void CCastleDeep::SetState_BLANK() // OK
{
	LogAdd(LOG_BLACK,"[Castle Deep] SetState DISABLED");
}

void CCastleDeep::SetState_EMPTY() // OK
{
	this->m_TimeNotify = 0;
	this->m_Stage = 1;
	this->m_Group = 0;

	this->ClearMonster();

	this->CheckSync();

	LogAdd(LOG_BLACK,"[Castle Deep] SetState EMPTY");
}

void CCastleDeep::SetState_START() // OK
{
	this->m_RemainTime = gServerInfo->m_CastleDeepEventTime*60;

	this->m_TargetTime = (int)(time(0)+this->m_RemainTime);

	while(this->m_Group == 0)
	{
		for(std::vector<CASTLE_DEEP_GROUP>::iterator it=this->m_CastleDeepGroup.begin();it != this->m_CastleDeepGroup.end();it++)
		{
			if((GetLargeRand()%100) < it->Rate)
			{
				this->m_Group = it->Index;
				break;
			}
		}
	}

	LogAdd(LOG_BLACK,"[Castle Deep] SetState START");
}

void CCastleDeep::CheckSync() // OK
{
	if(this->m_CastleDeepStartTime.empty() != 0)
	{
		this->SetState(CD_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<CASTLE_DEEP_START_TIME>::iterator it=this->m_CastleDeepStartTime.begin();it != this->m_CastleDeepStartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(CD_STATE_BLANK);
		return;
	}

	this->m_RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	this->m_TargetTime = (int)ScheduleTime.GetTime();

	int sec = this->m_RemainTime;

	int day = sec/86400;
	sec %= 86400;

	int hr = sec/3600;
	sec %= 3600;

	int min = sec/60;

	if(day > 0)
	{
		LogAdd(LOG_BLACK,"[Castle Deep] Sync Open Time. [%d] day [%d] hour [%d] min remain",day,hr,min);
	}
	else if(hr > 0)
	{
		LogAdd(LOG_BLACK,"[Castle Deep] Sync Open Time. [%d] hour [%d] min remain",hr,min);
	}
	else
	{
		LogAdd(LOG_BLACK,"[Castle Deep] Sync Open Time. [%d] min remain",min);
	}
}

void CCastleDeep::ClearMonster() // OK
{
	for(int n=0;n < MAX_OBJECT_MONSTER;n++)
	{
		if(gObjIsConnected(n) != 0 && gObj[n].Attribute == ATTRIBUTE_CASTLE)
		{
			gObjDel(n);
		}
	}
}

void CCastleDeep::AddMonster(int stage,int group) // OK 
{
	for(std::vector<CASTLE_DEEP_MONSTER>::iterator it=this->m_CastleDeepMonster.begin();it != this->m_CastleDeepMonster.end();it++)
	{
		if(it->Index != stage || it->Group != group)
		{
			continue;
		}

		for(int n=0;n < it->Count;n++)
		{
			int index = gMonster->AddMonster(MAP_CASTLE_SIEGE);

			if(OBJECT_RANGE(index) == 0)
			{
				continue;
			}

			LPOBJ lpObj = &gObj[index];

			int px = it->X;
			int py = it->Y;

			if(gObjGetRandomFreeLocation(MAP_CASTLE_SIEGE,&px,&py,(it->TX-it->X),(it->TY-it->Y),10) == 0)
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
			lpObj->Dir = GetLargeRand()%8;
			lpObj->Map = MAP_CASTLE_SIEGE;

			if(gMonster->SetMonster(index,it->Class) == 0)
			{
				gObjDel(index);
				continue;
			}

			lpObj->DieRegen = 0;
			lpObj->RegenTime = 1;
			lpObj->Attribute = ATTRIBUTE_CASTLE;
			lpObj->MaxRegenTime = 1000;
			lpObj->LastCheckTick = GetTickCount();
		}
	}
}

void CCastleDeep::MonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	int aIndex = gMonster->GetTopHitDamageUser(lpObj);

	if(OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	if(lpObj->Attribute == ATTRIBUTE_CASTLE && (lpObj->Class == 295 || (lpObj->Class >= 300 && lpObj->Class <= 303)))
	{
		gNotice->GCNoticeSendToAll(0,0,0,0,0,0,gMessage->GetMessage(55),lpObj->Name,lpTarget->Name);
	}
}

void CCastleDeep::ForceStart() // OK
{
	SYSTEMTIME SystemTime;

	GetLocalTime(&SystemTime);

	CASTLE_DEEP_START_TIME info;

	info.Year = SystemTime.wYear;

	info.Month = SystemTime.wMonth;

	info.Day = SystemTime.wDay;

	info.DayOfWeek = -1;

	info.Hour = SystemTime.wHour;

	info.Minute = (SystemTime.wMinute+1);

	info.Second = SystemTime.wSecond;

	this->m_CastleDeepStartTime.push_back(info);

	this->Init();
}