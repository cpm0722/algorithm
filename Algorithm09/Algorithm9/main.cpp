#include<stdio.h>
#include"set.h"
#include"edge.h"

#define N 10
#define EdgeCount 13 
#define INFINITE_NUMBER  99


void Print(int question, Set<int> V, Set<int> Y, Set<Edge> F) {
	printf("\nQuestion%d\n", question);
	printf("[V집합]: ");
	V.PrintSet();
	printf("[Y집합]: ");
	Y.PrintSet();
	printf("[F집합]: ");
	F.PrintSet();
	return;
}

void Question01(int W[N][N], Edge* edgeArray, int n, int edgeCount) {
	EdgeController edgeController(edgeArray, edgeCount);

	int tempSource[N] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	//V집합 생성 (V1과 연결되지 않은 Vertex의 집합, V1제외 전부 입력)
	Set<int> V(0b1111111110, tempSource, n);
	//Y집합 생성 (V1과 이미 연결된 Vertex의 집합, V1만 입력)
	Set<int> Y(0b1, tempSource, n);

	//distance, nearest 배열 v1으로 초기화
	//nearest: V집합의 원소 vi에서 가장 가까운 Y집합의 원소, 초기값: 0(v1)
	//distance: vi에서 nearest[i]까지의 거리, 초기값: v1 to vi edge weight
	int nearest[N];
	int distance[N];
	nearest[0] = -1;
	distance[0] = INFINITE_NUMBER;
	for (int i = 1; i < n; i++) {
		nearest[i] = 0;
		distance[i] = W[0][i];
	}
	//Edge 배열 F 공집합으로 초기화
	Set<Edge> F(0, edgeArray, edgeCount);
	
	//n-1회 반복 (V1제외)
	while(V.getCount() > 0){
		int min = INFINITE_NUMBER;
		//vnear: Y집합에서 가장 가까운 V집합 원소의 인덱스
		int vnear = -1;
		for (int i = 1; i < n; i++) {
			if ((0 <= distance[i]) && (distance[i] <= min)) {
				//V집합의 원소 중 Y집합과 가장 가까운 Vertex 탐색
				min = distance[i];
				vnear = i;
			}
		}
		//F집합에 vnear(V)과 nearest[vnear](Y) 사이의 edge 추가
		F = F + edgeController.getEdgeIndex(vnear, nearest[vnear]);
		distance[vnear] = -1;
		nearest[vnear] = -1;
		Y = Y + vnear;
		V = V - vnear;
		for (int i = 1; i < n; i++) {
			if (W[i][vnear] < distance[i]) {
				distance[i] = W[i][vnear];
				nearest[i] = vnear;
			}
		}
	}

	Print(1, V, Y, F);

	return;
}

void Question02(int W[N][N], Edge *edgeArray, int n, int edgeCount) {
	EdgeController edgeController(edgeArray, edgeCount);

	int tempSource[N] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	//V집합 생성 (V1과 연결되지 않은 Vertex의 집합, V10제외 전부 입력)
	Set<int> V(0b0111111111, tempSource, n);
	//Y집합 생성 (V1과 이미 연결된 Vertex의 집합, V10만 입력)
	Set<int> Y(0b1000000000, tempSource, n);

	//distance, nearest 배열 v1으로 초기화
	//nearest: V집합의 원소 vi에서 가장 가까운 Y집합의 원소, 초기값: 9(v10)
	//distance: vi에서 nearest[i]까지의 거리, 초기값: v10 to vi edge weight
	int nearest[N];
	int distance[N];
	nearest[N-1] = -1;
	distance[N-1] = INFINITE_NUMBER;
	for (int i = 0; i < n-1; i++) {
		nearest[i] = N-1;
		distance[i] = W[N-1][i];
	}
	//Edge 배열 F 공집합으로 초기화
	Set<Edge> F(0, edgeArray, edgeCount);
	
	//n-1회 반복 (V1제외)
	while(V.getCount() > 0){
		int min = INFINITE_NUMBER;
		//vnear: Y집합에서 가장 가까운 V집합 원소의 인덱스
		int vnear = -1;
		for (int i = 0; i < n - 1; i++) {
			if ((0 <= distance[i]) && (distance[i] <= min)) {
				//V집합의 원소 중 Y집합과 가장 가까운 Vertex 탐색
				min = distance[i];
				vnear = i;
			}
		}
		//F집합에 vnear(V)과 nearest[vnear](Y) 사이의 edge 추가
		F = F + edgeController.getEdgeIndex(vnear, nearest[vnear]);
		distance[vnear] = -1;
		nearest[vnear] = -1;
		Y = Y + vnear;
		V = V - vnear;
		for (int i = 0; i < n - 1; i++) {
			if (W[i][vnear] < distance[i]) {
				distance[i] = W[i][vnear];
				nearest[i] = vnear;
			}
		}
	}
	
	Print(2, V, Y, F);

	return;
}

void Question03(int W[N][N], Edge *edgeArray, int n, int edgeCount) {
	//Question03에서 사용할 edgeArray 생성
	Edge sortedEdgeArray[EdgeCount];
	for (int i = 0; i < edgeCount; i++)
		sortedEdgeArray[i] = edgeArray[i];
	EdgeController edgeController(sortedEdgeArray, edgeCount);
	//edge 배열 비감소 순으로 정렬
	edgeController.sorting(NONDECREASING_ORDER);
	
	int tempSource[N] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	//setArray: 10개의 집합을 저장하는 배열
	Set<int> *setArray[N] = { NULL, };
	int setArrayCount = N;
	int startEdgeIndex = 0;
	Set<Edge> usedEdge(0, sortedEdgeArray, edgeCount);
	//setArray의 각각의 원소에 Vertex 1개를 원소로 갖는 집합 생성
	for (int i = 0; i < n; i++)
		setArray[i] = new Set<int>((int)pow(2, i), tempSource, n);

	for (int i = 0; i < edgeCount; i++) {
		Edge edge = edgeController.getEdge(i);
		//startVertex, endVertex: edge의 startVertex, endVertex
		int startVertex = edge.getStartVertex();
		int endVertex = edge.getEndVertex();
		//startSet: startVertex가 속한 집합의 setArray에서의 index
		//endSet: endVertex가 속한 집합의 setArray에서의 index
		int startSet = -1, endSet = -1;
		//startSet, endSet 찾기
		for (int j = 0; j < setArrayCount; j++) {
			if (setArray[j]->find(startVertex))
				startSet = j;
			if (setArray[j]->find(endVertex))
				endSet = j;
		}
		if (startSet != endSet) {
			//usedEdge 집합에 현재 edge 추가
			usedEdge = usedEdge + Set<Edge>((int)pow(2, edgeController.getEdgeIndex(startVertex, endVertex)), sortedEdgeArray, edgeCount);
			//startVertex가 들어간 집합과 endVertex가 들어간 집합을 합침
			*setArray[startSet] = *setArray[startSet] + *setArray[endSet];
			//endVertex가 들어갔던 집합은 공집합으로 초기화
			*setArray[endSet] = 0;
		}
	}
	Set<int> *Y = NULL;
	for(int i = 0; i < n; i++)
		if (!(setArray[i]->isEmpty())) {
			Y = setArray[i];
			break;
		}

	printf("\nQuestion3\n");
	printf("[Y집합]: ");
	Y->PrintSet();
	printf("[F집합]: ");
	usedEdge.PrintSet();
	return;
}

int main(void) {

	//W 배열 생성
	int W[N][N] = {
	{0, 32, INFINITE_NUMBER, 17, INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER},
	{32, 0,INFINITE_NUMBER, INFINITE_NUMBER, 45, INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER},
	{INFINITE_NUMBER, INFINITE_NUMBER, 0, 18, INFINITE_NUMBER, INFINITE_NUMBER, 5, INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER},
	{17, INFINITE_NUMBER,18, 0, 10, INFINITE_NUMBER, INFINITE_NUMBER, 3, INFINITE_NUMBER, INFINITE_NUMBER},
	{INFINITE_NUMBER, 45, INFINITE_NUMBER, 10, 0, 28, INFINITE_NUMBER, INFINITE_NUMBER, 25, INFINITE_NUMBER},
	{INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER, 28, 0, INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER, 6},
	{INFINITE_NUMBER, INFINITE_NUMBER, 5, INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER, 0, 59, INFINITE_NUMBER, INFINITE_NUMBER},
	{INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER, 3, INFINITE_NUMBER, INFINITE_NUMBER, 59, 0, 4, INFINITE_NUMBER},
	{INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER, 25, INFINITE_NUMBER, INFINITE_NUMBER, 4, 0, 12},
	{INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER, INFINITE_NUMBER, 6, INFINITE_NUMBER, INFINITE_NUMBER, 12, 0}
	};
	
	//Edge 배열 생성
	Edge edgeArray[EdgeCount] = {*new Edge(32, 0, 1), *new Edge(17, 0, 3), *new Edge(45, 1, 4), *new Edge(10, 3, 4),\
		*new Edge(18, 2, 3), *new Edge(5, 2, 6), *new Edge(59, 6, 7), *new Edge(3, 3, 7), *new Edge(4, 7, 8), \
		*new Edge(25, 4, 8), *new Edge(28, 4, 5), *new Edge(6, 5, 9), *new Edge(12, 8, 9)};

	Question01(W, edgeArray, N, EdgeCount);
	Question02(W, edgeArray, N, EdgeCount);
	Question03(W, edgeArray, N, EdgeCount);
	
	return 0;
}

