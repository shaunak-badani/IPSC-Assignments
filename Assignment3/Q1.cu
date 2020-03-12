// #include <cuda_runtime.h>
#include <sys/time.h>

#include <stdio.h>
#include <string.h>

#define THREADS 10
#define ROOM_SIZE 10
#define BLOCKS (ROOM_SIZE * ROOM_SIZE + THREADS - 1) / THREADS
#define ITERATION_LIMIT 100

__global__ void simulate_room(float *H) {
	int index = threadIdx.x + blockIdx.x * THREADS;

	int j = index % ROOM_SIZE;
	int i = index / ROOM_SIZE;


	float tmp = H[index];
	for(int it = 0 ; it < ITERATION_LIMIT ; it++) {
		if(i > 0 && i < ROOM_SIZE - 1 && j > 0 && j < ROOM_SIZE - 1)
			tmp = 0.25 * (H[(i - 1) * ROOM_SIZE + j] + H[(i + 1) * ROOM_SIZE + j] + H[i * ROOM_SIZE + j + 1] + H[i * ROOM_SIZE + j - 1]);

		__syncthreads();

		H[index] = tmp;

		__syncthreads();
	}
}

int main(int argc, char* argv[]) {
	float *h_H, *d_H;

	h_H = (float *)malloc(sizeof(float) * ROOM_SIZE * ROOM_SIZE);

	for(int i = 0 ; i < ROOM_SIZE ; i++) {
		for(int j = 0 ; j < ROOM_SIZE ; j++)
			h_H[i * ROOM_SIZE + j] = 0;
	}

	for(int i = 0 ; i < ROOM_SIZE ; i++) {
		h_H[i * ROOM_SIZE + 0] = 20;
		h_H[i * ROOM_SIZE + ROOM_SIZE - 1] = 20;
		h_H[0 * ROOM_SIZE + i] = 20;
		h_H[ROOM_SIZE * (ROOM_SIZE - 1) + i] = 20;
	}

	for(int i = 3 * (ROOM_SIZE) / 10 ; i < 7 * ROOM_SIZE / 10 ; i++)
		h_H[i] = 100;

	cudaMalloc((void **) &d_H, sizeof(float) * ROOM_SIZE * ROOM_SIZE);

	// for(int i = 0 ; i < ROOM_SIZE; i++) {
	// 	for(int j = 0 ; j < ROOM_SIZE ; j++)
	// 		printf("%.0f ", h_H[i][j]);
	// 	printf("\n");
	// }

	cudaMemcpy(d_H, h_H, sizeof(float) * ROOM_SIZE * ROOM_SIZE, cudaMemcpyHostToDevice);
	printf("THREADS %d BLOCKS %d\n", THREADS, BLOCKS);

	struct timeval t1, t2;
	gettimeofday(&t1, 0);

	simulate_room<<<BLOCKS,THREADS>>>(d_H);

	cudaDeviceSynchronize();
	gettimeofday(&t2, 0);
	double time1 = (t2.tv_usec-t1.tv_usec);
	printf("Time for GPU:  %.8f us \n", time1);
	cudaMemcpy(h_H, d_H, sizeof(float) * ROOM_SIZE * ROOM_SIZE, cudaMemcpyDeviceToHost);

	for(int i = 0 ; i < ROOM_SIZE; i++) {
		for(int j = 0 ; j < ROOM_SIZE ; j++)
			printf("%.0f ", h_H[i * ROOM_SIZE + j]);
		printf("\n");
	}


	// for(int i = 0 ; i < ROOM_SIZE ; i++)

}