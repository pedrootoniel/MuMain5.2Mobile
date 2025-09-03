#include "stdafx.h"
#include "PCRoom.h"
#include "ZzzInterface.h"	
#include "UsefulDef.h"		
#include "UIManager.h"		
#include "NewUISystem.h"

extern int GetScreenWidth();
extern int PartyNumber;

CPCRoomPtSys::CPCRoomPtSys() : m_bPCRoom(false), m_nNowPt(0), m_nMaxPt(0), m_bPCRoomPtShopMode(false), m_byBuyItemPos(0xff)
{
	/* */
}

CPCRoomPtSys::~CPCRoomPtSys()
{
	/* */
}

CPCRoomPtSys& CPCRoomPtSys::Instance()
{
	static CPCRoomPtSys s_PCRoomPtSys;

	return s_PCRoomPtSys;
}

