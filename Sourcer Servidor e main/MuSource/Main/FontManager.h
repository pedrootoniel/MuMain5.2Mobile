#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

namespace FontType 
{
	enum Index 
	{
		Normal,
		Bold,
		Big,
		Other,
	};
}

struct FONT_DATA
{
	int Bold;
	int AddSize;
	int CharSet;
	std::string Name;
};

class CFontManager 
{
public:
	CFontManager();
	~CFontManager();

	void Init();
	void SetFont(int Index, int Bold, int AddSize, int CharSet, std::string Name);

public:
	FONT_DATA m_Data[4];
};

extern CFontManager gFontManager;

#endif