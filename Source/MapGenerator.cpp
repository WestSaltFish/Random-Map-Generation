#include "MapGenerator.h"

MapGenerator::MapGenerator()
{
}

MapGenerator::~MapGenerator()
{
}

#pragma region Dungeon global map

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
	if (!debugMode)
	{
		DungeonMapBacktrack(ret, &rooms, currentPos);
		return ret;
	}

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
}

#pragma endregion

//------------------------------------------------------------------------------------------------------------------------------

#pragma region map with Cellular Automata

Map* MapGenerator::GenerateDungeonMapCA(uint row, uint col, uint tileWidth, uint tileHeight, int eSeek)
{
	Map* ret = new Map(row, col, tileWidth, tileHeight);

	InitSeek(eSeek);

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

	for (int i = 0; i < map->row; i++)
	{
		for (int j = 0; j < map->col; j++)
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

			// The edges should not change
			if (map->CheckBorder(temp.mapPos)) continue;

			int neighbor = map->CheckNeighborTile(temp.mapPos, 1);

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

	// Init first area
	vector<Tile> area;

	area.push_back(tileStack.top());

	// Add first area to area vector
	ret.push_back(area);

	groundTiles.del(groundTiles.At(0));

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

			vector<Tile> area;

			area.push_back(tileStack.top());

			ret.push_back(area);

			groundTiles.del(groundTiles.At(randNum));
		}
	}

	system("cls");

	// Delete area smaller 2
	for (int i = 0, size = ret.size(); i < size; i++)
	{
		if (ret[i].size() < 2)
		{
			for (int j = 0; j < ret[i].size(); j++) map->tiles[ret[i][j].mapPos.y * map->col + ret[i][j].mapPos.x].type = 1;
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

	for (int i = 0; i < areas.size(); i++)
	{
		biggest.second = areas[i].size() > biggest.first ? i : biggest.second;
	}

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

			map->tiles[secondPos.y * map->col + secondPos.x].type = 0;
		}
		// Open the path Y
		for (int i = 0; i < y; i++)
		{
			secondPos.y -= dir.y;

			map->tiles[secondPos.y * map->col + secondPos.x].type = 0;
		}
	}
}

#pragma endregion

//------------------------------------------------------------------------------------------------------------------------------

#pragma region Seek

void MapGenerator::InitSeek(int eSeek)
{
	uint seek;

	// If exist eSeek, chage seek to decrypted eSeek
	if (eSeek != 0) seek = Decrypt(eSeek);
	// Else init seek with time
	else seek = time(NULL);

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

#pragma endregion