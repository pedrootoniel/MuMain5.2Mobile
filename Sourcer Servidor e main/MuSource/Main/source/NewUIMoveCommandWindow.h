// NewUIMoveCommandWindow.h: interface for the CNewUIMoveCommandWindow class.
//////////////////////////////////////////////////////////////////////
#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "MoveCommandData.h"
#include "NewUIChatLogWindow.h"

namespace SEASON3B
{
	class CNewUIMoveCommandWindow : public CNewUIObj
	{	
		enum 
		{
			MOVECOMMAND_SCROLLBTN_WIDTH = 15,
			MOVECOMMAND_SCROLLBTN_HEIGHT = 30,
			MOVECOMMAND_SCROLLBAR_TOP_WIDTH = 7,
			MOVECOMMAND_SCROLLBAR_TOP_HEIGHT = 3,
			MOVECOMMAND_SCROLLBAR_MIDDLE_WIDTH = 7,
			MOVECOMMAND_SCROLLBAR_MIDDLE_HEIGHT = 15,
			MOVECOMMAND_MAX_RENDER_TEXTLINE = 25,	

		};

		enum MOVECOMMAND_MOUSE_EVENT
		{
			MOVECOMMAND_MOUSEBTN_NORMAL = 0,
			MOVECOMMAND_MOUSEBTN_OVER,
			MOVECOMMAND_MOUSEBTN_CLICKED,
		};
	private:
	
		CNewUIManager*				m_pNewUIMng;
		POINT						m_Pos;
		int							m_iRealFontHeight;
		std::list<CMoveCommandData::MOVEINFODATA*>	m_listMoveInfoData;
		POINT						m_StartUISubjectName;
		POINT						m_StartMapNamePos;
		POINT						m_MapNameUISize;
		POINT						m_MapNamePos;
		POINT						m_ReqLevelPos;
		POINT						m_ReqZenPos;
		int							m_iSelectedMapName;
		POINT						m_ScrollBarPos;
		POINT						m_ScrollBtnStartPos;
		POINT						m_ScrollBtnPos;
		int							m_iScrollBarHeightPixel;
		int							m_iRenderStartTextIndex;
		int							m_iRenderEndTextIndex;
		int							m_iSelectedTextIndex;
		int							m_iScrollBtnInterval;
		int							m_iScrollBarMiddleNum;
		int							m_iScrollBarMiddleRemainderPixel;
		int							m_iNumPage;
		int							m_iCurPage;
		int							m_iTotalMoveScrBtnPixel;
		int							m_iRemainMoveScrBtnPixel;
		int							m_icurMoveScrBtnPixelperStep;
		int							m_iMaxMoveScrBtnPixelperStep;
		int							m_iMinMoveScrBtnPixelperStep;
		int							m_iTotalMoveScrBtnperStep;
		int							m_iRemainMoveScrBtnperStep;
		int							m_iTotalNumMaxMoveScrBtnperStep;
		int							m_iTotalNumMinMoveScrBtnperStep;
		int							m_iAcumMoveMouseScrollPixel;
		int							m_iMousePosY;
		int							m_iScrollBtnMouseEvent;
		bool						m_bScrollBtnActive;
		DWORD						m_dwMoveCommandKey;

	public:
		CNewUIMoveCommandWindow();
		virtual ~CNewUIMoveCommandWindow();
		
		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		
		void SetPos(int x, int y);
		
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		
		bool BtnProcess();
		
		virtual void OpenningProcess();
		void ClosingProcess();
		float GetLayerDepth();

		bool IsLuckySealBuff();
		bool IsMapMove( const std::string& src );

		void SetMoveCommandKey(DWORD dwKey);
		DWORD GetMoveCommandKey();

		BOOL IsTheMapInDifferentServer(const int iFromMapIndex, const int iToMapIndex) const;
		int GetMapIndexFromMovereq(const char *pszMapName);

	private:
		void SettingCanMoveMap();
		void RenderFrame();
		void UpdateScrolling();
		void ScrollUp(int iMoveValue);
		void ScrollDown(int iMoveValue);
		void RecursiveCalcScroll(IN int piScrollValue, OUT int* piMovePixel, bool bSign = true);
		void LoadImages();
		void UnloadImages();
	};
};



