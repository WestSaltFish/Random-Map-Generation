#ifndef __MAP_H__
#define __MAP_H__

#include "List.h"
#include "Point.h"
#include "Application.h"

struct Tile
{
public:
	int width = 0;

	int height = 0;

	// position in the map
	iPoint mapPos;

	iPoint worldPos;

	int type = 0;

	void InitTile(int width, int height, iPoint mapPos, int type)
	{
		this->width = width;

		this->height = height;

		this->mapPos = mapPos;

		this->type = type;

		this->worldPos.x = mapPos.x * width;

		this->worldPos.y = mapPos.y * height;
	}
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

	/// <summary>
	/// Detects if there is a room
	/// </summary>
	/// <param name="mapPos">: map position NOT WORLD POSITION!</param>
	/// <returns></returns>
	bool CheckRoom(iPoint mapPos);
	
	void PostUpdate();
};

#endif // !__MAP_H__