#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

bool condition(int (*rules)[2], int r_len, int a, int b) {
  for (int i = 0; i < r_len; i++) {
    if (rules[i][0] == a && rules[i][1] == b) {
      return true;
    }
  }
  return false;
}

bool correct(int *nums, int n, int (*rules)[2], int r_len) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      if (condition(rules, r_len, nums[j], nums[i])) {
        return false;
      }
    }
  }

  return true;
}

void reorder(int *nums, int n, int (*rules)[2], int r_len) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      if (condition(rules, r_len, nums[j], nums[i])) {
        int temp = nums[i];
        nums[i] = nums[j];
        nums[j] = temp;
      }
    }
  }
}

int main(int argc, char *argv[])
{
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

  int (*rules)[2] = NULL;
  char *line = buffer;
  char *end;
  int r_len = 0;
  char rule_or_update = 'r';
  int a, b;
  int p1 = 0, p2 = 0;

  while ((end = strchr(line, '\n')) != NULL) {
    *end = '\0';

    if (strlen(line) == 0) {
      rule_or_update = 'u';
      line = end + 1;
      continue;
    }

    if (rule_or_update == 'r') {
      rules = realloc(rules, (r_len + 1) * sizeof(int[2]));
      sscanf(line, "%d|%d", &a, &b);
      memcpy(rules[r_len], (int[2]){a,b}, sizeof(int[2]));
      r_len++;
    } else {
      int *nums = NULL;
      int n = 0;
      char *num = strtok(line, ",");

      while (num != NULL) {
        nums = realloc(nums, (n + 1) * sizeof(int));
        nums[n] = atoi(num);
        n++;
        num = strtok(NULL, ",");
      }

      if (correct(nums, n, rules, r_len)) {
        p1 += nums[n / 2];
      } else {
        reorder(nums, n, rules, r_len);
        p2 += nums[n / 2];
      }

      free(nums);
    }

    line = end + 1;
  }

  if (*line != '\0') {
    printf("Last token: %s\n", line);
  }

  printf("%d\n", p1);
  printf("%d\n", p2);

  free(rules);
  free(buffer);

  return 0;
}
