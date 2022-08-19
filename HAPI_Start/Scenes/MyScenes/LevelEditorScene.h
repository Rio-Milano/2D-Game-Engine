#ifndef LEVELEDITORSCENE_H
#define LEVELEDITORSCENE_H

#include"../SceneCore/Scene.h"

class ListBoxWidget;
enum class SaveSelection;

class LevelEditorScene final : public Scene
{
public:
	LevelEditorScene(Renderer& renderer, const Scene_Enum& sceneEnum);
	~LevelEditorScene() = default;
	
	void SetSaveSelection(const SaveSelection& saveSelectionArg); 

	void Init() override;
	void DeInit() override;

	void Render(Renderer& renderer) override;
	
private:
	void Update(const float& deltaTime, SceneManager& sceneManager) override;

	void EnableEditor();
	void DisableEditor();

	void PlaceEntities();
	void ProcessUI(SceneManager& sceneManager) override;

	void CreateTextWidgetAndBindToPrefabEntity(const std::shared_ptr<ListBoxWidget>& entityListBox, const std::string& textElementName, const uint32_t& prefabEntityID, const std::string& tagName);

	SaveSelection m_saveSelection;
	bool m_saveLoaded{ false };

	std::string fileName{ "" };

	/*
	* Widget ID -> {PrefabEntityID, TagName}
	*/
	std::vector<std::pair<uint32_t, std::pair<uint32_t, std::string>>> m_widgetID_To_prefabEntityID_And_TagName;

	bool isClicking{ false };



};

#endif