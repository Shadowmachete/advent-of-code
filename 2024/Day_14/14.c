#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

typedef struct {
  int px;
  int py;
  int vx;
  int vy;
} pos_velo;

int* pop_left(int (*queue)[2], int *queue_len) {
  int *res = malloc(2 * sizeof(int));
  res[0] = queue[0][0];
  res[1] = queue[0][1];
  for (int i = 0; i < (*queue_len) - 1; i++) {
    queue[i][0] = queue[i + 1][0];
    queue[i][1] = queue[i + 1][1];
  }
  (*queue_len)--;
  return res;
}

int found(int (*arr)[2], int arr_len, int p1, int p2) {
  for (int i = 0; i < arr_len; i++) {
    if (arr[i][0] == p1 && arr[i][1] == p2) {
      return 1;
    }
  }
  return 0;
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
  int bot_len = 0;
  int bot_cap = 500;
  pos_velo *bots = malloc(bot_cap * sizeof(pos_velo));
  while (line != NULL) {
    int px, py, vx, vy;
    sscanf(line, "p=%d,%d v=%d,%d", &px, &py, &vx, &vy);
    if (bot_len == bot_cap) {
      bot_cap *= 2;
      bots = realloc(bots, bot_cap * sizeof(pos_velo));
    }
    pos_velo temp;
    temp.px = px;
    temp.py = py;
    temp.vx = vx;
    temp.vy = vy;
    bots[bot_len++] = temp;
    line = strtok(NULL, "\n");
  }

  const int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  int p2 = 10001;
  pos_velo *p1bots = malloc(bot_len * sizeof(pos_velo));
  for (int i = 1; i < 10000; i++) {
    if (i % 1000 == 0) {
      printf("%d\n", i);
    }
    char grid[103][101];
    memset(grid, '.', sizeof(grid));
    for (int j = 0; j < bot_len; j++) {
      pos_velo *temp = &bots[j];
      temp->px = (temp->px + temp->vx) % 101;
      if (temp->px < 0) {
        temp->px += 101;
      }
      temp->py = (temp->py + temp->vy) % 103;
      if (temp->py < 0) {
        temp->py += 103;
      }
      if (temp->py >= 0 && temp->py < 103 && temp->px >= 0 && temp->px < 101) {
        grid[temp->py][temp->px] = '#';
      }
    }

    int seen_len = 0;
    int seen_cap = 100;
    int (*seen)[2] = malloc(seen_cap * sizeof(int[2]));
    int count = 0;
    for (int j = 0; j < bot_len; j++) {
      pos_velo temp = bots[j];
      if (found(seen, seen_len, temp.px, temp.py)) {
        continue;
      }
      int new_count = 0;
      int queue_len = 1;
      int queue_cap = 50;
      int (*queue)[2] = malloc(queue_cap * sizeof(int[2]));
      queue[0][0] = temp.px;
      queue[0][1] = temp.py;
      while (queue_len > 0) {
        int *left = pop_left(queue, &queue_len);
        if (found(seen, seen_len, left[0], left[1])) {
          free(left);
          continue;
        }
        new_count++;
        if (seen_len == seen_cap) {
          seen_cap *= 2;
          seen = realloc(seen, seen_cap * sizeof(int[2]));
        }
        seen[seen_len][0] = left[0];
        seen[seen_len][1] = left[1];
        seen_len++;
        free(left);
        for (int d = 0; d < 4; d++) {
          int dx = dirs[d][0], dy = dirs[d][1];
          if (0 <= left[0] + dx && left[0] + dx < 101 && 0 <= left[1] + dy && left[1] + dy < 103 && grid[left[1] + dy][left[0] + dx] == '#') {
            if (queue_len == queue_cap) {
              queue_cap *= 2;
              queue = realloc(queue, queue_cap * sizeof(int[2]));
            }
            queue[queue_len][0] = left[0] + dx;
            queue[queue_len][1] = left[1] + dy;
            queue_len++;
          }
        }
      }
      free(queue);
      if (new_count > count) {
        count = new_count;
      }
    }
    if (count > 50) {
      if (i < p2) {
        p2 = i;
      }
      for (int j = 0; j < 103; j++) {
        for (int k = 0; k < 101; k++) {
          printf("%c", grid[j][k]);
        }
        printf("\n");
      }
    }
    if (i == 100) {
      memcpy(p1bots, bots, bot_len * sizeof(pos_velo));
    }
    free(seen);
  }

  int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
  for (int i = 0; i < bot_len; i++) {
    pos_velo temp = p1bots[i];
    int px = temp.px, py = temp.py;
    if (px < 50 && py < 51) {
      q1++;
    } else if (px > 50 && py < 51) {
      q2++;
    } else if (px > 50 && py > 51) {
      q3++;
    } else if (px < 50 && py > 51) {
      q4++;
    }
  }

  long long p1 = (long long)(q1 * q2 * q3 * q4);

  printf("%lld\n", p1);
  printf("%d\n", p2);

  free(buffer);
  free(bots);
  free(p1bots);

  return 0;
}
