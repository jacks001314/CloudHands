#ifndef CH_RULE_POOL_H
#define CH_RULE_POOL_H

typedef struct ch_rule_pool_t ch_rule_pool_t;

#include "ch_rule_group.h"
#include "ch_list.h"
#include "ch_rule_group.h"
#include "ch_tables.h"

struct ch_rule_pool_t {

    struct list_head node;
    struct list_head rules;
    ch_pool_t *mp;
    int matchThenStop;

    ch_rule_group_t *rgroup;

    ch_array_header_t *accept_protos; 
};


extern ch_rule_pool_t * ch_rule_pool_create(ch_rule_group_t *rule_group,int matchThenStop,const char *protos);

extern void ch_rule_pool_destroy(ch_rule_pool_t *rpool);

extern void ch_rule_pool_dump(FILE *fp,ch_rule_pool_t *rpool);
#endif /*CH_RULE_POOL_H*/
