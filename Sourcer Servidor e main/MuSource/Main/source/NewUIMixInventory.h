// NewUIMixInventory.h: interface for the CNewUIMixInventory class.
//////////////////////////////////////////////////////////////////////

#pragma once

#include "NewUIBase.h"
#include "NewUIInventoryCtrl.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "UIControls.h"

namespace SEASON3B
{
	class CNewUIMixInventory : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_MIXINVENTORY_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP,	
			IMAGE_MIXINVENTORY_LOW = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LOW,

			IMAGE_MIXINVENTORY_MIXBTN = BITMAP_INTERFACE_NEW_MIXINVENTORY_BEGIN,
			IMAGE_MIXINVENTORY_MIXBTN_CLICK,
		};
		enum MIX_STATE
		{
			MIX_READY = 0,
			MIX_REQUESTED,
			MIX_FINISHED
		};
		
	private:
		enum
		{
			INVENTORY_WIDTH = 190,
			INVENTORY_HEIGHT = 429,
		};
		
		CNewUIManager*			m_pNewUIMng;
		CNewUIInventoryCtrl*	m_pNewInventoryCtrl;
		POINT m_Pos;

		int m_iMixState;
		int m_iMixEffectTimer;
		float m_fInventoryColor[3];
		float m_fInventoryWarningColor[3];

		int iClickState;

	public:
		CNewUIMixInventory();
		virtual ~CNewUIMixInventory();
		
		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		bool InsertItem(int iIndex, BYTE* pbyItemPacket);
		void DeleteItem(int iIndex);
		void DeleteAllItems();
		
		void OpeningProcess();
		bool ClosingProcess();

		void SetMixState(int iMixState);
		int GetMixState() { return m_iMixState; }
		
		int GetPointedItemIndex();

		void SetPos(int x, int y);

		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();

		float GetLayerDepth();	//. 3.4f

		CNewUIInventoryCtrl* GetInventoryCtrl() const;
		
		static void UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB);

	private:
		void LoadImages();
		void UnloadImages();

		void RenderFrame();
		bool InventoryProcess();
		bool BtnProcess();

		void RenderMixDescriptions(float fPos_x, float fPos_y);
		
		void CheckMixInventory();
		bool Mix();
		void RenderMixEffect();

		int Rtn_MixRequireZen( int _nMixZen, int _nTax );
		
	};
}

