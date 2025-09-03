// NewUICharacterInfoWindow.h: interface for the CNewUICharacterInfoWindow class.
//////////////////////////////////////////////////////////////////////
#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIButton.h"

namespace SEASON3B
{

	class CNewUICharacterInfoWindow : public CNewUIObj  
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_CHAINFO_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP,
			IMAGE_CHAINFO_LOW = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LOW,

			IMAGE_CHAINFO_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,

			IMAGE_CHAINFO_LEVEL_BOX = BITMAP_INTERFACE_NEW_CHAINFO_WINDOW_BEGIN,

			IMAGE_CHAINFO_BTN_STAT,
			IMAGE_CHAINFO_BTN_STAT_CLICK,

			IMAGE_CHAINFO_BTN_QUEST,
			IMAGE_CHAINFO_BTN_QUEST_CLICK,

			IMAGE_CHAINFO_BTN_PET,
			IMAGE_CHAINFO_BTN_PET_CLICK,
		};

		enum
		{
			CHAINFO_WINDOW_WIDTH = 190,
			CHAINFO_WINDOW_HEIGHT = 429,

			HEIGHT_STRENGTH = 120,
			HEIGHT_DEXTERITY = 175,
			HEIGHT_VITALITY = 240,
			HEIGHT_ENERGY = 295,
			HEIGHT_CHARISMA = 350,

			BTN_STAT_COUNT = 5,

			STAT_STRENGTH = 0,
			STAT_DEXTERITY,
			STAT_VITALITY,
			STAT_ENERGY,
			STAT_CHARISMA,
		};
		
	private:
		CNewUIManager*				m_pNewUIMng;
		POINT						m_Pos;

	public:
		CNewUICharacterInfoWindow();
		virtual ~CNewUICharacterInfoWindow();
		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		void SetPos(int x, int y);
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		float GetLayerDepth();	//. 5.1f
		void OpenningProcess();
		
	private:
		void LoadImages();
		void UnloadImages();
		void ResetEquipmentLevel();
		bool BtnProcess();
		void RenderFrame();
		void RenderTexts();
		void RenderSubjectTexts();
		void RenderTableTexts();
		void RenderAttribute();
		void RenderButtons();

		int EventState[5];
	};


}

