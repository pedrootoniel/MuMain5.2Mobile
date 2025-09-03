// Notice.h: interface for the CNotice class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_NOTICE_SEND
{
	PBMSG_HEAD header; // C1:0D
	BYTE type;
	BYTE count;
	BYTE opacity;
	WORD delay;
	DWORD color;
	BYTE speed;
	char message[256];
};

//**********************************************//
//**********************************************//
//**********************************************//

struct NOTICE_INFO
{
	char Message[128];
	int Type;
	int Count;
	int Opacity;
	int Delay;
	int Color;
	int Speed;
	int Time;
};

class CNotice
{
	CNotice();
	virtual ~CNotice();
	SingletonInstance(CNotice)
public:
	void Load(char* path);
	void MainProc();
	void GCNoticeSend(int aIndex,BYTE type,BYTE count,BYTE opacity,WORD delay,DWORD color,BYTE speed,char* message,...);
	void GCNoticeSendToAll(BYTE type,BYTE count,BYTE opacity,WORD delay,DWORD color,BYTE speed,char* message,...);
private:
	std::vector<NOTICE_INFO> m_NoticeInfo;
	time_t m_NoticeTime;
	int m_NoticeValue;
};

#define gNotice SingNull(CNotice)