# CS50 Indexer
## Faustino Cortina, April 2018
##### Some files used in test cases were taken from the CS50 directory


### Testing Indexer

To test `indexer`, I created the bash executable `indexertest.sh`.

`indexertest.sh` does the following:

1. Tests `indexer` and `indextest` with various forms of incorrect command-line arguments to ensure that their command-line parsing, and validation of those parameters, work correctly. This includes validating the correct number of arguments are given, and ensuring that files are readable/writable before using them.

2. Test `indexer` with `crawler-output-small`, a small crawler output file with only 7 files in it. Save the resulting file as `file`

4. Prints the first 10 lines of `file` to show that it is not empty and in the correct format.

3. Put both `file` and `cs50_indexer_file` in alphabetical order using the `indexsort.awk` script so that they can be compared with one another. `cs50_indexer_file` is an indexed file of `crawler-output-small` that was provided in the `cs50` public directory as the correct output of `indexer`. The sorted `file` and `cs50_indexer_file` files are saved as `file.sorted` and `cs50_indexer_file.sorted`, respectively.

4. Use the `diff --brief` command to determine whether `file.sorted` and `cs50_indexer_file.sorted` are identical, which they should be. No output from the `diff` command means that the two file are the same.

5. Tests `indextest` by passing `file` as `oldIndexFilename` and `file2` as the `newIndexFilename` to be created from the information read in `file`

6. Puts `file2` in alphabetical order (saved as `file2.sorted`) so that it can be compared with `file`.

7. Checks that `indextest` worked correctly by running the `diff --brief` command on `file.sorted` and `file2.sorted`, which should indicate that the two files are identical.


### Test run

An example test run is in `indextest.out`, built with

	   make test > indextest.out 2>&1

In which the output of `indextest.sh` is directed into `indextest.out`.
