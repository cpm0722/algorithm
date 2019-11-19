#include<stdio.h>
#include<iostream>

#define NUMBEROFMATRIX 5
#define NUMBEROFNODE 6

template <typename T>
T minimum(T a, T b) {
	if (a < 0)
		return b;
	else if (b < 0)
		return a;
	
	if (a < b)
		return a;
	else return b;
}

//�迭 ��� �Լ�
void PrintArray(int** array, int row, int col) {
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++) {
			printf("%d\t", array[i][j]);
		}
		putchar('\n');
	}
	putchar('\n');
	return;
}

//�迭 ��� �Լ�
void PrintDoubleArray(double** array, int row, int col) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (array[i][j] >= 0)
				putchar('+');
			printf("%.2lf\t", array[i][j]);
		}
		putchar('\n');
	}
	return;
}

//��� ��� �Լ�
void PrintPath(int **P, int row, int col) {
	if (row == col)
		printf("A[%d]", row);
	else {
		putchar('(');
		PrintPath(P, row, P[row][col]);
		PrintPath(P, P[row][col]+1, col);
		putchar(')');
	}
}

void MatrixChainMultiplication(int *d, int n) {
	// M, P �迭 �ʱ�ȭ
	int **M = new int *[n];
	int **P = new int *[n - 1];
	for (int i = 0; i < n; i++) {
		M[i] = new int[n];
		if (i != n - 1)
			P[i] = new int[n];
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			if (i != n - 1)
				M[i][j] = P[i][j] = -1;
			else M[i][j] = -1;
			if (i == j)
				M[i][j] = 0;
		}

	//M, P �迭 �ϼ�
	//diagonal: �밢�� ����, 1~n-1
	for (int diagonal = 1; diagonal <= n-1; diagonal++) {
		//i: row ����, 0~n-dianogal
		for (int i = 0; i < n - diagonal; i++) {
			//j: col ����, i + diagonal
			int j = i + diagonal;
			int tmp;
			for (int k = i; k < j; k++) {
				tmp = M[i][j];
				//�ּڰ� M[i][j]�� �Է�, d���������� ������ index���� + 1
				M[i][j] = minimum(tmp, M[i][k] + M[k + 1][j] + (d[i] * d[k+1] * d[j+1]));
				//M[i][j]���� ��ȭ������ P[i][j]�� k�� �Է�
				if (tmp != M[i][j])
					P[i][j] = k;
			}
		}
	}

	//Print Array
	printf("M\n");
	PrintArray(M, n, n);
	printf("P\n");
	PrintArray(P, n - 1, n);

	//Print Path
	PrintPath(P, 0, n - 1);

	//free
	for (int i = 0; i < n; i++) {
		delete(M[i]);
		if (i != n - 1)
			delete(P[i]);
	}
	delete(M);
	delete(P);

	return;
}

struct node {
	double key;
	node *left;
	node *right;
};

//R �迭 �̿��� Optimal�� BinaryTree ���� �Լ�
node* Tree(int ** R, int i, int j) {
	int k;
	node* p;
	k = R[i][j];
	if (i == 6 && j == 6)
		return NULL;
	if (k == -1) {
		printf("%d\t", i);
		return NULL;
	}
	else {
		printf("%d\t", k);
		p = new node;
		p->key = k;
		p->left = Tree(R, i, k - 1);
		p->right = Tree(R, k + 1, j);
		return p;
	}
}

//Optimal Binary Tree ���� �Լ�
void OptimalBinaryTree(double *pb, int n) {
	//�迭 ����
	double **A = new double *[n + 1];
	int **R = new int *[n + 1];
	for (int i = 0; i < n + 1; i++) {
		A[i] = new double[n + 1];
		R[i] = new int[n + 1];
	}

	//�迭 �ʱ�ȭ (size: n+1 x n+1)
	for (int i = 0; i < n + 1; i++) {
		for (int j = 0; j < n + 1; j++) {
			A[i][j] = -1;
			R[i][j] = -1;
			if (i == j) {
				A[i][j] = 0;
				R[i][j] = -1;
			}
			else if (i + 1 == j) {
				A[i][j] = pb[i];
				R[i][j] = i;
			}
		}
	}
	
	//�迭 �� �Է�
	//diagonalL �밢�� ����, 1~n (diagonal=1�� �̹� �迭 �ʱ�ȭ �� Ȯ�� ������ �Ҵ��߱⿡ Ž��X)
	for (int diagonal=2; diagonal<n+1; diagonal++) {
		//i: row����, 0~n-diagonal
		for (int i=0; i<=n-diagonal; i++) {
			//j: col����, i + diagonal��
			int j = i + diagonal;
			//k�� roop ������ �̿��� �ּҰ� ã�Ƽ� A[i][j]�� �� �Է�
			//k: 0~diagonal-1
			//k = 0�� ���� �̸� A[i][j]�� �Է� (minimum �Լ� ��� ����)
			int k = 0;
			A[i][j] = A[i][i + k] + A[i + 1 + k][j];
			R[i][j] = i + 0;
			for (k=1; k<diagonal; k++) {
				double tmp = A[i][j];
				A[i][j] = minimum(A[i][j], A[i][i+k]+A[i + 1 + k][j]);
				if (tmp != A[i][j])
					R[i][j] = i + k;
			}
			//Ȯ���� �Է�
			double sigma = 0;
			for (int k = 0; k < diagonal; k++)
				sigma += A[i+k][i+1+k];
			A[i][j] += sigma;
		}

	}

	//Print Array
	printf("\nA\n");
	PrintDoubleArray(A, n + 1, n + 1);
	printf("R\n");
	PrintArray(R, n + 1, n + 1);

	Tree(R, 0, n);

	//free
	for (int i = 0; i < n + 1; i++) {
		delete A[i];
		delete R[i];
	}
	delete A;
	delete R;
	return;
}

int main(void) {
	int d[NUMBEROFMATRIX + 1] = { 10, 4, 5, 20, 2, 50 };
	MatrixChainMultiplication(d, NUMBEROFMATRIX);

	double pb[NUMBEROFNODE] = { 0.05, 0.15, 0.05, 0.35, 0.05, 0.35 };
	OptimalBinaryTree(pb, NUMBEROFNODE);
	return 0;
}