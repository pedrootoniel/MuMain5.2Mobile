// CLanguage.cpp: implementation of the CLanguage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Language.h"
#include "CustomMessage.h"
#include "Util.h"

char lang[4],filename[13][MAX_PATH];

void InitLanguage() // OK
{
	HKEY key;

	if(RegOpenKeyEx(HKEY_CURRENT_USER,"SOFTWARE\\Webzen\\Mu\\Config",0,KEY_ALL_ACCESS,&key) == ERROR_SUCCESS)
	{
		DWORD type=REG_SZ,size=sizeof(lang);

		if(RegQueryValueEx(key,"LangSelection",0,&type,(BYTE*)lang,&size) != ERROR_SUCCESS)
		{
			strcpy_s(lang,"Eng");
		}

		RegCloseKey(key);
	}

	if(_stricmp(lang,"Eng") != 0 && _stricmp(lang,"Por") != 0 && _stricmp(lang,"Spn") != 0)
	{
		strcpy_s(lang,"Eng");
	}

	if(_stricmp(lang,"Por") == 0)
	{
		gCustomMessage.m_LangSelected = 1;
	}
	else if(_stricmp(lang,"Spn") == 0)
	{
		gCustomMessage.m_LangSelected = 2;
	}
	else
	{
		gCustomMessage.m_LangSelected = 0;
	}

	wsprintf(filename[0],"Data\\Local\\%s\\BuffEffect_%s.bmd",lang,lang);
	wsprintf(filename[1],"Data\\Local\\%s\\Dialog_%s.bmd",lang,lang);
	wsprintf(filename[2],"Data\\Local\\%s\\Item_%s.bmd",lang,lang);
	wsprintf(filename[3],"Data\\Local\\%s\\ItemSetOption_%s.bmd",lang,lang);
	wsprintf(filename[4],"Data\\Local\\%s\\JewelOfHarmonyOption_%s.bmd",lang,lang);
	wsprintf(filename[5],"Data\\Local\\%s\\JewelOfHarmonySmelt_%s.bmd",lang,lang);
	wsprintf(filename[6],"Data\\Local\\%s\\Movereq_%s.bmd",lang,lang);
	wsprintf(filename[7],"Data\\Local\\%s\\NpcName_%s.txt",lang,lang);
	wsprintf(filename[8],"Data\\Local\\%s\\Quest_%s.bmd",lang,lang);
	wsprintf(filename[9],"Data\\Local\\%s\\Skill_%s.bmd",lang,lang);
	wsprintf(filename[10],"Data\\Local\\%s\\Slide_%s.bmd",lang,lang);
	wsprintf(filename[11],"Data\\Local\\%s\\SocketItem_%s.bmd",lang,lang);
	wsprintf(filename[12],"Data\\Local\\%s\\Text_%s.bmd",lang,lang);

	SetDword(0x005F1E07,(DWORD)&filename[0]);
	SetDword(0x005DBCA3,(DWORD)&filename[1]);
	SetDword(0x005DBC62,(DWORD)&filename[2]);
	SetDword(0x0048FBA3,(DWORD)&filename[3]);
	SetDword(0x0045BC77,(DWORD)&filename[4]);
	//SetDword(0x006B2249,(DWORD)&filename[5]);
	SetDword(0x005DBCC8,(DWORD)&filename[6]);
	SetDword(0x005DBD22,(DWORD)&filename[7]);
	SetDword(0x005DBCB0,(DWORD)&filename[8]);
	SetDword(0x005DBC6F,(DWORD)&filename[9]);
	SetDword(0x00748B8E,(DWORD)&filename[10]);
	SetDword(0x004BD6A6,(DWORD)&filename[11]);
	SetDword(0x005DBE97,(DWORD)&filename[12]);
}