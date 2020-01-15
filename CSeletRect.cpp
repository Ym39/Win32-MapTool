#include "CSeletRect.h"

CSeletRect::CSeletRect(int v, int h) :MAXVERTICAL(v), MAXHORIZON(h),curX(4),curY(4)
{
	GridRect = new RECT * [MAXVERTICAL];
	for (int i = 0; i < MAXHORIZON; i++)
	{
		GridRect[i] = new RECT[MAXHORIZON];
	}
}

CSeletRect::~CSeletRect()
{
	for (int i = 0; i < MAXVERTICAL; i++)
		delete[] GridRect[i];

	delete[] GridRect;
}

void CSeletRect::InitSeletRect()
{
	int topTemp = 0;
	int bottomTemp = 65;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (j == 0)
			{
				GridRect[i][j].left = 0;
				GridRect[i][j].top = topTemp;
				GridRect[i][j].right = 65;
				GridRect[i][j].bottom = bottomTemp;
			}
			else
			{
				GridRect[i][j].left = GridRect[i][j - 1].left + 65;
				GridRect[i][j].top = topTemp;
				GridRect[i][j].right = GridRect[i][j - 1].right + 65;
				GridRect[i][j].bottom = bottomTemp;
			}
		}
		topTemp += 65;
		bottomTemp += 65;
	}
}

void CSeletRect::PrintCurRect(HDC hdc)
{
	HPEN Pen, oPen;
	HBRUSH Brush, oBrush;

	Pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	Brush = (HBRUSH)GetStockObject(NULL_BRUSH);
	oPen = (HPEN)SelectObject(hdc, Pen);
	oBrush = (HBRUSH)SelectObject(hdc, Brush);

	Rectangle(hdc, GridRect[curY][curX].left, GridRect[curY][curX].top, GridRect[curY][curX].right, GridRect[curY][curX].bottom);

	SelectObject(hdc, oPen);
	SelectObject(hdc, oBrush);
	DeleteObject(Pen);
	DeleteObject(Brush);

}

const int CSeletRect::GetCurX() const
{
	return curX;
}

const int CSeletRect::GetCurY() const
{
	return curY;
}

void CSeletRect::CursorMove_Left()
{
	if (curX == 0) return;
	curX--;
}

void CSeletRect::CursorMove_Up()
{
	if (curY == 0)return;
	curY--;
}

void CSeletRect::CursorMove_Right()
{
	if (curX == MAXHORIZON - 1)return;
	curX++;
}

void CSeletRect::CursorMove_Down()
{
	if (curY == MAXVERTICAL - 1)return;
	curY++;
}
