#ifndef STR_MAP_H
#define STR_MAP_H

#include <stdlib.h>

typedef struct pair pair;
typedef struct str_map str_map;

struct str_map {
    unsigned int capacity;
	unsigned int count;
	pair * pairs;
};

str_map * sm_create(size_t capacity);
void sm_put(str_map * map, char * key, char * value);
char * sm_get(str_map * map, char * key);
int sm_exists(str_map * map, char * value);
void sm_destroy(str_map * map);
void sm_print(str_map * map);

#endif