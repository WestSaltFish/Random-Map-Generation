#include "MapGenerator.h"
#include <iostream>
#include <fstream>
using namespace std;

#include "External/SDL/include/SDL.h"

MapGenerator::MapGenerator()
{
}

MapGenerator::~MapGenerator()
{
}

Map* MapGenerator::GenerateDungeonMap(uint row, uint col, uint rooms, uint tileWidth, uint tileHeight, int eSeek)
{
	Map* ret = new Map(row, col, tileWidth, tileHeight);

	InitSeek(eSeek);

	// Limit room nums
	rooms = rooms > row * col ? row * col : rooms;

	// Get random position, and it should not be on the edge
	iPoint currentPos;

	currentPos.x = (rand() % (row - 2)) + 1;

	currentPos.y = (rand() % (row - 2)) + 1;

	// Create dungeon
	//DungeonMapBacktrack(ret, &rooms, currentPos);

	// Test code
	currentPos_t = currentPos;
	rooms_t = rooms;
	map_t = ret;

	return ret;
}

void MapGenerator::DungeonMapBacktrack(Map* map, uint* rooms, iPoint currentPos)
{
	if (rooms <= 0) return;

	Tile temp;

	// For rooms checker
	iPoint dir[4] = { {1,0}, {0,1}, {-1,0}, {0,-1} };

	// Free space to create rooms
	List<iPoint> freeSpace;

	// Init room with dimension and position
	temp.InitTile(map->tileWidth, map->tileHeight, currentPos, 1);

	map->tiles.add(temp);

	if (--(*rooms) <= 0)return;

	while (*rooms > 0)
	{
		for (int i = 0; i < 4; i++)
		{
			// Check position
			iPoint posCheck = temp.mapPos + dir[i];

			// If not exist room in this position, add to freeSpace
			if (!map->CheckTile(posCheck)) freeSpace.add(posCheck);
		}

		// If not exixt any space
		if (freeSpace.count() <= 0) return;

		int nextPos = (rand() % freeSpace.count());

		DungeonMapBacktrack(map, rooms, freeSpace[nextPos]);

		freeSpace.clear();
	}	
}

void MapGenerator::TestDungeonMapBacktrack()
{
	if (rooms_t <= 0) return;

	Tile temp;

	// For rooms checker
	iPoint dir[4] = { {1,0}, {0,1}, {-1,0}, {0,-1} };

	// Free space to create rooms
	List<iPoint> freeSpace;

	// Init room with dimension and position
	temp.InitTile(map_t->tileWidth, map_t->tileHeight, currentPos_t, 1);

	// Get current Tile
	map_t->currentTile_t = temp;

	// Add to tile list
	map_t->tiles.add(temp);

	// Clear map freeSpace
	map_t->freeSpace_t.clear();

	// Check room nums
	if (--rooms_t <= 0)
	{
		map_t->currentTile_t.height = 0;
		return;
	}

	for (int i = 0; i < 4; i++)
	{
		// Check position
		iPoint posCheck = temp.mapPos + dir[i];

		// If not exist room in this position, add to freeSpace
		if (!map_t->CheckTile(posCheck))
		{
			// Update freeSpace
			Tile temp2;
			temp2.InitTile(map_t->tileWidth, map_t->tileHeight, posCheck, 1);
			map_t->freeSpace_t.add(temp2);
		}
	}

	// If not exist free space, get free Space in last tiles 
	if (map_t->freeSpace_t.count() <= 0)
	{
		for (int i = map_t->tiles.count() - 2; i >= 0; i--)
		{
			temp = map_t->tiles[i];

			for (int i = 0; i < 4; i++)
			{
				// Check position
				iPoint posCheck = temp.mapPos + dir[i];

				// If not exist room in this position, add to freeSpace
				if (!map_t->CheckTile(posCheck))
				{
					// Update freeSpace
					temp.InitTile(map_t->tileWidth, map_t->tileHeight, posCheck, 1);
					map_t->freeSpace_t.add(temp);
				}
			}

			if (map_t->freeSpace_t.count() > 0) break;
		}
		if (map_t->freeSpace_t.count() <= 0) return;
	}

	// Generate next position
	int nextPos = (rand() % map_t->freeSpace_t.count());

	currentPos_t = map_t->freeSpace_t[nextPos].mapPos;

	/*
	for (int i = 0; i < 4; i++)
	{
		// Check position
		iPoint posCheck = temp.mapPos + dir[i];

		// If not exist room in this position, add to freeSpace
		if (!map_t->CheckTile(posCheck))
		{
			for (int i = 0; i < map_t->freeSpace_t.count(); i++)
			{
				if (map_t->freeSpace_t[i].mapPos == posCheck)
					map_t->freeSpace_t.del(map_t->freeSpace_t.At(i));
			}			

			freeSpace.add(posCheck);
		}
	}

	// If not exixt any space
	if (freeSpace.count() <= 0)
	{
		int dif = map_t->freeSpace_t.count() - lastFreeSpace;

		int nextPos = (rand() % lastFreeSpace + dif);

		currentPos_t = map_t->freeSpace_t[nextPos].mapPos;

		map_t->freeSpace_t.del(map_t->freeSpace_t.At(nextPos));

		map_t->currentTile_t.InitTile(map_t->tileWidth, map_t->tileHeight, currentPos_t, 1);

		return;
	}

	int nextPos = (rand() % freeSpace.count());

	currentPos_t = freeSpace[nextPos];

	freeSpace.del(freeSpace.At(nextPos));

	map_t->currentTile_t.InitTile(map_t->tileWidth, map_t->tileHeight, currentPos_t, 1);

	if (freeSpace.count() > 1)lastFreeSpace = freeSpace.count();

	for (int i = 0; i < freeSpace.count(); i++)
	{
		temp.InitTile(map_t->tileWidth, map_t->tileHeight, freeSpace[i], 1);
		map_t->freeSpace_t.add(temp);
	}

	freeSpace.clear();
	*/
}

void MapGenerator::InitSeek(int eSeek)
{
	// Init seek
	uint seek = time(NULL);

	// If exist eSeek, chage seek to decrypted eSeek
	if (eSeek != 0) seek = Decrypt(eSeek);

	// Save new seek to file
	SaveSeek(seek);

	// Init srand with seek
	srand(seek);
}

void MapGenerator::SaveSeek(int seek)
{
	// Open file in mode trunc
	ofstream outfile;

	outfile.open("Assets/MySeek.seek", ios::trunc);

	// Write data
	outfile << Encryption(seek) << endl;

	// Close file
	outfile.close();
}

int MapGenerator::Encryption(int data)
{
	// ^ (XOR): same = 0, different = 1
	return data ^ MAP_KEY;
}

uint MapGenerator::Decrypt(int data)
{
	// ^ (XOR): same = 0, different = 1
	return data ^ MAP_KEY;
}
