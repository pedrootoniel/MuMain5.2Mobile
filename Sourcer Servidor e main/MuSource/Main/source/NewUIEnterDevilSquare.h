// NewUIEnterDevilSquare.h: interface for the CNewUIPartyInfo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ENTERDEVILSQUARE_H_
#define _ENTERDEVILSQUARE_H_

#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIMyInventory.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"

namespace SEASON3B
{
	class CNewUIEnterDevilSquare : public CNewUIObj  
	{
	public:
		enum IMAGE_LIST
		{		
			IMAGE_ENTERDS_BASE_WINDOW_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP,			
			IMAGE_ENTERDS_BASE_WINDOW_LOW = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LOW,			

			IMAGE_ENTERDS_BASE_WINDOW_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,		//. newui_exit_00.tga

			IMAGE_ENTERDS_BASE_WINDOW_BTN_ENTER = CNewUIMessageBoxMng::IMAGE_MSGBOX_BTN_EMPTY_BIG	//. newui_btn_empty_big.tga	(180, 87)
		};
		
	private:
		enum ENTERDS_WINDOW_SIZE
		{
			ENTERDS_BASE_WINDOW_WIDTH = 190,
			ENTERDS_BASE_WINDOW_HEIGHT = 429,
		};
		
		enum ENTERDS_ENTERBTN_STATE
		{
			ENTERBTN_DISABLE = 0,
			ENTERBTN_ENABLE,
		};
		
		enum
		{
			ENTER_BTN_VAL = 33,			// ��ư ������ ����

			MAX_ENTER_GRADE = 7,
		};

	private:
		CNewUIManager*				m_pNewUIMng;
		POINT						m_Pos;
		POINT						m_BtnEnterStartPos;
		POINT						m_EnterUITextPos;

		int							m_iDevilSquareLimitLevel[MAX_ENTER_GRADE*2][2]; 
		int							m_iNumActiveBtn;		// Ȱ��ȭ �Ǿ��ִ� ��ư
		DWORD						m_dwBtnTextColor[2];	// 0 - Disabled, 1 - Enable 
		
	public:
		CNewUIEnterDevilSquare();
		virtual ~CNewUIEnterDevilSquare();
		
		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		
		void SetPos(int x, int y);
		
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();

		bool BtnProcess();
		
		float GetLayerDepth();	//. 4.0f
		
		void OpenningProcess();
		void ClosingProcess();
		
	private:
		void SetBtnPos(int x, int y);
		void LoadImages();
		void UnloadImages();

		int	CheckLimitLV(int iIndex);
	};
}

#endif // _ENTERDEVILSQUARE_H_
