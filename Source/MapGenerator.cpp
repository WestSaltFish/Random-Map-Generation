#include "MapGenerator.h"
#include <iostream>
#include <fstream>
using namespace std;

#include "ModuleInput.h"


MapGenerator::MapGenerator()
{
}

MapGenerator::~MapGenerator()
{
}

Map* MapGenerator::GenerateDungeonMap(uint row, uint col, uint rooms, uint width, uint height, int eSeek)
{
	Map* ret = new Map(row, col, width, height);

	InitSeek(eSeek);

#pragma region init variables

	// Limit room nums
	rooms = rooms > row * col ? row * col : rooms;

	// Create First room 
	Tile temp;

	// Free space to create rooms
	List<iPoint> freeSpace;

	// For rooms checker
	iPoint dir[4] = { {1,0}, {0,1}, {-1,0}, {0,-1} };

	// Get random position, and it should not be on the edge
	iPoint currentPos;

	currentPos.x = (rand() % (row - 2)) + 1;

	currentPos.y = (rand() % (row - 2)) + 1;

#pragma endregion

#pragma region Create first room

	// Init room with dimension and position
	temp.InitTile(width, height, currentPos, 1);

	// Add to Map
	ret->tiles.add(temp);

	// Always when we create a room, rooms--
	rooms--;

	for (int i = 0; i < 4; i++)
	{
		// Check position
		iPoint posCheck = temp.mapPos + dir[i];
		 
		// If not exist room in this position, add to freeSpace
		if (!ret->CheckTile(posCheck)) freeSpace.add(posCheck);
	}

	int randNum = (rand() % freeSpace.count());

	//currentPos = freeSpace[randNum];

	//freeSpace.del(freeSpace.At(randNum));

#pragma endregion

	/*
#pragma region Generate all rooms

	while (rooms > 0)
	{
		//printf("Count : %d\tX: %d\tY: %d\n", rooms, currentPos.x, currentPos.y);

		temp.InitTile(width, height, currentPos, 1);

		ret->tiles.add(temp);

		rooms--;

		List<iPoint> tempPos;

		// Check free position
		for (int i = 0; i < 4; i++)
		{
			// Check position
			iPoint posCheck = temp.mapPos + dir[i];

			// If not exist room in this position, add to tempPos
			if (!ret->CheckTile(posCheck))
			{
				for (int i = 0; i < freeSpace.count(); i++)
				{
					if (freeSpace[i] == posCheck)
					{
						freeSpace.del(freeSpace.At(i));
					}
				}		
				tempPos.add(posCheck);
			}
		}

		// If there is no space around this room
		if (tempPos.count() <= 0)
		{
			// Check if exist space in freeSpace
			if (freeSpace.count() <= 0) return ret;

			// Change current Posiion for next generation
			int randNum = (rand() % freeSpace.count());

			currentPos = freeSpace[randNum];

			freeSpace.del(freeSpace.At(randNum));

			continue;
		}
		
		// If exist free space around this room

		// Change current Posiion for next generation
		int randNum = (rand() % tempPos.count());

		currentPos = tempPos[randNum];

		tempPos.del(tempPos.At(randNum));

		// If exist more 
		if (tempPos.count() <= 0) continue;

		// Add position in freeSpace for spare
		for (int i = 0, count = tempPos.count(); i < count; i++) freeSpace.add(tempPos[i]);

		tempPos.clear();	
	}

#pragma endregion

*/

	return ret;
}

void MapGenerator::DungeonMapBacktrack(Map* map, uint rooms, iPoint currentPos)
{

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
