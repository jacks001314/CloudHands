
#ifndef CH_RULE_ENGINE_CONTEXT_H
#define CH_RULE_ENGINE_CONTEXT_H

typedef struct ch_rule_engine_context_t ch_rule_engine_context_t;

#include "ch_mpool.h"
#include "ch_list.h"
#include "ch_rule_pool.h"
#include "ch_rule_match.h"

struct ch_rule_engine_context_t {

    ch_pool_t *mp;

    struct list_head rpools;

    ch_rule_group_pool_t *rule_group_pool;

    const char *ruleGroupPath;
    const char *engine;
    const char *protos;

    int matchThenStop;
};

extern ch_rule_engine_context_t *ch_rule_engine_context_create(ch_pool_t *mp,const char *cfname);

extern void ch_rule_engine_context_destroy(ch_rule_engine_context_t *recontext);

extern int ch_rule_engine_context_run_match(ch_rule_engine_context_t *recontext,ch_rule_target_context_t *tcontext);

extern int ch_rule_engine_context_run_nmatch(ch_rule_engine_context_t *recontext,
        ch_rule_match_context_t *mcontext,
        ch_rule_target_context_t *tcontext);

#endif /*CH_RULE_ENGINE_CONTEXT_H*/
