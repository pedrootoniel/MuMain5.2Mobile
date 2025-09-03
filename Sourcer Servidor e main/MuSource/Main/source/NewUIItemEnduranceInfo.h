// NewUIItemEnduranceInfo.h: interface for the CNewUIItemEnduranceInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWUIITEMENDURANCEINFO_H__ADB04FC1_C3E3_47B5_8026_C78C5800500C__INCLUDED_)
#define AFX_NEWUIITEMENDURANCEINFO_H__ADB04FC1_C3E3_47B5_8026_C78C5800500C__INCLUDED_

#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"

namespace SEASON3B
{
	class CNewUIItemEnduranceInfo : public CNewUIObj
	{
	protected:
		enum IMAGE_LIST
		{		
			IMAGE_PETHP_FRAME = BITMAP_ITEM_ENDURANCE_INFO_BEGIN,
			IMAGE_PETHP_BAR,
		};
		
		enum IMAGE_SIZE
		{
			PETHP_FRAME_WIDTH = 57,
			PETHP_FRAME_HEIGHT = 23,
			PETHP_BAR_WIDTH = 50,
			ITEM_DUR_WIDTH = 23,
			ITEM_DUR_HEIGHT = 23,
		};

		enum ITEM_DUR_ARROW_TYPE
		{
			ARROWTYPE_NONE = -1,	
			ARROWTYPE_BOW = ITEM_BOW + 15,
			ARROWTYPE_CROSSBOW = ITEM_BOW + 7,
		};

		enum 
		{
			UI_INTERVAL_HEIGHT = 1,
			UI_INTERVAL_WIDTH = 2,
		};

	private:
		CNewUIManager*				m_pNewUIMng;
		POINT						m_UIStartPos;
		int							m_iTextEndPosX;
		POINT						m_ItemDurUIStartPos;

		int							m_iCurArrowType;
		int							m_iTooltipIndex;

	public:
		CNewUIItemEnduranceInfo();
		virtual ~CNewUIItemEnduranceInfo();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		
		void SetPos( int x, int y);
		void SetPos( int x );
		
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();

		bool BtnProcess();
		
		float GetLayerDepth();	//. 3.5f
		
		void OpenningProcess();
		void ClosingProcess();
		
	private:
		void LoadImages();
		void UnloadImages();

		void RenderLeft();
		void RenderRight();

		void RenderHPUI( int iX, int iY, unicode::t_char* pszName, int iLife, int iMaxLife = 255, bool bWarning = false );
		void RenderTooltip( int iX, int iY, const ITEM* pItem, const DWORD& dwTextColor);
		bool RenderEquipedHelperLife( int iX, int iY );
		bool RenderEquipedPetLife( int iX, int iY );
		bool RenderSummonMonsterLife( int iX, int iY );
		bool RenderNumArrow( int iX, int iY );
		bool RenderPCRoomPoint(int iX, int iY);
 		bool RenderItemEndurance( int ix, int iY );
	};
}

#endif // !defined(AFX_NEWUIITEMENDURANCEINFO_H__ADB04FC1_C3E3_47B5_8026_C78C5800500C__INCLUDED_)
