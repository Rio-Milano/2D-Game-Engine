#ifndef NODE_H
#define NODE_H

#define INF 10000

#include<memory>

struct Node
{

	//node properties 
	bool
		m_locked{ false },
		m_discovered = { false };
	
	int32_t m_collideWithEntity{ -1 },//used when node collides with dynamic collidable
		m_isPermanentlyLocked{ false };//does node collide with static collidable rect

	Vector2f m_worldPosition{ 0.0f, 0.0f };
	Vector2i m_graphIndex{0, 0};

	//the heuristic values
	float	m_g_cost{ INF },
		m_h_cost{ INF };


	//gets the combined heuristic costs
	const float Get_F_Cost_() const { return (m_g_cost + m_h_cost); };

	//when running path finding this is used for backtracking 
	std::shared_ptr<Node> m_parent{ nullptr };

	//position in the heap
	int pos_in_PQ = 1;

};
#endif                                                                                                                                                                                                                                                                                                 