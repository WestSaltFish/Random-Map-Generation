#include "Map.h"
#include "ModuleRender.h"

Map::Map(int row, int col, int gridWidth, int gridHeight) :row(row), col(col), gridWidth(gridWidth), gridHeight(gridHeight)
{	
	mapWidth = col * gridWidth;
	mapHeight = row * gridHeight;
}

Map::~Map()
{
}

void Map::PostUpdate()
{
	// Draw tiles;
	for (int i = 0, count = tiles.count(); i < count; ++i)
	{
		Tile t = tiles[i];

		if (t.weight == 0) continue;

		//SDL_Rect rec = { t.worldPos.x, t.worldPos.y, t.width, t.height };
		//SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
		//SDL_SetRenderDrawColor(App->render->renderer, 255, 80, 80, 255);
		//SDL_RenderFillRect(App->render->renderer, &rec);

		App->render->AddRectRenderQueue({ t.worldPos.x, t.worldPos.y, t.width, t.height }, { 255, 80, 80, 255 });
	}

	// Draw grid row
	for (int i = 0; i <= row; i++)
	{
		App->render->AddRectRenderQueue({ 0, i * gridWidth, mapWidth, 1 }, { 100, 100, 100, 255 });
	}

	// Draw grid col
	for (int i = 0; i <= col; i++)
	{
		App->render->AddRectRenderQueue({ i * gridHeight, 0 , 1, mapHeight }, { 100, 100, 100, 255 });
	}
}
