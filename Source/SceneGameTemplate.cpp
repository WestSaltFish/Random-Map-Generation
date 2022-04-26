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

	SceneGame::Start();

	return true;
}

void SceneGameTemplate::PreUpdate()
{

	SceneGame::PreUpdate();
}

void SceneGameTemplate::Update()
{
	SceneGame::Update();

	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_DOWN)
	{
		//Do something...
	}
}

void SceneGameTemplate::PostUpdate()
{
	App->render->AddRectRenderQueue(rec, 250, 80, 80, 255);

	for (int i = 0; i < 100; i++)
	{
		App->render->AddRectRenderQueue({ 0, 20 * i, 1280, 1 }, 0, 0, 0, 100);
		App->render->AddRectRenderQueue({ 20*i, 0, 1, 720 }, 0, 0, 0, 100);
	}

	SceneGame::PostUpdate();
}

void SceneGameTemplate::CleanUp()
{
	SceneGame::CleanUp();
}
