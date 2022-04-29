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

bool Map::CheckTile(iPoint mapPos)
{
	if (mapPos.x < 0 || mapPos.y < 0 || mapPos.x > (col-1) || mapPos.y > (row-1)) return true;

	for (int i = 0, count = tiles.count(); i < count; i++)
	{
		iPoint tilePos = tiles[i].mapPos;

		if (tilePos == mapPos) return true;
	}

	return false;
}

void Map::PostUpdate()
{
	// Draw freeSpace JUST FOR TESTING!!!
	for (int i = 0, count = freeSpace_t.count(); i < count; ++i)
	{
		Tile t = freeSpace_t[i];

		App->render->AddRectRenderQueue({ t.worldPos.x, t.worldPos.y, t.width, t.height }, { 80, 165, 255, 255 });
	}
	App->render->AddRectRenderQueue({ currentTile_t.worldPos.x, currentTile_t.worldPos.y, currentTile_t.width, currentTile_t.height }, { 255, 190, 80, 255 },2);

	// Draw tiles
	for (int i = 0, count = tiles.count(); i < count; ++i)
	{
		Tile t = tiles[i];

		if (t.type == 0) continue;

		App->render->AddRectRenderQueue({ t.worldPos.x, t.worldPos.y, t.width, t.height }, { 255, 80, 80, 255 });
	}

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
