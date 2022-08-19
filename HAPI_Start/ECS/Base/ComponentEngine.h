#ifndef COMPONENTENGINE_H
#define COMPONENTENGINE_H


#include<memory>
#include<list>
#include<unordered_map>
#include"BaseComponent.h"



//forward decleration of ECS_Engine to fix circular reference 
class ECS_Engine;

class ComponentEngine final
{
public:

	ComponentEngine() = default;
	~ComponentEngine() = default;

	//remove all components from memory
	void ClearComponents();

	//get a component from the component map
	std::shared_ptr<BaseComponent> GetComponent(const uint32_t& entityID);

	//get a new component
	template<class T>
	std::shared_ptr<T> InstansiateComponent();

	void AttachComponentToEntity(const uint32_t& entityID, std::shared_ptr<BaseComponent> component);

	//Remove a component from the system map
	void RemoveComponent(const uint32_t& entityID);


	std::unordered_map<uint32_t, std::shared_ptr<BaseComponent>>& GetComponentPool();

	//pre-allocation function that makes components outside of game loop. Type T MUST be a derived class from BaseComponent
	template<class T>
	void AllocateComponents(const size_t& componentsToAllocate = ECS_Engine::maxEntities);
	
	//maps entityID to component
	std::unordered_map<uint32_t, std::shared_ptr<BaseComponent>> m_Component_DataStore;

private:
	/*
	List of records where by each one has a component linked to an in-use status.
	*/
	std::vector<std::shared_ptr<BaseComponent>> m_componentPool;

};


template<class T>
inline std::shared_ptr<T> ComponentEngine::InstansiateComponent()
{
	//loop components
	for (const std::shared_ptr<BaseComponent>& component : m_componentPool)
	{
		//if component not in use
		if (!component->isInUse)
		{
			//set in use
			component->isInUse = true;
			//send component
			return std::static_pointer_cast<T>(component);
		}
	}
	return nullptr;
}


template<class T>
inline void ComponentEngine::AllocateComponents(const size_t& componentsToAllocate)
{
	//Allocate a given amount of components so heap allocation is not needed during run time

	m_componentPool.resize(componentsToAllocate);
	for (size_t componentIndex{ 0 }; componentIndex < componentsToAllocate; componentIndex++)
	{
		m_componentPool[componentIndex] = std::make_shared<T>();
	}
}


#endif