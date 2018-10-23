#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "hashtable.h"
#include "counters.h"
#include "file.h"
#include "webpage.h"

/**/
static void addCounters(hashtable_t *index, char *word, int id, int count);
static void counter_iterate_func(void *arg, const int key, int count);
static void hashtable_iterate_func(void *arg, const char *key, void *item);

void index_save(FILE *fp, hashtable_t *index)
{
  hashtable_iterate(index, fp, hashtable_iterate_func);
}

void index_load(FILE *fp, hashtable_t *index)
{
  char *word = readwordp(fp);
  while (word != NULL) {
    char *id_input = readwordp(fp);
    while (id_input != NULL && !isalpha(id_input[0])) {
      char *count_input = readwordp(fp);
      int id;
      int count;
      sscanf(id_input, "%d", &id);
      sscanf(count_input, "%d", &count);
      addCounters(index, word, id, count);
      free(id_input);
      free(count_input);
      id_input = readwordp(fp);
    }
    free(word);
    word = id_input;
  }
}

static void addCounters(hashtable_t *index, char *word, int id, int count)
{
  counters_t *word_counters = hashtable_find(index, word);
  if (word_counters == NULL) {
    word_counters = counters_new();
    counters_set(word_counters, id, count);
    hashtable_insert(index, word, word_counters);
  }
  else {
    counters_set(word_counters, id, count);
  }
}

static void hashtable_iterate_func(void *arg, const char *key, void *item)
{
  fprintf((FILE *)arg, "%s", key);
  counters_iterate((counters_t *)item, arg, counter_iterate_func);
  fprintf((FILE *)arg, "\n");
}

static void counter_iterate_func(void *arg, const int key, int count)
{
  fprintf((FILE *)arg, " %d %d", key, count);
}
