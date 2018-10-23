# CS50 Crawler
## Faustino Cortina, April 2018
##### Some test cases ideas were copied from the CS50 lab 4 description


### Testing Crawler

To test `crawler`, I created the bash executable `crawlertest.sh`.

`crawlertest.sh` does the following:

1. Tests the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

2. Tests the crawler with a seedURL that points to a non-internal server.

3. Tests the crawler with a seedURL that points to an internal server, but a non-existent webpage (if not internal, URL will trigger non-internal error indicated in \#2 regardless of whether it is an existent webpage).

4. Crawls a simple, closed set of cross-linked web pages to crawl, <http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/>. This location has some page(s) that are mentioned multiple times within a page, links that are mentioned multiple times across the set of pages, and closed loops of pages.

5. Explores <http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/> at multiple depths and verifies that the files created match expectations.

6. Starts at different seed pages in that same site, <http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html> and <http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html>, and explores different depths.

7. Points the crawler at the complex CS50 Wikipedia playground, <http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia
/>. Explores at depths 0, 1, 2 (depth 2 may take a while).


### Test run

An example test run is in `crawlertest.out`, built with

	   `make test > crawlertest.out 2>&1`

In which the output of `crawlertest.sh` is directed into `crawlertest.out`.
