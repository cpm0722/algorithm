#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<stdbool.h>
#include "mkl.h"

#define PRINTARRAY 1 
#define COMPARE_ALGO3 0
#define LOOP_COUNT 1

void printArray(double *, int, int);

void ALGO1(double *A, double *B, double *C, int m, int n, int p) {
	int i, j, k, r;
	i = j = k = r = 0;
	for (r = 0; r < LOOP_COUNT; r++)
		for (i = 0; i < m; i++)
			for (j = 0; j < n; j++)
				for (k = 0; k < p; k++)
					C[n*i + j] += A[p*i + k] * B[n*k + j];
	return;
}
void ALGO2(double *A, double *B, double *C, int m, int n, int p) {
	int i, j, k, r;
	i = j = k = r = 0;
	for (r = 0; r < LOOP_COUNT; r++)
		for (k = 0; k < p; k++)
			for (i = 0; i < m; i++)
				for (j = 0; j < n; j++)
					C[n*i + j] += A[p*i + k] * B[n*k + j];
	return;
}

void ALGO3(double *A, double *B, double *C, int m, int n, int p) {
	int i, j, k, r;
	i = j = k =  r = 0;
	for (r = 0; r < LOOP_COUNT; r++)
		for (i = 0; i < m; i++)
			for (k = 0; k < p; k++)
				for (j = 0; j < n; j++)
					C[n*i + j] += A[p*i + k] * B[n*k + j];
	return;
}

void ALGO3_1(double *A, double *B, double *C, int m, int n, int p) {
	int i, j, k, r;
	i = j = k = r = 0;
	for (r = 0; r < LOOP_COUNT; r++)
		for (j = 0; j < m; j++)
			for (k = 0; k < p; k++)
				for (i = 0; i < n; i++)
					C[n*i + j] += A[p*i + k] * B[n*k + j];
	return;
}

void ALGO4(double *A, double *B, double *C, int m, int n, int p, double alpha, double beta) {
	int i, j, k, r;
	i = j = k = r = 0;
	for(r = 0; r < LOOP_COUNT; r++)
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, p, alpha, A, p, B, n, beta, C, n);
	return;
}

double CallCalculatingFunction(int algoNum, double *A, double *B, double **C, int m, int n, int p) {

	//시간 측정 정확도 향상 위해 기본 알고리즘 1번 실행
	double *C_temp = (double *)mkl_malloc(m*n * sizeof(double), 64);
	for (int i = 0; i < m*n; i++)
		C_temp[i] = 0;
	ALGO1(A, B, C_temp, m, n, p);

	//실제 측정 (LOOP_COUNT번 수행 후 수행 시간 평균 구함)
	double s_initial = dsecnd();
	switch (algoNum) {
	case 1:
		ALGO1(A, B, C[0], m, n, p);
		break;
	case 2:
		ALGO2(A, B, C[1], m, n, p);
		break;
	case 3:
		ALGO3(A, B, C[2], m, n, p);
		break;
	case 4:
		ALGO4(A, B, C[3], m, n, p, 1.0, 0.0);
		break;
	case -1:
		ALGO3_1(A, B, C[2], m, n, p);
		break;
	}
	double s_elapsed = (dsecnd() - s_initial) / LOOP_COUNT;

	if (PRINTARRAY) {
		printArray(C[0], m, n);
	}

	mkl_free(C_temp);

	return s_elapsed * 1000;
}

void printArray(double *array, int m, int n) {
	double temp;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			temp = array[i * n + j];
			if (temp >= 0)
				putchar('+');
			printf("%.3lf ", temp);
		}
		putchar('\n');
	}
	putchar('\n');
	return;
}

int MatrixMultiplication(double *A, double *B, int m, int n, int p, bool includeALGO4, double *timeArray) {
	int i, j, k, r;
	double sum;
	double s_initial, s_elapsed;

	//예외 처리
	if (A == NULL || B == NULL) {
		printf("\n ERROR: Can't allocate memory for matrices. Aborting... \n\n");
		mkl_free(A);
		mkl_free(B);
		return -1;
	}

	int cSize = 3;
	if (includeALGO4)
		cSize++;
	double **C = (double **)malloc(sizeof(double *) * cSize);

	for (int i = 0; i < cSize; i++) {
		C[i] = (double *)mkl_malloc(m*n * sizeof(double), 64);
		for (j = 0; j < (m * n); j++)
			C[i][j] = 0.0;
	}

	//실제 계산 함수 호출 후 측정 시간 저장
	for(int i = 0; i < cSize; i++)
		timeArray[i] = CallCalculatingFunction(i + 1, A, B, C, m, n, p);


	//저장된 측정 시간 출력
	printf("---------------------------------------------------------------------------------\n");
	printf("n         |                               %5d                                    \n", n);
	printf("Algorithm |  ALGO_1         |  ALGO_2         | ALGO_3         | ALGO_4         |\n");
	printf("Time(ms)  |  ");
	for (i = 0; i < cSize; i++) {
		printf("%-14.3lf | ", timeArray[i]);
		if (i == 0)
			putchar(' ');
	}
	putchar('\n');

	//Free Array
	for (i = 0; i < cSize; i++)
		mkl_free(C[i]);
	free(C);

	return 0;
}