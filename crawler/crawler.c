/*
 * crawler.c - 'crawler' module
 *
 * README.md and IMPLEMENTATION.md for more information.
 *
 * Faustino Cortina – April 2018
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "hashtable.h"
#include "bag.h"
#include "webpage.h"
#include "memory.h"
#include "pagedir.h"


/**** FUNCTION PROTOTYPES *****/
static void crawler(bag_t *unvisited_webpages, hashtable_t *visited_URLs,
            char *page_dir, int max_depth, char *null_item);
static bool page_fetcher(webpage_t *webpage);
static int pagescanner(webpage_t *webpage, int pos, char **url);
// log one word (1-9 chars) about a given url; taken from CS50 crawler lecture notes
inline static void logr(const char *word, const int depth, const char *url)
{
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}

/****** FUNCTIONS *******/
/* The main method parses the command line arguments, checks the arguments for
   errors, and initialize the modules needed for the rest of the program */
int main(const int argc, char *argv[])
{
  char *null_item = "(null)";   // doesn't matter what hashtable's item is

  // exits if three arguments aren't passed with ./crawler
  if (argc != 4) {
    fprintf(stderr, "usage: ./crawler seedURL pageDirectory maxDepth\n");
    exit(1);
  }

  char *seed_URL = argv[1];     // set first argument to seed url
  char *page_dir = argv[2];     // set second argument to page directory
  int max_depth;                // set third argument to max depth

  // exits if max_depth isn't a number
  if ((sscanf(argv[3], "%d", &max_depth)) != 1) {
    fprintf(stderr, "invalid maxDepth '%s'\n", argv[3]);
    exit(2);
  }

  // exits if max_depth isn't in the range [1:10]
  if (max_depth < 0 || max_depth > 10) {
    fprintf(stderr, "maxDepth '%d' must be in range [1:10]\n", max_depth);
    exit(3);
  }

  // exits if page_dir is an unwritable directory
  FILE *fp;
  char test_dir[strlen(page_dir)+1];
  strcpy(test_dir, page_dir);
  strcat(test_dir, "/.crawler"); // filepath [page_dir]/.crawler
  if ((fp = fopen(test_dir, "w")) == NULL) {    // try writing in test_dir filepath
    fprintf(stderr, "invalid or unwritable directory '%s'\n", page_dir);
    exit(4);
  }
  fclose(fp);

  // exits if the seed_URL is not normalizable
  if (!NormalizeURL(seed_URL)) {
    fprintf(stderr, "un-normalizable seedURL '%s'\n", seed_URL);
    exit(5);
  }

  // exits if the seed_URL is not internal
  if (!IsInternalURL(seed_URL)) {
    fprintf(stderr, "non-internal seedURL '%s'\n", seed_URL);
    exit(6);
  }

  /* initialize data structures needed for crawler program */
  // keeps track of webpages that haven't been crawled
  bag_t *unvisited_webpages = bag_new();
  // keeps track of URLs that have been visited
  hashtable_t *visited_URLs = hashtable_new(10);
  // webpage data structure representing seed_URL
  webpage_t *seed_webpage = webpage_new(seed_URL, 0, NULL);

  // make sure data structures were properly initialized
  char *message = "Failed to initialize data structures in main()";
  assertp(visited_URLs, message);
  assertp(unvisited_webpages, message);
  assertp(seed_webpage, message);

  // add seed_URL to visited_URLs and add seed_webpage to unvisited_webpages
  bag_insert(unvisited_webpages, seed_webpage);
  hashtable_insert(visited_URLs, seed_URL, null_item);

  // run the crawler function
  crawler(unvisited_webpages, visited_URLs, page_dir, max_depth, null_item);

  // when crawler finishes, free memory allocated ot the data structures
  bag_delete(unvisited_webpages, webpage_delete);
  hashtable_delete(visited_URLs, NULL);
}

/* saves each webpages in unvisited_webpages, and, if it's depth is less than
   the max_depth, scans the webpage for valid, internal URL references and adds
   these URL to the unvisited_webpages bag */
static void crawler(bag_t *unvisited_webpages, hashtable_t *visited_URLs,
            char *page_dir, int max_depth, char *null_item)
{
  webpage_t *webpage;
  int id = 1;
  // run until bag is empty
  while ((webpage = (webpage_t *)bag_extract(unvisited_webpages)) != NULL) {
    // if URL's HTML cannot be fetched, then ignore webpage
    if (page_fetcher(webpage)) {
      page_saver(page_dir, webpage, id++);    // save webpage information in page_dir
      // as long as the current webpage's depth < max_depth
      if (webpage_getDepth(webpage) < max_depth) {
        logr("Scanning", webpage_getDepth(webpage), webpage_getURL(webpage));
        char *url;
        int pos = 0;
        // run until webpage has been fully scanned for internal URL references
        while ((pos = pagescanner(webpage, pos, &url)) > 0) {
          // if a URL already visited, ignore it
          if (hashtable_insert(visited_URLs, url, null_item)) {
            // otherwise, create a new webpage for the URL and add it to unvisited_webpages
            logr("Added", webpage_getDepth(webpage), url);
            int new_depth = webpage_getDepth(webpage) + 1;
            webpage_t *new_page = webpage_new(url, new_depth, NULL);
            bag_insert(unvisited_webpages, new_page);
          }
          else {
            logr("IgnDplct", webpage_getDepth(webpage), url);
          }
          free(url);    // free space allocated for url
        }
      }
    }
    webpage_delete(webpage);    // delete webpage once program is done with it
  }
}

/* fetch the HTML of the webpage's URL and save it in the webpage data structure.
   return true if the webpage's HTML is successfully fetched, otherwise returns
   false. This function relies heavily on webpage_fetch of webpage.h. */
static bool page_fetcher(webpage_t *webpage)
{
  if (webpage_fetch(webpage)) {
    logr("Fetched", webpage_getDepth(webpage), webpage_getURL(webpage));
    return true;
  }
  else {
    logr("FAILED", webpage_getDepth(webpage), webpage_getURL(webpage));
    return false;
  }
}

/* scan a webpage's HTML for link referenes to URLs that are normalizable and
   internal. Return the position at the webpage right after the returned URL */
static int pagescanner(webpage_t *webpage, int pos, char **url)
{
  // while loop emulates example in wepage.h for webpage_getNextURL method
  while ((pos = webpage_getNextURL(webpage, pos, url)) > 0) {
    logr("Found", webpage_getDepth(webpage), *url);
    if (!NormalizeURL(*url)) {
      logr("UnNrmlzd", webpage_getDepth(webpage), *url);
    }
    else if (IsInternalURL(*url)) {
      return pos;
    }
    else {
      logr("IgnExtrn", webpage_getDepth(webpage), *url);
    }
    free(*url);
  }
  return -1;
}
