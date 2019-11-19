#include "MatrixMultiflication.h"

int main(void) {
	int m, n, p, Count = 11;
	bool includeALGO4 = 1;
	//���� �ð� ������ �迭
	double **timeArray = (double **)malloc(sizeof(double *) * Count);

	//n �� �Է�
	int *nArray = (int *)malloc(sizeof(int) * Count);
	nArray[0] = 10;
	for (int i = 1; i < Count; i++) {
		nArray[i] = i * 1000;
	}

	for (int c = 0; c < Count; c++) {
		m = n = p = nArray[c];
		//-1~1���� ���� �Ǽ� ����
		srand((unsigned int)time(NULL));
		double *A = (double *)malloc(sizeof(double) * m * p);
		for (int i = 0; i < m * n; i++)
			A[i] = (rand() % 2001) / (double)1000 - (double)1;

		double *B = (double *)malloc(sizeof(double) * p * n);
		for (int i = 0; i < m * n; i++)
			B[i] = (rand() % 2001) / (double)1000 - (double)1;

		//���� �ð� ������ �迭 ����
		timeArray[c] = (double *)malloc(sizeof(double) * 4);

		//��� ���� �Լ� ȣ��
		MatrixMultiplication(A, B, m, n, p, includeALGO4, timeArray[c]);

		free(A);
		free(B);
	}

	//���� �Ҵ� ����
	for (int i = 0; i < Count; i++)
		free(timeArray[i]);
	free(timeArray);
	free(nArray);

	return 0;
}
