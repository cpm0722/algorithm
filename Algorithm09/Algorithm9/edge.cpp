#include<stdio.h>
#include<iostream>
#include"edge.h"

//Edge 생성자
Edge::Edge(int inputEdgeValue, int inputStartVertex, int inputEndVertex) {
	this->edgeValue = inputEdgeValue;
	this->startVertex = inputStartVertex;
	this->endVertex = inputEndVertex;
}

//Edge Default 생성자
Edge::Edge() {
	this->edgeValue = this->startVertex = this->endVertex = -1;
}

//Edge 대입 연산자 Overloading (깊은 복사)
Edge Edge::operator =(Edge &b) {
	edgeValue = b.getEdgeValue();
	startVertex = b.getStartVertex();
	endVertex = b.getEndVertex();
	return *this;
}

//Edge 출력 연산자 Overloading
ostream &operator <<(ostream &c, const Edge &edge){
	edge.printEdge();
	return c;
}

//Edge 정보 출력
void Edge::printEdge(void) const {
	printf("V%d~V%d:%02d", startVertex + 1, endVertex + 1, edgeValue);
	return;
}

//EdgeController Default 생성자
EdgeController::EdgeController() {
	this->edgeNum = -1;
	this->edgeArray = NULL;
}

//EdgeController 생성자
EdgeController::EdgeController(Edge* inputEdgeArray, int inputEdgeNum) {
	this->edgeNum = inputEdgeNum;
	this->edgeArray = inputEdgeArray;
}

//두 Vertex를 연결하는 Edge의 weight 반환
int EdgeController::getEdgeValue(int startVertex, int endVertex) {
	for (int i = 0; i < this->edgeNum; i++)
		if (((edgeArray[i].getStartVertex() == startVertex) && (edgeArray[i].getEndVertex() == endVertex)) || \
			((edgeArray[i].getStartVertex() == endVertex) && (edgeArray[i].getEndVertex() == startVertex)))
			return edgeArray[i].getEdgeValue();
	return -1;

}

//두 Vertex를 연결하는 Edge의 weight 입력
void EdgeController::setEdgeValue(int startVertex, int endVertex, int value) {
	for (int i = 0; i < this->edgeNum; i++)
		if (((edgeArray[i].getStartVertex() == startVertex) && (edgeArray[i].getEndVertex() == endVertex)) || \
			((edgeArray[i].getStartVertex() == endVertex) && (edgeArray[i].getEndVertex() == startVertex)))
			edgeArray[i].setEdgeValue(value);
	return;
}

//두 Vertex로 연결하는 Edge의 edgeArray에서의 index 반환
int EdgeController::getEdgeIndex(int startVertex, int endVertex) {
	for (int i = 0; i < this->edgeNum; i++)
		if (((edgeArray[i].getStartVertex() == startVertex) && (edgeArray[i].getEndVertex() == endVertex)) || \
			((edgeArray[i].getStartVertex() == endVertex) && (edgeArray[i].getEndVertex() == startVertex)))
			return i;
	return -1;
}

//두 Vertex로 연결하는 Edge 반환
Edge EdgeController::getEdge(int startVertex, int endVertex) {
	for (int i = 0; i < this->edgeNum; i++)
		if (((edgeArray[i].getStartVertex() == startVertex) && (edgeArray[i].getEndVertex() == endVertex)) || \
			((edgeArray[i].getStartVertex() == endVertex) && (edgeArray[i].getEndVertex() == startVertex)))
			return edgeArray[i];
	return *new Edge(0, 0, 0);
}

//index로 Edge 반환
Edge EdgeController::getEdge(int index) {
	return edgeArray[index];
}

//sorting 시 사용하는 swap 함수
bool EdgeController::swapEdge(int index1, int index2) {
	if ((index1 >= edgeNum) || (index2 >= edgeNum))
		return false;
	
	Edge tmp = edgeArray[index2];
	edgeArray[index2] = edgeArray[index1];
	edgeArray[index1] = tmp;
	return true;
}

//Bubble Sorting 함수
bool EdgeController::sorting(int order) {
	for (int i = 0; i < edgeNum; i++) {
		for (int j = 0; j < edgeNum  - 1 - i; j++) {
			if (order == NONDECREASING_ORDER)
				if (edgeArray[j].getEdgeValue() > edgeArray[j + 1].getEdgeValue())
					swapEdge(j, j + 1);
			else if (order == NONINCREASING_ORDER)
				if (edgeArray[j].getEdgeValue() > edgeArray[j + 1].getEdgeValue())
					swapEdge(j, j + 1);
			else return false;
		}
	}
	return true;
}

//edgeArray 출력 함수
void EdgeController::Print(void) {
	for (int i = 0; i < edgeNum; i++)
		cout << edgeArray[i] << ' ';
	return;
}


