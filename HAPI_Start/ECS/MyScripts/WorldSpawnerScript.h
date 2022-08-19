#ifndef WORLDSPAWNERSCRIPT_H
#define WORLDSPAWNERSCRIPT_H

#include"../ECS/Base/BasesScript.h"
#include<memory>
#include<string>

class WorldSpawnerScript final : public BaseScript
{
public:
	void FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID) override;

	void ReInitialize() override;

	std::shared_ptr<BaseScript> CloneScript() override;





};

#endif