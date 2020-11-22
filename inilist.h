#ifndef __INILIST_T_H_
#define __INILIST_T_H_

#include "inistr.h"

#define inilist_iterator_t_foreach(item, itr)	for(item = itr->node->element; itr->count < itr->size; itr->node = itr->node->next, itr->count++)
#define inilist_foreach(item, list)	linked_node* __node;for(__node = list->first, item = __node->element; __node != NULL; __node = __node->next, item = __node != NULL ? __node->element : NULL)

typedef struct inilist_node inilist_node_t;
typedef struct inilist inilist_t;
typedef struct inilist_iterator inilist_iterator_t;
typedef struct inientry inientry_t;

struct inientry {
    inistr_t* key;
    inistr_t* value;
};

struct inilist_node {
	inientry_t* element;
	struct inilist_node* next;
};

struct inilist {
	inilist_node_t* first;
	inilist_node_t* last;
	int count;
};

struct inilist_iterator {
	inilist_node_t* node;
	int count;
	int size;
};

void inientry_free(inientry_t* entry);

inilist_t* inilist_create();
void inilist_free(inilist_t* list);

void inilist_add(inilist_t* const list, inientry_t* const element);
void inilist_add_at_first(inilist_t* const list, inientry_t* const element);
void inilist_add_at(inilist_t* const list, inientry_t* const element, int index);

inientry_t* inilist_remove_at(inilist_t* const list, int index);
inientry_t* inilist_remove_last(inilist_t* const list);
inientry_t* inilist_remove_first(inilist_t* const list);
bool inilist_remove_object(inilist_t* const list, inientry_t* element);

inientry_t* inilist_get_at(const inilist_t* const list, int index);
inientry_t* inilist_get_first(const inilist_t* const list);
inientry_t* inilist_get_last(const inilist_t* const list);
int inilist_index_of(const inilist_t* const list, inientry_t* data);


inilist_iterator_t* inilist_get_iterator(const inilist_t* const list);
void inilist_iterator_free(inilist_iterator_t* iterator);
int inilist_iterator_has_next(const inilist_iterator_t* const iterator);
inientry_t* inilist_iterator_get_next(inilist_iterator_t* const iterator);

#endif
