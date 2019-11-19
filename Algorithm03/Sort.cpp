#include<iostream>
#include<ctime>
#include<cstdlib>
#include<cstdio>
#include<ctime>
#include<Windows.h>

#define NARRAYSIZE 12 

using namespace std;

LARGE_INTEGER Frequency;
LARGE_INTEGER BeginTime;
LARGE_INTEGER EndTime;

//array 배열에서 i번째와 j번째 값을 교환하는 함수
template <typename T>
void swap(T *array, int i, int j) {
	T temp = array[i];
	array[i] = array[j];
	array[j] = temp;
	return;
}

//교환 정렬
template <typename T>
T* ExchangeSort(int len, T *array) {
	for (int i = 0; i < len - 1; i++)	//i를 1부터 n - 1까지 탐색 
		for (int j = i + 1; j < len; j++)	//j를 i + 1부터 n까지 탐색
			if (array[i] > array[j])	//i값이 j값보다 크면 swap
				swap(array, i, j);
	return array;
}

//삽입 정렬
template <typename T>
T* InsertionSort(int len, T* array) {
	for (int i = len - 1; i >= 0; i--)	//i를 n부터 0까지 탐색
		for (int j = 0; j < i; j++)		//j를 0부터 i까지 탐색
			if (array[j] > array[i])	//j값이 i값보다 크면 swap
				swap(array, i, j);
	return array;
}

//병합정렬에서 좌측 배열과 우측 배열 병합하는 함수
template <typename T>
void Merge(T* array, int left, int mid, int right) {	
	if (right == left)	//개수가 1이면 종료
		return;
	if (right - left == 1) {	//개수가 2이면
		if (array[left] > array[right])		//내림차순일 때만 swap
			swap(array, left, right);
		return;
	}
	int len = right - left + 1;		//병합할 배열 총 길이 파악
	T* tempArray = new T[len];		//임시 저장 배열 선언
	for (int t = 0; t < len; t++)
		tempArray[t] = 0;
	int i = left, j = mid + 1, k = 0;	//i(좌측), j(우측), k(임시 배열) 탐색
	while ((i <= mid) && (j <= right)) {//좌측 or 우측 배열 탐색 끝날 때까지
		if (array[i] < array[j])
			tempArray[k++] = array[i++];
		else tempArray[k++] = array[j++];
	}
	if (i <= mid)		//탐색이 끝난 후 좌측 배열이 남아있는 경우
		for (; i <= mid; i++)	//남은 좌측 배열 모두 임시 배열에 추가
			tempArray[k++] = array[i];
	else if(j <= right)	//탐색이 끝난 후 우측 배열이 남아있는 경우
		for (; j <= right; j++) {//남은 우측 배열 모두 임시 배열에 추가
			tempArray[k] = array[j];
			k++;
		}
	for (k = 0; k < len; k++)	//임시 배열의 값을 실제 배열로 이동
		array[left + k] = tempArray[k];
	delete []tempArray;
	return;
}

//재귀 이용한 병합 정렬 구현
template <typename T>
void RecursiveMergeSort(T* array, int left, int right) {
	int mid = (left + right) / 2;	//mid 값 설정
	//left == right일 때(갯수가 1개일 때)까지 재귀 호출
	if (left < right) {	
		RecursiveMergeSort(array, left, mid);
		RecursiveMergeSort(array, mid + 1, right);
		//mid 기준으로 좌측, 우측 배열을 병합하면서 전체 비내림차순으로 정렬
		Merge(array, left, mid, right);	
	}
	return;
}

//병합 정렬
template <typename T>
T* MergeSort(int len, T* array) {
	RecursiveMergeSort(array, 0, len - 1);	//재귀 이용하기 위해 함수 호출
	return array;
}

//퀵정렬을 구현해주는 재귀 함수
template <typename T>
void RecursiveQuickSort(T* array, int l, int r) {
	if (r == l)		//개수가 1개이면 return		
		return;
	else if (r - l == 1) {			//개수가 2개이면
		if (array[l] > array[r])	//l값이 r값보다 크면 swap
			swap(array, l, r);
		return;
	}
	int len = r - l + 1;
	//i(좌축)와 j(우측)은 l-1, k(실제 배열)은 l부터 탐색
	int i = l - 1, j = l - 1, k = l;
	while (j < r) {			//j가 r보다 작으면(= 탐색이 끝나지 않았다면)
		if (array[k] >= array[r])//k값이 r값보다 크다면 j+1
			j++;
		else {			//k값이 r값보다 작다면 i+1, j+1, i+1값과 j값 교환
			i++;
			j++;
			swap(array, i, k);
		}
		k++;
	}
	swap(array, i + 1, r);
	if (i + 1 == l)	//r값보다 작은 값이 없다면	 우측만 QuickSort 재귀 호출
		RecursiveQuickSort(array, l + 1, r);
	else if (i + 1 == r)//r값보다 큰 값이 없다면	좌측만 QuickSort 재귀 호출
		RecursiveQuickSort(array, l, r - 1);
	else {	//그 외 좌측과 우측 모두 QuickSort 재귀 호출
		RecursiveQuickSort(array, l, i);
		RecursiveQuickSort(array, i + 2, r);
	}
	return;
}

//퀵 정렬
template <typename T>
T* QuickSort(int len, T* array) {
	//Quick정렬 구현 위해 재귀 함수 사용
	RecursiveQuickSort(array, 0, len - 1);
	return array;
}

//배열 출력 함수
template <typename T>
void PrintArray(int len, T *array) {
	for (int i = 0; i < len; i++)
		printf("%.3lf\t", array[i]);
	putchar('\n');
	return;
}

//배열 복사 함수
template <typename T>
T* CopyArray(int len, T* a) {
	T* b = new T[len];
	for (int i = 0; i < len; i++)
		b[i] = a[i];
	return b;
}

//n개의 random -1~+1 float값을 가진 배열 생성 후
//4가지 정렬 알고리즘으로 계산 및 시간 측정
void AllSorts(int n, bool needPrint = false) {
	float *originalArray = new float[n];
	//-1~1사이 랜덤 실수 생성
	for (int i = 0; i < n; i++)
		originalArray[i] = (rand() % 2001) / (float)1000 - (float)1;
	if (needPrint) {	//결과값 출력 O, 시간 측정 X
		QueryPerformanceFrequency(&Frequency);
		cout << "n: " << n << endl;
		cout << "Original: " << endl;
		PrintArray(n, originalArray);
		cout << endl << "ExchangeSort: " << endl;
		PrintArray(n, ExchangeSort(n, CopyArray(n, originalArray)));
		cout << endl << "InsertionSort: " << endl;
		PrintArray(n, InsertionSort(n, CopyArray(n, originalArray)));
		cout << endl << "MergeSort: " << endl;
		PrintArray(n, MergeSort(n, CopyArray(n, originalArray)));
		cout << endl << "QuickSort: " << endl;
		PrintArray(n, QuickSort(n, CopyArray(n, originalArray)));
	}
	else {	//결과값 출력 X, 시간 측정 O
		_int64 timeArray[4] = { 0, };
		//교환정렬
		QueryPerformanceCounter(&BeginTime);
		ExchangeSort(n, CopyArray(n, originalArray));
		QueryPerformanceCounter(&EndTime);
		timeArray[0] = (EndTime.QuadPart - BeginTime.QuadPart)\
			/ (Frequency.QuadPart / 1000000);
		
		//삽입정렬
		QueryPerformanceCounter(&BeginTime);
		InsertionSort(n, CopyArray(n, originalArray));
		QueryPerformanceCounter(&EndTime);
		timeArray[1] = (EndTime.QuadPart - BeginTime.QuadPart)\
			/ (Frequency.QuadPart / 1000000);
		
		//병합정렬
		QueryPerformanceCounter(&BeginTime);
		MergeSort(n, CopyArray(n, originalArray));
		QueryPerformanceCounter(&EndTime);
		timeArray[2] = (EndTime.QuadPart - BeginTime.QuadPart)\
			/ (Frequency.QuadPart / 1000000);
		
		//퀵정렬
		QueryPerformanceCounter(&BeginTime);
		QuickSort(n, CopyArray(n, originalArray));
		QueryPerformanceCounter(&EndTime);
		timeArray[3] = (EndTime.QuadPart - BeginTime.QuadPart)\
			/ (Frequency.QuadPart / 1000000);

		//연산 시간 출력
		for (int i = 0; i < 4; i++)
			cout << (long long int)timeArray[i] << '\t';
		cout << endl;
	}
	delete []originalArray;
	return;
}

//Main Function
int main(void) {
	QueryPerformanceFrequency(&Frequency);
	srand((unsigned int)time(NULL));
	AllSorts(10, true);
	cout << "-------------------------------------------------\
-----------------------------" << endl;
	int nArray[NARRAYSIZE] = { 100, 200, 300, 400, 500, 750, 1000, 1500,\
		2000, 3000, 4000, 5000 };
	for (int i = 0; i < NARRAYSIZE; i++) {
		cout << "n: " << nArray[i] << endl;
		AllSorts(nArray[i], false);
	}
	return 0;
}