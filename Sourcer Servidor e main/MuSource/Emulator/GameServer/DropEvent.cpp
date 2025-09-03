// DropEvent.cpp: implementation of the CDropEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DropEvent.h"
#include "CrywolfSync.h"
#include "DSProtocol.h"
#include "GameServer.h"
#include "ItemDrop.h"
#include "ItemManager.h"
#include "ItemOptionRate.h"
#include "MapManager.h"
#include "ReadFile.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "RandomManager.h"
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDropEvent::CDropEvent() // OK
{
	for(int n = 0; n < MAX_DROP_EVENT; n++)
	{
		DROP_EVENT_INFO* lpInfo = &this->m_EventDropInfo[n];

		lpInfo->Index = n;
		lpInfo->State = DROP_EVENT_STATE_BLANK;
		lpInfo->RemainTime = 0;
		lpInfo->TargetTime = 0;
		lpInfo->TickCount = GetTickCount();

		lpInfo->StartTime.clear();

		lpInfo->ItemInfo.clear();
	}
}

CDropEvent::~CDropEvent() // OK
{

}

void CDropEvent::Init() // OK
{
	for(int n = 0; n < MAX_DROP_EVENT; n++)
	{
		if(gServerInfo->m_DropEventSwitch == 0)
		{
			this->SetState(&this->m_EventDropInfo[n],DROP_EVENT_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_EventDropInfo[n],DROP_EVENT_STATE_EMPTY);
		}
	}
}

void CDropEvent::Load(char* path) // OK
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

	for(int n = 0; n < MAX_DROP_EVENT; n++)
	{
		EditMenuLabel(2,1,2600+n,"<EMPTY>",0);
		this->m_EventDropInfo[n].StartMessage = -1;
		this->m_EventDropInfo[n].FinalMessage = -1;
		this->m_EventDropInfo[n].DropEventTime = 0;
		this->m_EventDropInfo[n].StartTime.clear();
		this->m_EventDropInfo[n].ItemInfo.clear();
	}

	try
	{
		while (true)
		{
			if(lpReadFile->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpReadFile->GetNumber();

			while (true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpReadFile->GetAsString()) == 0)
					{
						break;
					}

					DROP_EVENT_START_TIME info;

					int index = lpReadFile->GetNumber();

					info.Year = lpReadFile->GetAsNumber();

					info.Month = lpReadFile->GetAsNumber();

					info.Day = lpReadFile->GetAsNumber();

					info.DayOfWeek = lpReadFile->GetAsNumber();

					info.Hour = lpReadFile->GetAsNumber();

					info.Minute = lpReadFile->GetAsNumber();

					info.Second = lpReadFile->GetAsNumber();

					this->m_EventDropInfo[index].StartTime.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpReadFile->GetAsString()) == 0)
					{
						break;
					}

					int index = lpReadFile->GetNumber();

					this->m_EventDropInfo[index].StartMessage = lpReadFile->GetAsNumber();

					this->m_EventDropInfo[index].FinalMessage = lpReadFile->GetAsNumber();

					this->m_EventDropInfo[index].DropEventTime = lpReadFile->GetAsNumber();

					strcpy_s(this->m_EventDropInfo[index].Name,lpReadFile->GetAsString());

					EditMenuLabel(2,1,2600+index,this->m_EventDropInfo[index].Name,1);
				}
				else if(section == 2)
				{
					if(strcmp("end",lpReadFile->GetAsString()) == 0)
					{
						break;
					}

					DROP_EVENT_ITEM_INFO info;

					memset(&info,0,sizeof(info));

					int index = lpReadFile->GetNumber();

					info.ItemIndex = SafeGetItem(GET_ITEM(lpReadFile->GetAsNumber(),lpReadFile->GetAsNumber()));

					info.Level = lpReadFile->GetAsNumber();

					info.Durability = lpReadFile->GetAsNumber();

					info.Option0 = lpReadFile->GetAsNumber();

					info.Option1 = lpReadFile->GetAsNumber();

					info.Option2 = lpReadFile->GetAsNumber();

					info.Option3 = lpReadFile->GetAsNumber();

					info.Option4 = lpReadFile->GetAsNumber();

					info.Option5 = lpReadFile->GetAsNumber();

					#if(GAMESERVER_UPDATE>=401)

					info.Option6 = lpReadFile->GetAsNumber();

					#endif

					info.Duration = lpReadFile->GetAsNumber();

					info.MapNumber = lpReadFile->GetAsNumber();

					info.MonsterClass = lpReadFile->GetAsNumber();

					info.MonsterLevelMin = lpReadFile->GetAsNumber();

					info.MonsterLevelMax = lpReadFile->GetAsNumber();

					info.AccountLevel = lpReadFile->GetAsNumber();

					info.DropRate = lpReadFile->GetAsNumber();

					this->m_EventDropInfo[index].ItemInfo.push_back(info);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadFile->GetLastError());
	}

	delete lpReadFile;
}

void CDropEvent::MainProc() // OK
{
	if(gServerInfo->m_DropEventSwitch == 0)
	{
		return;
	}

	for(int n = 0; n < MAX_DROP_EVENT; n++)
	{
		DROP_EVENT_INFO* lpInfo = &this->m_EventDropInfo[n];

		if((GetTickCount()- lpInfo->TickCount) >= 1000)
		{
			lpInfo->TickCount = GetTickCount();

			lpInfo->RemainTime = (int)difftime(lpInfo->TargetTime,time(0));

			switch(lpInfo->State)
			{
				case DROP_EVENT_STATE_BLANK:
					this->ProcState_BLANK(lpInfo);
					break;
				case DROP_EVENT_STATE_EMPTY:
					this->ProcState_EMPTY(lpInfo);
					break;
				case DROP_EVENT_STATE_START:
					this->ProcState_START(lpInfo);
					break;
			}
		}
	}
}

void CDropEvent::ProcState_BLANK(DROP_EVENT_INFO* lpInfo) // OK
{

}

void CDropEvent::ProcState_EMPTY(DROP_EVENT_INFO* lpInfo) // OK
{
	if(lpInfo->RemainTime <= 0)
	{
		LogAdd(LOG_BLACK,"[Drop Event] (%d) SetState START",lpInfo->Index);
		if(lpInfo->StartMessage != -1) { gNotice->GCNoticeSendToAll(0,0,0,0,0,0,"%s",gMessage->GetMessage(lpInfo->StartMessage)); }
		this->SetState(lpInfo,DROP_EVENT_STATE_START);
	}
}

void CDropEvent::ProcState_START(DROP_EVENT_INFO* lpInfo) // OK
{
	if(lpInfo->RemainTime <= 0)
	{
		LogAdd(LOG_BLACK,"[Drop Event] (%d) SetState EMPTY",lpInfo->Index);
		if(lpInfo->FinalMessage != -1) { gNotice->GCNoticeSendToAll(0,0,0,0,0,0,"%s",gMessage->GetMessage(lpInfo->FinalMessage)); }
		this->SetState(lpInfo,DROP_EVENT_STATE_EMPTY);
	}
}

void CDropEvent::SetState(DROP_EVENT_INFO* lpInfo,int state) // OK
{
	if(gServerInfo->m_DropEventSwitch == 0)
	{
		return;
	}

	lpInfo->State = state;

	switch(lpInfo->State)
	{
		case DROP_EVENT_STATE_BLANK:
			this->SetState_BLANK(lpInfo);
			break;
		case DROP_EVENT_STATE_EMPTY:
			this->SetState_EMPTY(lpInfo);
			break;
		case DROP_EVENT_STATE_START:
			this->SetState_START(lpInfo);
			break;
	}
}

void CDropEvent::SetState_BLANK(DROP_EVENT_INFO* lpInfo) // OK
{
	
}

void CDropEvent::SetState_EMPTY(DROP_EVENT_INFO* lpInfo) // OK
{
	this->CheckSync(lpInfo);
}

void CDropEvent::SetState_START(DROP_EVENT_INFO* lpInfo) // OK
{
	lpInfo->RemainTime = lpInfo->DropEventTime;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void CDropEvent::CheckSync(DROP_EVENT_INFO* lpInfo) // OK
{
	if(lpInfo->StartTime.empty() != 0)
	{
		this->SetState(lpInfo,DROP_EVENT_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<DROP_EVENT_START_TIME>::iterator it = lpInfo->StartTime.begin(); it != lpInfo->StartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpInfo,DROP_EVENT_STATE_BLANK);
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
		LogAdd(LOG_BLACK,"[Drop Event] (%d) Sync Start Time. [%d] day [%d] hour [%d] min remain",lpInfo->Index,day,hr,min);
	}
	else if(hr > 0)
	{
		LogAdd(LOG_BLACK,"[Drop Event] (%d) Sync Start Time. [%d] hour [%d] min remain",lpInfo->Index,hr,min);
	}
	else
	{
		LogAdd(LOG_BLACK,"[Drop Event] (%d) Sync Start Time. [%d] min remain",lpInfo->Index,min);
	}
}

int CDropEvent::GetState(int index) // OK
{
	if(CHECK_RANGE(index,MAX_DROP_EVENT) == 0)
	{
		return DROP_EVENT_STATE_BLANK;
	}
	else
	{
		return this->m_EventDropInfo[index].State;
	}
}

int CDropEvent::GetRemainTime(int index) // OK
{
	if(CHECK_RANGE(index,MAX_DROP_EVENT) == 0)
	{
		return 0;
	}

	DROP_EVENT_INFO* lpInfo = &this->m_EventDropInfo[index];

	if(lpInfo->StartTime.empty() != 0)
	{
		return 0;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<DROP_EVENT_START_TIME>::iterator it = lpInfo->StartTime.begin(); it != lpInfo->StartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		return 0;
	}

	int RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	return (((RemainTime % 60) == 0) ? (RemainTime/60) : ((RemainTime/60)+1));
}

int CDropEvent::DropItem(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(gMapManager->GetMapItemDrop(lpTarget->Map) == 0)
	{
		return 0;
	}

	CRandomManager RandomManager;

	for(int n = 0; n < MAX_DROP_EVENT; n++)
	{
		if(this->m_EventDropInfo[n].State == DROP_EVENT_STATE_START)
		{
			for(std::vector<DROP_EVENT_ITEM_INFO>::iterator it = this->m_EventDropInfo[n].ItemInfo.begin(); it != this->m_EventDropInfo[n].ItemInfo.end(); it++)
			{
				int DropRate;

				ITEM_INFO ItemInfo;

				if(gItemManager->GetInfo(it->ItemIndex,&ItemInfo) == 0)
				{
					continue;
				}

				if(it->MapNumber != -1 && it->MapNumber != lpObj->Map)
				{
					continue;
				}

				if(it->MonsterClass != -1 && it->MonsterClass != lpObj->Class)
				{
					continue;
				}

				if(it->MonsterLevelMin != -1 && it->MonsterLevelMin > lpObj->Level)
				{
					continue;
				}

				if(it->MonsterLevelMax != -1 && it->MonsterLevelMax < lpObj->Level)
				{
					continue;
				}

				if(it->AccountLevel != -1 && it->AccountLevel != lpTarget->AccountLevel)
				{
					continue;
				}

				if((DropRate = it->DropRate) == -1 || (GetLargeRand() % 1000000) < (DropRate = this->GetItemDropRate(lpObj,lpTarget,it->ItemIndex,it->DropRate)))
				{
					int rate = (1000000/((DropRate == -1) ? 1000000 : DropRate));

					RandomManager.AddElement((int)(&(*it)),rate);
				}
			}
		}
	}

	DROP_EVENT_ITEM_INFO* lpItemDropInfo;

	if(RandomManager.GetRandomElement((int*)&lpItemDropInfo) == 0)
	{
		return 0;
	}
	else
	{
		WORD ItemIndex = lpItemDropInfo->ItemIndex;
		BYTE ItemLevel = lpItemDropInfo->Level;
		BYTE ItemDur = lpItemDropInfo->Durability;
		BYTE ItemOption1 = 0;
		BYTE ItemOption2 = 0;
		BYTE ItemOption3 = 0;
		BYTE ItemNewOption = 0;
		BYTE ItemSetOption = 0;
		BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

		gItemOptionRate->GetItemOption0(lpItemDropInfo->Option0,&ItemLevel);

		gItemOptionRate->GetItemOption1(lpItemDropInfo->Option1,&ItemOption1);

		gItemOptionRate->GetItemOption2(lpItemDropInfo->Option2,&ItemOption2);

		gItemOptionRate->GetItemOption3(lpItemDropInfo->Option3,&ItemOption3);

		gItemOptionRate->GetItemOption4(lpItemDropInfo->Option4,&ItemNewOption);

		gItemOptionRate->GetItemOption5(lpItemDropInfo->Option5,&ItemSetOption);

		gItemOptionRate->GetItemOption6(lpItemDropInfo->Option6,&ItemSocketOption[0]);

		gItemOptionRate->MakeNewOption(ItemIndex,1,ItemNewOption,&ItemNewOption);

		gItemOptionRate->MakeSetOption(ItemIndex,ItemSetOption,&ItemSetOption);

		gItemOptionRate->MakeSocketOption(ItemIndex,ItemSocketOption[0],&ItemSocketOption[0]);

		GDCreateItemSend(lpTarget->Index,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y,ItemIndex,ItemLevel,ItemDur,ItemOption1,ItemOption2,ItemOption3,lpTarget->Index,ItemNewOption,ItemSetOption,0,0,ItemSocketOption,0xFF,((lpItemDropInfo->Duration>0)?((DWORD)time(0)+lpItemDropInfo->Duration):0));

		return 1;
	}
}

int CDropEvent::GetItemDropRate(LPOBJ lpObj,LPOBJ lpTarget,int ItemIndex,int DropRate) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	if(gItemManager->IsJewelItem(ItemIndex) != 0 && gCrywolfSync->CheckApplyPenalty() != 0 && gCrywolfSync->GetOccupationState() == 1)
	{
		if((GetLargeRand() % 100) >= gCrywolfSync->GetGemDropPenaltiyRate())
		{
			return 0;
		}
	}

	#endif

	return DropRate;
}

void CDropEvent::ForceStart(int index) // OK
{
	if(gServerInfo->m_DropEventSwitch == 0)
	{
		return;
	}

	if(index < 0 || index >= MAX_DROP_EVENT)
	{
		return;
	}

	DROP_EVENT_INFO* lpInfo = &this->m_EventDropInfo[index];

	if(this->GetState(index) == DROP_EVENT_STATE_BLANK)
	{
		return;
	}

	SYSTEMTIME SystemTime;

	GetLocalTime(&SystemTime);

	DROP_EVENT_START_TIME info;

	info.Year = SystemTime.wYear;

	info.Month = SystemTime.wMonth;

	info.Day = SystemTime.wDay;

	info.DayOfWeek = -1;

	info.Hour = SystemTime.wHour;

	info.Minute = SystemTime.wMinute;

	info.Second = SystemTime.wSecond;

	lpInfo->StartTime.push_back(info);

	this->SetState(lpInfo,DROP_EVENT_STATE_EMPTY);
}