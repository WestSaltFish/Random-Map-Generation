#ifndef __MAP_GENERATOR_H__
#define __MAP_GENERATOR_H__

#include "Map.h"

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
	/// <param name="width">: tile width</param>
	/// <param name="height">: tile height</param>
	/// <param name="eSeek">: map encrypted seek</param>
	/// <returns></returns>
	Map* GenerateDungeonMap(uint row, uint col, uint rooms = 1, uint width = 20, uint height = 20, int eSeek = 0);

private: 

	void SaveSeek(int seek);

	int Encryption(int data);

	uint Decrypt(int data);
};

#endif // !__MAP_GENERATOR_H__