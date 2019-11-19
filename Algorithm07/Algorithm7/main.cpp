#include<stdio.h>
#include<math.h>

#include"set.h"

#define N 5
#define INF 99

//Byte를 {Vn}으로 출력
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

//배열 출력
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

//P배열 이용해 경로 출력
void PrintPath(int **P, Set **setList, int n, \
	int setListNum, int *source) {
	int l = setListNum - 1, v = 0, p = 0;
	//V1 출력
	printf("V%d -> ", p + 1);
	while (true) {
		//공집합일 때 반복 종료
		if (l == 0) break;
		//P[Vi][VA]
		p = P[v][l];
		//P[Vi][VA] 출력
		printf("V%d -> ", p + 1);
		//Vi = P[Vi][VA]
		v = p;
		int v_binary = (int)pow(2, v);
		//VA = VA - {Vi}
		l = ((*setList[l]) - (Set(v_binary, \
			source, n))).getByte() / 2;
	}
	//V1 출력
	printf("V%d\n", 1);
	return;
}

void TravelSalesPerson(int (*W)[N], int n) {
	//source, 각각의 집합 원소의 값 저장
	//(TravelSalesPerson에서는 사용 X)
	int source[5] = { 0, 0, 0, 0, 0 };
	// 2^(N-1), V1,을 제외한 Vertix들의 Subset 갯수
	int setListNum = (int)pow(2, n - 1);
	//V1을 제외한 Vertix들의 Subset 배열
	Set **setList = new Set*[setListNum];
	for (int i = 0; i < setListNum; i++) {
		setList[i] = new Set(i * 2, source, n);
	}

	//D[n][2^(n-1)] 배열 생성
	int **D = new int *[n];
	for (int i = 0; i < n; i++)
		D[i] = new int[setListNum];

	//P[n][2^(n-1)] 배열 생성
	int **P = new int *[n];
	for (int i = 0; i < n; i++)
		P[i] = new int[setListNum];
	
	//배열 초기화
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < setListNum; j++) {
			//Vn, 공집합은 Vn~V1 edge의 weight
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

	//k: 1 ~ n-2개 (V1과 Vi를 제외)
	for (int k = 1; k <= n - 2; k++) {
		//j: 1 ~ 2^(n-1) V1 없는 모든 부분집합 탐색
		//(l:0 공집합은 이미 초기화 때 값 입력했으므로 제외)
		for (int l = 1; l < setListNum; l++) {
			//부분집합 중 원소 갯수 k개일 때
			if (setList[l]->getCount() == k) {
				//해당 부분집합의 원소가 아니면서 V1이 아닌 것들(총 n-1-k개)
				int * complementIndex = (setList[l]->Complement()).getIndex();
				//해당 부분집합의 원소인 것들(총 k개)
				int * index = setList[l]->getIndex();
				//해당 부분집합의 원소가 아니면서 V1이 아닌 것들 탐색
				//complementIndex[i]는 항상 V1이기에 i:1부터 시작
				for (int i = 1; i < n - k; i++) {
					int cidx = complementIndex[i];
					//해당 부분집합의 원소인 것들 탐색
					for (int j = 0; j < k; j++) {
						int idx = index[j];
						//w: W[Vi][Vj]
						int w = W[cidx][idx];
						//해당 원소를 이진수로 변환
						int idx_binary = (int)pow(2, idx);
						//{Va - Vj}를 이진수로 변환
						int tmp_binary = ((*setList[l]) - Set(idx_binary, source, n)).getByte();
						//d: D[Vj][{Va-Vj}], tmp_binary를 l 형태로 변환(2로 나눔)
						int d = D[idx][tmp_binary / 2];
						//현재의 값이 기존 값보다 작을 경우
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
		//idx_binary: i 이진수로 변환
		int idx_binary = (int)pow(2, idx);
		//{Vn}-{V1}에서 Vi 제외
		int tmp_binary = ((*setList[setListNum - 1])\
			- Set(idx_binary, source, n)).getByte();
		//d: D[Vi][{Vn}-{V1, Vi}]
		int d = D[idx][tmp_binary / 2];
		//현재의 값이 기존 값보다 작을 경우
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
	printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\
ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\
ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
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