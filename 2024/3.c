#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <ctype.h>

bool is_valid_format(const char *str) {
  if (strncmp(str, "mul(", 4) != 0) return false;

  int i = 4;

  while (isdigit(str[i])) {
    i++;
  }

  if (str[i] != ',') return false;

  i++;

  while (isdigit(str[i])) {
    i++;
  }

  if (str[i] != ')') return false;

  return true;
}

void check_do_dont(char *curr, char *next, bool *ok) {
  char *doo = strstr(curr, "do()");
  char *dont = strstr(curr, "don't()");
  if (next == NULL) return;

  if (doo != NULL && doo < next) {
    *ok = true;
  }
  if (dont != NULL && dont < next) {
    *ok = false;
  }
}

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

  char buffer[fsize + 1];
  size_t bytes_read = fread(buffer, 1, fsize, fileptr);

  if (bytes_read != fsize) {
    perror("File reading failed");
    fclose(fileptr);
    return 1;
  }

  fclose(fileptr);
  if (argc != 2) {
    free(file);
  }

  buffer[fsize] = '\0';
 
  int p1 = 0, p2 = 0;
  int val;
  char temp[13];
  char a[4], b[4];
  char *mul = strstr(buffer, "mul(");
  char *next_mul;
  bool ok = true;
  check_do_dont(buffer, mul, &ok);

  while (mul != NULL) {
    snprintf(temp, sizeof(temp), "%.12s", mul);
    if (is_valid_format(temp)) {
      sscanf(temp, "mul(%3[0-9],%3[0-9])", a, b);
      val = atoi(a) * atoi(b);
      p1 += val;
      if (ok) {
        p2 += val;
      }
    }

    next_mul = strstr(mul + 1, "mul(");

    check_do_dont(mul, next_mul, &ok);

    mul = next_mul;
  }
  
  printf("%d\n", p1);
  printf("%d\n", p2);

  return 0;
}
