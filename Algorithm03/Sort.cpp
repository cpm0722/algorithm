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

//array �迭���� i��°�� j��° ���� ��ȯ�ϴ� �Լ�
template <typename T>
void swap(T *array, int i, int j) {
	T temp = array[i];
	array[i] = array[j];
	array[j] = temp;
	return;
}

//��ȯ ����
template <typename T>
T* ExchangeSort(int len, T *array) {
	for (int i = 0; i < len - 1; i++)	//i�� 1���� n - 1���� Ž�� 
		for (int j = i + 1; j < len; j++)	//j�� i + 1���� n���� Ž��
			if (array[i] > array[j])	//i���� j������ ũ�� swap
				swap(array, i, j);
	return array;
}

//���� ����
template <typename T>
T* InsertionSort(int len, T* array) {
	for (int i = len - 1; i >= 0; i--)	//i�� n���� 0���� Ž��
		for (int j = 0; j < i; j++)		//j�� 0���� i���� Ž��
			if (array[j] > array[i])	//j���� i������ ũ�� swap
				swap(array, i, j);
	return array;
}

//�������Ŀ��� ���� �迭�� ���� �迭 �����ϴ� �Լ�
template <typename T>
void Merge(T* array, int left, int mid, int right) {	
	if (right == left)	//������ 1�̸� ����
		return;
	if (right - left == 1) {	//������ 2�̸�
		if (array[left] > array[right])		//���������� ���� swap
			swap(array, left, right);
		return;
	}
	int len = right - left + 1;		//������ �迭 �� ���� �ľ�
	T* tempArray = new T[len];		//�ӽ� ���� �迭 ����
	for (int t = 0; t < len; t++)
		tempArray[t] = 0;
	int i = left, j = mid + 1, k = 0;	//i(����), j(����), k(�ӽ� �迭) Ž��
	while ((i <= mid) && (j <= right)) {//���� or ���� �迭 Ž�� ���� ������
		if (array[i] < array[j])
			tempArray[k++] = array[i++];
		else tempArray[k++] = array[j++];
	}
	if (i <= mid)		//Ž���� ���� �� ���� �迭�� �����ִ� ���
		for (; i <= mid; i++)	//���� ���� �迭 ��� �ӽ� �迭�� �߰�
			tempArray[k++] = array[i];
	else if(j <= right)	//Ž���� ���� �� ���� �迭�� �����ִ� ���
		for (; j <= right; j++) {//���� ���� �迭 ��� �ӽ� �迭�� �߰�
			tempArray[k] = array[j];
			k++;
		}
	for (k = 0; k < len; k++)	//�ӽ� �迭�� ���� ���� �迭�� �̵�
		array[left + k] = tempArray[k];
	delete []tempArray;
	return;
}

//��� �̿��� ���� ���� ����
template <typename T>
void RecursiveMergeSort(T* array, int left, int right) {
	int mid = (left + right) / 2;	//mid �� ����
	//left == right�� ��(������ 1���� ��)���� ��� ȣ��
	if (left < right) {	
		RecursiveMergeSort(array, left, mid);
		RecursiveMergeSort(array, mid + 1, right);
		//mid �������� ����, ���� �迭�� �����ϸ鼭 ��ü �񳻸��������� ����
		Merge(array, left, mid, right);	
	}
	return;
}

//���� ����
template <typename T>
T* MergeSort(int len, T* array) {
	RecursiveMergeSort(array, 0, len - 1);	//��� �̿��ϱ� ���� �Լ� ȣ��
	return array;
}

//�������� �������ִ� ��� �Լ�
template <typename T>
void RecursiveQuickSort(T* array, int l, int r) {
	if (r == l)		//������ 1���̸� return		
		return;
	else if (r - l == 1) {			//������ 2���̸�
		if (array[l] > array[r])	//l���� r������ ũ�� swap
			swap(array, l, r);
		return;
	}
	int len = r - l + 1;
	//i(����)�� j(����)�� l-1, k(���� �迭)�� l���� Ž��
	int i = l - 1, j = l - 1, k = l;
	while (j < r) {			//j�� r���� ������(= Ž���� ������ �ʾҴٸ�)
		if (array[k] >= array[r])//k���� r������ ũ�ٸ� j+1
			j++;
		else {			//k���� r������ �۴ٸ� i+1, j+1, i+1���� j�� ��ȯ
			i++;
			j++;
			swap(array, i, k);
		}
		k++;
	}
	swap(array, i + 1, r);
	if (i + 1 == l)	//r������ ���� ���� ���ٸ�	 ������ QuickSort ��� ȣ��
		RecursiveQuickSort(array, l + 1, r);
	else if (i + 1 == r)//r������ ū ���� ���ٸ�	������ QuickSort ��� ȣ��
		RecursiveQuickSort(array, l, r - 1);
	else {	//�� �� ������ ���� ��� QuickSort ��� ȣ��
		RecursiveQuickSort(array, l, i);
		RecursiveQuickSort(array, i + 2, r);
	}
	return;
}

//�� ����
template <typename T>
T* QuickSort(int len, T* array) {
	//Quick���� ���� ���� ��� �Լ� ���
	RecursiveQuickSort(array, 0, len - 1);
	return array;
}

//�迭 ��� �Լ�
template <typename T>
void PrintArray(int len, T *array) {
	for (int i = 0; i < len; i++)
		printf("%.3lf\t", array[i]);
	putchar('\n');
	return;
}

//�迭 ���� �Լ�
template <typename T>
T* CopyArray(int len, T* a) {
	T* b = new T[len];
	for (int i = 0; i < len; i++)
		b[i] = a[i];
	return b;
}

//n���� random -1~+1 float���� ���� �迭 ���� ��
//4���� ���� �˰������� ��� �� �ð� ����
void AllSorts(int n, bool needPrint = false) {
	float *originalArray = new float[n];
	//-1~1���� ���� �Ǽ� ����
	for (int i = 0; i < n; i++)
		originalArray[i] = (rand() % 2001) / (float)1000 - (float)1;
	if (needPrint) {	//����� ��� O, �ð� ���� X
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
	else {	//����� ��� X, �ð� ���� O
		_int64 timeArray[4] = { 0, };
		//��ȯ����
		QueryPerformanceCounter(&BeginTime);
		ExchangeSort(n, CopyArray(n, originalArray));
		QueryPerformanceCounter(&EndTime);
		timeArray[0] = (EndTime.QuadPart - BeginTime.QuadPart)\
			/ (Frequency.QuadPart / 1000000);
		
		//��������
		QueryPerformanceCounter(&BeginTime);
		InsertionSort(n, CopyArray(n, originalArray));
		QueryPerformanceCounter(&EndTime);
		timeArray[1] = (EndTime.QuadPart - BeginTime.QuadPart)\
			/ (Frequency.QuadPart / 1000000);
		
		//��������
		QueryPerformanceCounter(&BeginTime);
		MergeSort(n, CopyArray(n, originalArray));
		QueryPerformanceCounter(&EndTime);
		timeArray[2] = (EndTime.QuadPart - BeginTime.QuadPart)\
			/ (Frequency.QuadPart / 1000000);
		
		//������
		QueryPerformanceCounter(&BeginTime);
		QuickSort(n, CopyArray(n, originalArray));
		QueryPerformanceCounter(&EndTime);
		timeArray[3] = (EndTime.QuadPart - BeginTime.QuadPart)\
			/ (Frequency.QuadPart / 1000000);

		//���� �ð� ���
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