#include "SceneGameTemplate.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleAudio.h"

int mapMode = 1;

const bool active = true;

SceneGameTemplate::SceneGameTemplate()
{
}

bool SceneGameTemplate::Start()
{
	// IMPORTANT: Init the parameters of this scene
	// because when we restart the scene, these values should be set by default

	if (!active) return true;
	
	if (mapMode == 0) map = mapGenerator.GenerateDungeonMap(5, 5, 10, 50, 50);
	else if (mapMode == 1) map = mapGenerator.GenerateDungeonMapCA(40, 20, 10, 10);

	string debugMode = mapGenerator.debugMode ? "On" : "Off";
	cout << "Debug Mode: " << debugMode << endl;

	SceneGame::Start();

	return true;
}

void SceneGameTemplate::PreUpdate()
{
	SceneGame::PreUpdate();
}

void SceneGameTemplate::Update()
{
	if (!active) return;

	if (App->input->keys[SDL_SCANCODE_A] == KEY_DOWN)
	{
		if (mapMode == 0) mapGenerator.TestDungeonMapBacktrack();
		else if (mapMode == 1) mapGenerator.TestGenerateDungeonMapCA();
	}
	if (App->input->keys[SDL_SCANCODE_S] == KEY_REPEAT)
	{
		if (mapMode == 0) mapGenerator.TestDungeonMapBacktrack();
	}
	if (App->input->keys[SDL_SCANCODE_G] == KEY_DOWN)
	{
		map->drawGrid = !map->drawGrid;
	}
	if (App->input->keys[SDL_SCANCODE_R] == KEY_DOWN)
	{
		RELEASE(map);
		if (mapMode == 0) map = mapGenerator.GenerateDungeonMap(5, 5, 10, 50, 50);
		else if (mapMode == 1) map = mapGenerator.GenerateDungeonMapCA(40, 20, 10, 10);
	}
	if (App->input->keys[SDL_SCANCODE_D] == KEY_DOWN)
	{
		system("cls");
		mapGenerator.debugMode = !mapGenerator.debugMode;
		string debugMode = mapGenerator.debugMode ? "On" : "Off";
		cout << "Debug Mode: " << debugMode << endl;
	}

	SceneGame::Update();
}

void SceneGameTemplate::PostUpdate()
{
	if (!active) return;

	map->PostUpdate();

	SceneGame::PostUpdate();
}

void SceneGameTemplate::CleanUp()
{
	RELEASE(map);

	SceneGame::CleanUp();
}
