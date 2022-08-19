#include"SpriteSystem.h"

#include"../MyComponents/SpriteComponent.h"
//#include"../ECS/MyComponents/"
#include"../UI/UI Core/UI_Manager.h"

#include"../Base/ECS_Engine.h"
#include<iostream>

SpriteSystem::SpriteSystem(const UpdatePriority& updatePriority)
	:System(updatePriority)
{
	m_componentEngine.AllocateComponents<SpriteComponent>();
}

//private system functions
std::shared_ptr<BaseComponent> SpriteSystem::DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)
{
	//we may not have a sprite component
	std::shared_ptr<SpriteComponent> toClone{ std::static_pointer_cast<SpriteComponent>(m_componentEngine.GetComponent(entityID)) };
	if (!toClone)
	{
		std::cerr << "Sprite Component of entity :" + std::to_string(entityID) + " was not found\n";
		return nullptr;
	}

	std::shared_ptr<SpriteComponent> duplicatedComponent = forignECS.AddComponent<SpriteComponent>(forignEntityID);

	duplicatedComponent->m_sprite.InitializeSprite(
		toClone->m_sprite.m_textureRef, toClone->m_sprite.m_useAlphaChannels, toClone->m_sprite.m_effects
	);

	return duplicatedComponent;
}




void SpriteSystem::ResetComponent(const uint32_t& entityID, ECS_Engine& ecs)
{
	//we may not have a spite component
	std::shared_ptr<SpriteComponent> spriteComponent{ ecs.GetComponent<SpriteComponent>(entityID)};
	if (!spriteComponent)
	{
		std::cerr << "Sprite Component of entity :" + std::to_string(entityID) + " was not found\n";
		return;
	}

	spriteComponent->m_sprite.m_effects = Effects::NoEffects;
	spriteComponent->m_sprite.m_textureRef = nullptr;
	spriteComponent->m_sprite.m_useAlphaChannels = false;
}


