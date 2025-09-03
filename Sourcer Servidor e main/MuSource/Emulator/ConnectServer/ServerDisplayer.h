// ServerDisplayer.h: interface for the CServerDisplayer class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_LOG_TEXT_LINE 29
#define MAX_LOG_TEXT_SIZE 200

enum eLogColor
{
	LOG_BLACK = 0,
	LOG_RED = 1,
	LOG_GREEN = 2,
	LOG_BLUE = 3,
};

struct LOG_DISPLAY_INFO
{
	char text[MAX_LOG_TEXT_SIZE];
	eLogColor color;
};

class CServerDisplayer
{
	CServerDisplayer();
	virtual ~CServerDisplayer();
	SingletonInstance(CServerDisplayer)
public:
	void Init(HWND hWnd);
	void Run();
	void LogTextPaint();
	void PaintBarInfo();
	void LogAddText(eLogColor color,char* text,int size);
private:
	HWND m_hwnd;
	HBRUSH m_brush;
	HFONT m_font;
	LOG_DISPLAY_INFO m_log[MAX_LOG_TEXT_LINE];
	int m_count;
};

#define gServerDisplayer SingNull(CServerDisplayer)
