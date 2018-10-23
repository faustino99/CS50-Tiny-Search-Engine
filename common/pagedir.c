/*
 * pagedir.c - 'pagedir' module
 *
 * see pagedir.h for more information.
 *
 * Faustino Cortina – April 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pagedir.h"
#include "webpage.h"
#include "memory.h"
#include "hashtable.h"
#include "file.h"
#include "word.h"
#include "counters.h"


static void updateIndex(hashtable_t *index, char* word, int id);
static int int_len(int i);
FILE *file_for_path(char *base_dir, int subfile);
FILE *crawler_output_file(char *base_dir, int subfile);

// saves a webpage in a file called 'id' in the directory 'page_dir'
void page_saver(char *page_dir, webpage_t *webpage, int id)
{
  FILE *fp;
  // calculate length of string [pagedir]/[id]
  int char_len = strlen(page_dir) + int_len(id) + 2;
  char dir_path[char_len];
  sprintf(dir_path, "%s/%d", page_dir, id); // create string "[pagedir]/[id]"
  // open file in dir_path for writing
  fp = fopen(dir_path, "w");
  assertp(fp, "page_saver cannot open file for writing"); // unexpected error; unwritable file

  fprintf(fp, "%s\n", webpage_getURL(webpage));   // first line is the URL
  fprintf(fp, "%d\n", webpage_getDepth(webpage)); // second line is the page's depth
  fprintf(fp, "%s\n", webpage_getHTML(webpage));  // rest of file is the HTML
  fclose(fp);
}

void index_build(char *directory, hashtable_t *index)
{
  int id = 1;
  FILE *crawler_file = crawler_output_file(directory, id);

  while (crawler_file != NULL) {
    free(readlinep(crawler_file));
    free(readlinep(crawler_file));
    char *html = readfilep(crawler_file);
    webpage_t *page = webpage_new("(null)", 0, html);

    int pos = 0;
    char *word;
    while ((pos = webpage_getNextWord(page, pos, &word)) > 0) {
      if (strlen(word) >2) {
        word = NormalizeWord(word);
        updateIndex(index, word, id);
      }
      free(word);
    }

    webpage_delete(page);

    id++;
    fclose(crawler_file);
    crawler_file = NULL;
    crawler_file = crawler_output_file(directory, id);
  }
}

static void updateIndex(hashtable_t *index, char* word, int id)
{
  counters_t *word_counters = hashtable_find(index, word);
  if (word_counters == NULL) {
    word_counters = counters_new();
    counters_add(word_counters, id);
    hashtable_insert(index, word, word_counters);
  }
  else {
    counters_add(word_counters, id);
  }
}

static int int_len(int i) {
  return floor(log10(abs(i))) + 1;
}

FILE *crawler_output_file(char *base_dir, int subfile) {
  int char_len = strlen(base_dir) + int_len(subfile) + 2;
  char page_filename[char_len];
  sprintf(page_filename, "%s/%d", base_dir, subfile); // create string "[pagedir]/.crawler"
  return fopen(page_filename,"r");
}

FILE *crawler_test_file(char *base_dir)
{
  int char_len = strlen(base_dir) + 10;
  char page_filename[char_len];
  sprintf(page_filename, "%s/.crawler", base_dir); // create string "[pagedir]/.crawler"
  return fopen(page_filename,"r");
}
