/*
 * pagedir.h - header file for CS50 'bag' module
 *
 * 'pagedir' is used by the crawler module to save webpages it has scanned into
 * into a directory, where the first line is the webpage's URL, the second is
 * is the webpage's depth (relative to the crawler's seed URL), and the rest is
 * the webpages HTML
 *
 */


#include "webpage.h"
#include "hashtable.h"
#include <stdio.h>

// saves content of a webpage in a file called 'id' in the directory 'page_dir'
void page_saver(char *page_dir, webpage_t *webpage, int id);
void index_build(char *directory, hashtable_t *index);
FILE *crawler_test_file(char *base_dir);
