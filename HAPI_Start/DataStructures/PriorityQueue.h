#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

class Graph;
struct Node;

#include"Vector.h"
#include<vector>
#include<memory>
/*
this class implements the min heap and is used for an efficient use of A* pathfinding
*/

class PriorityQueue
{

public:
	//ctors and dtors
	PriorityQueue() = default;
	~PriorityQueue() = default;

	/*
	Heapify ops
	*/

	//sift a node up the heap when adding nodes
	void SiftUp(int PQi, const std::vector<std::vector<std::shared_ptr<Node>>>& graph);

	//sift a node down the heap when removing nodes
	void Siftdown(int PQi, const std::vector<std::vector<std::shared_ptr<Node>>>& graph);

	//add a new node to the heap
	void AddToHeap(const Vector2i& nodeIndex, const std::vector<std::vector<std::shared_ptr<Node>>>& graph);

	//delete the min from the heap
	const Vector2i DeleteMin(const std::vector<std::vector<std::shared_ptr<Node>>>& graph);

	//get the min from the heap
	const Vector2i PeakNextMin();

	//remove all nodes from the heap
	void ClearHeap();

	//get the size of the heap
	const int size() { return nextFreePosInPrioityQueue - 1; }


private:
	//swap the 2 nodes at 2 positions
	void SwapNodes(const int PQia, const int PQib, const std::vector<std::vector<std::shared_ptr<Node>>>& graph);

	//get the next free position in the heap
	const int& getNextFree() const {return nextFreePosInPrioityQueue;};

	//the heap
	Vector2i heap[1000];//giving 1000 elements for the heap

	//the size of the heap
	int maxPosInPriorityQueue = 1000;//needed to know when heep is full
	int nextFreePosInPrioityQueue = 1;//heap is 1 bassed
};

#endif