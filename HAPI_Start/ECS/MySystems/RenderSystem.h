#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include"../base/System.h"
#include"../ECS/MyComponents/RenderComponent.h"



class RenderSystem final : public System
{

public:
	RenderSystem(const UpdatePriority& updatePriority);

	void ResetComponent(const uint32_t& entityId, ECS_Engine& ecs) override;


private:
	std::shared_ptr<BaseComponent> DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)override;

};

#endif
