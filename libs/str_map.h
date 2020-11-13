#ifndef STR_MAP_H
#define STR_MAP_H

#include <stdlib.h>

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


str_map * sm_create(size_t capacity);
void sm_put(str_map * map, const char * key, const char * value);
char * sm_get(str_map * map, const char * key);
int sm_exists(str_map * map, const char * value);
void sm_destroy(str_map * map);
void sm_print(str_map * map);
size_t sm_size(str_map * map);
char ** sm_get_keys(str_map * map);

#endif