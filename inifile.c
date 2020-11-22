#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "inifile.h"

void inifile_init(inifile_t* ini) {
    ini->grps = NULL;
    ini->grpcnt = 0;
    ini->usr_data = NULL;
}

void inifile_clean(inifile_t* ini) {
    inigroup_t* g = ini->grps;
    inigroup_t* gnext;
    while (g) {
        gnext = g->next;

        if (g->name) {
            inistr_clear(g->name);
            free(g->name);
        }

        if (g->map) {
            inimap_free(g->map);
        }

        free(g);
        g = gnext;
    }
    ini->grpcnt = 0;
    ini->grps = NULL;
}

int inifile_read(inifile_t* ini, const char* path) {
    int fd = open(path, O_RDWR | O_CREAT, 0666);

    if (fd == -1)
        return INI_ERR_FILE;
    
    ini->grpcnt = 0;
    ini->grps = NULL;
    
    char buf[1024];
    int len;
    char c;
    int ret = INI_ERR_NONE;
    
    inigroup_t* grp = NULL;
    inistr_t* key = NULL;
    inistr_t* value = NULL;

    enum {
        NONE = 0,
        SKIP_LINE,  // 跳过本行
        GRP,        // 组名parse
        GRP_OK,     // 组名ok, 本行再有字符则非法

        KEY,        // key
        KEY_OK,     // key ok
        VALUE       // value
    } status = NONE;

    while ((len = read(fd, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < len; i++) {
            c = buf[i];
            if (c == ' ' || c == '\r' || c == '\t') continue;

            if (c == '\0') goto eof;

            if (status == SKIP_LINE) {
                if (c == '\n') status = NONE;
                continue;
            }

            // 行首
            if (status == NONE) {
                // 新行不接受 =号
                if (c == '[') { 
                    inigroup_t* g = malloc(sizeof(inigroup_t));
                    if (!g) goto exception;

                    bzero(g, sizeof(inigroup_t));
                    g->file = ini;
                    g->map = inimap_create();
                    if (!g->map) goto exception;
                    g->name = malloc(sizeof(inistr_t));
                    inistr_init(g->name);
                    if (!g->name) goto exception;
                    
                    if (grp) {
                        g->last = grp;
                        grp->next = g;
                    } else {
                        ini->grps = g;
                    }
                    grp = g;
                    status = GRP;
                    ini->grpcnt++;
                    continue; 
                }
                if (c == '=') goto format;
                if (c == ';') { status = SKIP_LINE; continue; }
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_') {
                    status = KEY;
                    assert(key == NULL);
                    key = malloc(sizeof(inistr_t));
                    if (!key) goto exception;
                    inistr_init(key);
                    inistr_append_char(key, c);
                    continue;
                }
                if (c == '\n') continue;
                goto format;
            } else if (c == '\n' || c == ';') {
                // 行尾
                switch (status) {
                    case NONE: break;
                    case SKIP_LINE: status = c == '\n' ? NONE : SKIP_LINE; break;
                    case KEY: 
                    case KEY_OK:
                    case GRP: goto format;
                    case GRP_OK: status = c == '\n' ? NONE : SKIP_LINE; break;
                    case VALUE: {
                        assert(key);
                        if (!grp || !value) goto format;
                        inimap_put(grp->map, key, value);

                        key = NULL;
                        value = NULL;
                        status = c == '\n' ? NONE : SKIP_LINE;
                        break;
                    }
                }
            } else if (status == KEY) {
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_') {
                    assert(key != NULL);
                    inistr_append_char(key, c);
                } else if (c == '=') {
                    status = KEY_OK;
                    assert(key);
                    assert(!value);
                    value = malloc(sizeof(inistr_t));
                    if (!value) goto exception;
                    inistr_init(value);
                    status = VALUE;
                } else goto format;
            } else if (status == GRP) {
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_') {
                    assert(grp != NULL && grp->name != NULL);
                    inistr_append_char(grp->name, c);
                } else if (c == ']') {
                    status = GRP_OK;
                } else goto format;
            } else if (status == VALUE) {
                assert(value);
                inistr_append_char(value, c);
            } else 
                goto format;

        }
    }

    // 文件结束状态判断
    switch (status) {
        case NONE:
        case GRP_OK:
        case SKIP_LINE: break;
        case KEY: 
        case KEY_OK:
        case GRP: goto format;
        case VALUE: {
            assert(key);
            if (!grp || !value) goto format;
            inimap_put(grp->map, key, value);

            key = NULL;
            value = NULL;
            status = NONE;
            break;
        }
    }

    if (fd > 0)
        close(fd);
    return INI_ERR_NONE;

    
eof:
    ret = INI_ERR_FILE;
    printf("ini eof error\r\n");
    goto close;

exception:
    ret = INI_ERR_ALLCATION;
    printf("ini exception\r\n");
    goto close;

format:
    ret = INI_ERR_FORMAT;
    printf("ini format error\r\n");
    goto close;

close:
    if (fd > 0)
        close(fd);
    inifile_clean(ini);
    return ret;
}



void inifile_foreach(const inifile_t* ini, inifile_loop_cb cb) {
    int index = 0;
    inigroup_t* grp = ini->grps;
    while (grp) {
        if (!grp) {
            printf("Count is diferent with memory size\r\n");
        } else if (!grp->map) {
            printf("Inifile map memory error\r\n");
        } else {
            inimap_iterator_t* itr = inimap_iterator_create(grp->map);
            while (inimap_iterator_has_next(itr)) {
                cb(grp, inimap_iterator_get_next(itr), index++);
            }
            grp = grp->next;
        }
    }
}
