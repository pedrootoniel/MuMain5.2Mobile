#pragma once

#define GetHotKeyItemIndex ((int(__thiscall*)(DWORD,int,bool))0x0071C635)
#define FindItem ((DWORD(__thiscall*)(LPVOID,short))0x0073440F)
#define CNewUISystem__GetInstance ((DWORD(_cdecl*)()) 0x00750101)
#define GetUI_NewUIItemHotKey ((DWORD(__thiscall*)(DWORD This)) 0x00750181)
#define GetUI_NewUISkillList ((DWORD(__thiscall*)(DWORD This)) 0x00750192)
#define	RenderNumber2D ((float(_cdecl*)(float,float,int,float,float))0x005E0768)
#define g_isCharacterBuff ((bool(__thiscall*)(DWORD,char))0x005EF7BB)
#define RenderSkillIcon ((void(__thiscall*)(DWORD,int,float,float,float,float))0x0071EB61)
#define RenderSkillInfo ((void(__thiscall*)(DWORD))0x0071E94E)
#define m_CrywolfState *(BYTE*)0x82711E5

enum IMAGE_LIST
{
	IMAGE_MENU_1 = 31288,// Menu01_new.jpg
	IMAGE_MENU_2,		// Menu02.jpg
	IMAGE_MENU_3,		// Menu03_new.jpg
	IMAGE_MENU_4,		// Menu_04.tga
	IMAGE_GAUGE_BLUE,	// Menu_Blue.jpg
	IMAGE_GAUGE_GREEN,	// Menu_Green.jpg
	IMAGE_GAUGE_RED,	// Menu_Red.jpg
	IMAGE_GAUGE_AG,		// Menu03_new_AG.jpg
	IMAGE_GAUGE_SD,		// menu01_new2_SD.jpg
	IMAGE_MENU_BTN_COMMAND,	// menu01_new2.jpg
	IMAGE_MENU_BTN_PARTY,	// Menu_Party.jpg
	IMAGE_MENU_BTN_CHAINFO,	// Menu_Character.jpg
	IMAGE_MENU_BTN_MYINVEN,	// Menu_Inventory.jpg
	IMAGE_MENU_BTN_FRIEND,	// win_push.jpg
	IMAGE_MENU_BTN_GUILD,	// Guild.jpg
	IMAGE_SKILLBOX = 40000, // SkillBox.jpg
};

enum EVENT_STATE
{
	EVENT_NONE = 0,
	EVENT_BTN_HOVER_CURRENTSKILL,
	EVENT_BTN_DOWN_CURRENTSKILL,
	EVENT_BTN_HOVER_SKILLHOTKEY,
	EVENT_BTN_DOWN_SKILLHOTKEY,
	EVENT_BTN_HOVER_SKILLLIST,
	EVENT_BTN_DOWN_SKILLLIST,
};

void InitDowngrade();
void LoadImages();
void RenderButtons();
void RenderFrame();
void RenderLifeMana();
void RenderGuageSD();
void RenderGuageAG();
void RenderExperience();
void RenderItems();
void RenderItemCount();
void UseItemRButton();
void RenderCurrentSkill();
bool RenderSkillList();
bool SkillListMouseEvent();
float RenderSkillNumber(float x, float y, int num, float scale);
bool IsMasterLevel();