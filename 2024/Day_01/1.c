#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

typedef struct HashMapEntry {
  int key;
  int value;
  struct HashMapEntry *next;
} HashMapEntry;

unsigned int hash(int key) {
  return key % 100;
}

void hashmap_insert(HashMapEntry *map[], int key) {
  unsigned int index = hash(key);

  HashMapEntry *curr = map[index];
  while (curr != NULL) {
    if (curr->key == key) {
      curr->value++;
      return;
    }
    curr = curr->next;
  }

  HashMapEntry *new_entry = (HashMapEntry *)malloc(sizeof(HashMapEntry));
  new_entry->key = key;
  new_entry->value = 1;
  new_entry->next = map[index];
  map[index] = new_entry;
}

int hashmap_get(HashMapEntry *map[], int key) {
  unsigned int index = hash(key);

  HashMapEntry *curr = map[index];
  while (curr != NULL) {
    if (curr->key == key) {
      return curr->value;
    }
    curr = curr->next;
  }
  return 0;
}

void hashmap_free(HashMapEntry *map[]) {
  for (int i = 0; i < 100; i++) {
    HashMapEntry *curr = map[i];
    while (curr != NULL) {
      HashMapEntry *temp = curr;
      curr = curr->next;
      free(temp);
    }
  }
}

int compare(const void *a, const void *b) {
  return (*(int *)a - *(int *)b);
}

int main(int argc, char *argv[])
{
  char *file;
  char *program_name = argv[0];
  char *base_name = basename(program_name);
  char *dot_pos = strchr(base_name, '.');

  int *LEFT = NULL, *RIGHT = NULL;
  int l, r;
  int size = 0;

  HashMapEntry *RIGHTCOUNT[100] = { NULL };

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
  while (line != NULL) {
    if (sscanf(line, "%d   %d", &l, &r) == 2) {
      LEFT = (int *)realloc(LEFT, (size + 1) * sizeof(int));
      RIGHT = (int *)realloc(RIGHT, (size + 1) * sizeof(int));

      if (LEFT == NULL || RIGHT == NULL) {
        perror("Memory reallocation failed");
        free(LEFT);
        free(RIGHT);
        return 1;
      }

      LEFT[size] = l;
      RIGHT[size] = r;
      hashmap_insert(RIGHTCOUNT, r);

      size++;
    }

    line = strtok(NULL, "\n");

  }
 
  int p1 = 0, p2 = 0;
  qsort(LEFT, size, sizeof(int), compare);
  qsort(RIGHT, size, sizeof(int), compare);

  for (int i = 0; i < size; i++) {
    l = LEFT[i];
    r = RIGHT[i];
    p1 += abs(l-r);
  }

  printf("%d\n", p1);
  
  for (int i = 0; i < size; i++) {
    l = LEFT[i];
    p2 += l * hashmap_get(RIGHTCOUNT, l);
  }

  printf("%d\n", p2);

  free(LEFT);
  free(RIGHT);
  free(buffer);

  return 0;
}
