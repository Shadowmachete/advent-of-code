#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <math.h>

struct files {
  int pos;
  int size;
  int file_id;
};

struct dot {
  int pos;
  int size;
};

long long checksum(long *res, int resSize) {
  long long csum = 0;
  for (int i = 0; i < resSize; i++) {
    if (res[i] == -1) {
      continue;
    }
    csum += i * res[i];
  }
  return csum;
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
  int lineLen = strlen(line);
  int resCap = 10;
  long *res1 = malloc(resCap * sizeof(long));
  int resSize = 0;
  long fileId = 0;
  int pos = 0;
  int idsCap = 10;
  int idsLen = 0;
  struct files *fileIds = malloc(idsCap * sizeof(struct files));
  int dotsCap = 10;
  int dotsLen = 0;
  struct dot *dots = malloc(dotsCap * sizeof(struct dot));

  for (int i = 0; i < lineLen; i++) {
    int n = line[i] - '0';
    //printf("%d\n", n);
    if (resSize + n > resCap) {
      resCap *= 2;
      res1 = realloc(res1, resCap * sizeof(long));
    }

    if (i % 2 == 0) {
      if (idsLen == idsCap) {
        idsCap *= 2;
        fileIds = realloc(fileIds, idsCap * sizeof(struct files));
      }

      struct files temp;
      temp.pos = pos;
      temp.size = n;
      temp.file_id = fileId;
      fileIds[idsLen++] = temp;

      //printf("%d\n", fileId);
      for (int j = 0; j < n; j++) {
        res1[resSize++] = fileId;
        pos++;
      }
      fileId++;
    } else {
      if (dotsLen == dotsCap) {
        dotsCap *= 2;
        dots = realloc(dots, dotsCap * sizeof(struct dot));
      }

      struct dot temp;
      temp.pos = pos;
      temp.size = n;
      dots[dotsLen++] = temp;

      for (int j = 0; j < n; j++) {
        res1[resSize++] = -1;
        pos++;
      }
    }
  }

  long *res2 = malloc((resSize + 1) * sizeof(long));
  memcpy(res2, res1, resSize * sizeof(long));
  int resSize2 = resSize;

  for (int c = 0; c < resSize; c++) {
    if (res1[c] != -1) {
      continue;
    }
    for (int loc = resSize - 1; loc > -1; loc--) {
      int fileId = res1[loc];
      if (fileId != -1) {
        res1[loc] = -1;
        res1[c] = fileId;
        resSize--;
        break;
      } else {
        res1[loc] = -1;
        resSize--;
      }
    }
  }

  for (int i = idsLen - 1; i > -1; i--) {
    struct files *file_entry = &fileIds[i];
    int pos = file_entry->pos;
    int size = file_entry->size;
    int file_id = file_entry->file_id;

    for (int j = 0; j < dotsLen; j++) {
      struct dot *dot_entry = &dots[j];
      int dot_pos = dot_entry->pos;
      int dot_size = dot_entry->size;

      if (dot_pos < pos && size <= dot_size) {
        for (int k = 0; k < size; k++) {
          res2[pos + k] = -1;
          res2[dot_pos + k] = file_id;
        }
        struct dot new_dot_entry;
        new_dot_entry.pos = dot_pos + size;
        new_dot_entry.size = dot_size - size;
        dots[j] = new_dot_entry;
        break;
      }
    }
  }

  long long p1 = checksum(res1, resSize);
  long long p2 = checksum(res2, resSize2);
  printf("%lld\n", p1);
  printf("%lld\n", p2);

  free(buffer);
  free(res1);
  free(res2);
  free(fileIds);
  free(dots);

  return 0;
}
