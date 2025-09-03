// NewUIMyShopInventory.h: interface for the CNewUIMyShopInventory class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWUIMYSHOPINVENTORY_H__A0C3DD4A_C4D5_4CF2_9702_DF54540DB6FD__INCLUDED_)
#define AFX_NEWUIMYSHOPINVENTORY_H__A0C3DD4A_C4D5_4CF2_9702_DF54540DB6FD__INCLUDED_

#pragma once

#include "NewUIBase.h"
#include "NewUIInventoryCtrl.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIButton.h"
#include "UIControls.h"

namespace SEASON3B
{
	class CNewUIMyShopInventory : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_MYSHOPINVENTORY_TOP		= BITMAP_MYSHOPINTERFACE_NEW_PERSONALINVENTORY_BEGIN,
			IMAGE_MYSHOPINVENTORY_LOW,

			IMAGE_MYSHOPINVENTORY_EDIT,
			IMAGE_MYSHOPINVENTORY_BOX,
		};
		
		enum SHOPTYEP
		{
			PERSONALSHOPSALE = 0,
			PERSONALSHOPPURCHASE,
		};
		
	private:
		enum
		{
			INVENTORY_WIDTH = 190,
			INVENTORY_HEIGHT = 429,
		};

		enum
		{
			MYSHOPINVENTORY_EXIT = 0,
			MYSHOPINVENTORY_OPEN,
			MYSHOPINVENTORY_CLOSE,
			MYSHOPINVENTORY_MAXBUTTONCOUNT,
		};
		
		CNewUIManager*			m_pNewUIMng;
		CNewUIInventoryCtrl*	m_pNewInventoryCtrl;
		POINT m_Pos;

	public:
		CNewUIMyShopInventory();
		virtual ~CNewUIMyShopInventory();
		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		void SetPos(int x, int y);
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		void ClosingProcess();
		float GetLayerDepth();	//. 3.2f
		
		CNewUIInventoryCtrl* GetInventoryCtrl() const;

	public:
		void ChangeSourceIndex( int sindex );
		const int GetSourceIndex();
		void ChangeTargetIndex( int tindex );
		const int GetTargetIndex();
		ITEM* FindItem(int iLinealPos);
		void ChangeEditBox(const UISTATES type);
		void ChangeTitle( unicode::t_string& titletext );
		void GetTitle( unicode::t_string& titletext );
		void ChangePersonal( bool state );
		const bool IsEnablePersonalShop() const;
		void OpenButtonState(int State);
		int GetPointedItemIndex();
		void ResetSubject();
		bool IsEnableInputValueTextBox();
		void SetInputValueTextBox(bool bIsEnable);

		int m_ButtonState;

	public:
		bool InsertItem(int iIndex, BYTE* pbyItemPacket);
		void DeleteItem(int iIndex);
		void DeleteAllItems();
		
	private:
		void LoadImages();
		void UnloadImages();

	private:
		bool MyShopInventoryProcess();
		
	private:
		void RenderFrame();
		void RenderTextInfo();

	private:
		int					m_TargetIndex;
		int					m_SourceIndex;
		bool				m_EnablePersonalShop;
		bool				m_bIsEnableInputValueTextBox;

		CNewUIButton*			m_Button;
		CUITextInputBox*        m_EditBox;
		const int MAX_SHOPTITLE_UTF16;
	};

	inline
	void CNewUIMyShopInventory::ChangeTitle( unicode::t_string& titletext )
	{
		m_EditBox->SetText( titletext.c_str() );
	}

	inline
	void CNewUIMyShopInventory::ChangeSourceIndex( int sindex )
	{
		m_SourceIndex = sindex;
	}

	inline
	void CNewUIMyShopInventory::ChangeTargetIndex( int tindex )
	{
		m_TargetIndex = tindex;
	}
	
	inline
	float CNewUIMyShopInventory::GetLayerDepth()
	{ 
		return 3.2f; 
	}

	inline
	CNewUIInventoryCtrl* CNewUIMyShopInventory::GetInventoryCtrl() const
	{ 
		return m_pNewInventoryCtrl; 
	}

	inline
	const int CNewUIMyShopInventory::GetSourceIndex()
	{
		return m_SourceIndex;
	}

	inline
	const int CNewUIMyShopInventory::GetTargetIndex()
	{
		return m_TargetIndex;
	}
}

#endif // !defined(AFX_NEWUIMYSHOPINVENTORY_H__A0C3DD4A_C4D5_4CF2_9702_DF54540DB6FD__INCLUDED_)
