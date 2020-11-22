#ifndef __INISTR_H_
#define __INISTR_H_

#include <stddef.h>
#include <stdbool.h>

/*!-- 动态扩容步进值 --*/
#define INISTR_STEP_SIZE   128

typedef struct {
    char *p;
    size_t length;

    size_t size;
    int hash;
} inistr_t;

int inistr_hashcode(inistr_t* o);
bool inistr_equals(inistr_t* s1, inistr_t* s2);
bool inistr_starts_with(inistr_t* str, const char* cstr, size_t len);

void inistr_init(inistr_t* str);
void inistr_append_char(inistr_t* str, const char c);
void inistr_append_nchar(inistr_t* str, const char* c, size_t size);
void inistr_append_str(inistr_t* str, const inistr_t* s);
void inistr_output(const inistr_t* str);
void inistr_clear(inistr_t* str);
void inistr_replace_char(inistr_t* str, char c1, char c2);
void inistr_replace_str(inistr_t* str, const inistr_t* s1, const inistr_t s2);

#endif
