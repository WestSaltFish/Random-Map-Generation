#include "Map.h"
#include "ModuleRender.h"

Map::Map(int row, int col, int tileWidth, int tileHeight) :row(row), col(col), tileWidth(tileWidth), tileHeight(tileHeight)
{	
	mapWidth = col * tileWidth;
	mapHeight = row * tileHeight;
}

Map::~Map()
{
}

int Map::CheckTileType(iPoint mapPos)
{
	if (mapPos.x < 0 || mapPos.y < 0 || mapPos.x >(col - 1) || mapPos.y >(row - 1)) return -1;

	return tiles[mapPos.y * col + mapPos.x].type;
}

int Map::CheckNeighborTile(iPoint mapPos, int type)
{
	int ret = 0;

	iPoint dir[8] = { {1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {-1,-1}, {-1,1}, {1,-1} };

	for (int i = 0; i < 8; i++)
	{
		iPoint checkPos = mapPos + dir[i];

		if (CheckTileType(checkPos) == type) ret++;
	}

	return ret;
}

bool Map::CheckBorder(iPoint mapPos)
{
	if (mapPos.x <= 0 || mapPos.y <= 0 || mapPos.x >= (col - 1) || mapPos.y >= (row - 1)) return true;

	return false;
}

void Map::PostUpdate()
{
	// Draw freeSpace JUST FOR TESTING!!!
	for (int i = 0, count = posibleDir_t.size(); i < count; ++i)
	{
		Tile t = tiles[posibleDir_t[i].y * col + posibleDir_t[i].x];

		App->render->AddRectRenderQueue({ t.worldPos.x, t.worldPos.y, tileWidth, tileHeight }, { 80, 165, 255, 255 });
	}
	// Draw current tile
	App->render->AddRectRenderQueue({ currentTile.worldPos.x, currentTile.worldPos.y, currentTile.width, currentTile.height }, { 255, 190, 80, 255 },2);

	// Draw tiles
	for (int i = 0, count = tiles.count(); i < count; ++i)
	{
		Tile t = tiles[i];

		if (t.type != 1) continue;

		App->render->AddRectRenderQueue({ t.worldPos.x, t.worldPos.y, t.width, t.height }, { 255, 80, 80, 255 });
	}

	if (!drawGrid) return;

	// Draw grid col
	for (int i = 0; i <= col; i++)
	{
		App->render->AddRectRenderQueue({ i * tileWidth, 0 , 1, mapHeight }, { 100, 100, 100, 255 });
	}

	// Draw grid row
	for (int i = 0; i <= row; i++)
	{
		App->render->AddRectRenderQueue({ 0, i * tileHeight, mapWidth, 1 }, { 100, 100, 100, 255 });
	}
}
