#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

typedef struct {
  int r;
  int c;
} pos;


int found(int r, int c, pos *positions, int pos_len) {
  for (int i = 0; i < pos_len; i++) {
    if (positions[i].r == r && positions[i].c == c) {
      return 1;
    }
  }
  return 0;
}

pos pop_left(pos *queue, int *queue_len) {
  pos res = queue[0];
  for (int i = 0; i < (*queue_len) - 1; i++) {
    queue[i] = queue[i + 1];
  }
  (*queue_len)--;
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
  char (*grid)[C] = malloc(grid_capacity * sizeof(char[C]));
  while (line != NULL) {
    if (R == grid_capacity) {
      grid_capacity *= 2;
      grid = realloc(grid, grid_capacity * sizeof(char[C]));
    }
    for (int i = 0; i < C; i++) {
      grid[R][i] = line[i];
    }

    R++;
    line = strtok(NULL, "\n");
  }

  int p1 = 0, p2 = 0;
  int vis_len = 0;
  int vis_cap = 10;
  pos (*visited) = malloc(vis_cap * sizeof(pos));
  const pos dirs[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  for (int r = 0; r < R; r++) {
    for (int c = 0; c < C; c++) {
      if (found(r, c, visited, vis_len)) {
        continue;
      }

      int area = 0;
      int perimeter = 0;
      pos *perim[4];
      int pl0 = 0, pl1 = 0, pl2 = 0, pl3 = 0;
      int pc0 = 5, pc1 = 5, pc2 = 5, pc3 = 5;
      perim[0] = (pos *)malloc(pc0 * sizeof(pos));
      perim[1] = (pos *)malloc(pc1 * sizeof(pos));
      perim[2] = (pos *)malloc(pc2 * sizeof(pos));
      perim[3] = (pos *)malloc(pc3 * sizeof(pos));
      int queue_cap = 10;
      pos *queue = malloc(queue_cap * sizeof(pos));
      queue[0] = (pos){r, c};
      int queue_len = 1;
      while (queue_len > 0) {
        pos p = pop_left(queue, &queue_len);
        int r1 = p.r, c1 = p.c;
        if (found(r1, c1, visited, vis_len)) {
          continue;
        }
        if (vis_len == vis_cap) {
          vis_cap *= 2;
          visited = realloc(visited, vis_cap * sizeof(int[2]));
        }
        visited[vis_len++] = (pos){r1, c1};
        area++;
        for (int i = 0; i < 4; i++) {
          int dr = dirs[i].r, dc = dirs[i].c;
          int rr = dr + r1, cc = dc + c1;
          if (0 <= rr && rr < R && 0 <= cc && cc < C && grid[rr][cc] == grid[r][c]) {
            if (queue_len == queue_cap) {
              queue_cap *= 2;
              queue = realloc(queue, queue_cap * sizeof(pos));
            }
            queue[queue_len++] = (pos){rr, cc};
          } else {
            perimeter++;
            int *l, *c;
            switch (i)
            {
            case 0:
              l = &pl0;
              c = &pc0;
              break;
            case 1:
              l = &pl1;
              c = &pc1;
              break;
            case 2:
              l = &pl2;
              c = &pc2;
              break;
            case 3:
              l = &pl3;
              c = &pc3;
              break;
            }

            int found = 0;
            for (int j = 0; j < *l; j++) {
              if (perim[i][j].r == r1 && perim[i][j].c == c1) {
                found = 1;
                break;
              }
            }
            if (!found) {
              if (*l == *c) {
                (*c)++;
                perim[i] = realloc(perim[i], (*c) * sizeof(pos));
              }
              perim[i][(*l)++] = (pos){r1, c1};
            }
          }
        }
      }
      free(queue);

      int sides = 0;
      for (int i = 0; i < 4; i++) {
        int vis_per_len = 0;
        int vis_per_cap = 10;
        pos *visited_perimeters = malloc(vis_per_cap * sizeof(pos));
        int *l, *c;
        switch (i)
        {
        case 0:
          l = &pl0;
          break;
        case 1:
          l = &pl1;
          break;
        case 2:
          l = &pl2;
          break;
        case 3:
          l = &pl3;
          break;
        }
        for (int j = 0; j < *l; j++) {
          int r1 = perim[i][j].r, c1 = perim[i][j].c;
          if (!found(r1, c1, visited_perimeters, vis_per_len)) {
            sides++;
            int queue_len = 1;
            int queue_cap = 10;
            pos *queue = malloc(queue_cap * sizeof(pos));
            queue[0] = (pos){r1, c1};
            while (queue_len > 0) {
              pos p = pop_left(queue, &queue_len);
              int r2 = p.r, c2 = p.c;
              if (found(r2, c2, visited_perimeters, vis_per_len)) {
                continue;
              }
              if (vis_per_len == vis_per_cap) {
                vis_per_cap *= 2;
                visited_perimeters = realloc(visited_perimeters, vis_per_cap * sizeof(pos));
              }
              visited_perimeters[vis_per_len++] = (pos){r2, c2};
              for (int d = 0; d < 4; d++) {
                int dr = dirs[d].r, dc = dirs[d].c;
                int rr = r2 + dr, cc = c2 + dc;
                if (found(rr, cc, perim[i], *l)) {
                  if (queue_len == queue_cap) {
                    queue_cap *= 2;
                    queue = realloc(queue, queue_cap * sizeof(pos));
                  }
                  queue[queue_len++] = (pos){rr, cc};
                }
              }
            }
            free(queue);
          }
        }
        free(visited_perimeters);
      }

      p1 += area * perimeter;
      p2 += area * sides;

      free(perim[0]);
      free(perim[1]);
      free(perim[2]);
      free(perim[3]);
    }
  }

  printf("%d\n", p1);
  printf("%d\n", p2);

  free(buffer);
  free(grid);
  free(visited);

  return 0;
}
