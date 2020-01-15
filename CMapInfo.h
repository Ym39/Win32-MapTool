#pragma once
#include"CMapTile.h"

class CMapInfo
{
private:
	CMapTile** Tile;
	const int MAXVERTICAL;
	const int MAXHORIZON;
	int CollisionCount;
	int GateCount;
	int ShopCount;
	int MonsterCount;

	HBITMAP hBit[2];
	HBITMAP ShopBit;
	HBITMAP MonsterBit[2];
public:
	CMapInfo(int v, int h);
	~CMapInfo();

	void InitMap(HINSTANCE hInst);

	void SetTileType(int x, int y, int type);
	void SetCollider(int x, int y);
	void DeleteCollider(int x, int y);
	const int GetCollisionCount() const;

	void SetGate(int x, int y);
	void CancelGate(int x, int y);
	void SetNextIndex(int x, int y, int idx);

	void SetShop(int x, int y);
	void CancelShop(int x, int y);

	void SetMonster(int x, int y, int type, int index);
	void CancelMonster(int x, int y);

	void SetSpawnPoint(int x, int y);
	void CancelSpawnPoint(int x, int y);


	void PrintMap(HDC hdc);
	void DrawMap(HDC hdc,int i,int j,int type);
	void DrawShop(HDC hdc,int i, int j);
	void DrawMonster(HDC hdc, int i, int j,int type);
	void DrawGrid(HDC hdc);

	void CleanMap();
	BOOL LoadMapFile(char* strFileName);
	BOOL SaveMapFile(char* strFileName);
};

