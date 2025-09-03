#include "stdafx.h"
#include "InterfaceManager.h"
#include "UIControls.h"

CInterfaceManager gInterfaceManager;

void CInterfaceManager::RenderButton(int ImageIndex, int PosX, int PosY, int Width, int Height, float uWidht, float vHeight, int TextIndex)
{
    if (SEASON3B::CheckMouseIn(PosX, PosY, Width, Height))
    {
        glColor3f(1.0, 1.0, 1.0);
    }
    else
    {
        glColor3f(0.7, 0.7, 0.7);
    }

    RenderBitmap(ImageIndex, float(PosX), float(PosY), float(Width), float(Height), 0, 0, uWidht, vHeight, 1, 1, 0.0);

    g_pRenderText->RenderText(PosX + 15, PosY + 3, GlobalText[TextIndex], 74, 0, RT3_SORT_LEFT);

    glColor3f(1.0, 1.0, 1.0);
}

void CInterfaceManager::RenderBoxGoldRect(float x, float y, float sx, float sy, int fill)
{
	switch (fill)
	{
	case 1:
		glColor4ub(146, 144, 141, 200);
		RenderColor(x, y, sx, sy);
		EndRenderColor();
		break;
	};

	RenderBitmap(BITMAP_INVENTORY + 19, x, y, sx, 2, 10 / 256.f, 5 / 16.f, 170.f / 256.f, 2.f / 16.f);
	RenderBitmap(BITMAP_INVENTORY + 19, x, y + sy, sx + 1, 2, 10 / 256.f, 5 / 16.f, 170.f / 256.f, 2.f / 16.f);
	RenderBitmap(BITMAP_INVENTORY, x, y, 2, sy, 1.f / 256.f, 5 / 16.f, 2.f / 256.f, 125.f / 256.f);
	RenderBitmap(BITMAP_INVENTORY, x + sx, y, 2, sy, 1.f / 256.f, 5 / 16.f, 2.f / 256.f, 125.f / 256.f);
}