# CS50 Crawler Module
## Faustino Cortina, April 2018
##### This file contains information taken from the CS50 lab 4 page

### Crawler

`Crawler` is a standalone program that crawls the web and retrieves webpages starting from a “seed” URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given “depth”.

### Usage

The `crawler` program can be executed by typing:

          ./crawler seedURL pageDirectory maxDepth

Where `seedURL` is the URL the crawler should begin at, `pageDirectory` is the directory where the crawled webpages should be stored, and `maxDepth` is the maximum amount of links away from the `seedURL` that the crawler can access. A depth of 0 means that only the `seedURL` can be examined by the crawler, a depth of 1 means all the links the `seedURL` references can be accessed by crawler, etc.

### Implementation

See [IMPLEMENTATION.md](IMPLEMENTAION.md) for a detailed explanation of how crawler is implemented.

### Assumptions

*  The max depth of the crawler must be in the range

*  Crawler is limited to internal URLs where the internal URL is <http://old-www.cs.dartmouth.edu>

*  Crawler is not responsible for cleaning a pageDirectory of files. Any file in pageDirectory with a name matching a webpage's ID will be overwritten. If such a file is unwritable, then this may result in an unexpected error since crawler assumes the entire directory is writable if it can add the file `.crawler` to it.

### Compilation

To build crawler, run `make` from crawler's `lab4` parent directory.

To clean up, run `make clean` from crawler's `lab4` parent directory or from the `crawler` directory.

To test, simply type `make test` from the `crawler` directory after compiling. See [TESTING.md](TESTING.md) for details of testing and an example test run.
