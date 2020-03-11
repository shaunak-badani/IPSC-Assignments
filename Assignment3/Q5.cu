#include <cuda_runtime.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define THREADS 64
#define MAX_VALUE 10000
#define ll long long
#define D_SIZE 1000

__global__ void sum_array(ll *a_1, ll *a_2, ll *a_3, ll *a_4, ll *b_sum, ll *b_sq,ll *b_max, ll *b_min, ll N) {
	int index = threadIdx.x + blockIdx.x * THREADS;
	int local_index = threadIdx.x;

	// MAX ELEMENT COMPUTATION
	for(unsigned int width = THREADS / 2; width > 0; width /= 2) {
		if(local_index < width && index + width < N) {
			a_1[index] = max(a_1[index], a_1[index + width]);
		}
		__syncthreads();
	}	
	if(threadIdx.x == 0) b_max[blockIdx.x] = a_1[index];

	// MIN ELEMENT COMPUTATION
	for(unsigned int width = THREADS / 2; width > 0; width /= 2) {
		if(local_index < width && index + width < N) {
			a_2[index] = min(a_2[index], a_2[index + width]);
		}
		__syncthreads();
	}
	if(threadIdx.x == 0) b_min[blockIdx.x] = a_2[index];

	// SUM ELEMENT COMPUTATION
	for(unsigned int width = THREADS / 2; width > 0; width /= 2) {
		if(local_index < width && index + width < N) {
			a_3[index] += a_3[index + width];
		}
		__syncthreads();
	}
	if(threadIdx.x == 0) b_sum[blockIdx.x] = a_3[index];

	// STANDARD DEVIATION
	for(unsigned int width = THREADS / 2; width > 0; width /= 2) {
		if(local_index < width && index + width < N) {
			a_4[index] += a_4[index + width];
		}
		__syncthreads();
	}	
	if(threadIdx.x == 0) b_sq[blockIdx.x] = a_4[index];
}

int main(int argc, char* argv[]) {
	ll *a, *a_sq, *h_sq, *h_sum, *h_max, *h_min; 		
	ll *d_a, *d_a2, *d_a3, *d_a4, *d_sq, *d_sum, *d_max, *d_min; 
	ll N;
	struct timeval t1, t2;


	if(argc <= 1) {
		printf("No argument provided. Taking N = 1e8 \n");
		N = 100000000;
	}
	else {
		int val = (int)atoi(argv[1]);
		if(val == 0) {
			printf("Couldn't interpret N. Taking N = 1e8 \n");
			N = 100000000;
		}
		else
			N = (ll)val;
	}
	ll BLOCKS = (N + THREADS - 1)/THREADS;
	ll array_size = N * sizeof(ll);

	ll blocksize = sizeof(ll) * BLOCKS;

	cudaMalloc((void **) &d_a, array_size);
	cudaMalloc((void **) &d_a2, array_size);
	cudaMalloc((void **) &d_a3, array_size);
	cudaMalloc((void **) &d_a4, array_size);
	cudaMalloc((void **) &d_sum, blocksize);
	cudaMalloc((void **) &d_max, blocksize);
	cudaMalloc((void **) &d_min, blocksize);
	cudaMalloc((void **) &d_sq, blocksize);

	a = (ll *)malloc(array_size);
	a_sq = (ll *)malloc(array_size);
	h_sq = (ll *)malloc(blocksize);
	h_max = (ll *)malloc(blocksize);
	h_min = (ll *)malloc(blocksize);
	h_sum = (ll *)malloc(blocksize);

	for(int i = 0 ; i < N ; i++) {
		a[i] = rand() % MAX_VALUE;
		a_sq[i] = a[i] * a[i];
	}

	// Copy inputs to GPU
	cudaMemcpy(d_a, a, N * sizeof(ll), cudaMemcpyHostToDevice);
	cudaMemcpy(d_a2, a, N * sizeof(ll), cudaMemcpyHostToDevice);
	cudaMemcpy(d_a3, a, N * sizeof(ll), cudaMemcpyHostToDevice);
	cudaMemcpy(d_a4, a_sq, N * sizeof(ll), cudaMemcpyHostToDevice);

	gettimeofday(&t1, 0);

	sum_array<<<BLOCKS,THREADS>>>(d_a, d_a2, d_a3, d_a4, d_sum, d_sq, d_max, d_min, N);

	cudaDeviceSynchronize();
	gettimeofday(&t2, 0);
	double time1 = (t2.tv_usec-t1.tv_usec);
	printf("Time for GPU:  %.8f us \n", time1);


	// Copy result back to host
	cudaMemcpy(h_sq, d_sq, blocksize, cudaMemcpyDeviceToHost);
	cudaMemcpy(h_max, d_max, blocksize, cudaMemcpyDeviceToHost);
	cudaMemcpy(h_min, d_min, blocksize, cudaMemcpyDeviceToHost);
	cudaMemcpy(h_sum, d_sum, blocksize, cudaMemcpyDeviceToHost);

	ll max_arr, min_arr;
	double mean, std_dev;
	mean = 0;
	max_arr = -1e9;
	min_arr = 1e9;
	std_dev = 0;
	for(int i = 0 ; i < BLOCKS ; i++) {
		mean += h_sum[i];
		max_arr = max(max_arr, h_max[i]);
		min_arr = min(min_arr, h_min[i]);
		std_dev += h_sq[i];
	}
	mean /= N;
	std_dev = (std_dev / N) - mean * mean;
	printf("Parallel : \n");
	printf("Max : %lld, Min : %lld, Mean : %lf, Standard Deviation : %lf\n", max_arr, min_arr, mean, std_dev);
	printf("\n");

	ll max_s, min_s;
	double mean_s, std_dev_s;
	max_s = -1e9;
	min_s = 1e9;
	mean_s = std_dev_s = 0;


	// Serial Implementation

	gettimeofday(&t1, 0);
	for(int i = 0 ; i < N ; i++) {
		mean_s += a[i];
		std_dev_s += a[i] * a[i];
		max_s = max(max_s, a[i]);
		min_s = min(min_s, a[i]);
	}
	mean_s /= N;
	std_dev_s = (std_dev_s / N) - mean * mean;

	gettimeofday(&t2, 0);
	double time2 = (t2.tv_usec-t1.tv_usec);
	printf("Time for CPU:  %.8f us \n", time2);

	printf("Serial : \n");
	printf("Max : %lld, Min : %lld, Mean : %lf, Standard Deviation : %lf\n", max_s, min_s, mean_s, std_dev_s);
	printf("\n");

	printf("Speedup = %f\n", time2/time1);

	// End Serial Implementation

	// Cleanup
	free(a);
	free(a_sq);
	free(h_sq); 
	free(h_max); 
	free(h_min); 
	free(h_sum); 

	// Cuda free
	cudaFree(d_a);
	cudaFree(d_a2);
	cudaFree(d_a3);
	cudaFree(d_a4);
	cudaFree(d_sq);
	cudaFree(d_max);
	cudaFree(d_min);
	cudaFree(d_sum);

	return 0;
}