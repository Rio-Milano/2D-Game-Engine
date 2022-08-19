




#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <HAPI_lib.h>
#include<list>
#include<time.h>
using namespace HAPISPACE;

#include"../SceneCore/Scene.h"
#include"../DataStructures/Vector.h"
#include"../Macros/Colors.h"

enum class SaveSelection;

class MainScene final : public Scene
{
public:

	MainScene(Renderer& renderer, const Scene_Enum& sceneEnum);
	~MainScene() = default;

	void SetSaveSelection(const SaveSelection& saveSelectionArg);

	void Init() override;
	void DeInit() override;

	void Render(Renderer& renderer) override;

private:
	void ProcessUI(SceneManager& sceneManager)override;
	void Update(const float& deltaTime, SceneManager& sceneManager) override;

	SaveSelection m_saveSelection;
	bool m_saveLoaded{ false };
	bool m_paused{ false };
	std::string fileName{ "" };






};

#endif
