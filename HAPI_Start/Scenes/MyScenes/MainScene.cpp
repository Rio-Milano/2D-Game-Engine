#include "MainScene.h"

#include"../ECS/Base/Core_ECS_Assets.h"
#include"../Managers/AllManagerHeadders.h"
#include"../UI/UI Widget/WidgetHeadders.h"
#include"../MyScenes/SaveSelectionScene.h"
#include"../ECS/Base/ScriptAllocator.h"
#include"../Scenes/SceneCore/SceneManager.h"
#include"../Scenes/MyScenes/AllSceneHeadders.h"
#include"../Managers/SoundManager.h"
#define SoundManager SoundManager::GetInstance()

#define ScriptAllocator ScriptAllocator::GetInstance()
#include<assert.h>

MainScene::MainScene(Renderer& renderer, const Scene_Enum& sceneEnum)
	:Scene(renderer, sceneEnum)
{

	

}

void MainScene::SetSaveSelection(const SaveSelection& saveSelectionArg)
{
	this->m_saveSelection = saveSelectionArg;
}

void MainScene::Init()
{
	//dragon
	m_ecsEngine.GetSystem<AnimationSystem>()->LoadAnimationsToSystemFromSystem(m_prefabEngine);

	std::shared_ptr<Texture> animationFrameTexture{ m_ecsEngine.GetSystem<AnimationSystem>()->GetAnimationFrame("Player_Walk_Forward", 0) };
	uint32_t playerKnight{ m_ecsEngine.InstantiateEntity() };
	m_ecsEngine.AddComponent<TransformComponent>(playerKnight)->InitializeTransformComponent(Vector2f(100, 100));
	m_ecsEngine.AddComponent<SpriteComponent>(playerKnight)->m_sprite.InitializeSprite(animationFrameTexture, true, Effects::NoEffects);
	m_ecsEngine.AddComponent<AnimationComponent>(playerKnight)->InitializeAnimationComponent("Player_Walk_Forward", 0, 1.0f / 9.0f, false, true, false); Vector2i animationFrameSize{ animationFrameTexture->m_textureSize };
	m_ecsEngine.AddComponent<CollisionComponent>(playerKnight)->InitializeCollisionComponent(CollisionComponent::Default, true, FRect(0.f, 0.f, animationFrameSize.x/2.f, animationFrameSize.y/3.f *2.f, Vector2f(animationFrameSize.x / 4.f, (animationFrameSize.y / 3.f) / 2.f)), true);
	m_ecsEngine.AddComponent<MovementComponent>(playerKnight)->InitializeMovementComponent(Vector2f(0.0f, 0.0f), 200 );
	m_ecsEngine.AddComponent<RenderComponent>(playerKnight)->InitializeRenderComponent(RenderComponent::Sprite /*| RenderComponent::DrawBoundingBox | RenderComponent::DrawBoundingBoxCollisionComponent*/, RenderComponent::RenderLayer::WorldObject_1, true, true);
	m_ecsEngine.AddComponent<ScriptComponent>(playerKnight)->InitializeScriptComponent(ScriptAllocator.GetScript<PlayerScript>());
	m_ecsEngine.AddComponent<LifeComponent>(playerKnight)->InitializeLifeComponent(true, 100.0f, 100.0f, 0.0f, false, true);
	m_renderManager.AddEntityIdToRenderLayer(this->m_ecsEngine, playerKnight, m_ecsEngine.GetComponent<RenderComponent>(playerKnight)->m_renderLayer);
	m_ecsEngine.TagEntity("Player", playerKnight);

	//exit button
	std::shared_ptr<ButtonWidget> backButton{ std::make_shared<ButtonWidget>() };
	this->m_UI_Manager.AddWidget("BackButton", backButton);
	backButton->InitializeButton(this->m_ecsEngine, { 0.0f, 840.0f }, { 120, 60 }, HAPI_TColour(24, 15, 13, 255), HAPI_TColour(20, 168, 72, 255), "Back", { 23, 7 }, 37);
	//configure exit button
	std::shared_ptr<ButtonControllerScript> buttonControllerScript = std::static_pointer_cast<ButtonControllerScript>(this->m_ecsEngine.GetComponent<ScriptComponent>(backButton->GetWidgetID())->m_script);
	assert(buttonControllerScript);

	buttonControllerScript->m_idleStateButtonColour = HAPI_TColour(24, 15, 13, 255);
	buttonControllerScript->m_idleStateTextColour = HAPI_TColour(172, 168, 72, 255);

	buttonControllerScript->m_hoverStateButtonColour = HAPI_TColour(60, 39, 35, 255);
	buttonControllerScript->m_hoverStateTextColour = HAPI_TColour(210, 206, 128, 255);

	buttonControllerScript->m_clickingStateButtonColour = HAPI_TColour(107, 99, 99, 255);
	buttonControllerScript->m_clickingStateTextColour = HAPI_TColour(255, 255, 255, 255);


}

void MainScene::DeInit()
{
	this->m_sceneClosed = true;
	UI_Manager::SetEntityEngaged(-1);
	
	m_UI_Manager.Clear();
	m_ecsEngine.DeleteAllEntities();
	m_ecsEngine.UpdateECS(0.0f, m_renderManager, m_UI_Manager, m_ecsEngine);
	InitCamera();
	Init();
	std::static_pointer_cast<PlayerScript>(m_ecsEngine.GetComponent<ScriptComponent>(m_ecsEngine.GetTaggedEntities("Player")[0])->m_script)->m_positionSet = false;
	m_saveLoaded = false;

}


void MainScene::Render(Renderer& renderer)
{
	std::shared_ptr<TransformComponent> camersTransform{ m_ecsEngine.GetComponent<TransformComponent>(m_ecsEngine.GetTaggedEntities("Camera")[0]) };
	
	///
	Vector2f mouseInWorld{ camersTransform->position.x + Mouse.x , camersTransform->position.y + Mouse.y };
	RenderManager::mousePositionInWorldForTestingAIPathfindingInDevTesting = mouseInWorld;
	///

	m_renderManager.Render_RenderComponents(m_ecsEngine, renderer, GetCameraOffset());



	if (TextureManager.GetRunTimeErrors().size() > 0)
	{
		HAPI.RenderText(0, 30, RED, TextureManager.GetRunTimeErrors());
		HAPI.RenderText(80, 400, GREEN, RED, 0.3f, "Some Textures are missing, the program will close in 5 seconds!", 60, HAPI_TextStyle::eBold | HAPI_TextStyle::eItalic | HAPI_TextStyle::eUnderlined);

		if (HAPI.GetTime() >= 5000)
			HAPI.Close();
	}

	HAPI.RenderText(0, 0, HAPI_TColour::WHITE, "Player Score :" + std::to_string(std::static_pointer_cast<PlayerScript>(m_ecsEngine.GetComponent<ScriptComponent>(m_ecsEngine.GetTaggedEntities("Player")[0])->m_script)->GetPlayerScore()));
	HAPI.RenderText(0, 10, HAPI_TColour::WHITE, "Player Health :" + std::to_string(m_ecsEngine.GetComponent<LifeComponent>(m_ecsEngine.GetTaggedEntities("Player")[0])->m_health));

		

}
void MainScene::ProcessUI(SceneManager& sceneManager)
{
	if (this->m_ecsEngine.GetComponent<EventComponent>(this->m_UI_Manager.GetWidget<ButtonWidget>("BackButton")->GetWidgetID())->m_isMouseLeftClick)
	{
		sceneManager.PopScene();
		sceneManager.LoadScene("StartScene");
		SoundManager.PlaySound("ButtonClick");
		return;
	}

}
void MainScene::Update(const float& deltaTime, SceneManager& sceneManager)
{
	Utils::LoadLevel(this->m_saveLoaded, this->fileName, this->m_saveSelection, this->m_ecsEngine, this->m_prefabEngine, this->m_renderManager, this->m_UI_Manager);
	this->m_ecsEngine.GetSystem<EditorSystem>()->DisableSystem(this->m_ecsEngine);
	m_ecsEngine.UpdateECS(deltaTime, this->m_renderManager, this->m_UI_Manager, this->m_prefabEngine);

	ControlCamera(deltaTime);

	if (m_ecsEngine.GetComponent<LifeComponent>(m_ecsEngine.GetTaggedEntities("Player")[0])->m_health <= 0.0f)
	{
		sceneManager.PopScene();
		sceneManager.LoadScene("GameCycling");
		std::shared_ptr<GameCycling> scene{std::static_pointer_cast<GameCycling>(sceneManager.PeakScene())};
		scene->gameResult = "You Lose!";
		scene->playerScore = std::static_pointer_cast<PlayerScript>(m_ecsEngine.GetComponent<ScriptComponent>(m_ecsEngine.GetTaggedEntities("Player")[0])->m_script)->GetPlayerScore();
		return;
	}
	//check if spawnerX is empty && check if spawnedAreAllDead
	//do for other spawners

	//if true then player wins as all enemies are dead

	std::vector<uint32_t> Spawners{};
	
	Spawners.insert(Spawners.end(), m_ecsEngine.GetTaggedEntities("SilverKnight_Spawner").begin(), m_ecsEngine.GetTaggedEntities("SilverKnight_Spawner").end());
	Spawners.insert(Spawners.end(), m_ecsEngine.GetTaggedEntities("Bommer_Spawner").begin(), m_ecsEngine.GetTaggedEntities("Bommer_Spawner").end());

	std::vector<uint32_t> enemies{};

	enemies.insert(enemies.end(), m_ecsEngine.GetTaggedEntities("SilverKnight").begin(), m_ecsEngine.GetTaggedEntities("SilverKnight").end());
	enemies.insert(enemies.end(), m_ecsEngine.GetTaggedEntities("Bommer").begin(), m_ecsEngine.GetTaggedEntities("Bommer").end());


	for (const uint32_t& spawnerID : Spawners)
	{
		if (m_ecsEngine.GetComponent<SpawnerComponent>(spawnerID)->m_spawnAmount)
			return;
	};

	if (enemies.size())
		return;

	sceneManager.PopScene();
	sceneManager.LoadScene("GameCycling");
	std::shared_ptr<GameCycling> scene{ std::static_pointer_cast<GameCycling>(sceneManager.PeakScene()) };
	scene->gameResult = "You Win!";
	scene->playerScore = std::static_pointer_cast<PlayerScript>(m_ecsEngine.GetComponent<ScriptComponent>(m_ecsEngine.GetTaggedEntities("Player")[0])->m_script)->GetPlayerScore();
	return;




}






