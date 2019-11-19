#define NSIZE 5
#define INFINITY_NUMBER 99
#include<iostream>
#include<stdio.h>

//��� ����ϴ� �Լ�
void PrintArray(int A[][NSIZE], int rSize, int cSize) {
	for (int i = 0; i < rSize; i++) {
		for (int j = 0; j < cSize; j++)
			if(A[i][j] != INFINITY_NUMBER)
				printf("%3d\t", A[i][j]);
			else printf("inf\t");
		putchar('\n');
	}
	return;
	putchar('\n');
}

//��� ���� �����ϴ� �Լ�
void CopyArray(int A[][NSIZE], int B[][NSIZE], int rSize, int cSize) {
	for (int i = 0; i < rSize; i++)
		for(int j = 0; j < cSize; j++)
			A[i][j] = B[i][j];
	return;
}

//D�迭, P �迭 ����
void FloydAlgorithm(int n, int W[][NSIZE], int D[][NSIZE], int P[][NSIZE]){
	//W ����� ���� D ��Ŀ� ����
	CopyArray(D, W, NSIZE, NSIZE);
	for(int k = 0; k < n; k++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				//������ �ִ� ��κ��� ���� k�� ��ģ �ִ� ��ΰ� �� ���� ���
				if (D[i][k] + D[k][j] < D[i][j]) {
					//P��Ŀ� ���� k�� �ε��� �Է�
					P[i][j] = k;
					//D����� ���� �߰� ���� k�� ��ģ �ִ� ����� ������ ���� 
					D[i][j] = D[i][k] + D[k][j];
				}
	return;
}

//��� �̿��� ��� P���� �ִ� ��� ���
void Recursive_Path(int P[][NSIZE], int q, int r) {
	if ((P[q][r] != 0) && (P[q][r] != -1)) {
		Recursive_Path(P, q, P[q][r]);
		printf("V%d  ", P[q][r]);
		Recursive_Path(P, P[q][r], r);
	}
	return;
}

//�ִܰ�� ���
void PrintPath(int P[][NSIZE], int q, int r) {
	printf("V%d  ", q);
	//��� �Լ� ȣ��
	Recursive_Path(P, q, r);
	printf("V%d  ", r);
	return;
}

int main(void) {
	//���� 1�� ���� ���
	int W1[NSIZE][NSIZE] = { 
	{0, 10, 5, INFINITY_NUMBER, INFINITY_NUMBER},
	{INFINITY_NUMBER, 0, 2, 1, INFINITY_NUMBER},
	{INFINITY_NUMBER, 3, 0, 9, 2 },
	{INFINITY_NUMBER, INFINITY_NUMBER, INFINITY_NUMBER, 0, 4},
	{ 7, INFINITY_NUMBER, INFINITY_NUMBER, 5, 0 }};
	//���� 2�� ���� ���
	int W2[NSIZE][NSIZE] = { 
	{0, 1, INFINITY_NUMBER, 1, 5},
	{9, 0, 3, 2, INFINITY_NUMBER},
	{INFINITY_NUMBER, INFINITY_NUMBER, 0, 4, INFINITY_NUMBER},
	{INFINITY_NUMBER, INFINITY_NUMBER, 2, 0, 3},
	{3, INFINITY_NUMBER, INFINITY_NUMBER, INFINITY_NUMBER, 0}};
	//���� 3,4�� ���� ���
	int W3[NSIZE][NSIZE] = {
	{0, 4, INFINITY_NUMBER, INFINITY_NUMBER, INFINITY_NUMBER},
	{5, 0, 7, INFINITY_NUMBER, INFINITY_NUMBER},
	{INFINITY_NUMBER, INFINITY_NUMBER, 0, 10, 5},
	{1, INFINITY_NUMBER, INFINITY_NUMBER, 0, 2},
	{9, 2, INFINITY_NUMBER, 3, 0} };
	
	int D[NSIZE][NSIZE] = { 0, };
	int P[NSIZE][NSIZE];
	//��� P���� �߰� ���� 0�� �߰� ������ ���� ���� �����ϱ� ���� �ʱ� ���� -1�� �ʱ�ȭ
	for (int i = 0; i < NSIZE; i++)
		for (int j = 0; j < NSIZE; j++)
			P[i][j] = -1;
	
	FloydAlgorithm(NSIZE, W3, D, P);

	printf("\nMatrix W: \n");
	PrintArray(W3, NSIZE, NSIZE);
	printf("\nMatrix D: \n");
	PrintArray(D, NSIZE, NSIZE);
	printf("\nMatrix P: \n");
	PrintArray(P, NSIZE, NSIZE);
	printf("\nPath From a3 to a1: ");
	PrintPath(P, 2, 0);
	putchar('\n');
}