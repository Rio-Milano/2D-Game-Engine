#ifndef GRAPH_H
#define GRAPH_H

#include<vector>
#include<memory>

#include"../DataStructures/Vector.h"
#include"../GeneralObjects/Rect.h"

#include"../DataStructures/PriorityQueue.h"
struct Node;
class ECS_Engine;
class CollisionSystem;
class RenderManager;

class Graph
{
public:
	friend class AISystem;

	Graph() = default;
	~Graph() = default;

	void InstansiateGraph();
	void PermanentlyLockStaticCollidables(const std::shared_ptr<CollisionSystem>& collisionSystem);

	std::shared_ptr<Node> GetClosestNodePointFromPoint(const Vector2f& point, const uint32_t& entityID);

	//algorithm to prime the grid with data that can be used for path fetching 
	std::vector<Vector2f> GetNextPointInPath(const Vector2f& currentPosition, const Vector2f& goalPosition, const uint32_t entityID, ECS_Engine& ecs, ECS_Engine& forignEcs, RenderManager& renderManager);


	std::vector<std::vector<std::shared_ptr<Node>>>& GetNodeGraph() { return this->m_graph; };
	void SetGraphMeta(const FRect& worldBounds, const float& graphDensity);
	void Clear();

private:
	void PrimeGraphForPathFinding();

	std::vector<std::shared_ptr<Node>> GetAdjacentNodes(const Vector2i& nodeIndex);
	std::vector<std::vector<std::shared_ptr<Node>>> m_graph;

	PriorityQueue m_open_set;
	FRect m_worldBounds;
	float m_graphDensity{ 1.0f };
};

#endif