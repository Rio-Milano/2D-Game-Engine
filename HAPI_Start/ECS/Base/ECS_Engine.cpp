

#include "ECS_Engine.h"

#include"Core_ECS_Assets.h"
#include"../Managers/RenderManager.h"
#include"../UI/UI Core/UI_Manager.h"
#include<iostream>
#include<algorithm>
#include<utility>


ECS_Engine::ECS_Engine()
{
#pragma region ECS Defined Systems
	//maps component type id to system locationx
	MapComponentToSystem<TransformSystem>(typeid(TransformComponent), System::UpdatePriority::VeryLow);
	MapComponentToSystem<TextSystem>(typeid(TextComponent), System::UpdatePriority::VeryLow);
	MapComponentToSystem<SpriteSystem>(typeid(SpriteComponent), System::UpdatePriority::VeryLow);
	MapComponentToSystem<AnimationSystem>(typeid(AnimationComponent), System::UpdatePriority::VeryLow);
	MapComponentToSystem<RenderSystem>(typeid(RenderComponent), System::UpdatePriority::VeryLow);
	MapComponentToSystem<EditorSystem>(typeid(EditorComponent), System::UpdatePriority::VeryLow);
	MapComponentToSystem<LifeSystem>(typeid(LifeComponent), System::UpdatePriority::VeryLow);
	MapComponentToSystem<SpawnerSystem>(typeid(SpawnerComponent), System::UpdatePriority::VeryLow);
	MapComponentToSystem<MovementSystem>(typeid(MovementComponent), System::UpdatePriority::Low);
	MapComponentToSystem<ScriptSystem>(typeid(ScriptComponent), System::UpdatePriority::Medium);
	MapComponentToSystem<AISystem>(typeid(AIComponent), System::UpdatePriority::Medium);
	MapComponentToSystem<EventSystem>(typeid(EventComponent), System::UpdatePriority::High);
	MapComponentToSystem<CollisionSystem>(typeid(CollisionComponent), System::UpdatePriority::VeryHigh);



	//maps system type id to system location
	MapSystemToComponent(typeid(TransformSystem), typeid(TransformComponent));
	MapSystemToComponent(typeid(TextSystem), typeid(TextComponent));
	MapSystemToComponent(typeid(SpriteSystem), typeid(SpriteComponent));
	MapSystemToComponent(typeid(AnimationSystem), typeid(AnimationComponent));
	MapSystemToComponent(typeid(RenderSystem), typeid(RenderComponent));
	MapSystemToComponent(typeid(MovementSystem), typeid(MovementComponent));
	MapSystemToComponent(typeid(ScriptSystem), typeid(ScriptComponent));
	MapSystemToComponent(typeid(CollisionSystem), typeid(CollisionComponent));
	MapSystemToComponent(typeid(EventSystem), typeid(EventComponent));
	MapSystemToComponent(typeid(EditorSystem), typeid(EditorComponent));
	MapSystemToComponent(typeid(AISystem), typeid(AIComponent));
	MapSystemToComponent(typeid(SpawnerSystem), typeid(SpawnerComponent));
	MapSystemToComponent(typeid(LifeSystem), typeid(LifeComponent));

	std::static_pointer_cast<System>(GetSystem<AISystem>())->m_useSeccondUpdate = true;

#pragma endregion


	//loop through each system
	for (const std::pair<std::type_index, std::shared_ptr<System>>& element : m_ECS_ComponentID_To_System)
		//add the system to the update order
		m_SystemUpdateOrder.emplace_back(element.second);

	//sort the system update by order precedence Low - > Medium - > High 
	std::sort(m_SystemUpdateOrder.begin(), m_SystemUpdateOrder.end(), Compare);


}



const uint32_t& ECS_Engine::InstantiateEntity()
{
		m_ECS_Entity_DataStore.emplace_back(m_nextEntityID++);
		return m_ECS_Entity_DataStore.back();
}




void ECS_Engine::LoadEntityFromSystemIntoSystem(const uint32_t entityID, const uint32_t& foreignEntityID, ECS_Engine& foreignEngine)
{
	std::shared_ptr<System>
		localSystem{nullptr},
		foreignSystem{nullptr};

	//the size of the foreign and local system will be the same
	for (size_t i = 0; i < m_SystemUpdateOrder.size(); i++)
	{
		localSystem = m_SystemUpdateOrder[i];
		foreignSystem = foreignEngine.m_SystemUpdateOrder[i];

		if (localSystem && foreignSystem)
		{
			if (foreignSystem->GetComponentEngine().GetComponent(foreignEntityID))
			{
				foreignSystem->DuplicateComponent(foreignEntityID, entityID, *this);
			}
		}
	}
}

void ECS_Engine::TagEntity(const std::string& tagName, const uint32_t& entityID)
{
	m_ECS_TaggedEntity_IDs[tagName].emplace_back(entityID);
}

const std::vector<uint32_t>& ECS_Engine::GetTaggedEntities(const std::string& tagName)
{
	return m_ECS_TaggedEntity_IDs[tagName];
}

void ECS_Engine::DeletePrefabInstansiatedEntities(ECS_Engine& prefabEngine)
{
	for (const std::pair<std::string, std::vector<uint32_t>>& tagRecord : m_ECS_TaggedEntity_IDs)
	{
		if (prefabEngine.GetTaggedEntities(tagRecord.first).size() > 0)
		{
			for (const uint32_t& prefabInstansiatedID : tagRecord.second)
				DeleteEntity(prefabInstansiatedID);
			
			m_ECS_TaggedEntity_IDs[tagRecord.first].clear();
		}
	}
}

void ECS_Engine::DeleteEntity(const uint32_t& entityID)
{
	this->m_EntitiesQueuedToBeDeleted.emplace_back(entityID);
}

void ECS_Engine::DeleteAllEntities()
{
	for (const uint32_t& entity : this->m_ECS_Entity_DataStore)
		DeleteEntity(entity);




}

void ECS_Engine::MapSystemToComponent(const std::type_index& systemTypeID, const std::type_index& componentTypeID)
{
	//if system already mapped
	if (m_ECS_SystemID_To_System.find(systemTypeID) != m_ECS_SystemID_To_System.end())
	{
		std::cerr << std::string("System already mapped to a system!\n");
		return;
	}

	//if component record not found
	if (m_ECS_ComponentID_To_System.find(componentTypeID) == m_ECS_ComponentID_To_System.end())
	{
		std::cerr << std::string("Component is not stored\n");
		return;
	}

	//if the system does not yet exist and the component record has been created
	m_ECS_SystemID_To_System[systemTypeID] = m_ECS_ComponentID_To_System[componentTypeID];
}



	
void ECS_Engine::UpdateECS(const float& deltaTime, RenderManager& renderManager, UI_Manager& uiManager, ECS_Engine& forignECS)
{
	//if an update component call throws exception it can be caught 
	try {
		//loop through all entities
			//loop through all systems

		renderManager.RemoveDeletedEntitiesFromRenderLayers(*this);
		DeleteQueuedEntities();

		std::vector<std::shared_ptr<System>>::iterator systemUpdateItr{this->m_SystemUpdateOrder.begin()};

		while(systemUpdateItr != this->m_SystemUpdateOrder.end())
		{
			std::shared_ptr<System> System{*systemUpdateItr};

			if (System != nullptr)
			{
				if (System->GetSystemState() == true)
				{
					System->StartOfSystemUpdate();
					for (const auto& Entity : m_ECS_Entity_DataStore)
					{
						//if system is v
							//if system contains entity
						if (System->GetComponentEngine().GetComponent(Entity))
						{//update entity componenent 
							System->UpdateComponent(Entity, *this, forignECS, uiManager, renderManager, deltaTime);
						}
					}
					if (System->m_useSeccondUpdate)
					{
						for (const auto& Entity : m_ECS_Entity_DataStore)
						{
							//if system is v
								//if system contains entity
							if (System->GetComponentEngine().GetComponent(Entity))
							{//update entity componenent 
								System->SeccondUpdateComponent(Entity, *this, forignECS, uiManager, renderManager, deltaTime);
							}
						}
					}
					System->EndOfSystemUpdate();
				}
			}
			systemUpdateItr++;
		}
	}
	catch (std::string& e)
	{
		std::cerr << "Error: " << e;
		return;
	}
}



void ECS_Engine::DeleteQueuedEntities()
{
	const int32_t& UI_Engaged_With{UI_Manager::GetEntityEngaged()};

	uint32_t entityID{};
	while (this->m_EntitiesQueuedToBeDeleted.size() > 0)
	{
		entityID = this->m_EntitiesQueuedToBeDeleted.front();
		m_EntitiesQueuedToBeDeleted.pop_front();

		//loop through each system
		for (const std::shared_ptr<System>& system_ : m_SystemUpdateOrder)
		{
			//if system holds a component belonging to the entity
			if (system_->GetComponentEngine().GetComponent(entityID) != nullptr)
			{
				if (UI_Engaged_With == entityID)
					UI_Manager::SetEntityEngaged(-1);

				//remove the component from the system
				system_->GetComponentEngine().RemoveComponent(entityID);
			}
		}
		//remove the entity ID from local storage
		m_ECS_Entity_DataStore.remove(entityID);

		//loop through tagged entity map
		std::unordered_map<std::string, std::vector<uint32_t>>::iterator taggedEntity{ m_ECS_TaggedEntity_IDs.begin() };
		for (; taggedEntity != m_ECS_TaggedEntity_IDs.end(); taggedEntity++)
		{
			//loop through tagged entity map list
			std::vector<uint32_t>::iterator taggedEntityList{ taggedEntity->second.begin() };
			for (; taggedEntityList != taggedEntity->second.end();)
			{
				//if list contains entity id
				if (*taggedEntityList == entityID)
					//remove id from list
					taggedEntityList = taggedEntity->second.erase(taggedEntityList);
				else
					taggedEntityList++;


			}
		}
	}
}

bool ECS_Engine::Compare(const std::shared_ptr<System>& lhs, const std::shared_ptr<System>& rhs)
{
	return lhs->m_updatePriority < rhs->m_updatePriority;
}


