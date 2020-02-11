#include <stdio.h>
#include <stdlib.h>
#define min(x, y) ((x < y) ? x : y)
#define n 1000
#define SM (CLS/sizeof(double))

int main() {
	int i, j, k;
	double *A[n], *B[n], *C[n];
	for (i = 0; i < n; i++){
		A[i] = (double *) malloc(n * sizeof(double));
		B[i] = (double *) malloc(n * sizeof(double));
		C[i] =(double *) malloc(n * sizeof(double));
	}

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			A[i][j] = 1.0;
			B[i][j] = 1.0;
			C[i][j] = 0.0;
		}
	int jj, kk;
	double sum = 0.0;
	for (jj = 0; jj < n; jj += SM)
		for (kk = 0; kk < n; kk += SM)
			for (i=0; i < n; i++){
				for (j = jj; j < min(jj + SM, n); j++){
					sum = 0.0;
					for (k = kk; k < min(kk + SM,n); k++){
						sum += A[i][k] * B[k][j];
					}
					C[i][j] += sum;
				}
			}

	return 0;
}