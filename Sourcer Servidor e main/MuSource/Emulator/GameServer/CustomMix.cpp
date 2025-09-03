// CustomMix.cpp: implementation of the CCustomMix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomMix.h"
#include "ItemManager.h"
#include "ReadFile.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomMix::CCustomMix() // OK
{
	#if(GAMESERVER_UPDATE>=301)

	this->m_CustomMixInfo.clear();

	#endif
}

CCustomMix::~CCustomMix() // OK
{

}

void CCustomMix::Load(char* path) // OK
{
	#if(GAMESERVER_UPDATE>=301)

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

	this->m_CustomMixInfo.clear();

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

					CUSTOM_MIX_INFO info;

					info.Index = lpReadFile->GetNumber();

					info.MixMoney = lpReadFile->GetAsNumber();

					info.MixRate[0] = lpReadFile->GetAsNumber();

					info.MixRate[1] = lpReadFile->GetAsNumber();

					info.MixRate[2] = lpReadFile->GetAsNumber();

					info.MixRate[3] = lpReadFile->GetAsNumber();

					strcpy_s(info.Name,lpReadFile->GetAsString());

					this->SetInfo(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpReadFile->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_MIX_GROUP_INFO info;

					info.Index = lpReadFile->GetNumber();

					info.Group = lpReadFile->GetAsNumber();

					info.BagSpecial = lpReadFile->GetAsNumber();

					this->SetGroupInfo(info);
				}
				else if(section == 2)
				{
					if(strcmp("end",lpReadFile->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_MIX_REQUIRE_ITEM_INFO info;

					info.Index = lpReadFile->GetNumber();

					info.Group = lpReadFile->GetAsNumber();

					info.Count = lpReadFile->GetAsNumber();

					info.ItemIndex = SafeGetItem(GET_ITEM(lpReadFile->GetAsNumber(),lpReadFile->GetAsNumber()));

					info.ItemLevel = lpReadFile->GetAsNumber();

					info.ItemOption1 = lpReadFile->GetAsNumber();

					info.ItemOption2 = lpReadFile->GetAsNumber();

					info.ItemOption3 = lpReadFile->GetAsNumber();

					info.ItemNewOption = lpReadFile->GetAsNumber();

					info.ItemSetOption = lpReadFile->GetAsNumber();

					this->SetItemInfo(info);
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

	#endif
}

void CCustomMix::SetInfo(CUSTOM_MIX_INFO info) // OK
{
	#if(GAMESERVER_UPDATE>=301)

	std::map<int,CUSTOM_MIX_INFO>::iterator it = this->m_CustomMixInfo.find(info.Index);

	if(it != this->m_CustomMixInfo.end())
	{
		return;
	}

	this->m_CustomMixInfo.insert(std::pair<int,CUSTOM_MIX_INFO>(info.Index,info));

	#endif
}

void CCustomMix::SetGroupInfo(CUSTOM_MIX_GROUP_INFO info) // OK
{
	#if(GAMESERVER_UPDATE>=301)

	std::map<int,CUSTOM_MIX_INFO>::iterator it = this->m_CustomMixInfo.find(info.Index);

	if(it == this->m_CustomMixInfo.end())
	{
		return;
	}

	std::map<int,CUSTOM_MIX_GROUP_INFO>::iterator ir = it->second.MixGroup.find(info.Group);

	if(ir != it->second.MixGroup.end())
	{
		return;
	}

	it->second.MixGroup.insert(std::pair<int,CUSTOM_MIX_GROUP_INFO>(info.Group,info));

	#endif
}

void CCustomMix::SetItemInfo(CUSTOM_MIX_REQUIRE_ITEM_INFO info) // OK
{
	#if(GAMESERVER_UPDATE>=301)

	std::map<int,CUSTOM_MIX_INFO>::iterator it = this->m_CustomMixInfo.find(info.Index);

	if(it == this->m_CustomMixInfo.end())
	{
		return;
	}

	std::map<int,CUSTOM_MIX_GROUP_INFO>::iterator ir = it->second.MixGroup.find(info.Group);

	if(ir == it->second.MixGroup.end())
	{
		return;
	}

	ir->second.RequireItem.insert(std::pair<int,CUSTOM_MIX_REQUIRE_ITEM_INFO>(0,info));

	#endif
}

CUSTOM_MIX_INFO* CCustomMix::GetInfoMix(int index) // OK
{
	#if(GAMESERVER_UPDATE>=301)

	std::map<int,CUSTOM_MIX_INFO>::iterator ir = this->m_CustomMixInfo.find(index);

	if(ir == this->m_CustomMixInfo.end())
	{
		return 0;
	}

	return &ir->second;

	#else

	return 0;

	#endif
}

bool CCustomMix::CheckItem(LPOBJ lpObj,CUSTOM_MIX_REQUIRE_ITEM_INFO* lpInfo) // OK
{
	#if(GAMESERVER_UPDATE>=301)

	int ItemCount = 0;

	for(int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index != lpInfo->ItemIndex)
		{
			continue;
		}

		if(lpInfo->ItemLevel != -1 && lpInfo->ItemLevel != lpObj->ChaosBox[n].m_Level)
		{
			continue;
		}

		if(lpInfo->ItemOption1 != -1 && lpObj->ChaosBox[n].m_Option1 != lpInfo->ItemOption1)
		{
			continue;
		}

		if(lpInfo->ItemOption2 != -1 && lpObj->ChaosBox[n].m_Option2 != lpInfo->ItemOption2)
		{
			continue;
		}

		if(lpInfo->ItemOption3 != -1 && lpObj->ChaosBox[n].m_Option3 < lpInfo->ItemOption3)
		{
			continue;
		}

		if(lpInfo->ItemNewOption != -1 && (lpInfo->ItemNewOption == 0 && lpObj->ChaosBox[n].IsExcItem() != 0))
		{
			continue;
		}

		if(lpInfo->ItemSetOption != -1 && (lpInfo->ItemSetOption == 0 && lpObj->ChaosBox[n].IsSetItem() != 0))
		{
			continue;
		}

		ItemCount++;
	}

	return (ItemCount == lpInfo->Count) ? 1 : 0;

	#else

	return 0;

	#endif
}

bool CCustomMix::CheckGroup(LPOBJ lpObj,CUSTOM_MIX_GROUP_INFO* lpInfo) // OK
{
	#if(GAMESERVER_UPDATE>=301)

	for(std::map<int,CUSTOM_MIX_REQUIRE_ITEM_INFO>::iterator it = lpInfo->RequireItem.begin(); it != lpInfo->RequireItem.end(); it++)
	{
		if(this->CheckItem(lpObj,&it->second) == 0)
		{
			return 0;
		}
	}

	#endif

	return 1;
}