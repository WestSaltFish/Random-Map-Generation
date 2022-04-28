#include "MapGenerator.h"
#include <iostream>
#include <fstream>
using namespace std;


MapGenerator::MapGenerator()
{
}

MapGenerator::~MapGenerator()
{
}

Map* MapGenerator::GenerateMap(int row, int col, int width, int height, uint seek)
{
	Map* ret = new Map(row,col,width,height);

	if (seek == 0) seek = time(NULL);
	else seek = Decrypt(seek);

	SaveSeek(seek);

	srand(seek);

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
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

void MapGenerator::SaveSeek(int seek)
{
	// Open in mode append
	ofstream outfile;

	outfile.open("Assets/MySeek.seek", ios::trunc);

	cout << seek << endl;

	outfile << Encryption(seek) << endl;

	// close file
	outfile.close();
}

int MapGenerator::Encryption(int data)
{
	return data ^ MAP_KEY;
}

int MapGenerator::Decrypt(int data)
{
	return data ^ MAP_KEY;
}
