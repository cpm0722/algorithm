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

//rightChild�� Bound ���ϴ� �Լ�
//i��° Knapsack�� ���Ե��� ���� ��(Right Child�̱� ����)
int getRightBound(int A[N][M], Set<int> set, int i) {
	int rightBound = 0, tmpWeight = 0;
	//������ Knapsack ���տ��� ���Ե� Knapsack�� index ����
	int *rightChildIndex = (set - i).getIndex();
	int len = (set - i).getCount();
	for (int j = 0; j < len; j++) {
		int index = rightChildIndex[j];
		//�̹� ���Ե� Knapsack�� Profit�� rightBound�� ����
		rightBound += A[index][PROFIT];
		//�̹� ���Ե� Knapsack�� Weight�� tmpWeight�� ����
		tmpWeight += A[index][WEIGHT];
	}
	//i + 1���� Ž�� (i��° Knapsack�� �������� ����)
	for (int j = i + 1; j < N; j++) {
		rightBound += A[j][PROFIT];
		tmpWeight += A[j][WEIGHT];
		//tmpWeight�� W�� �ʰ��ϸ�
		if (tmpWeight > W) {
			//W�� �ʰ����� ������ŭ�� rightBound�� ����
			rightBound -= (tmpWeight - W) * A[j][PROFITPERWEIGHT];
			break;
		}
	}
	return rightBound;
}

//�ʿ���� Child ��Ȱ��ȭ
void setChildActivate(BinaryTree<KnapsackNode> *now, int maxProfit) {
	//Tree�� Leaf�� ��� ����
	if (now->getLeft() == NULL || now->getRight() == NULL)
		return;
	//�̹� weight == W�̰ų� �ʰ����� ��� ��� Child ��Ȱ��ȭ
	if (now->getValue()->getWeight() >= W) {
		now->getLeft()->getValue()->setIsActivate(false);
		now->getRight()->getValue()->setIsActivate(false);
		return;
	}
	//maxProfit���� Bound�� ���� Child�� ��Ȱ��ȭ
	if (now->getLeft()->getValue()->getBound() < maxProfit)
		now->getLeft()->getValue()->setIsActivate(false);
	if (now->getRight()->getValue()->getBound() < maxProfit)
		now->getRight()->getValue()->setIsActivate(false);
	//W���� weight�� ū Child�� ��Ȱ��ȭ
	if (now->getLeft()->getValue()->getWeight() > W)
		now->getLeft()->getValue()->setIsActivate(false);
	if (now->getRight()->getValue()->getWeight() > W)
		now->getRight()->getValue()->setIsActivate(false);
	return;
}

//Knapsack���� BinaryTree �����ϴ� �Լ�
void Knapsack_BinaryTree(int A[N][M], Set<int> set, BinaryTree<KnapsackNode> *now) {
	int i = now->getHeight();
	int profit = now->getValue()->getProfit();
	int weight = now->getValue()->getWeight();
	int bound = now->getValue()->getBound();
	int addProfit = A[i][PROFIT];
	int addWeight = A[i][WEIGHT];
	int leftBound = bound;
	if (weight + addWeight > W) leftBound = 0;
	//rightChild�� Bound �� ���ϱ�
	int rightBound = getRightBound(A, set, i);
	//Tree�� Leat�̸� Child NULL�� ����
	if (i == N) {
		now->setLeft(NULL);
		now->setRight(NULL);
	}
	else {
		//LeftChild ����
		set = set + i;
		now->setLeft(new BinaryTree <KnapsackNode>(new KnapsackNode(\
			profit + addProfit, weight + addWeight, leftBound, set), now, i + 1));
		//RightChild ����
		set = set - i;
		now->setRight(new BinaryTree <KnapsackNode>(\
			new KnapsackNode(profit, weight, rightBound, set), now, i + 1));
	}
	return;
}

//Depth First�� Ȱ��ȭ ��� ã�� �Լ�
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

//Breadth First�� Ȱ��ȭ Node ã�� �Լ�
BinaryTree <KnapsackNode> *FindNode_Breadth(BinaryTree <KnapsackNode> *root) {
	//�迭 2�� �̿��� Queue ����
	BinaryTree<KnapsackNode> **queue1 = new (BinaryTree<KnapsackNode>*[(int)pow(2, 0)]);
	BinaryTree<KnapsackNode> **queue2;
	queue1[0] = root;
	//return�� pointer
	BinaryTree<KnapsackNode> *result = NULL;
	//height���� �ݺ�
	for (int i = 0; i < N; i++) {
		//queue2 �迭 ���� ����
		queue2 = new(BinaryTree<KnapsackNode>*[(int)pow(2, i + 1)]);
		//queue2 �迭 �ʱ�ȭ
		for (int j = 0; j < (int)pow(2, i + 1); j++)
			queue2[j] = NULL;
		//queue1 �迭 ���� ȹ��
		int len = (int)pow(2, i);
		int k = 0;
		//queue1 �迭�� ����ִ� Node���� child���� queue2 �迭�� �߰�
		for (int j = 0; j < len; j++) {
			//queue1 �迭�� ���� NULL�� �ƴ� ��
			if (queue1[j] != NULL) {
				//queue1 �迭�� ���� Ȱ��ȭ Node�̸� return
				if (queue1[j]->getValue()->getIsActivate()) {
					result = queue1[j];
					delete(queue1);
					delete(queue2);
					return result;
				}
				//queue1 �迭�� Left Child�� queue2�迭�� �߰�
				if (queue1[j]->getLeft() != NULL) {
					queue2[k] = queue1[j]->getLeft();
					//Left Child�� Ȱ��ȭ Node�̸� return
					if (queue2[k]->getValue()->getIsActivate()) {
						result = queue2[k];
						delete(queue1);
						delete(queue2);
						return result;
					}
					k++;
				}
				//queue1 �迭�� Right Child�� queue2�迭�� �߰�
				if (queue1[j]->getRight() != NULL) {
					queue2[k] = queue1[j]->getRight();
					//Right Child�� Ȱ��ȭ Node�̸� return
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

//Queue���� ù��° Node(���� Bound�� ���� Node)�� ����� Child�� �����ϴ� �Լ�
void Queue_Best(BinaryTree<KnapsackNode> **queue, int index, int len) {
	//Child�� ���� node
	BinaryTree<KnapsackNode> * node = queue[index];
	//Child�� �� Bound�� ū ���� first, ���� ���� second�� �Է�
	BinaryTree<KnapsackNode> *first = NULL, *second = NULL;
	if (node->getLeft()->getValue()->getBound() > node->getRight()->getValue()->getBound()) {
		first = node->getLeft();
		second = node->getRight();
	}
	else {
		first = node->getRight();
		second = node->getLeft();
	}
	//index�� ���鼭 ��ĭ�� ������ ���
	for (int i = index; i < len; i++) {
		queue[i] = queue[i + 1];
	}
	//first�� ���� �� ã��
	for (int i = 0; i < len; i++) {
		//first���� ���� bound�� ó������ ���� i
		if ((queue[i] == NULL) || (queue[i]->getValue()->getBound() <= first->getValue()->getBound())) {
			//������ i���� �� ĭ�� �ڷ� ���
			for (int j = len; j > i ; j--) {
				queue[j] = queue[j - 1];
			}
			//i�� first �Է�
			queue[i] = first;
			break;
		}
	}
	//second�� ���� �� ã��
	for (int i = 0; i < len; i++) {
		//second���� ���� bound�� ó������ ���� i
		if ((queue[i] == NULL) || (queue[i]->getValue()->getBound() <= second->getValue()->getBound())) {
			//������ i���� �� ĭ�� �ڷ� ���
			for (int j = len; j > i; j--) {
				queue[j] = queue[j - 1];
			}
			//i�� second �Է�
			queue[i] = second;
		}
	}
	return;
}

//Best First�� Ȱ��ȭ ��� ã�� �Լ�
BinaryTree<KnapsackNode> *FindNode_Best(BinaryTree <KnapsackNode> *root) {
	//queue �迭 ���� �� �ʱ�ȭ
	BinaryTree<KnapsackNode> **queue = new (BinaryTree<KnapsackNode>*[(int)pow(2, N)]);
	for (int i = 0; i < (int)pow(2, N); i++)
		queue[i] = NULL;
	queue[0] = root;
	BinaryTree<KnapsackNode> *result = NULL;
	int len = 1;
	while (true) {
		//Queue�� ù��° Node�� Child�� �����鼭 ��Ȱ��ȭ Node�̸� Ž�� ����
		if (((queue[0]->getLeft() == NULL) && \
			(queue[0]->getRight() == NULL))&&(!queue[0]->getValue()->getIsActivate())) {
			delete(queue);
			return NULL;
		}
		//Queue�� ù��° Node�� Ȱ��ȭ Node�̸� return
		if (queue[0]->getValue()->getIsActivate()) {
			result = queue[0];
			delete(queue);
			return result;
		}
		//Queue���� ù��° Node(���� Bound�� ���� Node)�� ����� �ش� ����� Child���� ����
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
	//maxBound�� ���� Node�� ����Ű�� ������
	BinaryTree<KnapsackNode> *maxPointer = now;
	int n = 1;
	while (true) {
		//now���� �ڽ� node ����
		printf("%2d��° Node �湮\tP: %d\tW: %d\tB: %d\t",\
			n++, now->getValue()->getProfit(), now->getValue()->getWeight(), now->getValue()->getBound());
		set = now->getValue()->getSet();
		Knapsack_BinaryTree(A, set, now);
		//�ʿ���� Child ��Ȱ��ȭ
		setChildActivate(now, maxProfit);
		//�ڱ� �ڽŵ� ��Ȱ��ȭ
		now->getValue()->setIsActivate(false);
		//now�� Child Ž���ϸ鼭 maxProfit ����
		//now�� Leaf�̸� ���� X
		if (now->getHeight() != N) {
			//Left�� Activate�� ��
			if (now->getLeft()->getValue()->getIsActivate()) {
				//Left�� Profit�� maxProfit���� �� ũ�ٸ�
				if (now->getLeft()->getValue()->getProfit() >= maxProfit) {
					maxProfit = now->getLeft()->getValue()->getProfit();
					maxPointer = now->getLeft();
				}
			}
			//Right�� Activate�� ��
			if (now->getRight()->getValue()->getIsActivate()) {
				//Right�� Profit�� maxProfit���� �� ũ�ٸ�
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
	//1��: profit, 2��: weight, 3��: price/weight
	int A[N][M] = { {20, 2, 10}, {30, 5, 6}, {35, 7, 5}, {12, 3, 4}, {3, 1, 3} };

	//ó�� total ����
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