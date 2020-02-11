#include <stdio.h>
#include <stdlib.h>

#define n 1000

int main() {
	int i, j, k;
	double *A[n], *B[n], *C[n], *BT[n];
	for (i = 0; i < n; i++){
		A[i] = (double *) malloc(n * sizeof(double));
		B[i] = (double *) malloc(n * sizeof(double));
		BT[i] = (double *) malloc(n * sizeof(double));
		C[i] =(double *) malloc(n * sizeof(double));
	}

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			A[i][j] = 1.0;
			B[i][j] = 1.0;
			C[i][j] = 1.0;
		}

	for(int i = 0; i < n ; i++) {
		for(int j = 0 ; j < n ; j++)
			BT[i][j] = B[j][i];
	}
	for (i = 0; i < n; i++)
		for (j = 0; j < n ; j++)
			for (k = 0; k < n ; k++)
				C[i][j] += A[i][k] * BT[j][k];
	return 0;
}