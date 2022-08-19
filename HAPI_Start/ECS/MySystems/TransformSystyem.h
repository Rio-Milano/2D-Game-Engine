#ifndef TRANSFORMSYSTEM_H
#define TRANSFORMSYSTEM_H

#include"../Base/System.h"
#include"../MyComponents/TransformComponent.h"
#include"../GeneralObjects/Rect.h"

class TransformSystem final: public System
{
public:
	/*
	SEE SYSTEM CLASS FOR COMMENTS
	*/

	//ctors and dtors
	TransformSystem(const UpdatePriority& updatePriority);
	~TransformSystem() = default;


	/*
	Overriding Virtual Functions
	*/
	void ResetComponent(const uint32_t& entityID, ECS_Engine& ecs) override;

private:
	std::shared_ptr<BaseComponent> DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)override;



};

#endif

