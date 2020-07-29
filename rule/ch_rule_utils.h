#ifndef CH_RULE_UTILS_H
#define CH_RULE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "ch_string.h"
#include "ch_tables.h"
#include "ch_rule_constants.h"

typedef struct ch_rule_int_t ch_rule_int_t;

struct ch_rule_int_t {

    int value;
};


#define ch_rule_int_equal(intv,input) ((intv)->value==(input))

static inline int ch_rule_int_values_contains(ch_array_header_t *arr,int value){

    int i;
    ch_rule_int_t *elt;
    ch_rule_int_t **elts = (ch_rule_int_t**)arr->elts;

    for(i = 0;i<arr->nelts;i++){

        elt = elts[i];

        if(elt->value == value)
            return 1;
    }

    return 0;
}


static inline ch_array_header_t* ch_strsplit_to_arrays(ch_pool_t *mp,const char *str,const char *split){

    ch_array_header_t *arr = ch_array_make(mp,8,sizeof(char*));
    char *ptr;
    char *part;
    
    while ((part = ch_strtok((char*)str,(char*)split, &ptr)) != NULL)
    {
         if (*part == '\0')
             continue;
         
         *(char**)ch_array_push(arr) = ch_pstrdup(mp,part);
         
         str = NULL;                                
    }

    return arr;
}

static inline ch_array_header_t* ch_protos_to_arrays(ch_pool_t *mp,const char *str,const char *split){


    char *ptr;
    char *part;
    ch_rule_int_t *int_v;

    ch_array_header_t *arr = ch_array_make(mp,8,sizeof(ch_rule_int_t*));

    while ((part = ch_strtok((char*)str,(char*)split, &ptr)) != NULL)
    {
         if (*part == '\0')
             continue;

         int_v = ch_pcalloc(mp,sizeof(*int_v));
         int_v->value = ch_proto_value_get(part);
         *(ch_rule_int_t**)ch_array_push(arr) = int_v;
         
         str = NULL;                                
    }

    return arr;
}

static inline int ch_rule_op_contains(const char *target,const char *match){

    size_t match_length;
    size_t target_length = 0;
    size_t i, i_max;

    if(target == NULL||match == NULL)
        return 0;

    match_length = strlen(match);
    target_length = strlen(target);

    if(match_length == 0||target_length == 0)
        return 0;

    /* This is impossible to match */
    if (match_length > target_length) {
        /* No match. */
        return 0;
    }

    /* scan for first character, then compare from there until we
     * have a match or there is no room left in the target
     */
    i_max = target_length - match_length;
    for (i = 0; i <= i_max; i++) {
        /* First character matched - avoid func call */
        if (target[i] == match[0]) {
            /* See if remaining matches */
            if (   (match_length == 1)
                    || (memcmp((match + 1), (target + i + 1), (match_length - 1)) == 0))
            {
                /* Match. */
                return 1;
            }
        }
    }

    /* No match. */
    return 0;
}

static inline int ch_rule_op_endsWith(const char *target,const char *match){

    size_t match_length;
    size_t target_length;
    
    if(target == NULL||match == NULL)
        return 0;

    match_length = strlen(match);
    target_length = strlen(target);

    if(match_length == 0||target_length == 0)
        return 0;

    /* This is impossible to match */
    if (match_length > target_length) {
        /* No match. */
        return 0;
    }

    if (memcmp(match, (target + (target_length - match_length)), match_length) == 0) {
        return 1;
    }

    /* No match. */
    return 0;

}

static inline int ch_rule_op_startsWith(const char *target,const char *match){

    size_t match_length;
    size_t target_length;
    
    if(target == NULL||match == NULL)
        return 0;

    match_length = strlen(match);
    target_length = strlen(target);

    if(match_length == 0||target_length == 0)
        return 0;

    /* This is impossible to match */
    if (match_length > target_length) {
        /* No match. */
        return 0;
    }

    if (memcmp(target,match, match_length) == 0) {
        return 1;
    }

    /* No match. */
    return 0;

}

static inline int ch_rule_op_eq(const char *target,const char *match){

    size_t match_length;
    size_t target_length;
    
    if(target == NULL||match == NULL)
        return 0;

    match_length = strlen(match);
    target_length = strlen(target);

    if(match_length == 0||target_length == 0)
        return 0;

    /* This is impossible to match */
    if (match_length !=target_length) {
        /* No match. */
        return 0;
    }

    if (memcmp(target,match, match_length) == 0) {
        return 1;
    }

    /* No match. */
    return 0;

}

static inline int ch_rule_op_lt(const char *target,const char *match){

    char *ptr;
    long target_value;
    long match_value;

    size_t match_length;
    size_t target_length;
    
    if(target == NULL||match == NULL)
        return 0;

    match_length = strlen(match);
    target_length = strlen(target);

    if(match_length == 0||target_length == 0)
        return 0;

    target_value = strtol(target,&ptr,10);
    match_value = strtol(match,&ptr,10);


    return target_value<match_value;

}

static inline int ch_rule_op_gt(const char *target,const char *match){

    char *ptr;
    long target_value;
    long match_value;

    size_t match_length;
    size_t target_length;
    
    if(target == NULL||match == NULL)
        return 0;

    match_length = strlen(match);
    target_length = strlen(target);

    if(match_length == 0||target_length == 0)
        return 0;

    target_value = strtol(target,&ptr,10);
    match_value = strtol(match,&ptr,10);


    return target_value>match_value;
}

static inline int ch_rule_op_le(const char *target,const char *match){

    char *ptr;
    long target_value;
    long match_value;

    size_t match_length;
    size_t target_length;
    
    if(target == NULL||match == NULL)
        return 0;

    match_length = strlen(match);
    target_length = strlen(target);

    if(match_length == 0||target_length == 0)
        return 0;

    target_value = strtol(target,&ptr,10);
    match_value = strtol(match,&ptr,10);


    return target_value<=match_value;

}

static inline int ch_rule_op_ge(const char *target,const char *match){

    char *ptr;
    long target_value;
    long match_value;

    size_t match_length;
    size_t target_length;
    
    if(target == NULL||match == NULL)
        return 0;

    match_length = strlen(match);
    target_length = strlen(target);

    if(match_length == 0||target_length == 0)
        return 0;

    target_value = strtol(target,&ptr,10);
    match_value = strtol(match,&ptr,10);


    return target_value>=match_value;

}

static inline const char * ch_rule_to_hex(ch_pool_t *mp, unsigned char *data, size_t len){

    static const unsigned char b2hex[] = "0123456789abcdef";
    char *hex = NULL;
    size_t i, j;

    hex = ch_palloc(mp, (len * 2) + 1);
    
    if (hex == NULL) 
        return NULL;

    j = 0;
    for(i = 0; i < len; i++) {
        hex[j++] = b2hex[data[i] >> 4];
        hex[j++] = b2hex[data[i] & 0x0f];
    }

    hex[j] = 0;

    return (const char*)hex;

}

static inline const char * ch_rule_to_hex_with_buff(unsigned char *hex, unsigned char *data, size_t len){

    static const unsigned char b2hex[] = "0123456789abcdef";
    size_t i, j;

    if (hex == NULL) 
        return NULL;

    j = 0;
    for(i = 0; i < len; i++) {
        hex[j++] = b2hex[data[i] >> 4];
        hex[j++] = b2hex[data[i] & 0x0f];
    }

    hex[j] = 0;

    return (const char*)hex;

}

static inline const char * ch_rule_dot_key_get(const char *value,const char *prefix){

    size_t plen;
    size_t vlen;

    if(value == NULL||prefix == NULL)
        return NULL;

    vlen = strlen(value);
    plen = strlen(prefix);

    if(vlen == 0||plen == 0||plen>=vlen)
        return NULL;

    return value+plen;
}

static inline const char * ch_rule_data_get(unsigned char *buff,size_t bsize,void *data,size_t dlen,size_t offset,size_t len,int isHex){


    size_t hbsize;
    size_t rlen;
    void *pdata;

    if(buff == NULL||data == NULL||bsize<=0||dlen<=0||offset>=dlen)
        return NULL;

    pdata = data+offset;
    dlen = dlen-offset;
    if(len>0&&dlen>len){
        dlen = len;
    }

    hbsize = (bsize/2)-1;
    rlen = dlen>=bsize?bsize-1:dlen;
    
    if(!isHex){

        memcpy((void*)buff,pdata,rlen);
        buff[rlen] = '\0';
    }else{

        rlen = dlen>=hbsize?hbsize:dlen;

        ch_rule_to_hex_with_buff(buff,(unsigned char*)pdata,rlen);
    }

    return (const char*)buff;
}

#endif /*CH_RULE_UTILS_H*/
