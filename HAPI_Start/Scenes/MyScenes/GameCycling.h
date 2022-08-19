#ifndef GAMECYCLING_H
#define GAMECYCLING_H

#include"../SceneCore/Scene.h"

class GameCycling final : public Scene
{
public:
	GameCycling(Renderer& renderer, const Scene_Enum& sceneEnum);
	~GameCycling() = default;

	void Init() override;
	void DeInit() override;

	void Render(Renderer& renderer) override;

	std::string gameResult{ "" };
	int playerScore{0};
private:
	void ProcessUI(SceneManager& sceneManager)override;
	void Update(const float& deltaTime, SceneManager& sceneManager) override;

	
};


#endif