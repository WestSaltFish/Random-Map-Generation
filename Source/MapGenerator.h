#ifndef __MAP_GENERATOR_H__
#define __MAP_GENERATOR_H__

#include "Map.h"
#include <stdlib.h> 
#include <time.h> 

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
	/// <returns></returns>
	Map* GenerateMap(int row, int col, int width = 20, int height = 20);
};

#endif // !__MAP_GENERATOR_H__