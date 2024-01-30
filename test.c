#include <stdio.h>
#include <math.h>

int is_prime(long n){
    int i = 2;
    for(i = 2; i <= sqrt(n); i++){
        if(n % i == 0)
            break;
    }
    if(i > sqrt(n))
        return 1;
    else
      return 0;
}

/*
  -Criterion-
    1. Each number must be prime
    2. There are 5133 prime numbers.
    Reference: List of prime numbers, https://www.numbers.education/list.html
*/

int main(void) {
  FILE *file = fopen("primes.txt", "r");
  if (file == NULL) {
    printf("Error opening file!\n");
    return 0;
  }

  long num;
  int count = 0;
  while (fscanf(file, "%ld\n", &num) != EOF) {
    if (!is_prime(num)) {
      printf("Test failed: %ld is not a prime number\n", num);
      fclose(file);
      return 0;
    }
    count++;
  }

  if (count != 5133) {
    printf("Test failed: There are not 5133 numbers in the file\n");
    fclose(file);
    return 0;
  }

  printf("Test passed: All numbers in the file are prime and there are 5133 numbers\n");
  fclose(file);
  return 1;
}