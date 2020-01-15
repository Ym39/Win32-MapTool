#pragma once
#include<Windows.h>

class CMapTile
{
private:
	RECT tileRect;
	int TileType; //0-공백  1- 벽 2- 길
	bool isCollider;
	bool isGate;
	bool isShop;
	bool isMonster;
	bool isSpawn;
	int MonsterType;
	int SceneIndex; //타일에 몬스터가 있을 경우 이어질 씬의 인덱스
	int nextIndex;
public:
	CMapTile();
	CMapTile(int left, int top, int right, int bottom);

	const int GetTileType() const;
	void SetTileType(int type);
	void SetCollider();
	void DeleteCollider();
	bool GetIsCollider() const;

	void SetGate();
	void CancelGate();
	bool GetIsGate() const;
	void SetNextIndex(int idx); 
	int GetNextIndex() const; 

	void SetShop();
	void CancelShop();
	bool GetIsShop() const;
	
	void SetMonster(int type,int index);
	void CancelMonster();
	bool GetIsMonster() const;
	const int GetMonsterType() const;
	int GetNextScene() const;

	void SetSpawnPoint();
	void CancleSpawnPoint();
	bool GetSpawnPoint() const;
	
	int GetLeft() const;
	void SetLeft(int left);
	int GetTop() const;
	void SetTop(int top);
	int GetRight() const;
	void SetRight(int right);
	int GetBottom() const;
	void SetBottom(int bottom);
};

