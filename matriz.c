#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mpi.h"
#include <string.h>

#define SIZE 50
int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];

int main(int argc, char** argv){
    int master = 0;
    int meu_rank, np, origem, destino, tag=0;
    char msg[100];
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&meu_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&np);


	int i,j,k,N;
	N = SIZE;
	// Inicializa Matriz com 1
	for(i = 0 ; i < SIZE; i++){
		for(j = 0; j < SIZE; j++) {
			A[i][j] = 1;
			B[i][j] = 1;
			C[i][j] = 1;
		}
	}

	for( j = 0; j < N; j++) {
		C[meu_rank][j] = 0.0;
		for (k = 0; k < N ; k++) {
			C[meu_rank][j] = C[meu_rank][j] + A[meu_rank][k] * B[k][j];
		}
	}

    if(meu_rank != master){
        MPI_Send(C[meu_rank],SIZE,MPI_INT,master,tag,MPI_COMM_WORLD);
    }else{
        int index;
        for(index = 1; index < np; index++) {
            int * TEMP;
            MPI_Recv(TEMP, SIZE, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
            C[status.MPI_SOURCE] = TEMP;
        }
    }

    // Printa a matriz
    if(meu_rank == master){
        printf("-------------- Multiplicacao de matriz --------------\n");
        for(i = 0; i < SIZE; i++) {
            for(j = 0; j < SIZE; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
     }
	
    MPI_Finalize();
    return 0;
}
