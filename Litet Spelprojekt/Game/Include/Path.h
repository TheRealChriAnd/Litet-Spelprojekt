#pragma once
#include <GLM\vec2.hpp>
#include "..\Include\Grid.h"
#include "..\Include\Tile.h"
#include <math.h>
#include <algorithm>

struct tls {
	float m_g = 0;
	float m_f;
	glm::ivec2 m_parentTile;
	bool m_closed = false;
};

class Path
{
private:
	tls** m_ppTiles;
	glm::ivec2* m_openList;
	glm::ivec2* m_pPath;
	glm::ivec2 m_currentTile;
	glm::ivec2 m_goalTile;
	glm::ivec2 m_startTile;
	glm::ivec2 m_closestTile;
	glm::ivec2 m_size;
	int** m_ppMap;
	int m_smallestH;
	int m_nrOfTilesOpen;
	int m_nrOfPathTiles = 0;
	bool m_goalSet;

private:
	void AddToOpen(int x, int y, int addX, int addY);
	void CheckAdjacent();
	bool MoveToNextTile();

public:
	Path(const Tile*** pppTilemap, glm::ivec2 size);
	~Path();

	glm::ivec2* FindPath(glm::ivec2 start, glm::ivec2 goal);
	bool IsGoalSet();
	int GetNrOfPathTiles();
};
