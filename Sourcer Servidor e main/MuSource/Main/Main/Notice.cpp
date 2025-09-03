#include "stdafx.h"
#include "Notice.h"
#include "Offset.h"

NOTICE_INFO m_NoticeInfo[MAX_NOTICE];

int NoticeCount = 0;
int NoticeTime = 300;

void ClearNotice() // OK
{
	for(int n = 0; n < MAX_NOTICE; n++)
	{
		memset(&m_NoticeInfo[n],0,sizeof(m_NoticeInfo[n]));
	}

	NoticeCount = 0;

	NoticeTime = 300;
}

void RenderNotice() // OK
{
	EnableAlphaTest(true);

	pTextSetFont(pTextThis(),pFontNormal);

	glColor3f(1.0f,1.0f,1.0f);

	pSetBGTextColor(pTextThis(),0,0,0,128);

	int line = MAX_NOTICE;

	int count = (((NoticeCount-1)>=0)?(NoticeCount-1):(MAX_NOTICE-1));

	for(int n = 0; n < MAX_NOTICE; n++)
	{
		switch(m_NoticeInfo[count].color)
		{
			case 0:
				pSetTextColor(pTextThis(),150,255,240,255);
				break;
			case 1:
				pSetTextColor(pTextThis(),100,120,255,255);
				break;
			case 2:
				pSetTextColor(pTextThis(),140,180,255,255);
				break;
			case 3:
				pSetTextColor(pTextThis(),200,220,255,255);
				break;
			case 4:
				pSetTextColor(pTextThis(),255,150,60,255);
				break;
			case 5:
				pSetTextColor(pTextThis(),255,80,30,255);
				break;
			default:
				pSetTextColor(pTextThis(),255,0,0,255);
				break;
		}

		pDrawText(pTextThis(),0,(30+(line*10)),m_NoticeInfo[count].text,0,0,0,0);

		count = (((--count)>=0)?count:(MAX_NOTICE-1));
		line--;
	}
}

void ScrollNotice() // OK
{
	if((--NoticeTime) < 1)
	{
		NoticeAdd(0,"");
	}
}

void NoticeAdd(BYTE color,char* text) // OK
{
	NoticeTime = 300;

	int size = strlen(text);

	size = ((size>=MAX_NOTICE_SIZE)?(MAX_NOTICE_SIZE-1):size);

	memset(&m_NoticeInfo[NoticeCount].text,0,sizeof(m_NoticeInfo[NoticeCount].text));

	memcpy(&m_NoticeInfo[NoticeCount].text,text,size);

	m_NoticeInfo[NoticeCount].color = color;

	NoticeCount = (((++NoticeCount)>=MAX_NOTICE)?0:NoticeCount);
}