#include <stdio.h>
#include <string.h>

#define LOAD_FACTOR 0.7

#include "str_map.h"

static unsigned long hash(const char *str);
static void pairs_destroy(pair * pairs, size_t pair_count);
static void sm_put_closed_hashing(str_map * map, char * key, char * value);
static void sm_set_pair(str_map * map, pair * pair, char * key, char * value);
static void sm_double_capacity(str_map * pair);
static void sm_swap(str_map * left, str_map * right);
static pair * sm_get_pair(str_map * map, char * key);

str_map * sm_create(size_t capacity) {
    str_map * map = malloc(sizeof(str_map));
    map->capacity = capacity;
    map->count = 0;
    map->pairs = malloc(map->capacity * sizeof(pair));
    map->keys = malloc(map->capacity * sizeof(char *));

    memset(map->pairs, 0, map->capacity * sizeof(pair));
    memset(map->keys, 0, map->capacity * sizeof(char *));

    return map;
}

void sm_put(str_map * map, const char * key, const char * value) {
    if (map == NULL) return;
    if (key == NULL) return;
    if (value == NULL) return;
    
    if (sm_exists(map, key)) {
        pair * existing_pair = sm_get_pair(map, key);

        size_t value_len = strlen(value);
        free(existing_pair->value);
        existing_pair->value = malloc(value_len + 1);
        strcpy(existing_pair->value, value);
        return;
    }

    double current_load = (double) map->count / map->capacity;
    if (current_load > LOAD_FACTOR) {
        sm_double_capacity(map);
    }

    sm_put_closed_hashing(map, key, value);
    return;
}

char * sm_get(str_map * map, const char * key) {
    pair * ppair = sm_get_pair(map, key);

    if (ppair == NULL) {
        return NULL;
    } else {
        return ppair->value;
    }
}

int sm_exists(str_map * map, const char * key) {
    
    if (map == NULL) {
        return 0;
    }

    if (key == NULL) {
        return 0;
    }
    
    if (sm_get_pair(map, key) == NULL) {
        return 0;
    }
    
    return 1;
}

size_t sm_size(str_map * map) {
    if (map == NULL) return 0;
    return map->count;
}

char ** sm_get_keys(str_map * map) {
    if (map == NULL) return NULL;
    return map->keys;
}

void sm_destroy(str_map * map) {
    if (map == NULL) return;

    for (size_t i = map->count; i < map->count; i++) {
        free(map->keys[i]);
    }
    free(map->keys);
    pairs_destroy(map->pairs, map->capacity);
    map->count = 0;
    map->capacity = 0;
    free(map);
}

void sm_print(str_map * map) {
    pair * pairs = map->pairs;
    for (size_t i = 0; i < map->capacity; i++) {
        printf("index: %ld key: %s value: %s\n", i, pairs[i].key, pairs[i].value);
    }
}

static pair * sm_get_pair(str_map * map, char * key) {
    pair * pairs = map->pairs;
    size_t hashed_index = hash(key) % map->capacity;
    size_t dest_index = hashed_index;

    if (pairs[dest_index].is_occupied == 0) {
        return NULL;
    }

    do {
        if (strcmp(pairs[dest_index].key, key) == 0) {
            return &(pairs[dest_index]);
        }
        dest_index = (dest_index + 1) % map->capacity;
    } while (pairs[dest_index].is_occupied != 0 && dest_index != hashed_index);

    return NULL;
}

static void sm_double_capacity(str_map * map) {
    str_map * temp_map = sm_create((map->capacity + 1) * 2);

    pair * curr_pairs = map->pairs;
    for (size_t i = 0; i < map->capacity; i++) {
        if (curr_pairs[i].is_occupied != 0) {
            sm_put_closed_hashing(temp_map, curr_pairs[i].key, curr_pairs[i].value);
        }
    }

    sm_swap(map, temp_map);
    sm_destroy(temp_map);
}

static void sm_swap(str_map * left, str_map * right) {
    size_t temp_capacity = left->capacity;
    size_t temp_count = left->count;
    pair * temp_pairs = left->pairs;
    char ** temp_keys = left->keys;

    left->capacity = right->capacity;
    left->count = right->count;
    left->pairs = right->pairs;
    left->keys = right->keys;

    right->capacity = temp_capacity;
    right->count = temp_count;
    right->pairs = temp_pairs;
    right->keys = temp_keys;
}

static void sm_put_closed_hashing(str_map * map, char * key, char * value) {
    size_t hashed_index = hash(key) % map->capacity;
    size_t dest_index = hashed_index;
    pair * pair_array = map->pairs;

    if (pair_array[dest_index].is_occupied == 0) {
        sm_set_pair(map, &pair_array[dest_index], key, value);
        return;
    } else {
        dest_index = (dest_index + 1) % map->capacity;
    }

    while (pair_array[dest_index].is_occupied != 0 && dest_index != hashed_index) {
        dest_index = (dest_index + 1) % map->capacity;
    }

    if (dest_index != hashed_index) {
        sm_set_pair(map, &pair_array[dest_index], key, value);
    }
}

static void sm_set_pair(str_map * map, pair * pair, char * key, char * value) {
    size_t key_len = strlen(key);
    size_t value_len = strlen(value);

    map->keys[map->count] = malloc(key_len + 1);
    pair->key = malloc(key_len + 1);
    pair->value = malloc(value_len + 1);

    strcpy(map->keys[map->count], key);
    strcpy(pair->key, key);
    strcpy(pair->value, value);

    pair->is_occupied = 1;

    map->count++;
}

static void pairs_destroy(pair * pairs, size_t pair_count) {
    if (pairs == NULL) return;
    
    for (size_t i = 0; i < pair_count; i++) {
        pairs[i].is_occupied = 0;
        free(pairs[i].key);
        free(pairs[i].value);
    }
    free(pairs);
}

static unsigned long hash(const char *str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}