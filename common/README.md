# CS50 Common Library
## Faustino Cortina, April 2018

### Common

`Common` is a library that represents functions that are interdisciplinary across TSE. In other words, the functions in this library are used by all the modules (crawler, indexer, and querier) in TSE. Serving mainly as a suite of helper methods, `common` helps organize code that is not specific to a single module.

### Overview

The `pagedir` module specializes in methods relating to reading/writing to a crawler output directory.

The `index` module specializes in methods relating to reading/writing to a index output file.

The `word` module contains a method for "normalizing" a word by turning it to lowercase

### Usage

The `common.a` library can be compiled by simply typing: `make`

The `common.a` library can be cleaned by simply typing `make clean`

### Assumptions

Most of the methods in `pagedir` and `index` are very specific to programs in other modules (ex: crawler, indexer, querier). Therefore, these methods may not do the most extensive error checking due to the assumption that many of these potential errors are already checked by the modules that are calling these methods, or due to the assumption by these modules that such errors should not occur.
