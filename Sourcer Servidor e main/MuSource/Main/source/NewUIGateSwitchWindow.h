// NewUIGateSwitchWindow.h: interface for the CNewUIGateSwitchWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWUIGATESWITCHWINDOW_H__89BA066C_7870_4064_B38E_F2F5AA919F9F__INCLUDED_)
#define AFX_NEWUIGATESWITCHWINDOW_H__89BA066C_7870_4064_B38E_F2F5AA919F9F__INCLUDED_

#pragma once

#include "NewUIBase.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"

namespace SEASON3B
{
	class CNewUIGateSwitchWindow : public CNewUIObj  
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_GATESWITCHWINDOW_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP,	
			IMAGE_GATESWITCHWINDOW_LOW = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LOW,
			
			IMAGE_GATESWITCHWINDOW_EXIT_BTN = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,
			IMAGE_GATESWITCHWINDOW_BUTTON = CNewUIMessageBoxMng::IMAGE_MSGBOX_BTN_EMPTY,
		};

	private:
		enum
		{
			INVENTORY_WIDTH = 190,
			INVENTORY_HEIGHT = 429,
		};

		CNewUIManager*			m_pNewUIMng;
		POINT m_Pos;

		CNewUIButton m_BtnExit;
		CNewUIButton m_BtnOpen;			// 열기 버튼

	public:
		CNewUIGateSwitchWindow();
		virtual ~CNewUIGateSwitchWindow();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		
		void SetPos(int x, int y);
		
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();

		void OpeningProcess();
		void ClosingProcess();

		float GetLayerDepth();	//. 5.0f

	private:
		void LoadImages();
		void UnloadImages();

		void RenderFrame();
		bool BtnProcess();

		void InitButton(CNewUIButton * pNewUIButton, int iPos_x, int iPos_y, const unicode::t_char * pCaption);

		void RenderOutlineUpper(float fPos_x, float fPos_y, float fWidth, float fHeight);
		void RenderOutlineLower(float fPos_x, float fPos_y, float fWidth, float fHeight);
	};
}
#endif // !defined(AFX_NEWUIGATESWITCHWINDOW_H__89BA066C_7870_4064_B38E_F2F5AA919F9F__INCLUDED_)
