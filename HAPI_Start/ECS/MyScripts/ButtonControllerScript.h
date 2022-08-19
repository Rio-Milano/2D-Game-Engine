#ifndef BUTTONCONTROLLERSCRIPT_H
#define BUTTONCONTROLLERSCRIPT_H

#include"../Base/BasesScript.h"

#include<HAPI_Types.h>
using namespace HAPISPACE;

class ButtonControllerScript final : public BaseScript
{
public:
	void FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID) override;

	void ReInitialize() override;

	std::shared_ptr<BaseScript> CloneScript() override;

public:

	bool m_useStateSettingColours{ !false };

	HAPI_TColour
		//idle state colours
		m_idleStateTextColour{HAPI_TColour::BLACK},
		m_idleStateButtonColour{ HAPI_TColour::RED },

		//hover state colours
		m_hoverStateTextColour{ HAPI_TColour::BLACK },
		m_hoverStateButtonColour{ HAPI_TColour(229, 83, 0) },

		//clicking state colours
		m_clickingStateTextColour{ HAPI_TColour::BLACK },
		m_clickingStateButtonColour{ HAPI_TColour::GREEN };
};

#endif