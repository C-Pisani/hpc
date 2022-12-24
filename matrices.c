#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

FILE *file1;
FILE *file2;

struct matrix{
    int rows;
    int columns;
    double *data;
    double *dataR;
    double *dataC;
};

double **results;

struct matrix *matrix1;
struct matrix *matrix2;

int openFile(char *filename){
    
    file1 = fopen(filename, "r");
        
    if (file1 == NULL){
        printf("Failed to open file  in File Open procedure\n");
        return 0;
    }
    else{
        return 1;
    }

}

void readFile(FILE *toOpen, struct matrix *theMatrix){
    int iRows = 1;
    int iColumns;
    int counter = 0;
    double thisRow;

    char linesize[500];  
    char *part; 
    char *term;   
    char delim[2] = ",";
    double **temp;
        
    while (fgets(linesize, sizeof(linesize), toOpen)) {
        iColumns = 0;
        //Tokenize the sting into individual sections (comma seperated numbers in files)
        part = strtok(linesize,delim);

        /* walk through other tokens */
        while( part != NULL ) {
            //Add more memory per data item found.
            //the counter variable counts all the items in the Matrix
            theMatrix->data = realloc(theMatrix->data, (counter+1)*sizeof(double));
            //Convert the string part to a double variable
            thisRow = strtod(part, &term);
            
            iColumns++;
            temp[iRows-1] = realloc(results[iRows-1], iColumns*sizeof(double));

            if (temp != NULL){
                results = temp;
            }
            else
            {
                results[iRows-1] = calloc(iColumns,sizeof(double));
            }
            
            theMatrix->columns = iColumns;
            
            theMatrix->data[counter] = thisRow;            
            results[iRows-1][iColumns-1] = thisRow;
            counter++;
            part = strtok(NULL, delim);
        }
        //printf("Row contains %d columns\n", iRows);
        
        if (feof(toOpen)){
            break;
        }
        else{
            results = realloc(results, iRows * sizeof(double *));
            iRows++;
            theMatrix->rows = iRows;            
        }
                    
    }

    printf("Matrix has %d Columns by %d Rows\n", iColumns, iRows);
    printf("Total data items is %d\n", counter);
}

int validation(){
    return (matrix1->columns == matrix2->rows);
}

void calculateMatrix(){
    int row1 = matrix1->rows;
    int row2 = matrix2->rows;
    int col1 = matrix1->columns;
    int col2 = matrix2->columns;
    int counter = 0;
    double answer; 

    for (int i = 0; i < row1; i++){
        for(int j = 0; j < col2; j++){
            //answer +=  
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

        results = calloc(1, sizeof(double *));
        results[0] = calloc(1, sizeof(double));

        if(openFile(argv[2]) == 1){
            readFile(file1, matrix1);
        }
        else{
            printf("File Opening procedure failed. Exiting now....\n");
            return 0;
        }
        
        int counter = 0;
        for (int i = 0; i < matrix1->rows; i++)
        {
            for (int j = 0; j <= matrix1->columns;j++){
                printf("%lf, \t", matrix1->data[counter]);
                counter++;
            }
            printf("\n");
            
        }

        matrix2 = (struct matrix *) calloc(1, sizeof(struct matrix));
        if (openFile(argv[3]) == 1){
            readFile(file1, matrix2);
        } 
        else{
            printf("File Opening procedure failed. Exiting now....\n");
            return 0;
        }
        
        counter = 0;
        for (int i = 0; i < matrix2->rows; i++)
        {
            for (int j = 0; j <= matrix2->columns;j++){
                printf("%lf, \t", matrix2->data[counter]);
                counter++;
            }
            printf("\n");
            
        }

        int validated = validation();
        printf("Validation value is %d\n", validated);

        free(matrix1);
        free(matrix2);
        free(results);
    }
}

//gcc matrices.c -o matrices
// ./matrices 2 Mat1.txt Mat2.txt