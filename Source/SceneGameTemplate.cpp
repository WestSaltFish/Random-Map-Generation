#include "SceneGameTemplate.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleAudio.h"

SceneGameTemplate::SceneGameTemplate()
{
}

bool SceneGameTemplate::Start()
{
	// IMPORTANT: Init the parameters of this scene
	// because when we restart the scene, these values should be set by default

	// map = mapGenerator.GenerateDungeonMap(30, 30, 200, 20, 20);

	map = mapGenerator.GenerateDungeonMapCA(50, 50, 10, 10);

	SceneGame::Start();

	return true;
}

void SceneGameTemplate::PreUpdate()
{
	SceneGame::PreUpdate();
}

void SceneGameTemplate::Update()
{
	if (App->input->keys[SDL_SCANCODE_A] == KEY_DOWN)
	{
		mapGenerator.TestDungeonMapBacktrack();
	}
	if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT)
	{
		mapGenerator.TestDungeonMapBacktrack();
	}
	if (App->input->keys[SDL_SCANCODE_G] == KEY_DOWN)
	{
		map->drawGrid = !map->drawGrid;
	}
	if (App->input->keys[SDL_SCANCODE_R] == KEY_DOWN)
	{
		RELEASE(map);
		//map = mapGenerator.GenerateDungeonMap(15, 15, 390, 30, 30);
		map = mapGenerator.GenerateDungeonMapCA(50, 50, 5, 5);
	}

	SceneGame::Update();
}

void SceneGameTemplate::PostUpdate()
{
	map->PostUpdate();

	SceneGame::PostUpdate();
}

void SceneGameTemplate::CleanUp()
{
	map->tiles.clear();

	RELEASE(map);

	SceneGame::CleanUp();
}
