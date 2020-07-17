#ifndef CH_CJSON_H
#define CH_CJSON_H

#include <cjson/cJSON.h>
#include "ch_mpool.h"
#include "ch_string.h"

static inline const char * ch_json_str_value_get(ch_pool_t *mp,cJSON *obj,const char *key){

    cJSON *entry = cJSON_GetObjectItem(obj,key);

    if(entry == NULL)
        return NULL;

    return ch_pstrdup(mp,cJSON_GetStringValue(entry));
}

static inline const char * ch_json_str_value_get_no_cp(cJSON *obj,const char *key){

    cJSON *entry = cJSON_GetObjectItem(obj,key);

    if(entry == NULL)
        return NULL;

    return cJSON_GetStringValue(entry);
}

static inline int ch_json_bool_value_get(cJSON *obj,const char *key){

    cJSON *entry = cJSON_GetObjectItem(obj,key);

    if(entry == NULL)
        return 0;

    return cJSON_IsTrue(entry);
}

static inline long ch_json_num_value_get(cJSON *obj,const char *key){

    cJSON *entry = cJSON_GetObjectItem(obj,key);

    if(entry == NULL)
        return 0;

    return (long)cJSON_GetNumberValue(entry);
    
}

extern int ch_cjson_parse(const char *fname,void *obj,int (*parse)(cJSON *root,void *obj));


#endif /*CH_CJSON_H*/
