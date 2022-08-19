#ifndef AISYSTEM_H
#define AISYSTEM_H

#include"../ECS/Base/System.h"

#include"../GeneralObjects/Graph.h"
class TransformSystem;
class CollisionSystem;

enum class AIDirection
{
	N,
	E,
	S,
	W,
	ZERO
};

class AISystem final : public System
{
public:
	AISystem(const UpdatePriority& updatePriority);
	~AISystem() = default;

	void ResetComponent(const uint32_t& entityID, ECS_Engine& ecs)override;
	void StartOfSystemUpdate()override;

	AIDirection GetEntityDirection(const uint32_t entityID, ECS_Engine& ecs) const;

	//this function create a graph for path finding to use. Should be called once world has been made
	//graph density is the value we divide the world width and height by to get a number of nodes in the graph to generate. The higher the value the fewer nodes in  the graph. I.e. 1 would give a ratio of 1 node per world unit
	void InitialisePathfindingGraph(const float& graphDensity, const std::shared_ptr<CollisionSystem>& collisionSystem);

	//used after loading a new level so old data is discarded
	void ClearGraph();
	std::vector<std::vector<std::shared_ptr<Node>>>& GetNodeGraph() { return this->m_graph.GetNodeGraph(); };

private:
	void UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs,  ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime)override;
	void SeccondUpdateComponent(const uint32_t& entityID, ECS_Engine& ecs, ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime)override;

	std::shared_ptr<BaseComponent> DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)override;

	Graph m_graph;
};

#endif