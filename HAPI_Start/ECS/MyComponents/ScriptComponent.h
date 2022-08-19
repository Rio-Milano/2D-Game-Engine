#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include"../ECS/Base/BaseComponent.h"
#include"../ECS/Base/BasesScript.h"

#include<memory>

struct ScriptComponent final : BaseComponent
{

	void InitializeScriptComponent(const std::shared_ptr<BaseScript>& scriptComponent)
	{
		m_script = scriptComponent;
		//if the script is known
		if (scriptComponent != nullptr)
			//set a flag to say we are attached to a script alloocated by the script allocator singleton
			this->attributesExternalLinkage = true;
		//if the script is not known then set the flag to false
		else
			this->attributesExternalLinkage = false;
	};

	std::shared_ptr<BaseScript> m_script{nullptr};
};

#endif