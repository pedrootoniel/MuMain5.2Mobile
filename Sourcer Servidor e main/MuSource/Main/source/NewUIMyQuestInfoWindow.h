// NewUIMyQuestInfoWindow.h: interface for the CNewUIMyQuestInfoWindow class.
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIGuildInfoWindow.h"
#include "UIControls.h"

typedef std::list<DWORD> DWordList;

namespace SEASON3B
{
	class CNewUIMyQuestInfoWindow  : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_MYQUEST_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP,
			IMAGE_MYQUEST_LOW = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LOW,
			
			IMAGE_MYQUEST_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,

			IMAGE_MYQUEST_LINE = BITMAP_INTERFACE_MYQUEST_WINDOW_BEGIN,
		};
		enum
		{
			MYQUESTINFO_WINDOW_WIDTH = 190,
			MYQUESTINFO_WINDOW_HEIGHT = 429,
		};

		enum TAB_BUTTON_INDEX
		{
			TAB_QUEST = 0,
			TAB_CASTLE,
			TAB_TEMPLE,
		};
		
	public:
		CNewUIMyQuestInfoWindow();
		virtual ~CNewUIMyQuestInfoWindow();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		
		void SetPos(int x, int y);
		
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		
		float GetLayerDepth();	//. 3.3f
		
		void OpenningProcess();
		void ClosingProcess();

	private:
		void LoadImages();
		void UnloadImages();
		
		void SetButtonInfo();
		
		bool BtnProcess();

		void RenderFrame();
		void RenderSubjectTexts();
		void RenderQuestTexts();
		void RenderTabQuest();
		void RenderTabCastle();
		void RenderTabTemple();
		void RenderButtons();

	private:
		CNewUIManager* m_pNewUIMng;
		POINT m_Pos;

		CNewUIRadioGroupButton m_BtnTab;

		CNewUIButton m_BtnExit;

		int m_iNumCurOpenTab;
	};
}
