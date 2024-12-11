#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

struct entry {
  long long n;
  int s;
  long long c;
};

long long count(struct entry **arr, int *arr_len, int *arr_cap, long long n, int steps) {
  int found = 0;
  long long return_val;
  for (int i = 0; i < (*arr_len); i++) {
    struct entry temp = (*arr)[i];
    if (temp.n == n && temp.s == steps) {
      return_val = temp.c;
      found = 1;
      break;
    }
  }
  if (found == 0) {
    if (steps == 0) {
      return_val = 1;
    } else if (n == 0) {
      return_val = count(arr, arr_len, arr_cap, 1, steps - 1);
    } else {
      char str_n[21];
      sprintf(str_n, "%lld", n);
      int str_len = strlen(str_n);
      if (str_len % 2 == 0) {
        long long left = 0;
        long long right = 0;
        for (int i = 0; i < str_len; i++) {
          if (i < str_len / 2) {
            left = left * 10 + str_n[i] - '0';
          } else {
            right = right * 10 + str_n[i] - '0';
          }
        }
        return_val = count(arr, arr_len, arr_cap, left, steps - 1) + count(arr, arr_len, arr_cap, right, steps - 1);
      } else {
        long long new_n = n * 2024;
        return_val = count(arr, arr_len, arr_cap, new_n, steps - 1);
      }
    }
    if (*arr_len == *arr_cap) {
      *arr_cap *= 2;
      *arr = realloc(*arr, (*arr_cap) * sizeof(struct entry));
    }
    struct entry temp;
    temp.n = n;
    temp.s = steps;
    temp.c = return_val;
    (*arr)[(*arr_len)++] = temp;
  }

  return return_val;
}

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
  int num_cap = 10;
  int num_len = 0;
  long *num_arr = malloc(num_cap * sizeof(long));
  char *num = strtok(buffer, " ");
  while (num != NULL) {
    if (num_len == num_cap) {
      num_cap *= 2;
      num_arr = realloc(num_arr, num_cap * sizeof(long));
    }
    num_arr[num_len++] = atoi(num);
    num = strtok(NULL, " ");
  }

  int arr_len = 0;
  int arr_cap = 10;
  struct entry *arr = malloc(arr_cap * sizeof(struct entry));
  long long p1 = 0, p2 = 0;
  for (int i = 0; i < num_len; i++) {
    long long n = num_arr[i];
    p1 += count(&arr, &arr_len, &arr_cap, n, 25);
    p2 += count(&arr, &arr_len, &arr_cap, n, 75);
  }

  printf("%lld\n", p1);
  printf("%lld\n", p2);

  free(buffer);
  free(num_arr);
  free(arr);
  return 0;
}
