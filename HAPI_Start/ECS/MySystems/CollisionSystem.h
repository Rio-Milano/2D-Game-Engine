#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include"../ECS/Base/System.h"
#include"../GeneralObjects/Rect.h"

class CollisionSystem final : public System
{
public:
	CollisionSystem(const UpdatePriority& updatePriority);

	template<class T>
	Rect<T> GetWorldBounds()const;

	virtual std::shared_ptr<BaseComponent> DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS) override;
	const bool IsColliding(const uint32_t& entityID, ECS_Engine& ecs, ECS_Engine& forignEcs, RenderManager& renderManager);
	const bool IsCollidingWithLineSegment(const uint32_t& entityID, ECS_Engine& ecs, float s1_x, float s1_y, float s2_x, float s2_y);


private:
	bool LineIntersectsLine(Vector2f l1p1, Vector2f l1p2, Vector2f l2p1, Vector2f l2p2);
	void ResetComponent(const uint32_t& entityID, ECS_Engine& ecs) override;
	virtual void UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs,  ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime) override;
};


template<class T>
inline Rect<T> CollisionSystem::GetWorldBounds()const
{
	//create a bounds rect to return
	Rect<T> worldBoundsRect;

	//loop on components that arent in use
	for (const std::pair<uint32_t, std::shared_ptr<BaseComponent>>& inUseBaseComponent : this->m_componentEngine.m_Component_DataStore)
	{
		const FRect& otherRect{ std::static_pointer_cast<CollisionComponent>(inUseBaseComponent.second)->m_Rect };

		//top left of world
		if (otherRect.x1 < worldBoundsRect.x1)
			worldBoundsRect.x1 = static_cast<T>(otherRect.x1);
		if (otherRect.y1 < worldBoundsRect.y1)
			worldBoundsRect.y1 = static_cast<T>(otherRect.y1);

		//bottom right of world
		if (otherRect.x2 > worldBoundsRect.x2)
			worldBoundsRect.x2 = static_cast<T>(otherRect.x2);
		if (otherRect.y2 > worldBoundsRect.y2)
			worldBoundsRect.y2 = static_cast<T>(otherRect.y2);
	}
	return worldBoundsRect;
}
#endif

