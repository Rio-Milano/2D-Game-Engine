#include "PriorityQueue.h"
#include"../GeneralObjects/Graph.h"
#include"../GeneralObjects/Node.h"

void PriorityQueue::SiftUp(int PQi, const std::vector<std::vector<std::shared_ptr<Node>>>& graph)
{
	//Calculate where the parent of the given node is
	int parent_ofVertex = int(floor(PQi / 2));

	//While the parent points to a valid node
	while
		(parent_ofVertex != 0 &&

			//the parent of the node has a bigger F cost than the current node
			(
				graph[(heap[parent_ofVertex].y)][heap[parent_ofVertex].x]->Get_F_Cost_() >
				graph[heap[PQi].y][heap[PQi].x]->Get_F_Cost_()
				//or
				||
				//the F costs are the same but the parents H cost is bigger than the current node
				(

					graph[(heap[parent_ofVertex].y)][heap[parent_ofVertex].x]->Get_F_Cost_() ==
					graph[heap[PQi].y][heap[PQi].x]->Get_F_Cost_() &&

					graph[(heap[parent_ofVertex].y)][heap[parent_ofVertex].x]->m_h_cost >
					graph[heap[PQi].y][heap[PQi].x]->m_h_cost
					)

				)
			)


	{
		//swap the parent and the given node
		SwapNodes(parent_ofVertex, PQi, graph);

		//Update new position of given node and calculate new position of parent
		PQi = parent_ofVertex;
		parent_ofVertex = int(floor(PQi / 2));
	}
}

void PriorityQueue::Siftdown(int PQi, const std::vector<std::vector<std::shared_ptr<Node>>>& graph)
{
	//positions of nodes needed to use
	int leftChildPos;
	int rightChildPos;
	int currentNodePos;


	//While the node is not in the correct position in heap
	while (1)
	{
		leftChildPos = PQi * 2;//get children positions
		rightChildPos = PQi * 2 + 1;
		currentNodePos = PQi;//get the current node position

		//If the node has no childen then it is in the correct place
		if (leftChildPos >= nextFreePosInPrioityQueue && rightChildPos >= nextFreePosInPrioityQueue)
			return;

		//If the node only has 1 left child
		else if (rightChildPos >= nextFreePosInPrioityQueue)
		{
			//Check if that left child is smaller in priority than the current node, if so swap it
			if (graph[heap[leftChildPos].y][heap[leftChildPos].x]->Get_F_Cost_() < graph[heap[currentNodePos].y][heap[currentNodePos].x]->Get_F_Cost_()
				||//OR
				(
					//the left child has the same priority as the current node but the left child has a lower H cost than the current node
					graph[(heap[leftChildPos].y)][heap[leftChildPos].x]->Get_F_Cost_() ==
					graph[heap[currentNodePos].y][heap[currentNodePos].x]->Get_F_Cost_() &&

					graph[(heap[leftChildPos].y)][heap[leftChildPos].x]->m_h_cost <
					graph[heap[currentNodePos].y][heap[currentNodePos].x]->m_h_cost
					))
			{
				//swap the nodes
				SwapNodes(graph[heap[leftChildPos].y][heap[leftChildPos].x]->pos_in_PQ, PQi, graph);//swap the left child and the given node

				//update the current node position
				PQi = graph[heap[leftChildPos].y][heap[leftChildPos].x]->pos_in_PQ;
			}

			else   //If the left node was not smaller then return as node is in correct place
				return;
		}

		//if the node has 2 children then check if either one of them is smaller than the current node
		else
		{
			if (graph[heap[currentNodePos].y][heap[currentNodePos].x]->Get_F_Cost_() > graph[heap[leftChildPos].y][heap[leftChildPos].x]->Get_F_Cost_() ||
				graph[heap[currentNodePos].y][heap[currentNodePos].x]->Get_F_Cost_() > graph[heap[rightChildPos].y][heap[rightChildPos].x]->Get_F_Cost_())
			{
				//if the left child is smaller
				if (graph[heap[leftChildPos].y][heap[leftChildPos].x]->Get_F_Cost_() <
					graph[heap[rightChildPos].y][heap[rightChildPos].x]->Get_F_Cost_())
				{

					//swap the left child with the given node
					SwapNodes(graph[heap[leftChildPos].y][heap[leftChildPos].x]->pos_in_PQ, PQi, graph);

					//Update new positions in priority queue
					PQi = graph[heap[leftChildPos].y][heap[leftChildPos].x]->pos_in_PQ;
				}
				else {
					//Swap current node with the right child
					SwapNodes(graph[heap[rightChildPos].y][heap[rightChildPos].x]->pos_in_PQ, PQi, graph);

					//Update new positions in priority queue
					PQi = graph[heap[rightChildPos].y][heap[rightChildPos].x]->pos_in_PQ;

				}
			}
			else//we cant make a swap bassed on F cost as they either both the same and OR == to the F cost of the current node
			{
				//if the node's F cost is smaller than both the left and right child then the node is in the correct place
				if (graph[heap[currentNodePos].y][heap[currentNodePos].x]->Get_F_Cost_() < graph[(heap[leftChildPos].y)][heap[leftChildPos].x]->Get_F_Cost_() &&
					graph[heap[currentNodePos].y][heap[currentNodePos].x]->Get_F_Cost_() < graph[(heap[rightChildPos].y)][heap[rightChildPos].x]->Get_F_Cost_())
					return;

				//the nodes F cost is the same as the f cost of the left and right child so use H cost

				if (graph[(heap[leftChildPos].y)][heap[leftChildPos].x]->m_h_cost < graph[heap[currentNodePos].y][heap[currentNodePos].x]->m_h_cost ||
					graph[(heap[rightChildPos].y)][heap[rightChildPos].x]->m_h_cost < graph[heap[currentNodePos].y][heap[currentNodePos].x]->m_h_cost)
				{
					//if the left node H cost is smaller then right node H cost
					if (graph[(heap[leftChildPos].y)][heap[leftChildPos].x]->m_h_cost < graph[(heap[rightChildPos].y)][heap[rightChildPos].x]->m_h_cost)
					{
						//swap with left child
						SwapNodes(graph[heap[leftChildPos].y][heap[leftChildPos].x]->pos_in_PQ, PQi, graph);

						//Update new positions in priority queue
						PQi = graph[heap[leftChildPos].y][heap[leftChildPos].x]->pos_in_PQ;
					}
					//else swap with right
					else if (graph[(heap[leftChildPos].y)][heap[leftChildPos].x]->m_h_cost > graph[(heap[rightChildPos].y)][heap[rightChildPos].x]->m_h_cost)
					{
						//Swap current node with the right child
						SwapNodes(graph[heap[rightChildPos].y][heap[rightChildPos].x]->pos_in_PQ, PQi, graph);

						//Update new positions in priority queue
						PQi = graph[heap[rightChildPos].y][heap[rightChildPos].x]->pos_in_PQ;
					}
					else
						return;

				}
				else return;

			}
		}
	}
}

void PriorityQueue::AddToHeap(const Vector2i& nodeIndex, const std::vector<std::vector<std::shared_ptr<Node>>>& graph)
{
	if (nextFreePosInPrioityQueue != maxPosInPriorityQueue)
	{
		//Add the entry into the next free position in the heap
		heap[nextFreePosInPrioityQueue] = nodeIndex;

		//set the new node pos in PQ
		graph[nodeIndex.y][nodeIndex.x]->pos_in_PQ = nextFreePosInPrioityQueue;

		//increment next avaliable space
		nextFreePosInPrioityQueue++;

		//Heapify the node
		SiftUp(nextFreePosInPrioityQueue - 1, graph);
	}
}

const Vector2i PriorityQueue::DeleteMin(const std::vector<std::vector<std::shared_ptr<Node>>>& graph)
{
	//if the heap is empty
	if (nextFreePosInPrioityQueue == 1)
	{
		//report error
		return{ -1, -1 };
	}
	else
	{
		//if heap is not empty
		Vector2i temp = heap[1];//Take the node from the top

		//place the end node at the start of heap
		heap[1] = heap[nextFreePosInPrioityQueue - 1];//set root node = to farthest node

		//set the new start node pos in PQ to 1
		graph[heap[1].y][heap[1].x]->pos_in_PQ = 1;//update the nodes position in the priority queue

		//decrement the next free in PQ
		nextFreePosInPrioityQueue--;//make farthest most node as an avaliable spot

		//If there is still nodes in the Priority Queue then heapify the current node
		if (nextFreePosInPrioityQueue != 1)
			Siftdown(1, graph);

		//Return the min node
		return temp;
	}
}

const Vector2i PriorityQueue::PeakNextMin()
{
	//if the heap is empty
	if (nextFreePosInPrioityQueue == 1)
	{
		//report error
		return { -1, -1 };
	}
	//if the heap is not empty then return the top of the heap
	return { heap[1].y, heap[1].x };
}

void PriorityQueue::SwapNodes(const int PQia, const int PQib, const std::vector<std::vector<std::shared_ptr<Node>>>& graph)
{
	//Swap the Vertices at the 2 given positions
	Vector2i temp = heap[PQia];
	heap[PQia] = heap[PQib];
	heap[PQib] = temp;

	//Update positions of the swapped vertices in priority queue
	graph[heap[PQib].y][heap[PQib].x]->pos_in_PQ = PQib; //>posInPQ = given_PriorityQueuePosition_B;
	graph[heap[PQia].y][heap[PQia].x]->pos_in_PQ = PQia; //>posInPQ = given_PriorityQueuePosition_B;
}

void PriorityQueue::ClearHeap()
{
	//set the next free to the first pos in the heap
	nextFreePosInPrioityQueue = 1;
}

