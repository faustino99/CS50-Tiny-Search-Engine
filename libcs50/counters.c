/*
 * counters.c - 'counters' module
 *
 * see counters.h for more information.
 *
 * code for this file was largely inspired by the bag module
 * various sections of this code copied/emulated portions of bag.c
 *
 *
 * Faustino Cortina, April 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "counters.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct counternode
{
  // key must be pointer; otherwise, const int type wouldn't be able to be assigned a number later on
  const int *key;
  int count;		      // counter
  struct counternode *next;	      // link to next node
} counternode_t;

/**************** global types ****************/
typedef struct counters
{
  struct counternode *head;	      // head of the list of items in counters
} counters_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see counters.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static counternode_t *counternode_new(const int key, int count);

/**************** counters_new() ****************/
counters_t *counters_new(void)
{
  counters_t *ctrs = malloc(sizeof(counters_t)); //initialize counters

  if (ctrs == NULL) {
    return NULL; // error allocating counters
  } else {
    // initialize contents of counters structure
    ctrs->head = NULL;
    return ctrs;
  }
}

/**************** counters_add() ****************/
void counters_add(counters_t *ctrs, const int key)
{
  if (ctrs != NULL && key >= 0) {
    int count;
    // check if key already exists
    if ((count = counters_get(ctrs, key)) == 0) {
      counternode_t *new = counternode_new(key, 1); // create node with value 1
      if (new != NULL) {
        // add it to the head of the list
        new->next = ctrs->head;
        ctrs->head = new;
      }
    }
    // increment existing key's counter by 1
    else {
      int incremented_count = count + 1;
      counters_set(ctrs, key, incremented_count);
    }
  }
}


/**************** counternode_new ****************/
/* Allocate and initialize a counternode; not visible outside this file */
static counternode_t * counternode_new(const int key, int count)
{
  counternode_t *node = malloc(sizeof(counternode_t));

  if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    int *key_cpy = (int*)malloc(sizeof(int)); // allocate heap space for key
    *key_cpy = (int)key;
    node->key = key_cpy;    // key is a pointer to a const int
    node->count = count;
    node->next = NULL;
    return node;
  }
}

/**************** counters_get() ****************/
int counters_get(counters_t *ctrs, const int key)
{
  if(ctrs != NULL) {
    counternode_t *curr_node = ctrs->head;
    bool key_exists = false;
    while (curr_node != NULL) {
      // check if node's key matches given key
      if (*(curr_node->key) == key) {
        key_exists = true;
        break;
      }
      curr_node = curr_node->next;
    }
    if(key_exists) {
      return curr_node->count; // return key's count
    }
    return 0;
  }
  else {
    return 0;
  }
}

/**************** counters_set() ****************/
void counters_set(counters_t *ctrs, const int key, int count)
{
  if (ctrs != NULL) {
    if (counters_get(ctrs, key) != 0) {
      // set the key's count to the given value
      for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
        if (*(node->key) == key) {
          node->count = count;
          break;
        }
      }
    }
    // if key doesn't exist, create new node with the given count
    else {
      counternode_t *node = counternode_new(key, count);
      node->next = ctrs->head;
      ctrs->head = node;
    }
  }
}

/**************** counters_print() ****************/
void counters_print(counters_t *ctrs, FILE *fp)
{
  if (fp != NULL) {
    if (ctrs != NULL) {
      fputc('{', fp);
      for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
      	// print this node
        fprintf(fp, "%d->%d,", *(node->key), node->count);
      }
      fprintf(fp, "}\n");
    } else {
      fputs("(null)\n", fp); // if null counters
    }
  }
}

/**************** counters_iterate() ****************/
void counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, int count) )
{
  if (ctrs != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, *(node->key), node->count);
    }
  }
}

/**************** counters_delete() ****************/
void counters_delete(counters_t *ctrs)
{
  if (ctrs != NULL) {
    for (counternode_t *node = ctrs->head; node != NULL; ) {
      counternode_t *next = node->next;
      free((char *)node->key);    // free the key
      free(node);			    // free the node
      node = next;
    }
    free(ctrs);     // free counters
  }
}
