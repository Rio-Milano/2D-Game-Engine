#ifndef BULLETSCRIPT_H
#define BULLETSCRIPT_H

#include"../ECS/Base/BasesScript.h"

class BulletScript final : public BaseScript
{
public:
	void FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID) override;

	void ReInitialize() override;

	std::shared_ptr<BaseScript> CloneScript() override;

	void OnCollide(const uint32_t& entityID, ECS_Engine& ecs, ECS_Engine& forignECS, RenderManager& renderManager) override;


};

#endif