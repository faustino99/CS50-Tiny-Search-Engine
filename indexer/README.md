# CS50 Indexer Module
## Faustino Cortina, April 2018
##### This file contains information taken from the CS50 lab 5 page

### Indexer

`indexer` is a program that reads the documents in the `pageDirectory` output by the `crawler`, builds an inverted index mapping from words to documents, and writes that index to a file. (Later, the `querier` will read the index and use it to answer queries.)

`indextest` reads an index file `oldIndexFilename` created by the `indexer` program and uses its content to create an inverted index representation of the file. It then writes this index into the file `newIndexFilename`. If executed successfully, then `oldIndexFilename` and `newIndexFilename` should have the same content.

### Usage

The `indexer` program can be executed by typing:

          ./indexer pageDirectory indexFilename

Where `pageDirectory` is a crawler output directory, created with the `crawler` program, and `indexFilename` is the file where the indexed words taken from the `pageDirectory` are stored

The `indextest` program can be executed by typing:

          ./indextest oldIndexFilename newIndexFilename

Where `oldIndexFilename` is an index file created by `indexer`, and `newIndexFilename` is the new file created by the inverted index created from reading `oldIndexFilename`.

### Implementation

See [IMPLEMENTATION.md](IMPLEMENTAION.md) for a detailed explanation of how indexer is implemented.

### Assumptions

* If the `pageDirectory` directory contains the `.crawler` file that the `crawler` program always creates upon successful execution, then it is assumed that crawler output files in `pageDirectory` have the correct output format that is expected of `crawler`

* Similarly, it is expected that `pageDirectory` contains files named 1, 2, 3, …, without gaps

* If readable, `oldIndexFilename` is assumed to be a properly formatted index file created by `indexer`

* If `indexFilename` or `newIndexFilename` already exist and are writable, they will be overwritten

### Compilation

To build `indexer` and `indextest`, run `make` from the parent directory of `indexer`.

To clean up, run `make clean` from the parent directory of the `indexer` directory or from the `indexer` directory.

To test, simply type `make test` from the `indexer` directory after compiling. This test file uses `indextest`. See [TESTING.md](TESTING.md) for details of testing and an example test run.
