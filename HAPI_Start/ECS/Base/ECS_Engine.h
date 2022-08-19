#ifndef ECS_ENGINE_H
#define ECS_ENGINE_H

#include<memory>
#include<unordered_map>
#include<typeindex>
#include<vector>
#include"System.h"
#include<iostream>
#include<queue>
#include<string>
#include<assert.h>

#include"../Utils/Utils.h"

/*
This class is the interface to the ECS design pattern
*/

class UI_Manager;
class ECS_Engine
{
public:

	friend bool Utils::SaveEcsToXML(const std::string& xmlFileNameArg, ECS_Engine& ecsArg, ECS_Engine& prefabEcsArg);

	//ctors and dtors
	ECS_Engine();
	~ECS_Engine() = default;


	/*
	Entity Methods
	*/

	const uint32_t& InstantiateEntity();

	//loading into local storage from a foreign system
	void LoadEntityFromSystemIntoSystem(const uint32_t entityID, const uint32_t& foreignEntityID, ECS_Engine& foreignEngine);

	void TagEntity(const std::string& tagName, const uint32_t& entityID);
	const std::vector<uint32_t>& GetTaggedEntities(const std::string& tagName);

	void DeletePrefabInstansiatedEntities(ECS_Engine& prefabEngine);
	void DeleteEntity(const uint32_t& entityID);
	void DeleteAllEntities();
	/*
	Component Methods
	*/
	//gets typeid of template type, does a search on the system map, validates system, searches system for component then casts to template type
	template<class T>
	std::shared_ptr<T> GetComponent(const uint32_t& entityID);

	//gets typeid of template, does a search in system map, valdates system, adds component to system, casts the return value to the template type
	template<class T>
	std::shared_ptr<T> AddComponent(const uint32_t& entityID);

	template<class T>
	std::shared_ptr<T> InstansiateComponent();

	template<class T>
	void MapComponentToSystem(const std::type_index& componentTypeID, const System::UpdatePriority& systemUpdatePriority);

	/*
	System
	*/
	template<class T>
	std::shared_ptr<T> GetSystem();

	void MapSystemToComponent(const std::type_index& systemTypeID, const std::type_index& componentTypeID);

	/*
	ECS Main
	*/
	//update each entities components 
	void UpdateECS(const float& deltaTime, RenderManager& renderManager, UI_Manager& uiManager, ECS_Engine& forignECS);

	std::list<uint32_t>& GetEntitiesQueuedToBeDeleted() { return this->m_EntitiesQueuedToBeDeleted; };

	static constexpr size_t maxEntities{ 2000 };


private:
	void DeleteQueuedEntities();

	static bool Compare(const std::shared_ptr<System>& lhs, const std::shared_ptr<System>& rhs);

	//allways set to the ID of the next entity to be made
	uint32_t m_nextEntityID{ 0 };

	std::list<uint32_t> m_ECS_Entity_DataStore{};
	std::list<uint32_t> m_EntitiesQueuedToBeDeleted{};
	std::unordered_map<std::string, std::vector<uint32_t>> m_ECS_TaggedEntity_IDs{};


	//more memory + less processing time  when not having 2 hash maps + vector vs less memory + more processing time when having 1 hash map
	std::unordered_map<std::type_index, std::shared_ptr<System>> m_ECS_ComponentID_To_System;
	std::unordered_map<std::type_index, std::shared_ptr<System>> m_ECS_SystemID_To_System;
	std::vector <std::shared_ptr<System>> m_SystemUpdateOrder;



};





template<class T>
inline std::shared_ptr<T> ECS_Engine::GetComponent(const uint32_t& entityID)
{
	//if component dosent exist
	//if (m_ECS_ComponentID_To_System.find(typeid(T)) == m_ECS_ComponentID_To_System.end())
	//{
	//	//report error
	//	std::cerr << std::string("Could not retrieve component as it is not indexed by its type id!");
	//	return nullptr;
	//}

	//get the typeid of the template type and search for a system under that type
	std::shared_ptr<System> system = m_ECS_ComponentID_To_System[typeid(T)];
	assert(system != nullptr);

	//if a system has been found
	//if (system)//retrieve the component from that system
	return std::static_pointer_cast<T>(system->GetComponentEngine().GetComponent(entityID));

	return nullptr;
}

template<class T>
inline std::shared_ptr<T> ECS_Engine::AddComponent(const uint32_t& entityID)
{
	//if component already exists
	if (m_ECS_ComponentID_To_System.find(typeid(T)) == m_ECS_ComponentID_To_System.end())
	{
		std::cerr << std::string("Can not add component as its type is not indexed by its type id!");
		return nullptr;
	}

	//get system pointer
	std::shared_ptr<System> system = m_ECS_ComponentID_To_System[typeid(T)];


	//if system valid
	if (system)
	{
		//hold component tempo
		std::shared_ptr<T> component = system->GetComponentEngine().InstansiateComponent<T>();
		system->m_componentEngine.AttachComponentToEntity(entityID, component);
		return component;
	}

	return nullptr;
}

template<class T>
inline std::shared_ptr<T> ECS_Engine::InstansiateComponent()
{
	//if component already exists
	if (m_ECS_ComponentID_To_System.find(typeid(T)) == m_ECS_ComponentID_To_System.end())
	{
		std::cerr << std::string("Can not add component as its type is not indexed by its type id!");
		return nullptr;
	}

	//get system pointer
	std::shared_ptr<System> system = m_ECS_ComponentID_To_System[typeid(T)];


	//if system valid
	if (system)
		return system->GetComponentEngine().InstansiateComponent<T>();

	return nullptr;
}

template<class T>
inline void ECS_Engine::MapComponentToSystem(const std::type_index& componentTypeID, const System::UpdatePriority& systemUpdatePriority)
{
	//if component record already exists
	if (m_ECS_ComponentID_To_System.find(componentTypeID) != m_ECS_ComponentID_To_System.end())
	{
		std::cerr << std::string("Component record already exists!\n");
		return;
	}

	m_ECS_ComponentID_To_System[componentTypeID] = std::make_shared<T>(systemUpdatePriority);
}

template<class T>
inline std::shared_ptr<T> ECS_Engine::GetSystem()
{
	//if system dosent exist in the data store
	if (m_ECS_SystemID_To_System.find(typeid(T)) == m_ECS_SystemID_To_System.end())
	{
		std::cerr << std::string("The system you are searching for can not be found!\n");
		return nullptr;
	}

	//get the system// system casted to type of template
	std::shared_ptr<T> system = std::static_pointer_cast<T>(m_ECS_SystemID_To_System[typeid(T)]);
	if (system == nullptr)
		std::cerr << std::string("The system record was not valid\n");
	
	return system;

}


#endif
