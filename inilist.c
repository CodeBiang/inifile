#include <stdlib.h>
#include "inilist.h"

void inientry_free(inientry_t* entry) {
    if (entry->key) {
        inistr_clear(entry->key);
        free(entry->key);
        entry->key = NULL;
    }
    if (entry->value) {
        inistr_clear(entry->value);
        free(entry->value);
        entry->value = NULL;
    }
}

//创建链表
inilist_t* inilist_create() {
    inilist_t* re = (inilist_t*) malloc(sizeof(inilist_t));
    re->count = 0;
    re->first = NULL;
    re->last = NULL;
    return re;
}
 
//释放链表
void inilist_free(inilist_t* list) {
    inilist_node_t* p;
    while (list->first) {
        p = list->first->next;
        free(list->first);
        list->first = p;
    }
    free(list);
}
 
//插入在尾部
void inilist_add(inilist_t* const list, inientry_t* const data) {
    inilist_node_t* node = (inilist_node_t *) malloc(sizeof(inilist_node_t));
    node->element = data;
    node->next = NULL;
    if (list->count) {
        list->last->next = node;
        list->last = node;
    } else {
        list->first = node;
        list->last = node;
    }
    (list->count)++;
}
 
//插入在首部
void inilist_add_at_first(inilist_t* const list, inientry_t* const data) {
    inilist_node_t* node = (inilist_node_t *) malloc(sizeof(inilist_node_t));
    node->element = data;
    node->next = NULL;
 
    if (list->count) {
        node->next = list->first;
        list->first = node;
    } else
    {
        list->first = node;
        list->last = node;
    }
    (list->count)++;
}
 
//打印
void myListOutput(const inilist_t* const list, void(*pt)(const inientry_t* const)) {
    inilist_node_t* p = list->first;
    while (p) {
        (*pt)(p->element);
        p = p->next;
    }
}
 
//删除在尾部
inientry_t* inilist_remove_last(inilist_t* const list) {
    if (list->count == 1) {
        return inilist_remove_first(list);
    }
    inilist_node_t* p = list->first;
    while (p->next != list->last) {
        p = p->next;
    }
    inientry_t*re = list->last->element;
    free(list->last);
    p->next = NULL;
    list->last = p;
    (list->count)--;
    return re;
}
 
//删除在首部
inientry_t* inilist_remove_first(inilist_t* const list) {
    inilist_node_t *p = list->first;
    list->first = p->next;
    inientry_t* re = p->element;
    free(p);
    (list->count)--;
    if (list->count == 0) {
        list->last = NULL;
    }
    return re;
}
 
//插入
void inilist_add_at(inilist_t* const list, inientry_t* const data, int index) {
    if (index == 0) {
        inilist_add_at_first(list, data);
        return;
    }
    if (index == list->count) {
        inilist_add(list, data);
        return;
    }
    inilist_node_t* node = (inilist_node_t *) malloc(sizeof(inilist_node_t));
    node->element = data;
    node->next = NULL;
 
    inilist_node_t* p = list->first;
    for (int i = 0; i < index - 1; i++) {
        p = p->next;
    }
    node->next = p->next;
    p->next = node;
 
    (list->count)++;
}
 
//删除
inientry_t* inilist_remove_at(inilist_t* const list, int index) {
    if (index == 0) {
        return inilist_remove_first(list);
    }
    if (index == list->count - 1) {
        return inilist_remove_last(list);
    }
 
    inilist_node_t* p = list->first;
    for (int i = 0; i < index - 1; i++) {
        p = p->next;
    }
    inilist_node_t *tp = p->next;
    p->next = p->next->next;
    inientry_t* re = tp->element;
    free(tp);
    (list->count)--;
    return re;
}
 
//取得数据
inientry_t* inilist_get_at(const inilist_t* const list, int index) {
    if (index == list->count - 1) {
        return inilist_get_last(list);
    }
    inilist_node_t* p = list->first;
    for (int i = 0; i < index; i++) {
        p = p->next;
    }
    return p->element;
}

//删除对象,返回是否删除成功
bool inilist_remove_object(inilist_t* const list, inientry_t* data) {
    inilist_iterator_t* it = inilist_get_iterator(list);
    bool a = false;
    while (inilist_iterator_has_next(it)) {
        inientry_t* ld = inilist_iterator_get_next(it);
        // 地址相同或者equals判同
        if (ld == data) {
            a = true;
            break;
        }
    }
    if (a) {
        inilist_remove_at(list, it->count - 1);
    }
    return a;
}

//取得第一个数据
inientry_t* inilist_get_first(const inilist_t* const list) {
    return list->first->element;
}
 
//取得最后一个数据
inientry_t* inilist_get_last(const inilist_t* const list) {
    return list->last->element;
}
 
//查找某个数据的位置,如果equals方法为空，比较地址，否则调用equals方法
//如果不存在返回-1，如果存在，返回出现的第一个位置
int inilist_index_of(const inilist_t* const list, inientry_t* data) {
    inilist_node_t* p = list->first;
    int re = 0;

    while (p) {
        if (p->element == data) {
            return re;
        }
        re++;
        p = p->next;
    }
 
    return -1;
}

 
//创建遍历器
inilist_iterator_t* inilist_get_iterator(const inilist_t* const list) {
    inilist_iterator_t* re = (inilist_iterator_t*) malloc(sizeof(inilist_iterator_t));
    if (list == NULL) {
    	free(re);
    	return NULL;
    }
    re->node = list->first;
    re->size = list->count;
    re->count = 0;
    return re;
}
 
//释放遍历器
void inilist_iterator_free(inilist_iterator_t* iterator) {
    free(iterator);
}
 
//遍历器是否有下一个元素
int inilist_iterator_has_next(const inilist_iterator_t* const iterator) {
    return iterator->count < iterator->size;
}
 
//返回遍历器的下一个元素
inientry_t* inilist_iterator_get_next(inilist_iterator_t* const iterator) {
    inientry_t* re = iterator->node->element;
    iterator->node = iterator->node->next;
    (iterator->count)++;
    return re;
}
 
