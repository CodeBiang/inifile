#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inistr.h"


int inistr_hashcode(inistr_t* str) {
	int i = 0;
	int h = str->hash;
	if (h == 0 && str->length > 0) {
		for (; i < str->length; i++) {
			// 这里的质数选择不一定高效
			h = 31 * h + str->p[i];
		}
		str->hash = h;
	}
	return h;
}

bool inistr_equals(inistr_t* s1, inistr_t* s2) {
    if (s1 == s2)
        return true;
    size_t n = s1->length;
    if (n == s2->length) {
        char* v1 = s1->p;
        char* v2 = s2->p;
        size_t i = 0;
        while (n-- != 0) {
            if (v1[i] != v2[i])
                return false;
            i++;
        }
        return true;
    }
    return false;
}

bool inistr_starts_with(inistr_t* s, const char* cstr, size_t len) {
	if (len > s->length) return false;
	size_t n = len;
	size_t i = 0;
	while (n-- != 0) {
		if (s->p[i] != cstr[i])
			return false;
		i++;
	}
	return true;
}

static bool __inistr_check_resize(inistr_t *str, size_t len) {
    if (str->p == NULL)
        inistr_init(str);
    if ((str->length + len) > str->size) {
        int steps = (str->length + len) / INISTR_STEP_SIZE + 1;
        size_t new_size = steps * INISTR_STEP_SIZE;
        char* p = calloc(new_size, sizeof(char));
        if (!p) return false;
        bzero(p, new_size);
        memcpy(p, str->p, str->length);

        str->size = new_size;
        free(str->p);
        str->p = p;
    }
    return true;
}

bool __inistr_init_with_size(inistr_t *str, size_t size) {
    str->length = 0;
    str->size = size;
    str->p = calloc(str->size, sizeof(char));
    return str->p != 0;
}

void inistr_init(inistr_t *str) {
    __inistr_init_with_size(str, INISTR_STEP_SIZE);
}


void inistr_append_char(inistr_t *str, const char c) {
    if (!__inistr_check_resize(str, 1))
        printf("inistr allocation failed\r\n");
    if (c != 0)
        str->p[str->length++] = c;
}

void inistr_append_nchar(inistr_t *str, const char* c, size_t size) {
    if (!__inistr_check_resize(str, size))
        printf("inistr allocation failed\r\n");
    for (int i = 0; i < size && c[i] != 0; i++) {
        str->p[str->length++] = c[i];
    }
}

void inistr_append_str(inistr_t *str, const inistr_t* s) {
    if (!__inistr_check_resize(str, s->length))
        printf("inistr allocation failed\r\n");
    for (int i = 0; i < s->length && s->p[i] != 0; i++) {
        str->p[str->length++] = s->p[i];
    }
}

void inistr_output(const inistr_t *str) {
    for (int i = 0; i < str->length; i++) {
        printf("%c", str->p[i]);
    }
    printf("\r\n");
}

void inistr_clear(inistr_t *str) {
    str->length = 0;
    str->size = 0;
    if (str->p != NULL) {
        free(str->p);
        str->p = NULL;
    }
}

void inistr_replace_char(inistr_t *str, char c1, char c2) {
    int i;
    for (i = 0; i < str->length; i++) {
        if (str->p[i] == c1) {
            str->p[i] = c2;
        }
    }
}


