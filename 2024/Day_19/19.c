#include <aoc_tools.h>

const int p = 31;
const int m = 1e9 + 9;
long long powers_of_p[63];

typedef struct HashMapEntry {
    char *key;
    long long value;
    struct HashMapEntry *next;
} HashMapEntry;

typedef struct {
    HashMapEntry **table;
    int size;
} HashMap;

void initialise_powers_of_p() {
  powers_of_p[0] = 1;
  for (int i = 1; i < 63; i++) {
    powers_of_p[i] = (powers_of_p[i - 1] * p) % m;
  }
}

long long hash(char *s) {
  long long hash_val = 0;
  int n = strlen(s);
  for (int i = 0; i < n; i++) {
    hash_val = (hash_val + (s[i] - 'a' + 1) * powers_of_p[i]) % m;
  }
  return hash_val;
}

HashMap *create_hashmap(int size) {
  HashMap *hashmap = malloc(sizeof(HashMap));
  hashmap->size = size;
  hashmap->table = malloc(size * sizeof(HashMapEntry *));
  for (int i = 0; i < size; i++) {
    hashmap->table[i] = NULL;
  }
  return hashmap;
}

HashMapEntry *hashmap_get(HashMap *hashmap, char *key) {
  long long hash_val = hash(key) % hashmap->size;
  HashMapEntry *entry = hashmap->table[hash_val];
  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      return entry;
    }
    entry = entry->next;
  }
  return NULL;
}

int hashmap_find(HashMap *hashmap, char *key) {
  long long hash_val = hash(key) % hashmap->size;
  HashMapEntry *entry = hashmap->table[hash_val];
  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      return 1;
    }
    entry = entry->next;
  }
  return 0;
}

void hashmap_insert(HashMap *hashmap, char *key, long long value) {
  long long hash_val = hash(key) % hashmap->size;
  HashMapEntry *new_entry = malloc(sizeof(HashMapEntry));
  new_entry->key = strdup(key);
  new_entry->value = value;
  new_entry->next = hashmap->table[hash_val];
  hashmap->table[hash_val] = new_entry;
}

void hashmap_free(HashMap *hashmap) {
  int size = hashmap->size;
  for (int i = 0; i < size; i++) {
    HashMapEntry *entry = hashmap->table[i];
    while (entry != NULL) {
      HashMapEntry *temp = entry;
      entry = entry->next;
      free(temp->key);
      free(temp);
    }
  }
  free(hashmap->table);
  free(hashmap);
}

long long solve(char *target, char **towels, int towels_len, HashMap *cache) {
  if (hashmap_find(cache, target)) {
    HashMapEntry *entry = hashmap_get(cache, target);
    return entry->value;
  }

  if (strlen(target) == 0) {
    return 1;
  }

  long long res = 0;
  for (int i = 0; i < towels_len; i++) {
    if (strncmp(target, towels[i], strlen(towels[i])) == 0) {
      res += solve(target + strlen(towels[i]), towels, towels_len, cache);
    }
  }
  hashmap_insert(cache, target, res);
  return res;
}

int main(int argc, char *argv[])
{
  char *buffer = read_file(argc, argv);
  if (buffer == NULL) {
    return 1;
  }

  char *towel = strtok(buffer, "\n\n");
  char *line = strtok(NULL, "\n");

  int towels_len = 0;
  int towels_cap = 100;
  char **towels = malloc(towels_cap * sizeof(char *));
  if (towels == NULL) {
    perror("Failed to allocate memory.");
    free(buffer);
    return 1;
  }

  char *context;
  char *towel_token = strtok_s(towel, ", ", &context);
  while (towel_token != NULL) {
    if (towels_len == towels_cap) {
      towels_cap *= 2;
      char **temp = realloc(towels, towels_cap * sizeof(char *));
      if (temp == NULL) {
        perror("Failed to allocate memory.");
        free(towels);
        free(buffer);
        return 1;
      }
      towels = temp;
    }
    towels[towels_len] = malloc((strlen(towel_token) + 1) * sizeof(char));
    if (towels[towels_len] == NULL) {
      perror("Failed to allocate memory for towel");
      for (int i = 0; i < towels_len; i++) {
          free(towels[i]);
      }
      free(towels);
      free(buffer);
      return 1;
    }
    strcpy(towels[towels_len], towel_token);
    towels_len++;
    towel_token = strtok_s(NULL, ", ", &context);
  }

  initialise_powers_of_p();
  HashMap *cache = create_hashmap(1000);

  int p1 = 0;
  double p2 = 0;
  while (line != NULL) {
    long long res = solve(line, towels, towels_len, cache);
    if (res == -1) {
      return 1;
    }
    if (res != 0) {
      p1++;
    }
    p2 += res;
    line = strtok(NULL, "\n");
  }

  printf("%d\n", p1);
  printf("%.0lf\n", p2);

  free(buffer);
  for (int i = 0; i < towels_len; i++) {
    free(towels[i]);
  }
  free(towels);
  hashmap_free(cache);

  return 0;
}
