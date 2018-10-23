# TSE Indexer
##### Some descriptions of lab 5 used in the CS50 page were copied into this page

## About

`indexer` is a program that reads the documents in the `pageDirectory` output by the `crawler`, builds an inverted index mapping from words to documents, and writes that index to a file. (Later, the `querier` will read the index and use it to answer queries.)

## Functions

`indexer` uses the following functions:

1. *main* which in both `indexer` and `indextest` parse the arguments passed and initialize data structures
2. *index_build* used by `indexer` to build an inverted index by reading the files in `pageDirectory`
3.  *index_save* used by both functions to save an inverted index into a file in the format specified above in Inputs and Outputs.

### main

`main` first ensures that the command line arguments passed into the `indexer` executable are valid. If there aren't 2 arguments passed, the program exits with 1 status. If `indexFilename` is unwritable, the program exits with 2 status. If `pageDirectory` doesn't contain a `.crawler` file used by `crawler` to indicate it's a crawler output directory, then the program exits with 3 status.

`main` then creates a *hashtable* with 500 slots to accomodate a large quantity of words that would be expected from a large crawler output directory.

`main` then calls *index_build* which populates the *hashtable* with *counters* representing the frequency of words in the files in the crawler output directory `pageDirectory`.

`main` then calls *index_save* which converts the *hashtable* representation of the indexed words to a readable text output (see Inputs and Outputs section in [DESIGN.md](DESIGN.md) for a description of the output format). This text is saved in 'indexFilename'.

`main` finally cleans up the program by closing all files and deleting the heap space it initialized for *hashtable* with the help of the helper function `delete_counters` to delete the *counters* stored in the table.

### index_build

`index_build` is a part of the `common.a` library and is in the file `pagedir.c`, which specializes in logic relating to reading/writing to crawler output directories.

`index_build` does the following:

1. Starts by setting `id` to one
2. As long as `directory`/`id` is a valid file:
  1. Skip the first two lines with `readlinep` (non-HTML text)
  2. Store the rest of the file as a `char *` representing the page's html
  3. Create a `webpage` object representing this HTML (URL and depth parameters aren't important)
  4. While `webpage_getNextWord` returns a word:
    1. If the word is more than two characters, then make it lowercase and add that word instance to `index` by calling the helper function `updateIndex`

`updateIndex` helper function does the following:

1. If the given word doesn't exist as a key in `index`
  1. Create a new *counters* object
  2. Add `word` with `id` as a key to *counters* using `counters_add`
2. Otherwise, call `counters_add` on the existing counter with `id` as the key to increment its count by one

### index_save

`index_save` is a part of the `common.a` library and is in the file `index.c`, which specializes in logic relating to reading/writing to index output files.

`index_save` uses the *hashtable_iterate* function to have to *hashtable* `index` print lines of data to the file pointer `fp` for each *counters* object it holds.

`hashtable_iterate_func` is the iterate function used by `hashtable_iterate`. For each item, this function prints out its key, which is the word representing the *counters* that are stored in the key's `void *item`. In order to print the data represented by the *counters,* the `counters_iterate` function is called.
Finally a newline is created for the next item.

`counter_iterate_func` is the iterate function used by `counters_iterate`. For each item, this function prints out its key, which is the id of a page, followed by its count, which is the count of the frequency of a word occurring in the page represented by id. `hashtable_iterate_func` already printed out the word that this *counter* object is representing at the beginning of the line.

The resulting file should have output that looks like the following:

```
applicants 4 1
include 2 2 3 1 4 18 5 12 6 10 7 1
contributors 1 1
fame 4 1
esalen 4 1
eberhart 4 2
hop 3 1 4 5
universal 3 2 6 1
uses 2 6 3 14 4 10 5 3 6 6 7 1
members 2 15 3 1 4 12
```

## Important Data Structures

The data structures used for TSE indexer are described below. For more detailed information on the modules from the `libcs50.a` library, see the header files in `libcs50`.

### Webpage

The `webpage` module is a part of the `libcs50` library. In order to parse through a page's HTML and iterate through its words, the `webpage` module is used. The `webpage_getNextWord` method was especially useful in being able to extract words out of the HTML language.

### Pagedir

The `pagedir` module is a part of the `common` library and contains a method `index_build` to build an inverted index structure using a crawler output directory that is crucial to the implementation of `indexer.c`. Described in depth in [index_build](#index_build), this function goes through each file in a crawler output directory and indexes every instance of the words read into a *hashtable* inverted index structure.

### Hashtable

`Hashtable` is the same data structure used in lab 3 to store key-value pairs, and is a part of the `libcs50` library. The `hashtable` module is used by the indexer to store an inverted index structure of words. *hashtable* does this by holding *counters.* Counters can be grabbed from a *hashtable* using the `hashtable_find` method and can be added using the `hashtable_insert` method.

### Counters

`Counters` is the same data structure used in lab 3 to store key-value pairs of id's and counts. This data structure is perfect for indexing words since the TSE indexer needs to count how many times words occur in specific pages, which are referenced to by id. Each time a word is read in a file, the word can be added to the correct *counters* object using `counters_add.` If the counter already has an occurrence of the word in the specified id, then its count is simply incremented by one.

### File

`file` is a module within the `libcs50` library. This module is very useful for reading files. `readfilep` was used by `index_build` to read and save the HTML of a page file in a crawler output directory as a `char *` variable. `readwordp` was especially useful for `index_load` when scanning an index file, where each word can represent either a word, id, or count. By having the functionality to process words one by one, `index_load` was able to function much more effectively.

### Word

`word` is a module within the `common` library. It is very simple, having a single function, `NormalizeWord.` This function was used by `index_build` to convert all read words to lower-case, in order to ensure that the indexer output file is not case sensitive with its counts.
