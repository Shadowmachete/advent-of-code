#include <aoc_tools.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

char *run(long long registers[3], int *program, int program_len) {
  int i = 0;
  char combos[] = {'0', '1', '2', '3', 'A', 'B', 'C'};
  char *out = malloc(40);
  out[0] = '\0';
  while (i < program_len) {
    int opcode = program[i], operand = program[i + 1];
    char char_combo_operand = combos[operand];
    long long combo_operand;
    if (isdigit(char_combo_operand)) {
      combo_operand = char_combo_operand - '0';
    } else {
      combo_operand = registers[char_combo_operand - 'A'];
    }

    switch (opcode) {
    case 0:
      registers[0] /= pow(2, combo_operand);
      break;
    case 1:
      registers[1] ^= operand;
      break;
    case 2:
      registers[1] = combo_operand % 8;
      break;
    case 3:
      if (registers[0] != 0) {
          i = operand;
          continue;
      }
      break;
    case 4:
      registers[1] ^= registers[2];
      break;
    case 5:
      char buffer[4];
      snprintf(buffer, sizeof(buffer), "%d,", combo_operand % 8);
      strcat(out, buffer);
      break;
    case 6:
      registers[1] = registers[0] / pow(2, combo_operand);
      break;
    case 7:
      registers[2] = registers[0] / pow(2, combo_operand);
      break;
    }

    i += 2;
  }
  return out;
}

int main(int argc, char *argv[]) {
  char *buffer = read_file(argc, argv);
  if (buffer == NULL) {
    return 1;
  }

  long long registers[3] = { 0, 0, 0 };
  char *line = strtok(buffer, "\n");
  sscanf(line, "Register A: %lld", &registers[0]);
  line = strtok(NULL, "\n");
  sscanf(line, "Register B: %lld", &registers[1]);
  line = strtok(NULL, "\n");
  sscanf(line, "Register C: %lld", &registers[2]);
  line = strtok(NULL, "\n");
  int line_len = strlen(line);
  int program_len = 0;
  int program_cap = 20;
  int *program = malloc(program_cap * sizeof(int));
  for (int c = 0; c < line_len; c++) {
    if (isdigit(line[c])) {
      if (program_len == program_cap) {
        program_cap *= 2;
        program = realloc(program, program_cap * sizeof(int));
      }
      program[program_len++] = line[c] - '0';
    }
  }

  char *p1 = run(registers, program, program_len);
  p1[strlen(p1) - 1] = '\0';

  long long p2 = 0;
  for (int i = program_len - 1; i > -1; i--) {
    char *p = malloc(40);
    p[0] = '\0';
    for (int j = i; j < program_len; j++) {
      char buffer[4];
      snprintf(buffer, sizeof(buffer), "%d,", program[j]);
      strcat(p, buffer);
    }
    p[strlen(p) - 1] = '\0';
    while (1) {
      //printf("%lld\n", p2);
      registers[0] = p2;
      char *out = run(registers, program, program_len);
      out[strlen(out) - 1] = '\0';
      //printf("%s\n", out);
      if (strcmp(out, p) == 0) {
        if (i != 0) {
          p2 *= 8;
        }
        break;
      }
      p2++;
      free(out);
    }
  }

  printf("%s\n", p1);
  printf("%lld\n", p2);

  free(buffer);
  free(program);
  free(p1);

  return 0;
}
