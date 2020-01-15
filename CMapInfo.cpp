#include "CMapInfo.h"
#include"resource.h"
#include<stdio.h>

CMapInfo::CMapInfo(int v, int h):MAXVERTICAL(v),MAXHORIZON(h),CollisionCount(0),GateCount(0),ShopCount(0),MonsterCount(0)
{
	Tile = new CMapTile*[MAXVERTICAL];
	for (int i = 0; i < MAXHORIZON; i++)
		Tile[i] = new CMapTile[MAXHORIZON];
}

CMapInfo::~CMapInfo()
{
	for (int i = 0; i < MAXVERTICAL; i++)
		delete[] Tile[i];

	delete[] Tile;
}



void CMapInfo::InitMap(HINSTANCE hInst)
{
	int topTemp = 0;
	int bottomTemp = 65;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (j == 0)
			{
			    Tile[i][j].SetLeft(0);
				Tile[i][j].SetTop(topTemp);
				Tile[i][j].SetRight(65);
				Tile[i][j].SetBottom(bottomTemp);
			}
			else
			{
				Tile[i][j].SetLeft(Tile[i][j - 1].GetLeft() + 65);
				Tile[i][j].SetTop(topTemp);
				Tile[i][j].SetRight(Tile[i][j - 1].GetRight() + 65);
				Tile[i][j].SetBottom(bottomTemp);
			}

		}
		topTemp += 65;
		bottomTemp += 65;
	}
	
	hBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_WALL));
	hBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_FLOOR));
	ShopBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_SHOP));
	MonsterBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_MONSTER_EYE));
	MonsterBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_MONSTER_DRAGONE));

}

void CMapInfo::SetTileType(int x, int y, int type)
{
	Tile[y][x].SetTileType(type);
}

void CMapInfo::SetCollider(int x, int y)
{
	if (CollisionCount == MAXVERTICAL * MAXHORIZON)
		return;
	Tile[y][x].SetCollider();
	CollisionCount++;
}

void CMapInfo::DeleteCollider(int x, int y)
{
	if (CollisionCount == 0)
		return;
	Tile[y][x].DeleteCollider();
	CollisionCount--;
}

const int CMapInfo::GetCollisionCount() const
{
	return CollisionCount;
}

void CMapInfo::SetGate(int x, int y)
{
	Tile[y][x].SetGate();
	GateCount++;
}

void CMapInfo::CancelGate(int x, int y)
{
	Tile[y][x].CancelGate();
	GateCount--;
}

void CMapInfo::SetNextIndex(int x, int y, int idx)
{
	Tile[y][x].SetNextIndex(idx);
}

void CMapInfo::SetShop(int x, int y)
{   
	if (Tile[y][x].GetIsShop() == true)
		return;
	Tile[y][x].SetShop();
	ShopCount++;
}

void CMapInfo::CancelShop(int x, int y)
{
	if (Tile[y][x].GetIsShop() == false)
		return;
	Tile[y][x].CancelShop();
	ShopCount--;
}

void CMapInfo::SetMonster(int x, int y, int type, int index)
{
	if (Tile[y][x].GetIsShop()) //해당 타일이 이미 상점으로 설정되어있다면 해제하고 몬스터를 배치함
	{
		Tile[y][x].CancelShop();
		ShopCount--;
	}
	Tile[y][x].SetMonster(type, index);
	MonsterCount++;
}

void CMapInfo::CancelMonster(int x, int y)
{
	if (Tile[y][x].GetIsMonster() == false) return;

	Tile[y][x].CancelMonster();
	MonsterCount--;
}

void CMapInfo::SetSpawnPoint(int x, int y)
{
	Tile[y][x].SetSpawnPoint();
}

void CMapInfo::CancelSpawnPoint(int x, int y)
{
	Tile[y][x].CancleSpawnPoint();
}



void CMapInfo::PrintMap(HDC hdc)
{
	for (int i = 0; i < MAXVERTICAL; i++)
	{
		for (int j = 0; j < MAXHORIZON; j++)
		{
			DrawMap(hdc,i,j,Tile[i][j].GetTileType());

		}
	}
	
	if (ShopCount != 0)
	{

		for (int i = 0; i < MAXVERTICAL; i++)
		{
			for (int j = 0; j < MAXHORIZON; j++)
			{
				
				if (Tile[i][j].GetIsShop() == true)
				{
					DrawShop(hdc, i, j);
				}

			}
		}
	}

	if (MonsterCount != 0)
	{
		for (int i = 0; i < MAXVERTICAL; i++)
		{
			for (int j = 0; j < MAXHORIZON; j++)
			{

				if (Tile[i][j].GetIsMonster() == true)
				{
					DrawMonster(hdc, i, j, Tile[i][j].GetMonsterType());
				}

			}
		}
	}

}

void CMapInfo::DrawMap(HDC hdc,int i,int j,int type)
{
	int x = Tile[i][j].GetLeft();
	int y = Tile[i][j].GetTop();

	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	switch (type)
	{
	case 1:
		OldBitmap = (HBITMAP)SelectObject(MemDC, hBit[0]);
		break;
	case 2:
		OldBitmap = (HBITMAP)SelectObject(MemDC, hBit[1]);
		break;
	default:
		DeleteDC(MemDC);
		return;
	}

	BitBlt(hdc,x, y, 65,65, MemDC,0,0, SRCCOPY);
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
	
}

void CMapInfo::DrawShop(HDC hdc,int i, int j)
{
	int x = Tile[i][j].GetLeft();
	int y = Tile[i][j].GetTop();

	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, ShopBit);
	TransparentBlt(hdc, x, y, 65, 65, MemDC, 0, 0, 65, 65, RGB(255, 255, 255));
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);

}

void CMapInfo::DrawMonster(HDC hdc, int i, int j,int type)
{
	int x = Tile[i][j].GetLeft();
	int y = Tile[i][j].GetTop();

	HDC MemDC;
	HBITMAP OldBitmap;
	MemDC = CreateCompatibleDC(hdc);
	switch (type)
	{
	case 1:
		OldBitmap = (HBITMAP)SelectObject(MemDC, MonsterBit[0]);
		break;
	case 2:
		OldBitmap = (HBITMAP)SelectObject(MemDC, MonsterBit[1]);
		break;
	default:
		DeleteDC(MemDC);
		return;
	}
	TransparentBlt(hdc, x, y, 65, 65, MemDC, 0, 0, 65, 65, RGB(255, 255, 255));
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

void CMapInfo::DrawGrid(HDC hdc)
{
	HPEN Pen, oPen,RPen;
	HBRUSH Brush, oBrush;

	Pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	RPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	Brush = (HBRUSH)GetStockObject(NULL_BRUSH);
	oPen = (HPEN)SelectObject(hdc, Pen);
	oBrush = (HBRUSH)SelectObject(hdc, Brush);
	for (int i = 0; i < MAXVERTICAL; i++)
	{
		for (int j = 0; j < MAXHORIZON; j++)
		{
			Rectangle(hdc, Tile[i][j].GetLeft(), Tile[i][j].GetTop(), Tile[i][j].GetRight(), Tile[i][j].GetBottom());

			if (Tile[i][j].GetIsCollider()==true)
			{

				MoveToEx(hdc, Tile[i][j].GetLeft(), Tile[i][j].GetTop(), NULL);
				LineTo(hdc, Tile[i][j].GetRight(), Tile[i][j].GetBottom());
			}
			if (Tile[i][j].GetIsGate() == true)
			{
				char s[4];
				int index = Tile[i][j].GetNextIndex();
				sprintf(s, "%d", index);

				SelectObject(hdc, RPen);
				MoveToEx(hdc, Tile[i][j].GetLeft(), Tile[i][j].GetTop(), NULL);
				LineTo(hdc, Tile[i][j].GetRight(), Tile[i][j].GetBottom());
				TextOut(hdc, Tile[i][j].GetLeft(), Tile[i][j].GetTop()+40, s, strlen(s));
				SelectObject(hdc, Pen);
			}
			if (Tile[i][j].GetIsMonster() == true)
			{
				char s[20];
				int index = Tile[i][j].GetNextScene();
				sprintf(s, "Scene : %d", index);
				TextOut(hdc, Tile[i][j].GetLeft(), Tile[i][j].GetTop() + 40, s, strlen(s));
			}
			if (Tile[i][j].GetSpawnPoint() == true)
			{
				char s[20];
				sprintf(s, "Spawn");
				TextOut(hdc, Tile[i][j].GetLeft(), Tile[i][j].GetTop() + 40, s, strlen(s));
			}
		}
	}

	SelectObject(hdc, oPen);
	SelectObject(hdc, oBrush);
	DeleteObject(Pen);
	DeleteObject(RPen);
	DeleteObject(Brush);
}

void CMapInfo::CleanMap()
{
	for (int i = 0; i < MAXVERTICAL; i++)
	{
		for (int j = 0; j < MAXHORIZON; j++)
		{
			Tile[i][j].CancelGate();
			Tile[i][j].CancelShop();
			Tile[i][j].CancelMonster();
			Tile[i][j].DeleteCollider();
			Tile[i][j].CancleSpawnPoint();
		}
	}
	CollisionCount = 0;
	GateCount = 0;
	ShopCount = 0;
	MonsterCount = 0;
}

BOOL CMapInfo::LoadMapFile(char* strFileName)
{
	FILE* fp;
	fp = fopen(strFileName, "rt");
	int tempType;
	int isColl;
	int isGate;
	int index;
	int isShop;
	int isMonster;
	int MonsterType;
	int nextScene;
	int isSpawn;

	CleanMap();

	fscanf(fp, "%d\n", &CollisionCount);
	fscanf(fp, "%d\n", &GateCount);
	fscanf(fp, "%d\n", &ShopCount);
	fscanf(fp, "%d\n", &MonsterCount);


	for (int i = 0; i < MAXVERTICAL; i++)
	{
		for (int j = 0; j < MAXHORIZON; j++)
		{
			fscanf(fp, "%d %d %d %d %d %d %d %d %d\n", &tempType,&isColl, &isGate, &index,&isShop,&isMonster,&MonsterType,&nextScene,&isSpawn);
			Tile[i][j].SetTileType(tempType);
			if (isColl == 1) Tile[i][j].SetCollider();
			if (isGate == 1)Tile[i][j].SetGate();
			Tile[i][j].SetNextIndex(index);
			if (isShop == 1)Tile[i][j].SetShop();
			if (isMonster == 1)
			{
				Tile[i][j].SetMonster(MonsterType, nextScene);
			}
			if (isSpawn == 1)
			{
				Tile[i][j].SetSpawnPoint();
			}
		}
	}

	fclose(fp);

	return TRUE;
}

BOOL CMapInfo::SaveMapFile(char* strFileName)
{
	FILE* fp;
	fp = fopen(strFileName, "wt");

	fprintf(fp, "%d\n", CollisionCount);
	fprintf(fp, "%d\n", GateCount);
	fprintf(fp, "%d\n", ShopCount);
	fprintf(fp, "%d\n", MonsterCount);

	for (int i = 0; i < MAXVERTICAL; i++)
	{
		for (int j = 0; j < MAXHORIZON; j++)
		{
			fprintf(fp, "%d %d %d %d %d %d %d %d %d\n", Tile[i][j].GetTileType(), Tile[i][j].GetIsCollider() ? 1 : 0,
				Tile[i][j].GetIsGate() ? 1 : 0, Tile[i][j].GetNextIndex(),Tile[i][j].GetIsShop()?1:0
			,Tile[i][j].GetIsMonster()?1:0 , Tile[i][j].GetMonsterType(),Tile[i][j].GetNextScene(),Tile[i][j].GetSpawnPoint()?1:0);
		}
	}

	fclose(fp);

	return TRUE;
}