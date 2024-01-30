#include <math.h>
#include <pthread.h>
#include <stdio.h>

/*
  ## Benchmarks
  
  Tests were run 5 times and the average was taken from the sum.
  
  **Multi-threaded:**
  - Real: 0m0.0642s
  - User: 0m0.035s
  - Sys: 0m0.0188s
  
  **Single-threaded:**
  - Real: 0m0.0788s (-18.5%)
  - User: 0m0.035s  (0%)
  - Sys: 0m0.0164s  (0.69%)

  Change
  
  ## Usage
  
  Follow these steps to run and test the program:
  
  1. Compile the main program: `gcc -pthread -o main main.c -lm`
  2. Run the main program: `./main`
  3. Compile the test program: `gcc -o test test.c -lm`
  4. Run the test program: `./test`
  5. To measure the execution time, use the `time` command: `time ./main`

  ## Testing
  
  The test reads the generated prime numbers from the file and verifies that all numbers are prime and contains 5133 numbers. If the test passes, it prints "Test passed: All numbers in the file are prime and there are 5133 numbers". If the test fails, "Test failed" along with the non-prime number.
*/

typedef struct {
  long begin;
  long end;
} Range;

//Create a mutex lock. Basically a lock so that a thread can't access the file at the same time. Needed because we're not just printing the primes but saving them into a file to test.

pthread_mutex_t lock;

//config
int save_file = 1; //1 for saving, 0 for not saving
//end of config

int is_prime(long n) {
  long i = 2;
  for (i = 2; i <= sqrt(n); i++) {
    if (n % i == 0)
      break;
  }
  if (i > sqrt(n))
    return 1;
  else
    return 0;
}

void *compute_prime(void *arg) {
  Range *range = (Range *)arg;
  for (long i = range->begin; i <= range->end; i++) {
    if(save_file){
      if (is_prime(i)) {
        pthread_mutex_lock(&lock);
        FILE *file = fopen("primes.txt", "a");
        if (file == NULL) {
          printf("Error opening file!\n");
          pthread_mutex_unlock(&lock);
          return NULL;
        }
        fprintf(file, "%ld\n", i);
        fclose(file);
        pthread_mutex_unlock(&lock);
      }
    }else{
      if (is_prime(i)) {
        printf("%ld\n", i);
      }
    }
  }
  return NULL;
}

// Single thread
/*
  FILE *file = fopen("primes.txt", "w");
  if (file == NULL) {
      printf("Error opening file!\n");
      return 0;
  }
  fclose(file);

  pthread_t thread;
  Range range = {2, 50000};
  pthread_create(&thread, NULL, compute_prime, &range);

  pthread_join(thread, NULL);

  return 0;
*/

// Multi threads (5)
int main(void) {
  
  // Clear file
  FILE *file = fopen("primes.txt", "w");
  
  if (file == NULL) {
    printf("Error opening file!\n");
    return 0;
  }
  fclose(file);
  
  if (pthread_mutex_init(&lock, NULL) != 0) {
    printf("\n mutex init failed\n");
    return 0;
  }
  
  pthread_t thread1, thread2, thread3, thread4, thread5;
  long sep = 10000;
  /* 
    Create a range so that each thread takes 10000 numbers [9998(10000 - 2) for thread1].
    Each range is put in a struct and passed to the thread.
  */
  Range ranges[5] = {{2, sep},
                     {sep + 1, sep * 2},
                     {sep * 2 + 1, sep * 3},
                     {sep * 3 + 1, sep * 4},
                     {sep * 4 + 1, sep * 5}};
  
  pthread_create(&thread1, NULL, compute_prime, &ranges[0]);
  pthread_create(&thread2, NULL, compute_prime, &ranges[1]);
  pthread_create(&thread3, NULL, compute_prime, &ranges[2]);
  pthread_create(&thread4, NULL, compute_prime, &ranges[3]);
  pthread_create(&thread5, NULL, compute_prime, &ranges[4]);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);
  pthread_join(thread4, NULL);
  pthread_join(thread5, NULL);
  pthread_mutex_destroy(&lock);
  
  return 1;
}