#include "SaveSelectionScene.h"

#include"../Managers/AllManagerHeadders.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../UI/UI Widget/WidgetHeadders.h"
#include"../SceneCore/SceneManager.h"
#include"../MyScenes/AllSceneHeadders.h"
#include<assert.h>
#include"../Managers/SoundManager.h"
#define SoundManager SoundManager::GetInstance()
SaveSelectionScene::SaveSelectionScene(Renderer& renderer, const Scene_Enum& sceneEnum)
	: Scene(renderer, sceneEnum)
{
}

void SaveSelectionScene::Init()
{
	std::shared_ptr<Texture> backgroundTexturePtr{ TextureManager.GetTexture("StartScreenBackground") };
	/*
	load game entities
	*/

	//load background entity
	uint32_t backgroundEntity{ this->m_ecsEngine.InstantiateEntity() };
	this->m_ecsEngine.AddComponent<TransformComponent>(backgroundEntity)->InitializeTransformComponent(Vector2f{ 0.0f, 0.0f });
	this->m_ecsEngine.AddComponent<SpriteComponent>(backgroundEntity)->m_sprite.InitializeSprite(backgroundTexturePtr, false, Effects::NoEffects);
	this->m_ecsEngine.AddComponent<RenderComponent>(backgroundEntity)->InitializeRenderComponent(RenderComponent::RenderConfigs::Sprite, RenderComponent::RenderLayer::Default, true, false);
	this->m_renderManager.AddEntityIdToRenderLayer(this->m_ecsEngine, backgroundEntity, RenderComponent::RenderLayer::Default);


	//scene title entity
	uint32_t menuTitleEntity{ this->m_ecsEngine.InstantiateEntity() };
	this->m_ecsEngine.AddComponent<TransformComponent>(menuTitleEntity)->InitializeTransformComponent(Vector2f{ 553.0f , 42.0f });
	this->m_ecsEngine.AddComponent<TextComponent>(menuTitleEntity)->InitializeTextComponent(HAPI_TColour(210, 206, 128, 255), "Save Selection", 80);
	this->m_ecsEngine.AddComponent<RenderComponent>(menuTitleEntity)->InitializeRenderComponent
		(
			RenderComponent::RenderConfigs::Text,
			RenderComponent::RenderLayer::UI,
			true,
			false
			);
	this->m_renderManager.AddEntityIdToRenderLayer(this->m_ecsEngine, menuTitleEntity, RenderComponent::RenderLayer::UI);

	//save 1 button
	std::shared_ptr<ButtonWidget> saveOneButton{ std::make_shared<ButtonWidget>() };
	this->m_UI_Manager.AddWidget("SaveOneButton", saveOneButton);
	saveOneButton->InitializeButton(this->m_ecsEngine, { 640.0f, 224.0f }, { 342, 60 }, HAPI_TColour(24, 15, 13, 255), HAPI_TColour(172, 168, 72, 255), "Save 1", { 120, 6 }, 37);
	//configure play button
	std::shared_ptr<ButtonControllerScript> buttonControllerScript{ std::static_pointer_cast<ButtonControllerScript>(this->m_ecsEngine.GetComponent<ScriptComponent>(saveOneButton->GetWidgetID())->m_script) };
	assert(buttonControllerScript);

	buttonControllerScript->m_idleStateButtonColour = HAPI_TColour(24, 15, 13, 255);
	buttonControllerScript->m_idleStateTextColour = HAPI_TColour(172, 168, 72, 255);

	buttonControllerScript->m_hoverStateButtonColour = HAPI_TColour(60, 39, 35, 255);
	buttonControllerScript->m_hoverStateTextColour = HAPI_TColour(210, 206, 128, 255);

	buttonControllerScript->m_clickingStateButtonColour = HAPI_TColour(107, 99, 99, 255);
	buttonControllerScript->m_clickingStateTextColour = HAPI_TColour(255, 255, 255, 255);

	//save 2 button
	std::shared_ptr<ButtonWidget> saveTwoButton{ std::make_shared<ButtonWidget>() };
	this->m_UI_Manager.AddWidget("SaveTwoButton", saveTwoButton);
	saveTwoButton->InitializeButton(this->m_ecsEngine, { 640.0f, 404.0f }, { 342, 60 }, HAPI_TColour(24, 15, 13, 255), HAPI_TColour(172, 168, 72, 255), "Save 2", { 120, 6 }, 37);
	//configure editor button
	buttonControllerScript = std::static_pointer_cast<ButtonControllerScript>(this->m_ecsEngine.GetComponent<ScriptComponent>(saveTwoButton->GetWidgetID())->m_script);
	assert(buttonControllerScript);

	buttonControllerScript->m_idleStateButtonColour = HAPI_TColour(24, 15, 13, 255);
	buttonControllerScript->m_idleStateTextColour = HAPI_TColour(172, 168, 72, 255);

	buttonControllerScript->m_hoverStateButtonColour = HAPI_TColour(60, 39, 35, 255);
	buttonControllerScript->m_hoverStateTextColour = HAPI_TColour(210, 206, 128, 255);

	buttonControllerScript->m_clickingStateButtonColour = HAPI_TColour(107, 99, 99, 255);
	buttonControllerScript->m_clickingStateTextColour = HAPI_TColour(255, 255, 255, 255);

	//save 3 button
	std::shared_ptr<ButtonWidget> saveThreeButton{ std::make_shared<ButtonWidget>() };
	this->m_UI_Manager.AddWidget("SaveThreeButton", saveThreeButton);
	saveThreeButton->InitializeButton(this->m_ecsEngine, { 640.0f, 584.0f }, { 342, 60 }, HAPI_TColour(24, 15, 13, 255), HAPI_TColour(172, 168, 72, 255), "Save 3", { 120, 6 }, 37);
	//configure configure button
	buttonControllerScript = std::static_pointer_cast<ButtonControllerScript>(this->m_ecsEngine.GetComponent<ScriptComponent>(saveThreeButton->GetWidgetID())->m_script);
	assert(buttonControllerScript);

	buttonControllerScript->m_idleStateButtonColour = HAPI_TColour(24, 15, 13, 255);
	buttonControllerScript->m_idleStateTextColour = HAPI_TColour(172, 168, 72, 255);

	buttonControllerScript->m_hoverStateButtonColour = HAPI_TColour(60, 39, 35, 255);
	buttonControllerScript->m_hoverStateTextColour = HAPI_TColour(210, 206, 128, 255);

	buttonControllerScript->m_clickingStateButtonColour = HAPI_TColour(107, 99, 99, 255);
	buttonControllerScript->m_clickingStateTextColour = HAPI_TColour(255, 255, 255, 255);

	//exit button
	std::shared_ptr<ButtonWidget> exitButton{ std::make_shared<ButtonWidget>() };
	this->m_UI_Manager.AddWidget("ExitButton", exitButton);
	exitButton->InitializeButton(this->m_ecsEngine, { 0.0f, 840.0f }, { 120, 60 }, HAPI_TColour(24, 15, 13, 255), HAPI_TColour(20, 168, 72, 255), "Exit", { 23, 7 }, 37);
	//configure exit button
	buttonControllerScript = std::static_pointer_cast<ButtonControllerScript>(this->m_ecsEngine.GetComponent<ScriptComponent>(exitButton->GetWidgetID())->m_script);
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
	backButton->InitializeButton(this->m_ecsEngine, { 125.0f, 840.0f }, { 120, 60 }, HAPI_TColour(24, 15, 13, 255), HAPI_TColour(20, 168, 72, 255), "Back", { 20, 7 }, 37);
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

void SaveSelectionScene::DeInit()
{
	this->m_sceneClosed = true;
	this->m_UI_Manager.GetWidget<ButtonWidget>("SaveOneButton")->ResetWidget(this->m_ecsEngine);
	this->m_UI_Manager.GetWidget<ButtonWidget>("SaveTwoButton")->ResetWidget(this->m_ecsEngine);
	this->m_UI_Manager.GetWidget<ButtonWidget>("SaveThreeButton")->ResetWidget(this->m_ecsEngine);
	this->m_UI_Manager.GetWidget<ButtonWidget>("ExitButton")->ResetWidget(this->m_ecsEngine);
	this->m_UI_Manager.GetWidget<ButtonWidget>("BackButton")->ResetWidget(this->m_ecsEngine);
	UI_Manager::SetEntityEngaged(-1);

}

void SaveSelectionScene::Render(Renderer& renderer)
{
	this->m_renderManager.Render_RenderComponents(this->m_ecsEngine, renderer, {0, 0});
}

void SaveSelectionScene::ProcessUI(SceneManager& sceneManager)
{
	if (this->m_ecsEngine.GetComponent<EventComponent>(this->m_UI_Manager.GetWidget<ButtonWidget>("SaveOneButton")->GetWidgetID())->m_isMouseLeftClick)
	{
		SoundManager.PlaySound("ButtonClick");
		SceneSelectionButtonLogic(sceneManager, SaveSelection::Save1);
		return;
	}

	if (this->m_ecsEngine.GetComponent<EventComponent>(this->m_UI_Manager.GetWidget<ButtonWidget>("SaveTwoButton")->GetWidgetID())->m_isMouseLeftClick)
	{
		SoundManager.PlaySound("ButtonClick");
		SceneSelectionButtonLogic(sceneManager, SaveSelection::Save2);
		return;
	}

	if (this->m_ecsEngine.GetComponent<EventComponent>(this->m_UI_Manager.GetWidget<ButtonWidget>("SaveThreeButton")->GetWidgetID())->m_isMouseLeftClick)
	{
		SoundManager.PlaySound("ButtonClick");
		SceneSelectionButtonLogic(sceneManager, SaveSelection::Save3);
		return;
	}

	if (this->m_ecsEngine.GetComponent<EventComponent>(this->m_UI_Manager.GetWidget<ButtonWidget>("BackButton")->GetWidgetID())->m_isMouseLeftClick)
	{
		SoundManager.PlaySound("ButtonClick");
		sceneManager.PopScene();
		sceneManager.LoadScene("MainMenu");
		return;
	}

	if (this->m_ecsEngine.GetComponent<EventComponent>(this->m_UI_Manager.GetWidget<ButtonWidget>("ExitButton")->GetWidgetID())->m_isMouseLeftClick)
	{
		SoundManager.PlaySound("ButtonClick");
		HAPI.Close();
		return;
	}
}

void SaveSelectionScene::SceneSelectionButtonLogic(SceneManager& sceneManager, const SaveSelection& saveSelection)
{
	sceneManager.PopScene();
	sceneManager.LoadScene(this->m_sceneToLoadUponSaveSelection);
	if (this->m_sceneToLoadUponSaveSelection == "LevelEditorScene")
	{
		Scene::m_editorMode = true;
		std::static_pointer_cast<LevelEditorScene>(sceneManager.PeakScene())->SetSaveSelection(saveSelection);
	}
	else if (this->m_sceneToLoadUponSaveSelection == "MainScene")
	{
		std::static_pointer_cast<MainScene>(sceneManager.PeakScene())->SetSaveSelection(saveSelection);
		Scene::m_editorMode = false;

	}
}

void SaveSelectionScene::Update(const float& deltaTime, SceneManager& sceneManager)
{
	this->m_ecsEngine.UpdateECS(deltaTime, this->m_renderManager, this->m_UI_Manager, this->m_prefabEngine);

}
