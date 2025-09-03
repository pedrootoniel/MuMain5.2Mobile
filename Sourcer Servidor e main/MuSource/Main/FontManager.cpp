#include "stdafx.h"
#include "FontManager.h"

CFontManager gFontManager;

CFontManager::CFontManager()
{
	ZeroMemory(this->m_Data, sizeof(this->m_Data));
}

CFontManager::~CFontManager()
{
	/* */
}

void CFontManager::Init()
{
	this->SetFont(FontType::Normal, 0, 0, 160, "Tahoma");
	this->SetFont(FontType::Bold, 1, 0, 160, "Tahoma");
	this->SetFont(FontType::Big, 0, 0, 160, "Tahoma");
	this->SetFont(FontType::Other, 0, 0, 160, "Tahoma");
}

void CFontManager::SetFont(int Index, int Bold, int AddSize, int CharSet, std::string Name)
{
	this->m_Data[Index].Bold = Bold;
	this->m_Data[Index].AddSize = AddSize;
	this->m_Data[Index].CharSet = CharSet;
	this->m_Data[Index].Name = Name;
}

