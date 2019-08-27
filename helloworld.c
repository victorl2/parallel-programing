#include <stdio.h>
#include <omp.h>

/*
 * Exibe o total de threads presentes na aplicação
 * utilizando OpenMP.
 * 
 * Para compilar utilize: 
 * export OMP_NUM THREADS=3
 * gcc -o helloworld.c hello -fopenmp
 */
void main(){
    #pragma omp parallel
    {
        int np = omp_get_num_threads();
        int iam = omp_get_thread_num();
        printf("Hello from thread(%d) from (total of %d)\n", iam,np);
    }
}