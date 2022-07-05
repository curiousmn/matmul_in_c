#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define DEBUG 1 
#define N 1024 

void checkValues(float C[], float val[]);
float A[N*N] __attribute__ ((aligned (32)));
float B[N*N] __attribute__ ((aligned (32)));
float C[N*N] __attribute__ ((aligned (32)));
float val[N*N] __attribute__ ((aligned (32))); 

int main(){
   
  FILE *f = fopen("/tmp/matmul", "rb");
  fread(A, 1, sizeof(int)*N*N, f);
  fread(B, 1, sizeof(int)*N*N, f);
  fread(val, 1, sizeof(int)*N*N, f);
  fclose(f);

  for(int x = 0; x < N; x++){
    for (int k = 0; k < N; k++){
      for (int y = 0; y < N; y++){
        C[N * x + (k)] += A[N * x + (y)] * B[N * y + (k)];
      }
    }
  }

#if DEBUG == 1

  for(int i=0; i < N; i++){
    for (int j=0; j < N; j++){
      if (C[N * i + j] - val[N * i + j] > 1e-3){
        printf("Missmatch at [%d][%d]\t values %.3f %.3f\n", i ,j , C[N * i+j], val[N * i+j]);
      }
    }
  }

#else
  checkValues(A, B);

#endif
}

void checkValues(float C[], float val[]){
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
      printf("%f %f\n", C[N * i + (j)], val[N * i + (j)]);
    }
    printf("\n");
  }

}
