#include "Visualisation.h"
#include"../Macros/Colors.h"

#include"../ECS/Base/Core_ECS_Assets.h"

#include"../Managers/TextureManager.h"
#define TextureManager TextureManager::GetInstance()
#include"../ECS/Base/ScriptAllocator.h"
#define ScriptAllocator ScriptAllocator::GetInstance()
#include"../Managers/SoundManager.h"
#define SoundManager SoundManager::GetInstance()

Visualisation::Visualisation()
	:m_sceneManager()
{
	m_hapiSuccess = m_renderer.InitHAPIWindow(windowSize, std::string("Milestone 4+"), HAPI_InitFlags::eDefaults);
	ECS_Engine& m_prefabEngine{ Scene::m_prefabEngine };

	SoundManager.LoadSound("GunFire", "Data\\gunFireSound.wav");
	SoundManager.LoadSound("ButtonClick", "Data\\ButtonClick.wav");
	SoundManager.LoadSound("EnemyTakingDamage", "Data\\EnemyTakingDamage.wav");
	SoundManager.LoadSound("BombSound", "Data\\BombSound.wav");


	//SpriteSheets
	{
		TextureManager.LoadTextureAndStore("Data\\TileSheets\\PokemonWorld.png", "PokemonWorldTileSheet", this->m_renderer.m_kBytesPerPixel);
		TextureManager.LoadTextureAndStore("Data\\fireThing.png", "FireThing", 4);
		TextureManager.LoadTextureAndStore("Data\\PlayerAnims.png", "PlayerAnims", this->m_renderer.m_kBytesPerPixel);
		TextureManager.LoadTextureAndStore("Data\\explosionAnimation2.png", "Explosions", this->m_renderer.m_kBytesPerPixel);
		TextureManager.LoadTextureAndStore("Data\\Knights.png", "Knights", this->m_renderer.m_kBytesPerPixel);
		TextureManager.LoadTextureAndStore("Data\\explosionAnimation.png", "Bom", this->m_renderer.m_kBytesPerPixel);
	}

	//Textures
	{
		//From Sprite Sheet
		{
			TextureManager.LoadTextureFromTextureAndStore("PokemonWorldTileSheet", IntRect(0, 100, 63, 196), "GreenTree", this->m_renderer.m_kBytesPerPixel);
		}

		//From Disk
		{
			TextureManager.LoadTextureAndStore("Data/alphaThing.tga", "alphaThing", 4);
			TextureManager.LoadTextureAndStore("Data\\GrassTile.png", "GrassTile", 4);
			TextureManager.LoadTextureAndStore("Data\\Stones.png", "Stones", 4);
			TextureManager.LoadTextureAndStore("Data\\LumpOfWood.png", "LumpOfWood", 4);
			TextureManager.LoadTextureAndStore("Data\\Rock1.png", "Rock1", 4);
			TextureManager.LoadTextureAndStore("Data\\Mud.png", "Mud", 4);
			TextureManager.LoadTextureAndStore("Data\\PersonPlacement.png", "PersonPlacement", 4);
			TextureManager.LoadTextureAndStore("Data\\SpawnCave.png", "SpawnCave", 4);
			TextureManager.LoadTextureAndStore("Data\\SpawnCave_2.png", "SpawnCave_2", 4);

		}
	}


	std::shared_ptr<AnimationSystem> animationSystem{ m_prefabEngine.GetSystem<AnimationSystem>() };
	//Animations
	{
		//PLAYER
		std::shared_ptr<Texture> spriteSheet{ TextureManager.GetTexture("PlayerAnims") };
		animationSystem->AddAnimationFromSpriteSheet("Player_Walk_Forward", spriteSheet, 9, 4, 0, 2, 9);
		animationSystem->AddAnimationFromSpriteSheet("Player_Walk_Back", spriteSheet, 9, 4, 0, 0, 9);
		animationSystem->AddAnimationFromSpriteSheet("Player_Walk_Left", spriteSheet, 9, 4, 0, 1, 9);
		animationSystem->AddAnimationFromSpriteSheet("Player_Walk_Right", spriteSheet, 9, 4, 0, 3, 9);

		//KNIGHT
		std::shared_ptr<Texture> knightAnimations{TextureManager.GetTexture("Knights")};
		animationSystem->AddAnimationFromSpriteSheet("SilverKnight_WalkForward", knightAnimations, 12, 4, 3, 0, 3);
		animationSystem->AddAnimationFromSpriteSheet("SilverKnight_WalkBackward", knightAnimations, 12, 4, 3, 3, 3);
		animationSystem->AddAnimationFromSpriteSheet("SilverKnight_WalkLeft", knightAnimations, 12, 4, 3, 1, 3);
		animationSystem->AddAnimationFromSpriteSheet("SilverKnight_WalkRight", knightAnimations, 12, 4, 3, 2, 3);

		//BOMMER
		animationSystem->AddAnimationFromSpriteSheet("Bommer_WalkForward", knightAnimations, 12, 4, 0, 0, 3);
		animationSystem->AddAnimationFromSpriteSheet("Bommer_WalkBackward", knightAnimations, 12, 4, 0, 3, 3);
		animationSystem->AddAnimationFromSpriteSheet("Bommer_WalkLeft", knightAnimations, 12, 4, 0, 1, 3);
		animationSystem->AddAnimationFromSpriteSheet("Bommer_WalkRight", knightAnimations, 12, 4, 0, 2, 3);

		//BOM
		animationSystem->AddAnimationFromSpriteSheet("Bom", TextureManager.GetTexture("Bom"), 5, 4, 0, 0, 17);

		std::shared_ptr<Texture> explosionSpriteSheet{ TextureManager.GetTexture("Explosions") };
		animationSystem->AddAnimationFromSpriteSheet("Explosions", explosionSpriteSheet, 5, 4, 0, 0, 18);

		animationSystem->AddAnimationFromSpriteSheet("AnimatedFireThing", TextureManager.GetTexture("FireThing"), 6, 3, 0, 1, 6);

	}

	//Prefab Entitiy Instansiation
	{


		//world entities
		{
			uint32_t firePole{ CreatePrefabEntity(RenderComponent::RenderLayer::WorldObject_1, animationSystem->GetAnimationFrame("AnimatedFireThing", 0), true, Effects::NoEffects, true, "AnimatedFireThing", false) };
			m_prefabEngine.AddComponent<AnimationComponent>(firePole)->InitializeAnimationComponent("AnimatedFireThing", 0, 1.0f / 7.0f, true, true, false);

			CreatePrefabEntity(RenderComponent::RenderLayer::WorldObject_3, TextureManager.GetTexture("GreenTree"), true, Effects::NoEffects, true, "GreenTree", false);
			CreatePrefabEntity(RenderComponent::RenderLayer::WorldObject_1, TextureManager.GetTexture("LumpOfWood"), true, Effects::NoEffects, true, "LumpOfWood", false);
			CreatePrefabEntity(RenderComponent::RenderLayer::WorldObject_1, TextureManager.GetTexture("Rock1"), true, Effects::NoEffects, true, "Rock1", false, 0, 0, true, CollisionComponent::Default, FRect(0, 0, 32, 32, Vector2f(16, 16)));
			
			uint32_t bullet{ CreatePrefabEntity(RenderComponent::RenderLayer::WorldObject_1, animationSystem->GetAnimationFrame("Explosions", 2), true, Effects::NoEffects, true, "Bullet", false, ScriptAllocator.GetScript<BulletScript>(), true, true, CollisionComponent::Default, FRect(0, 0, 8, 8, Vector2f(56.f, 56.f))) };
			m_prefabEngine.AddComponent<LifeComponent>(bullet)->InitializeLifeComponent(false, 0.0f, 0.0f, 5.0f, false, false);
			
			uint32_t explosion{ CreatePrefabEntity(RenderComponent::RenderLayer::WorldObject_1, animationSystem->GetAnimationFrame("Explosions", 0), true, Effects::NoEffects, true, "Explosion", false) };
			m_prefabEngine.AddComponent<AnimationComponent>(explosion)->InitializeAnimationComponent("Explosions", 0, 1.0f / 30.0f, true, false, true);
			
			uint32_t silverKnight{ CreatePrefabEntity(RenderComponent::RenderLayer::WorldObject_3, animationSystem->GetAnimationFrame("SilverKnight_WalkForward", 0), true, Effects::NoEffects, false, "SilverKnight", false, ScriptAllocator.GetScript<SilverKnightScript>(), true, true, CollisionComponent::CollisionLayer::Default, FRect(0.f, 0.f, 25.f, 42.f, Vector2f(7, 5)) )};
			m_prefabEngine.GetComponent<CollisionComponent>(silverKnight)->m_isDynamicCollidable = true;
			m_prefabEngine.AddComponent<AIComponent>(silverKnight)->InitializeAIComponent(true, 150.f);
			m_prefabEngine.AddComponent<AnimationComponent>(silverKnight)->InitializeAnimationComponent("SilverKnight_WalkForward", 0, 1.0f/10.0f, true, true, false);
			m_prefabEngine.AddComponent<LifeComponent>(silverKnight)->InitializeLifeComponent(true, 100.0f, 100.0f, 0.0f, true, true);

			uint32_t bommer{ CreatePrefabEntity(RenderComponent::RenderLayer::WorldObject_3, animationSystem->GetAnimationFrame("Bommer_WalkForward", 0), true, Effects::NoEffects, false, "Bommer", false, ScriptAllocator.GetScript<BommerScript>(), true, true, CollisionComponent::CollisionLayer::Default, FRect(0.f, 0.f, 25.f, 42.f, Vector2f(7, 5))) };
			m_prefabEngine.GetComponent<CollisionComponent>(bommer)->m_isDynamicCollidable = true;
			m_prefabEngine.AddComponent<AIComponent>(bommer)->InitializeAIComponent(true, 150.f);
			m_prefabEngine.AddComponent<AnimationComponent>(bommer)->InitializeAnimationComponent("Bommer_WalkForward", 0, 1.0f / 10.0f, true, true, false);
			m_prefabEngine.AddComponent<LifeComponent>(bommer)->InitializeLifeComponent(true, 10.0f, 10.0f, 0.0f, true, true);

			uint32_t bom{ CreatePrefabEntity(RenderComponent::RenderLayer::WorldObject_1, animationSystem->GetAnimationFrame("Bom", 0), true, Effects::NoEffects, true, "Bom", false) };
			m_prefabEngine.AddComponent<AnimationComponent>(bom)->InitializeAnimationComponent("Bom", 0, 1.0f / 30.0f, true, false, true);


		}

		//Tile Entities->
		{
			CreatePrefabEntity(RenderComponent::RenderLayer::Tile, TextureManager.GetTexture("GrassTile"), false, Effects::NoEffects, true, "GrassTile");
			CreatePrefabEntity(RenderComponent::RenderLayer::Tile, TextureManager.GetTexture("Stones"), false, Effects::NoEffects, true, "Stones", true, nullptr, 1, true, CollisionComponent::Default, FRect(0, 0, 128, 128));
			CreatePrefabEntity(RenderComponent::RenderLayer::Tile, TextureManager.GetTexture("Mud"), false, Effects::NoEffects, true, "Mud", true);


		}
		// Default Entities
		{
			uint32_t AlphaThing{ CreatePrefabEntity(RenderComponent::RenderLayer::Default, TextureManager.GetTexture("alphaThing"), true, Effects::UseGlobalAlpha, true, "SpriteThing", false, ScriptAllocator.GetScript<AlphaThingScript>(), true) };
			std::static_pointer_cast<AlphaThingScript>(m_prefabEngine.GetComponent<ScriptComponent>(AlphaThing)->m_script)->InitializeScript(30.f);
		}
		//Editor entities
		{
			uint32_t personPlacement{ CreatePrefabEntity(RenderComponent::RenderLayer::WorldObject_3, TextureManager.GetTexture("PersonPlacement"), true, Effects::NoEffects, true, "PersonPlacement", false) };
			m_prefabEngine.GetComponent<EditorComponent>(personPlacement)->m_oneInstanceOnly = true;
			m_prefabEngine.GetComponent<RenderComponent>(personPlacement)->m_onlyRenderWhenInEditorMode = true;

			uint32_t knightSpawner{ CreatePrefabEntity(RenderComponent::RenderLayer::WorldObject_2,	TextureManager.GetTexture("SpawnCave"),true, Effects::NoEffects, true,"SilverKnight_Spawner", false, ScriptAllocator.GetScript<WorldSpawnerScript>()) };
			m_prefabEngine.AddComponent<SpawnerComponent>(knightSpawner)->InitializeSpawnerComponent("SilverKnight", 10, 1.f / 200.f);

			uint32_t bommerSpawner{ CreatePrefabEntity(RenderComponent::RenderLayer::WorldObject_2,	TextureManager.GetTexture("SpawnCave_2"),true, Effects::NoEffects, true,"Bommer_Spawner", false, ScriptAllocator.GetScript<WorldSpawnerScript>()) };
			m_prefabEngine.AddComponent<SpawnerComponent>(bommerSpawner)->InitializeSpawnerComponent("Bommer", 10, 1.f / 200.f);
		};


	}

	m_sceneManager.LoadScenes(this->m_renderer);

	/*
	Game and Editor

	*/
	m_sceneManager.LoadScene("StartScene");

	//add the start screen to the queue
	//m_sceneManager.LoadScene("StartScene");
}
void Visualisation::Run()
{
	if (m_hapiSuccess)
	{
		while (HAPI.Update())
		{
			while (elapsedTime >= deltaTime)
			{
				//read this: https://stackoverflow.com/questions/10819436/bit-shifting-for-fixed-point-arithmetic-on-float-numbers-in-c
				m_sceneManager.UpdateScenes(deltaTime);
				elapsedTime -= deltaTime;

			}
			m_sceneManager.RenderScenes(m_renderer, HAPI_TColour(0, 0, 0, 0), false);

		//https://teams.microsoft.com/68007363-d8c6-405d-84f4-4c7b6b5bb994
		currentTime = static_cast<double>(HAPI.GetTime()) / 1000;
		elapsedTime += currentTime - previousTime;
		previousTime = currentTime;
		}

	}
	std::cerr << "Hapi failed to initialize!\n" << std::endl;
}

uint32_t Visualisation::CreatePrefabEntity(
	const unsigned int& renderLayer,
	const std::shared_ptr<Texture>& texture,
	const bool& useAlphaChannels,
	const Effects& effects,
	const bool& moveByMouse,
	const std::string& tagName,
	const bool& spatialLock,
	const std::shared_ptr<BaseScript>& baseScript,
	const bool& movementComponent,
	const bool& addCollisionComponent,
	const unsigned int& collisionLayer,
	FRect collisionRect
)
{
	static ECS_Engine& prefabEngine{ Scene::m_prefabEngine };

	uint32_t newEntityID{ prefabEngine.InstantiateEntity() };

	prefabEngine.AddComponent<TransformComponent>(newEntityID)->InitializeTransformComponent();

	prefabEngine.AddComponent<RenderComponent>(newEntityID)->InitializeRenderComponent(
		RenderComponent::RenderConfigs::Sprite,
		renderLayer,
		true,
		true);

	
	prefabEngine.AddComponent<SpriteComponent>(newEntityID)->m_sprite.InitializeSprite(
		texture,
		useAlphaChannels,
		effects);

	prefabEngine.AddComponent<EventComponent>(newEntityID)->InitializeEventComponent(
		texture->m_textureRect
		);

	prefabEngine.AddComponent<EditorComponent>(newEntityID)->InitializeEditorComponent(moveByMouse, spatialLock);

	if (baseScript != nullptr)
		prefabEngine.AddComponent<ScriptComponent>(newEntityID)->InitializeScriptComponent(baseScript);

	if (movementComponent)
		prefabEngine.AddComponent<MovementComponent>(newEntityID)->InitializeMovementComponent();

	if (addCollisionComponent)
	{
		prefabEngine.AddComponent<CollisionComponent>(newEntityID)->InitializeCollisionComponent(collisionLayer, true, collisionRect);
		//prefabEngine.GetComponent<RenderComponent>(newEntityID)->m_renderConfigs |= (RenderComponent::RenderConfigs::DrawBoundingBox | RenderComponent::RenderConfigs::DrawBoundingBoxCollisionComponent);
	}

	prefabEngine.TagEntity(tagName, newEntityID);

	return newEntityID;
}
