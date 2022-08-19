#include "LevelEditorScene.h"

#include"../Utils/Utils.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../Managers/AllManagerHeadders.h"
#include"../UI/UI Core/UI_CoreHeadders.h"
#include"../UI/UI Widget/WidgetHeadders.h"
#include"../SceneCore/SceneManager.h"
#include"../MyScenes/AllSceneHeadders.h"


#include<assert.h>



LevelEditorScene::LevelEditorScene(Renderer& renderer, const Scene_Enum& sceneEnum)
	:
	Scene(renderer, sceneEnum),
	m_saveSelection()
{
}

void LevelEditorScene::SetSaveSelection(const SaveSelection& saveSelectionArg)
{
	this->m_saveSelection = saveSelectionArg;
}

void LevelEditorScene::Init()
{
/*
Toggle that moves to editor state if in world and world state if in editor

	-> if in editor state show list box and grid
	

Save button that saves level to file

Reload button that reloads the level


*/
	/*
	Entity Selector
	*/
	m_UI_Manager.AddWidget("EntityListBox", std::make_shared<ListBoxWidget>());
	std::shared_ptr<ListBoxWidget> entityListBoxWidget{ m_UI_Manager.GetWidget<ListBoxWidget>("EntityListBox") };

	//configure size and position of list box
	m_ecsEngine.GetComponent<EventComponent>(entityListBoxWidget->GetWidgetID())->m_eventRect = IntRect(0, 0, 400, 800);
	m_ecsEngine.GetComponent<TransformComponent>(entityListBoxWidget->GetWidgetID())->position = Vector2f(1000.0f, 20.0f);

		//set horizontal offset
		entityListBoxWidget->SetHeaddingOffset("\t\t\t\t  ");
		entityListBoxWidget->SetSubHeaddingOffset("\t");
		entityListBoxWidget->SetTextElementOffset("\t\t\t\t\t\t\t\t");

			//add list box c
			entityListBoxWidget->AddHeadding("Entity List Box", this->m_renderManager, this->m_UI_Manager, this->m_ecsEngine);
	
				entityListBoxWidget->AddSubHeadding("Editor:", this->m_renderManager, this->m_UI_Manager, this->m_ecsEngine);
					CreateTextWidgetAndBindToPrefabEntity(entityListBoxWidget, "Person Placement", m_prefabEngine.GetTaggedEntities("PersonPlacement")[0], "PersonPlacement");

				entityListBoxWidget->AddSubHeadding("Tiles:", this->m_renderManager, this->m_UI_Manager, this->m_ecsEngine);
					CreateTextWidgetAndBindToPrefabEntity(entityListBoxWidget, "Grass", m_prefabEngine.GetTaggedEntities("GrassTile")[0], "GrassTile");
					CreateTextWidgetAndBindToPrefabEntity(entityListBoxWidget, "Stones", m_prefabEngine.GetTaggedEntities("Stones")[0], "Stones");
					CreateTextWidgetAndBindToPrefabEntity(entityListBoxWidget, "Mud", m_prefabEngine.GetTaggedEntities("Mud")[0], "Mud");

				entityListBoxWidget->AddSubHeadding("Static World Objects:", this->m_renderManager, this->m_UI_Manager, this->m_ecsEngine);
					CreateTextWidgetAndBindToPrefabEntity(entityListBoxWidget, "Lump Of Wood", m_prefabEngine.GetTaggedEntities("LumpOfWood")[0], "LumpOfWood");
					CreateTextWidgetAndBindToPrefabEntity(entityListBoxWidget, "Rock 1 - Collidable", m_prefabEngine.GetTaggedEntities("Rock1")[0], "Rock1");
					CreateTextWidgetAndBindToPrefabEntity(entityListBoxWidget, "Green Tree", m_prefabEngine.GetTaggedEntities("GreenTree")[0], "GreenTree");
					CreateTextWidgetAndBindToPrefabEntity(entityListBoxWidget, "Alpha Thing", m_prefabEngine.GetTaggedEntities("SpriteThing")[0], "SpriteThing");

				entityListBoxWidget->AddSubHeadding("World Spawners:", this->m_renderManager, this->m_UI_Manager, this->m_ecsEngine);
					CreateTextWidgetAndBindToPrefabEntity(entityListBoxWidget, "Silver Knight", m_prefabEngine.GetTaggedEntities("SilverKnight_Spawner")[0], "SilverKnight_Spawner");
					CreateTextWidgetAndBindToPrefabEntity(entityListBoxWidget, "Bommer", m_prefabEngine.GetTaggedEntities("Bommer_Spawner")[0], "Bommer_Spawner");


				entityListBoxWidget->AddSubHeadding("Animated World Objects:", this->m_renderManager, this->m_UI_Manager, this->m_ecsEngine);
					CreateTextWidgetAndBindToPrefabEntity(entityListBoxWidget, "Fire Torch", m_prefabEngine.GetTaggedEntities("AnimatedFireThing")[0], "AnimatedFireThing");

	m_ecsEngine.GetSystem<AnimationSystem>()->LoadAnimationsToSystemFromSystem(m_prefabEngine);

	/*
	Buttons
	*/

	//editor/game button toggle

	std::shared_ptr<ButtonWidget> viewToggleButton{ std::make_shared<ButtonWidget>() };
	this->m_UI_Manager.AddWidget("ViewToggleButton", viewToggleButton);
	viewToggleButton->InitializeButton(this->m_ecsEngine, { 0.0f, 0.0f }, { 120, 35 }, HAPI_TColour(24, 15, 13, 255), HAPI_TColour(20, 168, 72, 255), "Toggle View", { 20, 8 }, 15);
	//configure back button

	std::shared_ptr<ButtonControllerScript> buttonControllerScript = std::static_pointer_cast<ButtonControllerScript>(this->m_ecsEngine.GetComponent<ScriptComponent>(viewToggleButton->GetWidgetID())->m_script);
	assert(buttonControllerScript);

	buttonControllerScript->m_idleStateButtonColour = HAPI_TColour(24, 15, 13, 255);
	buttonControllerScript->m_idleStateTextColour = HAPI_TColour(172, 168, 72, 255);

	buttonControllerScript->m_hoverStateButtonColour = HAPI_TColour(60, 39, 35, 255);
	buttonControllerScript->m_hoverStateTextColour = HAPI_TColour(210, 206, 128, 255);

	buttonControllerScript->m_clickingStateButtonColour = HAPI_TColour(107, 99, 99, 255);
	buttonControllerScript->m_clickingStateTextColour = HAPI_TColour(255, 255, 255, 255);

	//save button
	std::shared_ptr<ButtonWidget> saveLevelButton{ std::make_shared<ButtonWidget>() };
	this->m_UI_Manager.AddWidget("SaveLevelButton", saveLevelButton);
	saveLevelButton->InitializeButton(this->m_ecsEngine, { 120.0f, 0.0f }, { 120, 35 }, HAPI_TColour(24, 15, 13, 255), HAPI_TColour(20, 168, 72, 255), "Save Level", { 20, 8 }, 15);
	//configure back button

	buttonControllerScript = std::static_pointer_cast<ButtonControllerScript>(this->m_ecsEngine.GetComponent<ScriptComponent>(saveLevelButton->GetWidgetID())->m_script);
	assert(buttonControllerScript);

	buttonControllerScript->m_idleStateButtonColour = HAPI_TColour(24, 15, 13, 255);
	buttonControllerScript->m_idleStateTextColour = HAPI_TColour(172, 168, 72, 255);

	buttonControllerScript->m_hoverStateButtonColour = HAPI_TColour(60, 39, 35, 255);
	buttonControllerScript->m_hoverStateTextColour = HAPI_TColour(210, 206, 128, 255);

	buttonControllerScript->m_clickingStateButtonColour = HAPI_TColour(107, 99, 99, 255);
	buttonControllerScript->m_clickingStateTextColour = HAPI_TColour(255, 255, 255, 255);
		

	//reload button
	std::shared_ptr<ButtonWidget> loadLevelButton{ std::make_shared<ButtonWidget>() };
	this->m_UI_Manager.AddWidget("LoadLevelButton", loadLevelButton);
	loadLevelButton->InitializeButton(this->m_ecsEngine, { 240.0f, 0.0f }, { 120, 35 }, HAPI_TColour(24, 15, 13, 255), HAPI_TColour(20, 168, 72, 255), "Load Level", { 20, 8 }, 15);
	//configure back button
	
	buttonControllerScript = std::static_pointer_cast<ButtonControllerScript>(this->m_ecsEngine.GetComponent<ScriptComponent>(loadLevelButton->GetWidgetID())->m_script);
	assert(buttonControllerScript);

	buttonControllerScript->m_idleStateButtonColour = HAPI_TColour(24, 15, 13, 255);
	buttonControllerScript->m_idleStateTextColour = HAPI_TColour(172, 168, 72, 255);

	buttonControllerScript->m_hoverStateButtonColour = HAPI_TColour(60, 39, 35, 255);
	buttonControllerScript->m_hoverStateTextColour = HAPI_TColour(210, 206, 128, 255);

	buttonControllerScript->m_clickingStateButtonColour = HAPI_TColour(107, 99, 99, 255);
	buttonControllerScript->m_clickingStateTextColour = HAPI_TColour(255, 255, 255, 255);

	//back button
	std::shared_ptr<ButtonWidget> backButton{ std::make_shared<ButtonWidget>() };
	this->m_UI_Manager.AddWidget("BackButton", backButton);
	backButton->InitializeButton(this->m_ecsEngine, { 360.0f, 0.0f }, { 120, 35 }, HAPI_TColour(24, 15, 13, 255), HAPI_TColour(20, 168, 72, 255), "Back", { 40, 8 }, 15);
	//configure back button

	buttonControllerScript = std::static_pointer_cast<ButtonControllerScript>(this->m_ecsEngine.GetComponent<ScriptComponent>(backButton->GetWidgetID())->m_script);
	assert(buttonControllerScript);

	buttonControllerScript->m_idleStateButtonColour = HAPI_TColour(24, 15, 13, 255);
	buttonControllerScript->m_idleStateTextColour = HAPI_TColour(172, 168, 72, 255);

	buttonControllerScript->m_hoverStateButtonColour = HAPI_TColour(60, 39, 35, 255);
	buttonControllerScript->m_hoverStateTextColour = HAPI_TColour(210, 206, 128, 255);

	buttonControllerScript->m_clickingStateButtonColour = HAPI_TColour(107, 99, 99, 255);
	buttonControllerScript->m_clickingStateTextColour = HAPI_TColour(255, 255, 255, 255);

}

void LevelEditorScene::DeInit()
{
	this->m_sceneClosed = true;
	this->m_saveLoaded = false;
	UI_Manager::SetEntityEngaged(-1);

	this->m_UI_Manager.GetWidget<ButtonWidget>("ViewToggleButton")->ResetWidget(this->m_ecsEngine);
	this->m_UI_Manager.GetWidget<ButtonWidget>("SaveLevelButton")->ResetWidget(this->m_ecsEngine);
	this->m_UI_Manager.GetWidget<ButtonWidget>("LoadLevelButton")->ResetWidget(this->m_ecsEngine);
	this->m_UI_Manager.GetWidget<ButtonWidget>("BackButton")->ResetWidget(this->m_ecsEngine);


}

void LevelEditorScene::Render(Renderer& renderer)
{
	if (m_editorMode)
	{
		Vector2i tileSize{64, 64};
		renderer.DrawEditorGrid(m_ecsEngine.GetComponent<TransformComponent>(m_ecsEngine.GetTaggedEntities("Camera")[0])->position.ToType<int>(), tileSize, HAPI_TColour{ 60, 0, 0, 0 }, false, true);

	}
	this->m_renderManager.Render_RenderComponents(this->m_ecsEngine, renderer, this->GetCameraOffset());
	if (m_editorMode)
	{
		//if in editor mode then display position of mouse next to mouse relative to the world
		std::shared_ptr<TransformComponent> camersTransform{ m_ecsEngine.GetComponent<TransformComponent>(m_ecsEngine.GetTaggedEntities("Camera")[0]) };

		HAPI.RenderText(Mouse.x + 40, Mouse.y + 20, HAPI_TColour::WHITE, "x :" + std::to_string(camersTransform->position.x + Mouse.x) + ", y :" + std::to_string(camersTransform->position.y + Mouse.y));
	}

}

void LevelEditorScene::Update(const float& deltaTime, SceneManager& sceneManager)
{
	Utils::LoadLevel(this->m_saveLoaded, this->fileName, this->m_saveSelection, this->m_ecsEngine, this->m_prefabEngine, this->m_renderManager, this->m_UI_Manager);
	this->m_ecsEngine.UpdateECS(deltaTime, this->m_renderManager, this->m_UI_Manager, this->m_prefabEngine);
		
	ControlCamera(deltaTime);

	PlaceEntities();

}


void LevelEditorScene::EnableEditor()
{
	this->m_ecsEngine.GetSystem<MovementSystem>()->DisableSystem(this->m_ecsEngine);
	this->m_ecsEngine.GetSystem<AnimationSystem>()->DisableSystem(this->m_ecsEngine);
	this->m_ecsEngine.GetSystem<EditorSystem>()->EnableSystem(this->m_ecsEngine);

}

void LevelEditorScene::DisableEditor()
{
	this->m_ecsEngine.GetSystem<MovementSystem>()->EnableSystem(this->m_ecsEngine);
	this->m_ecsEngine.GetSystem<AnimationSystem>()->EnableSystem(this->m_ecsEngine);
	this->m_ecsEngine.GetSystem<EditorSystem>()->DisableSystem(this->m_ecsEngine);

}


void LevelEditorScene::PlaceEntities()
{
	if (m_editorMode)
	{
		if ((!this->isClicking && Mouse.leftButtonDown && UI_Manager::GetEntityEngaged() == -1) ||
			(!this->isClicking && Mouse.leftButtonDown && UI_Manager::GetEntityEngaged() != -1 &&
				this->m_ecsEngine.GetComponent<RenderComponent>(UI_Manager::GetEntityEngaged())->m_renderLayer != RenderComponent::RenderLayer::UI)
			)
		{
			this->isClicking = true;

			for (const std::pair<uint32_t, std::pair<uint32_t, std::string>>& widgetPrefabRecord : this->m_widgetID_To_prefabEntityID_And_TagName)
			{
				std::shared_ptr<EventComponent> textElementEventComponent{ m_ecsEngine.GetComponent<EventComponent>(widgetPrefabRecord.first) };
				if (textElementEventComponent->m_tagComponent)
				{
					if (this->m_prefabEngine.GetComponent<EditorComponent>(widgetPrefabRecord.second.first)->m_oneInstanceOnly &&
						this->m_ecsEngine.GetTaggedEntities(widgetPrefabRecord.second.second).size())
						continue;

					uint32_t newInstansiatedEntityID{ m_ecsEngine.InstantiateEntity() };
					UI_Manager::SetEntityEngaged(static_cast<int32_t>(newInstansiatedEntityID));
					m_ecsEngine.LoadEntityFromSystemIntoSystem(newInstansiatedEntityID, widgetPrefabRecord.second.first, m_prefabEngine);

					std::shared_ptr<TransformComponent> newInstansiatedEntityTransform{ m_ecsEngine.GetComponent<TransformComponent>(newInstansiatedEntityID) };
					std::shared_ptr<SpriteComponent> newInstansiatedEntitySprite{ m_ecsEngine.GetComponent<SpriteComponent>(newInstansiatedEntityID) };

					newInstansiatedEntityTransform->position.x = static_cast<float>(GetCameraOffset().Invert().x + HAPI.GetMouseData().x - newInstansiatedEntitySprite->m_sprite.m_textureRef->m_textureSize.x / 2);
					newInstansiatedEntityTransform->position.y = static_cast<float>(GetCameraOffset().Invert().y + HAPI.GetMouseData().y - newInstansiatedEntitySprite->m_sprite.m_textureRef->m_textureSize.y / 2);

					m_renderManager.AddEntityIdToRenderLayer(this->m_ecsEngine, newInstansiatedEntityID, this->m_ecsEngine.GetComponent<RenderComponent>(newInstansiatedEntityID)->m_renderLayer);
					m_ecsEngine.TagEntity(widgetPrefabRecord.second.second, newInstansiatedEntityID);

					break;
				}
			}
		}
		if (this->isClicking && !Mouse.leftButtonDown)
			this->isClicking = false;
	}
}

void LevelEditorScene::ProcessUI(SceneManager& sceneManager)
{
	//if change view
	if (this->m_UI_Manager.GetWidget<ButtonWidget>("ViewToggleButton")->OneStepClick(this->m_ecsEngine))
	{
		if (this->m_editorMode)
		{
			this->m_editorMode = false;
			//remove UI elements

			this->m_UI_Manager.GetWidget<ListBoxWidget>("EntityListBox")->HideWidget(this->m_ecsEngine, this->m_renderManager);
			this->m_UI_Manager.GetWidget<ButtonWidget>("BackButton")->HideWidget(this->m_ecsEngine, this->m_renderManager);
			this->m_UI_Manager.GetWidget<ButtonWidget>("LoadLevelButton")->HideWidget(this->m_ecsEngine, this->m_renderManager);
			this->m_UI_Manager.GetWidget<ButtonWidget>("SaveLevelButton")->HideWidget(this->m_ecsEngine, this->m_renderManager);

		}
		else
		{
			this->m_editorMode = true;
			//append UI elements
			this->m_UI_Manager.GetWidget<ListBoxWidget>("EntityListBox")->ShowWidget(this->m_ecsEngine, this->m_renderManager);
			this->m_UI_Manager.GetWidget<ButtonWidget>("BackButton")->ShowWidget(this->m_ecsEngine, this->m_renderManager);
			this->m_UI_Manager.GetWidget<ButtonWidget>("LoadLevelButton")->ShowWidget(this->m_ecsEngine, this->m_renderManager);
			this->m_UI_Manager.GetWidget<ButtonWidget>("SaveLevelButton")->ShowWidget(this->m_ecsEngine, this->m_renderManager);
		}
	}

	if (m_editorMode)
		EnableEditor();
	else
		DisableEditor();

	if (this->m_UI_Manager.GetWidget<ButtonWidget>("SaveLevelButton")->OneStepClick(this->m_ecsEngine))
		Utils::SaveEcsToXML(fileName, this->m_ecsEngine, this->m_prefabEngine);

	if (this->m_UI_Manager.GetWidget<ButtonWidget>("LoadLevelButton")->OneStepClick(this->m_ecsEngine))
		Utils::Load_XML_Into_ECS(fileName, this->m_ecsEngine, this->m_prefabEngine, this->m_renderManager);

	if (this->m_UI_Manager.GetWidget<ButtonWidget>("BackButton")->OneStepClick(this->m_ecsEngine))
	{
		sceneManager.PopScene();
		sceneManager.LoadScene("SaveSelectionScene");
		return;
	}

}

void LevelEditorScene::CreateTextWidgetAndBindToPrefabEntity(const std::shared_ptr<ListBoxWidget>& entityListBox, const std::string& textElementName, const uint32_t& prefabEntityID, const std::string& tagName)
{
	//create the text widget to represet the entity in the list box
	uint32_t textWidgetID{entityListBox->AddTextElement(textElementName, this->m_renderManager, this->m_UI_Manager, this->m_ecsEngine)};
	this->m_widgetID_To_prefabEntityID_And_TagName.emplace_back(std::make_pair(textWidgetID, std::make_pair(prefabEntityID, tagName)));
}

