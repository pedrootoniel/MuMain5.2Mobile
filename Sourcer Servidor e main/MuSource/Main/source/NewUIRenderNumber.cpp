// NewUIRenderNumber.cpp: implementation of the CNewUIRenderNumber class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NewUIRenderNumber.h"	// self

SEASON3B::CNewUIRenderNumber* SEASON3B::CNewUIRenderNumber::GetInstance()
{
	static CNewUIRenderNumber s_NewUIRenderNumber;
	return &s_NewUIRenderNumber;
}

SEASON3B::CNewUIRenderNumber::CNewUIRenderNumber()
{
	LoadImage();
}

SEASON3B::CNewUIRenderNumber::~CNewUIRenderNumber()
{
	ReleaseImage();
}

void SEASON3B::CNewUIRenderNumber::LoadImage()
{
	LoadBitmap("Common\\Interface\\ui_font_number.tga", IMAGE_NUMBER1, GL_LINEAR);
	LoadBitmap("Common\\Interface\\ui_font_classic.tga", IMAGE_FONT_CLASSIC);
}

void SEASON3B::CNewUIRenderNumber::ReleaseImage()
{
	DeleteBitmap(IMAGE_NUMBER1);
}

float SEASON3B::CNewUIRenderNumber::RenderNumber(float x, float y, int iNum, float fScale)
{
	EnableAlphaTest();

	float width, height;

	if(fScale < 0.3f)
	{
		return x;
	}

	width = 12.f * (fScale - 0.3f);
	height = 16.f * (fScale - 0.3f);

	char strText[32];
	itoa(iNum, strText, 10);
	int iLength = (int)strlen(strText);

	x -= width * iLength / 2;

	for(int i=0; i<iLength; ++i)
	{
		float fU = (float)(strText[i] - 48) * 12.f / 128.f;
		RenderBitmap(IMAGE_NUMBER1, x, y, width, height, fU, 0.f, 12.f/128.f, 14.f/16.f);
		x += width * 0.8f;
	}

	return x;
}

float SEASON3B::CNewUIRenderNumber::RenderOldNumber(float X, float Y, int Value, float Width, float Height)
{
	EnableAlphaTest();

	char strText[32];

	itoa(Value, strText, 10);

	int iLength = (int)strlen(strText);

	X = X - (double)iLength * Width / 2.0;

	for (int i = 0; i < iLength; ++i)
	{
		float fU = (float)(strText[i] - 48) * 16.f / 256.f;

		RenderBitmap(IMAGE_FONT_CLASSIC, X, Y, Width, Height, fU, 0, 0.0625, 0.5, 1, 1, 0.0);

		X = Width * 0.69999999 + X;
	}

	glColor3f(1.0, 1.0, 1.0);

	return X;
}