#ifndef TEXTELEMENTRESPONSESCRIPT_H
#define TEXTELEMENTRESPONSESCRIPT_H

#include"../ECS/Base/BasesScript.h"

#include<HAPI_lib.h>
using namespace HAPISPACE;

class TextElementInteractionResponseScript final :public  BaseScript
{
public:
	void InitializeScript(const HAPI_TColour& idleColorArg, const HAPI_TColour& onHoverColorArg, const HAPI_TColour& onClickColorArg, const HAPI_TColour& tagComponentColorArg)
	{
		m_idleColor = idleColorArg;
		m_onHoverColor = onHoverColorArg;
		m_onClickColor = onClickColorArg;
		m_tagComponentColor = tagComponentColorArg;
	}

	void FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID) override;

	void ReInitialize() override;

	std::shared_ptr<BaseScript> CloneScript() override;

private:
	HAPI_TColour
		m_idleColor{ HAPI_TColour::WHITE },
		m_onHoverColor{ HAPI_TColour::WHITE },
		m_onClickColor{ HAPI_TColour::WHITE },
		m_tagComponentColor{ HAPI_TColour::WHITE };

	//note signed int for negative range
	static int32_t currentTaggedEntity;


};

#endif