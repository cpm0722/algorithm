#include<stdio.h>
#include<windows.h>
#include<WinBase.h>
#include"set.h"
#define N 8
#define ESTIMATE_TIME 20
#define M 6

//n-Queens Problem���� �������� Ȯ��
bool promising_nQueens(int col[N], int now) {
	bool isPromising = true;
	int i = 0;
	//now ������ ��ġ�� Queen��� ��
	while (i < now && isPromising) {
		//���� column, ���� �밢�� Ȯ��
		if (col[now] == col[i] || abs(col[now] - col[i] == abs(now - i)))
			isPromising = false;
		i++;
	}
	return isPromising;
}

//����Į�� Algorithm �̿��� n-Queens Problem ȿ�� ����
int estimate_nQueens(int n) {
	//col[i]: i���� row�� queen�� ��ġ�ϴ� col�� index
	int col[N] = { 0, };
	//Set<int> ����ϱ� ���� �ҽ� �迭
	int source_col[N] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	//������ ��ġ���� Promising�� Child�� index ����
	Set<int> prom_children(0, source_col, n);
	//numNodes: ���� return ��
	//m: ���� promising�� child�� ����
	//mProd: ������� ������ m���� ��
	int i = 0, numNodes = 1, m = 1, mProd = 1;
	//promising�� child�� ���ų� n���� Ž�� �Ϸ����� �� �ݺ� ����
	while (m != 0 && i < n - 1) {
		mProd = mProd * m;
		//n-Queen Problems���� t(���� Child ����)�� �׻� n
		numNodes = numNodes + mProd * n;
		i++;
		m = 0;
		//prom_children ���������� �ʱ�ȭ
		prom_children = (prom_children) && (*new Set<int>(0, source_col, n));
		//i��° row�� �ִ� Queen�� col�� 0~n���� �ϳ��� �����ϸ鼭 �ݺ�
		for (int j = 0; j < n; j++) {
			col[i] = j;
			//���簡 promising�ϴٸ�
			if (promising_nQueens(col, i)) {
				m++;
				//prom_children U {j}
				prom_children = prom_children + j;
			}
		}
		//promising�� child �� ������ �� �� �̾� col������ ����
		if (m != 0) {
			int rnd = prom_children.getRandomIndex();
			col[i] = rnd;
		}
	}
	return numNodes;
}


//Promising���� Ȯ��
 bool promising_sumOfSubsets(int i, int weight, int total, int W, int w[M]) {
	 return (weight + total >= W) && (weight == W || weight + w[i + 1] <= W); 
 } 
 


//Sum of Subsets Problem 
void sum_of_subsets(Set<int> subset, int i, int weight, int total, int W, int w[M]){
	//������ ���
	if (promising_sumOfSubsets(i, weight, total, W, w)) {
		//������ ��� ���� ���
		if (weight == W)
			subset.PrintSet();
		else {
			//i+1 ���� �� ��� ȣ��
			subset = subset + (i + 1);
			sum_of_subsets(subset, i + 1, weight + w[i + 1], total - w[i + 1], W, w);
			//i+1 ���� �� ��� ȣ��
			subset = subset - (i + 1);
			sum_of_subsets(subset, i + 1, weight, total - w[i + 1], W, w);
		}
	}
	return;
}

//Sum of Subsets Problem 
//���� ���� �� return true ���� �ٷ� ����
bool sum_of_subsets_only_one(Set<int> subset, int i, int weight, int total, int W, int w[M]) {
	if (promising_sumOfSubsets(i, weight, total, W, w)) {
		if (weight == W) {
			subset.PrintSet();
			return true;
		}
		else {
			subset = subset + (i + 1);
			if(sum_of_subsets_only_one(subset, i + 1, weight + w[i + 1], total - w[i + 1], W, w))
				return true;
			subset = subset - (i + 1);
			if (sum_of_subsets_only_one(subset, i + 1, weight, total - w[i + 1], W, w))
				return true;
		}
	}
	return false;
}

//����Į�� Algorithm ����� Sum of Subsets Problem ȿ�� ����
int estimate_sum_of_subsets(int n, int W, int w[M]) {
	int i = 0, m = 1, mProd = 1, numNodes = 1, weight = 0, total = 0;
	for (int j = 0; j < n; j++) {
		total += w[j];
	}
	//i+1��° ���Ұ� ���ԵǾ��� �� ��������, �����Ǿ��� �� ��������
	bool isIncludePromising = false, isExcludePromising = false;
	//���� �ʱ�ȭ
	Set<int> subsets = *new Set<int>(0, w, n);
	while (m != 0 && i < n - 1) {
		//boolean ���� �ʱ�ȭ
		isIncludePromising = false;
		isExcludePromising = false;
		//total ����
		total -= w[i];
		//mProd: numNodes�� ���ϱ� ���� ����
		mProd *= m;
		//sum_of_subsets Algorithm���� t�� �׻� 2 (���� or ����X)
		numNodes += mProd * 2;
		
		//i+1��° ���� ������ ���
		subsets = subsets + i;
		weight += w[i];
		isIncludePromising = promising_sumOfSubsets(i, weight, total, W, w);
		
		//i+1��° ���� ������ ���
		subsets = subsets - i;
		weight -= w[i];
		isExcludePromising = promising_sumOfSubsets(i, weight, total, W, w);

		//i+1��° ���� ������ ��� Promising
		//i+1��° ���� ������ ��� Promising
		if (isIncludePromising && isExcludePromising) {
			m = 2;
			int rnd = rand() % 2;
			if (rnd == 0) {
				subsets = subsets + i;
				weight += w[i];
			}
		}
		//i+1��° ���� ������ ��� Promising
		//i+1��° ���� ������ ��� NonPromising
		else if (isIncludePromising && !isExcludePromising) {
			m = 1;
			subsets = subsets + i;
			weight += w[i];
		}
		//i+1��° ���� ������ ��� NonPromising
		//i+1��° ���� ������ ��� Promising
		else if (!isIncludePromising && isExcludePromising) {
			m = 1;
		}
		//i+1��° ���� ������ ��� NonPromising
		//i+1��° ���� ������ ��� NonPromising
		else {
			break;
		}
		i++;
	}
	subsets.PrintSet();
	return numNodes;;
}

int main(void) {
	srand((unsigned int)time(NULL));
	//Question 11
	printf("[Question 11]\n");
	int tmp = 0, sum = 0;
	for (int i = 0; i < ESTIMATE_TIME; i++) {
		tmp = estimate_nQueens(N);
		printf("%02d��°: %d\n", i + 1, tmp);
		sum += tmp;
	}
	printf("Average numNodes: %02d\n", sum / ESTIMATE_TIME);
	
	//Question 14
	printf("\n\n[Question 14]\n");
	int w[M] = { 2, 10 ,13, 17, 22, 42 }, W = 52, total = 0;
	for (int i = 0; i < M; i++)
		total += w[i];
	Set<int> subset = *new Set<int>(0, w, M);
	sum_of_subsets_only_one(subset, 0, 0, total, W, w);


	//Question 16
	printf("\n\n[Question 16]\n");
	LARGE_INTEGER start, end, f;
	QueryPerformanceFrequency(&f);
	QueryPerformanceCounter(&start);
	sum_of_subsets_only_one(subset, 0, 0, total, W, w);
	QueryPerformanceCounter(&end);
	int microsec1 = (end.QuadPart - start.QuadPart) / (f.QuadPart / 1000000);
	QueryPerformanceCounter(&start);
	sum_of_subsets(subset, 0, 0, total, W, w);
	QueryPerformanceCounter(&end);
	int microsec2 = (end.QuadPart - start.QuadPart) / (f.QuadPart / 1000000);
	printf("�ϳ��� �ش丸 ã�� �˰����� ����ð�: %dmicroSecond\n��� �ش� ã�� �˰����� ����ð�: \t%dmicroSecond\n", microsec1, microsec2);

	
	//Question 17
	printf("\n\n[Question 17]\n");
	tmp = 0, sum = 0;
	for (int i = 0; i < ESTIMATE_TIME; i++) {
		tmp = estimate_sum_of_subsets(M, W, w);
		printf("%02d��°: %d\n", i + 1, tmp);
		sum += tmp;
	}
	printf("Average numNodes: %02d\n", sum / ESTIMATE_TIME);
	
	return 0;
}