#include "StartScene.h"

#include"../Managers/AllManagerHeadders.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../UI/UI Widget/WidgetHeadders.h"
#include"../SceneCore/SceneManager.h"
#include"../Managers/SoundManager.h"
#define SoundManager SoundManager::GetInstance()
#include<assert.h>

StartScene::StartScene(Renderer& renderer, const Scene_Enum& sceneEnum)
	:Scene(renderer, sceneEnum)
{
	//load any raw assets
	TextureManager.LoadTextureAndStore("Data\\BG.png", "StartScreenBackground", 4);

}

void StartScene::Init()
{

	std::shared_ptr<Texture> backgroundTexturePtr{ TextureManager.GetTexture("StartScreenBackground")};
	/*
	load game entities
	*/

	//load background entity
	uint32_t backgroundEntity{this->m_ecsEngine.InstantiateEntity()};
	this->m_ecsEngine.AddComponent<TransformComponent>(backgroundEntity)->InitializeTransformComponent(Vector2f{0.0f, 0.0f});
	this->m_ecsEngine.AddComponent<SpriteComponent>(backgroundEntity)->m_sprite.InitializeSprite(backgroundTexturePtr, false, Effects::NoEffects);
	this->m_ecsEngine.AddComponent<RenderComponent>(backgroundEntity)->InitializeRenderComponent(RenderComponent::RenderConfigs::Sprite, RenderComponent::RenderLayer::Default, true, false);
	this->m_renderManager.AddEntityIdToRenderLayer(this->m_ecsEngine, backgroundEntity, RenderComponent::RenderLayer::Default);

	//start button
	std::shared_ptr<ButtonWidget> startButton{ std::make_shared<ButtonWidget>() };
	this->m_UI_Manager.AddWidget("StartButton", startButton);
	startButton->InitializeButton(this->m_ecsEngine, { 580.0f, 788.0f }, { 436, 85 }, HAPI_TColour(24, 15, 13, 255), HAPI_TColour(172, 168, 72, 255), "Start", {175, 18}, 37);
		//configure start button
		std::shared_ptr<ButtonControllerScript> buttonControllerScript{ std::static_pointer_cast<ButtonControllerScript>(this->m_ecsEngine.GetComponent<ScriptComponent>(startButton->GetWidgetID())->m_script) };
		assert(buttonControllerScript);

		buttonControllerScript->m_idleStateButtonColour = HAPI_TColour(24, 15, 13, 255);
		buttonControllerScript->m_idleStateTextColour = HAPI_TColour(172, 168, 72, 255);

		buttonControllerScript->m_hoverStateButtonColour = HAPI_TColour(60, 39, 35, 255);
		buttonControllerScript->m_hoverStateTextColour = HAPI_TColour(210, 206, 128, 255);

		buttonControllerScript->m_clickingStateButtonColour = HAPI_TColour(107, 99, 99, 255);
		buttonControllerScript->m_clickingStateTextColour = HAPI_TColour(255, 255, 255, 255);


	//title text
	uint32_t titleTextEntity{ this->m_ecsEngine.InstantiateEntity() };
	this->m_ecsEngine.AddComponent<TransformComponent>(titleTextEntity)->InitializeTransformComponent(Vector2f{270.f, 230.f});
	this->m_ecsEngine.AddComponent<TextComponent>(titleTextEntity)->InitializeTextComponent(HAPI_TColour(210, 206, 128, 255), "Death From The Wood", 100);
	this->m_ecsEngine.AddComponent<RenderComponent>(titleTextEntity)->InitializeRenderComponent
		(
		RenderComponent::RenderConfigs::Text,
		RenderComponent::RenderLayer::UI,
		true, 
		false
		);
	this->m_renderManager.AddEntityIdToRenderLayer(this->m_ecsEngine, titleTextEntity, RenderComponent::RenderLayer::UI);


}

void StartScene::DeInit()
{
	this->m_sceneClosed = true;
	this->m_UI_Manager.GetWidget<ButtonWidget>("StartButton")->ResetWidget(this->m_ecsEngine);
	UI_Manager::SetEntityEngaged(-1);
}


void StartScene::Render(Renderer& renderer)
{
	//draw game entities
	m_renderManager.Render_RenderComponents(this->m_ecsEngine, this->m_rendererReferance, {0, 0});
}

void StartScene::ProcessUI(SceneManager& sceneManager)
{
	if (this->m_ecsEngine.GetComponent<EventComponent>(this->m_UI_Manager.GetWidget<ButtonWidget>("StartButton")->GetWidgetID())->m_isMouseLeftClick)
	{
		sceneManager.PopScene();
		SoundManager.PlaySound("ButtonClick");
		sceneManager.LoadScene("MainMenu");
		return;
	}
}

void StartScene::Update(const float& deltaTime, SceneManager& sceneManager)
{
	if (this->m_startBackgroundMusic)
	{
		this->m_startBackgroundMusic = false;
		HAPI_TSoundOptions so;
		so.loop = true;
		so.volume = 0.25f;
		int insID;//might use instance id in sound manager so sounds can be adjusted after played. 
		HAPI.PlayStreamedMedia("Data\\BackgroundMusic.wav", so, insID);
	}

	this->m_ecsEngine.UpdateECS(deltaTime, this->m_renderManager, this->m_UI_Manager, this->m_prefabEngine);

}

