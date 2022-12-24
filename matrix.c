#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include<omp.h>

FILE *file1;
FILE *file2;

struct matrix{
    int rows;
    int columns;
    double **data;
    //double *dataR;
    //double *dataC;
};

double **results;

struct matrix *matrix1;
struct matrix *matrix2;

struct range{
    int thread;
    int start;
    int end;
};

struct range *ranges;

FILE* openFile(char *filename){
    
    
    FILE *oFile = fopen(filename, "r");
        
    return oFile;

}

void populateMatrix(FILE *toOpen, struct matrix *theMatrix){
    int iRows = 0;
    int iColumns;
    double thisRow;

    char linesize[500];  
    char *part; 
    char *term;   
    char delim[2] = ",";
    double **temp;
        
    rewind(toOpen);
    
    while (fgets(linesize, sizeof(linesize), toOpen)) {
        iColumns = 0;
        //Tokenize the sting into individual sections (comma seperated numbers in files)
        part = strtok(linesize,delim);

        /* walk through other tokens */
        while( part != NULL ) {
            //Convert the string part to a double variable
            thisRow = strtod(part, &term);
            theMatrix->data[iRows][iColumns] = thisRow;            
                                    
            
            part = strtok(NULL, delim);
            if (part != NULL){
                iColumns++;
            }
        }
        //printf("Row contains %d columns\n", iRows);
        
        if (feof(toOpen)){
            break;
        }
        else{
            iRows++;
        }

    }
}

void readFile(FILE *toOpen, struct matrix *theMatrix){
    int iRows = 1;
    int iColumns;
    int counter = 0;
    int passes = 0;

    char linesize[500];  
    char *part; 
    char delim[2] = ",";
        
    while (fgets(linesize, sizeof(linesize), toOpen)) {
        iColumns = 0;
        //Tokenize the sting into individual sections (comma seperated numbers in files)
            part = strtok(linesize,delim);
        if (passes == 0){
            /* walk through other tokens */
            while( part != NULL ) {
                
                            
                counter++;
                part = strtok(NULL, delim);
                //if (part!= NULL){
                    iColumns++;                        
                    theMatrix->columns = iColumns;
                //}
            }
            //printf("Row contains %d columns\n", iRows);
        }    
        passes = 1;

        if (feof(toOpen)){
            break;
        }
        else{
            iRows++;
            theMatrix->rows = iRows;            
        }                                        
    }

    

    printf("Matrix has %d Columns by %d Rows\n", theMatrix->columns, theMatrix->rows);
    //printf("Total data items is %d\n", counter);
    
}

int validation(){
    if (matrix1->columns == matrix2->rows){
        return (matrix1->columns == matrix2->rows);
    }
    else{
        printf("Calculation cannot be performed\n");
        printf("Matrix 1 has %d columns and Matrix 2 has %d Rows", matrix1->columns, matrix2->rows);
        return (matrix1->columns == matrix2->rows);
    }
}

void* calculateMatrix(void* tid){
//void calculateMatrix(int sRow, int eRow, int sColumn, int eCloumn, int tid){    
    int i, j, k;

    int *cthread = (int *) tid; 

    omp_lock_t mlock;

    omp_init_lock(&mlock);

    /*for (i = ranges[*cthread].start; i < ranges[*cthread].end; ++i) {
        for pthreads
    */
    //for (i = 0; i< matrix1->rows; i++){
        i = *cthread;
      for (j = 0; j < matrix2->columns; ++j) {
         for (int k = 0; k < matrix1->columns; ++k) {
            omp_set_lock(&mlock);
            results[i][j] += matrix1->data [i][k] * matrix2->data[k][j];
            omp_unset_lock(&mlock);
         }
      }
    //}   

    int ompid = omp_get_thread_num();

    /*printf("\nOutput Matrix %d:\n", ompid);
    for (int i = 0; i < matrix1->rows; ++i) {
        for (int j = 0; j < matrix2->columns; ++j) {
            if (results[i][j] > 0){
                printf("%lf\t", results[i][j]);
            }                
            if (j == matrix1->rows - 1)
                printf("\n");
        }
    }*/
}

void divide(int rows, int divideBy, struct range *ranges){
    int threads = rows;
    int workers = divideBy;
    int remainder = 0;

    //printf("Parameters passed are %d and %d\n", threads, divideBy);

    //printf("Number of Threads is %d \n", threads);
    //printf("Number of workers is %d \n", workers);

    remainder = threads % workers;

    //printf("remainder is %d \n", remainder);

    int workThread = (threads-remainder)/workers;
    int start = 0;
    int end = 0;

    for (int i = 0; i < workers; i++){
        ranges[i].thread = i;
        if(remainder > 0){
            //printf("Worker %d has %d \n",i,  workThread+1);
            ranges[i].start = start;
            if (start==0){
                end = end + (workThread);
            }
            else{
                end = end + (workThread+1);
            }
            //end = end + (workThread);
            ranges[i].end = end;
            remainder--;
            //printf("Start is %d and end is %d \n", start, end);
            start = end + 1;
            
        }
        else
        {
            //printf("Worker %d has %d \n", i, workThread);
            ranges[i].start = start;
            if (start==0){
                end = end + (workThread-1);
            }
            else{
                end = end + (workThread);
            }
            
            ranges[i].end = end;
            //printf("Start is %d and end is %d \n", start, end); 
            start = end + 1;
            
        }

    }
}

int main(int argc, char *argv[]){

    if (argc < 4){
        printf("Please enter the following patameters\n");
        printf("Number of threads, File 1 and file 2\n");
        return 0;
    }
    else{
        //char fileName1[] = "Mat1.txt";
        matrix1 = (struct matrix *) calloc(1, sizeof(struct matrix));        
        
        file1 = openFile(argv[2]);

        if(file1 != NULL){
            readFile(file1, matrix1);
        }
        else{
            printf("File Opening procedure failed. Exiting now....\n");
            return 0;
        }
                

        matrix2 = (struct matrix *) calloc(1, sizeof(struct matrix));

        file2 = openFile(argv[3]);

        if (file2 != NULL){
            readFile(file2, matrix2);
        } 
        else{
            printf("File Opening procedure failed. Exiting now....\n");
            return 0;
        }        

        int validated = validation();
        printf("Validation value is %d\n", validated);

        results = calloc(matrix1->rows, sizeof(double *));
        for (int i = 0; i < matrix1->rows; i++){
            results[i] = calloc(matrix2->columns, sizeof(double));
        }

        matrix1->data = calloc(matrix1->rows, sizeof(double));
        for (int i = 0; i < matrix1->rows; i++){
            matrix1->data[i] = calloc(matrix1->columns, sizeof(double));
        }

        matrix2->data = calloc(matrix2->rows, sizeof(double));
        for (int i = 0; i < matrix2->rows; i++){
            matrix2->data[i] = calloc(matrix2->columns, sizeof(double));
        }   

        populateMatrix(file1, matrix1);
        populateMatrix(file2, matrix2);

        /*int counter = 0;
        for (int i = 0; i < matrix1->rows; i++)
        {
            for (int j = 0; j <= matrix1->columns;j++){
                printf("%lf, \t", matrix1->data[i][j]);
                counter++;
            }
            printf("\n");
            
        }*/

        int threads = atoi(argv[1]);

        ranges = (struct range *) calloc(threads, sizeof(struct range));

        divide(matrix1->rows, threads, ranges);

        omp_set_num_threads(threads);
        #pragma omp parallel
        {
            #pragma omp for
            for (int i = 0; i < matrix1->rows; i++){
                calculateMatrix(&i);
            }
        }


        /*pthread_t *mthreads = (pthread_t *) calloc(threads, sizeof(pthread_t));
        for(int i = 0; i < threads; i++){
            pthread_create(mthreads + i, NULL, calculateMatrix, &i);
        }*/

        /*for(int i = 0; i < threads; i++){
            calculateMatrix(&i);
        }*/
        
        //calculateMatrix(0, 1, 0, 1);

        printf("\nOutput Matrix:\n");
        for (int i = 0; i < matrix1->rows; ++i) {
            for (int j = 0; j < matrix2->columns; ++j) {
                if (results[i][j] > 0){
                    printf("%lf\t", results[i][j]);
                }                
                if (j == matrix1->rows - 1)
                    printf("\n");
            }
        }

        if(file1!= NULL){
            fclose(file1);
        }
        if(file2 != NULL){
            fclose(file2);
        }
        
        if (matrix1 != NULL){
            free(matrix1);
        }
        
        if (matrix2 != NULL){
            free(matrix2);
        }
        
        if (results != NULL){
            free(results);
        }

        if(ranges != NULL){
            free(ranges);
        }
        
    }
}

//gcc matrices.c -o matrices
// ./matrices 2 Mat1.txt Mat2.txt