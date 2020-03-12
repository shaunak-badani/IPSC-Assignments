#include "omp.h"
#include <stdio.h>

#define THREADS 7
#define ROOM_SIZE 10
#define ITERATION_LIMIT 100

void main() {
	omp_set_num_threads(THREADS);
	float H[ROOM_SIZE][ROOM_SIZE];

	// Initialization
	for(int i = 0 ; i < ROOM_SIZE ; i++)
		for(int j = 0 ; j < ROOM_SIZE ; j++)
			H[i][j] = 0;
	for(int i = 0 ; i < ROOM_SIZE ; i++) {
		H[i][0] = 20;				
		H[i][ROOM_SIZE - 1] = 20; 	
		H[0][i] = 20; 				
		H[ROOM_SIZE - 1][i] = 20;	
	}
	for(int i = (3 * ROOM_SIZE / 10); i < 7 * ROOM_SIZE / 10 ; i++) {
		H[0][i] = 100; 				// Fireplace Temperature = 100 Degree Celsius
	}

	int row_indices[THREADS];
	int rows_per_indices = ROOM_SIZE / THREADS;
	int rem_rows = ROOM_SIZE % THREADS;

	for(int i = 0 ; i < THREADS ; i++)
		row_indices[i] = rows_per_indices;
	for(int i = 0 ; rem_rows > 0 ; i++, rem_rows--)
		row_indices[i]++;
	int start_point[THREADS], end_point[THREADS];
	start_point[0] = 0;
	end_point[0] = row_indices[0];
	for(int i = 1 ; i < THREADS; i++) {
		start_point[i] = start_point[i-1] + row_indices[i-1];
		end_point[i] = start_point[i] + row_indices[i];
	}
	start_point[0] += 1;
	end_point[THREADS - 1] -= 1;
	float g[ROOM_SIZE][ROOM_SIZE];
	double end, start;

	start = omp_get_wtime();

	#pragma omp parallel shared(g, H)
	{
		int ID = 0;
		int rank = omp_get_thread_num();
		for(int it = 0; it < ITERATION_LIMIT ; it++ ) {
			for(int i = start_point[rank]; i < end_point[rank] ; i++) {
				for(int j = 1 ; j < ROOM_SIZE - 1; j++)
					g[i][j] = 0.25 * (H[i-1][j] + H[i+1][j] + H[i][j-1] + H[i][j+1]);
			}

			#pragma omp barrier
			for(int i = start_point[rank]; i < end_point[rank] ; i++) {
				for(int j = 1 ; j < ROOM_SIZE - 1; j++)
					H[i][j] = g[i][j];
			}
			#pragma omp barrier
		}
	}
	end = omp_get_wtime();	
	printf("Time taken : %lf\n", end - start);
	// for(int i = 0 ; i < ROOM_SIZE ; i++) {
	// 	for(int j = 0 ; j < ROOM_SIZE ; j++)
	// 		printf("%.0f ", H[i][j]);
	// 	printf("\n");
	// }

}