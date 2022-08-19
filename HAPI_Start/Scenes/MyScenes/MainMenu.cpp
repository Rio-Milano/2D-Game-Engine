#include "MainMenu.h"
#include"../Managers/AllManagerHeadders.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../UI/UI Widget/WidgetHeadders.h"
#include"../SceneCore/SceneManager.h"
#include"../MyScenes/AllSceneHeadders.h"
#include"../Managers/SoundManager.h"
#define SoundManager SoundManager::GetInstance()
#include<assert.h>

MainMenu::MainMenu(Renderer& renderer, const Scene_Enum& sceneEnum)
	:Scene(renderer, sceneEnum)
{

}

void MainMenu::Init()
{
	std::shared_ptr<Texture> backgroundTexturePtr{ TextureManager.GetTexture("StartScreenBackground") };
	/*
	load game entities
	*/

	//background entity
	uint32_t backgroundEntity{ this->m_ecsEngine.InstantiateEntity() };
	this->m_ecsEngine.AddComponent<TransformComponent>(backgroundEntity)->InitializeTransformComponent(Vector2f{ 0.0f, 0.0f });
	this->m_ecsEngine.AddComponent<SpriteComponent>(backgroundEntity)->m_sprite.InitializeSprite(backgroundTexturePtr, false, Effects::NoEffects);
	this->m_ecsEngine.AddComponent<RenderComponent>(backgroundEntity)->InitializeRenderComponent(RenderComponent::RenderConfigs::Sprite, RenderComponent::RenderLayer::Default, true, false);
	this->m_renderManager.AddEntityIdToRenderLayer(this->m_ecsEngine, backgroundEntity, RenderComponent::RenderLayer::Default);

	//scene title entity
	uint32_t menuTitleEntity{this->m_ecsEngine.InstantiateEntity()};
	this->m_ecsEngine.AddComponent<TransformComponent>(menuTitleEntity)->InitializeTransformComponent(Vector2f{ 583.0f , 42.0f });
	this->m_ecsEngine.AddComponent<TextComponent>(menuTitleEntity)->InitializeTextComponent(HAPI_TColour(210, 206, 128, 255), "Main Menu", 80);
	this->m_ecsEngine.AddComponent<RenderComponent>(menuTitleEntity)->InitializeRenderComponent
		(
		RenderComponent::RenderConfigs::Text,
		RenderComponent::RenderLayer::UI,
		true,
		false
		);
	this->m_renderManager.AddEntityIdToRenderLayer(this->m_ecsEngine, menuTitleEntity, RenderComponent::RenderLayer::UI);

	//play button
	std::shared_ptr<ButtonWidget> playButton{std::make_shared<ButtonWidget>()};
	this->m_UI_Manager.AddWidget("PlayButton", playButton);
	playButton->InitializeButton(this->m_ecsEngine, {615.0f, 224.0f }, { 342, 60 }, HAPI_TColour(24, 15, 13, 255), HAPI_TColour(172, 168, 72, 255), "Play", { 140, 7 }, 37);
		//configure play button
		std::shared_ptr<ButtonControllerScript> buttonControllerScript{ std::static_pointer_cast<ButtonControllerScript>(this->m_ecsEngine.GetComponent<ScriptComponent>(playButton->GetWidgetID())->m_script) };
		assert(buttonControllerScript);

		buttonControllerScript->m_idleStateButtonColour = HAPI_TColour(24, 15, 13, 255);
		buttonControllerScript->m_idleStateTextColour = HAPI_TColour(172, 168, 72, 255);

		buttonControllerScript->m_hoverStateButtonColour = HAPI_TColour(60, 39, 35, 255);
		buttonControllerScript->m_hoverStateTextColour = HAPI_TColour(210, 206, 128, 255);

		buttonControllerScript->m_clickingStateButtonColour = HAPI_TColour(107, 99, 99, 255);
		buttonControllerScript->m_clickingStateTextColour = HAPI_TColour(255, 255, 255, 255);

	//level editor button
	std::shared_ptr<ButtonWidget> levelEditorButton{ std::make_shared<ButtonWidget>() };
	this->m_UI_Manager.AddWidget("LevelEditorButton", levelEditorButton);
	levelEditorButton->InitializeButton(this->m_ecsEngine, { 615.0f, 404.0f }, { 342, 60 }, HAPI_TColour(24, 15, 13, 255), HAPI_TColour(172, 168, 72, 255), "Level Editor", { 80, 7 }, 37);
		//configure editor button
		buttonControllerScript = std::static_pointer_cast<ButtonControllerScript>(this->m_ecsEngine.GetComponent<ScriptComponent>(levelEditorButton->GetWidgetID())->m_script);
		assert(buttonControllerScript);

		buttonControllerScript->m_idleStateButtonColour = HAPI_TColour(24, 15, 13, 255);
		buttonControllerScript->m_idleStateTextColour = HAPI_TColour(172, 168, 72, 255);

		buttonControllerScript->m_hoverStateButtonColour = HAPI_TColour(60, 39, 35, 255);
		buttonControllerScript->m_hoverStateTextColour = HAPI_TColour(210, 206, 128, 255);

		buttonControllerScript->m_clickingStateButtonColour = HAPI_TColour(107, 99, 99, 255);
		buttonControllerScript->m_clickingStateTextColour = HAPI_TColour(255, 255, 255, 255);

	//configure button
	std::shared_ptr<ButtonWidget> configureButton{ std::make_shared<ButtonWidget>() };
	this->m_UI_Manager.AddWidget("ConfigureButton", configureButton);
	configureButton->InitializeButton(this->m_ecsEngine, { 615.0f, 584.0f }, { 342, 60 }, HAPI_TColour(24, 15, 13, 255), HAPI_TColour(172, 168, 72, 255), "Configure", { 100, 7 }, 37);
		//configure configure button
		buttonControllerScript = std::static_pointer_cast<ButtonControllerScript>(this->m_ecsEngine.GetComponent<ScriptComponent>(configureButton->GetWidgetID())->m_script);
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

void MainMenu::DeInit()
{
	this->m_sceneClosed = true;


	this->m_UI_Manager.GetWidget<ButtonWidget>("PlayButton")->ResetWidget(this->m_ecsEngine);
	this->m_UI_Manager.GetWidget<ButtonWidget>("LevelEditorButton")->ResetWidget(this->m_ecsEngine);
	this->m_UI_Manager.GetWidget<ButtonWidget>("ConfigureButton")->ResetWidget(this->m_ecsEngine);
	
	this->m_UI_Manager.GetWidget<ButtonWidget>("BackButton")->ResetWidget(this->m_ecsEngine);
	this->m_UI_Manager.GetWidget<ButtonWidget>("ExitButton")->ResetWidget(this->m_ecsEngine);

	UI_Manager::SetEntityEngaged(-1);

}


void MainMenu::Render(Renderer& renderer)
{
	//draw game entities
	m_renderManager.Render_RenderComponents(this->m_ecsEngine, this->m_rendererReferance, { 0, 0 });
}

void MainMenu::ProcessUI(SceneManager& sceneManager)
{
	if (this->m_ecsEngine.GetComponent<EventComponent>(this->m_UI_Manager.GetWidget<ButtonWidget>("BackButton")->GetWidgetID())->m_isMouseLeftClick)
	{
		sceneManager.PopScene();
		sceneManager.LoadScene("StartScene");
		SoundManager.PlaySound("ButtonClick");
		return;
	}

	if (this->m_ecsEngine.GetComponent<EventComponent>(this->m_UI_Manager.GetWidget<ButtonWidget>("ExitButton")->GetWidgetID())->m_isMouseLeftClick)
	{
		SoundManager.PlaySound("ButtonClick");
		HAPI.Close();
		return;
	}

	if (this->m_UI_Manager.GetWidget<ButtonWidget>("PlayButton")->OneStepClick(this->m_ecsEngine))
	{
		sceneManager.PopScene();
		sceneManager.LoadScene("SaveSelectionScene");
		SoundManager.PlaySound("ButtonClick");
		std::dynamic_pointer_cast<SaveSelectionScene>(sceneManager.PeakScene())->SetSceneToLoadUponSaveSelection("MainScene");
		return;

	}
	if (this->m_UI_Manager.GetWidget<ButtonWidget>("PlayButton")->OneStepClick(this->m_ecsEngine) ||
		this->m_UI_Manager.GetWidget<ButtonWidget>("LevelEditorButton")->OneStepClick(this->m_ecsEngine))
	{
		sceneManager.PopScene();
		sceneManager.LoadScene("SaveSelectionScene");
		SoundManager.PlaySound("ButtonClick");
		std::dynamic_pointer_cast<SaveSelectionScene>(sceneManager.PeakScene())->SetSceneToLoadUponSaveSelection("LevelEditorScene");
		return;

	}

	if (this->m_ecsEngine.GetComponent<EventComponent>(this->m_UI_Manager.GetWidget<ButtonWidget>("ConfigureButton")->GetWidgetID())->m_isMouseLeftClick)
	{
		SoundManager.PlaySound("ButtonClick");
		sceneManager.PopScene();
		sceneManager.LoadScene("ConfigureScene");
		return;

	}

}

void MainMenu::Update(const float& deltaTime, SceneManager& sceneManager)
{
	this->m_ecsEngine.UpdateECS(deltaTime, this->m_renderManager, this->m_UI_Manager, this->m_prefabEngine);
}
