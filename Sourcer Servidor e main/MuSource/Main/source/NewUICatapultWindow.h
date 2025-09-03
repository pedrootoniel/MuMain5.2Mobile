// NewUICatapultWindow.h: interface for the CNewUICatapultWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWUICATAPULTWINDOW_H__064BC38C_5F26_4003_A6C7_7270A11DEEBF__INCLUDED_)
#define AFX_NEWUICATAPULTWINDOW_H__064BC38C_5F26_4003_A6C7_7270A11DEEBF__INCLUDED_

#pragma once

#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIButton.h"

namespace SEASON3B
{
	
	class CNewUICatapultWindow	: public CNewUIObj	
	{
	private:

		class CCatapultGroupButton
		{
		public:
			CCatapultGroupButton();
			virtual ~CCatapultGroupButton();

			void Create(int iType, POINT ptWindow);
			int UpdateMouseEvent();
			void Render();
			int GetIndex();

		private:
			void Initialize();
			void AllUnLock();
			void BtnSelected(int iIndex);

			CNewUIButton* m_pButton;

			int m_iBtnNum;
			int m_iType;
			int m_iIndex;
		};

	public:
		enum IMAGE_LIST
		{
			IMAGE_CATAPULT_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP,
			IMAGE_CATAPULT_LOW = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LOW,

			IMAGE_CATAPULT_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,	//. newui_exit_00.tga

			// 버튼
			IMAGE_CATAPULT_BTN_FIRE = CNewUIMessageBoxMng::IMAGE_MSGBOX_BTN_EMPTY,
			
			IMAGE_CATAPULT_BTN_SMALL = BITMAP_CATAPULT_BEGIN,
			IMAGE_CATAPULT_BTN_BIG,
		};
		enum CATAPULT_TYPE
		{
			CATAPULT_ATTACK = 1,
			CATAPULT_DEFENSE = 2,
		};
		
	public:
		CNewUICatapultWindow();
		virtual ~CNewUICatapultWindow();
		
		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		
		void SetPos(int x, int y);
		
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		
		float GetLayerDepth();	//. 5.0f

		void OpenningProcess();
		void ClosingProcess();

		void Init(int iKey, int iType);
		void DoFire(int iKey, int iResult, int iType, int iPositionX, int iPositionY);
		void DoFireFixStartPosition(int iType, int iPositionX, int iPositionY);
		void SetCameraPos(float x=0.f, float y=0.f, float z=0.f);
		void GetCameraPos(vec3_t& vPos);

	private:
		void LoadImages();
		void UnloadImages();

		void SetButtonInfo();
		
		bool BtnProcess();

		void RenderFrame();
		void RenderTexts();
		void RenderButtons();

		void RenderOutlineUpper(float fPos_x, float fPos_y, float fWidth, float fHeight);
		void RenderOutlineLower(float fPos_x, float fPos_y, float fWidth, float fHeight);


	private:
		CNewUIManager* m_pNewUIMng;
		POINT m_Pos;

		CNewUIButton m_BtnExit;
		CCatapultGroupButton m_BtnChoiceArea;	// 지역선택
		CNewUIButton m_BtnFire;	// 발사버튼

		int m_iType;
		int m_iNpcKey;
		vec3_t m_vCameraPos;
	};
	
}

#endif // !defined(AFX_NEWUICATAPULTWINDOW_H__064BC38C_5F26_4003_A6C7_7270A11DEEBF__INCLUDED_)
