#ifndef SCENE_ENUM_H
#define SCENE_ENUM_H

enum class Scene_Enum : unsigned int
{
	StartScene = 0,
	MainScene = 1 << 0,
	MainMenuScene = 1 << 1,
	SaveSelectionScene = 1 << 2,
	ConfigureScene = 1 << 3,
	GameCycling = 1 << 4
};

#endif