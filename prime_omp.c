#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int *myArray;
int *myPrime;
int primeCount = 0;

void isPrime(int number){
    int b;
    int tid = omp_get_thread_num();
    printf("Checking array item %d, value of %d on thread %d\n", number, myArray[number], tid);
    for(b = 2; b <= myArray[number]; b++){
        if(myArray[number] % b == 0){
            break;
        }
    }

    //int *isprime = (int *) malloc(sizeof(int));

    if(b == myArray[number]){
        myPrime[number]= myArray[number];
        primeCount++;
    }else{
        myPrime[number] = 0;
    }
    
}

int main(int argc, char* argv[]){


    int size = atoi(argv[3]) - atoi(argv[2]);
    int start = atoi(argv[2]);
    int iT = atoi(argv[1]);
    printf("Array size will be %d\n", size);

    myArray = calloc(size, sizeof(int));
    myPrime = calloc(size, sizeof(int));

    for (int i = 0; i < size; i++){
        myArray[i] = start; 
        start++;
    }

    /*for (int i = 0; i < size; i++){
        printf("Array item %d has a value of %d\n", i, myArray[i]);
        
    }*/

    omp_set_num_threads(iT);
    #pragma omp parallel
    {
        # pragma omp for
        for (int i = 0; i < size; i++){
            isPrime(i);
        }
    }

    for (int i = 0; i < size; i++){
        if (myPrime[i] != 0){
            printf("Number %d is a Prime Number\n", myPrime[i]);
        }
    }
    printf("Total Prime Numbers found is %d\n", primeCount);
}

//gcc prime_omp.c -o prime_omp -fopenmp