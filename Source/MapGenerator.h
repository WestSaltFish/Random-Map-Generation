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
	/// Generate and return map
	/// </summary>
	/// <param name="row"></param>
	/// <param name="col"></param>
	/// <param name="width">tile width</param>
	/// <param name="height">tile height</param>
	/// <param name="seek">map seek</param>
	/// <returns></returns>
	Map* GenerateMap(int row, int col, int width = 20, int height = 20, uint seek = 0);

private: 

	void SaveSeek(int seek);

	int Encryption(int data);

	int Decrypt(int data);
};

#endif // !__MAP_GENERATOR_H__