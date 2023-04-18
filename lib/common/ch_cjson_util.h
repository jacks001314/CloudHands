#ifndef CH_CJSON_H
#define CH_CJSON_H

#include <cjson/cJSON.h>
#include "ch_mpool.h"
#include "ch_string.h"

static inline const char * ch_json_str_value_get(ch_pool_t *mp,cJSON *obj,const char *key,const char *defv){

    cJSON *entry = cJSON_GetObjectItem(obj,key);

    if(entry == NULL)
        return defv;

    return ch_pstrdup(mp,cJSON_GetStringValue(entry));
}

static inline const char * ch_json_str_value_get_no_cp(cJSON *obj,const char *key,const char *defv){

    cJSON *entry = cJSON_GetObjectItem(obj,key);

    if(entry == NULL)
        return defv;

    return cJSON_GetStringValue(entry);
}

static inline int ch_json_bool_value_get(cJSON *obj,const char *key,int defv){

    cJSON *entry = cJSON_GetObjectItem(obj,key);

    if(entry == NULL)
        return defv;

    return cJSON_IsTrue(entry);
}

static inline long ch_json_num_value_get(cJSON *obj,const char *key,long defv){

    cJSON *entry = cJSON_GetObjectItem(obj,key);

    if(entry == NULL)
        return defv;

    return (long)cJSON_GetNumberValue(entry);
    
}

static inline cJSON * ch_json_obj_get(cJSON *obj,const char *key) {

    return cJSON_GetObjectItem(obj,key);
}

static inline int ch_json_isArray(cJSON *obj) {

    return cJSON_IsArray(obj);
}

static inline void ch_for_json_array(cJSON *obj,void *priv_data,void (*fn)(cJSON *entry,void *priv_data)){

    int i;
    int n = cJSON_GetArraySize(obj);

    for(i=0;i<n;i++){
        cJSON *entry = cJSON_GetArrayItem(obj,i);

        fn(entry,priv_data);
    }
}

static inline int ch_json_array_size(cJSON *obj){
    return cJSON_GetArraySize(obj);
}

static inline cJSON * ch_json_array_item_get(cJSON *obj,int i) {

    return cJSON_GetArrayItem(obj,i);
}

int ch_cjson_parse(const char *fname,void *obj,int (*parse)(cJSON *root,void *obj));

int ch_cjson_parse_from_content(const char *content,void *obj,int (*parse)(cJSON *root,void *obj));

#endif /*CH_CJSON_H*/