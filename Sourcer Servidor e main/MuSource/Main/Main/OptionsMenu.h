#pragma once

#define MAX_RESOLUTIONS 9
#define MAX_FONTS 9

class COptionsMenu
{
public:
    COptionsMenu();
    ~COptionsMenu();

    void Init();

    static bool DrawOptionsMenuUI(DWORD This);
    static bool HandleOptionsMenuControl(LPVOID This);
	static void glViewport2(int x,int y,int Width,int Height);
	static void gluPerspective2(float a1, float a2, float a3, float a4);

    void DrawOptionsMenuMain();
    void DrawOptionsMenuRender();
    void DrawOptionsMenuGeneral();
    void DrawOptionsMenuResolution();
	void DrawOptionsMenuFont();

	void DrawFloorDeletionOption();
	void DrawMapObjectDeletionOption();
	void DrawObjectsDeletionOption();
	void DrawShadowDeletionOption();
	void DrawEffectDeletionOption();
	void DrawSkillDeletionOption();
	void DrawMonsterHealthBarOption();
	void DrawAutoAttackBarOption();
    void DrawAutomaticAttackOption();
	void DrawSoundWhisperOption();
	void DrawVolumeOption();
	void DrawSlideHelpOption();
	void DrawGlowOption();
	void DrawOptionsMenuClose();
	bool IsDeleteFloorEnabled();
    bool IsDeleteMapObjectsEnabled();
    bool IsDeleteObjectsEnabled();
    bool IsDeleteShadowsEnabled();
    bool IsDeleteEffectsEnabled();
    bool IsDeleteSkillsEnabled();
	bool IsMonsterHealthBarEnabled();
	bool IsAutoAttackBarEnabled();
    bool IsAutomaticAttackEnabled();
    bool IsSoundWhisperEnabled();
    bool IsVolumeEnabled();
    bool IsSlideHelpEnabled();
    bool IsGlowEnabled();
    bool IsResolutionsEnabled();
    bool IsFontSizeEnabled();
    bool IsCloseEnabled();
	void ApplyFullScreenSettings();
public:
    float MainWidth;
    float MainHeight;
    float MainPosX;
    float MainPosY;
    int SectionWidth;
    float SectionPosY;
    float AntilagPosX;
    float GeneralPosX;
    float ResolutionPosX;
    float FontPosX;
    int DeleteFloor;
    int DeleteMapObjects;
    int DeleteObjects;
    int DeleteShadows;
    int DeleteEffects;
    int DeleteSkills;
	int DeleteAttackBar;
    int Resolutions[MAX_RESOLUTIONS];
    char ResolutionsText[MAX_RESOLUTIONS][10];
    int Fonts[MAX_FONTS];
};

extern COptionsMenu gOptionsMenu;