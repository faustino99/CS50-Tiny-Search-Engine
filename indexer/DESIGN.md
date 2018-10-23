#TSE Indexer
##### Some text used in the lab 4 design spec & lab 5 description on the CS50 page were copied into this page

## User interface

The indexer's only interface with the user is on the command-line; it must always have two arguments.

        ./indexer pageDirectory indexFilename

For example:

        ./indexer crawler-output file

## Inputs and Outputs

Input: the only inputs are command-line parameters; see the User Interface above.

Output: the content stored in `pageDirectory` is read and indexed according to word frequency in each page, and is saved into `indexFilename`. Each line provides the word and one or more (docID, count) pairs, in the format
*  word docID count [docID count]…
*  where word is a string of lower-case letters,
*  where docID is a positive non-zero integer,
*  where count is a positive non-zero integer,
*  where the word and integers are separated by spaces.

Within the file, the lines may be in any order, and the docIDs may be in any order.

## Functional decomposition into modules

The following functions are excepted:

1. *main* functions for `indexer`, which parses arguments and initializes other modules, calls the methods below, and frees up all used memory when the programs finish
2. *index_build* builds an inverted index by reading the files in `pageDirectory`; inverted index is stored in a *hashtable* of *counters*
4. *index_save* saves an inverted index into a file in the format specified above in Inputs and Outputs.

And some helper modules that provide data structures:

1. *counters* tracking how often a word occurs in a page
2. *hashtable* of *counters* storing an inverted index of word occurrences in different files

## Pseudo code for logic/algorithmic flow

`indexer` runs as follows:

1. Checks only 2 arguments are provided, that `pageDirectory` is a crawler output directory, and that `indexFilename` is writable.
2. Make a *hashtable* to hold an inverted index of the words in `pageDirectory`. Make sure *hashtable* is initialized with enough slots to efficiently store the quantity of words one would expect to parse through in `pageDirectory`
3. Use *index_build* to read the files in `pageDirectory`, normalize each word read, and, if the word is greater than two characters, store it in the *hashtable* inverted index
4. Use *index_save* to represent the *hashtable* inverted index in a file `indexFilename` in the format specified in [Inputs and Outputs](Inputs and Outputs)

## Dataflow through modules

`indexer`:
1. *main* parses parameters and checks for errors
2. *main* passes the crawler output directory `pageDirectory` to *index_build*
3. The inverted index returned by *index_build* is passed to *index_save* along with *indexFilename* to save the inverted index into the file *indexFilename*

## Major data structures
Three helper modules provide data structures:

*counters* of word occurrences (id, count) of a word in different files
*set* of *counters* (indirectly used by *hashtable*)
*hashtable* of *counters*

## Testing Plan

*integration testing.* Assemble `indexer` and `indextest` and write a bash script to test both programs. This bash script should:

1. Test how both programs handle invalid arguments
2. Test `indexer` on a crawler output directory and make sure the output file is in the correct formatted
3. Check that this output file contains correct data by comparing it to an output file that is known to be correct
4. Run `indextester` on the output file of `indexer` and make sure the output file of this program has the same content as the output file of `indexer`

see [TESTING.md](TESTING.md) for more information
