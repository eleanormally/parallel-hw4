#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* compute_pi(void*);

typedef unsigned long long u64;

typedef struct ThreadArgs {
  u64 sampleCount;
  u64 result;
} ThreadArgs;

int main(int argc, char** argv) {
  int  threadCount = atoi(argv[1]);
  bool strongScaling = argc > 2 && strcmp(argv[2], "--strong") == 0;
  u64  sampleCount = 8 * (1ul << 30);
  if (strongScaling) {
    sampleCount = (2ul << 30) / threadCount * 32;
  }

  ThreadArgs arguments[threadCount];
  pthread_t  threads[threadCount];

  for (int i = 0; i < threadCount; i++) {
    arguments[i].sampleCount = sampleCount;
  }

  for (int i = 0; i < threadCount; i++) {
    pthread_create(&threads[i], NULL, compute_pi, &arguments[i]);
  }

  for (int i = 0; i < threadCount; i++) {
    void* c;
    pthread_join(threads[i], &c);
  }

  int totalResult = 0;
  for (int i = 0; i < threadCount; i++) {
    totalResult += arguments[i].result;
  }

  double calculation =
      (double)totalResult / (double)sampleCount / (double)threadCount * 4;
  printf("%s, %d, %lf\n", strongScaling ? "strong" : "weak", threadCount,
         calculation);
}

void* compute_pi(void* s) {
  ThreadArgs* args = (ThreadArgs*)s;
  args->result = 0;
  struct drand48_data seed_data;
  srand48_r((u64)s, &seed_data);
  double x, y;
  for (int i = 0; i < args->sampleCount; i++) {
    drand48_r(&seed_data, &x);
    drand48_r(&seed_data, &x);
    if (x * x + y * y < 1.0) {
      args->result++;
    }
  }
  return 0;
}
