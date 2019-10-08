#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mpi.h"
#include <string.h>
#include <omp.h>

#define master 0

/**
 * Função que se deseja aproximar
 */
float f(float x){
    return x*x;
}

/**
 * Método do trapezóide para o cálculo da integral
 */
float calcularIntegral(float inicio, float fim, int divisoes, float passo){
    float integral = (f(inicio) + f(fim)) / 2.0;

    int i = 0;
    float x = inicio , integralAtual = integral;

	#pragma omp parallel shared(a,b,n) private(i,integralAtual)
	{
		#pragma omp for
		for(; i< divisoes; i++){
            x += passo;
            integralAtual += f(x);
        }
    }
    integral = integralAtual;

    return integral * passo;
}


/**
 * mpicc integral-mpi.c -o integral
 * mpirun -np 5 integral
 */
int main(int argc, char** argv){
    int meu_rank;
    int processos;
    int tag = 200; // código para o envio de mensagem

    int partes = 1024; // total de pontos para cálculo do trapezóide
    float inicio = 0.0, fim = 1.0; //intervalo da função
    float passo = ( fim - inicio ) / partes;

    float resultadoFinal; // integral resultante

    int source; // remetente da integral
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &processos);

    int partesLocais = partes / processos;
    float a = inicio + meu_rank * partesLocais * passo;
    float b = a + partesLocais * passo;

    // integral no intervalo atual
    float resultadoParcial = calcularIntegral(a,b,partesLocais,passo);

    MPI_Reduce(&resultadoParcial,&resultadoFinal,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);

    if(meu_rank == 0)
        printf("O total da integral foi: %f\n", resultadoFinal);    
    MPI_Finalize();
}


