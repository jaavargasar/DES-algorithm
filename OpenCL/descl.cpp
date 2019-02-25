/* pi.c
 gcc pi.c -o pi -lOpenCL -I.
 http://pastebin.com/gq3h0KVn
*/

#include <stdio.h>
#include <stdlib.h>
#include <err_code.h>
#include <CL/cl.h>
#include <sys/types.h>

using namespace std;

typedef unsigned long ull;

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)
#define NUMTHREADS  3
#define WORKGROUPS  2
#define ITERATIONS  3 // 2e09


//global variables

ull LnRnBlocks[17*2]; // from l0r0 to l16r16

ull CnDnBlocks[17*2]; //from c0d0 to c16d16

ull keysBlocks[16];  //from key[1] = k0 to key[16] = k15

ull allCipherDES[NUMTHREADS];

ull Rotations[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

int PC1[56] = {
   57, 49, 41, 33, 25, 17,  9,
    1, 58, 50, 42, 34, 26, 18,
   10,  2, 59, 51, 43, 35, 27,
   19, 11,  3, 60, 52, 44, 36,
   63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
   14,  6, 61, 53, 45, 37, 29,
   21, 13,  5, 28, 20, 12,  4
};

int PC2[48] = {
   14, 17, 11, 24,  1,  5,
    3, 28, 15,  6, 21, 10,
   23, 19, 12,  4, 26,  8,
   16,  7, 27, 20, 13,  2,
   41, 52, 31, 37, 47, 55,
   30, 40, 51, 45, 33, 48,
   44, 49, 39, 56, 34, 53,
   46, 42, 50, 36, 29, 32
};

int IniPer[64] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

int reverseIniPer[64] ={
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25
};

int Expansion[48] ={
    32, 1,  2,   3, 4,  5,
    4,  5,  6,   7, 8,  9,
    8,  9,  10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

int Pbox[32] ={
    16,  7, 20, 21,
    29, 12, 28, 17,
     1, 15, 23, 26,
     5, 18, 31, 10,
     2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25
};

int Sbox[512] = { //8*4*16
    
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
     0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
     4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
     3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
     0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
     1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
     7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
     3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
     2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
     4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
     9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
     4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
     4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
     1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
     1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
     7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
     2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
 };

ull iniKey[8] = {
    0x13,0x34,0x57,0x79,0x9B,0xBC,0xDF,0xF1};

ull message[8] = {    
    0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};



int main()
{

  cl_device_id device_id = NULL;
  cl_context context = NULL;
  cl_command_queue command_queue = NULL;

  cl_mem d_pi = NULL;

    //host and devices copies
    cl_mem d_LnRnBlocks = NULL;//17 size
    cl_mem d_CnDnBlocks= NULL;//17 size
    cl_mem d_keysBlocks= NULL;//16 size
    cl_mem d_Rotations= NULL;//16 size
    cl_mem d_PC1 = NULL;//56 size
    cl_mem d_PC2 = NULL;//48 size
    cl_mem d_IniPer = NULL;//64 size
    cl_mem d_reverseIniPer = NULL;//64 size
    cl_mem d_Expansion = NULL;//48 size
    cl_mem d_Pbox = NULL;//32 size
    cl_mem d_Sbox = NULL;//8*8*16 size [8][4][16]
    cl_mem d_iniKey = NULL;//8 size
    cl_mem d_message = NULL;//8 size
    cl_mem d_result = NULL;//MAX SIZE
    ull *result = NULL; //MAX SIZE

        //size of host and device copies
    int sd_LnRnBlocks = 17 * 2 * sizeof(ull);
    int sd_CnDnBlocks = 17 * 2 * sizeof(ull);
    int sd_keysBlocks = 16 * sizeof(ull);
    int sd_Rotations = 16 * sizeof(ull);
    int sd_PC1 = 56 * sizeof(int);
    int sd_PC2 = 48 * sizeof(int);
    int sd_IniPer = 64 * sizeof(int);
    int sd_reverseIniPer = 64 * sizeof(int);
    int sd_Expansion = 48 * sizeof(int);
    int sd_Pbox = 32 * sizeof(int);
    int sd_Sbox = 512 * sizeof(int);
    int sd_iniKey = 8 * sizeof(ull);
    int sd_message = 8 * sizeof(ull);
    int sd_result = NUMTHREADS * sizeof(ull);
  
  
  cl_program program = NULL;
  cl_kernel kernel = NULL;
  cl_platform_id platform_id = NULL;
  cl_uint ret_num_devices;
  cl_uint ret_num_platforms;
  cl_int ret;

  int          err;               // error code returned from OpenCL calls
  char string[MEM_SIZE];
  double h_pi[NUMTHREADS];
  int iterations;
  /******************************************************************************/
  /* open kernel */
  FILE *fp;
  char fileName[] = "./cpl.cl";
  char *source_str;
  size_t source_size;

  /* Load the source code containing the kernel*/
  fp = fopen(fileName, "r");
  if (!fp) {
  fprintf(stderr, "Failed to load kernel.\n");
  exit(1);
  }
  source_str = (char*)malloc(MAX_SOURCE_SIZE);
  source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
  fclose(fp);
  /******************************************************************************/
  /* create objects */

  /* Get Platform and Device Info */
  ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
  ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

  /* Create OpenCL context */
  context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
  checkError(ret, "Creating context");

  /* Create Command Queue */
  command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
  checkError(ret, "Creating queue");

  /* Create Memory Buffer */
  d_LnRnBlocks = clCreateBuffer(context, CL_MEM_READ_WRITE, sd_LnRnBlocks, NULL, &ret);


  d_CnDnBlocks = clCreateBuffer(context, CL_MEM_READ_WRITE, sd_CnDnBlocks, NULL, &ret);


  d_keysBlocks = clCreateBuffer(context, CL_MEM_READ_WRITE, sd_keysBlocks , NULL, &ret);
  
  d_Rotations = clCreateBuffer(context, CL_MEM_READ_WRITE, sd_Rotations, NULL, &ret);


  d_PC1 = clCreateBuffer(context, CL_MEM_READ_WRITE, sd_PC1, NULL, &ret);


  d_PC2 = clCreateBuffer(context, CL_MEM_READ_WRITE, sd_PC2, NULL, &ret);


  d_IniPer = clCreateBuffer(context, CL_MEM_READ_WRITE, sd_IniPer, NULL, &ret);


  d_reverseIniPer = clCreateBuffer(context, CL_MEM_READ_WRITE, sd_reverseIniPer, NULL, &ret);


  d_Expansion = clCreateBuffer(context, CL_MEM_READ_WRITE, sd_Expansion, NULL, &ret);


  d_Pbox = clCreateBuffer(context, CL_MEM_READ_WRITE, sd_Pbox, NULL, &ret);


  d_Sbox = clCreateBuffer(context, CL_MEM_READ_WRITE, sd_Sbox, NULL, &ret);
 

  d_iniKey = clCreateBuffer(context, CL_MEM_READ_WRITE, sd_iniKey, NULL, &ret);


  d_message = clCreateBuffer(context, CL_MEM_READ_WRITE, sd_message, NULL, &ret);


  d_result = clCreateBuffer(context, CL_MEM_READ_WRITE, sd_result, NULL, &ret);
  checkError(ret, "Creating buffer d_pì");

  //alloc space for host copies
    result = (ull *) malloc(sd_result);

  //print kernel
  //printf("\n%s\n%i bytes\n", source_str, (int)source_size); fflush(stdout);
  /******************************************************************************/
  /* create build program */

  /* Create Kernel Program from the source */
  program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
  (const size_t *)&source_size, &ret);
  checkError(ret, "Creating program");

  /* Build Kernel Program */
  ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
  if (ret != CL_SUCCESS)
  {
      size_t len;
      char buffer[2048];

      printf("Error: Failed to build program executable!\n%s\n", err_code(ret));
      clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
      printf("%s\n", buffer);
      return EXIT_FAILURE;
  }
  /* Create OpenCL Kernel */
  kernel = clCreateKernel(program, "cipherDES", &ret);
  checkError(ret, "Creating kernel");

  /* Set OpenCL Kernel Parameters */
  iterations = ITERATIONS;
  ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&d_LnRnBlocks);
  checkError(ret, "Setting kernel arguments");

  ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&d_CnDnBlocks);
  checkError(ret, "Setting kernel arguments");

  ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&d_keysBlocks);
  checkError(ret, "Setting kernel arguments");

  ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&d_Rotations);
  checkError(ret, "Setting kernel arguments");

  ret = clSetKernelArg(kernel, 4, sizeof( cl_mem ), (void *)&d_PC1);
  checkError(ret, "Setting kernel arguments");  

  ret = clSetKernelArg(kernel, 5, sizeof(cl_mem), (void *)&d_PC2);
  checkError(ret, "Setting kernel arguments");

  ret = clSetKernelArg(kernel, 6, sizeof(cl_mem), (void *)&d_IniPer);
  checkError(ret, "Setting kernel arguments");

  ret = clSetKernelArg(kernel, 7, sizeof(cl_mem), (void *)&d_reverseIniPer);
  checkError(ret, "Setting kernel arguments");

  ret = clSetKernelArg(kernel, 8, sizeof(cl_mem), (void *)&d_Expansion);
  checkError(ret, "Setting kernel arguments");

  ret = clSetKernelArg(kernel, 9, sizeof(cl_mem), (void *)&d_Pbox);
  checkError(ret, "Setting kernel arguments");

  ret = clSetKernelArg(kernel, 10, sizeof(cl_mem), (void *)&d_Sbox);
  checkError(ret, "Setting kernel arguments");
  
  ret = clSetKernelArg(kernel, 11, sizeof(cl_mem), (void *)&d_iniKey);
  checkError(ret, "Setting kernel arguments");

  ret = clSetKernelArg(kernel, 12, sizeof(cl_mem), (void *)&d_message);
  checkError(ret, "Setting kernel arguments");

  ret = clSetKernelArg(kernel, 13, sizeof(cl_mem), (void *)&d_result);
  checkError(ret, "Setting kernel arguments");

  clEnqueueWriteBuffer(command_queue, d_LnRnBlocks, CL_TRUE, 0, sd_LnRnBlocks, &LnRnBlocks, 0, NULL, NULL);
  clEnqueueWriteBuffer(command_queue, d_CnDnBlocks, CL_TRUE, 0, sd_CnDnBlocks, &CnDnBlocks, 0, NULL, NULL);
  clEnqueueWriteBuffer(command_queue, d_keysBlocks, CL_TRUE, 0, sd_keysBlocks, &keysBlocks, 0, NULL, NULL);
  clEnqueueWriteBuffer(command_queue, d_Rotations, CL_TRUE, 0, sd_Rotations, &Rotations, 0, NULL, NULL);
  clEnqueueWriteBuffer(command_queue, d_PC1, CL_TRUE, 0, sd_PC1, &PC1, 0, NULL, NULL);
  clEnqueueWriteBuffer(command_queue, d_PC2, CL_TRUE, 0, sd_PC2, &PC2, 0, NULL, NULL);
  clEnqueueWriteBuffer(command_queue, d_IniPer, CL_TRUE, 0, sd_IniPer, &IniPer, 0, NULL, NULL);
  clEnqueueWriteBuffer(command_queue, d_reverseIniPer, CL_TRUE, 0, sd_reverseIniPer, &reverseIniPer, 0, NULL, NULL);
  clEnqueueWriteBuffer(command_queue, d_Expansion, CL_TRUE, 0, sd_Expansion, &Expansion, 0, NULL, NULL);
  clEnqueueWriteBuffer(command_queue, d_Pbox, CL_TRUE, 0, sd_Pbox, &Pbox, 0, NULL, NULL);
  clEnqueueWriteBuffer(command_queue, d_Sbox, CL_TRUE, 0, sd_Sbox, &Sbox, 0, NULL, NULL);
  clEnqueueWriteBuffer(command_queue, d_iniKey, CL_TRUE, 0, sd_iniKey, &iniKey, 0, NULL, NULL);
  clEnqueueWriteBuffer(command_queue, d_message, CL_TRUE, 0, sd_message, &message, 0, NULL, NULL);
  


  size_t global_work_size = NUMTHREADS;
  size_t local_work_size = NUMTHREADS/WORKGROUPS;
  cl_uint work_dim = 1;
  /* Execute OpenCL Kernel */
  //ret = clEnqueueTask(command_queue, kernel, 0, NULL,NULL);  //single work item
  ret = clEnqueueNDRangeKernel(command_queue, kernel, work_dim,
    0, &global_work_size, &local_work_size, 0, NULL, NULL);
  checkError(ret, "Enqueueing kernel");
  ret = clFinish(command_queue);
  checkError(ret, "Waiting for commands to finish");
  /******************************************************************************/
  /* Copy results from the memory buffer */
  ret = clEnqueueReadBuffer(command_queue, d_result, CL_TRUE, 0, sd_result, result, 0, NULL, NULL);
  checkError(ret, "Creating program");

    //printf result

  int i;
  for(i = 0; i < NUMTHREADS; i++){
    printf("cipher: %i\t%llX\n",i,result[i]);
    fflush(stdout);
  }

  /* Finalization */
  ret = clFlush(command_queue);
  ret = clFinish(command_queue);
  ret = clReleaseKernel(kernel);
  ret = clReleaseProgram(program);
  ret = clReleaseMemObject(d_pi);
  ret = clReleaseCommandQueue(command_queue);
  ret = clReleaseContext(context);

  free(source_str);
  //free host space
  free(result);

  return 0;
}
