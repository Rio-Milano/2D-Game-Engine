#ifndef CONFIGURESCENE_H
#define CONFIGURESCENE_H

#include"../SceneCore/Scene.h"

class ConfigureScene final : public Scene
{
public:
	ConfigureScene(Renderer& renderer, const Scene_Enum& sceneEnum);
	~ConfigureScene() = default;

	void Init() override;
	void DeInit() override;

	void Render(Renderer& renderer) override;

private:
	void ProcessUI(SceneManager& sceneManager)override;
	void Update(const float& deltaTime, SceneManager& sceneManager) override;

	std::vector<std::string> m_listOfDifficulties{"Easy", "Moderate", "Hard"};
	int m_difficultyIndex{1};
};


#endif