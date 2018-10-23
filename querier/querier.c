/*
 * querier.c - 'querier' module
 *
 * see README.md and IMPLEMENTATION.md for more information.
 *
 * Faustino Cortina – April 2018
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "file.h"
#include "pagedir.h"
#include "index.h"
#include "word.h"
#include "hashtable.h"
#include "counters.h"

/* Data Structures */
typedef struct pagescore {
  int doc_id;
  int score;
} pagescore_t;


/* Function Prototypes */
static char **tokenize_string(char* string, int num_words);
static int num_tokens(char* string);
static int check_query(char **words, int len);
static int isAndOr(char *str);
static void delete_counters (void *item);
static void print_query(char **words, int len);
static counters_t *query_results(char **words, int len, hashtable_t *index);
static void add_scores (void *arg, const int key, int count);
static void merge_scores (void *arg, const int key, int count);
static void clear_scores (void *arg, const int key, int count);
static int num_results (counters_t *page_scores);
static pagescore_t **sort_scores (counters_t *page_scores, int size);
static void increment_size (void *arg, const int key, int count);
static void create_sorted_array (void *arg, const int key, int count);
static void print_results(pagescore_t **sorted_scores, int size, char *page_directory);
static int int_len(int i);
static int column_len(pagescore_t **sorted_scores, int size);


int main(const int argc, char *argv[])
{
  // check that correct number of arguments were passed
  if (argc != 3) {
    fprintf(stderr, "usage: ./querier pageDirectory indexFilename\n");
    exit(1);
  }

  char *page_directory = argv[1];
  char *index_filename = argv[2];

  // check that page_directory is a crawler output directory
  FILE *test_file;
  if ((test_file = crawler_test_file(page_directory)) == NULL) {
    fprintf(stderr, "'%s' is not a valid crawler output directory\n", page_directory);
    exit(2);
  }
  fclose(test_file);

  // make sure that index_filename is readable
  FILE *index_file;
  if ((index_file = fopen(index_filename, "r")) == NULL) {
    fprintf(stderr, "%s is unreadable\n", index_filename);
    exit(3);
  }

  // initialize hashtable representation of inverted index structure
  int num_lines = lines_in_file(index_file);
  hashtable_t *index = hashtable_new(num_lines);  // num_lines = number of words in old_file

  index_load(index_file, index);    // build 'index' from 'old_file' index file
  fclose(index_file);

  // Prompt user for words as long as EOF isn't read
  char *line;
  printf("KEY WORDS:> ");
  while ((line = NormalizeWord(readlinep(stdin))) != NULL) { // make user input lowercase
    int len = num_tokens(line);   // get number of words inputted by user
    if (len != -1) {              // makes sure all characters are letters
      char **words = tokenize_string(line, len);  // convert input into an array of char pointers
      print_query(words, len);    // print space-separated, lowercase query of word
      // If there are no formatting errors
      if (check_query(words, len)) {
        // get query results in a counters data structure
        counters_t *page_scores = query_results(words, len, index);
        int size = num_results(page_scores);
        // sort the results by score in desending order
        pagescore_t **sorted_scores = sort_scores(page_scores, size);
        // print sorted results
        print_results(sorted_scores, size, page_directory);
        for (int i = 0; i < size; i++) {
          free(sorted_scores[i]);
        }
        free(sorted_scores);
        counters_delete(page_scores);
      }
      free(words);
    }
    free(line);
    printf("KEY WORDS:> ");
  }
  printf("\n");
  hashtable_delete(index, delete_counters);
}

// returns number of words in a string
static int num_tokens(char* string)
{
  char *word = string;
  int num_words = 0;
  // as long as word isn't at EOL
  while (*word != '\0') {
    if (isalpha(*word)) {
      word += 1;
      // if word encounters a character followed by a space, then increment num_words
      if (*word == '\0') {
        num_words += 1;
        break;
      }
      if (isspace(*word)) {
        num_words += 1;
        word += 1;
      }
    }
    // increment untill word is at a letter character
    else if (isspace(*word)) {
      word += 1;
    }
    // error if encountered a non-space and non-letter character
    else {
      fprintf(stderr, "Error: bad character '%c' in query\n", *word);
      return -1;
    }
  }
  return num_words;
}

// tokenize a string by returning an array of char pointers representing string
static char **tokenize_string(char* string, int num_words)
{
  char *word;
  char *rest;
  char **words;
  // if string is empty
  if (num_words == 0) {
    words = malloc(sizeof(char *));
    words[0] = "";
    return words;
  }
  words = calloc(num_words, sizeof(char *));
  word = string;
  // for each word in string
  for (int i = 0; i < num_words; i++) {
    // set word to next occurence of a letter
    while (!isalpha(*word)) {
      if (isspace(*word)) {
        word += 1;
      }
    }
    words[i] = word;  // set words[i] to where word is pointing to
    rest = word + 1;
    // set rest to next occurence of a space
    while (!isspace(*rest)) {
      if (*rest == '\0') {
        break;
      }
      if (isalpha(*rest)) {
        rest += 1;
      }
    }
    *rest = '\0';    // set rest to null character to ensure its a null terminating string
    word = rest + 1;
  }
  return words;
}

// checks to make sure there are no formatting error in the query
static int check_query(char **words, int len)
{
  // if empty string
  if (len == 0) {
    return 1;
  }
  // checks if "and" or "or" are the first or last word in the string
  char *first_word = words[0];
  char *last_word = words[len-1];
  if (isAndOr(first_word)) {
    fprintf(stderr, "Error: '%s' cannot be first\n", first_word);
    return 0;
  }
  if (isAndOr(last_word)) {
    fprintf(stderr, "Error: '%s' cannot be last\n", last_word);
    return 0;
  }
  // checks if "and" or "or" are ever next to one another in the string
  for (int i = 0; i < (len - 1); i++) {
    if (isAndOr(words[i])) {
      if (isAndOr(words[i+1])) {
        fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", words[i], words[i+1]);
        return 0;
      }
    }
  }
  return 1;
}

// returens true if str is "and" or "or"
static int isAndOr(char *str)
{
  return strcmp(str, "and") == 0 || strcmp(str, "or") == 0;
}

// helper function to delete counters in a hashtable structure
static void delete_counters(void *item)
{
  counters_delete((counters_t *)item);
}

// prints query in lowercase with single spacing
static void print_query(char **words, int len)
{
  printf("Query: ");
  // prints each word in 'words'
  for (int i = 0; i < len; i++) {
    printf("%s ", words[i]);
  }
  printf("\n");
}

// gets query results and returns them in a counters datatype
static counters_t *query_results(char **words, int len, hashtable_t *index)
{
  hashtable_t *scores_table = hashtable_new(2);
  counters_t *page_scores = counters_new();   // cumulative scores to be returned at end
  counters_t *and_score = counters_new();     // score of an additional word
  counters_t *or_score = counters_new();      // score of an and-sequence following "or" keyword
  counters_t *merged_scores = counters_new(); // temporary structure storing merged scores of two scores
  // add and_score and merged_score to hashtable, needed in a singe structure for later
  hashtable_insert(scores_table, "and_score", and_score);
  hashtable_insert(scores_table, "merged_scores", merged_scores);
  bool first_word = true;   // true if at the beginning of an and-sequence
  bool or_read = false;     // true if "or" has been read
  for (int i = 0; i < len; i++) {
    char *word = words[i];
    // if empty string
    if (word[0] == '\0') {
      break;
    }
    // if or read
    if (strcmp(word, "or") == 0) {
      // if or has been read in the past
      if (or_read) {
        counters_iterate(or_score, page_scores, add_scores);
        counters_iterate(or_score, or_score, clear_scores);
      }
      else {
        or_read = true;
      }
      first_word = true;    // begin new and-sequence
    }
    // if any other word (other than "and") is read
    else if (strcmp(word, "and") != 0 && strlen(word) > 2) {
      counters_t *word_counters;
      // if word occurs in any page
      if ((word_counters = (counters_t *)hashtable_find(index, word)) != NULL) {
        counters_t *scores;
        // if "or" has been read
        if (or_read) {
          scores = or_score;
        }
        else {
          scores = page_scores;
        }
        // if at the beginning of an and-sequence
        if (first_word) {
          counters_iterate(word_counters, scores, add_scores);
          first_word = false;
        } else {
          /* get score for current word and treat it as if it werefollowing an
          "and" keyword, so have this score merge with the cumulative
          score of the current and-sequence using 'merge_scores' */
          counters_iterate(word_counters, and_score, add_scores);
          counters_iterate(scores, scores_table, merge_scores);
          // clear counter by setting all counts to zero
          counters_iterate(page_scores, page_scores, clear_scores);
          counters_iterate(and_score, and_score, clear_scores);
          counters_iterate(merged_scores, page_scores, add_scores);
          counters_iterate(merged_scores, merged_scores, clear_scores);
        }
      }
    }
  }
  // if or read, combine scores of 'or_score' and 'page_score'
  if (or_read) {
    counters_iterate(or_score, page_scores, add_scores);
  }
  counters_delete(or_score);
  counters_delete(merged_scores);
  counters_delete(and_score);
  hashtable_delete(scores_table, NULL);
  return page_scores;
}

// helper function to query_results, adds score to a counter
static void add_scores (void *arg, const int key, int count)
{
  counters_t *page_scores = (counters_t *)arg;
  int curr_score;
  if ((curr_score = counters_get(page_scores, key)) == 0) {
    counters_set(page_scores, key, count);
  }
  else {
    counters_set(page_scores, key, curr_score + count);
  }
}

// helper function to query_results, sets counter counts to 0
static void clear_scores (void *arg, const int key, int count)
{
  counters_t *counters = (counters_t *)arg;
  counters_set(counters, key, 0);
}

// get size of a counters data structure
static int num_results (counters_t *page_scores)
{
  int size = 0;
  counters_iterate(page_scores, &size, increment_size);
  return size;
}

// helper function to query_results, gets union of two counters, and keeps the smaller value
static void merge_scores (void *arg, const int key, int count)
{
  hashtable_t *scores_table = (hashtable_t *)arg;
  // saves merged_scores in temporary counters structure 'merged_scores'
  counters_t *merged_scores = (counters_t *)hashtable_find(scores_table, "merged_scores");
  counters_t *additional_scores = (counters_t *)hashtable_find(scores_table, "and_score");

  int score;
  if ((score = counters_get(additional_scores, key)) != 0) {
    if (count < score) {
      counters_set(merged_scores, key, count);
    }
    else {
      counters_set(merged_scores, key, score);
    }
  }
}

// sort counts in a counters data structure in descending order
static pagescore_t **sort_scores (counters_t *page_scores, int size)
{
  pagescore_t **sorted_scores = calloc(size, sizeof(pagescore_t));
  // iterate through counters
  for (int i = 0; i < size; i++) {
    sorted_scores[i] = NULL;
  }
  // calls create_sorted_array helper method
  counters_iterate(page_scores, sorted_scores, create_sorted_array);
  return sorted_scores;
}

// add one to int variable 'arg', used to count size of a counter
static void increment_size (void *arg, const int key, int count)
{
  int *size = (int *)arg;
  if (count != 0) {
    (*size)++;
  }
}

// use insertion sort algorithmn to sort counters in descending order
static void create_sorted_array (void *arg, const int key, int count)
{
  if (count == 0) {
    return;
  }
  // pagescore datatype used for convenience to store data for each page
  pagescore_t **sorted_scores = (pagescore_t **)arg;
  pagescore_t *page_being_inserted = malloc(sizeof(pagescore_t));
  page_being_inserted->doc_id = key;
  page_being_inserted->score = count;
  int i = 0;
  // insertion sort algorithmn for a single item
  pagescore_t *curr_page = sorted_scores[i];
  while (curr_page != NULL) {
    if (page_being_inserted->score > curr_page->score) {
      sorted_scores[i] = page_being_inserted;
      page_being_inserted = curr_page;
    }
    curr_page = sorted_scores[++i];
  }
  sorted_scores[i] = page_being_inserted;
}

// print sorted query results in clean, aligned format
static void print_results(pagescore_t **sorted_scores, int size, char *page_directory)
{
  if (size != 0) {
    printf("Matches %d documents (ranked):\n", size);
    // get column lengths to make sure everything is properly aligned
    int score_column_len = int_len(sorted_scores[0]->score);
    int id_column_len = column_len(sorted_scores, size);
    // one line per page
    for (int i = 0; i < size; i++) {
      int score = sorted_scores[i]->score;
      int id = sorted_scores[i]->doc_id;
      // get url from page_directory/[id] (first line of the file)
      int char_len = strlen(page_directory) + int_len(id) + 2;
      char page_filename[char_len];
      sprintf(page_filename, "%s/%d", page_directory, id);
      FILE *page = fopen(page_filename,"r");
      char *url = readlinep(page);
      fclose(page);
      // print it all out
      printf("score %*d doc %*d: %s\n", score_column_len, score, id_column_len, id, url);
      free(url);
    }
  }
  // if query results is empty
  else {
    printf("No documents match\n");
  }
  printf("-----------------------------------------------\n");
}

// helper function gets length of an integer
static int int_len(int i)
{
  return floor(log10(abs(i))) + 1;
}

// helper function gets length of the the max doc_id value
static int column_len(pagescore_t **sorted_scores, int size) {
  int max_id = 1;
  for (int i = 0; i < size; i++) {
    int id = sorted_scores[i]->doc_id;
    if (id > max_id) {
      max_id = id;
    }
  }
  return int_len(max_id);
}
