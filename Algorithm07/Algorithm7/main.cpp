#include<stdio.h>
#include<math.h>

#include"set.h"

#define N 5
#define INF 99

//Byte�� {Vn}���� ���
void PrintVertice(int byte, int sourceNum) {
	int space = 0;
	for (int i = 1; i <= sourceNum; i++) {
		if ((byte & 1) == 1) {
			printf("%d ", i);
			space += 2;
		}
		byte = byte >> 1;
	}
	if (space == 0) {
		printf("empty ");
	}
	else {
		for (; space < 6; space++)
			putchar(' ');
	}
	printf("| ");
	return;
}

//�迭 ���
void PrintArray(int **array, int row, int col) {
	for (int i = 0; i < row; i++) {
		printf("V%d  |", i + 1);
		for (int j = 0; j < col; j++) {
			if (array[i][j] == INF) {
				printf("   INF| ");
			}
			else {
				printf("%6d| ", array[i][j]);
			}
		}
		putchar('\n');
	}
	putchar('\n');
	return;
}

//P�迭 �̿��� ��� ���
void PrintPath(int **P, Set **setList, int n, \
	int setListNum, int *source) {
	int l = setListNum - 1, v = 0, p = 0;
	//V1 ���
	printf("V%d -> ", p + 1);
	while (true) {
		//�������� �� �ݺ� ����
		if (l == 0) break;
		//P[Vi][VA]
		p = P[v][l];
		//P[Vi][VA] ���
		printf("V%d -> ", p + 1);
		//Vi = P[Vi][VA]
		v = p;
		int v_binary = (int)pow(2, v);
		//VA = VA - {Vi}
		l = ((*setList[l]) - (Set(v_binary, \
			source, n))).getByte() / 2;
	}
	//V1 ���
	printf("V%d\n", 1);
	return;
}

void TravelSalesPerson(int (*W)[N], int n) {
	//source, ������ ���� ������ �� ����
	//(TravelSalesPerson������ ��� X)
	int source[5] = { 0, 0, 0, 0, 0 };
	// 2^(N-1), V1,�� ������ Vertix���� Subset ����
	int setListNum = (int)pow(2, n - 1);
	//V1�� ������ Vertix���� Subset �迭
	Set **setList = new Set*[setListNum];
	for (int i = 0; i < setListNum; i++) {
		setList[i] = new Set(i * 2, source, n);
	}

	//D[n][2^(n-1)] �迭 ����
	int **D = new int *[n];
	for (int i = 0; i < n; i++)
		D[i] = new int[setListNum];

	//P[n][2^(n-1)] �迭 ����
	int **P = new int *[n];
	for (int i = 0; i < n; i++)
		P[i] = new int[setListNum];
	
	//�迭 �ʱ�ȭ
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < setListNum; j++) {
			//Vn, �������� Vn~V1 edge�� weight
			if (j == 0) {
				D[i][j] = W[i][0];
				P[i][j] = i;
			}
			else {
				D[i][j] = INF;
				P[i][j] = INF;
			}
		}
	}

	//k: 1 ~ n-2�� (V1�� Vi�� ����)
	for (int k = 1; k <= n - 2; k++) {
		//j: 1 ~ 2^(n-1) V1 ���� ��� �κ����� Ž��
		//(l:0 �������� �̹� �ʱ�ȭ �� �� �Է������Ƿ� ����)
		for (int l = 1; l < setListNum; l++) {
			//�κ����� �� ���� ���� k���� ��
			if (setList[l]->getCount() == k) {
				//�ش� �κ������� ���Ұ� �ƴϸ鼭 V1�� �ƴ� �͵�(�� n-1-k��)
				int * complementIndex = (setList[l]->Complement()).getIndex();
				//�ش� �κ������� ������ �͵�(�� k��)
				int * index = setList[l]->getIndex();
				//�ش� �κ������� ���Ұ� �ƴϸ鼭 V1�� �ƴ� �͵� Ž��
				//complementIndex[i]�� �׻� V1�̱⿡ i:1���� ����
				for (int i = 1; i < n - k; i++) {
					int cidx = complementIndex[i];
					//�ش� �κ������� ������ �͵� Ž��
					for (int j = 0; j < k; j++) {
						int idx = index[j];
						//w: W[Vi][Vj]
						int w = W[cidx][idx];
						//�ش� ���Ҹ� �������� ��ȯ
						int idx_binary = (int)pow(2, idx);
						//{Va - Vj}�� �������� ��ȯ
						int tmp_binary = ((*setList[l]) - Set(idx_binary, source, n)).getByte();
						//d: D[Vj][{Va-Vj}], tmp_binary�� l ���·� ��ȯ(2�� ����)
						int d = D[idx][tmp_binary / 2];
						//������ ���� ���� ������ ���� ���
						if (w+d < D[cidx][l]) {
							D[cidx][l] = w+d;
							P[cidx][l] = idx;
						}
					}
				}
				delete index;
				delete complementIndex;
			}
		}
	}
	//D[V1][V-{v1}], P[V1][V-{v1}]
	for (int idx = 1; idx < n; idx++) {
		//w: W[V1][Vi]
		int w = W[0][idx];
		//idx_binary: i �������� ��ȯ
		int idx_binary = (int)pow(2, idx);
		//{Vn}-{V1}���� Vi ����
		int tmp_binary = ((*setList[setListNum - 1])\
			- Set(idx_binary, source, n)).getByte();
		//d: D[Vi][{Vn}-{V1, Vi}]
		int d = D[idx][tmp_binary / 2];
		//������ ���� ���� ������ ���� ���
		if (w + d < D[0][setListNum - 1]) {
			D[0][setListNum - 1] = w + d;
			P[0][setListNum - 1] = idx;
		}
	}

	/*
	printf("Byte|");
	for (int i = 0; i < setListNum; i++) {
		printf("%6d|    ", setList[i]->getByte());
	}
	putchar('\n');
	*/
	printf("Vn  |");
	for (int i = 0; i < setListNum; i++) {
		PrintVertice(setList[i]->getByte(), n);
	}
	printf("\n");
	printf("�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\
�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\
�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�\n");
	printf("D\n");
	PrintArray(D, n, setListNum);
	printf("P\n");
	PrintArray(P, n, setListNum);
	printf("Path: ");
	PrintPath(P, setList, n, setListNum, source);
	
	for (int i = 0; i < n; i++) {
		delete D[i];
		delete P[i];
	}
	delete D;
	delete P;
	return;
}

int main(void) {

	int W[N][N] = 
	{{0, 8, 13, 18, 20},
	{3, 0, 7, 8, 10},
	{4, 11, 0, 10, 7},
	{6, 6, 7, 0, 11},
	{10, 6, 2, 1, 0}};

	TravelSalesPerson(W, N);

	return 0;
}