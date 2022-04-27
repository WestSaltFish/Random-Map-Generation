#ifndef __MAP_H__
#define __MAP_H__

#include "List.h"
#include "Point.h"
#include "Application.h"

struct Tile
{
	int width = 0;

	int height = 0;

	// position in the map
	iPoint mapPos;

	iPoint worldPos;

	int weight = 0;
};


class Map
{
public:

	Map(int row, int col, int gridWidth, int gridHeight);

	~Map();

	int row = 0;
	int col = 0;

	int gridWidth = 0;
	int gridHeight = 0;

	int mapWidth = 0;
	int mapHeight = 0;

	List<Tile>tiles;

	void PostUpdate();
};

#endif // !__MAP_H__