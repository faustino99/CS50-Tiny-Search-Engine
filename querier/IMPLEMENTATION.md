# TSE Querier
##### Some descriptions of lab 6 used in the CS50 page were copied into this page

## About

`querier` is a program that reads an index file produced by the `indexer` module, and page files produced by the `crawler` module, and answers search queries submitted via stdin.

## Functions

`querier` uses the following functions:

1. *main*, which parses arguments, reads `stdin`, calls the appropriate helper functions to parse, rank, and print the query results, and cleans up when the program is finished.
2. *index_load* used by `querier` to build an inverted index by reading the files in `pageDirectory`
3. *num_tokens*, which returns the number of words inputted by the user into `stdin`.
4. *tokenize_string*, which converts `stdin` input into an array of `char *` pointers representing the words inputted.
5. *print_query*, which prints the user's query back to the user, in all lowercase with each word separated by a single space.
6. *check_query*, which checks to make sure the user-inputted query is in the correct format and only contains letters.
7. *query_results*, which reads the query, and uses `indexFilename` to determine scores for how many occurrences of the query occur in each document.
8. *num_results*, which determines how many pages have non-zero scores and returns that number.
9. *sort_scores*, which sorts the scores in descending order
10. *print_results*, which prints the sorted scores in the format specified in [Inputs and Outputs](Inputs and Outputs)

### main

`main` first ensures that the command line arguments passed into the `querier` executable are valid. If there aren't 2 arguments passed, the program exits with 1 status. If `pageDirectory` doesn't contain a `.crawler` file used by `crawler` to indicate it's a crawler output directory, then the program exits with 2 status. If `indexFilename` is unreadable, the program exits with 3 status.

`main` then creates a *hashtable* with the same number slots as there are lines in `indexFilename` to accommodate approximately one word per hashtable slot.

`main` then calls *index_load* which populates the *hashtable* with *counters* representing the frequency of words as indicated in the `indexFilename` file.

`main` then prompts the user to input a query and reads the user input, and converts the line to lowercase using `NormalizeWord`.

While the line read isn't an `EOF` character, `main` then calls the following helper functions. After each iteration, the data structures used are free'd:
  1. *num_tokens* gets the number of word inputted. If a character isn't a letter, then skip to step 10:
  2. Parse the input with *tokenize_string* and return an array of `char *` pointers to each word
  3. Call *print_query*, which prints the user's query back to the user, in all lowercase with each word separated by a single space.
  5. Use *check_query* to make sure the user-inputted query is in the correct format. If the query isn't formatted correctly, skip to step 10.
  6. Use *query_results* to get the query scores for each page into a *counters* data structure
  7. Use *num_results* to get the number of non-zero scores obtained by *query_results*
  8. Use *sort_scores* to sort the scores in descending order and saves this into an array of `pagescore_t *` data structures.
  9. Use *print_results* to print these results into `stdout`

`main` finally cleans up the program by closing all files and deleting the heap space it initialized for *hashtable* with the help of the helper function `delete_counters` to delete the *counters* stored in the table.

### index_load

`index_load` is a part of the `common.a` library and is in the file `index.c`, which specializes in logic relating to reading/writing to index files.

`index_load` does the following:

1. As long as word read by `readwordp` isn't the EOF:
  1. Set word read to the `char *word`
  2. As long as the next word read is an integer:
    1. Set the next two words read to `int id` and `int count` respectively
    2. Use the `addCounters` helper function to add the word-id-count group to the inverted index structure `hashtable_t *index`
  3. If word read isn't an integer, set it to `char *word` and repeat the loop

`addCounters` helper function does the following:

1. If the given word doesn't exist as a key in `index`
  1. Create a new *counters* object
  2. set `word` with `id` as a key and `count` as its count to *counters* using `counters_set`
2. Otherwise, call `counters_set` on the existing counter with `id` as the key and increment its count by `count`

### num_tokens

`num_tokens` determines how many words were inputted by the user.

`num_tokens` has the following logic flow:
1. Set `char *word` to the beginning of the string and `int num_words` to 0
2. As long as `word` isn't at the end of the line:
  1. If the `word` is pointing to a space, increment the pointer by one until it points to a letter (or EOL)
  2. If `word` points to a letter, increment the pointer by one, and if `word` now points to a space or EOL, increment `num_words` by 1
  3. If 'word' isn't a letter or a space, return an error message and a value of -1
3. return `num_tokens`

### tokenize_string

`tokenize_string` parses user input, and returns an array of char pointers `char **words` pointing to each word inputted by the user.

`tokenize_string` uses the following logic:
1. if `int num_words` argument passed is 0, then simply return an array of size one pointing to a null character
2. Repeat `num_words` times:
  1. Set `char *word` and `char *rest` to the beginning of the user input
  2. Increment `word` until it points to a letter, then set `rest` to point the the character right after `word`
  3. Increment `rest` until it points to a space character
  4. Set `*rest` to a null character
  5. Set `words[i]` to `word` where i is the current number iteration of the loop
6. return `words`

### print_query

`print_query` has the following logic:

1. Print "Query: "
2. Repeat `int len` times, where `len` is the number of words inputted by the user
  1. print `words[i]` followed by a space, where i is the number iteration of the current loop and `words` is an array of char pointers created by *tokenize_string*
3. Print a newline

### check_query

`check_query` returns 1 if it finds no errors with the format of the query, and 0 if it finds an error

`check_query` has the following logic flow:
1. If the `int len` of the user input was 0 return true
2. Get the first word of `char **words` (`words[0]`), and return an error if its "and" or "or"
3. get the last word of `char **words` (`words[len-1]`), and return an error if its "and" or "or"
4. Iterate through each word in `words` and return an error if one of the words is "and" or "or" and the next word in `words` is also "and" or "or"
5. Otherwise return true.

### query_results

`query_results` reads the query, and uses `indexFilename` to determine scores for how many occurrences of the query occur in each document. The query is assumed to be in Backus-Naur Form. The functions returns a `counters` data structure representing each non-zero score (`int count`) of a page ID (`const int key`).

`query_results` has the following flow:
1. initializes four `counters_t *` structures: `page_scores`, `or_score`, `and_score`, and `merged_scores`. The last two counters listed are put into a `hashtable_t *` structure `scores_table`
2. set `bool first_word` to true, which indicates that the program is at the beginning of an and-sequence, and set `bool or_read` to false.
2. repeat `int len` times, where len is the number of words in the query:
  1. if `words[i]` (where `char **words` is an array of words representing the query) is equal to "or", set `or_read` to true. If "or" has been read in the past, combine the score of the previous and-sequence following the first "or" with the and-sequence preceding the "or". Set `first_word` to true
  2. else if any word that isn't "and" is read
    1. get the scores for that word and store it in `and_score`
    2. combine the `and_score` with the cumulative score of the current and-sequence (stored in either `or_score` or `page_scores` depending on whether "or" has been read) according to Backus-Naur rules for handling "and" keywords.
3. if "or" has been read, combine `page_scores` with `or_score` and save the combined scores in `page_scores`
4. return `page_scores`

### sort_scores

Data flow:
1. Create an array of `pagescore_t *` data structures representing the query results stored in a `counters_t` structure. This array can create an ordered list of scores
2. repeat `int count` times, where count is the number of results from the query:
  1. set each `pagescore_t` structure to null
3. perform an insertion sort on the array of `pagescore_t` structures with each `pagescore_t` item representing a counter in the `counters_t` query result data structure

### print_results

`print_results` prints a sorted array of `pagescore_t` data structures, in a nicely formatted output (see "Inputs and Outputs" section of [DESIGN.md](DESIGN.md) for a description of this format)

`print_results` first gets the max length of the largest score and the largest id in order to make sure the output is padded just enough to be aligned neatly

`print_results` then iterates through each `pagescore_t` item and gets the id and score values it holds. It then reads the given id file from `page_dir` and reads the first line to get the url of the page. Finally, the methods prints the line in a nice format

## Important Data Structures an Modules

The data structures used for TSE querier are described below. For more detailed information on the modules from the `libcs50.a` library, see the header files in `libcs50`.

### pagescore

The `pagescore` data structure is used by `querier` to easily store the query score of a specific page. This data structure contains two instance variables: `int doc_id` and `int score`, which store a document ID and the document's score respectively

### Index

The `index` module is a part of the `common` library and contains a method `index_load` to build an inverted index structure using an index file. Described in depth in [index_load](#index_load), this function goes through each line in an index file and indexes every instance of the words read into a *hashtable* inverted index structure.

### Hashtable

`Hashtable` is the same data structure used in lab 3 to store key-value pairs, and is a part of the `libcs50` library. The `hashtable` module is used by the indexer to store an inverted index structure of words. *hashtable* does this by holding *counters.* Counters can be grabbed from a *hashtable* using the `hashtable_find` method and can be added using the `hashtable_insert` method.

### Counters

`Counters` is the same data structure used in lab 3 to store key-value pairs of id's and counts. This data structure is perfect for storing query results, where the page id can be stored as the key, and the page score can be stored as the count.

### File

`file` is a module within the `libcs50` library. This module is very useful for reading files. `readlinep` was used by the *main* function to read a line of user input. `readwordp` was especially useful for `index_load` when scanning an index file, where each word can represent either a word, id, or count. By having the functionality to process words one by one, `index_load` was able to function much more effectively.

### Word

`word` is a module within the `common` library. It is very simple, having a single function, `NormalizeWord.` This function was used by `querier` to convert a user-inputed query into lowercase
