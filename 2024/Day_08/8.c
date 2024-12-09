#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <math.h>

struct antenna_positions {
  int pos_len;
  int capacity;
  int (*positions)[2];
};

int locate(char *antennaeSymbols, int antennaeLen, char c) {
  for (int i = 0; i < antennaeLen; i++) {
    if (antennaeSymbols[i] == c) {
      return i;
    }
  }
  return -1;
}

void add_to_set(int (**loc)[2], int *p, int *capacity, int pr, int pc) {
  if (*p == *capacity) {
    *capacity *= 2;
    *loc = realloc(*loc, *capacity * sizeof(int[2]));
  }

  for (int i = 0; i < *p; i++) {
    if ((*loc)[i][0] == pr && (*loc)[i][1] == pc) {
      return;
    }
  }

  (*loc)[*p][0] = pr;
  (*loc)[*p][1] = pc;
  (*p)++;
  return;
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
  int antennaeCapacity = 10;
  char *antennaeSymbols = malloc(antennaeCapacity * sizeof(char));
  struct antenna_positions *antennae = malloc(antennaeCapacity * sizeof(struct antenna_positions));
  int antennaeLen = 0;
  int C = strlen(line);
  int r = 0;

  while (line != NULL) {
    int n = strlen(line);
    for (int c = 0; c < n; c++) {
      if (line[c] == '.' || line[c] == '#') {
        continue;
      }

      if (antennaeLen == antennaeCapacity) {
        antennaeCapacity *= 2;
        antennae = realloc(antennae, antennaeCapacity * sizeof(struct antenna_positions));
        antennaeSymbols = realloc(antennaeSymbols, antennaeCapacity * sizeof(char));
      }

      int idx = locate(antennaeSymbols, antennaeLen, line[c]);
      if (idx == -1) {
        antennaeSymbols[antennaeLen] = line[c];
        struct antenna_positions temp;
        temp.pos_len = 0;
        temp.capacity = 5;
        temp.positions = malloc(temp.capacity * sizeof(int[2]));
        temp.positions[temp.pos_len][0] = r;
        temp.positions[temp.pos_len][1] = c;
        temp.pos_len++;
        antennae[antennaeLen] = temp;
        antennaeLen++;
      } else {
        struct antenna_positions *temp = &antennae[idx];
        if (temp->pos_len == temp->capacity) {
          temp->capacity *= 2;
          temp->positions = realloc(temp->positions, temp->capacity * sizeof(int[2]));
        }
        temp->positions[temp->pos_len][0] = r;
        temp->positions[temp->pos_len][1] = c;
        temp->pos_len++;
      }
    }
    r++;
    line = strtok(NULL, "\n");
  }

  int R = r;
  //printf("%d,%d\n", R, C);
  int p1 = 0, p2 = 0;
  int capacity1 = 10, capacity2 = 10;
  int (*loc1)[2] = malloc(capacity1 * sizeof(int[2]));
  int (*loc2)[2] = malloc(capacity2 * sizeof(int[2]));

  for (int i = 0; i < antennaeLen; i++) {
    struct antenna_positions *temp = &antennae[i];
    int n = temp->pos_len;
    int (*pos)[2] = temp->positions;
    for (int j = 0; j < n - 1; j++) {
      int pr1 = pos[j][0];
      int pc1 = pos[j][1];
      //printf("%d, %d\n", pr1, pc1);
      for (int k = j + 1; k < n; k++) {
        int pr2 = pos[k][0];
        int pc2 = pos[k][1];
        //printf("%d, %d\n\n", pr2, pc2);

        int dx = pr1 - pr2;
        int dy = pc1 - pc2;

        if (pr1 + dx >= 0 && pr1 + dx < R && pc1 + dy >= 0 && pc1 + dy < C) {
          add_to_set(&loc1, &p1, &capacity1, pr1 + dx, pc1 + dy);
        }
        if (pr2 - dx >= 0 && pr2 - dx < R && pc2 - dy >= 0 && pc2 - dy < C) {
          add_to_set(&loc1, &p1, &capacity1, pr2 - dx, pc2 - dy);
        }

        int pAr = pr1 - dx;
        int pAc = pc1 - dy;
        while (pAr >= 0 && pAr < R && pAc >= 0 && pAc < C) {
          add_to_set(&loc2, &p2, &capacity2, pAr, pAc);
          pAr -= dx;
          pAc -= dy;
        }

        int pBr = pr1 + dx;
        int pBc = pc1 + dy;
        while (pBr >= 0 && pBr < R && pBc >= 0 && pBc < C) {
          add_to_set(&loc2, &p2, &capacity2, pBr, pBc);
          pBr += dx;
          pBc += dy;
        }

        add_to_set(&loc2, &p2, &capacity2, pr1, pc1);
      }
    }
  }

  printf("%d\n", p1);
  printf("%d\n", p2);

  //for (int i = 0; i < p2; i++) {
  //  printf("%d, %d\n", loc2[i][0], loc2[i][1]);
  //}

  free(buffer);
  free(antennaeSymbols);
  for (int i = 0; i < antennaeLen; i++) {
    free(antennae[i].positions);
  }
  free(antennae);
  free(loc1);
  free(loc2);

  return 0;
}
