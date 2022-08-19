#ifndef CAMERASCRIPT_H
#define CAMERASCRIPT_H

#include"../ECS/Base/BasesScript.h"

class CameraScript final : public BaseScript
{
public:
	/*
	Initial setup
	*/
	void FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID) override;

	void ReInitialize() override;

	std::shared_ptr<BaseScript> CloneScript() override;

};

#endif