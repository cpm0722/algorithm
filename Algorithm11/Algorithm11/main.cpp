#include<stdio.h>
#include<math.h>
#include"set.h"
#include"tree.h"
#include"knapsack.h"

#define W 9
#define N 5
#define M 3

#define PROFIT 0
#define WEIGHT 1
#define PROFITPERWEIGHT 2

#define DEPTH 0
#define BREADTH 1
#define BEST 2

//rightChild의 Bound 구하는 함수
//i번째 Knapsack은 포함되지 않을 것(Right Child이기 때문)
int getRightBound(int A[N][M], Set<int> set, int i) {
	int rightBound = 0, tmpWeight = 0;
	//현재의 Knapsack 집합에서 포함된 Knapsack의 index 추출
	int *rightChildIndex = (set - i).getIndex();
	int len = (set - i).getCount();
	for (int j = 0; j < len; j++) {
		int index = rightChildIndex[j];
		//이미 포함된 Knapsack의 Profit을 rightBound에 더함
		rightBound += A[index][PROFIT];
		//이미 포함된 Knapsack의 Weight을 tmpWeight에 더함
		tmpWeight += A[index][WEIGHT];
	}
	//i + 1부터 탐색 (i번째 Knapsack은 포함하지 않음)
	for (int j = i + 1; j < N; j++) {
		rightBound += A[j][PROFIT];
		tmpWeight += A[j][WEIGHT];
		//tmpWeight이 W를 초과하면
		if (tmpWeight > W) {
			//W를 초과하지 않을만큼만 rightBound에 넣음
			rightBound -= (tmpWeight - W) * A[j][PROFITPERWEIGHT];
			break;
		}
	}
	return rightBound;
}

//필요없는 Child 비활성화
void setChildActivate(BinaryTree<KnapsackNode> *now, int maxProfit) {
	//Tree의 Leaf일 경우 종료
	if (now->getLeft() == NULL || now->getRight() == NULL)
		return;
	//이미 weight == W이거나 초과했을 경우 모든 Child 비활성화
	if (now->getValue()->getWeight() >= W) {
		now->getLeft()->getValue()->setIsActivate(false);
		now->getRight()->getValue()->setIsActivate(false);
		return;
	}
	//maxProfit보다 Bound가 낮은 Child는 비활성화
	if (now->getLeft()->getValue()->getBound() < maxProfit)
		now->getLeft()->getValue()->setIsActivate(false);
	if (now->getRight()->getValue()->getBound() < maxProfit)
		now->getRight()->getValue()->setIsActivate(false);
	//W보다 weight가 큰 Child는 비활성화
	if (now->getLeft()->getValue()->getWeight() > W)
		now->getLeft()->getValue()->setIsActivate(false);
	if (now->getRight()->getValue()->getWeight() > W)
		now->getRight()->getValue()->setIsActivate(false);
	return;
}

//Knapsack에서 BinaryTree 생성하는 함수
void Knapsack_BinaryTree(int A[N][M], Set<int> set, BinaryTree<KnapsackNode> *now) {
	int i = now->getHeight();
	int profit = now->getValue()->getProfit();
	int weight = now->getValue()->getWeight();
	int bound = now->getValue()->getBound();
	int addProfit = A[i][PROFIT];
	int addWeight = A[i][WEIGHT];
	int leftBound = bound;
	if (weight + addWeight > W) leftBound = 0;
	//rightChild의 Bound 값 구하기
	int rightBound = getRightBound(A, set, i);
	//Tree의 Leat이면 Child NULL로 지정
	if (i == N) {
		now->setLeft(NULL);
		now->setRight(NULL);
	}
	else {
		//LeftChild 생성
		set = set + i;
		now->setLeft(new BinaryTree <KnapsackNode>(new KnapsackNode(\
			profit + addProfit, weight + addWeight, leftBound, set), now, i + 1));
		//RightChild 생성
		set = set - i;
		now->setRight(new BinaryTree <KnapsackNode>(\
			new KnapsackNode(profit, weight, rightBound, set), now, i + 1));
	}
	return;
}

//Depth First로 활성화 노드 찾는 함수
BinaryTree<KnapsackNode> *FindNode_Depth(BinaryTree <KnapsackNode> *root) {
	BinaryTree<KnapsackNode> *result = NULL;
	if (root->getValue()->getIsActivate())
		result = root;
	if(result == NULL && root->getLeft() != NULL)
		result = FindNode_Depth(root->getLeft());
	if (result == NULL && root->getRight() != NULL)
		result = FindNode_Depth(root->getRight());
	return result;
}

//Breadth First로 활성화 Node 찾는 함수
BinaryTree <KnapsackNode> *FindNode_Breadth(BinaryTree <KnapsackNode> *root) {
	//배열 2개 이용해 Queue 구현
	BinaryTree<KnapsackNode> **queue1 = new (BinaryTree<KnapsackNode>*[(int)pow(2, 0)]);
	BinaryTree<KnapsackNode> **queue2;
	queue1[0] = root;
	//return할 pointer
	BinaryTree<KnapsackNode> *result = NULL;
	//height마다 반복
	for (int i = 0; i < N; i++) {
		//queue2 배열 새로 생성
		queue2 = new(BinaryTree<KnapsackNode>*[(int)pow(2, i + 1)]);
		//queue2 배열 초기화
		for (int j = 0; j < (int)pow(2, i + 1); j++)
			queue2[j] = NULL;
		//queue1 배열 길이 획득
		int len = (int)pow(2, i);
		int k = 0;
		//queue1 배열에 들어있는 Node들의 child들을 queue2 배열에 추가
		for (int j = 0; j < len; j++) {
			//queue1 배열의 값이 NULL이 아닐 때
			if (queue1[j] != NULL) {
				//queue1 배열의 값이 활성화 Node이면 return
				if (queue1[j]->getValue()->getIsActivate()) {
					result = queue1[j];
					delete(queue1);
					delete(queue2);
					return result;
				}
				//queue1 배열의 Left Child를 queue2배열에 추가
				if (queue1[j]->getLeft() != NULL) {
					queue2[k] = queue1[j]->getLeft();
					//Left Child가 활성화 Node이면 return
					if (queue2[k]->getValue()->getIsActivate()) {
						result = queue2[k];
						delete(queue1);
						delete(queue2);
						return result;
					}
					k++;
				}
				//queue1 배열의 Right Child를 queue2배열에 추가
				if (queue1[j]->getRight() != NULL) {
					queue2[k] = queue1[j]->getRight();
					//Right Child가 활성화 Node이면 return
					if (queue2[k]->getValue()->getIsActivate()) {
						result = queue2[k];
						delete(queue1);
						delete(queue2);
						return result;
					}
					k++;
				}
				queue1[j] = NULL;
			}
		}
		delete(queue1);
		queue1 = queue2;
	}
	delete(queue2);
	return result;
}

//Queue에서 첫번째 Node(가장 Bound가 높은 Node)를 지우고 Child를 삽입하는 함수
void Queue_Best(BinaryTree<KnapsackNode> **queue, int index, int len) {
	//Child를 구할 node
	BinaryTree<KnapsackNode> * node = queue[index];
	//Child들 중 Bound가 큰 것을 first, 작은 것을 second로 입력
	BinaryTree<KnapsackNode> *first = NULL, *second = NULL;
	if (node->getLeft()->getValue()->getBound() > node->getRight()->getValue()->getBound()) {
		first = node->getLeft();
		second = node->getRight();
	}
	else {
		first = node->getRight();
		second = node->getLeft();
	}
	//index를 빼면서 한칸씩 앞으로 당김
	for (int i = index; i < len; i++) {
		queue[i] = queue[i + 1];
	}
	//first를 넣을 곳 찾음
	for (int i = 0; i < len; i++) {
		//first보다 작은 bound를 처음으로 가진 i
		if ((queue[i] == NULL) || (queue[i]->getValue()->getBound() <= first->getValue()->getBound())) {
			//끝부터 i까지 한 칸씩 뒤로 당김
			for (int j = len; j > i ; j--) {
				queue[j] = queue[j - 1];
			}
			//i에 first 입력
			queue[i] = first;
			break;
		}
	}
	//second를 넣을 곳 찾음
	for (int i = 0; i < len; i++) {
		//second보다 작은 bound를 처음으로 가진 i
		if ((queue[i] == NULL) || (queue[i]->getValue()->getBound() <= second->getValue()->getBound())) {
			//끝부터 i까지 한 칸씩 뒤로 당김
			for (int j = len; j > i; j--) {
				queue[j] = queue[j - 1];
			}
			//i에 second 입력
			queue[i] = second;
		}
	}
	return;
}

//Best First로 활성화 노드 찾는 함수
BinaryTree<KnapsackNode> *FindNode_Best(BinaryTree <KnapsackNode> *root) {
	//queue 배열 선언 및 초기화
	BinaryTree<KnapsackNode> **queue = new (BinaryTree<KnapsackNode>*[(int)pow(2, N)]);
	for (int i = 0; i < (int)pow(2, N); i++)
		queue[i] = NULL;
	queue[0] = root;
	BinaryTree<KnapsackNode> *result = NULL;
	int len = 1;
	while (true) {
		//Queue의 첫번째 Node가 Child가 없으면서 비활성화 Node이면 탐색 종료
		if (((queue[0]->getLeft() == NULL) && \
			(queue[0]->getRight() == NULL))&&(!queue[0]->getValue()->getIsActivate())) {
			delete(queue);
			return NULL;
		}
		//Queue의 첫번째 Node가 활성화 Node이면 return
		if (queue[0]->getValue()->getIsActivate()) {
			result = queue[0];
			delete(queue);
			return result;
		}
		//Queue에서 첫번째 Node(가장 Bound가 높은 Node)를 지우고 해당 노드의 Child들을 삽입
		Queue_Best(queue, 0, len);
		len++;
	}
	return result;
}


//Knapsack Algorithm
void Knapsack(int A[N][M], int total, int which) {
	int source[N] = { 1, 2, 3, 4, 5 };
	Set<int> set = *(new Set<int>(0, source, N));
	BinaryTree<KnapsackNode> *root = \
		new BinaryTree<KnapsackNode>(new KnapsackNode(0, 0, total, set), NULL, 0);
	BinaryTree<KnapsackNode> *now = root;
	int maxProfit = now->getValue()->getProfit();
	//maxBound를 갖는 Node를 가리키는 포인터
	BinaryTree<KnapsackNode> *maxPointer = now;
	int n = 1;
	while (true) {
		//now에게 자식 node 생성
		printf("%2d번째 Node 방문\tP: %d\tW: %d\tB: %d\t",\
			n++, now->getValue()->getProfit(), now->getValue()->getWeight(), now->getValue()->getBound());
		set = now->getValue()->getSet();
		Knapsack_BinaryTree(A, set, now);
		//필요없는 Child 비활성화
		setChildActivate(now, maxProfit);
		//자기 자신도 비활성화
		now->getValue()->setIsActivate(false);
		//now의 Child 탐색하면서 maxProfit 갱신
		//now가 Leaf이면 실행 X
		if (now->getHeight() != N) {
			//Left가 Activate일 때
			if (now->getLeft()->getValue()->getIsActivate()) {
				//Left의 Profit이 maxProfit보다 더 크다면
				if (now->getLeft()->getValue()->getProfit() >= maxProfit) {
					maxProfit = now->getLeft()->getValue()->getProfit();
					maxPointer = now->getLeft();
				}
			}
			//Right가 Activate일 때
			if (now->getRight()->getValue()->getIsActivate()) {
				//Right의 Profit이 maxProfit보다 더 크다면
				if (now->getRight()->getValue()->getProfit() >= maxProfit) {
					maxProfit = now->getRight()->getValue()->getProfit();
					maxPointer = now->getRight();
				}
			}
		}
		printf("maxProfit: %d\n", maxProfit);
		if (which == DEPTH)
			now = FindNode_Depth(root);
		else if (which == BREADTH)
			now = FindNode_Breadth(root);
		else if (which == BEST)
			now = FindNode_Best(root);
		if (now == NULL)
			break;
		}
	printf("maxProfit: %d, maxWeight: %d, maxBoundary: %d\n",\
		maxPointer->getValue()->getProfit(), maxPointer->getValue()->getWeight(),\
		maxPointer->getValue()->getBound());
	return;
}

int main(void) {
	//1열: profit, 2열: weight, 3열: price/weight
	int A[N][M] = { {20, 2, 10}, {30, 5, 6}, {35, 7, 5}, {12, 3, 4}, {3, 1, 3} };

	//처음 total 구함
	int total = 0;
	int weight = 0;
	for (int i = 0; i < N; i++) {
		total += A[i][PROFIT];
		weight += A[i][WEIGHT];
		if (weight > W) {
			total -= (weight - W) * A[i][PROFITPERWEIGHT];
			break;
		}
	}
	printf("Depth First Knapsack\n\n");
	Knapsack(A, total, DEPTH);
	printf("\nBreadth First Knapsack\n\n");
	Knapsack(A, total, BREADTH);
	printf("\nBest First Knapsack\n\n");
	Knapsack(A, total, BEST);

	return 0;
}