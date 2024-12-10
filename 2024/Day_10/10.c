#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

struct positions {
  int n1;
  int n2;
  int (*found)[2];
};

const int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int locate(int (**found)[2], int *found_len, int r, int c) {
  for (int i = 0; i < *found_len; i++) {
    if ((*found)[i][0] == r && (*found)[i][1] == c) {
      return 1;
    }
  }
  return 0;
}

struct positions traverse(int R, int C, int r, int c, int (**found)[2], int *found_len, int *found_capacity, int **grid) {
  if (grid[r][c] == 9 && locate(found, found_len, r, c)) {
    struct positions res;
    res.n1 = 0;
    res.n2 = 1;
    res.found = *found;
    return res;
  } else if (grid[r][c] == 9) {
    if (*found_len == *found_capacity) {
      (*found_capacity) *= 2;
      *found = realloc(*found, (*found_capacity) * sizeof(int[2]));
    }
    (*found)[*found_len][0] = r;
    (*found)[*found_len][1] = c;
    (*found_len)++;

    struct positions res;
    res.n1 = 1;
    res.n2 = 1;
    res.found = *found;
    return res;
  }

  int n1 = 0, n2 = 0;
  for (int i = 0; i < 4; i++) {
    int nr = r + dirs[i][0];
    int nc = c + dirs[i][1];
    if (0 <= nr && nr < R && 0 <= nc && nc < C && grid[nr][nc] - grid[r][c] == 1) {
      struct positions pos = traverse(R, C, nr, nc, found, found_len, found_capacity, grid);
      n1 += pos.n1;
      n2 += pos.n2;
    }
  }
  struct positions res;
  res.n1 = n1;
  res.n2 = n2;
  res.found = *found;
  return res;
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
  int R = 0;
  int C = strlen(line);
  int grid_capacity = 10;
  int **grid = malloc(grid_capacity * sizeof(int *));
  while (line != NULL) {
    if (R == grid_capacity) {
      grid_capacity *= 2;
      grid = realloc(grid, grid_capacity * sizeof(int *));
    }
    int *row = malloc(C * sizeof(int));
    for (int i = 0; i < C; i++) {
      row[i] = line[i] - '0';
    }
    grid[R] = row;

    R++;
    line = strtok(NULL, "\n");
  }

  int p1 = 0, p2 = 0;

  for (int r = 0; r < R; r++) {
    for (int c = 0; c < C; c++) {
      if (grid[r][c] == 0) {
        int found_capacity = 3;
        int (*found)[2] = malloc(found_capacity * sizeof(int[2]));
        int found_len = 0;
        struct positions res = traverse(R, C, r, c, &found, &found_len, &found_capacity, grid);
        p1 += res.n1;
        p2 += res.n2;
      }
    }
  }

  printf("%d\n", p1);
  printf("%d\n", p2);

  free(buffer);
  for (int i = 0; i < R; i++) {
    free(grid[i]);
  }
  free(grid);

  return 0;
}
