#define NSIZE 5
#define INFINITY_NUMBER 99
#include<iostream>
#include<stdio.h>

//행렬 출력하는 함수
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

//행렬 내용 복사하는 함수
void CopyArray(int A[][NSIZE], int B[][NSIZE], int rSize, int cSize) {
	for (int i = 0; i < rSize; i++)
		for(int j = 0; j < cSize; j++)
			A[i][j] = B[i][j];
	return;
}

//D배열, P 배열 생성
void FloydAlgorithm(int n, int W[][NSIZE], int D[][NSIZE], int P[][NSIZE]){
	//W 행렬의 값을 D 행렬에 복사
	CopyArray(D, W, NSIZE, NSIZE);
	for(int k = 0; k < n; k++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				//현재의 최단 경로보다 정점 k를 거친 최단 경로가 더 빠를 경우
				if (D[i][k] + D[k][j] < D[i][j]) {
					//P행렬에 정점 k의 인덱스 입력
					P[i][j] = k;
					//D행렬의 값을 중간 정점 k를 거친 최단 경로의 값으로 갱신 
					D[i][j] = D[i][k] + D[k][j];
				}
	return;
}

//재귀 이용해 행렬 P에서 최단 경로 출력
void Recursive_Path(int P[][NSIZE], int q, int r) {
	if ((P[q][r] != 0) && (P[q][r] != -1)) {
		Recursive_Path(P, q, P[q][r]);
		printf("V%d  ", P[q][r]);
		Recursive_Path(P, P[q][r], r);
	}
	return;
}

//최단경로 출력
void PrintPath(int P[][NSIZE], int q, int r) {
	printf("V%d  ", q);
	//재귀 함수 호출
	Recursive_Path(P, q, r);
	printf("V%d  ", r);
	return;
}

int main(void) {
	//문제 1의 인접 행렬
	int W1[NSIZE][NSIZE] = { 
	{0, 10, 5, INFINITY_NUMBER, INFINITY_NUMBER},
	{INFINITY_NUMBER, 0, 2, 1, INFINITY_NUMBER},
	{INFINITY_NUMBER, 3, 0, 9, 2 },
	{INFINITY_NUMBER, INFINITY_NUMBER, INFINITY_NUMBER, 0, 4},
	{ 7, INFINITY_NUMBER, INFINITY_NUMBER, 5, 0 }};
	//문제 2의 인접 행렬
	int W2[NSIZE][NSIZE] = { 
	{0, 1, INFINITY_NUMBER, 1, 5},
	{9, 0, 3, 2, INFINITY_NUMBER},
	{INFINITY_NUMBER, INFINITY_NUMBER, 0, 4, INFINITY_NUMBER},
	{INFINITY_NUMBER, INFINITY_NUMBER, 2, 0, 3},
	{3, INFINITY_NUMBER, INFINITY_NUMBER, INFINITY_NUMBER, 0}};
	//문제 3,4의 인접 행렬
	int W3[NSIZE][NSIZE] = {
	{0, 4, INFINITY_NUMBER, INFINITY_NUMBER, INFINITY_NUMBER},
	{5, 0, 7, INFINITY_NUMBER, INFINITY_NUMBER},
	{INFINITY_NUMBER, INFINITY_NUMBER, 0, 10, 5},
	{1, INFINITY_NUMBER, INFINITY_NUMBER, 0, 2},
	{9, 2, INFINITY_NUMBER, 3, 0} };
	
	int D[NSIZE][NSIZE] = { 0, };
	int P[NSIZE][NSIZE];
	//행렬 P에서 중간 정점 0과 중간 정점이 없을 때를 구분하기 위해 초기 값을 -1로 초기화
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