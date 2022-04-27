#include "MapGenerator.h"

MapGenerator::MapGenerator()
{
}

MapGenerator::~MapGenerator()
{
}

Map* MapGenerator::GenerateMap(int row, int col, int width, int height)
{
	Map* ret = new Map(row,col,width,height);

	int count = 0;

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++, count++)
		{
			Tile temp;

			temp.width = width;
			temp.height = height;

			temp.mapPos.x = j;
			temp.mapPos.y = i;

			temp.worldPos.x = j * width;
			temp.worldPos.y = i * height;

			temp.weight = rand() % 2;

			ret->tiles.add(temp);
		}
	}

    return ret;
}
