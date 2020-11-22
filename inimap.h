#ifndef __INIMAP_H_
#define __INIMAP_H_
#include <stdbool.h>
#include "inilist.h"

/*!-- 默认初始容量 --*/
#define DEFAULT_INITIAL_CAPACITY 16
/*!-- 默认加载因子 --*/
#define DEFAULT_LOAD_FACTOR 0.75f

typedef struct inimap inimap_t;
typedef struct inimap_iterator inimap_iterator_t;

struct inimap {
    int size;
    int initial_capacity;
    float load_factor;
    inilist_t** entry_list;
};

struct inimap_iterator {
    // Index of LinkedList
    int index;
    inimap_t* map;
    inilist_node_t* current;
    // Index of LinkedNode
    int count;
};

//创建HashMap
inimap_t* inimap_create();

//释放HashMap
void inimap_free(inimap_t* map);

//是否包含某个key
bool inimap_contains_key(inimap_t* const map, inistr_t* const key);

//增加一条映射
void inimap_put(inimap_t* const map, inistr_t* const key, inistr_t* const value);

//通过key得到数据，如果没有数据则返回null
inistr_t* inimap_get(inimap_t* const map, inistr_t* const key);

//数据的容量
int inimap_get_size(const inimap_t* const map);

//创建Entry迭代器
inimap_iterator_t* inimap_iterator_create(inimap_t* const map);

//释放Entry迭代器
void inimap_iterator_free(inimap_iterator_t* iterator);

// Entry迭代器是否有下一个
bool inimap_iterator_has_next(inimap_iterator_t* iterator);

//遍历下一个Entry元素
inientry_t* inimap_iterator_get_next(inimap_iterator_t* iterator);

//删除一条数据，返回是否删除成功
bool inimap_remove(inimap_t* const map, inistr_t* const key);

//遍历
void inimap_output(inimap_t* map, void (*pt)(inientry_t*));

#endif
