#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

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

  char **lines = NULL;
  char *line = strtok(buffer, "\n");
  int r = 0, c = strlen(line);

  while (line != NULL) {
    lines = realloc(lines, (r + 1) * sizeof(char *));
    lines[r] = line;
    r++;
    line = strtok(NULL, "\n");
  }

  int p1 = 0, p2 = 0;
  int dirs[8][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {-1, 0}, {0, -1}};
  int dirX, dirY;

  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      for (int k = 0; k < 8; k++) {
        dirX = dirs[k][0];
        dirY = dirs[k][1];
        if (i + 3 * dirX < 0 || i + 3 * dirX >= r || j + 3 * dirY < 0 || j + 3 * dirY >= c) {
          continue;
        }

        if (lines[i + 0 * dirX][j + 0 * dirY] == 'X' && lines[i + 1 * dirX][j + 1 * dirY] == 'M' && lines[i + 2 * dirX][j + 2 * dirY] == 'A' && lines[i + 3 * dirX][j + 3 * dirY] == 'S') {
          p1++;
        }
      }

      if (i - 1 < 0 || i + 1 >= r || j - 1 < 0 || j + 1 >= c) {
        continue;
      }

      char corners[4] = {lines[i + 1][j + 1], lines[i + 1][j - 1], lines[i - 1][j - 1], lines[i - 1][j + 1]};

      if (lines[i][j] == 'A' && (strcmp(corners, "MMSS") == 0 || strcmp(corners, "MSSM") == 0 || strcmp(corners, "SSMM") == 0 || strcmp(corners, "SMMS") == 0)) {
        p2++;
      }
    }
  }

  free(lines);

  printf("%d\n", p1);
  printf("%d\n", p2);

  free(buffer);

  return 0;
}
