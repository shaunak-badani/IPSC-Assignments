#include <bits/stdc++.h>
#include <mpi.h>
#define ROOM_SIZE 20
#define ITERATION_LIMIT 100

using namespace std;


int main(int argc, char* argv[]) {
	MPI_Init(NULL, NULL);
	int no_of_processes, rank;
	int root = 0;

	MPI_Comm_size(MPI_COMM_WORLD, &no_of_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	float local_block[(ROOM_SIZE + no_of_processes - 1)/no_of_processes][ROOM_SIZE];
	int no_of_rows;
	float H[ROOM_SIZE][ROOM_SIZE];	

	// Initialization
	for(int i = 0 ; i < ROOM_SIZE ; i++)
		for(int j = 0 ; j < ROOM_SIZE ; j++)
			H[i][j] = 0;
	for(int i = 0 ; i < ROOM_SIZE ; i++) {
		H[i][0] = 20;				// Left Wall
		H[i][ROOM_SIZE - 1] = 20; 	// Right Wall
		H[0][i] = 20; 				// Top Wall
		H[ROOM_SIZE - 1][i] = 20;	// Bottom Wall
	}
	for(int i = (3 * ROOM_SIZE / 10); i < 7 * ROOM_SIZE / 10 ; i++) {
		H[0][i] = 100; 				// Fireplace Temperature = 100 Degree Celsius
	}

	int row_indices[no_of_processes]; // No of processes received by each of them	

	if(rank == root) {
		
		int rows_per_indices = ROOM_SIZE / no_of_processes;
		int rem_rows = ROOM_SIZE % no_of_processes;
		for(int i = 0 ; i < no_of_processes ; i++)
			row_indices[i] = rows_per_indices;
		for(int i = 0 ; rem_rows > 0 ; i++, rem_rows--)
			row_indices[i]++;
	}
	MPI_Bcast(&row_indices[0], no_of_processes, MPI_INT, root, MPI_COMM_WORLD);
	// printf("rank %d row_indices %d\n", rank, row_indices[rank]);
	for(int it = 0 ; it < ITERATION_LIMIT ; it++) {


	int start_indices;
	int start_point = 0;
	int end_point = 0;
	for(int i = 0; i < rank ; i++)
		start_point += row_indices[i];
	start_indices = start_point;
	if(rank == root) 
		start_point = 1;
	end_point = start_point + row_indices[rank];
	if(rank == root || rank == no_of_processes - 1)
		end_point -= 1;

	int g[ROOM_SIZE][ROOM_SIZE];
	for(int i = start_point; i < end_point ; i++){
		for(int j = 1 ; j < ROOM_SIZE - 1 ; j++)
			g[i][j] = 0.25 * (H[i - 1][j] + H[i + 1][j] + H[i][j - 1] + H[i][j + 1]);
	}
	for(int i = start_point ; i < end_point ; i++){
		for(int j = 1 ; j < ROOM_SIZE - 1; j++)
			H[i][j] = g[i][j];
	}

	int recv_count[no_of_processes], displs[no_of_processes];
	for(int i = 0 ; i < no_of_processes ; i++)
		recv_count[i] = row_indices[i] * ROOM_SIZE;

	displs[0] = 0;
	for(int i = 1 ; i < no_of_processes ; i++)
		displs[i] = displs[i - 1] + row_indices[i - 1] * ROOM_SIZE;
	// if(rank == root) {
	// for(int i = 0 ; i < no_of_processes ; i++) {
	// 	printf("displs : %d recv_count %d\n", displs[i], recv_count[i]);
	// } 
	// }
	// MPI_Allgather(&H[start_point][0], 2 * ROOM_SIZE, MPI_FLOAT, &H[0][0], 2 * no_of_processes * ROOM_SIZE, MPI_FLOAT, MPI_COMM_WORLD);
	int send_count = (row_indices[rank]) * ROOM_SIZE;
	// printf("")
	// for(int i = 0 ; i < no_of_processes ; i++)
	MPI_Allgatherv(&H[start_indices][0], send_count, MPI_FLOAT, &H[0][0], recv_count, displs, MPI_FLOAT, MPI_COMM_WORLD);
	}

	if(rank == root) {
		for(int i = 0 ; i < ROOM_SIZE ; i++){
			for(int j = 0 ; j < ROOM_SIZE; j++)
				printf("%.0f ", H[i][j]);
			printf("\n");
		}
	}



	MPI_Finalize();	
}