#ifndef SILVERKNIGHTSCRIPT_H
#define SILVERKNIGHTSCRIPT_H

#include"../ECS/Base/BasesScript.h"

class SilverKnightScript final : public BaseScript
{
public:
	void FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID) override;

	void ReInitialize() override;

	std::shared_ptr<BaseScript> CloneScript() override;

	void OnDeath(ECS_Engine& ecs) override;

private:
	float shootCooldown{ 0.0f };

};

#endif