#ifndef SAVESELECTIONSCENE_H
#define SAVESELECTIONSCENE_H

#include"../SceneCore/Scene.h"


enum class SaveSelection
{
	Save1,
	Save2,
	Save3
};

class SaveSelectionScene final : public Scene
{
public:

	SaveSelectionScene(Renderer& renderer, const Scene_Enum& sceneEnum);
	~SaveSelectionScene() = default;

	void Init() override;
	void DeInit() override;

	void Render(Renderer& renderer) override;

	void SetSceneToLoadUponSaveSelection(const std::string& sceneName) { this->m_sceneToLoadUponSaveSelection = sceneName; };
	const std::string& GetSceneToLoadUponSceneSelection()const { return this->m_sceneToLoadUponSaveSelection; };

private:
	void ProcessUI(SceneManager& sceneManager)override;
	void SceneSelectionButtonLogic(SceneManager& sceneManager, const SaveSelection& saveSelection);
	void Update(const float& deltaTime, SceneManager& sceneManager) override;

	std::string m_sceneToLoadUponSaveSelection{""};
};

#endif