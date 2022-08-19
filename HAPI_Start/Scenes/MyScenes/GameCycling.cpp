#include"GameCycling.h"
#include"../Managers/AllManagerHeadders.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../UI/UI Widget/WidgetHeadders.h"
#include"../SceneCore/SceneManager.h"
#include"../Managers/SoundManager.h"
#define SoundManager SoundManager::GetInstance()
#include<assert.h>

GameCycling::GameCycling(Renderer& renderer, const Scene_Enum& sceneEnum)
	:Scene(renderer, sceneEnum)
{
}

void GameCycling::Init()
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

	//exit button
	std::shared_ptr<ButtonWidget> exitButton{ std::make_shared<ButtonWidget>() };
	this->m_UI_Manager.AddWidget("ExitButton", exitButton);
	exitButton->InitializeButton(this->m_ecsEngine, { 0.0f, 840.0f }, { 120, 60 }, HAPI_TColour(24, 15, 13, 255), HAPI_TColour(20, 168, 72, 255), "Exit", { 23, 7 }, 37);
	//configure exit button
	std::shared_ptr<ButtonControllerScript> buttonControllerScript = std::static_pointer_cast<ButtonControllerScript>(this->m_ecsEngine.GetComponent<ScriptComponent>(exitButton->GetWidgetID())->m_script);
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

	//scene title entity
	uint32_t menuTitleEntity{ this->m_ecsEngine.InstantiateEntity() };
	this->m_ecsEngine.AddComponent<TransformComponent>(menuTitleEntity)->InitializeTransformComponent(Vector2f{ 650.f , 42.0f });
	this->m_ecsEngine.AddComponent<TextComponent>(menuTitleEntity)->InitializeTextComponent(HAPI_TColour(210, 206, 128, 255), gameResult, 80);
	this->m_ecsEngine.AddComponent<RenderComponent>(menuTitleEntity)->InitializeRenderComponent
	(
		RenderComponent::RenderConfigs::Text,
		RenderComponent::RenderLayer::UI,
		true,
		false
	);
	m_ecsEngine.TagEntity("GameResult", menuTitleEntity);
	this->m_renderManager.AddEntityIdToRenderLayer(this->m_ecsEngine, menuTitleEntity, RenderComponent::RenderLayer::UI);


	//difficulty text
	uint32_t difficultyText{ this->m_ecsEngine.InstantiateEntity() };
	this->m_ecsEngine.AddComponent<TransformComponent>(difficultyText)->InitializeTransformComponent(Vector2f{ 200.f, 230.f });
	this->m_ecsEngine.AddComponent<TextComponent>(difficultyText)->InitializeTextComponent(HAPI_TColour(210, 206, 128, 255), "<insert player score>" + std::to_string(playerScore), 60);
	this->m_ecsEngine.AddComponent<RenderComponent>(difficultyText)->InitializeRenderComponent
	(
		RenderComponent::RenderConfigs::Text,
		RenderComponent::RenderLayer::UI,
		true,
		false
	);
	m_ecsEngine.TagEntity("Score", difficultyText);
	this->m_renderManager.AddEntityIdToRenderLayer(this->m_ecsEngine, difficultyText, RenderComponent::RenderLayer::UI);
}

void GameCycling::DeInit()
{
	this->m_sceneClosed = true;
	this->m_UI_Manager.GetWidget<ButtonWidget>("BackButton")->ResetWidget(this->m_ecsEngine);
	this->m_UI_Manager.GetWidget<ButtonWidget>("ExitButton")->ResetWidget(this->m_ecsEngine);
	UI_Manager::SetEntityEngaged(-1);
}

void GameCycling::Render(Renderer& renderer)
{
	//draw game entities
	m_renderManager.Render_RenderComponents(this->m_ecsEngine, this->m_rendererReferance, { 0, 0 });

}

void GameCycling::ProcessUI(SceneManager& sceneManager)
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
}

void GameCycling::Update(const float& deltaTime, SceneManager& sceneManager)
{
	m_ecsEngine.GetComponent<TextComponent>(m_ecsEngine.GetTaggedEntities("GameResult")[0])->m_text = gameResult;
	m_ecsEngine.GetComponent<TextComponent>(m_ecsEngine.GetTaggedEntities("Score")[0])->m_text = "Your score: " + std::to_string(playerScore);

	this->m_ecsEngine.UpdateECS(deltaTime, this->m_renderManager, this->m_UI_Manager, this->m_prefabEngine);

}
