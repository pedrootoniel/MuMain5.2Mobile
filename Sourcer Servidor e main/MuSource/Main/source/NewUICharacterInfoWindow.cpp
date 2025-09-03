// NewUICharacterInfoWindow.cpp: implementation of the CNewUICharacterInfoWindow class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUICharacterInfoWindow.h"
#include "NewUISystem.h"
#include "CharacterManager.h"
#include "CSItemOption.h"
#include "ZzzBMD.h"
#include "ZzzCharacter.h"
#include "UIControls.h"
#include "ZzzInterface.h"
#include "ZzzScene.h"
#include "ZzzInventory.h"
#include "SkillManager.h"
#include "UIJewelHarmony.h"
#include "UIManager.h"
#include "wsclientinline.h"
#include "ServerListManager.h"

using namespace SEASON3B;

SEASON3B::CNewUICharacterInfoWindow::CNewUICharacterInfoWindow() 
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;

	for (int i = 0; i < 5; ++i)
	{
		this->EventState[i] = FALSE;
	}
}

SEASON3B::CNewUICharacterInfoWindow::~CNewUICharacterInfoWindow() 
{ 
	Release(); 
}

bool SEASON3B::CNewUICharacterInfoWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if(NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_CHARACTER, this);

	SetPos(x, y);

	LoadImages();

	Show(false);

	return true;
}

void SEASON3B::CNewUICharacterInfoWindow::Release()
{
	UnloadImages();

	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUICharacterInfoWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool SEASON3B::CNewUICharacterInfoWindow::UpdateMouseEvent()
{
	if(BtnProcess() == true)
	{
		return false;
	}

	if(CheckMouseIn(m_Pos.x, m_Pos.y, CHAINFO_WINDOW_WIDTH, CHAINFO_WINDOW_HEIGHT))
	{
		return false;
	}

	return true;
}

bool SEASON3B::CNewUICharacterInfoWindow::BtnProcess()
{
	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24, 24) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Hide(SEASON3B::INTERFACE_CHARACTER);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	if(CharacterAttribute->LevelUpPoint > 0)
	{
		int iBaseClass = gCharacterManager.GetBaseClass(Hero->Class);

		int iCount = 0;

		if (iBaseClass == CLASS_DARK_LORD)
		{
			iCount = 5;
		}
		else
		{
			iCount = 4;
		}

		int iPosYClick[] = { HEIGHT_STRENGTH, HEIGHT_DEXTERITY - 2, HEIGHT_VITALITY - 2, HEIGHT_ENERGY - 2, HEIGHT_CHARISMA - 2 };

		for (int i = 0; i < iCount; ++i)
		{
			if (SEASON3B::CheckMouseIn(m_Pos.x + 122, m_Pos.y + iPosYClick[i], 24, 24) == true && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;
				MouseUpdateTimeMax = 6;

				SendRequestAddPoint(i);

				PlayBuffer(SOUND_CLICK01);

				this->EventState[i] = TRUE;

				return true;
			}
			else
			{
				this->EventState[i] = FALSE;
			}
		}
	}

	if (SEASON3B::CheckMouseIn(m_Pos.x + 56, m_Pos.y + 395, 24, 24) == true && MouseLButtonPush)
	{
		MouseLButtonPush = false;

		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;

		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_MYQUEST);

		PlayBuffer(SOUND_CLICK01);

		return true;
	}

	if (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_DARK_LORD)
	{
		if (SEASON3B::CheckMouseIn(m_Pos.x + 83, m_Pos.y + 395, 24, 24) == true && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;
			MouseUpdateTimeMax = 6;

			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_PET);

			PlayBuffer(SOUND_CLICK01);

			return true;
		}
	}
	
	return false;
}

bool SEASON3B::CNewUICharacterInfoWindow::UpdateKeyEvent()
{
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER) == true)
	{
		if(SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_CHARACTER);
			PlayBuffer(SOUND_CLICK01);
	
			return false;
		}
	}

	return true;
}

bool SEASON3B::CNewUICharacterInfoWindow::Update()
{
	return true;
}

void SEASON3B::CNewUICharacterInfoWindow::RenderFrame()
{
	RenderBitmap(IMAGE_CHAINFO_TOP, m_Pos.x, m_Pos.y, 190.0, 256.0, 0, 0, 0.7421875, 1.0, 1, 1, 0.0);
	RenderBitmap(IMAGE_CHAINFO_LOW, m_Pos.x, m_Pos.y + 256.0, 190.0, 177.0, 0, 0, 0.7421875, 0.69140625, 1, 1, 0.0);

	RenderBitmap(IMAGE_CHAINFO_LEVEL_BOX, m_Pos.x + 10, m_Pos.y + 52.5, 75.0, 21.0, 0, 0, 0.5859375, 0.65625, 1, 1, 0.0);

	RenderBitmap(IMAGE_CHAINFO_LEVEL_BOX, m_Pos.x + 11, m_Pos.y + HEIGHT_STRENGTH, 75.0, 21.0, 0, 0, 0.5859375, 0.65625, 1, 1, 0.0);
	RenderBitmap(IMAGE_CHAINFO_LEVEL_BOX, m_Pos.x + 11, m_Pos.y + HEIGHT_DEXTERITY, 75.0, 21.0, 0, 0, 0.5859375, 0.65625, 1, 1, 0.0);
	RenderBitmap(IMAGE_CHAINFO_LEVEL_BOX, m_Pos.x + 11, m_Pos.y + HEIGHT_VITALITY, 75.0, 21.0, 0, 0, 0.5859375, 0.65625, 1, 1, 0.0);
	RenderBitmap(IMAGE_CHAINFO_LEVEL_BOX, m_Pos.x + 11, m_Pos.y + HEIGHT_ENERGY, 75.0, 21.0, 0, 0, 0.5859375, 0.65625, 1, 1, 0.0);

	if(gCharacterManager.GetBaseClass(Hero->Class) == CLASS_DARK_LORD)
	{
		RenderBitmap(IMAGE_CHAINFO_LEVEL_BOX, m_Pos.x + 11, m_Pos.y + HEIGHT_CHARISMA, 75.0, 21.0, 0, 0, 0.5859375, 0.65625, 1, 1, 0.0);
	}
}

bool SEASON3B::CNewUICharacterInfoWindow::Render()
{
	EnableAlphaTest();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	RenderFrame();

	RenderTexts();

	RenderButtons();

	DisableAlphaBlend();

	return true;
}

void SEASON3B::CNewUICharacterInfoWindow::RenderTexts()
{
	RenderSubjectTexts();
	RenderTableTexts();
	RenderAttribute();
}

void SEASON3B::CNewUICharacterInfoWindow::RenderSubjectTexts()
{
	char strID[256];
	sprintf(strID, "%s", CharacterAttribute->Name);
	unicode::t_char strClassName[256];
	unicode::_sprintf(strClassName, "(%s)", gCharacterManager.GetCharacterClassText(CharacterAttribute->Class));
	
	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetBgColor(30, 30, 30, 255);
	SetPlayerColor(Hero->PK);
	g_pRenderText->RenderText(m_Pos.x + 36, m_Pos.y + 12, strID, 120, 0, RT3_SORT_CENTER);

	unicode::t_char strServerName[MAX_TEXT_LENGTH];

	const char* apszGlobalText[4] = { GlobalText[461], GlobalText[460], GlobalText[3130], GlobalText[3131] };
	sprintf(strServerName, apszGlobalText[g_ServerListManager->GetNonPVPInfo()], g_ServerListManager->GetSelectServerName(), g_ServerListManager->GetSelectServerIndex());
		
	g_pRenderText->SetBgColor(30, 30, 30, 0);

	float fAlpha = sinf(WorldTime * 0.001f) + 1.f;
	g_pRenderText->SetTextColor(255, 255, 255, 127 * (2.f - fAlpha));
	g_pRenderText->RenderText(m_Pos.x + 36, m_Pos.y + 23, strClassName, 120, 0, RT3_SORT_CENTER);
	g_pRenderText->SetTextColor(255, 255, 255, 127 * fAlpha);
	g_pRenderText->RenderText(m_Pos.x + 36, m_Pos.y + 23, strServerName, 120, 0, RT3_SORT_CENTER);
}

void SEASON3B::CNewUICharacterInfoWindow::RenderTableTexts()
{
	unicode::t_char strLevel[32];
	unicode::t_char strExp[128];
	unicode::t_char strPoint[128];

	unicode::_sprintf(strLevel, GlobalText[200], CharacterAttribute->Level);
	unicode::_sprintf(strExp, GlobalText[201], CharacterAttribute->Experience, CharacterAttribute->NextExperince);

	if(CharacterAttribute->Level > 9)
	{
		int iMinus, iMaxMinus;
		if(CharacterAttribute->wMinusPoint == 0)
		{
			iMinus = 0;
		}
		else
		{
			iMinus = -CharacterAttribute->wMinusPoint;
		}

		iMaxMinus = -CharacterAttribute->wMaxMinusPoint;

		unicode::_sprintf(strPoint, "%s %d/%d | %s %d/%d", 
			GlobalText[1412], CharacterAttribute->AddPoint, CharacterAttribute->MaxAddPoint, 
			GlobalText[1903], iMinus, iMaxMinus);
	}
	else 
	{
		wsprintf(strPoint, "%s %d/%d | %s %d/%d", GlobalText[1412], 0, 0, GlobalText[1903], 0, 0);
	}
	
	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(255, 255, 255, 250);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->RenderText(m_Pos.x + 30, m_Pos.y + 59, strLevel);

	if(CharacterAttribute->LevelUpPoint > 0)
	{
		unicode::t_char strLevelUpPoint[128];

		if (CharacterAttribute->LevelUpPoint > 0)
		{
			unicode::_sprintf(strLevelUpPoint, GlobalText[217], CharacterAttribute->LevelUpPoint);
		}

		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->SetTextColor(0, 0, 0, 255);
		g_pRenderText->SetBgColor(30, 110, 199, 255);
		g_pRenderText->RenderText(m_Pos.x + 90, m_Pos.y + 48, strLevelUpPoint, 80, 0, RT3_SORT_CENTER);
	}
	
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(32, 32, 32, 180);
	g_pRenderText->RenderText(m_Pos.x + 18, m_Pos.y + 80, strExp, 150, 0, RT3_SORT_LEFT);

	int iAddPoint, iMinusPoint;

	if(CharacterAttribute->AddPoint <= 10)
	{
		iAddPoint = 100;
	}
	else
	{
		int iTemp = 0;
		if(CharacterAttribute->MaxAddPoint != 0)
		{
			iTemp = (CharacterAttribute->AddPoint*100)/CharacterAttribute->MaxAddPoint;
		}
		if(iTemp <= 10)
		{
			iAddPoint = 70;
		}
		else if(iTemp > 10 && iTemp <= 30)
		{
			iAddPoint = 60;
		}
		else if(iTemp > 30 && iTemp <= 50)
		{
			iAddPoint = 50;
		}
		else if(iTemp > 50)
		{
			iAddPoint = 40;
		}
	}

	if(CharacterAttribute->wMinusPoint <= 10)
	{
		iMinusPoint = 100;
	}
	else
	{
		int iTemp = 0;
		if(CharacterAttribute->wMaxMinusPoint != 0)
		{
			iTemp = (CharacterAttribute->wMinusPoint*100)/CharacterAttribute->wMaxMinusPoint;
		}

		if(iTemp <= 10)
		{
			iMinusPoint = 70;
		}
		else if(iTemp > 10 && iTemp <= 30)
		{
			iMinusPoint = 60;
		}
		else if(iTemp > 30 && iTemp <= 50)
		{
			iMinusPoint = 50;
		}
		else if(iTemp > 50)
		{
			iMinusPoint = 40;
		}
	}

	unicode::t_char strPointProbability[128];

	unicode::_sprintf(strPointProbability, GlobalText[1907], iAddPoint, iMinusPoint);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(78, 136, 223, 255);
	g_pRenderText->SetBgColor(32, 32, 32, 180);
	g_pRenderText->RenderText(m_Pos.x + 90, m_Pos.y + 62, strPointProbability, 80, 0, RT3_SORT_CENTER);

	g_pRenderText->SetTextColor(78, 136, 223, 255);
	g_pRenderText->SetBgColor(32, 32, 32, 180);
	g_pRenderText->RenderText(m_Pos.x + 18, m_Pos.y + 95, strPoint, 140, 0, RT3_SORT_LEFT);
}

void SEASON3B::CNewUICharacterInfoWindow::RenderAttribute()
{
	g_pRenderText->SetFont(g_hFontBold);

	WORD wStrength;

	wStrength = CharacterAttribute->Strength + CharacterAttribute->AddStrength;

	if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion1))
	{
		g_pRenderText->SetTextColor(255, 120, 0, 255);
	}
	else
	{
		if (CharacterAttribute->AddStrength)
		{
			g_pRenderText->SetTextColor(100, 150, 255, 255);
		}
		else
		{
			g_pRenderText->SetTextColor(223, 191, 103, 255);
		}
	}

	unicode::t_char strStrength[32];
	unicode::_sprintf(strStrength, GlobalText[202], wStrength);

	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x + 25, m_Pos.y + HEIGHT_STRENGTH + 6, strStrength, 74, 0, RT3_SORT_LEFT);

	unicode::t_char strAttakMamage[256];

	int iAttackDamageMin = 0;
	int iAttackDamageMax = 0;

	bool bAttackDamage = GetAttackDamage(&iAttackDamageMin, &iAttackDamageMax);

	int Add_Dex = 0;
	int Add_Rat = 0;
	int Add_Dfe = 0;
	int Add_Att_Max = 0;
	int Add_Att_Min = 0;
	int Add_Dfe_Rat = 0;
	int Add_Ch_Dfe = 0;
	int Add_Mana_Max = 0;
	int Add_Mana_Min = 0;

	for(int i=0; i < MAX_MAGIC; ++i)
	{
		if(CharacterAttribute->Skill[i] >= AT_SKILL_TOMAN_ATTACKUP && CharacterAttribute->Skill[i] < AT_SKILL_TOMAN_ATTACKUP + 5)
		{
			Add_Dex = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else
		if(CharacterAttribute->Skill[i] >= AT_SKILL_ATTACK_RATEUP && CharacterAttribute->Skill[i] < AT_SKILL_ATTACK_RATEUP + 5)
		{
			Add_Rat = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else
		if(CharacterAttribute->Skill[i] >= AT_SKILL_TOMAN_DEFENCEUP && CharacterAttribute->Skill[i] < AT_SKILL_TOMAN_DEFENCEUP + 5)
		{
			Add_Dfe = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else
		if(CharacterAttribute->Skill[i] >= AT_SKILL_DEF_UP && CharacterAttribute->Skill[i] < AT_SKILL_DEF_UP + 5)
		{
			Add_Ch_Dfe = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else
		if(CharacterAttribute->Skill[i] >= AT_SKILL_MAX_ATTACKRATE_UP && CharacterAttribute->Skill[i] < AT_SKILL_MAX_ATTACKRATE_UP + 5)
		{
			Add_Att_Max = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else
		if(CharacterAttribute->Skill[i] >= AT_SKILL_MAX_ATT_MAGIC_UP && CharacterAttribute->Skill[i] < AT_SKILL_MAX_ATT_MAGIC_UP + 5)
		{
			Add_Mana_Max = Add_Att_Max = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else
		if(CharacterAttribute->Skill[i] >= AT_SKILL_MIN_ATT_MAGIC_UP && CharacterAttribute->Skill[i] < AT_SKILL_MIN_ATT_MAGIC_UP + 5)
		{
			Add_Mana_Min = Add_Att_Min = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else
		if(CharacterAttribute->Skill[i] >= AT_SKILL_MAX_MANA_UP && CharacterAttribute->Skill[i] < AT_SKILL_MAX_MANA_UP + 5)
		{
			Add_Mana_Max = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else
		if(CharacterAttribute->Skill[i] >= AT_SKILL_MIN_MANA_UP && CharacterAttribute->Skill[i] < AT_SKILL_MIN_MANA_UP + 5)
		{
			Add_Mana_Min = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else
		if(CharacterAttribute->Skill[i] >= AT_SKILL_MIN_ATTACKRATE_UP && CharacterAttribute->Skill[i] < AT_SKILL_MIN_ATTACKRATE_UP + 5)
		{
			Add_Att_Min = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
	}

	ITEM* pWeaponRight = &CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT];
	ITEM* pWeaponLeft = &CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT];
	
    int iAttackRating = CharacterAttribute->AttackRating + Add_Rat;
	int iAttackRatingPK = CharacterAttribute->AttackRatingPK + Add_Dex;
	iAttackDamageMax += Add_Att_Max;
	iAttackDamageMin += Add_Att_Min;

	if( g_isCharacterBuff((&Hero->Object), eBuff_AddAG) ) 
    {
    	WORD wDexterity = CharacterAttribute->Dexterity+ CharacterAttribute->AddDexterity;
        iAttackRating += wDexterity;
		iAttackRatingPK += wDexterity;
        if(PartyNumber >= 3)
        {
			int iPlusRating = (wDexterity * ((PartyNumber - 2) * 0.01f));
            iAttackRating += iPlusRating;
			iAttackRatingPK = iPlusRating;
        }
    }
	if( g_isCharacterBuff((&Hero->Object), eBuff_HelpNpc) )
	{
		int iTemp = 0;
		if(CharacterAttribute->Level > 180)
		{
			iTemp = (180 / 3) + 45;
		}
		else
		{
			iTemp = (CharacterAttribute->Level / 3) + 45;
		}
		
		iAttackDamageMin += iTemp;
		iAttackDamageMax += iTemp;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_Berserker))
	{
		int nTemp = CharacterAttribute->Strength + CharacterAttribute->AddStrength
			+ CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;
		float fTemp = int(CharacterAttribute->Energy / 30) / 100.f;
		iAttackDamageMin += nTemp / 7 * fTemp;
		iAttackDamageMax += nTemp / 4 * fTemp;
	}

	int iMinIndex = 0, iMaxIndex = 0, iMagicalIndex = 0; 

	StrengthenCapability SC_r, SC_l;

	int rlevel = (pWeaponRight->Level>>3)&15;

	if( rlevel >= pWeaponRight->Jewel_Of_Harmony_OptionLevel )
	{
		g_pUIJewelHarmonyinfo->GetStrengthenCapability( &SC_r, pWeaponRight, 1 );

		if( SC_r.SI_isSP )
		{
			iMinIndex     = SC_r.SI_SP.SI_minattackpower;
			iMaxIndex     = SC_r.SI_SP.SI_maxattackpower;
			iMagicalIndex = SC_r.SI_SP.SI_magicalpower;
		}
	}

	int llevel = (pWeaponLeft->Level>>3)&15;

	if( llevel >= pWeaponLeft->Jewel_Of_Harmony_OptionLevel )
	{
		g_pUIJewelHarmonyinfo->GetStrengthenCapability( &SC_l, pWeaponLeft, 1 );

		if( SC_l.SI_isSP )
		{
			iMinIndex     += SC_l.SI_SP.SI_minattackpower;
			iMaxIndex     += SC_l.SI_SP.SI_maxattackpower;
			iMagicalIndex += SC_l.SI_SP.SI_magicalpower;
		}
	}

	int iDefenseRate = 0, iAttackPowerRate = 0;
	
	StrengthenCapability rightinfo, leftinfo;

	int iRightLevel = (pWeaponRight->Level>>3)&15;

	if( iRightLevel >= pWeaponRight->Jewel_Of_Harmony_OptionLevel )
	{
		g_pUIJewelHarmonyinfo->GetStrengthenCapability( &rightinfo, pWeaponRight, 1 );
	}

	int iLeftLevel = (pWeaponLeft->Level>>3)&15;

	if( iLeftLevel >= pWeaponLeft->Jewel_Of_Harmony_OptionLevel )
	{
		g_pUIJewelHarmonyinfo->GetStrengthenCapability( &leftinfo, pWeaponLeft, 1 );
	}

	if( rightinfo.SI_isSP )
	{
		iAttackPowerRate += rightinfo.SI_SP.SI_attackpowerRate;
	}
	if( leftinfo.SI_isSP )
	{
		iAttackPowerRate += leftinfo.SI_SP.SI_attackpowerRate;
	}

	for( int k = EQUIPMENT_WEAPON_LEFT; k < MAX_EQUIPMENT; ++k )
	{
		StrengthenCapability defenseinfo;

		ITEM* pItem = &CharacterMachine->Equipment[k];

		int eqlevel = (pItem->Level>>3)&15;

		if( eqlevel >= pItem->Jewel_Of_Harmony_OptionLevel )
		{
			g_pUIJewelHarmonyinfo->GetStrengthenCapability( &defenseinfo, pItem, 2 );
		}

		if( defenseinfo.SI_isSD )
		{
			iDefenseRate += defenseinfo.SI_SD.SI_defenseRate;
		}
	}

	int itemoption380Attack = 0;
	int itemoption380Defense = 0;
	
	for(int j=0 ; j<MAX_EQUIPMENT; ++j)
	{
		bool is380item = CharacterMachine->Equipment[j].option_380;
		int i380type  = CharacterMachine->Equipment[j].Type;
		
		if( is380item && i380type > -1 && i380type < MAX_ITEM )
		{
			ITEM_ADD_OPTION item380option;
			
			item380option = g_pItemAddOptioninfo->GetItemAddOtioninfo( CharacterMachine->Equipment[j].Type );

			if( item380option.m_byOption1 == 1 )
			{
				itemoption380Attack += item380option.m_byValue1;
			}

			if( item380option.m_byOption2 == 1 )
			{
				itemoption380Attack += item380option.m_byValue2;
			}

			if( item380option.m_byOption1 == 3 )
			{
				itemoption380Defense += item380option.m_byValue1;
			}

			if( item380option.m_byOption2 == 3)
			{
				itemoption380Defense += item380option.m_byValue2;
			}
		}
	}

	ITEM *pItemRingLeft, *pItemRingRight;
	
	pItemRingLeft = &CharacterMachine->Equipment[EQUIPMENT_RING_LEFT];
	pItemRingRight = &CharacterMachine->Equipment[EQUIPMENT_RING_RIGHT];
	if(pItemRingLeft && pItemRingRight)
	{
		int iNonExpiredLRingType = -1;
		int iNonExpiredRRingType = -1;
		if(!pItemRingLeft->bPeriodItem || !pItemRingLeft->bExpiredPeriod)
		{
			iNonExpiredLRingType = pItemRingLeft->Type;
		}
		if(!pItemRingRight->bPeriodItem || !pItemRingRight->bExpiredPeriod)
		{
			iNonExpiredRRingType = pItemRingRight->Type;
		}

		int maxIAttackDamageMin = 0;
		int maxIAttackDamageMax = 0;
		if(iNonExpiredLRingType == ITEM_HELPER+41 || iNonExpiredRRingType == ITEM_HELPER+41)
		{
			maxIAttackDamageMin = max(maxIAttackDamageMin, 20);	
			maxIAttackDamageMax = max(maxIAttackDamageMax, 20);
		}
		if(iNonExpiredLRingType == ITEM_HELPER+76 || iNonExpiredRRingType == ITEM_HELPER+76)
		{
			maxIAttackDamageMin = max(maxIAttackDamageMin, 30);	
			maxIAttackDamageMax = max(maxIAttackDamageMax, 30);
		}
		if(iNonExpiredLRingType == ITEM_HELPER+122 || iNonExpiredRRingType == ITEM_HELPER+122)
		{
			maxIAttackDamageMin = max(maxIAttackDamageMin, 40);	
			maxIAttackDamageMax = max(maxIAttackDamageMax, 40);
		}

		iAttackDamageMin += maxIAttackDamageMin;
		iAttackDamageMax += maxIAttackDamageMax;
	}
	
	ITEM *pItemHelper = &CharacterMachine->Equipment[EQUIPMENT_HELPER];
	if(pItemHelper)
	{
		if(pItemHelper->Type == ITEM_HELPER+37 && pItemHelper->Option1 == 0x04)
		{
			WORD wLevel = CharacterAttribute->Level;
			iAttackDamageMin += (wLevel / 12);
			iAttackDamageMax += (wLevel / 12);
		}
		if(pItemHelper->Type == ITEM_HELPER+64)
		{
			if(false == pItemHelper->bExpiredPeriod)
			{
				iAttackDamageMin += int(float(iAttackDamageMin) * 0.4f);
				iAttackDamageMax += int(float(iAttackDamageMax) * 0.4f);
			}
		}
		if(pItemHelper->Type == ITEM_HELPER+123)
		{
			if(false == pItemHelper->bExpiredPeriod)
			{
				iAttackDamageMin += int(float(iAttackDamageMin) * 0.2f);
				iAttackDamageMax += int(float(iAttackDamageMax) * 0.2f);
			}
		}
		if(pItemHelper->Type == ITEM_HELPER+1)
		{
			iAttackDamageMin += int(float(iAttackDamageMin) * 0.3f);
			iAttackDamageMax += int(float(iAttackDamageMax) * 0.3f);
		}
	}

    if( iAttackRating > 0 )
    {
		if( iAttackDamageMin + iMinIndex >= iAttackDamageMax + iMaxIndex )
		{
			unicode::_sprintf(strAttakMamage, GlobalText[203], iAttackDamageMax + iMaxIndex, iAttackDamageMax + iMaxIndex, iAttackRating);
		}
		else
		{
			unicode::_sprintf(strAttakMamage, GlobalText[203], iAttackDamageMin + iMinIndex, iAttackDamageMax + iMaxIndex, iAttackRating);
		}
    }
    else
    {
		if( iAttackDamageMin + iMinIndex >= iAttackDamageMax + iMaxIndex )
		{
			unicode::_sprintf(strAttakMamage, GlobalText[204], iAttackDamageMax + iMaxIndex, iAttackDamageMax + iMaxIndex);
		}
		else
		{
			unicode::_sprintf(strAttakMamage, GlobalText[204], iAttackDamageMin + iMinIndex, iAttackDamageMax + iMaxIndex);
		}
    }

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(32, 32, 32, 180);

	if (bAttackDamage)
	{
		g_pRenderText->SetTextColor(100, 150, 255, 255);
	}
	else
	{
		g_pRenderText->SetTextColor(255, 255, 255, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin2))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll3))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_CherryBlossom_Petal))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	int iY = HEIGHT_STRENGTH + 25;
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + HEIGHT_STRENGTH + 25, strAttakMamage, 130, 0, RT3_SORT_LEFT);

	if(iAttackRatingPK > 0)
    {
		if( itemoption380Attack != 0 || iAttackPowerRate != 0 )
		{
			unicode::_sprintf(strAttakMamage, GlobalText[2109], iAttackRatingPK, itemoption380Attack + iAttackPowerRate );
		}
		else
		{
			unicode::_sprintf(strAttakMamage, GlobalText[2044], iAttackRatingPK);
		}

		iY += 13;
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strAttakMamage, 130, 0, RT3_SORT_LEFT);
    }

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetBgColor(0);

	if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion2))
	{
		g_pRenderText->SetTextColor(255, 120, 0, 255);
	}
	else
	{
		if (CharacterAttribute->AddDexterity)
		{
			g_pRenderText->SetTextColor(100, 150, 255, 255);
		}
		else
		{
			g_pRenderText->SetTextColor(223, 191, 103, 255);
		}
	}

	unicode::t_char strDexterity[32];

	WORD wDexterity = CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;

	unicode::_sprintf(strDexterity, GlobalText[205], wDexterity);

	g_pRenderText->RenderText(m_Pos.x + 25, m_Pos.y + HEIGHT_DEXTERITY + 6, strDexterity, 74, 0, RT3_SORT_LEFT);

	bool bDexSuccess = true;
	int iBaseClass = gCharacterManager.GetBaseClass(Hero->Class);

	for(int i=EQUIPMENT_HELM; i<=EQUIPMENT_BOOTS; ++i)
	{
        if( iBaseClass == CLASS_DARK )
        {
		    if((CharacterMachine->Equipment[i].Type == -1 && ( i!=EQUIPMENT_HELM && iBaseClass == CLASS_DARK )) 
				|| (CharacterMachine->Equipment[i].Type != -1 && CharacterMachine->Equipment[i].Durability<=0 ))
		    {
                bDexSuccess = false;
			    break;
		    }
        }
        else
        {
		    if((CharacterMachine->Equipment[i].Type == -1 ) ||
               (CharacterMachine->Equipment[i].Type != -1 && CharacterMachine->Equipment[i].Durability <= 0 ))
		    {
                bDexSuccess = false;
			    break;
		    }
        }
	}

	if(bDexSuccess)
	{
        int iType;
        if(iBaseClass == CLASS_DARK)       
		{

		    iType = CharacterMachine->Equipment[EQUIPMENT_ARMOR].Type;

            if(
				(iType != ITEM_ARMOR+15) 
				&& (iType != ITEM_ARMOR+20) 
				&& (iType != ITEM_ARMOR+23) 
				&& (iType != ITEM_ARMOR+32)
				&& (iType != ITEM_ARMOR+37)
				&& (iType != ITEM_ARMOR+47)
				&& (iType != ITEM_ARMOR+48)
				)
            {
                bDexSuccess = false;
            }

            iType = iType % MAX_ITEM_INDEX;
        }
        else
        {
		    iType = CharacterMachine->Equipment[EQUIPMENT_HELM].Type % MAX_ITEM_INDEX;
        }

        if(bDexSuccess)
        {
		    for(int i=EQUIPMENT_ARMOR; i<=EQUIPMENT_BOOTS; ++i)
		    {
			    if(iType != CharacterMachine->Equipment[i].Type % MAX_ITEM_INDEX)
			    {
				    bDexSuccess = false;
				    break;
			    }
		    }
        }
	}

	int t_adjdef = CharacterAttribute->Defense + Add_Ch_Dfe;

	if( g_isCharacterBuff((&Hero->Object), eBuff_HelpNpc) )
	{
		if(CharacterAttribute->Level > 180)
		{
			t_adjdef += 180 / 5 + 50;
		}
		else
		{
			t_adjdef += (CharacterAttribute->Level / 5 + 50);
		}
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_Berserker))
	{
		int nTemp = CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;
		float fTemp = (40 - int(CharacterAttribute->Energy / 60)) / 100.f;
		fTemp = MAX(fTemp, 0.1f);
		t_adjdef -= nTemp / 3 * fTemp;
	}

	int maxdefense = 0; 

	for(int j=0; j<MAX_EQUIPMENT; ++j)
	{
		int TempLevel = (CharacterMachine->Equipment[j].Level>>3)&15;
		if( TempLevel >= CharacterMachine->Equipment[j].Jewel_Of_Harmony_OptionLevel )
		{
			StrengthenCapability SC;

			g_pUIJewelHarmonyinfo->GetStrengthenCapability( &SC, &CharacterMachine->Equipment[j], 2 );

			if( SC.SI_isSD )
			{
				maxdefense += SC.SI_SD.SI_defense;
			}
		}
	}

	int iChangeRingAddDefense = 0;

	pItemRingLeft = &CharacterMachine->Equipment[EQUIPMENT_RING_LEFT];
	pItemRingRight = &CharacterMachine->Equipment[EQUIPMENT_RING_RIGHT];

	if(pItemRingLeft->Type == ITEM_HELPER+39 || pItemRingRight->Type == ITEM_HELPER+39)
	{
		iChangeRingAddDefense = (t_adjdef + maxdefense) / 10;
	}

	if(Hero->Helper.Type == MODEL_HELPER+80)
		iChangeRingAddDefense += 50;

	if(Hero->Helper.Type == MODEL_HELPER+106)
		iChangeRingAddDefense += 50;

	unicode::t_char strBlocking[256];

	if(bDexSuccess)
	{
		if(CharacterAttribute->SuccessfulBlocking > 0)
		{
			unicode::_sprintf(strBlocking, GlobalText[206], t_adjdef + maxdefense + iChangeRingAddDefense,CharacterAttribute->SuccessfulBlocking,(CharacterAttribute->SuccessfulBlocking) / 10);
		}
		else
		{
          	unicode::_sprintf(strBlocking, GlobalText[207],	t_adjdef + maxdefense + iChangeRingAddDefense,(t_adjdef + iChangeRingAddDefense) / 10);
		}
	}
	else
	{
		if(CharacterAttribute->SuccessfulBlocking > 0)
		{
          	unicode::_sprintf(strBlocking, GlobalText[208],	t_adjdef + maxdefense + iChangeRingAddDefense, CharacterAttribute->SuccessfulBlocking);
		}
		else
		{
          	unicode::_sprintf(strBlocking, GlobalText[209],	t_adjdef + maxdefense + iChangeRingAddDefense);
		}
	}

	iY = HEIGHT_DEXTERITY + 24;

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin3))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll2))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	g_pRenderText->SetBgColor(32, 32, 32, 180);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strBlocking, 130, 0, RT3_SORT_LEFT);

	WORD wAttackSpeed = CLASS_WIZARD == iBaseClass || CLASS_SUMMONER == iBaseClass ? CharacterAttribute->MagicSpeed : CharacterAttribute->AttackSpeed;

	unicode::_sprintf(strBlocking, GlobalText[64], wAttackSpeed);

	iY += 13;

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin1))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll1))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	ITEM* phelper = &CharacterMachine->Equipment[EQUIPMENT_HELPER];

	if (phelper->Durability != 0 && (phelper->Type == ITEM_HELPER + 64 || phelper->Type == ITEM_HELPER + 123))
	{
		if (IsRequireEquipItem(phelper))
		{
			if (false == pItemHelper->bExpiredPeriod)
			{
				g_pRenderText->SetTextColor(255, 0, 240, 255);
			}
		}
	}

	g_pRenderText->SetBgColor(32, 32, 32, 180);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strBlocking, 130, 0, RT3_SORT_LEFT);

	if (itemoption380Defense != 0 || iDefenseRate != 0)
	{
		unicode::_sprintf(strBlocking, GlobalText[2110], CharacterAttribute->SuccessfulBlockingPK + Add_Dfe, itemoption380Defense + iDefenseRate);
	}
	else
	{
		unicode::_sprintf(strBlocking, GlobalText[2045], CharacterAttribute->SuccessfulBlockingPK + Add_Dfe);
	}

	iY += 13;

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(32, 32, 32, 180);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strBlocking, 130, 0, RT3_SORT_LEFT);

	g_pRenderText->SetFont(g_hFontBold);

	WORD wVitality = CharacterAttribute->Vitality + CharacterAttribute->AddVitality;

	if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion3))
	{
		g_pRenderText->SetTextColor(255, 120, 0, 255);
	}
	else
	{
		if (CharacterAttribute->AddVitality)
		{
			g_pRenderText->SetTextColor(100, 150, 255, 255);
		}
		else
		{
			g_pRenderText->SetTextColor(223, 191, 103, 255);
		}
	}

	unicode::t_char strVitality[256];

	unicode::_sprintf(strVitality, GlobalText[210], wVitality);

	g_pRenderText->SetBgColor(0);

	g_pRenderText->RenderText(m_Pos.x + 25, m_Pos.y + HEIGHT_VITALITY + 6, strVitality, 74, 0, RT3_SORT_LEFT);

	g_pRenderText->SetFont(g_hFont);

	unicode::_sprintf(strVitality, GlobalText[211], CharacterAttribute->Life, CharacterAttribute->LifeMax);

	g_pRenderText->SetTextColor(255, 255, 255, 255);

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin4))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll5))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_CherryBlossom_RiceCake))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (phelper->Durability != 0 && phelper->Type == ITEM_HELPER + 65)
	{
		if (IsRequireEquipItem(phelper))
		{
			if (false == pItemHelper->bExpiredPeriod)
			{
				g_pRenderText->SetTextColor(255, 0, 240, 255);
			}
		}
	}

	g_pRenderText->SetBgColor(32, 32, 32, 180);

	iY = HEIGHT_VITALITY + 24;

	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strVitality, 130, 0, RT3_SORT_LEFT);

	g_pRenderText->SetFont(g_hFontBold);

	WORD wEnergy = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;

	if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion4))
	{
		g_pRenderText->SetTextColor(255, 120, 0, 255);
	}
	else
	{
		if (CharacterAttribute->AddEnergy)
		{
			g_pRenderText->SetTextColor(100, 150, 255, 255);
		}
		else
		{
			g_pRenderText->SetTextColor(223, 191, 103, 255);
		}
	}

	unicode::t_char strEnergy[256];

	unicode::_sprintf(strEnergy, GlobalText[212], wEnergy);

	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x + 25, m_Pos.y + HEIGHT_ENERGY + 6, strEnergy, 74, 0, RT3_SORT_LEFT);
	
	g_pRenderText->SetFont(g_hFont);
	
	unicode::_sprintf(strEnergy, GlobalText[213], CharacterAttribute->Mana,CharacterAttribute->ManaMax);

	g_pRenderText->SetBgColor(32, 32, 32, 180);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin5))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll6))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_CherryBlossom_Liguor))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	iY = HEIGHT_ENERGY + 24;

	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strEnergy, 130, 0, RT3_SORT_LEFT);

	if (iBaseClass == CLASS_WIZARD || iBaseClass == CLASS_DARK || iBaseClass == CLASS_SUMMONER)
	{
		int Level = (pWeaponRight->Level >> 3) & 15;
		int iMagicDamageMin;
		int iMagicDamageMax;

		gCharacterManager.GetMagicSkillDamage(CharacterAttribute->Skill[Hero->CurrentSkill], &iMagicDamageMin, &iMagicDamageMax);

		int iMagicDamageMinInitial = iMagicDamageMin;
		int iMagicDamageMaxInitial = iMagicDamageMax;

		iMagicDamageMin += Add_Mana_Min;
		iMagicDamageMax += Add_Mana_Max;

		if (CharacterAttribute->Ability & ABILITY_PLUS_DAMAGE)
		{
			iMagicDamageMin += 10;
			iMagicDamageMax += 10;
		}

		int maxMg = 0;

		for (int j = 0; j < MAX_EQUIPMENT; ++j)
		{
			int TempLevel = (CharacterMachine->Equipment[j].Level >> 3) & 15;

			if (TempLevel >= CharacterMachine->Equipment[j].Jewel_Of_Harmony_OptionLevel)
			{
				StrengthenCapability SC;
				g_pUIJewelHarmonyinfo->GetStrengthenCapability(&SC, &CharacterMachine->Equipment[j], 1);

				if (SC.SI_isSP)
				{
					maxMg += SC.SI_SP.SI_magicalpower;
				}
			}
		}

		pItemRingLeft = &CharacterMachine->Equipment[EQUIPMENT_RING_LEFT];
		pItemRingRight = &CharacterMachine->Equipment[EQUIPMENT_RING_RIGHT];


		int iNonExpiredLRingType = -1;
		int iNonExpiredRRingType = -1;

		if (!pItemRingLeft->bPeriodItem || !pItemRingLeft->bExpiredPeriod)
		{
			iNonExpiredLRingType = pItemRingLeft->Type;
		}
		if (!pItemRingRight->bPeriodItem || !pItemRingRight->bExpiredPeriod)
		{
			iNonExpiredRRingType = pItemRingRight->Type;
		}

		int maxIMagicDamageMin = 0;
		int maxIMagicDamageMax = 0;

		if (iNonExpiredLRingType == ITEM_HELPER + 41 || iNonExpiredRRingType == ITEM_HELPER + 41)
		{
			maxIMagicDamageMin = max(maxIMagicDamageMin, 20);
			maxIMagicDamageMax = max(maxIMagicDamageMax, 20);
		}
		if (iNonExpiredLRingType == ITEM_HELPER + 76 || iNonExpiredRRingType == ITEM_HELPER + 76)
		{
			maxIMagicDamageMin = max(maxIMagicDamageMin, 30);
			maxIMagicDamageMax = max(maxIMagicDamageMax, 30);
		}
		if (iNonExpiredLRingType == ITEM_HELPER + 122 || iNonExpiredRRingType == ITEM_HELPER + 122)
		{
			maxIMagicDamageMin = max(maxIMagicDamageMin, 40);
			maxIMagicDamageMax = max(maxIMagicDamageMax, 40);
		}

		iMagicDamageMin += maxIMagicDamageMin;
		iMagicDamageMax += maxIMagicDamageMax;

		pItemHelper = &CharacterMachine->Equipment[EQUIPMENT_HELPER];
		if (pItemHelper)
		{
			if (pItemHelper->Type == ITEM_HELPER + 37 && pItemHelper->Option1 == 0x04)
			{
				WORD wLevel = CharacterAttribute->Level;
				iMagicDamageMin += (wLevel / 25);
				iMagicDamageMax += (wLevel / 25);
			}

			if (pItemHelper->Type == ITEM_HELPER + 64)
			{
				if (false == pItemHelper->bExpiredPeriod)
				{
					iMagicDamageMin += int(float(iMagicDamageMin) * 0.4f);
					iMagicDamageMax += int(float(iMagicDamageMax) * 0.4f);
				}

			}
			if (pItemHelper->Type == ITEM_HELPER + 123)
			{
				if (false == pItemHelper->bExpiredPeriod)
				{
					iMagicDamageMin += int(float(iMagicDamageMin) * 0.2f);
					iMagicDamageMax += int(float(iMagicDamageMax) * 0.2f);
				}
			}
			if (pItemHelper->Type == ITEM_HELPER + 1)
			{
				iMagicDamageMin += int(float(iMagicDamageMin) * 0.3f);
				iMagicDamageMax += int(float(iMagicDamageMax) * 0.3f);
			}
		}

		if (g_isCharacterBuff((&Hero->Object), eBuff_Berserker))
		{
			int nTemp = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;
			float fTemp = int(CharacterAttribute->Energy / 30) / 100.f;
			iMagicDamageMin += nTemp / 9 * fTemp;
			iMagicDamageMax += nTemp / 4 * fTemp;
		}

		if ((pWeaponRight->Type >= MODEL_STAFF - MODEL_ITEM
			&& pWeaponRight->Type < (MODEL_STAFF + MAX_ITEM_INDEX - MODEL_ITEM))
			|| pWeaponRight->Type == (MODEL_SWORD + 31 - MODEL_ITEM)
			|| pWeaponRight->Type == (MODEL_SWORD + 23 - MODEL_ITEM)
			|| pWeaponRight->Type == (MODEL_SWORD + 25 - MODEL_ITEM)
			|| pWeaponRight->Type == (MODEL_SWORD + 21 - MODEL_ITEM)
			|| pWeaponRight->Type == (MODEL_SWORD + 28 - MODEL_ITEM)
			)
		{
			float magicPercent = (float)(pWeaponRight->MagicPower) / 100;

			ITEM_ATTRIBUTE* p = &ItemAttribute[pWeaponRight->Type];
			float   percent = CalcDurabilityPercent(pWeaponRight->Durability, p->MagicDur, pWeaponRight->Level, pWeaponRight->Option1, pWeaponRight->ExtOption);

			magicPercent = magicPercent - magicPercent * percent;
			unicode::_sprintf(strEnergy, GlobalText[215], iMagicDamageMin + maxMg, iMagicDamageMax + maxMg, (int)((iMagicDamageMaxInitial + maxMg) * magicPercent));
		}
		else
		{
			unicode::_sprintf(strEnergy, GlobalText[216], iMagicDamageMin + maxMg, iMagicDamageMax + maxMg);
		}

		iY += 13;
		g_pRenderText->SetBgColor(32, 32, 32, 180);
		g_pRenderText->SetTextColor(255, 255, 255, 255);

		if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin2))
		{
			g_pRenderText->SetTextColor(255, 0, 240, 255);
		}

		if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll4))
		{
			g_pRenderText->SetTextColor(255, 0, 240, 255);
		}

		if (g_isCharacterBuff((&Hero->Object), eBuff_CherryBlossom_Petal))
		{
			g_pRenderText->SetTextColor(255, 0, 240, 255);
		}

		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strEnergy, 130, 0, RT3_SORT_LEFT);
	}

	if (iBaseClass == CLASS_SUMMONER)
	{
		int iCurseDamageMin = 0;
		int iCurseDamageMax = 0;

		gCharacterManager.GetCurseSkillDamage(CharacterAttribute->Skill[Hero->CurrentSkill], &iCurseDamageMin, &iCurseDamageMax);

		if (g_isCharacterBuff((&Hero->Object), eBuff_Berserker))
		{
			int nTemp = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;
			float fTemp = int(CharacterAttribute->Energy / 30) / 100.f;
			iCurseDamageMin += nTemp / 9 * fTemp;
			iCurseDamageMax += nTemp / 4 * fTemp;
		}

		int iNonExpiredLRingType = -1;
		int iNonExpiredRRingType = -1;

		if (!pItemRingLeft->bPeriodItem || !pItemRingLeft->bExpiredPeriod)
		{
			iNonExpiredLRingType = pItemRingLeft->Type;
		}
		if (!pItemRingRight->bPeriodItem || !pItemRingRight->bExpiredPeriod)
		{
			iNonExpiredRRingType = pItemRingRight->Type;
		}

		int maxICurseDamageMin = 0;
		int maxICurseDamageMax = 0;

		if (iNonExpiredLRingType == ITEM_HELPER + 76 || iNonExpiredRRingType == ITEM_HELPER + 76)
		{
			maxICurseDamageMin = max(maxICurseDamageMin, 30);
			maxICurseDamageMax = max(maxICurseDamageMax, 30);
		}
		if (iNonExpiredLRingType == ITEM_HELPER + 122 || iNonExpiredRRingType == ITEM_HELPER + 122)
		{
			maxICurseDamageMin = max(maxICurseDamageMin, 40);
			maxICurseDamageMax = max(maxICurseDamageMax, 40);
		}

		iCurseDamageMin += maxICurseDamageMin;
		iCurseDamageMax += maxICurseDamageMax;

		pItemHelper = &CharacterMachine->Equipment[EQUIPMENT_HELPER];
		if (pItemHelper)
		{
			if (pItemHelper->Type == ITEM_HELPER + 64)
			{
				if (false == pItemHelper->bExpiredPeriod)
				{
					iCurseDamageMin += int(float(iCurseDamageMin) * 0.4f);
					iCurseDamageMax += int(float(iCurseDamageMax) * 0.4f);
				}
			}
			if (pItemHelper->Type == ITEM_HELPER + 123)
			{
				if (false == pItemHelper->bExpiredPeriod)
				{
					iCurseDamageMin += int(float(iCurseDamageMin) * 0.2f);
					iCurseDamageMax += int(float(iCurseDamageMax) * 0.2f);
				}
			}
		}

		if (ITEM_STAFF + 21 <= pWeaponLeft->Type && pWeaponLeft->Type <= ITEM_STAFF + 29)
		{
			float fCursePercent = (float)(pWeaponLeft->MagicPower) / 100;

			ITEM_ATTRIBUTE* p = &ItemAttribute[pWeaponLeft->Type];
			float fPercent = ::CalcDurabilityPercent(pWeaponLeft->Durability,
				p->MagicDur, pWeaponLeft->Level, pWeaponLeft->Option1,
				pWeaponLeft->ExtOption);

			fCursePercent -= fCursePercent * fPercent;
			unicode::_sprintf(strEnergy, GlobalText[1693],
				iCurseDamageMin, iCurseDamageMax,
				(int)((iCurseDamageMax)*fCursePercent));
		}
		else
		{
			unicode::_sprintf(strEnergy, GlobalText[1694],
				iCurseDamageMin, iCurseDamageMax);
		}

		iY += 13;
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strEnergy, 130, 0, RT3_SORT_LEFT);
	}

	iY += 13;

	if (iBaseClass == CLASS_KNIGHT)
	{
		unicode::_sprintf(strEnergy, GlobalText[582], 200 + (wEnergy / 10));
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strEnergy, 130, 0, RT3_SORT_LEFT);
	}

	if (iBaseClass == CLASS_DARK)
	{
		unicode::_sprintf(strEnergy, GlobalText[582], 200);
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strEnergy, 130, 0, RT3_SORT_LEFT);
	}

	if (iBaseClass == CLASS_DARK_LORD)
	{
		unicode::_sprintf(strEnergy, GlobalText[582], 200 + (wEnergy / 20));
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strEnergy, 130, 0, RT3_SORT_LEFT);
	}

	if (iBaseClass == CLASS_DARK_LORD)
	{
		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->SetBgColor(0);

		WORD wCharisma;

		wCharisma = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;

		if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion5))
		{
			g_pRenderText->SetTextColor(255, 120, 0, 255);
		}
		else
		{
			if (CharacterAttribute->AddCharisma)
			{
				g_pRenderText->SetTextColor(100, 150, 255, 255);
			}
			else
			{
				g_pRenderText->SetTextColor(223, 191, 103, 255);
			}
		}

		unicode::t_char strCharisma[256];

		unicode::_sprintf(strCharisma, GlobalText[1170], wCharisma);

		g_pRenderText->RenderText(m_Pos.x + 22, m_Pos.y + HEIGHT_CHARISMA + 6, strCharisma, 74, 0, RT3_SORT_LEFT);
	}
}

void SEASON3B::CNewUICharacterInfoWindow::RenderButtons()
{
	EnableAlphaTest();

	if (CharacterAttribute->LevelUpPoint > 0)
	{
		RenderBitmap(this->EventState[0] == FALSE ? IMAGE_CHAINFO_BTN_STAT : IMAGE_CHAINFO_BTN_STAT_CLICK, m_Pos.x + 125, m_Pos.y + HEIGHT_STRENGTH, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
		RenderBitmap(this->EventState[1] == FALSE ? IMAGE_CHAINFO_BTN_STAT : IMAGE_CHAINFO_BTN_STAT_CLICK, m_Pos.x + 125, m_Pos.y + HEIGHT_DEXTERITY - 2, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
		RenderBitmap(this->EventState[2] == FALSE ? IMAGE_CHAINFO_BTN_STAT : IMAGE_CHAINFO_BTN_STAT_CLICK, m_Pos.x + 125, m_Pos.y + HEIGHT_VITALITY - 2, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
		RenderBitmap(this->EventState[3] == FALSE ? IMAGE_CHAINFO_BTN_STAT : IMAGE_CHAINFO_BTN_STAT_CLICK, m_Pos.x + 125, m_Pos.y + HEIGHT_ENERGY - 2, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);

		if (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_DARK_LORD)
		{
			RenderBitmap(this->EventState[4] == FALSE ? IMAGE_CHAINFO_BTN_STAT : IMAGE_CHAINFO_BTN_STAT_CLICK, m_Pos.x + 125, m_Pos.y + HEIGHT_CHARISMA - 2, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
		}	
	}

	char szBuffer[256] = { 0 };

	if (SEASON3B::CheckMouseIn(m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0) == true)
	{
		sprintf(szBuffer, GlobalText[927], "C");

		RenderTipText(m_Pos.x + 20, m_Pos.y + 380, szBuffer);
	}

	RenderBitmap(IMAGE_CHAINFO_BTN_EXIT, m_Pos.x + 23, m_Pos.y + 395, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);

	if (SEASON3B::CheckMouseIn(m_Pos.x + 56, m_Pos.y + 395, 24.0, 24.0) == true)
	{
		sprintf(szBuffer, "%s(%s)", GlobalText[1140], "T");

		RenderTipText(m_Pos.x + 50, m_Pos.y + 380, szBuffer);
	}

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MYQUEST) == true)
	{
		RenderBitmap(IMAGE_CHAINFO_BTN_QUEST_CLICK, m_Pos.x + 53, m_Pos.y + 395, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
	}
	else
	{
		RenderBitmap(IMAGE_CHAINFO_BTN_QUEST, m_Pos.x + 53, m_Pos.y + 395, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
	}

	if (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_DARK_LORD)
	{
		if (SEASON3B::CheckMouseIn(m_Pos.x + 83, m_Pos.y + 395, 24.0, 24.0) == true)
		{
			sprintf(szBuffer, GlobalText[1217]);

			RenderTipText(m_Pos.x + 77, m_Pos.y + 380, szBuffer);
		}

		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_PET) == true)
		{
			RenderBitmap(IMAGE_CHAINFO_BTN_PET_CLICK, m_Pos.x + 83, m_Pos.y + 395, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
		}
		else
		{
			RenderBitmap(IMAGE_CHAINFO_BTN_PET, m_Pos.x + 83, m_Pos.y + 395, 24.0, 24.0, 0, 0, 0.75, 0.75, 1, 1, 0.0);
		}
	}

	DisableAlphaBlend();
}

float SEASON3B::CNewUICharacterInfoWindow::GetLayerDepth()
{ 
	return 5.1f; 
}

void SEASON3B::CNewUICharacterInfoWindow::LoadImages()
{
	LoadBitmap("Common\\Interface\\ui_item_backtop.jpg", IMAGE_CHAINFO_TOP);
	LoadBitmap("Common\\Interface\\ui_item_backlow.jpg", IMAGE_CHAINFO_LOW);

	LoadBitmap("Common\\Interface\\ui_level_box.jpg", IMAGE_CHAINFO_LEVEL_BOX);

	LoadBitmap("Common\\Interface\\ui_charinfo_addstat.jpg", IMAGE_CHAINFO_BTN_STAT);
	LoadBitmap("Common\\Interface\\ui_charinfo_addstat_click.jpg", IMAGE_CHAINFO_BTN_STAT_CLICK);

	LoadBitmap("Common\\Interface\\ui_menu_exit.jpg", IMAGE_CHAINFO_BTN_EXIT);
	
	LoadBitmap("Common\\Interface\\ui_charinfo_btn_quest.jpg", IMAGE_CHAINFO_BTN_QUEST);
	LoadBitmap("Common\\Interface\\ui_charinfo_btn_quest_click.jpg", IMAGE_CHAINFO_BTN_QUEST_CLICK);

	LoadBitmap("Common\\Interface\\ui_charinfo_btn_pet.jpg", IMAGE_CHAINFO_BTN_PET);
	LoadBitmap("Common\\Interface\\ui_charinfo_btn_pet_click.jpg", IMAGE_CHAINFO_BTN_PET_CLICK);
}

void SEASON3B::CNewUICharacterInfoWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_CHAINFO_BTN_PET);
	DeleteBitmap(IMAGE_CHAINFO_BTN_PET_CLICK);
	DeleteBitmap(IMAGE_CHAINFO_BTN_QUEST);
	DeleteBitmap(IMAGE_CHAINFO_BTN_QUEST_CLICK);
	DeleteBitmap(IMAGE_CHAINFO_BTN_STAT);
	DeleteBitmap(IMAGE_CHAINFO_BTN_STAT_CLICK);
	DeleteBitmap(IMAGE_CHAINFO_LEVEL_BOX);
	DeleteBitmap(IMAGE_CHAINFO_BTN_EXIT);

	DeleteBitmap(IMAGE_CHAINFO_TOP);
	DeleteBitmap(IMAGE_CHAINFO_LOW);
}

void SEASON3B::CNewUICharacterInfoWindow::OpenningProcess()
{
	ResetEquipmentLevel();

	g_csItemOption.init();

	if( CharacterMachine->IsZeroDurability() )
	{
		CharacterMachine->CalculateAll();
	}
}

void SEASON3B::CNewUICharacterInfoWindow::ResetEquipmentLevel()
{
	ITEM *pItem = CharacterMachine->Equipment;

	Hero->Weapon[0].Level = (pItem[EQUIPMENT_WEAPON_RIGHT].Level>>3)&15;
	Hero->Weapon[1].Level = (pItem[EQUIPMENT_WEAPON_LEFT ].Level>>3)&15;

	Hero->BodyPart[BODYPART_HELM ].Level = (pItem[EQUIPMENT_HELM].Level>>3)&15;
	Hero->BodyPart[BODYPART_ARMOR].Level = (pItem[EQUIPMENT_ARMOR].Level>>3)&15;
	Hero->BodyPart[BODYPART_PANTS].Level = (pItem[EQUIPMENT_PANTS].Level>>3)&15;
	Hero->BodyPart[BODYPART_GLOVES].Level = (pItem[EQUIPMENT_GLOVES].Level>>3)&15;
	Hero->BodyPart[BODYPART_BOOTS].Level = (pItem[EQUIPMENT_BOOTS].Level>>3)&15;

	CheckFullSet(Hero);	
}

