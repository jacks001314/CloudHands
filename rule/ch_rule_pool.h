#ifndef CH_RULE_POOL_H
#define CH_RULE_POOL_H

typedef struct ch_rule_pool_t ch_rule_pool_t;

#include "ch_rule_group.h"


struct ch_rule_pool_t {

    struct list_head rules;

    int matchThenStop;

    ch_rule_context_t *rcontext;

    ch_rule_group_t *rgroup;

    ch_array_header_t *accept_protos; 
};


extern ch_rule_pool_t * ch_rule_pool_create(ch_rule_group_t *rule_group,const char *protos);

#endif /*CH_RULE_POOL_H*/
