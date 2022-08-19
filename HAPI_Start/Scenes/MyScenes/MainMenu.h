#ifndef MAINMENU_H
#define MAINMENU_H

#include"../Scenes/SceneCore/Scene.h"

class MainMenu final : public Scene
{
public:
	MainMenu(Renderer& renderer, const Scene_Enum& sceneEnum);
	~MainMenu() = default;

	void Init() override;
	void DeInit() override;

	void Render(Renderer & renderer) override;

private:
	void ProcessUI(SceneManager& sceneManager)override;
	void Update(const float& deltaTime, SceneManager & sceneManager) override;
};


#endif