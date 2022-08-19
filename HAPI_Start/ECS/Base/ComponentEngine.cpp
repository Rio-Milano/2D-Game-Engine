#include "ComponentEngine.h"
#include<iostream>
#include<string>
#include<memory>
#include"../ECS/MyComponents/ScriptComponent.h"
#include"../ECS/Base/ScriptAllocator.h"
#define ScriptAllocator ScriptAllocator::GetInstance()

void ComponentEngine::ClearComponents()
{
	//clear all entity->component binds
	m_Component_DataStore.clear();
	//deallocate all components from current use
	for (const std::shared_ptr<BaseComponent>& component : m_componentPool)
		component->isInUse = false;
}


std::shared_ptr<BaseComponent> ComponentEngine::GetComponent(const uint32_t& entityID)
{
	//if entity->component bind dosent exist
	if (m_Component_DataStore.find(entityID) == m_Component_DataStore.end())
		return nullptr;
	
	//dont output error as systems will know exact type of component so it can give more detailed output  for 
	//when the component is absolutely required

	return m_Component_DataStore[entityID];
}



void ComponentEngine::AttachComponentToEntity(const uint32_t& entityID, std::shared_ptr<BaseComponent> component)
{
	m_Component_DataStore[entityID] = component;
}


void ComponentEngine::RemoveComponent(const uint32_t& entityID)
{
	//if entity has no bind to a component
	if (m_Component_DataStore.find(entityID) == m_Component_DataStore.end())
	{
		std::cerr << std::string("Cant remove component as it dosent exist!\n");
		return;
	}

	//allow component to be re-used by another entity
	m_Component_DataStore[entityID]->isInUse = false;

	//if a script is attached to the component
	if (m_Component_DataStore[entityID]->attributesExternalLinkage)
	{
		//cast the component to a script component
		std::shared_ptr<ScriptComponent> scriptComponent{std::static_pointer_cast<ScriptComponent>(m_Component_DataStore[entityID])};
		assert(scriptComponent != nullptr);//validate the component
		ScriptAllocator.FreeScript(scriptComponent->m_script);//un-allocate the attached script
		m_Component_DataStore[entityID]->attributesExternalLinkage = false;//set flag to script linkage to false
	}

	//remove the entity->component bind
	m_Component_DataStore.erase(entityID);
}

std::unordered_map<uint32_t, std::shared_ptr<BaseComponent>>& ComponentEngine::GetComponentPool()
{
	//returns the entity->component binds
	return m_Component_DataStore;
}
