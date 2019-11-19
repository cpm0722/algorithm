#include<stdio.h>
#include<windows.h>
#include<WinBase.h>
#include"set.h"
#define N 8
#define ESTIMATE_TIME 20
#define M 6

//n-Queens Problem에서 유망한지 확인
bool promising_nQueens(int col[N], int now) {
	bool isPromising = true;
	int i = 0;
	//now 이전에 배치된 Queen들과 비교
	while (i < now && isPromising) {
		//같은 column, 같은 대각선 확인
		if (col[now] == col[i] || abs(col[now] - col[i] == abs(now - i)))
			isPromising = false;
		i++;
	}
	return isPromising;
}

//몬테칼로 Algorithm 이용해 n-Queens Problem 효율 추정
int estimate_nQueens(int n) {
	//col[i]: i번쨰 row의 queen이 위치하는 col의 index
	int col[N] = { 0, };
	//Set<int> 사용하기 위한 소스 배열
	int source_col[N] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	//현재의 위치에서 Promising한 Child의 index 집합
	Set<int> prom_children(0, source_col, n);
	//numNodes: 최종 return 값
	//m: 현재 promising한 child의 갯수
	//mProd: 현재까지 구해진 m들의 곱
	int i = 0, numNodes = 1, m = 1, mProd = 1;
	//promising한 child가 없거나 n까지 탐색 완료했을 때 반복 종료
	while (m != 0 && i < n - 1) {
		mProd = mProd * m;
		//n-Queen Problems에서 t(실제 Child 갯수)는 항상 n
		numNodes = numNodes + mProd * n;
		i++;
		m = 0;
		//prom_children 공집합으로 초기화
		prom_children = (prom_children) && (*new Set<int>(0, source_col, n));
		//i번째 row에 있는 Queen의 col을 0~n으로 하나씩 지정하면서 반복
		for (int j = 0; j < n; j++) {
			col[i] = j;
			//현재가 promising하다면
			if (promising_nQueens(col, i)) {
				m++;
				//prom_children U {j}
				prom_children = prom_children + j;
			}
		}
		//promising한 child 중 임의의 한 개 뽑아 col값으로 지정
		if (m != 0) {
			int rnd = prom_children.getRandomIndex();
			col[i] = rnd;
		}
	}
	return numNodes;
}


//Promising한지 확인
 bool promising_sumOfSubsets(int i, int weight, int total, int W, int w[M]) {
	 return (weight + total >= W) && (weight == W || weight + w[i + 1] <= W); 
 } 
 


//Sum of Subsets Problem 
void sum_of_subsets(Set<int> subset, int i, int weight, int total, int W, int w[M]){
	//유망할 경우
	if (promising_sumOfSubsets(i, weight, total, W, w)) {
		//정답일 경우 집합 출력
		if (weight == W)
			subset.PrintSet();
		else {
			//i+1 포함 후 재귀 호출
			subset = subset + (i + 1);
			sum_of_subsets(subset, i + 1, weight + w[i + 1], total - w[i + 1], W, w);
			//i+1 제외 후 재귀 호출
			subset = subset - (i + 1);
			sum_of_subsets(subset, i + 1, weight, total - w[i + 1], W, w);
		}
	}
	return;
}

//Sum of Subsets Problem 
//정답 도출 시 return true 통해 바로 종료
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

//몬테칼로 Algorithm 사용해 Sum of Subsets Problem 효율 추정
int estimate_sum_of_subsets(int n, int W, int w[M]) {
	int i = 0, m = 1, mProd = 1, numNodes = 1, weight = 0, total = 0;
	for (int j = 0; j < n; j++) {
		total += w[j];
	}
	//i+1번째 원소가 포함되었을 때 유망한지, 배제되었을 때 유망한지
	bool isIncludePromising = false, isExcludePromising = false;
	//집합 초기화
	Set<int> subsets = *new Set<int>(0, w, n);
	while (m != 0 && i < n - 1) {
		//boolean 변수 초기화
		isIncludePromising = false;
		isExcludePromising = false;
		//total 갱신
		total -= w[i];
		//mProd: numNodes에 더하기 위한 변수
		mProd *= m;
		//sum_of_subsets Algorithm에서 t는 항상 2 (포함 or 포함X)
		numNodes += mProd * 2;
		
		//i+1번째 원소 포함한 경우
		subsets = subsets + i;
		weight += w[i];
		isIncludePromising = promising_sumOfSubsets(i, weight, total, W, w);
		
		//i+1번째 원소 배제한 경우
		subsets = subsets - i;
		weight -= w[i];
		isExcludePromising = promising_sumOfSubsets(i, weight, total, W, w);

		//i+1번째 원소 포함한 경우 Promising
		//i+1번째 원소 배제한 경우 Promising
		if (isIncludePromising && isExcludePromising) {
			m = 2;
			int rnd = rand() % 2;
			if (rnd == 0) {
				subsets = subsets + i;
				weight += w[i];
			}
		}
		//i+1번째 원소 포함한 경우 Promising
		//i+1번째 원소 배제한 경우 NonPromising
		else if (isIncludePromising && !isExcludePromising) {
			m = 1;
			subsets = subsets + i;
			weight += w[i];
		}
		//i+1번째 원소 포함한 경우 NonPromising
		//i+1번째 원소 배제한 경우 Promising
		else if (!isIncludePromising && isExcludePromising) {
			m = 1;
		}
		//i+1번째 원소 포함한 경우 NonPromising
		//i+1번째 원소 배제한 경우 NonPromising
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
		printf("%02d번째: %d\n", i + 1, tmp);
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
	printf("하나의 해답만 찾는 알고리즘의 수행시간: %dmicroSecond\n모든 해답 찾는 알고리즘의 수행시간: \t%dmicroSecond\n", microsec1, microsec2);

	
	//Question 17
	printf("\n\n[Question 17]\n");
	tmp = 0, sum = 0;
	for (int i = 0; i < ESTIMATE_TIME; i++) {
		tmp = estimate_sum_of_subsets(M, W, w);
		printf("%02d번째: %d\n", i + 1, tmp);
		sum += tmp;
	}
	printf("Average numNodes: %02d\n", sum / ESTIMATE_TIME);
	
	return 0;
}