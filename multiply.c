#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <cblas.h>
#include <time.h>
#include <string.h>
//#include <pthrad.h> For using threads


#define DEBUG 0 
#define N 128 
#define FAST 0
#define BLOCK 32 

void checkValues(float C[], float val[]);
void block_matrix(float A[], float B[], float val[]);
void blocked_multiply(float A[], float B[], float C[]); 

float A[N*N];
float B[N*N];
float C[N*N];
float val[N*N];
//void matmul_w_blas();

uint64_t nanos() {
  struct timespec start;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  return (uint64_t)start.tv_sec*1000000000 + (uint64_t)start.tv_nsec;
}


int main(int argc, char *argv[]){
  struct timespec et;
  struct timespec st;
  
  FILE *f = fopen("/tmp/matmul", "rb");
  fread(A, 1, sizeof(int)*N*N, f);
  fread(B, 1, sizeof(int)*N*N, f);
  fread(val, 1, sizeof(int)*N*N, f);
  fclose(f);
  double flop = (2.0*N*N*N)*1e-9;
  printf("%lf", flop);
#if FAST == 0
  uint64_t start = nanos();
  for(int x = 0; x < N; x++){
    for (int k = 0; k < N; k++){
      for (int y = 0; y < N; y++){
        C[N * x + (k)] += A[N * x + (y)] * B[N * y + (k)];
      }
    }
  }

  //blocked_multiply(A, B, C);
  uint64_t end = nanos();
  double s = (end - start) * 1e-9;
  printf("%lf\n", s);
  //printf("Real Time %ld\n", (st.tv_nsec - et.tv_nsec));
  printf("Gflops %lf\n", flop/s );
#else 
  blocked_multiply(A, B, C);
  //else{
   // matmul_w_blas();
  //}
#endif

#if DEBUG == 1

  for(int i=0; i < N; i++){
    for (int j=0; j < N; j++){
      if (C[N * i + j] - val[N * i + j] > 1e-3){
        printf("Missmatch at [%d][%d]\t values %.3f %.3f\n", i ,j , C[N * i+j], val[N * i+j]);
      }
    }
  }

#else
  //checkValues(A, B);

#endif
}

// compare values in 2 different arrays
void checkValues(float C[], float val[]){
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
      printf("%f %f\n", C[N * i + (j)], val[N * i + (j)]);
    }
    printf("\n");
  }
}

/*void bitFaster(float A[], float B[], float val[]){
  for (int i = 0; i < N; i++){
    float t = A[N * i +(0)];
    for (int j = 0; j < N; j++){
      val[N * i + (j)] = t * B[N * 0 + (j)];
    }

    for (int k = 0; k < N; k++){
      float s = 0;
      for (int j = 0; j < N; j++){
        s += A[N * i + (k)] * B[N * k +(j)];
      }
      C[N * i + (k)] = s;
    }
  }
}*/

//void matmul_w_blas(){
  //cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, A, N, B, N, 0.0, C, N);
//}

// Taking chunks of blocks in a matrix and multiply by those blocked size chunks
// But it doesn't work

void blocked_multiply(float A[], float B[], float C[]){
  float sum;

  for (int i = 0; i < N; i++){
    for (int k = 0; k < N; k++){
      C[(i * N) + k] = 0.0;
    }
  }

  int en = BLOCK * (N / BLOCK); 
  for (int kk = 0; kk < en; kk += BLOCK){
    for (int jj = 0; jj < en; jj += BLOCK){
      for (int i = 0; i < N; i++){
        for (int j = jj; j < jj + BLOCK; j++){
          sum = C[(i * N) + j];
          for (int k = kk; k < kk + BLOCK; k++){
            sum += A[(i * N) + k] + B[(k * N) + j];
          }
          C[(i * N) + j] += sum;
        }
      }
    }
  }
  // C = C + A * B
}

