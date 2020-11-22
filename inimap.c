#include <stdlib.h>
#include "inimap.h"

inientry_t* list_contains_key(inilist_t* list, inistr_t* key,
        bool(*equal)(inistr_t* key1, inistr_t* key2)) {
	if (list == NULL || list->count == 0) {
		return NULL;
	}
	inilist_iterator_t* it = inilist_get_iterator(list);
    while (inilist_iterator_has_next(it)) {
        inientry_t* entry = (inilist_iterator_get_next(it));
        if (entry->key == key || (equal != NULL && (*equal)(entry->key, key))) {
            return entry;
        }
    }
    inilist_iterator_free(it);
    return NULL;
}

void inimap_rebuild(inimap_t* map) {
    int new_size = map->initial_capacity * 2;
    inilist_t** new_entry_list = (inilist_t**)malloc(sizeof(inilist_t*) * new_size);
    for (int i = 0; i < new_size; i++) {
        new_entry_list[i] = inilist_create();
    }
    inimap_iterator_t* it = inimap_iterator_create(map);
    while (inimap_iterator_has_next(it)) {
        inientry_t* entry = inimap_iterator_get_next(it);
        int m_hash_code = inistr_hashcode(entry->key);
        m_hash_code %= new_size;
        if (m_hash_code < 0)
            m_hash_code += new_size;
        inilist_add(new_entry_list[m_hash_code], entry);
    }
    inimap_iterator_free(it);
    for (int i = 0; i < map->initial_capacity; i++) {
        inilist_free(map->entry_list[i]);
    }
    free(map->entry_list);
    map->entry_list = new_entry_list;
    map->initial_capacity = new_size;
}

inimap_t* inimap_create() {
    inimap_t* re = (inimap_t*)malloc(sizeof(inimap_t));
    re->size = 0;
    re->load_factor = DEFAULT_LOAD_FACTOR;
    re->initial_capacity = DEFAULT_INITIAL_CAPACITY;
    re->entry_list = (inilist_t**)malloc(sizeof(inilist_t*) * re->initial_capacity);
    for (int i = 0; i < re->initial_capacity; i++) {
        re->entry_list[i] = inilist_create();
    }
    return re;
}

bool inimap_contains_key(inimap_t* const map, inistr_t* const key) {
    int m_hash_code = inistr_hashcode(key);
    m_hash_code %= map->initial_capacity;
    if (m_hash_code < 0)
        m_hash_code += map->initial_capacity;
    inientry_t* re = list_contains_key(map->entry_list[m_hash_code], key, inistr_equals);
    return re != NULL;
}

void inimap_put(inimap_t* const map, inistr_t* const key, inistr_t* const value) {
    inientry_t* et;
    int m_hash_code = inistr_hashcode(key);
    if (key == NULL)
    	return;
    m_hash_code %= map->initial_capacity;
    if (m_hash_code < 0)
        m_hash_code += map->initial_capacity;
    inientry_t* re = list_contains_key(map->entry_list[m_hash_code], key, inistr_equals);
    if (re == NULL) {
        et = malloc(sizeof(inientry_t));
        et->key = key;
        et->value = value;
        inilist_add(map->entry_list[m_hash_code], et);
        (map->size)++;
        if (map->size > map->initial_capacity* map->load_factor) {
            inimap_rebuild(map);
        }
    } else {
        inientry_free(re);
        re->key = key;
        re->value = value;
    }
}



inistr_t* inimap_get(inimap_t* const map, inistr_t* const key) {
    int m_hash_code = inistr_hashcode(key);
    m_hash_code %= map->initial_capacity;
    if (m_hash_code < 0)
        m_hash_code += map->initial_capacity;
      
    inientry_t* re = list_contains_key(map->entry_list[m_hash_code], key, inistr_equals);
    if (re == NULL) {
        return NULL;
    }
    return re->value;
}


int inimap_get_size(const inimap_t* const map) {
    return map->size;
}


inimap_iterator_t* inimap_iterator_create(inimap_t* const map) {
    inimap_iterator_t* re = (inimap_iterator_t*) malloc(
            sizeof(inimap_iterator_t));
    re->count = 0;
    re->index = 0;
    re->map = map;
    re->current = (map->entry_list[0])->first;

    return re;
}

void inimap_iterator_free(inimap_iterator_t* iterator) {
    free(iterator);
}


bool inimap_iterator_has_next(inimap_iterator_t* iterator) {
    return iterator->count < iterator->map->size;
}

inientry_t* inimap_iterator_get_next(inimap_iterator_t* iterator) {
    (iterator->count)++;
    while (!(iterator->current)) {
        (iterator->index)++;
        iterator->current = iterator->map->entry_list[iterator->index]->first;
    }
    inientry_t* re = (inientry_t*) iterator->current->element;
    iterator->current = iterator->current->next;
    return re;
}


bool inimap_remove(inimap_t* const map, inistr_t* const key) {
    int hash_code = inistr_hashcode(key);
    inientry_t* et;
    inientry_t* entry;
    inilist_iterator_t* it;
    bool re = false;
  
    hash_code %= map->initial_capacity;
    if (hash_code < 0)
        hash_code += map->initial_capacity;
    it = inilist_get_iterator(map->entry_list[hash_code]); 
    while (inilist_iterator_has_next(it)) {
        entry = (inilist_iterator_get_next(it));
        if (inistr_equals(entry->key, key)) {
            et = inilist_remove_at(map->entry_list[hash_code], it->count - 1);
            if (et != NULL) {
            	inientry_free(et);
            }
            re = true;
            (map->size)--;
            break;
        }
    }
    inilist_iterator_free(it);
    return re;
}

void inientry_freeall(inientry_t* et) {
    if (et) {
        inientry_free(et);
        free(et);
    }
}

void inimap_free(inimap_t *map) {
    inimap_output(map, inientry_freeall);
    for (int i = 0; i < map->initial_capacity; i++) {
        inilist_free(map->entry_list[i]);
    }
    free(map->entry_list);
    free(map);
}

void inimap_output(inimap_t *map, void(*pt)(inientry_t*)) {
    inimap_iterator_t* iterator = inimap_iterator_create(map);
    while (inimap_iterator_has_next(iterator)) {
        pt(inimap_iterator_get_next(iterator));
    }
    inimap_iterator_free(iterator);
}
