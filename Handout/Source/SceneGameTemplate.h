#ifndef __SCENE_GAME_TEMPLATE_H__
#define __SCENE_GAME_TEMPLATE_H__

#include "SceneGame.h"
#include "MapGenerator.h"

// Test class, should be removed at the start of the project
class SceneGameTemplate : public SceneGame
{
public:

	SceneGameTemplate();

	bool Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

private:
	MapGenerator mapGenerator;

	Map* map = nullptr;
};
#endif
