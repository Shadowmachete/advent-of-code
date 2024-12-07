#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

struct rcd {
  int r;
  int c;
  int dir_len;
  int (*dirs)[2];
};

struct int_rcdarr_tuple {
  int p2_loop_found;
  int pos_len;
  struct rcd *positions;
};

int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

struct int_rcdarr_tuple* traverse(int pr, int pc, int br, int bc, int r, int c, char **lines) {
  struct rcd *positions = malloc(100 * sizeof(struct rcd));
  int pos_len = 0;
  int pos_capacity = 100;
  int d = 0;

  bool *visited = (bool *)malloc((r * c * 4) * sizeof(bool));
  while (visited == NULL) {
    bool *visited = (bool *)malloc((r * c * 4) * sizeof(bool));
  }

  bool **vis = malloc(r * sizeof(bool *));
  for (int i = 0; i < r; i++) {
    vis[i] = calloc(c, sizeof(bool));
  }

  for (size_t i = 0; i < (r * c * 4); i++) {
    visited[i] = false;
  }

  while (true) {
    int dr = directions[d][0], dc = directions[d][1];
    int hash = (pr * c + pc) * 4 + d;

    if (visited[hash]) {
      struct int_rcdarr_tuple *res = malloc(sizeof(struct int_rcdarr_tuple));
      res->p2_loop_found = 1;
      res->pos_len = pos_len;
      res->positions = positions;
      free(visited);
      return res;
    } else if (vis[pr][pc]) {
      for (int pos_idx = 0; pos_idx < pos_len; pos_idx++) {
        if (positions[pos_idx].r == pr && positions[pos_idx].c == pc) {
          positions[pos_idx].dirs[positions[pos_idx].dir_len][0] = dr;
          positions[pos_idx].dirs[positions[pos_idx].dir_len][1] = dc;
          positions[pos_idx].dir_len++;
          break;
        }
      }
    } else {
      if (pos_len == pos_capacity) {
        pos_capacity *= 2;
        positions = realloc(positions, pos_capacity * sizeof(struct rcd));
      }

      struct rcd new_position;
      new_position.r = pr;
      new_position.c = pc;
      new_position.dir_len = 1;
      new_position.dirs = malloc(4 * sizeof(int[2]));
      memcpy(new_position.dirs[0], (int[2]){dr, dc}, sizeof(int[2]));
      positions[pos_len] = new_position;
      pos_len++;
      visited[hash] = true;
      vis[pr][pc] = true;
    }


    int nr = pr + dr;
    int nc = pc + dc;

    if (0 <= nr && nr < r && 0 <= nc && nc < c && (lines[nr][nc] == '#' || (br != -1 && bc != -1 && nr == br && nc == bc))) {
      d = (d + 1) % 4;
      continue;
    }

    if (nr < 0 || nr >= r || nc < 0 || nc >= c) {
      break;
    }

    pr = nr;
    pc = nc;
  }

  struct int_rcdarr_tuple *res = malloc(sizeof(struct int_rcdarr_tuple));
  res->p2_loop_found = 0;
  res->pos_len = pos_len;
  res->positions = positions;

  free(visited);

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

  char **lines = NULL;
  char *line = strtok(buffer, "\n");
  int R = 0, C = strlen(line);

  while (line != NULL) {
    lines = realloc(lines, (R + 1) * sizeof(char *));
    lines[R] = line;
    R++;
    line = strtok(NULL, "\n");
  }

  int sr, sc;
  for (int r = 0; r < R; r++) {
    for (int c = 0; c < C; c++) {
      if (lines[r][c] == '^') {
        sr = r;
        sc = c;
        break;
      }
    }
  }

  struct int_rcdarr_tuple *res = traverse(sr, sc, -1, -1, R, C, lines);
  int p1 = res->pos_len;

  printf("%d\n", p1);

  int num_blocks = 0;
  int (*checked_blocks)[2] = malloc(100 * sizeof(int[2]));
  int checked_blocks_capacity = 100;
  int p2 = 0;
  for (int pos_idx = 0; pos_idx < res->pos_len; pos_idx++) {
    int pr = res->positions[pos_idx].r;
    int pc = res->positions[pos_idx].c;
    int dir_len = res->positions[pos_idx].dir_len;
    int (*dirs)[2] = res->positions[pos_idx].dirs;
    for (int dir_idx = 0; dir_idx < dir_len; dir_idx++) {
      int dr = dirs[dir_idx][0];
      int dc = dirs[dir_idx][1];

      int br = pr + dr;
      int bc = pc + dc;

      if (br < 0 || br >= R || bc < 0 || bc >= C || lines[br][bc] == '#' || (br == sr && bc == sc)) {
        continue;
      }

      int checked = 0;
      for (int i = 0; i < num_blocks; i++) {
        if (checked_blocks[i][0] == br && checked_blocks[i][1] == bc) {
          checked = 1;
          break;
        }
      }

      if (checked) {
        continue;
      }

      if (num_blocks == checked_blocks_capacity) {
        checked_blocks_capacity *= 2;
        checked_blocks = realloc(checked_blocks, checked_blocks_capacity * sizeof(int[2]));
      }
      checked_blocks[num_blocks][0] = br;
      checked_blocks[num_blocks][1] = bc;
      num_blocks++;

      struct int_rcdarr_tuple *res2 = traverse(sr, sc, br, bc, R, C, lines);
      p2 += res2->p2_loop_found;
      free(res2);
    }
  }

  printf("%d\n", p2);

  free(buffer);
  free(lines);
  for (int pos_idx = 0; pos_idx < res->pos_len; pos_idx++) {
    free(res->positions[pos_idx].dirs);
  }
  free(res->positions);
  free(res);
  free(checked_blocks);

  return 0;
}
