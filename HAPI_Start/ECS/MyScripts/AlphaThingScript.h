#ifndef ALPHATHINGSCRIPT_H
#define ALPHATHINGSCRIPT_H

#include"../ECS/Base/BasesScript.h"

class AlphaThingScript final : public BaseScript
{
public:
	/*
	Initial Setup functions for script
	*/
	void InitializeScript(const float& alphaThingVelocity = 0.0f) { m_alphaThingVelocity = alphaThingVelocity; };

	void FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID) override;

	void ReInitialize() override;

	std::shared_ptr<BaseScript> CloneScript() override;

private:
	float m_alphaThingVelocity{};
};

#endif