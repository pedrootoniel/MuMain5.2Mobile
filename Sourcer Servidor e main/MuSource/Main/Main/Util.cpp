#include "stdafx.h"
#include "Util.h"
#include "Offset.h"

DWORD CalcConstA;
DWORD CalcConstB;
DWORD CharacterMaxLevel;

void SetByte(DWORD offset,BYTE value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,1,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(BYTE*)(offset)=value;

	VirtualProtect((void*)offset,1,OldProtect,&OldProtect);
}

void SetWord(DWORD offset,WORD value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,2,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(WORD*)(offset)=value;

	VirtualProtect((void*)offset,2,OldProtect,&OldProtect);
}

void SetDword(DWORD offset,DWORD value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,4,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(DWORD*)(offset)=value;

	VirtualProtect((void*)offset,4,OldProtect,&OldProtect);
}

void SetFloat(DWORD offset,float value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,4,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(float*)(offset)=value;

	VirtualProtect((void*)offset,4,OldProtect,&OldProtect);
}

void SetDouble(DWORD offset,double value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,8,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(double*)(offset)=value;

	VirtualProtect((void*)offset,8,OldProtect,&OldProtect);
}

void SetCompleteHook(BYTE head,DWORD offset,...) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,5,PAGE_EXECUTE_READWRITE,&OldProtect);

	if(head != 0xFF)
	{
		*(BYTE*)(offset)=head;
	}

	DWORD* function=&offset+1;

	*(DWORD*)(offset+1) =(*function)-(offset+5);

	VirtualProtect((void*)offset,5,OldProtect,&OldProtect);
}

void MemoryCpy(DWORD offset,void* value,DWORD size) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,size,PAGE_EXECUTE_READWRITE,&OldProtect);

	memcpy((void*)offset,value,size);

	VirtualProtect((void*)offset,size,OldProtect,&OldProtect);
}

void MemorySet(DWORD offset,DWORD value,DWORD size) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,size,PAGE_EXECUTE_READWRITE,&OldProtect);

	memset((void*)offset,value,size);

	VirtualProtect((void*)offset,size,OldProtect,&OldProtect);
}

void VirtualizeOffset(DWORD offset,DWORD size) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,size,PAGE_EXECUTE_READWRITE,&OldProtect);

	DWORD HookAddr =(DWORD)malloc(size+5);

	memcpy((void*)HookAddr,(void*)offset,size);

	*(BYTE*)(HookAddr+size)=0xE9;

	*(DWORD*)(HookAddr+size+1) =(offset+size)-((HookAddr+size)+5);

	*(BYTE*)(offset)=0xE9;

	*(DWORD*)(offset+1)=HookAddr-(offset+5);

	memset((void*)(offset+5),0x90,(size-5));

	VirtualProtect((void*)offset,size,OldProtect,&OldProtect);
}

void PacketArgumentEncrypt(char* out_buff,char* in_buff,int size) // OK
{
	BYTE XorTable[3]={0xFC,0xCF,0xAB};

	for(int n=0;n<size;n++)
	{
		out_buff[n]=in_buff[n]^XorTable[n%3];
	}
}

char* ConvertModuleFileName(char* name) // OK
{
	static char buff[MAX_PATH]={0};

	for(int n=strlen(name);n > 0;n--)
	{
		if(name[n] == '\\')
		{
			strcpy_s(buff,sizeof(buff),&name[(n+1)]);
			break;
		}
	}

	return buff;
}

void ErrorMessageBox(char* message,...) // OK
{
	char buff[256];

	memset(buff,0,sizeof(buff));

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	MessageBox(0,buff,"Error",MB_OK | MB_ICONERROR);
}

char* GetHardwareId() //OK
{
	char WinDir[MAX_PATH];

	if(GetSystemDirectory(WinDir,sizeof(WinDir)) == 0)
	{
		return 0;
	}

	WinDir[3]=0;

	DWORD VolumeSerialNumber;

	if(GetVolumeInformation(WinDir,0,0,&VolumeSerialNumber,0,0,0,0) == 0)
	{
		return 0;
	}

	SYSTEM_INFO SystemInfo;

	GetSystemInfo(&SystemInfo);

	DWORD ComputerHardwareId1 =(VolumeSerialNumber ^ VolumeSerialNumber)+0x12BA1074;
	
	DWORD ComputerHardwareId2 =(VolumeSerialNumber * VolumeSerialNumber)-0x13B06451;
	
	DWORD ComputerHardwareId3 =(VolumeSerialNumber | SystemInfo.dwNumberOfProcessors << 16) * 0x14CE1989;
	
	DWORD ComputerHardwareId4 =(VolumeSerialNumber | SystemInfo.wProcessorArchitecture << 16) / 4;
	
	DWORD ComputerHardwareId5 =((SystemInfo.wProcessorLevel & 0xF5FB) |(SystemInfo.wProcessorRevision << 16)) ^ 0x15CA2020;

	static char HardwareId[45];

	wsprintf(HardwareId,"%08X-%08X-%08X-%08X-%08X",ComputerHardwareId1,ComputerHardwareId2,ComputerHardwareId3,ComputerHardwareId4,ComputerHardwareId5);

	return HardwareId;
}

void SetExperienceTable(int MaxLevel,int ConstA,int ConstB,int DeleteLevel) // OK
{
	SetWord(0x00405B9B,DeleteLevel); // Delete Level
	SetWord(0x0040BCEE,DeleteLevel); // Delete Level Msg

	SetByte(0x0050B23D+2,(BYTE)ConstA); // CalcNextExperience 10
	SetByte(0x0071B5B1+2,(BYTE)ConstA); // CalcNextExperience 10

	SetWord(0x0050B276+2,(WORD)ConstB); // CalcNextExperience 1000
	SetWord(0x0071B5FD+2,(WORD)ConstB); // CalcNextExperience 1000

	CalcConstA = ConstA;
	CalcConstB = ConstB;
	CharacterMaxLevel = MaxLevel;
}

void Console(int color,char* text,...) // OK
{
	#if(DEBUG_CONSOLE==1)

	char buff[1024];

	va_list arg;
	va_start(arg,text);
	vsprintf_s(buff,text,arg);
	va_end(arg);

	wsprintf(buff,"%s\n",buff);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	switch(color)
	{
		case 0:
			SetConsoleTextAttribute(hConsole,7);
			break;
		case 1:
			SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
			break;
		case 2:
			SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN);
			break;
		case 3:
			SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE);
			break;
	}

	WriteConsole(hConsole,buff,strlen(buff),0,0);

	SetConsoleTextAttribute(hConsole,7);

	#endif
}