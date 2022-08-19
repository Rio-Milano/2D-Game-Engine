
#ifndef STARTSCENE_H
#define STARTSCENE_H

#include"../SceneCore/Scene.h"

class StartScene final: public Scene
{
public:
	StartScene(Renderer& renderer, const Scene_Enum& sceneEnum);
	~StartScene() = default;

	void Init() override;
	void DeInit() override;

	void Render(Renderer& renderer) override;

private:
	void ProcessUI(SceneManager& sceneManager)override;
	void Update(const float& deltaTime, SceneManager& sceneManager) override;

	bool m_startBackgroundMusic{true};
};

#endif