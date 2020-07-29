#ifndef CH_RULE_MATCH_H
#define CH_RULE_MATCH_H

#include "ch_rule_pool.h"
#include "ch_rule.h"
typedef struct ch_rule_target_context_t ch_rule_target_context_t;
typedef struct ch_rule_match_context_t ch_rule_match_context_t;

struct ch_rule_target_context_t {

    const char *proto;

    void *data;
    
    int (*isMyProto)(ch_rule_target_context_t *tcontext,int proto);

    const char* (*target)(ch_rule_target_context_t *tcontext,ch_rule_target_t *target,int isHex);

};

struct ch_rule_match_context_t {

    struct list_head match_rules;
    int match_n;
};


extern int ch_rule_match(ch_rule_target_context_t *tcontext,ch_rule_pool_t *rpool);

extern int ch_rule_nmatch(ch_rule_match_context_t *mcontext,ch_rule_target_context_t *tcontext,ch_rule_pool_t *rpool);

#endif /*CH_RULE_MATCH_H*/
