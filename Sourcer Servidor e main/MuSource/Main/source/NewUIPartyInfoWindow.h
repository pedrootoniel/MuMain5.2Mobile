// NewUIPartyInfo.h: interface for the CNewUIPartyInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWUIPARTYINFO_H__8F1E7392_C9CC_487E_8EAD_10DBD47AF20F__INCLUDED_)
#define AFX_NEWUIPARTYINFO_H__8F1E7392_C9CC_487E_8EAD_10DBD47AF20F__INCLUDED_

#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIMyInventory.h"
#include "NewUIButton.h"

namespace SEASON3B
{
	class CNewUIPartyInfoWindow : public CNewUIObj  
	{
	public:
		enum IMAGE_LIST
		{		
			IMAGE_PARTY_BASE_WINDOW_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP,
			IMAGE_PARTY_BASE_WINDOW_LOW = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LOW,

			IMAGE_PARTY_BASE_WINDOW_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,

			IMAGE_PARTY_HPBAR_BACK = BITMAP_PARTY_INFO_BEGIN,		// newui_party_lifebar01.jpg	(151, 8)
			IMAGE_PARTY_HPBAR,										// newui_party_lifebar02.jpg	(147, 4)
			IMAGE_PARTY_FLAG,										// newui_party_flag.tga			(10, 12)	
			IMAGE_PARTY_EXIT,										// newui_Party_x.tga			(13, 26)	// 임시
		};		
		
	private:
		enum PARTY_WINDOW_SIZE
		{
			PARTY_INFO_WINDOW_WIDTH = 190,
			PARTY_INFO_WINDOW_HEIGHT = 429,
		};

	private:
		CNewUIManager*				m_pNewUIMng;
		POINT						m_Pos;

		int							m_iSelectedCharID;		// Party List에서 캐릭터 ID를 선택 (default : -1)

		bool						m_bParty;

		
		
	public:
		CNewUIPartyInfoWindow();
		virtual ~CNewUIPartyInfoWindow();
		
		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		
		void SetPos(int x, int y);
		
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();

		bool BtnProcess();
		
		float GetLayerDepth();	//. 2.4f
		
		void OpenningProcess();
		void ClosingProcess();

		bool LeaveParty( const int iIndex );	// 파티장이 상대방을 강퇴

		void SetParty( bool bParty );
		
	private:
		void LoadImages();
		void UnloadImages();

		void InitButtons();
		
		void RenderMemberStatue( int iIndex, PARTY_t* pMember);
	};
}

#endif // !defined(AFX_NEWUIPARTYINFO_H__8F1E7392_C9CC_487E_8EAD_10DBD47AF20F__INCLUDED_)
