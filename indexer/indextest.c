/*
 * indextest.c - 'indexer' module
 *
 * see README.md and IMPLEMENTATION.md for more information.
 *
 * Faustino Cortina – April 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "hashtable.h"
#include "counters.h"
#include "index.h"

void delete_counters (void *item);


int main(const int argc, char *argv[])
{
  // check correct number of arguments passed
  if (argc != 3) {
    fprintf(stderr, "usage: ./indextest oldIndexFilename newIndexFilename\n");
    exit(1);
  }

  char *old_index_file = argv[1];
  char *new_index_file = argv[2];

  // make sure old_file is readable
  FILE *old_file;
  if ((old_file = fopen(old_index_file, "r")) == NULL) {
    fprintf(stderr, "%s is unreadable\n", old_index_file);
    exit(2);
  }

  // make sure new_file is writable
  FILE *new_file;
  if ((new_file = fopen(new_index_file, "w")) == NULL) {
    fprintf(stderr, "%s is unwriteable\n", new_index_file);
    exit(3);
  }

  // initialize hashtable representation of inverted index structure
  int num_lines = lines_in_file(old_file);
  hashtable_t *index = hashtable_new(num_lines);  // num_lines = number of words in old_file

  index_load(old_file, index);    // build 'index' from 'old_file' index file
  index_save(new_file, index);    // convert 'index' to text output in 'new_file'

  // cleanup: close files and free hashtable
  fclose(old_file);
  fclose(new_file);
  hashtable_delete(index, delete_counters);  // calls helper function delete_counters
}

// frees counters stored in hashtable using 'counters_delete'
void delete_counters (void *item)
{
  counters_delete((counters_t *)item);
}
