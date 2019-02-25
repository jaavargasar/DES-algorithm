/* pi.cl */
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

typedef unsigned long long ull;

__kernel void cipherDES(
    __global ull *d_LnRnBlocks,
    __global ull *d_CnDnBlocks,
    __global ull *d_keysBlocks,
    __global ull *d_Rotations,
    __global int *d_PC1,
    __global int *d_PC2,
    __global int *d_IniPer,
    __global int *d_reverseIniPer,
    __global int *d_Expansion,
    __global int *d_Pbox,
    __global int *d_Sbox,
    __global ull *d_iniKey,
    __global ull *d_message,
    __global ull *d_result)
{   

    int id = get_global_id(0);
    int numthreads = get_global_size(0);
    printf("id: %i - thtreads: %i - other: %i \n",id,numthreads,d_Pbox[0]);
}
