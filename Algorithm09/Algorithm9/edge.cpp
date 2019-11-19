#include<stdio.h>
#include<iostream>
#include"edge.h"

//Edge ������
Edge::Edge(int inputEdgeValue, int inputStartVertex, int inputEndVertex) {
	this->edgeValue = inputEdgeValue;
	this->startVertex = inputStartVertex;
	this->endVertex = inputEndVertex;
}

//Edge Default ������
Edge::Edge() {
	this->edgeValue = this->startVertex = this->endVertex = -1;
}

//Edge ���� ������ Overloading (���� ����)
Edge Edge::operator =(Edge &b) {
	edgeValue = b.getEdgeValue();
	startVertex = b.getStartVertex();
	endVertex = b.getEndVertex();
	return *this;
}

//Edge ��� ������ Overloading
ostream &operator <<(ostream &c, const Edge &edge){
	edge.printEdge();
	return c;
}

//Edge ���� ���
void Edge::printEdge(void) const {
	printf("V%d~V%d:%02d", startVertex + 1, endVertex + 1, edgeValue);
	return;
}

//EdgeController Default ������
EdgeController::EdgeController() {
	this->edgeNum = -1;
	this->edgeArray = NULL;
}

//EdgeController ������
EdgeController::EdgeController(Edge* inputEdgeArray, int inputEdgeNum) {
	this->edgeNum = inputEdgeNum;
	this->edgeArray = inputEdgeArray;
}

//�� Vertex�� �����ϴ� Edge�� weight ��ȯ
int EdgeController::getEdgeValue(int startVertex, int endVertex) {
	for (int i = 0; i < this->edgeNum; i++)
		if (((edgeArray[i].getStartVertex() == startVertex) && (edgeArray[i].getEndVertex() == endVertex)) || \
			((edgeArray[i].getStartVertex() == endVertex) && (edgeArray[i].getEndVertex() == startVertex)))
			return edgeArray[i].getEdgeValue();
	return -1;

}

//�� Vertex�� �����ϴ� Edge�� weight �Է�
void EdgeController::setEdgeValue(int startVertex, int endVertex, int value) {
	for (int i = 0; i < this->edgeNum; i++)
		if (((edgeArray[i].getStartVertex() == startVertex) && (edgeArray[i].getEndVertex() == endVertex)) || \
			((edgeArray[i].getStartVertex() == endVertex) && (edgeArray[i].getEndVertex() == startVertex)))
			edgeArray[i].setEdgeValue(value);
	return;
}

//�� Vertex�� �����ϴ� Edge�� edgeArray������ index ��ȯ
int EdgeController::getEdgeIndex(int startVertex, int endVertex) {
	for (int i = 0; i < this->edgeNum; i++)
		if (((edgeArray[i].getStartVertex() == startVertex) && (edgeArray[i].getEndVertex() == endVertex)) || \
			((edgeArray[i].getStartVertex() == endVertex) && (edgeArray[i].getEndVertex() == startVertex)))
			return i;
	return -1;
}

//�� Vertex�� �����ϴ� Edge ��ȯ
Edge EdgeController::getEdge(int startVertex, int endVertex) {
	for (int i = 0; i < this->edgeNum; i++)
		if (((edgeArray[i].getStartVertex() == startVertex) && (edgeArray[i].getEndVertex() == endVertex)) || \
			((edgeArray[i].getStartVertex() == endVertex) && (edgeArray[i].getEndVertex() == startVertex)))
			return edgeArray[i];
	return *new Edge(0, 0, 0);
}

//index�� Edge ��ȯ
Edge EdgeController::getEdge(int index) {
	return edgeArray[index];
}

//sorting �� ����ϴ� swap �Լ�
bool EdgeController::swapEdge(int index1, int index2) {
	if ((index1 >= edgeNum) || (index2 >= edgeNum))
		return false;
	
	Edge tmp = edgeArray[index2];
	edgeArray[index2] = edgeArray[index1];
	edgeArray[index1] = tmp;
	return true;
}

//Bubble Sorting �Լ�
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

//edgeArray ��� �Լ�
void EdgeController::Print(void) {
	for (int i = 0; i < edgeNum; i++)
		cout << edgeArray[i] << ' ';
	return;
}


