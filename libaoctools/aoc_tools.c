#include "aoc_tools.h"
#include <libgen.h>

char *read_file(int argc, char *argv[]) {
  char *file;

  if (argc != 2) {
    char *program_name = argv[0];
    char *base_name = basename(program_name);
    char *dot_pos = strchr(base_name, '.');
    if (dot_pos != NULL) {
      *dot_pos = '\0';
    }
    int program_number = atoi(base_name);
    char *buff = malloc(20);
    if (buff == NULL) {
      perror("Memory allocation failed");
      return NULL;
    }
    snprintf(buff, sizeof(buff), "%d.txt", program_number);
    file = buff;
  } else {
    file = argv[1];
  }

  FILE *fileptr = fopen(file, "r");
  if (fileptr == NULL) {
    perror("File opening failed");
    if (argc != 2) {
      free(file);
    }
    return NULL;
  }

  fseek(fileptr, 0, SEEK_END);
  long fsize = ftell(fileptr);
  fseek(fileptr, 0, SEEK_SET);

  if (fsize < 0) {
    perror("Error determining file size");
    fclose(fileptr);
    if (argc != 2) {
      free(file);
    }
    return NULL;
  }

  char *buffer = malloc(fsize + 1);
  if (buffer == NULL) {
    perror("Memory allocation failed");
    fclose(fileptr);
    if (argc != 2) {
      free(file);
    }
    return NULL;
  }

  size_t total_read = 0;
  while (total_read < fsize) {
    size_t bytes_read = fread(buffer + total_read, 1, fsize - total_read, fileptr);
    if (bytes_read == 0) {
      if (ferror(fileptr)) {
        perror("File reading failed");
        fclose(fileptr);
        free(buffer);
        if (argc != 2) {
          free(file);
        }
        return NULL;
      }
      break;
    }
    total_read += bytes_read;
  }

  fclose(fileptr);
  buffer[fsize] = '\0';

  if (argc != 2) {
    free(file);
  }

  return buffer;
}
