#include<stdio.h>
#include <stdlib.h>
#include "omp.h"
#include <time.h>

FILE* file;
int **mta , **mtb;
int N;
int nthreads;


void readMatrix(int  **matrix){
     for(int i=0;i<N;i++){
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


void iniMatrices(int **mc){
    mta = (int **)malloc(N*sizeof(int*));
    mtb = (int **)malloc(N*sizeof(int*));
    for(int i=0;i<N;i++){
        mc[i] = malloc(N * sizeof(int));
        mta[i] = malloc(N * sizeof(int));
        mtb[i] = malloc(N * sizeof(int));
    }

    // for(int i=0;i<N;i++)
    //     for(int j=0;j<N;j++)
    //         mc[i][j]=0; 
}


int getRandNumber(int nmin, int nmax){ 
    return rand() % (nmax - nmin +1 ) + nmin; 
}

void generateMatrix(int **matrix,int **mc){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            mc[i][j]=0;
            matrix[i][j] = getRandNumber(-100,100);
        } 
    }
}

void  multiplyMatrices(int ini,int end,int **mc){

    for(int i=ini;i<end;i++){
        for(int j=0;j<N;j++){
           for(int k=0;k<N;k++){
                mc[i][j] += ( mta[i][k] * mtb[k][j]);
           }
        }
    }
}

void resetMatrix(int **matrix){
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            matrix[i][j]=0;
    
    printMatrix(matrix);
}

void secuential(int **matrix){
    multiplyMatrices(0,N,matrix);
    //printMatrix(matrix);
}

void parallel(int **matrix){

    int chunkSize = N /nthreads; 
    #pragma omp parallel num_threads( nthreads )
    {
        int ithread = omp_get_thread_num();
        multiplyMatrices(ithread*chunkSize,(ithread+1)*chunkSize,matrix);
    }
    
    multiplyMatrices(N-(N%nthreads),N,matrix); 
    //printMatrix(matrix);
}

void freeSize(int **mc){
    for(int i=0;i<N;i++){
        free(mta[i]); free(mtb[i]); free(mc[i]);
    }
    free(mta);free(mtb);free(mc);
}


int main(int argc, char *argv[]){

    // file = fopen ("test", "r"); fclose(file);

    
    sscanf (argv[1], "%i", &N);
    printf("N: %d\n",N);

    srand(time(0));
    int **mc = (int **)malloc(N*sizeof(int*));
    iniMatrices(mc);
    generateMatrix(mta,mc); generateMatrix(mtb,mc);
   
    
    secuential(mc);
    //parallel(mc);
    freeSize(mc);
    return 0;   

}
