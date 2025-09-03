#include "stdafx.h"
#include "Font.h"
#include "Offset.h"
#include "Util.h"

int FontSize;

DWORD FontCharSet;

char FontName[100];

int ReadFontFile(char* path) // OK
{	
	FontSize = GetPrivateProfileInt("FontInfo","FontSize",13,path);
	
	FontCharSet = GetPrivateProfileInt("FontInfo","FontCharSet",1,path);
	
	GetPrivateProfileString("FontInfo","FontName","Tahoma",FontName,sizeof(FontName),path);

	return GetPrivateProfileInt("FontInfo","ChangeFontSwitch",0,path);
}

void InitFont() // OK
{
	if(ReadFontFile(".\\Config.ini") != 0)
	{
		SetCompleteHook(0xE8,0x005E5B15,&FontNormal);
		
		SetCompleteHook(0xE8,0x005E5B7C,&FontBool);
		
		SetCompleteHook(0xE8,0x005E5BE5,&FontBig);
		
		SetCompleteHook(0xE8,0x005E5C4E,&FontFixed);
		
		SetByte(0x005E5B1A,0x90);
		
		SetByte(0x005E5B81,0x90);
		
		SetByte(0x005E5BEA,0x90);
		
		SetByte(0x005E5C53,0x90);
	}
}

void ReloadFont() // OK
{
	pFontNormal = FontNormal();

	pFontBold = FontBool();

	pFontBig = FontBig();

	pFontFixed = FontFixed();

	*(DWORD*)MAIN_FONT_SIZE = FontSize;

	((void(*)())0x005D93D2)();

	((void(__thiscall*)(LPVOID))0x0041D81A)(pTextThis());

	((void(__thiscall*)(LPVOID,int,HDC))0x0041D77F)(pTextThis(),0,*(HDC*)0x07FE06FC);

	((void(__thiscall*)(DWORD,DWORD,DWORD,DWORD,DWORD))0x006D9BEB)(*(DWORD*)((DWORD)pWindowThis()+12),*(DWORD*)(pWindowThis()),*(DWORD*)((DWORD)pWindowThis()+16),0,382);
}

HFONT FontNormal() // OK
{
	return CreateFont(FontSize,0,0,0,FW_NORMAL,0,0,0,FontCharSet,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,FontName);
}

HFONT FontBool() // OK
{
	return CreateFont(FontSize,0,0,0,FW_BOLD,0,0,0,FontCharSet,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,FontName);
}

HFONT FontBig() // OK
{
	return CreateFont(FontSize*2,0,0,0,FW_BOLD,0,0,0,FontCharSet,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,FontName);
}

HFONT FontFixed() // OK
{
	return CreateFont(FontSize,0,0,0,FW_NORMAL,0,0,0,FontCharSet,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,FontName);
}