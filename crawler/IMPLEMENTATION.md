# Lab 4: TSE Crawler
##### Some descriptions of lab 4 used in the CS50 page were copied into this page

## About

`crawler` is a standalone program that crawls the web and retrieves webpages starting from a “seed” URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given “depth”. Each retrieved webpage is stored in its own file in a directory.

## Functions

`crawler` has 4 main functions:

1. *main*, which parses arguments and initializes other modules
2. *crawler*, which loops over pages to explore, until the list is exhausted
3. *page_fetcher*, which fetches a page from a URL
4. *page_scanner*, which extracts URLs from a page and processes each one

### main

`main` first ensures that the command line arguments passed into the `crawler` executable are valid. If there are an incorrect number of arguments, the page directory is unwritable, the maximum depth is out of range/not a number, or the seed URL is non-internal/non-normalizable, then the program exits with an error code.

Main then initializes [bag](#Bag), [hashtable](#Hashtable), and [webpage](#Webpage) data structures to represent a list of `webpage` modules to process, a list of URLs that have been visited, and the seed URL at depth 0, respectively. The seed URL is added to the bag, and the `webpage *` representing the seed URL is added to the hashtable.

Main then calls crawler, which will return once it has completed its crawl.

Main finally cleans up the program by deleting the heap space it initialized for structures.

### crawler

As long as the bag of webpages to examine isn't empty, `crawler` continues the following loop:
1. A webpage is extracted from the bag of webpages to crawl
2. The program pauses for a second to prevent overloading a server with HTTP requests
3. *page_fetcher* is called to retrieve the HTML of the webpage and save it in the `webpage` data type. If the URL cannot be read, then skip to step 6
4. *page_saver* from the `common` library is called to write the contents of the webpage into a file with a unique ID in the page directory
5. If the webpage's depth is less than the max depth, explore the webpage to find links
  1. As long as page_scanner returns a new URL from the webpage:
    1. Normalize the URL and check to make sure it's internal (see [webpage](#Webpage) to see how this is done)
    2. If the URL is not a duplicate of another URL in the URLs visited hashtable, make a new webpage for that URL at depth+1 (relative to the webpage being examined at the moment)
    3. Free the heap memory allocated to the URL (if used by any other data structure, it was copied)
6. Free the heap memory allocated to the webpage after its been fully examined

### page_fetcher

`page_fetcher` uses webpage_fetch (see [webpage](#Webpage)) method to get the HTML of a `webpage` data structure's URL instance variable. If the method fails to access the URL provided, an error message is printed, but the program does not exits

### page_scanner

`page_scanner` uses the webpage_getNextURL (see [webpage](#Webpage)) method to scan a page's HTML (provided by a `webpage` data structure) at a given `int pos` and returns the next occurrence of a link in the code. If the link isn't internal or normalizable, then `page_scanner` prints a log message indicating this, and continues searching the page's HTML code until it finds a valid, internal URL link.

If a valid link is found, `page_scanner` saves the URL in `char *url` and returns an `int` representing a location on the webpage directly following the link reference. If a link is not found the function returns -1. The `int pos` variable is returned in order for `page_scanner` to keep track of where it has already scanned in the case that the function is called multiple times

## Data Structures

The data structures used for TSE crawler are described below. For more detailed information on the `bag` and `hashtable` interfaces, see the `libcs50` library.

### Webpage

In order to keep track of the URL's visited by the crawler and their corresponding HTML, the `webpage` module is used. The `webpage` module has the following internal structure:

```c
typedef struct webpage {
  char *url;                               // url of the page
  char *html;                              // html code of the page
  size_t html_len;                         // length of html code
  int depth;                               // depth of crawl
} webpage_t;
```

Where `char *url` and `char *html` represent a webpage's URL and HTML code, respectively, and `int depth` represents how many webpages are between the seed URL (where the crawler started) and the current URL. Each of these instance variables are only accessible outside the module through their getter methods. 'size_t html_len' is an internal parameter that cannot be accessed outside the module.

The webpage has the following functions accessible through `webpage.h`:

```c
/* getter methods */
int   webpage_getDepth(const webpage_t *page);
char *webpage_getURL(const webpage_t *page);
char *webpage_getHTML(const webpage_t *page);

/**************** webpage_new ****************/
/* Allocate and initialize a new webpage_t structure.
 * Do NOT fetch the html from url; instead, the
 * caller can fetch it later with webpage_fetch().
 * Parameters:
 *   url   must be non-null; its string is copied.
 *   depth must be non-negative.
 *   html  may be null; the pointer is copied, but the string is NOT copied.
 * Returns NULL on any error.
 */
webpage_t *webpage_new(char *url, const int depth, char *html);

/**************** webpage_delete ****************/
/* Delete a webpage_t structure created by webpage_new().
 * This function may be called from something like bag_delete().
 * This function calls free() on both the url and the html, if not NULL.
 */
void webpage_delete(void *data);

/***************** webpage_fetch ******************************/
/* retrieve HTML from page->url, store into page->html
 * @page: the webpage struct containing the url to fetch
 *
 * Returns true if the fetch was successful; otherwise, false.
 * If the fetch succeeded, page->html will contain the content retrieved.
 * Parameter 'page' should have been allocated by the caller,
 * but the page->html pointer is expected to be NULL.
 * If this function is successful, a new, null-terminated character
 * buffer will be allocated as page->html. The caller must later free this
 * memory, typically by calling webpage_delete().
 *
 * Assumptions:
 *     1. page has been allocated by caller
 *     2. page->url contains the url to fetch
 *     3. page->html is NULL at call time
 *
 * Returns:
 *     True: success; caller must later free html via webpage_delete(page).
 *     False: some error fetching page.
 *
 * Limitations:
 *   * can only handle http (not https or other schemes)
 *   * can only handle URLs of form http://host[:port][/pathname]
 *   * cannot handle redirects (HTTP 301 or 302 response codes)
 */
bool webpage_fetch(webpage_t *page);


/**************** webpage_getNextWord ***********************************/
/* return the next word from html[pos] into word
 * @page: pointer to the webpage info
 * @pos: current position in html buffer
 * @result: a pointer to character pointer, used to pass the word back out
 *
 * Returns the current position search so far in html; otherwise, returns < 0.
 * The page should already exist (not NULL), and contain non-NULL html.
 * The pos argument should be 0 on the initial call; the new pos is returned.
 * The result argument should be a NULL pointer.
 * On successful parse of html, result will contain a newly allocated character
 * buffer; may be NULL on failed return. The caller is responsible for free'ing
 * this memory.
 *
 * Side effect: the page's html will be compressed to remove white space.
 *
 * Memory contract:
 *     1. inbound, webpage points to an existing struct, with existing html;
 *     2. on return, *word points to malloc'd space
 *                   and the caller is responsible for freeing that space.
 */

int webpage_getNextWord(webpage_t *page, int pos, char **word);

/****************** webpage_getNextURL ***********************************/
/* return the next url from html[pos] into result
 * @page: pointer to the webpage info
 * @pos: current position in html buffer
 * @result: a pointer to character pointer, used to pass the url back out
 *
 * Returns the current position search so far in html; otherwise, returns < 0.
 * The page should already exist (not NULL), and contain non-NULL html.
 * The pos argument should be 0 on the initial call; the new pos is returned.
 * The result argument should be a NULL pointer.
 * On successful parse of html, result will contain a newly allocated character
 * buffer; may be NULL on failed return. The caller is responsible for free'ing
 * this memory.
 *
 * Side effect: the page's html will be compressed to remove white space.
 *
 * Memory contract:
 *     1. inbound, webpage points to an existing struct, with existing html;
 *     2. on return, *result points to malloc'd space
 *                   and the caller is responsible for freeing that space.
 */

int webpage_getNextURL(webpage_t *page, int pos, char **result);

/***********************************************************************
 * NormalizeURL - attempts to normalize the url
 * @url: absolute url to normalize
 *
 * Returns true on success;
 * returns false if the url can't be parsed or normalized.
 * returns false if the url refers to a file unlikely to contain html.
 */
bool NormalizeURL(char *url);


/***********************************************************************
 * IsInternalURL -
 * @url: absolute url to normalize and verify
 *
 * Normalizes URL as a side effect.
 * Returns true if the URL is valid and 'internal' to our domain,
 * returns false otherwise.
 *
 * "valid" means that NormalizeURL() returns true;
 * "internal" means that the url begins with INTERNAL_URL_PREFIX.
 */
bool IsInternalURL(char *url);

// All URLs beginning with this prefix are considered "internal"
static const
  char INTERNAL_URL_PREFIX[] = "http://old-www.cs.dartmouth.edu";
```

This suite of methods is used throughout `crawler` to read, clean, and scan URLs. `webpage_fetch` gets a URL's HTML code and stores it in its `char *html` instance variable. `webpage_getNextWord` scans a page's HTML for the next occurrence of a reference to a link from a given position in the HTML code (and updates the position variable past this reference). `NormalizeURL` cleans a URL so that it's in a clean, canonical form. `IsInternalURL` determines whether a URL begins with INTERNAL_URL_PREFIX, which for this lab is <http://old-www.cs.dartmouth.edu>.

### Pagedir

The `pagedir` module is a part of the `common` library and contains a single function to save the contents of a `webpage *` webpage data structure into a file called `int id` in the directory `char *page_dir`. This function is defined in 'pagedir.h' as follows:

```c
void page_saver(char *page_dir, webpage_t *webpage, int id);
```

In each file, the first line is the URL of the `webpage *` data structure. The second line is the depth of the `webpage *` structure (see [Webpage](#Webpage) for an explanation of the `depth` variable). The rest of the file is the HTML of the `webpage *` data structure.

If for some reason the page directory isn't writable (which shouldn't happen since `crawler` should have already tested writing in the directory), then an error message is printed and the function exits.

### Hashtable

`Hashtable` is the same data structure used in lab 3 to store key-value pairs, and is a part of the `libcs50` library. The `hashtable` module is used by the crawler to keep track of the URLs visited by the crawler in order to prevent the crawler from visiting the same site twice. Hashtable's `hashtable_insert` method is especially useful for this since it checks for duplicate keys. URLs are stored as `char *` keys, and each key's item is not used, so its value is not important.

### Bag

`Bag` is the same data structure used in lab 3 to store an unordered collection `void *` items, and is a part of the libcs50 library. The `bag` module is used by crawler to store `webpage *` items, representing webpages that the crawler has detected but has yet to save (to pageDirectory) and potentially scan (for URLs it points towards). The `bag_extract` method is especially useful for crawler in simultaneously since each time it takes a `webpage *` item from the bag, it also deletes it from the bag, preventing the same webpage from being examined twice. When `bag` is empty, then the crawler knows it has saved and scanned everything it needed to and stops crawling the web.
