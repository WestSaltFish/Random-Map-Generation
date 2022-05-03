#include "MapGenerator.h"

MapGenerator::MapGenerator()
{
}

MapGenerator::~MapGenerator()
{
}

#pragma region Dungeon global map

Map* MapGenerator::GenerateDungeonMap(uint row, uint col, uint rooms, uint tileWidth, uint tileHeight, int eSeed)
{
	Map* ret = new Map(row, col, tileWidth, tileHeight);

	InitSeed(eSeed);

	// Limit room nums
	rooms = rooms > row * col ? row * col : rooms;


	iPoint currentPos = { 1, 1};

	// TODO A0
	// Get random position, and it should not be on the edge 
	// Use func rand()

		// Do something here...

	CreateBaseMap(ret);

	// Create dungeon
	if (!debugMode) DungeonMapBacktrack(ret, &rooms, currentPos);

	// Test code
	rooms_t = rooms;
	map_t = ret;
	map_t->currentTile = ret->tiles[currentPos.y * ret->row + currentPos.x];
	map_t->currentTile.width = 0;

	return ret;
}

void MapGenerator::DungeonMapBacktrack(Map* map, uint* rooms, iPoint currentPos)
{
	if (rooms <= 0) return;

	// For rooms checker
	iPoint dir[4] = { {1,0}, {0,1}, {-1,0}, {0,-1} };

	// Free space to create rooms
	List<iPoint> freeSpace;

	// Init room with dimension and position
	map->tiles[currentPos.y * map->row + currentPos.x].type = 1;

	if (--(*rooms) <= 0)return;

	while (*rooms > 0)
	{
		for (int i = 0; i < 4; i++)
		{
			// Check position
			iPoint posCheck = currentPos + dir[i];

			// TODO A1
			// If not exist room in this position, add to freeSpace
			// Use func Map::CheckTileType()
			// ground type == 0, room type == 1

				// Do something here...		
		}

		// If not exixt any space
		if (freeSpace.count() <= 0) return;

		int nextPos = (rand() % freeSpace.count());

		DungeonMapBacktrack(map, rooms, freeSpace[nextPos]);
	}	
}

void MapGenerator::CreateBaseMap(Map* map)
{
	for (int i = 0; i < map->col; i++)
	{
		for (int j = 0; j < map->row; j++)
		{
			Tile temp;

			// Init all tiles with dimension and position
			temp.InitTile(map->tileWidth, map->tileHeight, { j, i }, 0);

			map->tiles.add(temp);
		}
	}
}

void MapGenerator::TestDungeonMapBacktrack()
{
	if (rooms_t <= 0) return;

	map_t->currentTile.width = map_t->tileWidth;

	// For rooms checker
	iPoint dir[4] = { {1,0}, {0,1}, {-1,0}, {0,-1} };

	if(map_t->posibleDir_t.size() > 0)
	{
		// Get 1 of possible directions for next Position
		int nextPosIndex = rand() % map_t->posibleDir_t.size();

		iPoint nextPos = map_t->posibleDir_t[nextPosIndex];

		map_t->currentTile = map_t->tiles[nextPos.y * map_t->row + nextPos.x];

		for (int i = 0; i < map_t->posibleDir_t.size(); i++)
		{
			// Pass the nextposition
			if (i == nextPosIndex) continue;
			map_t->freeSpace_t.push(map_t->tiles[map_t->posibleDir_t[i].y * map_t->row + map_t->posibleDir_t[i].x]);
		}
	}
	else if(map_t->freeSpace_t.size() > 0)
	{
		// If does not exist possible direccion
		// If exist free space	
		for (int i = 0; i < map_t->freeSpace_t.size(); i++)
		{
			iPoint nextPos = map_t->freeSpace_t.top().mapPos;

			// Delete last free position
			map_t->freeSpace_t.pop();

			// If is wall, continue to next
			if (map_t->tiles[nextPos.y * map_t->col + nextPos.x].type == 1) continue;

			// Take last position of free space for currentPos_t
			map_t->currentTile = map_t->tiles[nextPos.y * map_t->col + nextPos.x];
			break;
		}	
	}

	// Clear posible positions
	map_t->posibleDir_t.clear();

	for (int i = 0; i < 4; i++)
	{
		// Check position
		iPoint posCheck = map_t->currentTile.mapPos + dir[i];

		// If not exist room in this position, add to freeSpace
		if (map_t->CheckTileType(posCheck) == 0)
		{
			// Get possible directions
			map_t->posibleDir_t.push_back(posCheck);
		}
	}
	
	// Change current tile to Room
	map_t->tiles[map_t->currentTile.mapPos.y * map_t->row + map_t->currentTile.mapPos.x].type = 1;

	// Check room nums
	if (--rooms_t <= 0)
	{
		map_t->currentTile.width = 0;
		// Clear posibleDir_t so it doesn't keep rendering
		map_t->posibleDir_t.clear();
		return;
	}
}

#pragma endregion

//------------------------------------------------------------------------------------------------------------------------------

#pragma region Map with Cellular Automata

Map* MapGenerator::GenerateDungeonMapCA(uint row, uint col, uint tileWidth, uint tileHeight, int eSeed)
{
	Map* ret = new Map(row, col, tileWidth, tileHeight);

	InitSeed(eSeed);

	CreateBaseMapCA(ret);

	// Optimize map with Cellular Automata algorithm
	if (!debugMode)
	{
		LoopOptimizeMapCA(ret, 3);

		// Find and get map areas
		vector<vector<Tile>> areas = FindAreasMapCA(ret);

		// Connect areas
		ConnectAreasMapCA(ret, areas);

		return ret;
	}

	// Test code
	mapCAStep_t = 4;

	map_t = ret;

	return ret;
}

void MapGenerator::TestGenerateDungeonMapCA()
{
	if (mapCAStep_t <= 0) return;

	if (mapCAStep_t > 1) LoopOptimizeMapCA(map_t, 1);

	else ConnectAreasMapCA(map_t, FindAreasMapCA(map_t));

	mapCAStep_t--;
}

void MapGenerator::CreateBaseMapCA(Map* map)
{
	Tile temp;

	for (int i = 0; i < map->col; i++)
	{
		for (int j = 0; j < map->row; j++)
		{
			int randNum;

			// If is border, create wall
			if (map->CheckBorder({ j, i })) randNum = 2;

			// If not, random
			else randNum = rand() % 3;

			// 1/3 chance to create a wall 
			int types[3] = { 0, 0, 1 };

			temp.InitTile(map->tileWidth, map->tileHeight, { j, i }, types[randNum]);

			map->tiles.add(temp);
		}
	}
}

void MapGenerator::LoopOptimizeMapCA(Map* map, uint loopCA)
{
	// We keep the type data separately
	List<int> tempTypes;

	Tile temp;

	for (int j = 0, count = map->tiles.count(); j < count; j++) tempTypes.add(map->tiles[j].type);

	// Loop for optimize the map
	for (int i = 0; i < loopCA; i++)
	{
		for (int j = 0, count = map->tiles.count(); j < count; j++)
		{			
			temp = map->tiles[j];

			// TODO B0
			// The edges should NOT change
			// Use Map::CheckBorder()
			 
				// Do something here...

			// Get neighbors with func Map::CheckNeighborTile() to get walls nums
			// wall type == 1

				// Do something here...

			int neighbor = 0;

			tempTypes[j] = (neighbor >= 4 - temp.type) ? 1 : 0;

			// Delete this region in the release version
#pragma region Print Progress bar

			float loadNum = (float)(count * i + j) / (float)(count * loopCA);

			int load = (int)(loadNum * 10);

			cout << "Optimize Map with CA [";

			for (int i = 0; i < 10; i++)
			{
				if (load-- > 0) cout << "=";
				else cout << ":";
			}

			cout << "] " << loadNum * 100 << "%" << endl;

#pragma endregion
		}

		// Update tiles type when finish a loop
		for (int j = 0, count = tempTypes.count(); j < count; j++) map->tiles[j].type = tempTypes[j];
	}

	system("cls");
}

vector<vector<Tile>> MapGenerator::FindAreasMapCA(Map* map)
{
	// All free tiles
	List<Tile> groundTiles;

	// Stack for DFS tiles
	stack<Tile> tileStack;

	// All areas
	vector<vector<Tile>> ret;

	// Direccion for check
	iPoint dir[4] = { {1,0}, {0,1}, {-1,0}, {0,-1} };

	// Get all free tiles
	for (int i = 0, count = map->tiles.count(); i < count; i++)
	{
		if (map->tiles[i].type == 0) groundTiles.add(map->tiles[i]);
	}

	// Init tileStack
	tileStack.push(groundTiles[0]);

	groundTiles.del(groundTiles.At(0));

	// Create first area
	vector<Tile> area;

	area.push_back(tileStack.top());

	// Add first area to area vector
	ret.push_back(area);

	int groundTileStartCount = groundTiles.count();	// Just for testing for Print Progress bar !!!!

	// Find all areas
	while (groundTiles.count() > 0)
	{
		// Delete this region in the release version
#pragma region Print Progress bar

		float loadNum = ((float)groundTileStartCount - groundTiles.count()) / (float)groundTileStartCount;

		int load = (int)(loadNum * 10);

		cout << "Find & connect areas [";

		for (int i = 0; i < 10; i++)
		{
			if (load-- > 0) cout << "=";
			else cout << ":";
		}

		cout << "] " << loadNum * 100 << "%" << endl;

#pragma endregion

		Tile currentTile = tileStack.top();

		tileStack.pop();

		// Check around free space
		for (int i = 0; i < 4; i++)
		{
			iPoint checkPos = currentTile.mapPos + dir[i];

			for (int i = 0, count = groundTiles.count(); i < count; i++)
			{
				if (groundTiles[i].mapPos == checkPos)
				{
					tileStack.push(groundTiles[i]);

					ret.back().push_back(tileStack.top());

					groundTiles.del(groundTiles.At(i));

					break;
				}
			}
		}

		if (groundTiles.count() <= 0) break;

		// Create new area
		if (tileStack.size() <= 0)
		{
			// random tile in groundTiles
			int randNum = rand() % groundTiles.count();

			tileStack.push(groundTiles[randNum]);

			groundTiles.del(groundTiles.At(randNum));

			// TODO B1 
			// Create and add area to area vector (ret)
			// Reference : line 330 - 336

				// Do something here
		}
	}

	system("cls");

	// Delete area smaller 2
	for (int i = 0, size = ret.size(); i < size; i++)
	{
		if (ret[i].size() <= 2)
		{
			for (int j = 0; j < ret[i].size(); j++) map->tiles[ret[i][j].mapPos.y * map->row + ret[i][j].mapPos.x].type = 1;
			ret[i].clear();
			ret.erase(ret.begin() + i);
			i = -1;
			size = ret.size();
		}
	}

	return ret;
}

void MapGenerator::ConnectAreasMapCA(Map* map, std::vector<std::vector<Tile>> areas)
{
	// Find the biggest area
	pair<int, int> biggest;

	biggest.first = 0;

	// Find the biggest area
	for (int i = 0; i < areas.size(); i++)
	{
		biggest.second = areas[i].size() > biggest.first ? i : biggest.second;
	}

	// Connect all area with biggest area
	for (int i = 0; i < areas.size(); i++)
	{
		// If is biggest room, pass to next
		if (i == biggest.second) continue;

		iPoint firstPos = areas[i][rand() % areas[i].size()].mapPos;

		iPoint secondPos = areas[biggest.second][rand() % areas[biggest.second].size()].mapPos;

		iPoint dir = secondPos - firstPos;

		// Step X
		int x = abs(dir.x);
		// Step Y
		int y = abs(dir.y);

		// Init direccion
		if (dir.x != 0) dir.x = dir.x > 0 ? 1 : -1;

		if (dir.y != 0) dir.y = dir.y > 0 ? 1 : -1;

		// Open the path X
		for (int i = 0; i < x; i++)
		{
			secondPos.x -= dir.x;

			map->tiles[secondPos.y * map->row + secondPos.x].type = 0;
		}
		// Open the path Y
		for (int i = 0; i < y; i++)
		{
			secondPos.y -= dir.y;

			map->tiles[secondPos.y * map->row + secondPos.x].type = 0;
		}
	}
}

#pragma endregion

//------------------------------------------------------------------------------------------------------------------------------

#pragma region Seed

void MapGenerator::InitSeed(int eSeed)
{
	uint seed = 0;

	// TODO Seed
	// Default eSeed == 0
	// If exist eSeed, chage seed to decrypted eSeed

		// Do something here...
 
	// Else init seed with time(NULL)

		// Do something here...

	// Save new seed to file
	SaveSeed(seed);

	// Init srand with seed
	srand(seed);
}

void MapGenerator::SaveSeed(int seed)
{
	// Open file in mode trunc
	ofstream outfile;

	outfile.open("Assets/MySeed.seed", ios::trunc);

	// Write data
	outfile << Encryption(seed) << endl;

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

#pragma endregion