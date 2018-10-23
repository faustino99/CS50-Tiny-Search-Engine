/*
 * hashtable.c - 'hashtable' module
 *
 * see hashtable.h for more information.
 *
 * code for this file was largely inspired by the bag module
 * various sections of this code copied/emulated portions of bag.c
 *
 * Faustino, April 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "set.h"
#include "jhash.h"
#include "hashtable.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
typedef struct hashtable
{
  const int *num_slots;     // number of 'slots' in the hashtable
  set_t **hash_sets;	      // list of set pointers, representing hashtable's slots
} hashtable_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see hashtable.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */

/**************** hashtable_new() ****************/
hashtable_t *hashtable_new(const int num_slots)
{
  hashtable_t *ht = malloc(sizeof(hashtable_t));

  if (ht == NULL) {
    return NULL; // error allocating hashtable
  }
  else {
    // initialize contents of the hashtable structure
    int *slots_cpy = malloc(sizeof(int));
    *slots_cpy = num_slots;
    ht->num_slots = slots_cpy;
    // initialize list of set pointers and initialize each set pointer
    ht->hash_sets = calloc(num_slots, sizeof(set_t *));
    for (int i = 0; i < num_slots; i++) {
      if ((ht->hash_sets[i] = set_new()) == NULL) {
        return NULL;
      }
    }
    return ht;
  }
}

/**************** hashtable_insert() ****************/
bool hashtable_insert(hashtable_t *ht, const char *key, void *item)
{
  if (ht != NULL && item != NULL && key != NULL) {
    int i = JenkinsHash(key, *(ht->num_slots));     // hash the key
    set_t *curr_set = ht->hash_sets[i];     // set at slot number provided by hash function
    if (set_find(curr_set, key) == NULL) {
      return set_insert(curr_set, key, item); // insert node into the set
    }
    else {
      return false;
    }
  }
  else {
    return false;
  }
}

/**************** hashtable_find() ****************/
void *hashtable_find(hashtable_t *ht, const char *key)
{
  if (ht == NULL || key == NULL) {
    return NULL; // hashtable or key is null
  } else {
    int i = JenkinsHash(key, *(ht->num_slots));
    set_t *curr_set = ht->hash_sets[i];     // go to set where key should be in
    return set_find(curr_set, key); // use set_find on given set to see if key exists
  }
}

/**************** hashtable_print() ****************/
void hashtable_print(hashtable_t *ht, FILE *fp,
                     void (*itemprint)(FILE *fp, const char *key, void *item))
{
  if (fp != NULL) {
    if (ht != NULL) {
      for (int i = 0; i < *(ht->num_slots); i++) {
        printf("%d->", i); // slot number of the hashtable
        set_print(ht->hash_sets[i], fp, itemprint); // print set using set_print
        printf("\n");
      }
    } else {
      fputs("(null)", fp);
    }
  }
}

/**************** hashtable_iterate() ****************/
void hashtable_iterate(hashtable_t *ht, void *arg,
               void (*itemfunc)(void *arg, const char *key, void *item) )
{
  if (ht != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (int i = 0; i < *(ht->num_slots); i++) {
      set_iterate(ht->hash_sets[i], arg, itemfunc);
    }
  }
}

/**************** hashtable_delete() ****************/
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item))
{
  if (ht != NULL) {
    for (int i = 0; i < *(ht->num_slots); i++) {
      set_delete(ht->hash_sets[i], itemdelete);   // free each set in hash_sets
    }
    free((int *)ht->num_slots);     // free the num_slots const int pointer
    free(ht->hash_sets);      // free the set array of the hashtable
    free(ht);                      // free the hashtable
  }
}
