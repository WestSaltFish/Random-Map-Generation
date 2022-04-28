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

	map = mapGenerator.GenerateMap(3, 3, 30, 30);

	SceneGame::Start();

	return true;
}

void SceneGameTemplate::PreUpdate()
{
	SceneGame::PreUpdate();
}

void SceneGameTemplate::Update()
{
	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_DOWN)
	{
		//Do something...
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
