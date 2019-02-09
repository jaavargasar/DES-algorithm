#include <iostream>
#include <stdio.h>
#include <stdlib.h>

const int PC1[56] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
 };


 __host__ __device__ void printSomething(int *d_PC1,int number){
     printf("printing something new \n");
     printf("%i ------ %i\n",d_PC1[10],5);
 }

 __global__ void doingSomething(int *d_PC1){
    printf("so then what\n");
    printf("%i\n",d_PC1[1]);
    printSomething(d_PC1,5);
 }

int main() {
    //printSomething();
    printf("hello world\n");
    printf("%i\n",PC1[0]);

    //host and device copies
    int *d_PC1;

    //size
    int sizePC1 = 56 * sizeof( int );

    //alloc space in cuda
    cudaMalloc( (void **)&d_PC1, sizePC1 );

    //copy input to device
    cudaMemcpy(d_PC1,PC1,sizePC1,cudaMemcpyHostToDevice);

    //launch kernel on GPU
    doingSomething<<<1,1>>>(d_PC1);

    //free space
    cudaFree(d_PC1);
    return 0;
}