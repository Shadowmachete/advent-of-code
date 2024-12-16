#include <aoc_tools.h>
#include <string.h>

int found(int (*coords)[2], int coords_len, int c1, int c2) {
  for (int i = 0; i < coords_len; i++) {
    if (coords[i][0] == c1 && coords[i][1] == c2) {
      return 1;
    }
  }
  return 0;
}

void add(int (**coords)[2], int *coords_len, int *coords_cap, int c1, int c2) {
  if ((*coords_len) == (*coords_cap)) {
    (*coords_cap) *= 2;
    (*coords) = realloc((*coords), (*coords_cap) * sizeof(int[2]));
  }
  (*coords)[*coords_len][0] = c1;
  (*coords)[*coords_len][1] = c2;
  (*coords_len)++;
  return;
}

void move(char (***G), int R, int C, char *movements, int sr, int sc) {
  int movement_len = strlen(movements);
  for (int move = 0; move < movement_len; move++) {
    if (movements[move] == '\n') {
      continue;
    }

    int dr = 0, dc = 0;

    switch (movements[move]) {
      case '^':
        dr = -1;
        dc = 0;
        break;
      case 'v':
        dr = 1;
        dc = 0;
        break;
      case '<':
        dr = 0;
        dc = -1;
        break;
      case '>':
        dr = 0;
        dc = 1;
        break;
    }

    int p2o_len = 1;
    int p2o_cap = 10;
    int (*p2o)[2] = malloc(p2o_cap * sizeof(int[2]));
    p2o[0][0] = sr;
    p2o[0][1] = sc;
    int i = 0;
    int collides = 0;
    while (i < p2o_len) {
      int r = p2o[i][0], c = p2o[i][1];
      int nr = r + dr, nc = c + dc;
      if ((*G)[nr][nc] == 'O' || (*G)[nr][nc] == '[' || (*G)[nr][nc] == ']') {
        if (!found(p2o, p2o_len, nr, nc)) {
          add(&p2o, &p2o_len, &p2o_cap, nr, nc);
        }
        if ((*G)[nr][nc] == '[' && !found(p2o, p2o_len, nr, nc + 1)) {
          add(&p2o, &p2o_len, &p2o_cap, nr, nc + 1);
        }
        if ((*G)[nr][nc] == ']' && !found(p2o, p2o_len, nr, nc - 1)) {
          add(&p2o, &p2o_len, &p2o_cap, nr, nc - 1);
        }
      } else if ((*G)[nr][nc] == '#') {
        collides = 1;
        break;
      }
      i++;
    }

    if (collides) {
      continue;
    }

    //char (*new_G)[C] = malloc(R * sizeof(char[C]));
    //for (int r = 0; r < R; r++) {
    //  for (int c = 0; c < C; c++) {
    //    new_G[r][c] = (*G)[r][c];
    //  }
    //}

    //for (int j = 0; j < p2o_len; j++) {
    //  int pr = p2o[j][0], pc = p2o[j][1];
    //  new_G[pr][pc] = '.';
    //}

    //for (int j = 0; j < p2o_len; j++) {
    //  int pr = p2o[j][0], pc = p2o[j][1];
    //  new_G[pr + dr][pc + dc] = (*G)[pr][pc];
    //}

    int characters_len = 0;
    int characters_cap = p2o_len;
    char *characters = malloc(characters_cap * sizeof(char));

    for (int j = 0; j < p2o_len; j++) {
      int pr = p2o[j][0], pc = p2o[j][1];
      if (characters_len == characters_cap) {
        characters_cap *= 2;
        characters = realloc(characters, characters_cap * sizeof(char));
      }
      characters[characters_len++] = (*G)[pr][pc];
      (*G)[pr][pc] = '.';
    }

    for (int j = 0; j < p2o_len; j++) {
      int pr = p2o[j][0], pc = p2o[j][1];
      (*G)[pr + dr][pc + dc] = characters[j];
    }

    //(*G) = new_G;

    sr += dr;
    sc += dc;

    free(p2o);
    free(characters);

  }
}

int main(int argc, char *argv[]) {
  char *buffer = read_file(argc, argv);
  if (buffer == NULL) {
    return 1;
  }

  char *buffer2 = strdup(buffer);
  char *line = strtok(buffer, "\n");
  int C = strlen(line);
  int R = 0;
  int capacity = 100;
  char **G = malloc(capacity * sizeof(*G));
  int movement_len = 0;
  while (line != NULL) {
    if (line[0] == '#') {
      if (R == capacity) {
        capacity *= 2;
        G = realloc(G, capacity * sizeof(*G));
      }
      G[R] = malloc((C + 1) * sizeof(char));
      strcpy(G[R++], line);
    } else {
      movement_len += strlen(line);
    }
    line = strtok(NULL, "\n");
  }

  char *movements = malloc(movement_len + 1);
  if (movements == NULL) {
    perror("Unable to allocate memory");
    free(buffer);
    free(G);
    return 1;
  }
  movements[0] = '\0';

  line = strtok(buffer2, "\n");
  while (line != NULL) {
    if (line[0] != '#') {
      strcat(movements, line);
    }
    line = strtok(NULL, "\n");
  }

  int R2 = R;
  int C2 = 2 * C;
  char **G2 = malloc(R2 * sizeof(*G2));
  for (int r = 0; r < R2; r++) {
    G2[r] = malloc((C2 + 1) * sizeof(char));
  }

  int sr = 0, sc = 0;
  int sr2 = 0, sc2 = 0;

  for (int r = 0; r < R; r++) {
    char row[C2];
    for (int c = 0; c < C; c++) {
      if (G[r][c] == '#') {
        row[2 * c] = '#';
        row[2 * c + 1] = '#';
      } else if (G[r][c] == '.') {
        row[2 * c] = '.';
        row[2 * c + 1] = '.';
      } else if (G[r][c] == '@') {
        sr = r;
        sc = c;

        sr2 = r;
        sc2 = 2 * c;

        row[2 * c] = '@';
        row[2 * c + 1] = '.';
      } else {
        row[2 * c] = '[';
        row[2 * c + 1] = ']';
      }
    }
    strcpy(G2[r], row);
  }

  move(&G, R, C, movements, sr, sc);
  move(&G2, R2, C2, movements, sr2, sc2);

  int p1 = 0, p2 = 0;
  for (int r = 0; r < R; r++) {
    for (int c = 0; c < C; c++) {
      if (G[r][c] == 'O') {
        p1 += 100 * r + c;
      }
    }
    for (int c = 0; c < C2; c++) {
      if (G2[r][c] == '[') {
        p2 += 100 * r + c;
      }
    }
  }

  printf("%d\n", p1);
  printf("%d\n", p2);

  free(G);
  free(G2);
  free(movements);
  free(buffer);

  return 0;
}
