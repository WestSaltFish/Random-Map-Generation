#ifndef __MAP_GENERATOR_H__
#define __MAP_GENERATOR_H__

#include "Map.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <vector>

using namespace std;

#define MAP_KEY 2333333333

class MapGenerator
{
public:

	MapGenerator();

	~MapGenerator();

	/// <summary>
	/// Generate and return Dungeon map
	/// </summary>
	/// <param name="row">: map row</param>
	/// <param name="col">: map col</param>
	/// <param name="rooms">: numbers of room you want to create</param>
	/// <param name="tileWidth">: tile width in pixel</param>
	/// <param name="tileHeight">: tile height in pixel</param>
	/// <param name="eSeek">: map encrypted seek</param>
	/// <returns></returns>
	Map* GenerateDungeonMap(uint row, uint col, uint rooms = 1, uint tileWidth = 10, uint tileHeight = 10, int eSeek = 0);

	void TestDungeonMapBacktrack();

	/// <summary>
	/// Generate and return Dungeon map with CellularAutomation
	/// </summary>
	/// <param name="row">: map row</param>
	/// <param name="col">: map col</param>
	/// <param name="tileWidth">: tile width in pixel</param>
	/// <param name="tileHeight">: tile height in pixel</param>
	/// <param name="eSeek">>: map encrypted seek</param>
	/// <returns></returns>
	Map* GenerateDungeonMapCA(uint row, uint col, uint tileWidth = 10, uint tileHeight = 10, int eSeek = 0);

private: 

	void DungeonMapBacktrack(Map* map, uint* rooms, iPoint currentPos);


	void CreateBaseMapCA(Map* map);

	void LoopOptimizeMapCA(Map* map, uint loopCA = 3);

	std::vector<std::vector<Tile>> FindAreasMapCA(Map* map);

	void ConnectAreasMapCA(Map* map, std::vector<std::vector<Tile>> areas);


	void InitSeek(int eSeek);

	void SaveSeek(int seek);

	int Encryption(int data);

	uint Decrypt(int data);

private:
	uint tileWidth = 1;

	uint tileHeight = 1;

	// For testing HARDCODE DO NOT USE FOR PROJECT
	Map* map_t = nullptr;

	uint rooms_t = 0;

	iPoint currentPos_t;

	int lastFreeSpace = 0;
};

#endif // !__MAP_GENERATOR_H__