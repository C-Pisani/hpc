#include <stdio.h>
#include <omp.h>

int main(){
    omp_set_num_threads(2);
    #pragma omp parallel
    {
        # pragma omp for
        for (int i = 0; i < 4; i++){
            int tid = omp_get_thread_num();
            printf("Hello from thread %d\n", tid);
        }
    }

    
}

//gcc main.c -o main -fopenmp