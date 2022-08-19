#ifndef BASESCRIPT_H
#define BASESCRIPT_H

class ECS_Engine;
class UI_Manager;
#include"../Managers/RenderManager.h"
#include<memory>

class BaseScript
{
public:
	BaseScript() = default;
	~BaseScript() = default;
	
	//this function works under fixed timestep found in visualization run function. Once derrived custom functionallity can be achieved
	virtual void FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID) = 0;

	virtual void ReInitialize() = 0;

	virtual std::shared_ptr<BaseScript> CloneScript() = 0;

	virtual void OnCollide(const uint32_t& entityID, ECS_Engine& ecs, ECS_Engine& forignECS, RenderManager& renderManager) {};

	virtual void OnDeath(ECS_Engine& ecs) {};

	bool isScriptInUse{ false };

};

#endif