#pragma once
#include<Windows.h>

class CSeletRect
{
private:
	RECT** GridRect;
	const int MAXVERTICAL;
	const int MAXHORIZON;

	int curX;
	int curY;
public:
	CSeletRect(int v, int h);
	~CSeletRect();
	
	void InitSeletRect();
	void PrintCurRect(HDC hdc);

	const int GetCurX() const;
	const int GetCurY() const;
	
	void CursorMove_Left();
	void CursorMove_Up();
	void CursorMove_Right();
	void CursorMove_Down();

};

