#pragma	once

#define	MAIN_WINDOW				0x07FE06F4
#define	MAIN_CONNECTION_STATUS	0x07FE8E38
#define	MAIN_SCREEN_STATE		0x00853434
#define	MAIN_CHARACTER_STRUCT	0x079FAE60
#define	MAIN_VIEWPORT_STRUCT	0x079B9D48
#define	MAIN_PACKET_SERIAL		0x07FE8E33
#define	MAIN_FONT_SIZE			0x079FAE78
#define	MAIN_RESOLUTION			0x07FE0520
#define	MAIN_RESOLUTION_X		0x00852B98
#define	MAIN_RESOLUTION_Y		0x00852B9C
#define	MAIN_PARTY_MEMBER_COUNT	0x07B55338
#define	MAIN_CURRENT_MAP		0x0084253C
#define	MAIN_HOOK_RECV			0x008125E8
#define	MAIN_HOOK_SEND			0x008125F8
#define MAIN_ACTIVE_SOCKET		0x07FE4BC0

#define DrawInterface			((void(*)(DWORD,float,float,float,float))0x006E26E3)
#define DrawInterfaceText		((void(*)(char*,int,int,int,int,int,int,int))0x0070EF47)
#define	ProtocolCore			((BOOL(*)(DWORD,BYTE*,DWORD,DWORD))0x0062C170)
#define	pGetPosFromAngle		((void(__cdecl*)(float*,int*,int*))0x005DEF20)
#define	pCursorX				*(int*)0x07FE0218
#define	pCursorY				*(int*)0x07FE0214
#define	pTextThis				((LPVOID(*)())0x0041D732)
#define	pDrawText				((int(__thiscall*)(LPVOID,int,int,char*,int,int,int,int))0x0041D9F1)
#define	pDrawBarForm			((void(__cdecl*)(float,float,float,float))0x005E093C)
#define	pDrawBigText			((float(_cdecl*)(float,float,DWORD,float,float))0x005E0768)
#define	pDrawImage				((void(*)(DWORD,float,float,float,float,float,float,float,float,int,int,GLfloat))0x005E0C03)
#define	pDrawMessage			((int(__cdecl*)(char*,int))0x0050C9F0)
#define	pLoadItemModel			((void(*)(int,char*,char*,int))0x005C5873)
#define	pLoadItemTexture		((void(*)(int,char*,int,int,int))0x005C51C0)
#define pViewportAddress		*(DWORD*)(0x079B9D40)
#define pChaosMixIndex			((DWORD(__thiscall*)(DWORD*))0x006EC8C0)(&*(DWORD*)(0x008A4BF0))

#define pRenderPartObjectEffect	((void(*)(DWORD,int,float*,float,int,int,int,int,int))0x005BD57B)
#define pTransformPosition      ((int(__thiscall*)(DWORD,DWORD,float*,float*,bool))0x004C6B8E)
#define pCreateSprite	        ((int(*)(int,float*,float,float*,DWORD,float,int))0x006A33B2)
#define pCreateParticle			((int(__cdecl*)(DWORD,float*,DWORD,float*,DWORD,float,DWORD))(0x00685A69))
#define pCreateEffect			((void(__cdecl*)(int,float*,DWORD,float*,int,DWORD,short,BYTE,float,BYTE,float*))0x0063CAB0)

#define pGetTextLine(x)			(((char*(__thiscall*)(void*,int))0x004024D0)(((void(*)())0x0079F5190),x))

#define pWindowThis				((LPVOID(*)())0x00750101)
#define CheckWindow(x)			((bool(__thiscall*)(LPVOID,DWORD))0x0074E829)(pWindowThis(),x)
#define CloseWindow(x)			((bool(__thiscall*)(LPVOID,DWORD))0x0074F13F)(pWindowThis(),x)
#define OpenWindow(x)			((bool(__thiscall*)(LPVOID,DWORD))0x0074E850)(pWindowThis(),x)
#define ToggleWindow(x)			((bool(__thiscall*)(LPVOID,DWORD))0x0074F975)(pWindowThis(),x)
#define HideAllWindow			((void(__thiscall*)(LPVOID))0x0074F9B9)(pWindowThis())

#define pLoadWaveFile			((void(*)(int,char*,int,int))0x0063942D)
#define pTextSetFont			((void(__thiscall*)(LPVOID,HFONT))0x0041D9C6)
#define	pSetTextColor			((void(__thiscall*)(LPVOID,BYTE,BYTE,BYTE,BYTE))0x0041D902)
#define	pSetBGTextColor			((void(__thiscall*)(LPVOID,BYTE,BYTE,BYTE,BYTE))0x0041D964)
#define pLoadImage				((int(*)(char*,int,GLint,GLint,int,int))0x006A92BE)
#define pPlayBuffer				((int(__cdecl*)(int,int,int))0x006397B5)
#define pRenderTipText			((void(*)(int,int,char*))0x0050C6F0)
#define pMouseOnZone			((bool(__cdecl*)(int,int,int,int,int))0x00416836)
#define pRenderBitmapRotate		((void(*)(int,float,float,float,float,float))0x005E0DD8)
#define pSetPlayerStop			((void(*)(DWORD))0x004CE9B5)

#define pGetFontDC				((HDC(__thiscall*)(LPVOID)) 0x0041D860)
#define pFontNormal				*(HFONT*)0x07FE0704
#define pFontBold				*(HFONT*)0x07FE0708
#define pFontBig				*(HFONT*)0x07FE070C
#define pFontFixed				*(HFONT*)0x07FE0710

#define	EnableLightMap			((void(*)())0x005DF6D7)
#define	EnableAlphaBlend		((void(*)())0x005DF41B)
#define	EnableAlphaBlend2		((void(*)())0x005DF530)
#define	EnableAlphaBlendMinus	((void(*)())0x005DF4A4)
#define	EnableAlphaTest			((void(*)(bool))0x005DF380)
#define	DisableAlphaBlend		((void(*)())0x005DF301)

#define ITEM_BASE_MODEL 819
#define GET_ITEM(x,y)(((x)*512)+(y))
#define GET_ITEM_MODEL(x,y)((((x)*512)+(y))+ITEM_BASE_MODEL)
#define GET_ITEM_OPT_LEVEL(x)((x>>3)&15)
#define GET_ITEM_OPT_EXC(x)(x-(x&64))

#define pTextureBlind			((void(__cdecl*)()) 0x005E0A1F)
#define IsRelease				((bool(__cdecl*)(int))0x006E298D)
#define IsPress					((bool(__cdecl*)(int))0x006E29A2)
#define pWindowOption			((LPVOID(__thiscall*)(LPVOID))0x007503DB)(pWindowThis())
#define SetAutoAttack			((void(__thiscall*)(LPVOID,bool))0x0074107C)
#define IsAutoAttack			((bool(__thiscall*)(LPVOID))0x00741095)
#define SetWhisperSound 		((void(__thiscall*)(LPVOID,bool))0x007410A9)
#define IsWhisperSound 			((bool(__thiscall*)(LPVOID))0x007410C2)
#define SetSlideHelp 			((void(__thiscall*)(LPVOID,bool))0x007410D6)
#define IsSlideHelp 			((bool(__thiscall*)(LPVOID))0x007410EF)
#define SetVolumeLevel 			((void(__thiscall*)(LPVOID,int))0x00741103)
#define GetVolumeLevel 			((int(__thiscall*)(LPVOID))0x0074111C)
#define SetRenderLevel 			((void(__thiscall*)(LPVOID,int))0x00741130)
#define GetRenderLevel 			((int(__thiscall*)(LPVOID))0x00741149)

#define GetItemEquiped(x)			(*(DWORD*)(0x079FAE5C)+(4320+(93*x)))
#define GetItemEquipedIndex(x)		(*(WORD*)(GetItemEquiped(x)))
#define InventoryDeleteItem			((DWORD*(__thiscall*)(LPVOID,int))0x0073320C)
#define InventoryFindEmptySlot		((int(__thiscall*)(LPVOID,int,int))0x007344BF)
#define InventoryFindItem			((DWORD(__thiscall*)(LPVOID,int,int))0x0071387A)
#define InventoryThis				((LPVOID(__thiscall*)(LPVOID))0x007501C5)
#define InventoryUnequipItem		((DWORD*(__thiscall*)(LPVOID,int))0x00732A27)
#define ResetMouseLButton			((void(*)())0x0073890A)
#define ResetMouseRButton			((void(*)())0x0073892A)
#define SendRequestEquipmentItem	((bool(_cdecl*)(int,int,DWORD,int,int))0x006EC990)
#define InventoryPointer			((LPVOID(__thiscall*)(LPVOID))0x007343FE)
#define InventoryFindItemSlot		((int(__thiscall*)(LPVOID,int,int))0x007138A8)
#define MixGetMyInventoryCtrl		((LPVOID(__thiscall*)(LPVOID))0x0072C5D7)
#define MixInventoryThis			((LPVOID(__thiscall*)(LPVOID))0x007501F8)
#define InventoryMixDeleteItem		((DWORD*(__thiscall*)(LPVOID,int))0x00745E09)
#define InventoryMixFindEmptySlot	((int(__thiscall*)(LPVOID,int,int))0x00713905)
#define PickedItem					(*(DWORD*)0x08270250)

#define IsInputEnable			((bool(*)())0x0045C765)

//#define GetScreenWidth				((int(*)())0x005906B0)

#define MouseOnWindow			*(bool*)0x079FAE8C
#define MouseLButton			*(bool*)0x07FE0260
#define MouseRButton			*(bool*)0x07FE0248
#define MouseLButtonPush		*(bool*)0x07FE00C0
#define MouseRButtonPush		*(bool*)0x07FE026C
#define MouseLButtonPop			*(bool*)0x07FE00D8
#define MouseRButtonPop			*(bool*)0x07FE022C
#define MouseUpdateTime			*(int*)0x07A8E0D0
#define MouseUpdateTimeMax		*(int*)0x00840BC4

#define EventChipDialogEnable	*(int*)0x07B553D8
#define pDrawButton				((void(__cdecl*)(DWORD,float,float,float,float,float,float))0x006E27D3) //S6 -> 0x00790E40)

#define InventoryItemCount		((int(__thiscall*)(LPVOID,short))0x007387ED)

#define pglViewport2			((void(*)(int,int,int,int))0x005DF763)
#define pgluPerspective2		((void(*)(float,float,float,float))0x005DECD2)
#define pGetOpenGLMatrix		((void(*)(LPVOID))0x005DEC67)
#define pCameraMatrix			(LPVOID*)0x07FE00DC
#define pEnableDepthTest		((void(*)())0x005DF1C9)
#define pEnableDepthMask		((void(*)())0x005DF20D)
#define pRenderItem3D			((void(*)(float,float,float,float,int,int,int,int,int))0x005939D0)
#define pBeginBitmap			((void(*)())0x005E0826)
#define pUpdateMousePosition	((void(*)())0x005DFA39)

#define Attacking				*(int*)0x00840C30

#define SiegeWarfare			((LPVOID(__thiscall*)(LPVOID))0x006C603E)
#define SiegeInitMiniMapUI		((DWORD*(__thiscall*)(LPVOID))0x0075048F)

enum INTERFACE_LIST
{
	INTERFACE_BEGIN,
	INTERFACE_FRIEND,
	INTERFACE_MOVEMAP,
	INTERFACE_PARTY,
	INTERFACE_MYQUEST,
	INTERFACE_NPCQUEST,
	INTERFACE_GUILDINFO,
	INTERFACE_TRADE,
	INTERFACE_STORAGE,
	INTERFACE_MIXINVENTORY,
	INTERFACE_COMMAND,
	INTERFACE_PET,
	INTERFACE_NPCSHOP,
	INTERFACE_INVENTORY,
	INTERFACE_MYSHOP_INVENTORY,
	INTERFACE_PURCHASESHOP_INVENTORY,
	INTERFACE_CHARACTER,
	INTERFACE_NPCBREEDER,
	INTERFACE_SERVERDIVISION,
	INTERFACE_DEVILSQUARE,
	INTERFACE_BLOODCASTLE,

	INTERFACE_NPCGUILDMASTER,
	INTERFACE_GUARDSMAN,
	INTERFACE_SENATUS,
	INTERFACE_GATEKEEPER,
	INTERFACE_GATESWITCH,
	INTERFACE_CATAPULT,
	INTERFACE_REFINERY,
	INTERFACE_REFINERYINFO,
	INTERFACE_KANTURU2ND_ENTERNPC,
	INTERFACE_CURSEDTEMPLE_NPC,
	INTERFACE_CURSEDTEMPLE_GAMESYSTEM,
	INTERFACE_CURSEDTEMPLE_RESULT,
	INTERFACE_CHATINPUTBOX,
	INTERFACE_WINDOW_MENU,
	INTERFACE_OPTION,
	INTERFACE_HELP,
	INTERFACE_ITEM_EXPLANATION,
	INTERFACE_SETITEM_EXPLANATION,
	INTERFACE_QUICK_COMMAND,

	INTERFACE_KANTURU_INFO,
	INTERFACE_CHATLOGWINDOW,
	INTERFACE_PARTY_INFO_WINDOW,
	INTERFACE_BLOODCASTLE_TIME,
	INTERFACE_CHAOSCASTLE_TIME,
	INTERFACE_BATTLE_SOCCER_SCORE,
	INTERFACE_SLIDEWINDOW,
	INTERFACE_HERO_POSITION_INFO,
	INTERFACE_MESSAGEBOX,
	INTERFACE_DUEL_WINDOW,
	INTERFACE_CRYWOLF,
	INTERFACE_NAME_WINDOW,
	INTERFACE_SIEGEWARFARE,
	INTERFACE_MAINFRAME,
	INTERFACE_SKILL_LIST,
	INTERFACE_ITEM_ENDURANCE_INFO,
	INTERFACE_BUFF_WINDOW,

	INTERFACE_MASTER_LEVEL,
	INTERFACE_GOLD_BOWMAN,
	INTERFACE_LUCKYCOIN_REGISTRATION,
	INTERFACE_EXCHANGE_LUCKYCOIN,
};

struct CHARACTER_ATTRIBUTE
{
	char Name[11];
	BYTE Class;
	BYTE Skin;
	WORD Level;
	DWORD Experience;
	DWORD NextExperince;
	WORD Strength;
	WORD Dexterity;
	WORD Vitality;
	WORD Energy;
	WORD Leadership;
	WORD Life;
	WORD Mana;
	WORD LifeMax;
	WORD ManaMax;
	WORD Shield;
	WORD ShieldMax;
	WORD AttackRatingPK;
	WORD SuccessfulBlockingPK;
	WORD AddStrength;
	WORD AddDexterity;
	WORD AddVitality;
	WORD AddEnergy;
	WORD AddLifeMax;
	WORD AddManaMax;
	WORD AddLeadership;
	WORD SkillMana;
	WORD SkillManaMax;
	BYTE Ability;
	WORD AbilityTime[3];
	short AddPoint;
	short MaxAddPoint;
	WORD MinusPoint;
	WORD MaxMinusPoint;
	WORD AttackSpeed;
	WORD AttackRating;
	WORD AttackDamageMinRight;
	WORD AttackDamageMaxRight;
	WORD AttackDamageMinLeft;
	WORD AttackDamageMaxLeft;
	WORD MagicSpeed;
	WORD MagicDamageMin;
	WORD MagicDamageMax;
	WORD CurseDamageMin;
	WORD CurseDamageMax;
	WORD CriticalDamage;
	WORD SuccessfulBlocking;
	WORD Defense;
	WORD MagicDefense;
	WORD WalkSpeed;
	WORD LevelUpPoint;
};