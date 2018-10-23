/*
 * indexer.c - 'indexer' module
 *
 * see README.md and IMPLEMENTATION.md for more information.
 *
 * Faustino Cortina – April 2018
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "counters.h"
#include "index.h"
#include "pagedir.h"

void delete_counters (void *item);

int main(const int argc, char *argv[])
{
  // check that correct number of arguments were passed
  if (argc != 3) {
    fprintf(stderr, "usage: ./indexer pageDirectory indexFilename\n");
    exit(1);
  }

  char *page_directory = argv[1];
  char *index_filename = argv[2];

  // check that 'index_filename' is writable
  FILE *index_file;
  if ((index_file = fopen(index_filename, "w")) == NULL) {  // .crawler file not present
    fprintf(stderr, "unwritable file '%s'\n", index_filename);
    exit(2);
  }

  // check that page_directory is a crawler output directory
  FILE *test_file;
  if ((test_file = crawler_test_file(page_directory)) == NULL) {
    fprintf(stderr, "'%s' is not a valid crawler output directory\n", page_directory);
    exit(3);
  }
  fclose(test_file);
  
  // initialize hashtable representation of soon-to-be inverted index structure
  hashtable_t *index = hashtable_new(500);

  index_build(page_directory, index);   // build 'index' from 'page_directory' files
  index_save(index_file, index);        // convert 'index' to 'index_file' text output

  // clean up: close files and delete hashtable
  hashtable_delete(index, delete_counters);
  fclose(index_file);
}

// hashtable_delete helper method that deletes the 'counters' items within it
void delete_counters (void *item)
{
  counters_delete((counters_t *)item);
}
