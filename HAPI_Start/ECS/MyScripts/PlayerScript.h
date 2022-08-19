#ifndef DRAGONSCRIPT_H
#define DRAGONSCRIPT_H

#include"../ECS/Base/BasesScript.h"

class PlayerScript final : public BaseScript
{
public:
	/*
	Initial Setup functions for script
	*/
	void FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID)override;

	void ReInitialize() override;

	std::shared_ptr<BaseScript> CloneScript() override;

	void GivePlayerScore(const unsigned int& points = 0u);
	const unsigned int& GetPlayerScore()const { return m_playerScore; };

	bool m_positionSet{false};
private:
	void UpdateDynamics(ECS_Engine& ecs, const uint32_t& entityID);
	void UpdatePosition(ECS_Engine& ecs, const uint32_t& entityID);
	void UpdateCollision(ECS_Engine& ecs, const uint32_t& entityID);


	unsigned int m_playerScore{0u};
};

#endif