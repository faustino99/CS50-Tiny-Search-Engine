# CS50 Querier Module
## Faustino Cortina, April 2018
##### This file contains information taken from the CS50 lab 6 page

### Indexer

`querier` is a standalone program that reads the index file produced by the `indexer` module, and page files produced by the `querier` module, and answers search queries submitted via stdin.


### Usage

The `querier` program can be executed by typing:

          `./querier pageDirectory indexFilename`

Where `pageDirectory` is a crawler output directory, created with the `crawler` program, and `indexFilename` is the file where the indexed words taken from the `pageDirectory` are stored

### Implementation

See [IMPLEMENTATION.md](IMPLEMENTAION.md) for a detailed explanation of how indexer is implemented.

### Assumptions

* If the `pageDirectory` directory contains the `.crawler` file that the `crawler` program always creates upon successful execution, then it is assumed that crawler output files in `pageDirectory` have the correct output format that is expected of `crawler`

* Similarly, it is expected that a valid crawler-output `pageDirectory` contains files named 1, 2, 3, …, without gaps

* If readable, `indexFilename` is assumed to be a properly formatted index file created by `indexer`

### Compilation

To build `querier`, run `make` from the `querier` directory.

To clean up, run `make clean` from the `querier` directory.

To test, simply type `make test` from the `querier` directory after compiling. See [TESTING.md](TESTING.md) for details of testing and an example test run.
