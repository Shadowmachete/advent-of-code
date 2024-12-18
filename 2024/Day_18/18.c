#include <aoc_tools.h>
#include <math.h>
#include <limits.h>

typedef struct {
  int r;
  int c;
} pos;

int compare(void *dist, const void *a, const void *b) {
  const int *posA = (const int *)a;
  const int *posB = (const int *)b;
  int *distArray = (int *)dist;

  int idxA = posA[0] * 71 + posA[1];
  int idxB = posB[0] * 71 + posB[1];

  if (distArray[idxA] < distArray[idxB]) return -1;
  if (distArray[idxA] > distArray[idxB]) return 1;
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

int *dijkstra(char G[71][71], int sr, int sc, int er, int ec) {
  int dirs[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  int *dist = malloc(5041 * sizeof(int));
  for (int i = 0; i < 5041; i++) {
    dist[i] = INT_MAX;
  }
  dist[sr * 71 + sc] = 0;
  int Q_len = 1;
  int Q_cap = 30;
  pos *Q = malloc(Q_cap * sizeof(pos));
  Q[0].r = sr;
  Q[0].c = sc;

  while (Q_len > 0) {
    qsort_s(Q, Q_len, sizeof(int[2]), compare, dist);
    pos curr = pop_left(Q, &Q_len);
    int r = curr.r, c = curr.c;
    int dis = dist[r * 71 + c];

    for (int i = 0; i < 4; i++) {
      int dr = dirs[i][0], dc = dirs[i][1];
      int nr = r + dr, nc = c + dc;
      if (nr < 0 || nr > 70 || nc < 0 || nc > 70 || G[nr][nc] == '#') {
        continue;
      }

      int new_dis = dis + 1;
      if (new_dis < dist[nr * 71 + nc]) {
        dist[nr * 71 + nc] = new_dis;
        if (Q_len == Q_cap) {
          Q_cap *= 2;
          Q = realloc(Q, Q_cap * sizeof(int[2]));
        }
        pos temp;
        temp.r = nr, temp.c = nc;
        Q[Q_len] = temp;
        Q_len++;
      }
    }
  }

  free(Q);
  return dist;
}

int main(int argc, char *argv[])
{
  char *buffer = read_file(argc, argv);
  if (buffer == NULL) {
    return 1;
  }

  char G[71][71];
  for (int i = 0; i < 71; i++) {
      for (int j = 0; j < 71; j++) {
          G[i][j] = '.';
      }
  }
  int i = 0;
  char *line = strtok(buffer, "\n");
  int lines_len = 0;
  int lines_cap = 500;
  char **lines = malloc(lines_cap * sizeof(char *));
  if (lines == NULL) {
    free(buffer);
    return 1;
  }

  while (line != NULL) {
    if (lines_len == lines_cap) {
      lines_cap *= 2;
      char **temp = realloc(lines, lines_cap * sizeof(char *));
      if (temp == NULL) {
        for (int j = 0; j < lines_len; j++) {
          free(lines[j]);
        }
        free(lines);
        free(buffer);
        return 1;
      }
      lines = temp;
    }
    lines[lines_len] = strdup(line);
    if (lines[lines_len] == NULL) {
      for (int j = 0; j < lines_len; j++) {
        free(lines[j]);
      }
      free(lines);
      free(buffer);
      return 1;
    }
    lines_len++;
    line = strtok(NULL, "\n");
  }

  while (i < 1024) {
    int x, y;
    sscanf(lines[i], "%d,%d", &x, &y);
    G[y][x] = '#';
    i++;
  }

  int sr = 0, sc = 0, er = 70, ec = 70;
  int p1;
  char p2[6];

  int *dist = dijkstra(G, sr, sc, er, ec);
  p1 = dist[er * 71 + ec];
  free(dist);

  int left = 1024, right = lines_len - 1;
  while (left <= right) {
    int mid = left + (right - left) / 2;
    int j = 1024;
    int x, y;
    while (j < mid) {
      sscanf(lines[j], "%d,%d", &x, &y);
      G[y][x] = '#';
      j++;
    }
    dist = dijkstra(G, sr, sc, er, ec);
    if (dist[er * 71 + ec] == INT_MAX) {
      right = mid - 1;
    } else {
      left = mid + 1;
    }
    free(dist);

    j = 1024;
    while (j < mid) {
      sscanf(lines[j], "%d,%d", &x, &y);
      G[y][x] = '.';
      j++;
    }
  }

  strcpy(p2, lines[right]);

  printf("%d\n", p1);
  printf("%s\n", p2);

  free(buffer);
  free(lines);

  return 0;
}
