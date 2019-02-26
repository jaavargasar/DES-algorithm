#include<stdio.h>
#include <stdlib.h>

FILE* file;
int dt;
int N=4;


void readMatrix(int  **matrix){
     for(int i=0;i<N;i++){
         matrix[i] = malloc(N * sizeof(int));
        for(int j=0;j<N;j++){
            fscanf(file,"%d", &matrix[i][j]);
        } 
    }
}

void printMatrix(int **matrix){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%d ",matrix[i][j]);
        }
        printf("\n");   
    }
    printf("\n"); 
}

void  multiplyMatrices(int **ma, int **mb,int **mc){

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
           for(int k=0;k<N;k++){
                mc[i][j] += ( ma[i][k] * mb[k][j]);
           }
        }
    }
}

void main(){

    file = fopen ("test", "r");
    int **mta = (int **)malloc(N*sizeof(int*));
    int **mtb = (int **)malloc(N*sizeof(int*));
    int **mc =  (int **)malloc(N*sizeof(int*));
    for(int i=0;i<N;i++)
        mc[i] = malloc(N * sizeof(int));

    readMatrix(mta); readMatrix(mtb);
    multiplyMatrices(mta,mtb,mc);
    printMatrix(mc);
    fclose(file);

}
