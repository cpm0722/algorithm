#include<stdio.h>
#include<iostream>
#define NONDECREASING_ORDER 0
#define NONINCREASING_ORDER 1

using namespace std;

class Edge {

private:
	int edgeValue;
	int startVertex;
	int endVertex;

public:

	Edge();
	Edge(int inputEdgeValue, int inputStartVertex, int inputEndVertex);
	~Edge() {}
	
	Edge operator=(Edge &b);

	friend ostream &operator <<(ostream &c, const Edge &edge);
	
	void setEdgeValue(int i) { 
		edgeValue = i; return; }
	void setStartVertex(int i) { 
		startVertex = i; return; }
	void setEndVertex(int i) { 
		endVertex = i; return; }

	int getEdgeValue(void) const { 
		return edgeValue; }
	int getStartVertex(void) const { 
		return startVertex; }
	int getEndVertex(void) const { 
		return endVertex; }

	void printEdge(void) const;

};

ostream &operator <<(ostream &c, const Edge &edge);

class EdgeController{

private:
	int edgeNum;
	Edge *edgeArray;

public:
	EdgeController();
	EdgeController(Edge* inputEdgeArray, int inputEdgeNum);
	~EdgeController() {}

	int getEdgeNum(void) { return edgeNum; }
	int getEdgeValue(int startVertex, int endVertex);
	void setEdgeValue(int startVertex, int endVertex, int value);
	int getEdgeIndex(int startVertex, int endVertex);
	Edge getEdge(int startVertex, int endVertex);
	Edge getEdge(int index);

	bool swapEdge(int index1, int index2);
	bool sorting(int order);

	void Print(void);
};

