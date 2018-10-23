#TSE Querier
##### Some text used in the lab 4 design spec & lab 6 description on the CS50 page were copied into this page

## About

`querier` is a program that reads an index file produced by the `indexer` module, and page files produced by the `crawler` module, and answers search queries submitted via stdin.

## User interface

The indexer's only interface with the user is on the command-line; it must always have two arguments.

        `./querier pageDirectory indexFilename`

Where `pageDirectory` is a directory containing output from the `crawler` module, and `indexFilename` is the file output of the `indexer` module. For example:


## Inputs and Outputs

Input: The user can input search queries when prompted by the program. After a search query's results are outputted, the user can continuing inputting search queries into stdin, until the user inputs an `EOF` character when finished with the program.

Output: The program responds to user-inputted search queries by searching the index file for occurrences of the query. The resulting query results are ranked by word occurrence frequency and sorted from highest to lowest score. Each line outputted provides information for a single webpage in the following format.  

*  "score [score] doc [docID]: [URL]"
*  where score is the score of the page as computed by `querier`,
*  where docID is the ID of the page, as provided by the `crawler` module,
*  where URL is the url of the page, which can be found in pageDirectory,
*  where the score and docID columns are padded to be in line with one another

Within the file, the lines are ordered from highest to lowest score.

## Functional decomposition into modules

The following functions are excepted:

1. *main*, which parses arguments, reads `stdin`, calls the appropriate helper functions to parse, rank, and print the query results, and cleans up when the program is finished.

Various helper functions were used by *main*, and contain the bulk of the `querier` logic. The primary helper functions are the following:

1. *index_load* from the `index.c` file in the `common` library, which converts the `indexFilename` file into an inverted index data structure.
2. *num_tokens*, which returns the number of words inputted by the user into `stdin`. Function also checks that the user input only contains letters (returns -1 otherwise). This value is used by the following three methods to determine how many words to parse/print.
3. *tokenize_string*, which converts `stdin` input into an array of `char *` pointers representing the words inputted.
4. *print_query*, which prints the user's query back to the user, in all lowercase with each word separated by a single space.
5. *check_query*, which checks to make sure the user-inputted query is in the correct format and only contains letters. This method ensures that the operators "and" and "or" are not in the beginning or end of a query, and that they aren't ever next to each other.
6. *query_results*, which reads the query, and uses `indexFilename` to determine scores for how many occurrences of the query occur in each document. The query is assumed to be in Backus-Naur Form. The functions returns a `counters` data structure representing each non-zero score (`int count`) of a page ID (`const int key`).
7. *num_results*, which determines how many pages have non-zero scores and returns that number. This number is used by the following two methods, to determine how many scores to sort/print.
8. *sort_scores*, which sorts the scores in descending order and saves this into an array of `pagescore_t *` data structures.
9. *print_results*, which prints the sorted scores in the format specified in [Inputs and Outputs](Inputs and Outputs)

## Data Structures

`querier` uses the following data structures:

1. *hashtable* of *counters* storing an inverted index of word occurrences in different files
2. *counters* keeping track of the score of a page ID from a query
3. An array of *pagescore* data types, which allow for a ID-score pairs to be sorted in a set order

## Pseudo code for logic/algorithmic flow

`querier` runs as follows:

1. *main* checks that only 2 arguments are provided, that `pageDirectory` is a crawler output directory, and that `indexFilename` is readable.
2. *index_load* makes a *hashtable* to hold an inverted index of the words in `indexFilename`. Make sure *hashtable* is initialized with enough slots to efficiently store the quantity of words in `indexFilename`
3. As long as the user input read isn't `EOF`:
  1. Use *num_tokens* to get the number of word inputted:
  2. Parse the input with *tokenize_string*, and make sure the query is formatted correctly
  3. Call *print_query*, which prints the user's query back to the user, in all lowercase with each word separated by a single space.
  5. Use *check_query* to make sure the user-inputted query is in the correct format and only contains letters
  6. Use *query_results* to get the query scores for each page into a *counters* data structure
  7. Use *num_results* to get the number of non-zero scores obtained by *query_results*
  8. Use *sort_scores* to sort the scores in descending order and saves this into an array of `pagescore_t *` data structures.
  9. Use *print_results* to print these results into `stdout`
4. close all files and free memory allocated to data structures

## Testing Plan

*integration testing.* Assemble `querier` and write a bash script to test `querier`. This bash script should:

1. Test how `querier` handles invalid arguments
3. Test how `querier` handles invalid query inputs
4. Test `querier` on a crawler output directory and make sure the output of valid queries are in line with what is expected
5. Compare `querier` output with output generated by the cs50 `querier` example runs on test files count in `~cs50/data/tse-output`
6. Test `querier` with random inputs using fuzz-testing script `fuzzquery.c`

see [TESTING.md](TESTING.md) for more information
