/**
 * Unix ini file rw tool
 * 
 * @version 1.0.0
 * @author  CodeBiang@163.com
 * @date    2020/11/19
 */
#ifndef __INIFILE_H_
#define __INIFILE_H_

#include <stddef.h>
#include <stdlib.h>
#include "inimap.h"

#ifdef __GNUC__
#define INI_USE_GNU
#endif

#ifdef INI_USE_GNU
#define __INI_NONNULL(params) __nonnull(params)
#endif

/*!-- Key/Groupname max length --*/
#define INI_FIELD_MAX_LENGTH            100
/*!-- Value max length --*/
#define INI_VALUE_MAX_LENGTH            500

enum inierr {
    INI_ERR_NONE = 0,
    INI_ERR_FILE = 1,
    INI_ERR_FORMAT= 2,
    INI_ERR_ALLCATION = 3
};

/*!-- Readonly types --*/
/*!-- Note that it's best not to use theses types directly --*/
typedef struct inigroup inigroup_t;
typedef struct inifile inifile_t;

/*!-- Loop callback funcs --*/
typedef void (*inigroup_loop_cb)(const inientry_t* group, const int index);
typedef void (*inifile_loop_cb)(const inigroup_t* group, const inientry_t* entry, const int index);

struct inigroup {
    inifile_t* file;

    inistr_t* name;
    inimap_t* map;

    inigroup_t* next;
    inigroup_t* last;
};

struct inifile {
    inigroup_t* grps;
    int grpcnt;

    void* usr_data;
};

/*!-- Basic API --*/

/**
 * Initialize `inifile_t`
 * 
 * @param ini inifile
 * @return error code
 */
void inifile_init(inifile_t* ini);

/**
 * Close xx.ini and release memory
 * 
 * @param ini inifile
 */
void inifile_clean(inifile_t* ini);

/*!-- Reader API --*/
int inifile_read(inifile_t* ini, const char* path);
void inifile_foreach(const inifile_t* ini, inifile_loop_cb cb) __INI_NONNULL((1, 2));

void inigroup_foreach(const inigroup_t* group, inigroup_loop_cb cb) __INI_NONNULL((1, 2));

/*!-- Writer API --*/
// void inifile_group_begin(inifile_t* ini, const char* name);
// void inifile_group_end(inifile_t* ini);
// void inifile_set(inifile_t* ini);

#endif