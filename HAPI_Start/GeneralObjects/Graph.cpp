#include "Graph.h"

#include"../ECS/Base/Core_ECS_Assets.h"
#include"../ECS/Base/ECS_Engine.h"
#include"../Managers/RenderManager.h"
#include"../DataStructures/PriorityQueue.h"
#include"Node.h"

#include<math.h>



void Graph::InstansiateGraph()
{
	//define a vector to hold the path
	std::vector<Vector2f> path_;

	//remember to call Meta function before this function so it can work properly

	const size_t
		graphRows{ static_cast<size_t>(this->m_worldBounds.GetRectHeight()/this->m_graphDensity)+1 },
		graphColumns{static_cast<size_t>(this->m_worldBounds.GetRectWidth()/this->m_graphDensity)+1};

	float
		yN{ this->m_worldBounds.GetRectHeight() / this->m_graphDensity },
		yWhole{ std::floor(yN) },
		yFractional{ std::modf(yN, &yWhole) },

		xN{ this->m_worldBounds.GetRectWidth() / this->m_graphDensity },
		xWhole{ std::floor(xN) },
		xFractional{ std::modf(xN, &xWhole) };

	//size to rows
	m_graph.resize(graphRows);

	//loop on rows and fill to col size
	for (std::vector<std::shared_ptr<Node>>& row : m_graph)
		row.resize(graphColumns);

	Vector2f topLeftOfWorld{this->m_worldBounds.GetPosition()};

	//loop entire graphs nodes to assign memory to them
	
	//loop graph rows
	int row{0};
	for (std::vector<std::shared_ptr<Node>>& graphRow : m_graph)
	{
		//loop nodes on row
		int column{0};
		for (std::shared_ptr<Node>& node : graphRow)
		{
			//make a new node and assign to node in graph
			node = std::make_shared<Node>();
		
			node->m_worldPosition = Vector2f{topLeftOfWorld.x + column * (this->m_graphDensity + xFractional), topLeftOfWorld.y + row * (this->m_graphDensity + yFractional)};
			node->m_graphIndex = Vector2i(column, row);
			column++;
		}
		row++;
	}
}

void Graph::PermanentlyLockStaticCollidables(const std::shared_ptr<CollisionSystem>& collisionSystem)
{
	//loop all bound collision components that are in use
	for (const std::pair<uint32_t, std::shared_ptr<BaseComponent>>& collisionComponentRecord : collisionSystem->GetComponentEngine().m_Component_DataStore)
	{
		//take base component and cast it to a collision component
		std::shared_ptr<CollisionComponent> collisionComponent{std::static_pointer_cast<CollisionComponent>(collisionComponentRecord.second)};

		//if the component is a static
		if (!collisionComponent->m_isDynamicCollidable)
		{
			//loop through every node in the graph
			for (std::vector<std::shared_ptr<Node>>& graphRow : this->m_graph)
			{
				for (std::shared_ptr<Node>& graphNode : graphRow)
				{
					if (collisionComponent->m_Rect.IsPointLyingOnRect(graphNode->m_worldPosition))
						graphNode->m_isPermanentlyLocked = true;
				}
			}
		}
	}
}

std::shared_ptr<Node>Graph::GetClosestNodePointFromPoint(const Vector2f& point, const uint32_t& entityID)
{
	if (this->m_graph.empty())
		return nullptr;

	Vector2f WorldStart_TO_Point{ point - this->m_worldBounds.GetPosition() };

	int
		graphRow = int(std::round(WorldStart_TO_Point.y / this->m_graphDensity)),
		graphColumn = int(std::round(WorldStart_TO_Point.x / this->m_graphDensity));

	if (graphRow < 0 || graphRow >= this->m_graph.size() || graphColumn < 0 || graphColumn >= this->m_graph[0].size())
		return nullptr;

	std::shared_ptr<Node> node{ this->m_graph[graphRow][graphColumn] };
	if (!node->m_isPermanentlyLocked  && (node->m_collideWithEntity == -1 || (node->m_collideWithEntity == entityID)))
		return node;

	for (std::shared_ptr<Node> adjNode : GetAdjacentNodes(Vector2i(graphRow, graphColumn)))
	{
		if (!adjNode->m_isPermanentlyLocked && (adjNode->m_collideWithEntity == -1 || (adjNode->m_collideWithEntity == entityID)))
			return adjNode;
	};



	return nullptr;

	
}

std::vector<Vector2f> Graph::GetNextPointInPath(const Vector2f& currentPosition, const Vector2f& goalPosition, const uint32_t entityID, ECS_Engine& ecs, ECS_Engine& forignEcs, RenderManager& renderManager)
{
	int currentSearchCount{ 0 }, maxSearchCount{ 40 };
	//get collision system for branching to other nodes
	std::shared_ptr<CollisionSystem> collisionSystem{ ecs.GetSystem<CollisionSystem>() };
	assert(collisionSystem != nullptr);

	//resets all node attributes so a new path can be made
	PrimeGraphForPathFinding();

	// Clear the open set for the next run of the Alg
	m_open_set.ClearHeap();

	//define a vector to hold the path
	std::vector<Vector2f> path_;

	//define a current node
	std::shared_ptr<Node> 
		current_node_{ this->GetClosestNodePointFromPoint(currentPosition, entityID) },
		goal_node_{ this->GetClosestNodePointFromPoint(goalPosition, entityID) };


	if (goal_node_ == nullptr || current_node_ == nullptr)
		return path_;


	//init the start vertex
	current_node_->m_g_cost = 0;

	//add the start vertex onto the open set
	m_open_set.AddToHeap(current_node_->m_graphIndex, this->m_graph);

	//while the open set has nodes
	while (m_open_set.size() > 0)
	{
		if (currentSearchCount++ == maxSearchCount)
			return path_;

		//get the current node
		Vector2i optimal_node_{ m_open_set.DeleteMin(this->m_graph) };
		current_node_ = this->m_graph[optimal_node_.y][optimal_node_.x];//m_open_set[0];

		//lock the current node
		current_node_->m_locked = true;

		//if the current node is found then end the alg
		if (current_node_ == goal_node_)
			break;

		//loop through all adjacent nodes
		for (const std::shared_ptr<Node>& adj : GetAdjacentNodes(current_node_->m_graphIndex))
		{
			//if node has been locked by dynamic collidable
			if (adj->m_isPermanentlyLocked)
				continue;
			//if node has been processed
			if (adj->m_locked)
				continue;
			//if node is being used by another entity
			if (adj->m_collideWithEntity != -1)
			{
				//if entity using node is not current entity
				if (adj->m_collideWithEntity != entityID)
					continue;
			}


			std::shared_ptr<CollisionComponent> collisionComponent{ecs.GetComponent<CollisionComponent>(entityID)};
			FRect R1{collisionComponent->m_Rect};
			FRect R2{collisionComponent->m_Rect};

			Vector2f translate_R1_ToCurrentNode{current_node_->m_worldPosition};
			translate_R1_ToCurrentNode.x -= R1.GetRectWidth() / 2.f;
			translate_R1_ToCurrentNode.y -= R1.GetRectHeight() / 2.f;
			R1.Translate(translate_R1_ToCurrentNode);

			Vector2f translate_R2_ToAdjNode{ adj->m_worldPosition };
			translate_R2_ToAdjNode.x -= R2.GetRectWidth() / 2.f;
			translate_R2_ToAdjNode.y -= R2.GetRectHeight() / 2.f;
			R2.Translate(translate_R2_ToAdjNode);

			FRect tempCollisionComponentRect{collisionComponent->m_Rect};
			collisionComponent->m_Rect = R2;

			if (collisionSystem->IsColliding(entityID, ecs, forignEcs, renderManager))
			{
				if (collisionComponent->m_CollidingWith != ecs.GetTaggedEntities("Player")[0])
				{
					collisionComponent->m_Rect = tempCollisionComponentRect;
					continue;
				}
			}

			collisionComponent->m_Rect = tempCollisionComponentRect;


			//Taking up too much performance so ignorning diagonal moves meaning this is not needed
			//if (
			//	collisionSystem->IsCollidingWithLineSegment(entityID, ecs, R1.x1, R1.y1, R2.x1, R2.y1) ||
			//	collisionSystem->IsCollidingWithLineSegment(entityID, ecs, R1.x2, R1.y1, R2.x2, R2.y1) ||
			//	collisionSystem->IsCollidingWithLineSegment(entityID, ecs, R1.x1, R1.y2, R2.x1, R2.y2) ||
			//	collisionSystem->IsCollidingWithLineSegment(entityID, ecs, R1.x2, R1.y2, R2.x2, R2.y2))
			//{
			//	collisionComponent->m_Rect = tempCollisionComponentRect;
			//	continue;
			//}
			//

			//if node is not discovered
			if (adj->m_discovered == false)
			{
				//make discovered
				adj->m_discovered = true;

				//set the h cost
				adj->m_h_cost = static_cast<float>(Vector2f::Distance(adj->m_worldPosition, goalPosition));

				//add node to open set
				m_open_set.AddToHeap(adj->m_graphIndex, this->m_graph);
			}

			//get distance to adj node
			float adj_cost_ = Vector2f::Distance(current_node_->m_worldPosition, adj->m_worldPosition);

			//if current node g cost + adj distance cost is less than the adj g cost
			if (adj_cost_ < adj->m_g_cost)
			{
				//set the g cost
				adj->m_g_cost = adj_cost_;

				//set the parent of adj
				adj->m_parent = current_node_;

				//sift up node as its attributes have been reduced in value
				m_open_set.SiftUp(adj->pos_in_PQ, this->m_graph);
			}
		}


	};
	this->m_graph;


	std::shared_ptr<Node> backTrackingNode{goal_node_};

	//while the end node has a parent 
	while (backTrackingNode->m_parent != nullptr)
	{
		if (backTrackingNode->m_parent->m_parent == nullptr)
		{
			backTrackingNode->m_parent->m_collideWithEntity = entityID;
		}

		path_.push_back(backTrackingNode->m_worldPosition);
		
		//update the node to hold its parent
		backTrackingNode = backTrackingNode->m_parent;
	}

	return path_;

}

void Graph::SetGraphMeta(const FRect& worldBounds, const float& graphDensity)
{
	this->m_worldBounds = worldBounds;
	this->m_graphDensity = graphDensity;
}

void Graph::Clear()
{
	m_graph.clear();
}


void Graph::PrimeGraphForPathFinding()
{
	//loop through each position of grid and reset their values to their default 
	for (const std::vector<std::shared_ptr<Node>>& graphRow : this->m_graph)
	{
		for (const std::shared_ptr<Node>& node : graphRow)
		{
			node->m_g_cost = INF;
			node->m_h_cost = INF;
			node->m_locked = false;
			node->m_discovered = false;
			node->m_parent = nullptr;
			node->pos_in_PQ = 1;
		}
	}
}

std::vector<std::shared_ptr<Node>> Graph::GetAdjacentNodes(const Vector2i& nodeIndex)
{

	std::vector<std::shared_ptr<Node>> adjecent_vertices_;
	//define a vector to hold vertices
	if (this->m_graph.size() < 1 || this->m_graph[0].size() < 1 || this->m_graph[0][0] == nullptr)
		return adjecent_vertices_;

	//loop through a 3x3 grid arround the vertex
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			////if the vertex is a diagonal then ignore it
			if ((i == 0 && j == 0) || (i == -1 && j == -1) || (i == -1 && j == 1) || (i == 1 && j == -1) || (i == 1 && j == 1))
				continue;

			//define the node position
			int adj_X = nodeIndex.x + j;
			int adj_Y = nodeIndex.y + i;

			//if vertex position is out of bounds then ignore it
			if (adj_X < 0 || adj_X >= this->m_graph[0].size() || adj_Y < 0 || adj_Y >= this->m_graph.size())
				continue;

			//add the vertex to the grid
			adjecent_vertices_.emplace_back(this->m_graph[adj_Y][adj_X]);
		}
	}
	//return the final list of ajacent vertices
	return adjecent_vertices_;
}
