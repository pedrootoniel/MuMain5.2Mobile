// MasterSkillTree.cpp: implementation of the CMasterSkillTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MasterSkillTree.h"
#include "QueryManager.h"
#include "SocketManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMasterSkillTree::CMasterSkillTree() // OK
{

}

CMasterSkillTree::~CMasterSkillTree() // OK
{

}

void CMasterSkillTree::GDMasterSkillTreeRecv(SDHP_MASTER_SKILL_TREE_RECV* lpMsg,int index) // OK
{
	SDHP_MASTER_SKILL_TREE_SEND pMsg;

	pMsg.header.set(0x0D,0x00,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	if(gQueryManager->ExecQuery("SELECT * FROM MasterSkillTree WHERE Name='%s'",lpMsg->name) == 0 || gQueryManager->Fetch() == SQL_NO_DATA)
	{
		gQueryManager->Close();

		pMsg.MasterLevel = 0;

		pMsg.MasterPoint = 0;

		pMsg.MasterExperience = 0;
	}
	else
	{
		pMsg.MasterLevel = gQueryManager->GetAsInteger("MasterLevel");

		pMsg.MasterPoint = gQueryManager->GetAsInteger("MasterPoint");

		pMsg.MasterExperience = gQueryManager->GetAsInteger64("MasterExperience");

		gQueryManager->Close();
	}

	gSocketManager->DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
}

void CMasterSkillTree::GDMasterSkillTreeSaveRecv(SDHP_MASTER_SKILL_TREE_SAVE_RECV* lpMsg) // OK
{
	if(gQueryManager->ExecQuery("SELECT Name FROM MasterSkillTree WHERE Name='%s'",lpMsg->name) == 0 || gQueryManager->Fetch() == SQL_NO_DATA)
	{
		gQueryManager->Close();
		gQueryManager->ExecQuery("INSERT INTO MasterSkillTree (Name,MasterLevel,MasterPoint,MasterExperience) VALUES ('%s',%d,%d,%I64d)",lpMsg->name,lpMsg->MasterLevel,lpMsg->MasterPoint,lpMsg->MasterExperience);
		gQueryManager->Close();
	}
	else
	{
		gQueryManager->Close();
		gQueryManager->ExecQuery("UPDATE MasterSkillTree SET MasterLevel=%d,MasterPoint=%d,MasterExperience=%I64d WHERE Name='%s'",lpMsg->MasterLevel,lpMsg->MasterPoint,lpMsg->MasterExperience,lpMsg->name);
		gQueryManager->Close();
	}
}
