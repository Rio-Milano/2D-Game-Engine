#ifndef TEXTSYSTEM_H
#define TEXTSYSTEM_H

#include"../Base/System.h"
#include"../MyComponents/TransformComponent.h"

class TextSystem final : public System
{
public:
	/*
	SEE SYSTEM CLASS FOR COMMENTS
	*/

	//ctors and dtors
	TextSystem(const UpdatePriority& updatePriority);
	~TextSystem() = default;

	/*
	Overriding Virtual Functions
	*/
	void ResetComponent(const uint32_t& entityID, ECS_Engine& ecs) override;

private:
	std::shared_ptr<BaseComponent> DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)override;



};

#endif
