# CS50 Indexer
## Faustino Cortina, April 2018
##### Some files used in test cases were taken from the CS50 directory


### Testing Querier

To test `querier`, I created the bash executable `queriertest.sh`.

`queriertest.sh` does the following:

1. Test `querier` with various forms of incorrect command-line arguments.

2. Test `querier` with `fuzzquery`, a fuzz tester provided by cs50 that tests `querier` with random queries. Use `wiki_index_file` and `cs50_wiki_crawler_output` as arguments.

3. Test querier with `cs50_crawler_output` and `cs50_index_file` as arguments, and pipe in queries through pre-written commands provided by the `testqueries` text file. Save this output into `queryresults`.

4. `cs50_querier_output` contains the output that `querier` should have outputted with the command in step 3 (This file was taken from the tse-output directory in the cs50 directory). Compare this file with `queryresults` by printing the two files to the console; their query outputs should be identical.


### Test run

An example test run is in `queriertest.out`, built with

	   make test > queriertest.out 2>&1

In which the output of `queriertest.sh` is directed into `queriertest.out`.
