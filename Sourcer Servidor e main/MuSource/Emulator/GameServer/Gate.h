// Gate.h: interface for the CGate class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct GATE_INFO
{
	int Index;
	int Flag;
	int Map;
	int X;
	int Y;
	int TX;
	int TY;
	int TargetGate;
	int Dir;
	int MinLevel;
	int MaxLevel;
	int MinReset;
	int MaxReset;
};

class CGate
{
	CGate();
	virtual ~CGate();
	SingletonInstance(CGate)
public:
	void Load(char* path);
	bool GetInfo(int index,GATE_INFO* lpInfo);
	int GetGateMap(int index);
	int GetMoveLevel(LPOBJ lpObj,int map,int level);
	bool IsGate(int index);
	bool IsInGate(LPOBJ lpObj,int index);
	bool GetGate(int index,int* gate,int* map,int* x,int* y,int* dir,int* level);
public:
	std::map<int,GATE_INFO> m_GateInfo;
};

#define gGate SingNull(CGate)