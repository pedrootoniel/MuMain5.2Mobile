#include "stdafx.h"
#include "resource.h"
#include "Attack.h"
#include "Main.h"
#include "Camera.h"
#include "ChaosBox.h"
#include "ChatWindow.h"
#include "CCRC32.H"
#include "Common.h"
#include "Downgrade.h"
#include "EventEntryLevel.h"
#include "Fog.h"
#include "Font.h"
#include "GoldenArcher.h"
#include "HackCheck.h"
#include "HealthBar.h"
#include "Item.h"
#include "ItemMove.h"
#include "ItemToolTip.h"
#include "Language.h"
#include "Map.h"
#include "MiniMap.h"
#include "Monster.h"
#include "Offset.h"
#include "OptionsMenu.h"
#include "PacketManager.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "Protocol.h"
#include "Reconnect.h"
#include "Resolution.h"
#include "ServerList.h"
#include "Shop.h"
#include "TrayMode.h"
#include "Util.h"
#include "WindowTime.h"

HHOOK HookKB,HookMS;
HINSTANCE hins;

LRESULT CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam) // OK
{
	if(nCode == HC_ACTION)
	{
		if(((DWORD)lParam & (1 << 30)) != 0 && ((DWORD)lParam & (1 << 31)) != 0 && GetForegroundWindow() == *(HWND*)(MAIN_WINDOW))
		{
			if(*(DWORD*)(MAIN_SCREEN_STATE) == 5 && wParam == 72)
			{
				WindowButtonToggle();
			}
			else if(gProtect.m_MainInfo.KeyCodeAutoAttackSwitch != 0 && wParam == gProtect.m_MainInfo.KeyCodeAutoAttackSwitch)
			{
				AutoAttackToggle();
			}
			else if(gProtect.m_MainInfo.KeyCodeHealthBarSwitch != 0 && wParam == gProtect.m_MainInfo.KeyCodeHealthBarSwitch)
			{
				HealthBarToggle();
			}
			else if(gProtect.m_MainInfo.KeyCodeCamera3DSwitch != 0 && wParam == gProtect.m_MainInfo.KeyCodeCamera3DSwitch)
			{
				gCamera.Toggle();
			}
			else if(gProtect.m_MainInfo.KeyCodeCamera3DRestore != 0 && wParam == gProtect.m_MainInfo.KeyCodeCamera3DRestore)
			{
				gCamera.Restore();
			}
			else if(gProtect.m_MainInfo.KeyCodeTrayModeSwitch != 0 && wParam == gProtect.m_MainInfo.KeyCodeTrayModeSwitch)
			{
				gTrayMode.Toggle();
			}
		}
	}

	return CallNextHookEx(HookKB,nCode,wParam,lParam);
}

LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam) // OK
{
	if(nCode == HC_ACTION)
	{
		MOUSEHOOKSTRUCTEX* HookStruct =(MOUSEHOOKSTRUCTEX*)lParam;

		if(GetForegroundWindow() == *(HWND*)(MAIN_WINDOW))
		{
			switch(wParam)
			{
				case WM_MOUSEMOVE:
					gCamera.Move(HookStruct);
					break;
				case WM_MBUTTONDOWN:
					gCamera.SetIsMove(1);
					gCamera.SetCursorX(HookStruct->pt.x);
					gCamera.SetCursorY(HookStruct->pt.y);
					break;
				case WM_MBUTTONUP:
					gCamera.SetIsMove(0);
					break;
				case WM_MOUSEWHEEL:
					gCamera.Zoom(HookStruct);
					break;
			}
		}
	}

	return CallNextHookEx(HookMS,nCode,wParam,lParam);
}

SHORT WINAPI KeysProc(int nCode) // OK
{
	if(GetForegroundWindow() != *(HWND*)(MAIN_WINDOW))
	{
		return 0;
	}

	return GetAsyncKeyState(nCode);
}

HICON WINAPI IconProc(HINSTANCE hInstance,LPCSTR lpIconName) // OK
{
	FILE* file;

	if(fopen_s(&file,".\\main.ico","r") != 0)
	{
		gTrayMode.m_TrayIcon = (HICON)LoadImage(hins,MAKEINTRESOURCE(IDI_CLIENT),IMAGE_ICON,GetSystemMetrics(SM_CXICON),GetSystemMetrics(SM_CYICON),LR_DEFAULTCOLOR);
	}
	else
	{
		fclose(file);
		gTrayMode.m_TrayIcon = (HICON)LoadImage(hins,".\\main.ico",IMAGE_ICON,GetSystemMetrics(SM_CXICON),GetSystemMetrics(SM_CYICON),LR_LOADFROMFILE | LR_DEFAULTCOLOR);
	}

	return gTrayMode.m_TrayIcon;
}

LRESULT WINAPI WindowProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam) // OK
{
	switch (msg)
	{
		case WM_LBUTTONDOWN:
		{
			if(MouseRButton || MouseRButtonPush)
			{
				return 0;
			}
		}

		case WM_RBUTTONDOWN:
		{
			if(MouseLButton || MouseLButtonPush)
			{
				return 0;
			}
		}
	}

	return CallWindowProc((WNDPROC)(0x005E32C0),hWnd,msg,wParam,lParam);
}

void WINAPI ReduceConsumeProc() // OK
{
	while(true)
	{
		Sleep(5000);
		SetProcessWorkingSetSize(GetCurrentProcess(),0xFFFFFFFF,0xFFFFFFFF);
		SetThreadPriority(GetCurrentProcess(),THREAD_PRIORITY_LOWEST);
	}
}

extern "C" _declspec(dllexport) void EntryProc() // OK
{
	if(gProtect.ReadMainFile("ServerInfo.sse") != 0)
	{
		SetByte(0x0083FB1C,0xA0); // Accent
		SetByte(0x005E5200,0xEB); // Crack (mu.exe)
		SetByte(0x005E551A,0xEB); // Crack (GameGuard)
		SetByte(0x005E5563,0xE9); // Crack (GameGuard)
		SetByte(0x005E5564,0x8A); // Crack (GameGuard)
		SetByte(0x005E5565,0x00); // Crack (GameGuard)
		SetByte(0x005E5566,0x00); // Crack (GameGuard)
		SetByte(0x005E5567,0x00); // Crack (GameGuard)
		SetByte(0x0060112F,0xEB); // Crack (ResourceGuard)
		SetByte(0x005ECAEB,0xE9); // Website
		SetByte(0x005ECAEC,0xA7); // Website
		SetByte(0x005ECAED,0x00); // Website
		SetByte(0x005008E6,0xEB); // Corrupted Filter.bmd
		SetByte(0x0048FC6A,0xEB); // Corrupted ItemSetType.bmd
		SetByte(0x0048FE9F,0xEB); // Corrupted ItemSetOption.bmd
		SetByte(0x004B832B,0xEB); // Ctrl Fix
		SetByte(0x004B8339,0x02); // Ctrl Fix
		SetByte(0x005E516D,0xEB); // Multi Instance
		SetByte(0x004F6EEA,0x0D); // Fix Effect +13
		SetByte(0x00853FF4,(gProtect.m_MainInfo.ClientVersion[0]+1)); // Version
		SetByte(0x00853FF5,(gProtect.m_MainInfo.ClientVersion[2]+2)); // Version
		SetByte(0x00853FF6,(gProtect.m_MainInfo.ClientVersion[3]+3)); // Version
		SetByte(0x00853FF7,(gProtect.m_MainInfo.ClientVersion[5]+4)); // Version
		SetByte(0x00853FF8,(gProtect.m_MainInfo.ClientVersion[6]+5)); // Version
		SetWord(0x00853430,(gProtect.m_MainInfo.IpAddressPort)); // IpAddressPort
		SetDword(0x005E41CC,(DWORD)gProtect.m_MainInfo.WindowName);
		SetDword(0x005ECCB2,(DWORD)gProtect.m_MainInfo.ScreenShotPath);
		SetDword(0x00812480,(DWORD)KeysProc);
		SetDword(0x00812530,(DWORD)IconProc);
		SetDword(0x005E41DA,(DWORD)WindowProc);
		SetDword(0x004F6C7F+3,0x100); // Fix Evolution
		SetDword(0x004F6CC1+3,0x120); // Fix Evolution
		SetDword(0x004F6D04+3,0x140); // Fix Evolution
		SetDword(0x004F6D46+3,0x160); // Fix Evolution

		SetByte(0x00425B3F,0xEB); // Show Castle Guild Register Guilds 00425AA3

		MemorySet(0x0060144B,0x90,10); // C1:F3:04

		MemorySet(0x005E2333,0x90,0x0E); // Remove MuError.log

		MemorySet(0x004D1CF3,0x90,0x3F); // Remove Reflect Effect

		MemoryCpy(0x0083FB5C,gProtect.m_MainInfo.IpAddress,sizeof(gProtect.m_MainInfo.IpAddress)); // IpAddress

		MemoryCpy(0x00853FFC,gProtect.m_MainInfo.ClientSerial,sizeof(gProtect.m_MainInfo.ClientSerial)); // ClientSerial

		SetCompleteHook(0xE8,0x00575F55,&DrawNewHealthBar);

		SetCompleteHook(0xFF,0x0062B9A3,&ProtocolCoreEx);

		gCustomEffect.Load(gProtect.m_MainInfo.CustomEffectInfo);

		gCustomFog.Load(gProtect.m_MainInfo.CustomFogInfo);

		gCustomItem.Load(gProtect.m_MainInfo.CustomItemInfo);

		gCustomJewel.Load(gProtect.m_MainInfo.CustomJewelInfo);

		gCustomMap.Load(gProtect.m_MainInfo.CustomMapInfo);

		gCustomMessage.LoadEng(gProtect.m_MainInfo.EngCustomMessageInfo);

		gCustomMessage.LoadPor(gProtect.m_MainInfo.PorCustomMessageInfo);

		gCustomMessage.LoadSpn(gProtect.m_MainInfo.SpnCustomMessageInfo);

		gCustomMonster.Load(gProtect.m_MainInfo.CustomMonsterInfo);

		gCustomTooltip.Load(gProtect.m_MainInfo.CustomTooltipInfo);

		gCustomWing.Load(gProtect.m_MainInfo.CustomWingInfo);

		gSkillAttackSpeed.Load(gProtect.m_MainInfo.SkillAttackSpeedInfo);

		gPacketManager.LoadEncryptionKey("Data\\Enc1.dat");

		gPacketManager.LoadDecryptionKey("Data\\Dec2.dat");

		gOptionsMenu.Init();

		InitChaosBox();

		InitChatWindow();

		InitCommon();

		InitDowngrade();

		InitEventEntryLevel();

		InitFog();

		InitFont();

		InitGoldenArcher();

		InitHackCheck();

		InitItem();

		InitItemMove();

		InitItemToolTip();

		InitBundle();

		InitJewel();

		InitLanguage();

		InitWing();

		InitMap();

		InitMiniMap();

		InitMonster();

		InitPrintPlayer();

		InitReconnect();

		InitResolution();

		InitServerList();

		InitShop();

		InitWindowTime();

		gProtect.CheckLauncher();

		gProtect.CheckInstance();

		gProtect.CheckClientFile();

		gProtect.CheckPluginFile();

		HookKB=SetWindowsHookEx(WH_KEYBOARD,KeyboardProc,hins,GetCurrentThreadId());

		HookMS=SetWindowsHookEx(WH_MOUSE,MouseProc,hins,GetCurrentThreadId());

		if(gProtect.m_MainInfo.ReduceConsumption != 0)
		{
			CreateThread(0,0,(LPTHREAD_START_ROUTINE)ReduceConsumeProc,0,0,0);
		}

		#if(DEBUG_CONSOLE == 1)

		if(AllocConsole() == 0)
		{
			ErrorMessageBox("Could not open AllocConsole()");
			return;
		}

		SetConsoleTitleA("Debugger");

		DeleteMenu(GetSystemMenu(GetConsoleWindow(),0),SC_CLOSE,MF_BYCOMMAND);

		#endif
	}
	else
	{
		ErrorMessageBox("Could not load ServerInfo.sse!");
		gProtect.SafeExitProcess();
	}
}

BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved) // OK
{
	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			hins =(HINSTANCE)hModule;
			break;
	}

	return 1;
}