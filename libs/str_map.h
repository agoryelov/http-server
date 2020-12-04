#ifndef STR_MAP_H
#define STR_MAP_H

#include <stdlib.h>
#include <dc/stdlib.h>

typedef struct  {
    int is_occupied;
	char *key;
	char *value;
} pair;

typedef struct {
    unsigned int capacity;
	unsigned int count;
	pair * pairs;
    char ** keys;
} str_map;

/**
 * Creates a new string map with a minimum capacity.
 */
str_map * sm_create(size_t capacity);

/**
 * Adds the key-value pair to the specified string map. The strings
 * passed in for key and value are copied and original values are unchanged.
 */
void sm_put(str_map * map, char * key, char * value);

/**
 * Returns the memory address pointing to the beginning of the string
 * value for the specified key or NULL if the key does not exist.
 */
char * sm_get(str_map * map, char * key);

/**
 * Checks whether a key exists in the specified string map.
 */
int sm_exists(str_map * map, char * key);

/**
 * De-allocates memory associated with specified string map.
 */
void sm_destroy(str_map * map);

/**
 * Prints out the values in the specified string map.
 */
void sm_print(str_map * map);

/**
 * Returns the size of the specified string map.
 */
size_t sm_size(str_map * map);

/**
 * Returns a dynamic array of keys contained in the passed in
 * string map. Used to iterate through the values of this map.
 */
char ** sm_get_keys(str_map * map);

#endif