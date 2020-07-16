#ifndef CH_RULE_CONTEXT_H
#define CH_RULE_CONTEXT_H

typedef struct ch_rule_context_t ch_rule_context_t;

#define MAX_LINE_SIZE 1024 

#include "ch_tables.h"

struct ch_rule_context_t {

    ch_pool_t *mp;
    ch_table_t *maps;

    char *buff;
    size_t buff_size;
};


extern ch_rule_context_t *ch_rule_context_create(void);

extern void ch_rule_context_destroy(ch_rule_context_t *rcontext);

extern ch_array_header_t *ch_rule_context_find_value_arrays(ch_rule_context_t *rcontext,const char *value);

static inline  char *ch_rule_context_buff_update(ch_rule_context_t *context,const char *value){

    size_t len = strlen(value);
    if(len>=context->buff_size)
    {
        free(context->buff);
        context->buff = (char*)malloc(len+1);
        if(context->buff == NULL)
            return NULL;
        
        context->buff_size = len+1;
    }

    memset(context->buff,0,context->buff_size);

    memcpy(context->buff,value,len);

    return context->buff;
}

#endif /*CH_RULE_CONTEXT_H*/
