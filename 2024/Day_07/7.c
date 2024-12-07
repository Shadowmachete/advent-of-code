#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <math.h>

int main(int argc, char *argv[]) {
  char *file;
  char *program_name = argv[0];
  char *base_name = basename(program_name);
  char *dot_pos = strchr(base_name, '.');

  if (dot_pos != NULL) {
    *dot_pos = '\0';
  }
  int program_number = atoi(base_name);

  if (argc != 2) {
    char *buff = malloc(20);
    if (buff == NULL) {
      perror("Memory allocation failed");
      return 1;
    }
    snprintf(buff, sizeof(buff), "%d.txt", program_number);
    file = buff;
  } else {
    file = argv[1];
  }

  FILE *fileptr = fopen(file, "r");
  if (fileptr == NULL) {
    perror("File opening failed");
    free(file);
    return 1;
  }

  fseek(fileptr, 0, SEEK_END);
  long fsize = ftell(fileptr);
  fseek(fileptr, 0, SEEK_SET);

  char *buffer = malloc(fsize + 1);
  size_t bytes_read = fread(buffer, 1, fsize, fileptr);

  if (bytes_read != fsize) {
    perror("File reading failed");
    fclose(fileptr);
    free(buffer);
    return 1;
  }

  fclose(fileptr);
  if (argc != 2) {
    free(file);
  }

  buffer[fsize] = '\0';
  char *line = buffer;
  char *end;
  long long p1 = 0, p2 = 0;
  int powers_of_3[13];
  powers_of_3[0] = 1;
  for (int i = 1; i < 13; i++) {
    powers_of_3[i] = powers_of_3[i - 1] * 3;
  }

  while ((end = strchr(line, '\n')) != NULL) {
    int ok1 = 0;
    int ok2 = 0;
    long long goal;
    int capacity = 15;
    long long *nums = malloc(capacity * sizeof(long long));
    int num_len = 0;
    *end = '\0';

    char *tok = strtok(line, ": ");
    goal = atoll(tok);
    tok = strtok(NULL, " ");
    while (tok != NULL) {
      if (num_len >= capacity) {
        capacity *= 2;
        nums = realloc(nums, capacity * sizeof(long long));
      }
      nums[num_len] = atoll(tok);
      num_len++;
      tok = strtok(NULL, " ");
    }

    int power = num_len - 1;
    int n2 = 1 << power;
    int n3 = powers_of_3[power];

    for (int num = 0; num < n2; num++) {
      long long sum1 = nums[0];
      int i = 1;
      while (i < num_len) {
        int q = (num >> (power - i)) & 1;
        if (q == 0) {
          sum1 += nums[i];
        } else {
          sum1 *= nums[i];
        }
        i++;
      }
      if (sum1 == goal) {
        ok1 = 1;
        break;
      }
    }

    for (int num = 0; num < n3; num++) {
      int num_copy = num;
      long long sum2 = nums[0];
      int i = 1;
      while ((power - i) >= 0) {
        int q = num_copy / powers_of_3[power - i];
        if (q == 0) {
          sum2 += nums[i];
        } else if (q == 1) {
          sum2 *= nums[i];
        } else {
          long long num_copy_2 = nums[i];
          int l = 0;
          while (num_copy_2 > 0) {
            l++;
            num_copy_2 /= 10;
          }
          sum2 = sum2 * pow(10, l) + nums[i];
        }
        num_copy %= powers_of_3[power - i];
        i++;
      }
      if (sum2 == goal) {
        ok2 = 1;
        break;
      }
    }

    if (ok1) {
      p1 += goal;
    }
    if (ok2) {
      p2 += goal;
    }

    line = end + 1;
    free(nums);
  }

  if (*line != '\0') {
    printf("Last token: %s\n", line);
  }

  printf("%lld\n", p1);
  printf("%lld\n", p2);

  free(buffer);

  return 0;
}
