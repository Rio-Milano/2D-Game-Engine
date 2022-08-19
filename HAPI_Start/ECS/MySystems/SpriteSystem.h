#ifndef SPRITESYSTEM_H
#define SPRITESYSTEM_H

#include"../Base/System.h"
#include"../MyComponents/SpriteComponent.h"

class SpriteSystem : public System
{
public:
	/*
	SEE SYSTEM CLASS FOR COMMENTS
	*/
	SpriteSystem(const UpdatePriority& updatePriority);
	~SpriteSystem() = default;

	/*
	Overriding Virtual Functions
	*/
	void ResetComponent(const uint32_t& entityID, ECS_Engine& ecs) override;

private:
	std::shared_ptr<BaseComponent> DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)override;


};

#endif
