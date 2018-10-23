/*
 * set.c - 'set' module
 *
 * see set.h for more information.
 *
 * code for this file was largely inspired by the bag module
 * various sections of this code copied/emulated portions of bag.c
 *
 * Faustino, April 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct set
{
  struct setnode *head;	      // head of the list of items in set
} set_t;

/**************** local types ****************/
typedef struct setnode
{
  const char *key;		      // key string representing item
  void *item;		      // pointer to item for setnode
  struct setnode *next;	      // link to next node
} setnode_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static setnode_t *setnode_new(const char *key, void *item);

/**************** set_new() ****************/
set_t *set_new(void)
{
  set_t *set = malloc(sizeof(set_t));

  if (set == NULL) {
    return NULL; // error allocating set
  }
  else {
    // initialize contents of set structure
    set->head = NULL;
    return set;
  }
}

/**************** set_insert() ****************/
bool set_insert(set_t *set, const char *key, void *item)
{
  if (set != NULL && item != NULL && key != NULL) {
    // allocate a new node to be added to the list
    setnode_t *new;
    if ((set_find(set, key)) == NULL) {
      if ((new = setnode_new(key, item)) != NULL) {
        // add it to the head of the list
        new->next = set->head;
        set->head = new;
        return true;
      }
      return false;
    }
    else {
      return false;
    }
  }
  else {
    return false;
  }
}

/**************** setnode_new ****************/
/* Allocate and initialize a setnode */
static setnode_t *setnode_new(const char *key, void *item)
{
  setnode_t *node = malloc(sizeof(setnode_t));

  if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    // add item and key to new setnode
    char *key_cpy = (char*)malloc(strlen(key)+1);
    strcpy(key_cpy, key); // copy key provided
    node->key = key_cpy;
    node->item = item;
    node->next = NULL;
    return node;
  }
}

/****************set_find() ****************/
void *set_find(set_t *set, const char *key)
{
  if (set == NULL || key == NULL) {
    return NULL; // set or key is null
  } else {
    // Search through list for key
    setnode_t *curr_node = set->head;
    bool key_exists = false;
    while (curr_node != NULL) {
      // compare the setnode's key with provided key
      if (strcmp(curr_node->key, key) == 0) {
        key_exists = true;
        break;
      }
      curr_node = curr_node->next;
    }
    // return key's item if key exists
    if (key_exists) {
      return curr_node->item;
    }
    return NULL;
  }
}

/**************** set_print() ****************/
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item) )
{
  if (fp != NULL) {
    if (set != NULL) {
      fputc('{', fp);
      for (setnode_t *node = set->head; node != NULL; node = node->next) {
        // print this node
        if (itemprint != NULL) {  // print the node's item
          (*itemprint)(fp, node->key, node->item);
          fputc(',', fp);
        }
      }
      fputc('}', fp);
    } else {
      fputs("(null)", fp);
    }
  }
}

/**************** set_iterate() ****************/
void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) )
{
  if (set != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (setnode_t *node = set->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->item);
    }
  }
}

/**************** set_delete() ****************/
void set_delete(set_t *set, void (*itemdelete)(void *item) )
{
  if (set != NULL) {
    for (setnode_t *node = set->head; node != NULL; ) {
      if (itemdelete != NULL) {             // if possible...
        (*itemdelete)(node->item);          // delete node's item
      }
      free((char*) node->key);                // free the key
      setnode_t *next = node->next;
      free(node);                     // free the node
      node = next;
    }
    free(set);                        // free the set
  }
}
