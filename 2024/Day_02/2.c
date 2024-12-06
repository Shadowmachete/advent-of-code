#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <libgen.h>

bool is_good(int *line, int size) {
  bool inc = true;
  bool dec = true;
  bool safe = true;

  for (int i = 0; i < size - 1; i++) {
    int diff = line[i] - line[i+1];
    if (diff > 0) {
      dec = false;
    }
    if (diff < 0) {
      inc = false;
    }
    if (abs(diff) < 1 || abs(diff) > 3) {
      safe = false;
      break;
    }
  }
 
  return (inc || dec) && safe;
}

int *splice_array(int *arr, int size, int index) {
  if (index < 0 || index >= size) {
    printf("Index out of bounds\n");
    return NULL;
  }

  int *new_arr = malloc((size - 1) * sizeof(int));
  if (new_arr == NULL) {
    perror("Memory allocation failed");
    return NULL;
  }
  for (int i = 0; i < size - 1; i++) {
    if (i < index) {
      new_arr[i] = arr[i];
    } else {
      new_arr[i] = arr[i + 1];
    }
  }

  return new_arr;
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
 
  int p1 = 0, p2 = 0;

  char *line = strtok(buffer, "\n");
  char *context = NULL;

  while (line != NULL) {
    int *num_line = NULL;
    int size = 0;

    char *line_copy = malloc(strlen(line) + 1);
    if (line_copy == NULL) {
      perror("Memory allocation failed");
      return 1;
    }
    strcpy(line_copy, line);

    char *num = strtok_s(line_copy, " ", &context);
    while (num != NULL) {
      int number = atoi(num);
      num_line = realloc(num_line, (size + 1) * sizeof(int));
      num_line[size] = number;
      num = strtok_s(NULL, " ", &context);
      size++;
    }

    p1 += (int)is_good(num_line, size);

    bool good = false;
    for (int i = 0; i < size; i++) {
      int *num_line_2 = splice_array(num_line, size, i);
      if (is_good(num_line_2, size - 1)) {
        good = true;
        break;
      }
    }

    p2 += (int)good;

    free(line_copy);
    free(num_line);
    line = strtok(NULL, "\n");
  }
 
  printf("%d\n", p1);
  printf("%d\n", p2);

  free(buffer);
  return 0;
}
