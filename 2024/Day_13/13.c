#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <math.h>

void gaussian_elimination(double (*matrix)[2][3]) {
  // normalise top row
  double first = (*matrix)[0][0];
  for (int i = 0; i < 3; i++) {
    (*matrix)[0][i] /= first;
  }

  // subtract n times of top row from bottom
  double n = ((*matrix)[1][0] / (*matrix)[0][0]);
  for (int i = 0; i < 3; i++) {
    (*matrix)[1][i] -= (*matrix)[0][i] * n;
  }

  // normalise 2nd row
  double second = (*matrix)[1][1];
  for (int i = 1; i < 3; i++) {
    (*matrix)[1][i] /= second;
  }

  // subtract n times of bottom row from top
  n = ((*matrix)[0][1] / (*matrix)[1][1]);
  for (int i = 1; i < 3; i++) {
    (*matrix)[0][i] -= (*matrix)[1][i] * n;
  }
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
  char *line = strtok(buffer, "\n");
  int p1 = 0;
  long long p2 = 0;
  while (line != NULL) {
    double x1, y1, x2, y2, pos1, pos2;
    sscanf(line, "Button A: X+%lf, Y+%lf", &x1, &y1);
    line = strtok(NULL, "\n");
    sscanf(line, "Button B: X+%lf, Y+%lf", &x2, &y2);
    line = strtok(NULL, "\n");
    sscanf(line, "Prize: X=%lf, Y=%lf", &pos1, &pos2);
    line = strtok(NULL, "\n");
    double matrix[2][3] = {{x1, x2, pos1}, {y1, y2, pos2}};
    gaussian_elimination(&matrix);
    int a = (int)round(matrix[0][2]);
    int b = (int)round(matrix[1][2]);
    if (matrix[0][2] >= 0 && matrix[1][2] >= 0 && (a * x1 + b * x2 == pos1) && (a * y1 + b * y2 == pos2)) {
      p1 += (3 * a + b);
    }
    double bigger_matrix[2][3] = {{x1, x2, pos1 + 10000000000000}, {y1, y2, pos2 + 10000000000000}};
    gaussian_elimination(&bigger_matrix);
    long long a2 = (long long)round(bigger_matrix[0][2]);
    long long b2 = (long long)round(bigger_matrix[1][2]);
    if (bigger_matrix[0][2] >= 0 && bigger_matrix[1][2] >= 0 && (a2 * x1 + b2 * x2 == pos1 + 10000000000000) && (a2 * y1 + b2 * y2 == pos2 + 10000000000000)) {
      p2 += (3 * a2 + b2);
    }
  }

  printf("%d\n", p1);
  printf("%lld\n", p2);

  free(buffer);

  return 0;
}
