// Notice.cpp: implementation of the CNotice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Notice.h"
#include "ReadFile.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNotice::CNotice() // OK
{
	this->m_NoticeInfo.clear();

	this->m_NoticeTime = time(0);

	this->m_NoticeValue = 0;
}

CNotice::~CNotice() // OK
{

}

void CNotice::Load(char* path) // OK
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

	this->m_NoticeInfo.clear();

	try
	{
		while(true)
		{
			if(lpReadFile->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpReadFile->GetString()) == 0)
			{
				break;
			}

			NOTICE_INFO info;

			memset(&info,0,sizeof(info));

			strcpy_s(info.Message,lpReadFile->GetString());

			info.Type = lpReadFile->GetAsNumber();

			info.Count = lpReadFile->GetAsNumber();

			info.Opacity = lpReadFile->GetAsNumber();

			info.Delay = lpReadFile->GetAsNumber();

			info.Color = 0;
			
			info.Color |= lpReadFile->GetAsNumber();
			
			info.Color |= (lpReadFile->GetAsNumber() << 8);
			
			info.Color |= (lpReadFile->GetAsNumber() << 16);
			
			info.Color |= (info.Opacity << 24);

			info.Speed = lpReadFile->GetAsNumber();

			info.Time = lpReadFile->GetAsNumber();

			this->m_NoticeInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpReadFile->GetLastError());
	}

	delete lpReadFile;
}

void CNotice::MainProc() // OK
{
	if(this->m_NoticeInfo.empty())
	{
		NOTICE_INFO* lpInfo = &this->m_NoticeInfo[this->m_NoticeValue];

		if((time(0)-this->m_NoticeTime) >= lpInfo->Time)
		{
			if((++this->m_NoticeValue) >= (int)this->m_NoticeInfo.size())
			{
				this->m_NoticeValue = 0;
			}

			this->m_NoticeTime = time(0);

			this->GCNoticeSendToAll(lpInfo->Type,lpInfo->Count,lpInfo->Opacity,lpInfo->Delay,lpInfo->Color,lpInfo->Speed,"%s",lpInfo->Message);
		}
	}
}

void CNotice::GCNoticeSend(int aIndex,BYTE type,BYTE count,BYTE opacity,WORD delay,DWORD color,BYTE speed,char* message,...) // OK
{
	char buff[256] = {0};

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	int size = strlen(buff);

	size = ((size>MAX_MESSAGE_SIZE)?MAX_MESSAGE_SIZE:size);

	PMSG_NOTICE_SEND pMsg;

	pMsg.header.set(0x0D,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	pMsg.type = type;

	pMsg.count = count;

	pMsg.opacity = opacity;

	pMsg.delay = delay;

	pMsg.color = color;

	pMsg.speed = speed;

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CNotice::GCNoticeSendToAll(BYTE type,BYTE count,BYTE opacity,WORD delay,DWORD color,BYTE speed,char* message,...) // OK
{
	char buff[256] = {0};

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	int size = strlen(buff);

	size = ((size>MAX_MESSAGE_SIZE)?MAX_MESSAGE_SIZE:size);

	PMSG_NOTICE_SEND pMsg;

	pMsg.header.set(0x0D,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	pMsg.type = type;

	pMsg.count = count;

	pMsg.opacity = opacity;

	pMsg.delay = delay;

	pMsg.color = color;

	pMsg.speed = speed;

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
		}
	}
}