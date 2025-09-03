///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UIManager.h"
#include "GuildCache.h"
#include "ZzzOpenglUtil.h"
#include "ZzzBMD.h"
#include "ZzzInfomation.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzLodTerrain.h"
#include "ZzzInterface.h"
#include "ZzzInventory.h"
#include "ZzzTexture.h"
#include "ZzzOpenData.h"
#include "ZzzScene.h"
#include "ZzzEffect.h"
#include "ZzzAI.h"
#include "DSPlaySound.h"
#include "wsclientinline.h"
#include "SMD.h"
#include "Local.h"
#include "MatchEvent.h"
#include "PhysicsManager.h"
#include "./Utilities/Log/ErrorReport.h"
#include "CSQuest.h"
#include "PersonalShopTitleImp.h"
#include "uicontrols.h"
#include "GOBoid.h"
#include "GMHellas.h"
#include "CSItemOption.h"
#include "GMBattleCastle.h"
#include "GMHuntingGround.h"
#include "GMAida.h"
#include "GMCrywolf1st.h"
#include "npcBreeder.h"
#include "CSPetSystem.h"
#include "GIPetManager.h"
#include "CComGem.h"
#include "UIMapName.h"	// rozy
#include "./Time/Timer.h"
#include "Input.h"
#include "UIMng.h"
#include "LoadingScene.h"
#include "CDirection.h"
#include "GM_Kanturu_3rd.h"
#include "Event.h"
#include "./Utilities/Log/muConsoleDebug.h"
#include "MixMgr.h"
#include "GameCensorship.h"
#include "GM3rdChangeUp.h"
#include "NewUISystem.h"
#include "NewUICommonMessageBox.h"
#include "PartyManager.h"
#include "w_CursedTemple.h"
#include "w_MapHeaders.h"
#include "w_PetProcess.h"
#include "PortalMgr.h"
#include "ServerListManager.h"
#include "MapManager.h"

extern CUITextInputBox * g_pSingleTextInputBox;
extern CUITextInputBox * g_pSinglePasswdInputBox;
extern int g_iChatInputType;
extern BOOL g_bUseChatListBox;
extern DWORD g_dwMouseUseUIID;
extern DWORD g_dwActiveUIID;
extern DWORD g_dwKeyFocusUIID;	
extern CUIMapName* g_pUIMapName;
extern bool HighLight;
extern CTimer*	g_pTimer;
	
bool g_bTimeCheck = false;
int g_iBackupTime = 0;

float g_fMULogoAlpha = 0;

extern float g_fSpecialHeight;

short   g_shCameraLevel = 0;

int g_iLengthAuthorityCode = 20;

char *szServerIpAddress = "192.168.0.100";

WORD g_ServerPort = 44405;

int  SceneFlag = WEBZEN_SCENE;

int MoveSceneFrame = 0;

extern int g_iKeyPadEnable;

CPhysicsManager g_PhysicsManager;

char *g_lpszMp3[NUM_MUSIC] =
{
	"data\\music\\Pub.mp3",
	"data\\music\\Mutheme.mp3",
	"data\\music\\Church.mp3",
	"data\\music\\Devias.mp3",
	"data\\music\\Noria.mp3",
	"data\\music\\Dungeon.mp3",
	"data\\music\\atlans.mp3",
	"data\\music\\icarus.mp3",
	"data\\music\\tarkan.mp3",
	"data\\music\\lost_tower_a.mp3",
	"data\\music\\lost_tower_b.mp3",
	"data\\music\\kalima.mp3",
    "data\\music\\castle.mp3",
    "data\\music\\charge.mp3",
    "data\\music\\lastend.mp3",
	"data\\music\\huntingground.mp3",
	"data\\music\\Aida.mp3",
	"data\\music\\crywolf1st.mp3",
	"data\\music\\crywolf_ready-02.ogg",
	"data\\music\\crywolf_before-01.ogg",
	"data\\music\\crywolf_back-03.ogg",
	"data\\music\\main_theme.mp3",
	"data\\music\\kanturu_1st.mp3",
	"data\\music\\kanturu_2nd.mp3",
	"data\\music\\KanturuMayaBattle.mp3",
	"data\\music\\KanturuNightmareBattle.mp3",
	"data\\music\\KanturuTower.mp3",
	"data\\music\\BalgasBarrack.mp3",
	"data\\music\\BalgasRefuge.mp3",
	"data\\music\\cursedtemplewait.mp3",
	"data\\music\\cursedtempleplay.mp3",
	"data\\music\\elbeland.mp3",
	"data\\music\\login_theme.mp3",
	"data\\music\\SwampOfCalmness.mp3",
	"data\\music\\Raklion.mp3",
	"data\\music\\Raklion_Hatchery.mp3",
	"data\\music\\Santa_Village.mp3",
	"data\\music\\DuelArena.mp3",
	"data\\music\\PK_Field.mp3",
	"data\\music\\ImperialGuardianFort.mp3",
	"data\\music\\ImperialGuardianFort.mp3",
	"data\\music\\ImperialGuardianFort.mp3",
	"data\\music\\ImperialGuardianFort.mp3",
	"data\\music\\iDoppelganger.mp3",
	"data\\music\\iDoppelganger.mp3",
#ifdef ASG_ADD_MAP_KARUTAN
	"data\\music\\Karutan_A.mp3",
	"data\\music\\Karutan_B.mp3",
#endif	// ASG_ADD_MAP_KARUTAN
};

extern char Mp3FileName[256];

#define MAX_LENGTH_CMB	( 38)

DWORD   g_dwWaitingStartTick;
int     g_iRequestCount;

int     g_iMessageTextStart     = 0;
char    g_cMessageTextCurrNum   = 0;
char    g_cMessageTextNum       = 0;
int     g_iNumLineMessageBoxCustom;
char    g_lpszMessageBoxCustom[NUM_LINE_CMB][MAX_LENGTH_CMB];
int     g_iCustomMessageBoxButton[NUM_BUTTON_CMB][NUM_PAR_BUTTON_CMB];

int     g_iCustomMessageBoxButton_Cancel[NUM_PAR_BUTTON_CMB];

int		g_iCancelSkillTarget	= 0;

#define NUM_LINE_DA		( 1)
int g_iCurrentDialogScript = -1;
int g_iNumAnswer = 0;
char g_lpszDialogAnswer[MAX_ANSWER_FOR_DIALOG][NUM_LINE_DA][MAX_LENGTH_CMB];

DWORD GenerateCheckSum2( BYTE *pbyBuffer, DWORD dwSize, WORD wKey);

void StopMusic()
{
	for ( int i = 0; i < NUM_MUSIC; ++i)
	{
		StopMp3( g_lpszMp3[i]);
	}
}

bool CheckAbuseFilter(char *Text, bool bCheckSlash)
{
	if (bCheckSlash == true)
	{
		if ( Text[0] == '/')
		{
			return false;
		}
	}

	int icntText = 0;
	char TmpText[2048];
	for( int i=0; i<(int)strlen(Text); ++i )
	{
		if ( Text[i]!=32 )
		{
			TmpText[icntText] = Text[i];
			icntText++;
		}
	}
	TmpText[icntText] = 0;

	for(int i=0;i<AbuseFilterNumber;i++)
	{
        if(FindText(TmpText,AbuseFilter[i]))
		{
			return true;
		}
	}
	return false;
}

bool CheckAbuseNameFilter(char *Text)
{
	int icntText = 0;
	char TmpText[256];
	for( int i=0; i<(int)strlen(Text); ++i )
	{
		if ( Text[i]!=32 )
		{
			TmpText[icntText] = Text[i];
			icntText++;
		}
	}
	TmpText[icntText] = 0;

	for(int i=0;i<AbuseNameFilterNumber;i++)
	{
		if(FindText(TmpText,AbuseNameFilter[i]))
		{
			return true;
		}
	}
	return false;
}

bool CheckName()
{
    if( CheckAbuseNameFilter(InputText[0]) || CheckAbuseFilter(InputText[0]) ||
		FindText(InputText[0]," ") || FindText(InputText[0],"��") ||
		FindText(InputText[0],".") || FindText(InputText[0],"��") || FindText(InputText[0],"��") ||
		FindText(InputText[0],"Webzen") || FindText(InputText[0],"WebZen") || FindText(InputText[0],"webzen") ||  FindText(InputText[0],"WEBZEN") ||
		FindText(InputText[0],GlobalText[457]) || FindText(InputText[0],GlobalText[458]))
		return true;
	return false;
}

#ifdef MOVIE_DIRECTSHOW
void MovieScene(HDC hDC)
{
	if(g_pMovieScene->GetPlayNum() == 0)
	{
		g_pMovieScene->InitOpenGLClear(hDC);
		
		g_pMovieScene->Initialize_DirectShow(g_hWnd, MOVIE_FILE_WMV);
		
		if(g_pMovieScene->IsFile() == FALSE || g_pMovieScene->IsFailDirectShow() == TRUE)
		{
			g_pMovieScene->Destroy();
			SAFE_DELETE(g_pMovieScene);
			SceneFlag = WEBZEN_SCENE;
			return;	
		}

		g_pMovieScene->PlayMovie();

		if(g_pMovieScene->IsEndMovie())
		{
			g_pMovieScene->Destroy();
			SAFE_DELETE(g_pMovieScene);
			SceneFlag = WEBZEN_SCENE;
			return;
		}
		else
		{
			if(HIBYTE(GetAsyncKeyState(VK_ESCAPE))==128 || HIBYTE(GetAsyncKeyState(VK_RETURN))==128)
			{
				g_pMovieScene->Destroy();
				SAFE_DELETE(g_pMovieScene);
				SceneFlag = WEBZEN_SCENE;
				return;
			}
		}
	}
	else
	{
		g_pMovieScene->Destroy();
		SAFE_DELETE(g_pMovieScene);
		SceneFlag = WEBZEN_SCENE;
		return;
	}
}
#endif // MOVIE_DIRECTSHOW

bool EnableMainRender = false;
extern int HeroKey;

void WebzenScene(HDC hDC)
{
	CUIMng& rUIMng = CUIMng::Instance();

	OpenFont();

	ClearInput();

	LoadBitmap("Interface\\lo_back_01.jpg", BITMAP_TITLE, GL_LINEAR);
	LoadBitmap("Interface\\lo_back_02.jpg", BITMAP_TITLE + 1, GL_LINEAR);
	LoadBitmap("Interface\\lo_mu_logo.tga", BITMAP_TITLE + 2, GL_LINEAR);
	LoadBitmap("Interface\\lo_121518.tga", BITMAP_TITLE + 3, GL_LINEAR);
	LoadBitmap("Interface\\lo_webzen_logo.tga", BITMAP_TITLE + 4, GL_LINEAR);
	LoadBitmap("Interface\\lo_lo.jpg", BITMAP_TITLE + 5, GL_LINEAR, GL_REPEAT);

	rUIMng.CreateTitleSceneUI();
	
	FogEnable = false;
	
	::EnableAlphaTest();
	OpenBasicData(hDC);

	g_pNewUISystem->LoadMainSceneInterface();

	CUIMng::Instance().RenderTitleSceneUI(hDC, 11, 11);

	rUIMng.ReleaseTitleSceneUI();

	DeleteBitmap(BITMAP_TITLE);
   	DeleteBitmap(BITMAP_TITLE+1);
	DeleteBitmap(BITMAP_TITLE+2);
   	DeleteBitmap(BITMAP_TITLE+3);
	DeleteBitmap(BITMAP_TITLE+4);
   	DeleteBitmap(BITMAP_TITLE+5);

	g_ErrorReport.Write( "> Loading ok.\r\n");

	SceneFlag = LOG_IN_SCENE;
}

int MenuStateCurrent = MENU_SERVER_LIST;
int MenuStateNext    = MENU_SERVER_LIST;
int DeleteGuildIndex = -1;

void DeleteCharacter()
{
	SelectedHero = -1;
	if (g_iChatInputType == 1)
	{
		g_pSinglePasswdInputBox->GetText(InputText[0]);
		g_pSinglePasswdInputBox->SetText(NULL);
		g_pSinglePasswdInputBox->SetState(UISTATE_HIDE);
	}
	SendRequestDeleteCharacter(CharactersClient[SelectedHero].ID,InputText[0]);

	MenuStateCurrent = MENU_DELETE_LEFT;
	MenuStateNext    = MENU_NEW_DOWN;
	PlayBuffer(SOUND_MENU01);
	
	ClearInput();
	InputEnable = false;
}
int  ErrorMessage = NULL;
int	 ErrorMessageNext = NULL;
extern bool g_bEnterPressed;

bool IsEnterPressed() {
	return g_bEnterPressed;
}

void SetEnterPressed( bool enterpressed ) {
	g_bEnterPressed = enterpressed;
}

BOOL CheckOptionMouseClick(int iOptionPos_y, BOOL bPlayClickSound = TRUE)
{
	if (CheckMouseIn((640-120)/2, 30+iOptionPos_y, 120, 22) && MouseLButtonPush)
	{
		MouseLButtonPush = false;
		MouseUpdateTime = 0;
		MouseUpdateTimeMax = 6;
		if (bPlayClickSound == TRUE) PlayBuffer(SOUND_CLICK01);
		return TRUE;
	}
	return FALSE;
}

int SeparateTextIntoLines( const char *lpszText, char *lpszSeparated, int iMaxLine, int iLineSize)
{
	int iLine = 0;
	const char *lpLineStart = lpszText;
	char *lpDst = lpszSeparated;
	const char *lpSpace = NULL;
	int iMbclen = 0;
	for ( const char* lpSeek = lpszText; *lpSeek; lpSeek += iMbclen, lpDst += iMbclen)
	{
		iMbclen = _mbclen( ( unsigned char*)lpSeek);
		if ( iMbclen + ( int)( lpSeek - lpLineStart) >= iLineSize)
		{
			if ( lpSpace && ( int)( lpSeek - lpSpace) < min( 10, iLineSize / 2))
			{
				lpDst -= ( lpSeek - lpSpace - 1);
				lpSeek = lpSpace + 1;
			}

			lpLineStart = lpSeek;
			*lpDst = '\0';
			if ( iLine >= iMaxLine - 1)
			{
				break;
			}
			++iLine;
			lpDst = lpszSeparated + iLine * iLineSize;
			lpSpace = NULL;
		}

		memcpy( lpDst, lpSeek, iMbclen);
		if ( *lpSeek == ' ')
		{
			lpSpace = lpSeek;
		}
	}
	*lpDst = '\0';

	return ( iLine + 1);
}

void SetEffectVolumeLevel ( int level )
{
	if(level > 9)
		level = 9;
	if(level < 0)
		level = 0;

	if(level == 0)
	{
		SetMasterVolume(-10000);
	}
	else
	{
		long vol = -2000*log10(10.f/float(level));
		SetMasterVolume(vol);
	}
}

void SetViewPortLevel ( int Wheel )
{
    if ( (HIBYTE( GetAsyncKeyState(VK_CONTROL))==128) )
    {
        if ( Wheel>0 )
            g_shCameraLevel--;
        else if ( Wheel<0 )
            g_shCameraLevel++;

        MouseWheel = 0;

	    if ( g_shCameraLevel>4 )
		    g_shCameraLevel = 4;
	    if ( g_shCameraLevel<0 )
		    g_shCameraLevel = 0;
    }
}

void RenderInfomation3D()
{
	bool Success = false;

	if ( ( ( ErrorMessage==MESSAGE_TRADE_CHECK || ErrorMessage==MESSAGE_CHECK ) && AskYesOrNo==1 ) 
		|| ErrorMessage==MESSAGE_USE_STATE 
		|| ErrorMessage==MESSAGE_USE_STATE2) 
	{
		Success = true;
	}

    if ( ErrorMessage==MESSAGE_TRADE_CHECK && AskYesOrNo==5 )
	{
		Success = true;
	}
	if ( ErrorMessage==MESSAGE_PERSONALSHOP_WARNING ) 
	{
		Success = true;
	}

	if ( Success )
    {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glViewport2(0,0,WindowWidth,WindowHeight);
        gluPerspective2(1.f,(float)(WindowWidth)/(float)(WindowHeight),CameraViewNear,CameraViewFar);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        GetOpenGLMatrix(CameraMatrix);
        EnableDepthTest();
        EnableDepthMask();

        float Width, Height;
        float x = (640-150)/2;
        float y;
        if ( ErrorMessage==MESSAGE_TRADE_CHECK )
        {
            y = 60+55;
        }
        else
        {
            y = 60+55;
        }

	    Width=40.f;Height=60.f;
		int iRenderType = ErrorMessage;
		if(AskYesOrNo == 5)
			iRenderType = MESSAGE_USE_STATE;
		switch( iRenderType )
		{
		case MESSAGE_USE_STATE :
		case MESSAGE_USE_STATE2 :
        case MESSAGE_PERSONALSHOP_WARNING :
            RenderItem3D(x,y,Width,Height,TargetItem.Type,TargetItem.Level,TargetItem.Option1,TargetItem.ExtOption,true);
			break;

		default :
            RenderItem3D(x,y,Width,Height,PickItem.Type,PickItem.Level,PickItem.Option1,PickItem.ExtOption,true);
			break;
		}

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		UpdateMousePositionn();
    }
}

void RenderInfomation()
{
	RenderNotices();
    
	CUIMng::Instance().Render();

	if(SceneFlag == LOG_IN_SCENE || SceneFlag == CHARACTER_SCENE)
	{
		RenderCursor();
	}
	
    RenderInfomation3D();
}

BOOL ShowCheckBox( int num, int index, int message )
{
	if ( message==MESSAGE_USE_STATE || message==MESSAGE_USE_STATE2)
    {
        char Name[50] = { 0, };
        if ( TargetItem.Type==ITEM_HELPER+15 )
        {
            switch ( (TargetItem.Level>>3)&15 )
            {
            case 0:sprintf(Name,"%s", GlobalText[168] );break;
            case 1:sprintf(Name,"%s", GlobalText[169] );break;
            case 2:sprintf(Name,"%s", GlobalText[167] );break;
            case 3:sprintf(Name,"%s", GlobalText[166] );break;
			case 4:sprintf(Name,"%s", GlobalText[1900] );break;
            }
        }

		if (message==MESSAGE_USE_STATE2)
			sprintf ( g_lpszMessageBoxCustom[0], "( %s%s )", Name, GlobalText[1901] );
		else
			sprintf ( g_lpszMessageBoxCustom[0], "( %s )", Name );
		
        num++;
        for ( int i=1; i<num; ++i )
        {
	        sprintf ( g_lpszMessageBoxCustom[i], GlobalText[index] );
        }
        g_iNumLineMessageBoxCustom = num;
    }
	else if ( message==MESSAGE_PERSONALSHOP_WARNING )
	{
		char szGold[256];
		ConvertGold(InputGold,szGold);
	    sprintf ( g_lpszMessageBoxCustom[0], GlobalText[index], szGold );

        for ( int i=1; i<num; ++i )
        {
	        sprintf ( g_lpszMessageBoxCustom[i], GlobalText[index+i] );
        }
        g_iNumLineMessageBoxCustom = num;
	}
    else if ( message==MESSAGE_CHAOS_CASTLE_CHECK )
    {
	    g_iNumLineMessageBoxCustom = 0;
        for ( int i=0; i<num; ++i )
        {
	        g_iNumLineMessageBoxCustom += SeparateTextIntoLines( GlobalText[index+i], g_lpszMessageBoxCustom[g_iNumLineMessageBoxCustom], NUM_LINE_CMB, MAX_LENGTH_CMB);
        }
    }
	else if ( message==MESSAGE_GEM_INTEGRATION3)
	{
		char tBuf[MAX_GLOBAL_TEXT_STRING];
		char tLines[2][30];
		for(int t = 0; t < 2; ++t) memset(tLines[t], 0, 20);
		g_iNumLineMessageBoxCustom = 0;
		if(COMGEM::isComMode())
		{
			if(COMGEM::m_cGemType == 0) sprintf(tBuf, GlobalText[1809], GlobalText[1806], COMGEM::m_cCount);
			else sprintf(tBuf, GlobalText[1809], GlobalText[1807], COMGEM::m_cCount);

			g_iNumLineMessageBoxCustom += SeparateTextIntoLines( tBuf, 
				tLines[g_iNumLineMessageBoxCustom], 2, 30);

			for(int t = 0; t < 2; ++t) strcpy(g_lpszMessageBoxCustom[t], tLines[t]);

			sprintf(g_lpszMessageBoxCustom[g_iNumLineMessageBoxCustom], GlobalText[1810], COMGEM::m_iValue);
			++g_iNumLineMessageBoxCustom;

		}
		else
		{
			int t_GemLevel = COMGEM::GetUnMixGemLevel()+1;
			if(COMGEM::m_cGemType == 0) sprintf(tBuf, GlobalText[1813], GlobalText[1806], t_GemLevel);
			else sprintf(tBuf, GlobalText[1813], GlobalText[1807], t_GemLevel);

			g_iNumLineMessageBoxCustom += SeparateTextIntoLines( tBuf, 
				tLines[g_iNumLineMessageBoxCustom], 2, 30);

			for(int t = 0; t < 2; ++t) strcpy(g_lpszMessageBoxCustom[t], tLines[t]);

			sprintf(g_lpszMessageBoxCustom[g_iNumLineMessageBoxCustom], GlobalText[1814], COMGEM::m_iValue);
			++g_iNumLineMessageBoxCustom;
		}
	}
	else if(message == MESSAGE_CANCEL_SKILL)
	{
		char tBuf[MAX_GLOBAL_TEXT_STRING];	
		sprintf(tBuf, "%s%s", SkillAttribute[index].Name, GlobalText[2046]);
		g_iNumLineMessageBoxCustom = SeparateTextIntoLines(tBuf, g_lpszMessageBoxCustom[0], 2, MAX_LENGTH_CMB);
		g_iCancelSkillTarget = index;
	}
    else
    {
        for ( int i=0; i<num; ++i )
        {
	        strcpy ( g_lpszMessageBoxCustom[i], GlobalText[index+i]);
        }
        g_iNumLineMessageBoxCustom = num;
    }

	ZeroMemory( g_iCustomMessageBoxButton, NUM_BUTTON_CMB * NUM_PAR_BUTTON_CMB * sizeof ( int) );

    int iOkButton[5]     = { 1,  21, 90, 70, 21};
    int iCancelButton[5] = { 3, 120, 90, 70, 21};

	if(message == MESSAGE_USE_STATE2)
	{
		iOkButton[1]	 = 22;	
		iOkButton[2]	 = 92;	// y
		iOkButton[3]     = 49;
		iOkButton[4]     = 16;

		iCancelButton[1] = 82;
		iCancelButton[2] = 92;	// y
		iCancelButton[3] = 49;
		iCancelButton[4] = 16;

		g_iCustomMessageBoxButton_Cancel[0] = 5;
		g_iCustomMessageBoxButton_Cancel[1] = 142;	// x
		g_iCustomMessageBoxButton_Cancel[2] = 92;	// y
		g_iCustomMessageBoxButton_Cancel[3] = 49;	// width
		g_iCustomMessageBoxButton_Cancel[4] = 16;	// height
	}

    if ( message==MESSAGE_CHAOS_CASTLE_CHECK )
    {
        iOkButton[2]     = 120;
        iCancelButton[2] = 120;
    }
	
    memcpy( g_iCustomMessageBoxButton[0], iOkButton, 5 * sizeof ( int));
    memcpy( g_iCustomMessageBoxButton[1], iCancelButton, 5 * sizeof ( int));

	return true;
}

int    CameraWalkCut;
int    CurrentCameraCount    = -1;
int    CurrentCameraWalkType = 0;
int    CurrentCameraNumber   = 0;
vec3_t CurrentCameraPosition;
vec3_t CurrentCameraAngle;
float  CurrentCameraWalkDelta[6]; 
float  CameraWalk[] = 
{
	 0.f,-1000.f,500.f,-80.f,0.f, 0.f,
	 0.f,-1100.f,500.f,-80.f,0.f, 0.f,
	 0.f,-1100.f,500.f,-80.f,0.f, 0.f,
     0.f,-1100.f,500.f,-80.f,0.f, 0.f,
	 0.f,-1100.f,500.f,-80.f,0.f, 0.f,
	200.f,-800.f,250.f,-87.f,0.f, -10.f,
};

void MoveCharacterCamera(vec3_t Origin,vec3_t Position,vec3_t Angle)
{
	vec3_t TransformPosition;
	CameraAngle[0] = 0.f;
	CameraAngle[1] = 0.f;
	CameraAngle[2] = Angle[2];
	float Matrix[3][4];
	AngleMatrix(CameraAngle,Matrix);
	VectorIRotate(Position,Matrix,TransformPosition);
	VectorAdd(Origin,TransformPosition,CameraPosition);
	CameraAngle[0] = Angle[0];
}

void MoveCamera()
{
	if(CurrentCameraCount == -1)
	{
		for(int i=0;i<3;i++)
		{
			CurrentCameraPosition[i] = CameraWalk[i  ];
			CurrentCameraAngle   [i] = CameraWalk[i+3];
		}
		CurrentCameraNumber = 1;
		CurrentCameraWalkType = 1;

		for(int i=0;i<3;i++)
		{
			CurrentCameraWalkDelta[i  ] = (CameraWalk[CurrentCameraNumber*6+i  ]-CurrentCameraPosition[i])/128;
			CurrentCameraWalkDelta[i+3] = (CameraWalk[CurrentCameraNumber*6+i+3]-CurrentCameraAngle   [i])/128;
		}
	}
	CurrentCameraCount++;
    if((CameraWalkCut==0 && CurrentCameraCount>=40) || (CameraWalkCut>0 && CurrentCameraCount>=128))
	{
        CurrentCameraCount = 0;
		if(CameraWalkCut==0)
		{
			CameraWalkCut = 1;
		}
		else
		{
            if(SceneFlag == LOG_IN_SCENE)
			{
				CurrentCameraNumber = rand()%4+1;
				CurrentCameraWalkType = rand()%2;
			}
			else
			{
      			CurrentCameraNumber = 5;
				CurrentCameraWalkType = 0;
			}
		}
		for(int i=0;i<3;i++)
		{
			CurrentCameraWalkDelta[i  ] = (CameraWalk[CurrentCameraNumber*6+i  ]-CurrentCameraPosition[i])/128;
			CurrentCameraWalkDelta[i+3] = (CameraWalk[CurrentCameraNumber*6+i+3]-CurrentCameraAngle   [i])/128;
		}
	}
	if(CurrentCameraWalkType==0)
	{
    	for(int i=0;i<3;i++)
		{
			CurrentCameraPosition[i] += (CameraWalk[CurrentCameraNumber*6+i  ]-CurrentCameraPosition[i])/6;
			CurrentCameraAngle   [i] += (CameraWalk[CurrentCameraNumber*6+i+3]-CurrentCameraAngle   [i])/6;
		}
	}
	else
	{
    	for(int i=0;i<2;i++)
		{
			CurrentCameraPosition[i] += CurrentCameraWalkDelta[i  ];
		}
	}
    CameraFOV = 45.f;
	vec3_t Position;
	Vector(0.f,0.f,0.f,Position);
    MoveCharacterCamera(Position,CurrentCameraPosition,CurrentCameraAngle);
}

bool MenuCancel         = true;
bool EnableSocket       = false;
bool InitLogIn          = false;
bool InitLoading        = false;
bool InitCharacterScene = false;
bool InitMainScene      = false;
int  MenuY = 480;
int  MenuX = -200;
extern char LogInID[MAX_ID_SIZE+1];
extern char m_ExeVersion[11];

BOOL Util_CheckOption( char *lpszCommandLine, unsigned char cOption, char *lpszString);

extern DWORD g_dwBKConv;
extern DWORD g_dwBKSent;
extern BOOL g_bIMEBlock;

int SelectedHero = -1;
bool MoveMainCamera();

void StartGame()
{
	{
		if (CTLCODE_01BLOCKCHAR & CharactersClient[SelectedHero].CtlCode)
			CUIMng::Instance().PopUpMsgWin(MESSAGE_BLOCKED_CHARACTER);
		else
		{
			CharacterAttribute->Level = CharactersClient[SelectedHero].Level;
			CharacterAttribute->Class = CharactersClient[SelectedHero].Class;
			CharacterAttribute->Skin  = CharactersClient[SelectedHero].Skin;
			::strcpy(CharacterAttribute->Name, CharactersClient[SelectedHero].ID);

			::ReleaseCharacterSceneData();
			InitLoading = false;
			SceneFlag = LOADING_SCENE;
		}
	}
}

void CreateCharacterScene()
{
	g_pNewUIMng->ResetActiveUIObj();

	EnableMainRender = true;
	MouseOnWindow = false;
	ErrorMessage = NULL;

	gMapManager.WorldActive = WD_54CHARACTERSCENE;

	gMapManager.LoadWorld(gMapManager.WorldActive);
    OpenCharacterSceneData();

    CreateCharacterPointer(&CharacterView,MODEL_FACE+1,0,0);
	CharacterView.Class = 1;
	CharacterView.Object.Kind = 0;

	SelectedHero = -1;
	CUIMng::Instance().CreateCharacterScene();

    ClearInventory();
    CharacterAttribute->SkillNumber = 0;

	for(int i=0;i<MAX_MAGIC;i++)
		CharacterAttribute->Skill[i] = 0;

	for(int i=EQUIPMENT_WEAPON_RIGHT;i<EQUIPMENT_HELPER;i++)
		CharacterMachine->Equipment[i].Level = 0;

	g_pNewUISystem->HideAll();

	g_iKeyPadEnable = 0;
	GuildInputEnable = false;
	TabInputEnable   = false;
	GoldInputEnable  = false;
	InputEnable      = true;
    ClearInput();
	InputIndex = 0;
    InputTextWidth = 90;
    InputNumber = 1;

	for(int i=0;i<MAX_WHISPER;i++)
	{
		g_pChatListBox->AddText("", "", SEASON3B::TYPE_WHISPER_MESSAGE);
	}

	HIMC hIMC = ImmGetContext(g_hWnd);
    DWORD Conversion, Sentence;

	Conversion = IME_CMODE_NATIVE;
	Sentence = IME_SMODE_NONE;

	g_bIMEBlock = FALSE;
	RestoreIMEStatus();
	ImmSetConversionStatus(hIMC, Conversion, Sentence);
	ImmGetConversionStatus(hIMC, &g_dwBKConv, &g_dwBKSent);
	SaveIMEStatus();
	ImmReleaseContext(g_hWnd, hIMC);
	g_bIMEBlock = TRUE;

    g_ErrorReport.Write( "> Character scene init success.\r\n");
}

void NewMoveCharacterScene()
{
	if (CurrentProtocolState < RECEIVE_CHARACTERS_LIST)
	{
		return;
	}

	if (!InitCharacterScene)
	{
		InitCharacterScene = true;
		CreateCharacterScene();
	}
    InitTerrainLight();
    MoveObjects();
	MoveBugs();
    MoveCharactersClient();
    MoveCharacterClient(&CharacterView);

	MoveEffects();
    MoveJoints();
    MoveParticles();
	MoveBoids();

	ThePetProcess().UpdatePets();

    MoveCamera();

#if defined _DEBUG || defined FOR_WORK
	char lpszTemp[256];
	if (::Util_CheckOption(::GetCommandLine(), 'c', lpszTemp))
	{
		SelectedHero = ::atoi(lpszTemp);
		::StartGame();
	}
#endif

	CInput& rInput = CInput::Instance();
	CUIMng& rUIMng = CUIMng::Instance();

	if (rInput.IsKeyDown(VK_RETURN))
	{
		if (!(rUIMng.m_MsgWin.IsShow() || rUIMng.m_CharMakeWin.IsShow()
			|| rUIMng.m_SysMenuWin.IsShow() || rUIMng.m_OptionWin.IsShow())
			&& SelectedHero > -1 && SelectedHero < 5)
		{
			::PlayBuffer(SOUND_CLICK01);

			if(SelectedCharacter >= 0)
				SelectedHero = SelectedCharacter;

			::StartGame();
		}
	}
	else if (rInput.IsKeyDown(VK_ESCAPE))
	{
		if (!(rUIMng.m_MsgWin.IsShow() || rUIMng.m_CharMakeWin.IsShow()
			|| rUIMng.m_SysMenuWin.IsShow() || rUIMng.m_OptionWin.IsShow()
			)
			&& rUIMng.IsSysMenuWinShow() )
		{
			::PlayBuffer(SOUND_CLICK01);
			rUIMng.ShowWin(&rUIMng.m_SysMenuWin);
		}
	}

	if (rUIMng.IsCursorOnUI())
	{
		return;
	}

	if (rInput.IsLBtnDbl() && rUIMng.m_CharSelMainWin.IsShow())
	{
		if (SelectedCharacter < 0 || SelectedCharacter > 4)
		{
			return;
		}

		SelectedHero = SelectedCharacter;
		::StartGame();
	}
	else if(rInput.IsLBtnDn())
	{
		if (SelectedCharacter < 0 || SelectedCharacter > 4)
			SelectedHero = -1;
		else
			SelectedHero = SelectedCharacter;
		rUIMng.m_CharSelMainWin.UpdateDisplay();
	}

	g_ConsoleDebug->UpdateMainScene();
}

bool NewRenderCharacterScene(HDC hDC)
{
	if(!InitCharacterScene) 
	{
		return false;
	}

	if(CurrentProtocolState < RECEIVE_CHARACTERS_LIST) 
	{
		return false;
	}

    FogEnable = false;

    MoveMainCamera();

	int Width,Height;

	glColor3f(1.f, 1.f, 1.f);

	BeginBitmap();

	Width = 320;
	Height = 320;

	RenderBitmap(BITMAP_LOG_IN + 9, (float)0, (float)25, (float)Width, (float)Height, 0.f, 0.f);
	RenderBitmap(BITMAP_LOG_IN + 10, (float)320, (float)25, (float)Width, (float)Height, 0.f, 0.f);

	EndBitmap();

	Height = 480;
	Width = GetScreenWidth();
    
	glClearColor(0.f,0.f,0.f,1.f);

	BeginOpengl(0,25,640,430);
	
	vec3_t pos;

	Vector(12975.0f, 15423.0f, 333.0f, pos);

	CreateFrustrum((float)Width / (float)640, pos);

	OBJECT *o = &CharactersClient[SelectedHero].Object;

	CreateScreenVector(MouseX,MouseY,MouseTarget);

	for (int i = 0; i < 5; i++)
	{
		CharactersClient[i].Object.Position[2] = 163.0f;

		Vector(0.0f, 0.0f, 0.0f, CharactersClient[i].Object.Light);
	}

	if (SelectedHero != -1 && o->Live)
	{
		EnableAlphaBlend();

		vec3_t Light;

		Vector(1.0f, 1.0f, 1.0f, Light);
		Vector(1.0f, 1.0f, 1.0f, o->Light);

		AddTerrainLight(o->Position[0], o->Position[1], Light, 1, PrimaryTerrainLight);

		DisableAlphaBlend();
	}

	CHARACTER* pCha = NULL;
	OBJECT* pObj = NULL;

	for (int i = 0; i < 5; ++i)
	{
		pCha = &CharactersClient[i];
		pObj = &pCha->Object;

		if (pCha->Helper.Type == MODEL_HELPER + 3)
		{
			pObj->Position[2] = 194.5f;
		}
		else
		{
			pObj->Position[2] = 169.5f;
		}
	}

	RenderTerrain(false);

	RenderObjects();

	RenderCharactersClient();

	if (!CUIMng::Instance().IsCursorOnUI())
	{
		SelectObjects();
	}

	RenderBugs();

	RenderBlurs();

	RenderJoints();

	RenderEffects();

	ThePetProcess().RenderPets();

	RenderBoids();

	RenderObjects_AfterCharacter();

	CheckSprites();

	if (SelectedHero != -1 && o->Live)
	{
		vec3_t vLight;

		Vector(1.0f, 1.0f, 1.f, vLight);

		float fLumi = sinf(WorldTime * 0.0015f) * 0.3f + 0.5f;

		Vector(fLumi * vLight[0], fLumi * vLight[1], fLumi * vLight[2], vLight);

		EnableAlphaBlend();

		/*RenderTerrainAlphaBitmap(BITMAP_GM_AURORA, o->Position[0], o->Position[1], 1.8f, 1.8f, vLight, WorldTime * 0.01f);
		RenderTerrainAlphaBitmap(BITMAP_GM_AURORA, o->Position[0], o->Position[1], 1.2f, 1.2f, vLight, -WorldTime * 0.01f);*/

		DisableAlphaBlend();

		float Rotation = (int)WorldTime % 3600 / (float)10.f;

		Vector(0.15f, 0.15f, 0.15f, o->Light);

		CreateParticle(BITMAP_EFFECT, o->Position, o->Angle, o->Light, 4);
		CreateParticle(BITMAP_EFFECT, o->Position, o->Angle, o->Light, 5);

		g_csMapServer.SetHeroID((char*)CharactersClient[SelectedHero].ID);
	}

	BeginSprite();

	RenderSprites();

	RenderParticles();

	RenderPoints();

	EndSprite();

	BeginBitmap();

	RenderInfomation();

	EndBitmap();

	EndOpengl();

	return true;
}

void CreateLogInScene()
{
	EnableMainRender = true;

	gMapManager.WorldActive = WD_55LOGINSCENE;

	gMapManager.LoadWorld(gMapManager.WorldActive);

	OpenLogoSceneData();

	CUIMng::Instance().CreateLoginScene();

	CurrentProtocolState = REQUEST_JOIN_SERVER;
    CreateSocket(szServerIpAddress,g_ServerPort);
    EnableSocket = true;

	GuildInputEnable = false;
	TabInputEnable   = false;
	GoldInputEnable  = false;
	InputEnable      = true;

	ClearInput();

	CreateHero(2, 4, 0, 0, 0, 0);

	CharactersClient[2].Object.Scale = 1.6f;
	CharactersClient[2].Object.Angle[0] = 0.0f;
	CharactersClient[2].Object.Angle[1] = 0.0f;
	CharactersClient[2].Object.Angle[2] = 188.0f;
	CharactersClient[2].Object.Position[0] = 10603.0f;
	CharactersClient[2].Object.Position[1] = 14112.0f;
	CharactersClient[2].Object.Position[2] = 580.0f;

	OBJECT* pHeroObject = &CharactersClient[2].Object;

	CreateBug(MODEL_DARK_HORSE, pHeroObject->Position, pHeroObject);
	CreatePetDarkSpirit_Now(CharactersClient);

	if (g_iChatInputType == 0)
	{
		strcpy(InputText[0],m_ID);
		InputLength[0] = strlen(InputText[0]);
		InputTextMax[0] = MAX_ID_SIZE;
		if(InputLength[0] == 0)	InputIndex = 0;
		else InputIndex = 1;
	}

	InputNumber = 2;
    InputTextHide[1] = 1;
	
	MoveMainCamera();

	g_fMULogoAlpha = 0;
	
	::PlayMp3(g_lpszMp3[MUSIC_LOGIN_THEME]);

	g_ErrorReport.Write( "> Login Scene init success.\r\n");
}

void NewMoveLogInScene()
{
	if(!InitLogIn)
	{
		InitLogIn = true;
		CreateLogInScene();
	}

	OBJECT* pHeroObject = &CharactersClient[2].Object;

	int restartAnimation = (int)(((int)WorldTime % 2300) * 0.0099999998f);

	BYTE random;

	if (!restartAnimation)
	{
		random = (BYTE)(rand() % 6);

		if (random == 1)
		{
			SetAction(pHeroObject, 88, true);
		}
		else if (random == 2)
		{
			SetAction(pHeroObject, 89, true);
		}
	}

	if (!CUIMng::Instance().m_CreditWin.IsShow())
	{
		InitTerrainLight();

		MoveObjects();

		MoveBugs();

		MoveLeaves();

		MoveCharactersClient();

		MoveEffects();

		MoveJoints();

		MoveParticles();

		MoveBoids();

		ThePetProcess().UpdatePets();

		MoveCamera();
	}

	if (CInput::Instance().IsKeyDown(VK_ESCAPE))
	{
		CUIMng& rUIMng = CUIMng::Instance();

		if (!(rUIMng.m_MsgWin.IsShow() || rUIMng.m_LoginWin.IsShow() || rUIMng.m_SysMenuWin.IsShow() || rUIMng.m_OptionWin.IsShow()	|| rUIMng.m_CreditWin.IsShow())
			&& rUIMng.m_LoginMainWin.IsShow() && rUIMng.m_ServerSelWin.IsShow()
			&& rUIMng.IsSysMenuWinShow())
		{
			::PlayBuffer(SOUND_CLICK01);
			rUIMng.ShowWin(&rUIMng.m_SysMenuWin);
		}
	}

	if (RECEIVE_LOG_IN_SUCCESS == CurrentProtocolState)
	{
		g_ErrorReport.Write( "> Request Character list\r\n");

		SceneFlag = CHARACTER_SCENE;

		SendRequestCharactersList(g_pMultiLanguage->GetLanguage());

        ReleaseLogoSceneData();

		ClearCharacters();
	}

	g_ConsoleDebug->UpdateMainScene();
}

bool NewRenderLogInScene(HDC hDC)
{
	if(!InitLogIn) return false;

	FogEnable = false;

    MoveMainCamera();

	int Width,Height;

	glColor3f(1.f,1.f,1.f);

	BeginBitmap();

	Width = 320;	
	Height = 320;
	
	RenderBitmap(BITMAP_LOG_IN + 9, (float)0, (float)25, (float)Width, (float)Height, 0.f, 0.f);
	RenderBitmap(BITMAP_LOG_IN + 10, (float)320, (float)25, (float)Width, (float)Height, 0.f, 0.f);
	
	EndBitmap();

	Height = 480;
    Width = GetScreenWidth();

	glClearColor(0.f,0.f,0.f,1.f);

	BeginOpengl(0,25,640,430);

	float Pos[3] = { 11679.0f, 14191.0f, 333.0f };

	CreateFrustrum((float)Width / (float)640, Pos);

	if (!CUIMng::Instance().m_CreditWin.IsShow())
	{
		RenderTerrain(false);

		CameraViewFar = 7000.f;

		RenderCharactersClient();

		RenderBugs();

		RenderObjects();

		RenderJoints();

		RenderEffects();

		CheckSprites();

		RenderLeaves();

		RenderBoids();

		RenderObjects_AfterCharacter();

		ThePetProcess().RenderPets();
	}

	BeginSprite();
	RenderSprites();
	RenderParticles();
	EndSprite();
	BeginBitmap();

	SIZE Size;
	char Text[100];
	
	g_pRenderText->SetFont(g_hFont);

	InputTextWidth = 256;
	glColor3f(0.8f,0.7f,0.6f);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 128);
	
	strcpy(Text,GlobalText[454]);
	g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), Text,lstrlen(Text),&Size);
	g_pRenderText->RenderText(335-Size.cx*640/WindowWidth,480-Size.cy*640/WindowWidth-1,Text);

	strcpy(Text,GlobalText[455]);
	
	g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), Text,lstrlen(Text),&Size);
	g_pRenderText->RenderText(335,480-Size.cy*640/WindowWidth-1,Text);

	sprintf(Text,GlobalText[456],m_ExeVersion);
	
	g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), Text,lstrlen(Text),&Size);
	g_pRenderText->RenderText(0, 480-Size.cy*640/WindowWidth-1,Text);

    RenderInfomation();

	EndBitmap();

	EndOpengl();

	return true;
}

void LoadingScene(HDC hDC)
{
	g_ConsoleDebug->Write(MCD_NORMAL, "LoadingScene_Start");

	CUIMng& rUIMng = CUIMng::Instance();

	if (!InitLoading)
	{
		LoadingWorld = 9999999;

		InitLoading = true;
		
		LoadBitmap("Interface\\LSBg01.JPG", BITMAP_TITLE, GL_LINEAR);
		LoadBitmap("Interface\\LSBg02.JPG", BITMAP_TITLE+1, GL_LINEAR);
		LoadBitmap("Interface\\LSBg03.JPG", BITMAP_TITLE+2, GL_LINEAR);
		LoadBitmap("Interface\\LSBg04.JPG", BITMAP_TITLE+3, GL_LINEAR);

		::StopMp3(g_lpszMp3[MUSIC_LOGIN_THEME]);

		rUIMng.m_pLoadingScene = new CLoadingScene;
		rUIMng.m_pLoadingScene->Create();
	}

    FogEnable = false;
	::BeginOpengl();
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	::BeginBitmap();

	rUIMng.m_pLoadingScene->Render();

	::EndBitmap();
	::EndOpengl();
	::glFlush();
	::SwapBuffers(hDC);

	SAFE_DELETE(rUIMng.m_pLoadingScene);

	SceneFlag = MAIN_SCENE;
	for (int i = 0; i < 4; ++i)
		::DeleteBitmap(BITMAP_TITLE+i);

	::ClearInput();

	g_ConsoleDebug->Write(MCD_NORMAL, "LoadingScene_End");
}

float CameraDistanceTarget = 1000.f;
float CameraDistance = CameraDistanceTarget;

bool MoveMainCamera()
{
    bool bLockCamera = false;

	CameraFOV = 35.f;

	CameraAngle[0] = 0.f;
	CameraAngle[1] = 0.f;

	if(CameraTopViewEnable)
	{
		CameraViewFar = 3200.f;
		CameraPosition[0] = Hero->Object.Position[0];
		CameraPosition[1] = Hero->Object.Position[1];
		CameraPosition[2] = CameraViewFar;
	}
	else
	{
		int iIndex = TERRAIN_INDEX((Hero->PositionX),(Hero->PositionY));
		vec3_t Position,TransformPosition;
		float Matrix[3][4];

        if ( battleCastle::InBattleCastle2( Hero->Object.Position ) )
        {
            CameraViewFar = 3000.f;
        }
        else if ( gMapManager.InBattleCastle() && SceneFlag == MAIN_SCENE)
        {
            CameraViewFar = 2500.f;
        }
		else if (gMapManager.WorldActive == WD_51HOME_6TH_CHAR)
		{
			CameraViewFar = 2800.f * 1.15f;
		}
 		else if(gMapManager.IsPKField()	|| IsDoppelGanger2())
		{
			CameraViewFar = 3700.0f;
 		}
        else
        {
            switch ( g_shCameraLevel )
            {
            case 0:
				if(SceneFlag == LOG_IN_SCENE)
				{
					CameraViewFar = 20400.0f;
				}
				else if(SceneFlag == CHARACTER_SCENE)
				{
					CameraViewFar = 11519.9f;
				}
				else if (g_Direction.m_CKanturu.IsMayaScene())
				{
					CameraViewFar = 2000.f * 10.0f * 0.115f;
				}
				else
				{
					CameraViewFar = 2000.f; 
				}
				break;
            case 1: CameraViewFar = 2500.f; break;
            case 2: CameraViewFar = 2600.f; break;
            case 3: CameraViewFar = 2950.f; break;
			case 5:
            case 4: CameraViewFar = 3200.f; break;
            }
        }

		Vector(0.f,-CameraDistance,0.f,Position);//-750
		AngleMatrix(CameraAngle,Matrix);
		VectorIRotate(Position,Matrix,TransformPosition);

		if(SceneFlag == MAIN_SCENE)
		{
			g_pCatapultWindow->GetCameraPos(Position);
		}

		if(g_Direction.IsDirection() && !g_Direction.m_bDownHero)
		{
			Hero->Object.Position[2] = 300.0f;
			g_shCameraLevel = g_Direction.GetCameraPosition(Position);
		}
 		else if(gMapManager.IsPKField()	|| IsDoppelGanger2())
		{
 			g_shCameraLevel =5;
 		}
		else if (IsDoppelGanger1())
		{
 			g_shCameraLevel =5;
		}
		else g_shCameraLevel =0;

		VectorAdd ( Position,TransformPosition,CameraPosition);

		if (gMapManager.InBattleCastle() == true)
		{
			CameraPosition[2] = 255.f;//700
		}
		else
		{
			CameraPosition[2] = Hero->Object.Position[2];//700
		}
		
		if ((TerrainWall[iIndex] & TW_HEIGHT) == TW_HEIGHT)
		{
			CameraPosition[2] = g_fSpecialHeight = 1200.f + 1;
		}

		CameraPosition[2] += CameraDistance-150.f;

		if (SceneFlag == LOG_IN_SCENE)
		{
			CameraAngle[0] = -96.5f;
			CameraAngle[1] = 0.0f;
			CameraAngle[2] = -36.0f;
			CameraPosition[0] = 12778.0f;
			CameraPosition[1] = 12678.0f;
			CameraPosition[2] = 579.0f;
		}
		else if(SceneFlag == CHARACTER_SCENE)
		{
			CameraAngle[0] = -84.5f;
			CameraAngle[1] = 0.0f;
			CameraAngle[2] = -45.0f;
			CameraPosition[0] = 13909.0f;
			CameraPosition[1] = 14482.0f;
			CameraPosition[2] = 543.0f;
		}
		else
		{
			CameraAngle[0] = -48.5f;
		}

		CameraAngle[0] += EarthQuake;

		if ((TerrainWall[iIndex] & TW_CAMERA_UP) == TW_CAMERA_UP)
		{
			if (g_fCameraCustomDistance <= CUSTOM_CAMERA_DISTANCE1)
			{
				g_fCameraCustomDistance += 10;
			}
		}
		else
		{
			if (g_fCameraCustomDistance > 0)
			{
				g_fCameraCustomDistance -= 10;
			}
		}

		if (g_fCameraCustomDistance > 0)
		{
			vec3_t angle = { 0.f, 0.f, -45.f };

			Vector(0.f, g_fCameraCustomDistance, 0.f, Position);

			AngleMatrix(angle, Matrix);

			VectorIRotate(Position, Matrix, TransformPosition);

			VectorAdd(CameraPosition, TransformPosition, CameraPosition);
		}
		else if (g_fCameraCustomDistance < 0)
		{
			vec3_t angle = { 0.f, 0.f, -45.f };

			Vector(0.f, g_fCameraCustomDistance, 0.f, Position);

			AngleMatrix(angle, Matrix);

			VectorIRotate(Position, Matrix, TransformPosition);

			VectorAdd(CameraPosition, TransformPosition, CameraPosition);
		}
	}

	if (gMapManager.WorldActive == 5)
	{
		CameraAngle[0] += sinf(WorldTime * 0.0005f) * 2.f;
		CameraAngle[1] += sinf(WorldTime * 0.0008f) * 2.5f;
	}
	else
    {
		if (gMapManager.InBattleCastle())
		{
			CameraDistanceTarget = 1100.f;
			CameraDistance = CameraDistanceTarget;
		}
		else
		{
			switch (g_shCameraLevel)
			{
			case 0: CameraDistanceTarget = 1000.f; CameraDistance += (CameraDistanceTarget - CameraDistance) / 3; break;
			case 1: CameraDistanceTarget = 1100.f; CameraDistance += (CameraDistanceTarget - CameraDistance) / 3; break;
			case 2: CameraDistanceTarget = 1200.f; CameraDistance += (CameraDistanceTarget - CameraDistance) / 3; break;
			case 3: CameraDistanceTarget = 1300.f; CameraDistance += (CameraDistanceTarget - CameraDistance) / 3; break;
			case 4: CameraDistanceTarget = 1400.f; CameraDistance += (CameraDistanceTarget - CameraDistance) / 3; break;
			case 5: CameraDistanceTarget = g_Direction.m_fCameraViewFar; CameraDistance += (CameraDistanceTarget - CameraDistance) / 3; break;
			}
		}
    }

    return bLockCamera;
}

void MoveMainScene()
{
	if(!InitMainScene)
	{
		g_pMainFrame->ResetSkillHotKey();
		
		g_ConsoleDebug->Write( MCD_NORMAL, "Join the game with the following character: %s", CharactersClient[SelectedHero].ID);

		g_ErrorReport.Write( "> Character selected <%d> \"%s\"\r\n", SelectedHero+1, CharactersClient[SelectedHero].ID);

        InitMainScene = true;
		
		g_ConsoleDebug->Write( MCD_SEND, "SendRequestJoinMapServer");

	    SendRequestJoinMapServer(CharactersClient[SelectedHero].ID);

		CUIMng::Instance().CreateMainScene();

		CameraAngle[2] = -45.f;

		ClearInput();
		InputEnable = false;
		TabInputEnable = false;
		InputTextWidth = 256;
		InputTextMax[0] = 42;
		InputTextMax[1] = 10;
		InputNumber = 2;

		for(int i=0;i<MAX_WHISPER;i++)
		{
			g_pChatListBox->AddText("", "", SEASON3B::TYPE_WHISPER_MESSAGE);
		}

		g_GuildNotice[0][0] = '\0';
		g_GuildNotice[1][0] = '\0';
	
		g_pPartyManager->Create();

		g_pChatListBox->ClearAll();

		g_pSlideHelpMgr->Init();		
		g_pUIMapName->Init();

		g_GuildCache.Reset();

		g_PortalMgr.Reset();

		ClearAllObjectBlurs();
		
		SetFocus(g_hWnd);

		g_ErrorReport.Write( "> Main Scene init success. ");
		g_ErrorReport.WriteCurrentTime();

		g_ConsoleDebug->Write(MCD_NORMAL, "MainScene Init Success");
	}
	
	if(CurrentProtocolState == RECEIVE_JOIN_MAP_SERVER)
	{
		EnableMainRender = true;
	}
	if(EnableMainRender == false)
	{
		return;
	}
	//init
	EarthQuake *= 0.2f;

	InitTerrainLight();

	CheckInventory = NULL;
	CheckSkill = -1;
	MouseOnWindow = false;

	if(!CameraTopViewEnable	&& LoadingWorld < 30 )
	{
		if(MouseY>=(int)(480-48))
			MouseOnWindow = true;

		g_pPartyManager->Update();
		g_pNewUISystem->Update();
		
		if (MouseLButton == true && false == g_pNewUISystem->CheckMouseUse() && g_dwMouseUseUIID == 0 && g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHATINPUTBOX) == false )
		{
			g_pWindowMgr->SetWindowsEnable(FALSE);
			g_pFriendMenu->HideMenu();
			g_dwKeyFocusUIID = 0;
			if(GetFocus() != g_hWnd)
			{
				SaveIMEStatus();
				SetFocus(g_hWnd);
			}
		}
		MoveInterface();
		MoveTournamentInterface();
		if( ErrorMessage != MESSAGE_LOG_OUT )
			g_pUIManager->UpdateInput();
	}

	if(ErrorMessage != NULL)
		MouseOnWindow = true;

    MoveObjects();

    if(!CameraTopViewEnable)
    	MoveItems();

	if ( ( gMapManager.WorldActive==WD_0LORENCIA && HeroTile!=4 ) || 
         ( gMapManager.WorldActive==WD_2DEVIAS && HeroTile!=3 && HeroTile<10 ) 
		 || gMapManager.WorldActive==WD_3NORIA 
		 || gMapManager.WorldActive==WD_7ATLANSE 
		 || gMapManager.InDevilSquare() == true
		 || gMapManager.WorldActive==WD_10HEAVEN 
         || gMapManager.InChaosCastle()==true 
         || gMapManager.InBattleCastle()==true
		 || M31HuntingGround::IsInHuntingGround()==true
		 || M33Aida::IsInAida()==true
		 || M34CryWolf1st::IsCyrWolf1st()==true
		|| gMapManager.WorldActive == WD_42CHANGEUP3RD_2ND
		|| IsIceCity()
		|| IsSantaTown()
		|| gMapManager.IsPKField()
		|| IsDoppelGanger2()
		|| gMapManager.IsEmpireGuardian1() 
		|| gMapManager.IsEmpireGuardian2()
		|| gMapManager.IsEmpireGuardian3()
		|| gMapManager.IsEmpireGuardian4()
		|| IsUnitedMarketPlace()
	 )
	{
        MoveLeaves();
	}
	
	MoveBoids();
   	MoveFishs();
	MoveBugs();
	MoveChat();
	UpdatePersonalShopTitleImp();
	MoveHero();
    MoveCharactersClient();
	ThePetProcess().UpdatePets();
    MovePoints();
	MovePlanes();
	MoveEffects();
    MoveJoints();
    MoveParticles();
    MovePointers();

	g_Direction.CheckDirection();

	g_GameCensorship->Update();

	g_ConsoleDebug->UpdateMainScene();
}

bool RenderMainScene()
{
	if(EnableMainRender == false)  
	{
		return false;
	}

    if(( LoadingWorld) > 30)
	{
		return false;
	}

    FogEnable = false;

    vec3_t pos;

    if(MoveMainCamera() == true)
    {
        VectorCopy ( Hero->Object.StartPosition, pos );
    }
    else
    {
		g_pCatapultWindow->GetCameraPos(pos);
		
		if(g_Direction.IsDirection() && g_Direction.m_bDownHero == false)
		{
			g_Direction.GetCameraPosition(pos);
		}
    }

	int Width,Height;

    BYTE byWaterMap = 0;

	if(CameraTopViewEnable == false)
	{
		Height = 480-48;
	}
	else
	{
		Height = 480;
	}

    Width = GetScreenWidth();
    if(gMapManager.WorldActive == WD_0LORENCIA)      
	{
		glClearColor(10/256.f,20/256.f,14/256.f,1.f);
	}
    else if(gMapManager.WorldActive == WD_2DEVIAS)
	{
		glClearColor(0.f/256.f,0.f/256.f,10.f/256.f,1.f);
	}
    else if(gMapManager.WorldActive == WD_10HEAVEN)
	{
		glClearColor(3.f/256.f,25.f/256.f,44.f/256.f,1.f);
	}
    else if(gMapManager.InChaosCastle() == true)
	{
		glClearColor(0/256.f,0/256.f,0/256.f,1.f);
	}
	else if(gMapManager.WorldActive >= WD_45CURSEDTEMPLE_LV1 && gMapManager.WorldActive <= WD_45CURSEDTEMPLE_LV6) 
	{
		glClearColor(9.f/256.f,8.f/256.f,33.f/256.f,1.f);
	}
    else if(gMapManager.InHellas() == true)
    {
        byWaterMap = 1;
        glClearColor(0.f/256.f,0.f/256.f,0.f/256.f,1.f);
    }
    else    
	{
		glClearColor(0/256.f,0/256.f,0/256.f,1.f);
	}

	BeginOpengl(0,0,Width,Height);

	CreateFrustrum((float)Width/(float)640, pos);

    if ( gMapManager.InBattleCastle() )
    {
        if ( battleCastle::InBattleCastle2( Hero->Object.Position ) )
        {
            vec3_t Color = { 0.f, 0.f, 0.f };
            battleCastle::StartFog ( Color );
        }
        else
        {
            glDisable ( GL_FOG );
        }
    }

	CreateScreenVector(MouseX,MouseY,MouseTarget);

    if ( IsWaterTerrain()==false )
    {
		if(gMapManager.WorldActive==WD_39KANTURU_3RD)
		{
			if(!g_Direction.m_CKanturu.IsMayaScene())
				RenderTerrain(false);
		}
		else
        if(gMapManager.WorldActive!=WD_10HEAVEN && gMapManager.WorldActive != -1)
        {
			if(gMapManager.IsPKField() || IsDoppelGanger2())
			{
				RenderObjects();
			}
            RenderTerrain(false);
        }
    }

	if(!gMapManager.IsPKField()	&& !IsDoppelGanger2())
		RenderObjects();

	RenderEffectShadows();
   	RenderBoids(); 

	RenderCharactersClient();

	if(EditFlag!=EDIT_NONE)
	{
		RenderTerrain(true);
    }
    if(!CameraTopViewEnable)
     	RenderItems();

   	RenderFishs();
   	RenderBugs();
    RenderLeaves();

	if (!gMapManager.InChaosCastle())
		ThePetProcess().RenderPets();

	RenderBoids(true);
	RenderObjects_AfterCharacter();

    RenderJoints(byWaterMap);
	RenderEffects();
    RenderBlurs();
    CheckSprites();
    BeginSprite();

	if ((gMapManager.WorldActive == WD_2DEVIAS && HeroTile != 3 && HeroTile < 10)
		|| IsIceCity()
		|| IsSantaTown()
		|| gMapManager.IsPKField()
		|| IsDoppelGanger2()
		|| gMapManager.IsEmpireGuardian1()
		|| gMapManager.IsEmpireGuardian2()
		|| gMapManager.IsEmpireGuardian3()
		|| gMapManager.IsEmpireGuardian4()
		|| IsUnitedMarketPlace()
		)
	{
		RenderLeaves();
	}

	RenderSprites();
	RenderParticles();

    if ( IsWaterTerrain()==false )
    {
        RenderPoints ( byWaterMap );
    }

    EndSprite();

	RenderAfterEffects();

    if(IsWaterTerrain() == true)
    {
        byWaterMap = 2;

		EndOpengl();
	    BeginOpengl(0, 0, Width, Height );
        RenderWaterTerrain();
        RenderJoints(byWaterMap );
        RenderEffects( true );
        RenderBlurs();
        CheckSprites();
        BeginSprite();

        if(gMapManager.WorldActive==WD_2DEVIAS && HeroTile!=3 && HeroTile<10)
            RenderLeaves();

		RenderSprites(byWaterMap);
		RenderParticles(byWaterMap);
        RenderPoints ( byWaterMap );

        EndSprite();
		EndOpengl();

		BeginOpengl( 0, 0, Width, Height );
    }

    if(gMapManager.InBattleCastle())
    {
        if(battleCastle::InBattleCastle2(Hero->Object.Position))
        {
            battleCastle::EndFog();
        }
    }

    SelectObjects();
	BeginBitmap();	
    RenderObjectDescription();
	
	if(CameraTopViewEnable == false)
	{
        RenderInterface(true);
	}
	RenderTournamentInterface();
	EndBitmap();						
	
	g_pPartyManager->Render();
	g_pNewUISystem->Render();
	
	BeginBitmap();

	RenderInfomation();

#ifdef ENABLE_EDIT
	RenderDebugWindow();
#endif //ENABLE_EDIT

	EndBitmap();
	BeginBitmap();

    RenderCursor();

	EndBitmap();
    EndOpengl();
	
	return true;
}

int TimeRemain = 40;
extern int ChatTime;
extern int WaterTextureNumber;

int TestTime = 0;
extern int  GrabScreen;

void MoveCharacter(CHARACTER *c,OBJECT *o);

int TimePrior = GetTickCount();

void MainScene(HDC hDC)
{
   	CalcFPS();
	
	int32_t Remain = 0;

	for (Remain = TimeRemain; Remain >= 40; Remain -= 40)
	{
		g_pNewKeyInput->ScanAsyncKeyState();

		if (LOG_IN_SCENE == SceneFlag || CHARACTER_SCENE == SceneFlag)
		{
			double dDeltaTick = g_pTimer->GetTimeElapsed();
			dDeltaTick = MIN(dDeltaTick, 200.0);
			g_pTimer->ResetTimer();

			CInput::Instance().Update();
			CUIMng::Instance().Update(dDeltaTick);
		}

		g_dwMouseUseUIID = 0;

		switch (SceneFlag)
		{
		case LOG_IN_SCENE:
			NewMoveLogInScene();
			break;

		case CHARACTER_SCENE:
			NewMoveCharacterScene();
			break;

		case MAIN_SCENE:
			MoveMainScene();
			break;
		}
		
		for ( int iCount = 0; iCount < 5; ++iCount)
		{
			g_PhysicsManager.Move(0.005f);
		}

        MoveNotices();

		if(PressKey(VK_SNAPSHOT))
		{
			if(GrabEnable)
				GrabEnable = false;
			else
				GrabEnable = true;
		}
        if(ChatTime > 0) ChatTime--;
		if(MacroTime > 0) MacroTime--;
		WaterTextureNumber++;
		WaterTextureNumber%=32;
		MoveSceneFrame++;
	} 

	if (Destroy) {
		return;
	}

	Bitmaps.Manage();

	Set3DSoundPosition();

	const bool bCaptureMode = !HIBYTE(GetAsyncKeyState(VK_SHIFT));

	char Text[256];

	char lpszTemp[64];

	if (GrabEnable)
	{
		SYSTEMTIME st;

		GetLocalTime(&st);

		sprintf(GrabFileName, "ScreenShots\\Screen(%02d_%02d-%02d_%02d)-%04d.jpg", st.wMonth, st.wDay, st.wHour, st.wMinute, GrabScreen);

		sprintf(Text, GlobalText[459], GrabFileName);

		wsprintf(lpszTemp, " [%s / %s]", g_ServerListManager->GetSelectServerName(), Hero->ID);

		strcat(Text, lpszTemp);

		if (GrabEnable && bCaptureMode)
		{
			g_pChatListBox->AddText("", Text, SEASON3B::TYPE_SYSTEM_MESSAGE);
		}
	}

    if( gMapManager.WorldActive==WD_10HEAVEN )
    {
        glClearColor(3.f/256.f,25.f/256.f,44.f/256.f,1.f);
    }
#ifdef PJH_NEW_SERVER_SELECT_MAP
	else if (gMapManager.WorldActive == WD_73NEW_LOGIN_SCENE|| gMapManager.WorldActive == WD_74NEW_CHARACTER_SCENE)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    }
#endif //PJH_NEW_SERVER_SELECT_MAP
    else if (gMapManager.InHellas())
    {
        glClearColor(30.f/256.f,40.f/256.f,40.f/256.f,1.f);
    }
    else if ( gMapManager.InChaosCastle()==true )
    {
        glClearColor ( 0.f, 0.f, 0.f, 1.f );
    }
    else if ( gMapManager.InBattleCastle() && battleCastle::InBattleCastle2( Hero->Object.Position ) )
    {
        glClearColor ( 0.f, 0.f, 0.f, 1.f );
    }
	else if ( gMapManager.WorldActive >= WD_45CURSEDTEMPLE_LV1 && gMapManager.WorldActive <= WD_45CURSEDTEMPLE_LV6) 
	{
		glClearColor(9.f/256.f,8.f/256.f,33.f/256.f,1.f);
	}
	else if ( gMapManager.WorldActive == WD_51HOME_6TH_CHAR 
#ifndef PJH_NEW_SERVER_SELECT_MAP
		|| World == WD_77NEW_LOGIN_SCENE
#endif //PJH_NEW_SERVER_SELECT_MAP
		)
	{
		glClearColor(178.f/256.f,178.f/256.f,178.f/256.f,1.f);
	}
#ifndef PJH_NEW_SERVER_SELECT_MAP
	else if(World == WD_78NEW_CHARACTER_SCENE)
	{
		glClearColor(0.f,0.f,0.f,1.f);
	}
#endif //PJH_NEW_SERVER_SELECT_MAP
	else if(gMapManager.WorldActive == WD_65DOPPLEGANGER1)
	{
		glClearColor(148.f/256.f,179.f/256.f,223.f/256.f,1.f);
	}
    else
    {
        glClearColor(0/256.f,0/256.f,0/256.f,1.f);
    }
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	int32_t DifTimer = 0;
	uint32_t LastTimeCurrent = TimePrior;
	TimePrior = GetTickCount();

	bool Success = false;

	if(SceneFlag == LOG_IN_SCENE)
	{
		Success = NewRenderLogInScene(hDC);
	}
	else if(SceneFlag == CHARACTER_SCENE)
	{
		Success = NewRenderCharacterScene(hDC);
	}
	else if(SceneFlag == MAIN_SCENE)
	{
		Success = RenderMainScene();
	}

	g_PhysicsManager.Render();

	if(GrabEnable)
	{
		SaveScreen();
	}

	if (GrabEnable && !bCaptureMode)
	{
		g_pChatListBox->AddText("", Text, SEASON3B::TYPE_SYSTEM_MESSAGE);
	}

	GrabEnable = false;

	if(Success)
	{
		glFlush();
		SwapBuffers(hDC);
	}

	DifTimer = TimePrior - LastTimeCurrent;

	if (DifTimer < 40)
	{
		int32_t dwMilliseconds = 40 - DifTimer;
		std::this_thread::sleep_for(std::chrono::milliseconds(dwMilliseconds)); 
		TimePrior += dwMilliseconds;
		DifTimer = 40;
	}

	DifTimer = DifTimer + Remain;

	if(EnableSocket && SceneFlag==MAIN_SCENE)
	{
		if (SocketClient.GetSocket() == INVALID_SOCKET)
		{
			static BOOL s_bClosed = FALSE;

			if (!s_bClosed)
			{
				s_bClosed = TRUE;

				g_ErrorReport.Write("> Connection closed. ");

				g_ErrorReport.WriteCurrentTime();

				SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CServerLostMsgBoxLayout));
			}
		}
	}

	if(SceneFlag == MAIN_SCENE)
	{
		switch(gMapManager.WorldActive)
		{
		case WD_0LORENCIA:
			if(HeroTile==4)
			{
				StopBuffer(SOUND_WIND01,true);
				StopBuffer(SOUND_RAIN01,true);
			}
			else
			{
				PlayBuffer(SOUND_WIND01,NULL,true);
				if(RainCurrent>0)
					PlayBuffer(SOUND_RAIN01,NULL,true);
			}
			break;
		case WD_1DUNGEON:
			PlayBuffer(SOUND_DUNGEON01,NULL,true);
			break;
		case WD_2DEVIAS:
			if(HeroTile==3 || HeroTile>=10)
				StopBuffer(SOUND_WIND01,true);
			else
				PlayBuffer(SOUND_WIND01,NULL,true);
			break;
		case WD_3NORIA:
			PlayBuffer(SOUND_WIND01,NULL,true);
			if(rand()%512==0)
				PlayBuffer(SOUND_FOREST01);
			break;
		case WD_4LOSTTOWER:
			PlayBuffer(SOUND_TOWER01,NULL,true);
			break;
		case WD_5UNKNOWN:
			//PlayBuffer(SOUND_BOSS01,NULL,true);
			break;
		case WD_7ATLANSE:
			PlayBuffer(SOUND_WATER01,NULL,true);
			break;
		case WD_8TARKAN:
			PlayBuffer(SOUND_DESERT01,NULL,true);
			break;
        case WD_10HEAVEN:
            PlayBuffer(SOUND_HEAVEN01,NULL,true);
            if( (rand()%100)==0 )
            {
//                PlayBuffer(SOUND_HEAVEN01);
            }
            else if( (rand()%10)==0 )
            {
//                PlayBuffer(SOUND_THUNDERS02);
            }
            break;
		case WD_58ICECITY_BOSS:
			PlayBuffer(SOUND_WIND01, NULL, true);
			break;
		case WD_79UNITEDMARKETPLACE:
			{
				PlayBuffer(SOUND_WIND01,NULL,true);
				PlayBuffer(SOUND_RAIN01,NULL,true);
			}
			break;
#ifdef ASG_ADD_MAP_KARUTAN
		case WD_80KARUTAN1:
			PlayBuffer(SOUND_KARUTAN_DESERT_ENV, NULL, true);
			break;
		case WD_81KARUTAN2:
			if (HeroTile == 12)
			{
				StopBuffer(SOUND_KARUTAN_DESERT_ENV, true);
				PlayBuffer(SOUND_KARUTAN_KARDAMAHAL_ENV, NULL, true);
			}
			else
			{
				StopBuffer(SOUND_KARUTAN_KARDAMAHAL_ENV, true);
				PlayBuffer(SOUND_KARUTAN_DESERT_ENV, NULL, true);
			}
			break;
#endif	// ASG_ADD_MAP_KARUTAN
		}
		if(gMapManager.WorldActive != WD_0LORENCIA && gMapManager.WorldActive != WD_2DEVIAS && gMapManager.WorldActive != WD_3NORIA && gMapManager.WorldActive != WD_58ICECITY_BOSS	&& gMapManager.WorldActive != WD_79UNITEDMARKETPLACE)
		{
			StopBuffer(SOUND_WIND01,true);
		}
		if ( gMapManager.WorldActive != WD_0LORENCIA && gMapManager.InDevilSquare() == false && gMapManager.WorldActive != WD_79UNITEDMARKETPLACE)
		{
			StopBuffer(SOUND_RAIN01,true);
		}
		if(gMapManager.WorldActive != WD_1DUNGEON)
		{
			StopBuffer(SOUND_DUNGEON01,true);
		}
		if(gMapManager.WorldActive != WD_3NORIA)
		{
			StopBuffer(SOUND_FOREST01,true);
		}
		if(gMapManager.WorldActive != WD_4LOSTTOWER)
		{
			StopBuffer(SOUND_TOWER01,true);
		}
		if(gMapManager.WorldActive != WD_7ATLANSE)
		{
			StopBuffer(SOUND_WATER01,true);
		}
		if(gMapManager.WorldActive != WD_8TARKAN)
		{
			StopBuffer(SOUND_DESERT01,true);
		}
        if(gMapManager.WorldActive != WD_10HEAVEN)
		{
            StopBuffer(SOUND_HEAVEN01,true);
		}
		if(gMapManager.WorldActive != WD_51HOME_6TH_CHAR)
		{
			StopBuffer(SOUND_ELBELAND_VILLAGEPROTECTION01, true);
			StopBuffer(SOUND_ELBELAND_WATERFALLSMALL01, true);
			StopBuffer(SOUND_ELBELAND_WATERWAY01, true);
			StopBuffer(SOUND_ELBELAND_ENTERDEVIAS01, true);
			StopBuffer(SOUND_ELBELAND_WATERSMALL01, true);
			StopBuffer(SOUND_ELBELAND_RAVINE01, true);
			StopBuffer(SOUND_ELBELAND_ENTERATLANCE01, true);
		}
#ifdef ASG_ADD_MAP_KARUTAN
		if (!IsKarutanMap())
			StopBuffer(SOUND_KARUTAN_DESERT_ENV, true);
		if (World != WD_80KARUTAN1)
			StopBuffer(SOUND_KARUTAN_INSECT_ENV, true);
		if (World != WD_81KARUTAN2)
			StopBuffer(SOUND_KARUTAN_KARDAMAHAL_ENV, true);
#endif	// ASG_ADD_MAP_KARUTAN

		if(gMapManager.WorldActive==WD_0LORENCIA)
		{
			if(Hero->SafeZone)
			{
				if(HeroTile==4)
					PlayMp3( g_lpszMp3[MUSIC_PUB]);
				else
					PlayMp3( g_lpszMp3[MUSIC_MAIN_THEME]);
			}
		}
		else
		{	
			StopMp3( g_lpszMp3[MUSIC_PUB]);
			StopMp3( g_lpszMp3[MUSIC_MAIN_THEME]);
		}
		if(gMapManager.WorldActive==WD_2DEVIAS)	
		{
			if(Hero->SafeZone)
			{
				if(( Hero->PositionX)>=205 && ( Hero->PositionX)<=214 &&
					( Hero->PositionY)>=13 && ( Hero->PositionY)<=31)
                {
					PlayMp3( g_lpszMp3[MUSIC_CHURCH]);
                }
				else
                {
                    PlayMp3( g_lpszMp3[MUSIC_DEVIAS]);
                }
			}
		}
		else
		{
			StopMp3( g_lpszMp3[MUSIC_CHURCH]);
			StopMp3( g_lpszMp3[MUSIC_DEVIAS]);
		}
		if(gMapManager.WorldActive==WD_3NORIA)
		{
			if(Hero->SafeZone)
				PlayMp3( g_lpszMp3[MUSIC_NORIA]);
		}
		else
		{
			StopMp3( g_lpszMp3[MUSIC_NORIA]);
		}
		if(gMapManager.WorldActive==WD_1DUNGEON || gMapManager.WorldActive==WD_5UNKNOWN)
		{
			PlayMp3( g_lpszMp3[MUSIC_DUNGEON]);
		}
		else
		{
			StopMp3( g_lpszMp3[MUSIC_DUNGEON]);
		}

		if(gMapManager.WorldActive==WD_7ATLANSE) {
			PlayMp3(g_lpszMp3[MUSIC_ATLANS]);
		}
		else {
			StopMp3(g_lpszMp3[MUSIC_ATLANS]);
		}
		if(gMapManager.WorldActive==WD_10HEAVEN) {
			PlayMp3(g_lpszMp3[MUSIC_ICARUS]);
		}
		else {
			StopMp3(g_lpszMp3[MUSIC_ICARUS]);
		}
		if(gMapManager.WorldActive==WD_8TARKAN) {
			PlayMp3(g_lpszMp3[MUSIC_TARKAN]);
		}
		else {
			StopMp3(g_lpszMp3[MUSIC_TARKAN]);
		}
		if(gMapManager.WorldActive==WD_4LOSTTOWER) {
			PlayMp3(g_lpszMp3[MUSIC_LOSTTOWER_A]);
		}
		else {
			StopMp3(g_lpszMp3[MUSIC_LOSTTOWER_A]);
		}

		if(gMapManager.InHellas()) {
			PlayMp3(g_lpszMp3[MUSIC_KALIMA]);
		}
		else {
			StopMp3(g_lpszMp3[MUSIC_KALIMA]);
		}

		if(gMapManager.WorldActive==WD_31HUNTING_GROUND) {
			PlayMp3(g_lpszMp3[MUSIC_BC_HUNTINGGROUND]);
		}
		else {
			StopMp3(g_lpszMp3[MUSIC_BC_HUNTINGGROUND]);
		}

		if(gMapManager.WorldActive==WD_33AIDA) {
			PlayMp3(g_lpszMp3[MUSIC_BC_ADIA]);
		}
		else {
			StopMp3(g_lpszMp3[MUSIC_BC_ADIA]);
		}

		M34CryWolf1st::ChangeBackGroundMusic(gMapManager.WorldActive);
		M39Kanturu3rd::ChangeBackGroundMusic(gMapManager.WorldActive);

		if (gMapManager.WorldActive == WD_37KANTURU_1ST)
			PlayMp3(g_lpszMp3[MUSIC_KANTURU_1ST]);
		else
			StopMp3(g_lpszMp3[MUSIC_KANTURU_1ST]);
		M38Kanturu2nd::PlayBGM();
		SEASON3A::CGM3rdChangeUp::Instance().PlayBGM();
		if( gMapManager.IsCursedTemple() )
		{
			g_CursedTemple->PlayBGM();
		}
		if(gMapManager.WorldActive==WD_51HOME_6TH_CHAR) {
			PlayMp3(g_lpszMp3[MUSIC_ELBELAND]);
		}
		else {
			StopMp3(g_lpszMp3[MUSIC_ELBELAND]);
		}

		if(gMapManager.WorldActive==WD_56MAP_SWAMP_OF_QUIET) {
			PlayMp3(g_lpszMp3[MUSIC_SWAMP_OF_QUIET]);
		}
		else {
			StopMp3(g_lpszMp3[MUSIC_SWAMP_OF_QUIET]);
		}


		g_Raklion.PlayBGM();
		g_SantaTown.PlayBGM();
		g_PKField.PlayBGM();
		g_DoppelGanger1.PlayBGM();
		g_EmpireGuardian1.PlayBGM();
		g_EmpireGuardian2.PlayBGM();
		g_EmpireGuardian3.PlayBGM();
		g_EmpireGuardian4.PlayBGM();
		g_UnitedMarketPlace.PlayBGM();
#ifdef ASG_ADD_MAP_KARUTAN
		g_Karutan1.PlayBGM();
#endif	// ASG_ADD_MAP_KARUTAN
	}
	TimeRemain = DifTimer;
}

float g_Luminosity;

extern int g_iNoMouseTime;
extern GLvoid KillGLWindow(GLvoid);

void Scene(HDC hDC)
{
	g_Luminosity = sinf(WorldTime * 0.004f) * 0.15f + 0.6f;

	switch (SceneFlag)
	{
	case WEBZEN_SCENE:
		WebzenScene(hDC);
		break;
	case LOADING_SCENE:
		LoadingScene(hDC);
		break;
	case LOG_IN_SCENE:
	case CHARACTER_SCENE:
	case MAIN_SCENE:
		MainScene(hDC);
		break;
	}

	if (g_iNoMouseTime > 31)
	{
		KillGLWindow();
	}
}

bool GetTimeCheck(int DelayTime)
{
	int PresentTime = timeGetTime();
	
	if(g_bTimeCheck)
	{
		g_iBackupTime = PresentTime;
		g_bTimeCheck = false;
	}

	if(g_iBackupTime+DelayTime <= PresentTime)
	{
		g_bTimeCheck = true;
		return true;
	}
	return false;	
}