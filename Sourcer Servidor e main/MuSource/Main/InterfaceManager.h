#ifndef INTERFACE_MANAGER_H
#define INTERFACE_MANAGER_H

class CInterfaceManager
{
public:
	void RenderButton(int ImageIndex, int PosX, int PosY, int Width, int Height, float uWidht = (1.0f), float vHeight = (1.0f), int TextIndex = 0);
	void RenderBoxGoldRect(float x, float y, float sx, float sy, int fill = 0);
};

extern CInterfaceManager gInterfaceManager;

#endif