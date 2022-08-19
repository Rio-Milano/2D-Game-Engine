#ifndef SYSTEM_H
#define SYSTEM_H

#include<memory>
#include<list>
#include<unordered_map>
#include"BaseComponent.h"
#include"ComponentEngine.h"

/*
This is the base class to all of the systems 
*/

//forward decleration of ECS_Engine to fix circular reference 
class ECS_Engine;
class UI_Manager;
class RenderManager;
class System
{
public:
	friend class ECS_Engine;

	enum class UpdatePriority : unsigned int
	{
		VeryLow = 0,
		Low = 1 << 0,
		Medium = 1 << 1,
		High = 1 << 2,
		VeryHigh = 1 << 3
	};

	//ctors and dtors
	System(const UpdatePriority& UpdatePriority)
		:m_updatePriority(UpdatePriority) {};
	~System() = default;

	void SetSystemState(const bool& state) { this->m_systemState = state; };
	const bool& GetSystemState()const { return this->m_systemState; };
	//Puts component back to its default state
	virtual void ResetComponent(const uint32_t& entityID, ECS_Engine& ecs) = 0;

	void EnableSystem(ECS_Engine& ecs);
	void DisableSystem(ECS_Engine& ecs);

	virtual void StartOfSystemUpdate() {};
	virtual void EndOfSystemUpdate(){};

	virtual void SystemEnableProdecure(ECS_Engine& ecs) {};
	virtual void SystemDisableProcedure(ECS_Engine& ecs) {};
	

	//get the component engine
	ComponentEngine& GetComponentEngine();

private:
	UpdatePriority m_updatePriority;

	bool m_systemState{ true };

	//Updates component data
	virtual void UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs,  ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime) {};
	virtual void SeccondUpdateComponent(const uint32_t& entityID, ECS_Engine& ecs, ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime) {};

	//makes another componet, coppies data into it then returns it
	virtual std::shared_ptr<BaseComponent> DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS) = 0;

	bool m_useSeccondUpdate{false};

protected:

	ComponentEngine m_componentEngine;


	

};

#endif