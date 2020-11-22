#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "inifile.h"
#include "initest.h"

void out(inientry_t* et) {
    inistr_output(et->key);
    inistr_output(et->value);
}

void ini_callback(const inigroup_t* grp, const inientry_t* et, const int index) {
    printf("%d: [%.*s]", index, (int)grp->name->length, grp->name->p);
    printf("[%.*s]", (int)et->key->length, et->key->p);
    printf("[%.*s]\n", (int)et->value->length, et->value->p);

    // 不同芯片的处理
	if (inistr_starts_with(grp->name, "5901", strlen("5901"))) {
		
	} else if (inistr_starts_with(grp->name, "5401", strlen("5401"))) {
		
	} else if (inistr_starts_with(grp->name, "5904", strlen("5904"))) {
		
	} else if (inistr_starts_with(grp->name, "4159", strlen("4159"))) {
		

        // 4159计量/点频模式下的特殊配置
            if (inistr_starts_with(et->key, "R6_4159", strlen("R6_4159"))) {
                
            } else if (inistr_starts_with(et->key, "R5_4159", strlen("R5_4159"))) {
                
            } else if (inistr_starts_with(et->key, "R5_2_4159", strlen("R5_2_4159"))) {
                
            } else if (inistr_starts_with(et->key, "R2_4159", strlen("R2_4159"))) {
                
            } else if (inistr_starts_with(et->key, "R3_4159", strlen("R3_4159"))) {
                
            } else if (inistr_starts_with(et->key, "R1_4159", strlen("R1_4159"))) {
                
            } else if (inistr_starts_with(et->key, "R0_4159", strlen("R0_4159"))) {
                
            }
	} else return;

}

int main(void) {
    inimap_t* map = inimap_create();
    for (int i = 0; i < 100; i++) {
        inistr_t* key = malloc(sizeof(inistr_t));
        inistr_init(key);
        inistr_append_nchar(key, "kkk", 3);
        inistr_append_char(key, '0' + i / 10);
        inistr_append_char(key, '0' + i % 10);

        inistr_t* value = malloc(sizeof(inistr_t));
        inistr_init(value);
        inistr_append_nchar(value, "vvv", 5);
        inistr_append_char(value, '0' + i / 10);
        inistr_append_char(value, '0' + i % 10);
        inimap_put(map, key, value);
        inimap_output(map, out);
    }
    inimap_output(map, out);
    inimap_free(map);
    map = NULL;

    inifile_t file;
    inifile_init(&file);
    if (inifile_read(&file, "./test.ini") == INI_ERR_NONE) {
        inifile_foreach(&file, ini_callback);
    }
    
    inifile_clean(&file);
}