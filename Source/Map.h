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

	/// <summary>
	/// 0 = Path/Ground/Null, 1 = Wall/Room
	/// </summary>
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

	Map(int row, int col, int tileWidth, int tileHeight);

	~Map();

	/// <summary>
	/// Detects if there is a room
	/// </summary>
	/// <param name="mapPos">: map position NOT WORLD POSITION!</param>
	/// <returns></returns>
	bool CheckTile(iPoint mapPos);

	/// <summary>
	/// Detects the room type
	/// </summary>
	/// <param name="mapPos">: map position NOT WORLD POSITION!</param>
	/// <returns> return -1 if is out of range</returns>
	int CheckTileType(iPoint mapPos);
	
	/// <summary>
	/// Check how many tiles of the given type exist around the position it receives
	/// </summary>
	/// <param name="mapPos"> map position NOT WORLD POSITION! </param>
	/// <param name="type"> The type you want to check </param>
	/// <returns></returns>
	int CheckNeighborTile(iPoint mapPos, int type = 0);

	/// <summary>
	/// Check if the received position is an edge or not
	/// </summary>
	/// <param name="mapPos"> map position NOT WORLD POSITION! </param>
	/// <returns></returns>
	bool CheckBorder(iPoint mapPos);

	void PostUpdate();

public:
	int row = 0;
	int col = 0;

	int tileWidth = 0;
	int tileHeight = 0;

	int mapWidth = 0;
	int mapHeight = 0;

	List<Tile> tiles;

	Tile* tiles2 = nullptr;

	bool drawGrid = true;

	// For testing
	List<Tile> freeSpace_t;
	Tile currentTile_t;
};

#endif // !__MAP_H__