#ifndef CH_RULE_ENGINE_H
#define CH_RULE_ENGINE_H

typedef struct ch_rule_engine_t ch_rule_engine_t;

#include "ch_mpool.h"
#include "ch_rule_engine_context.h"
#include "ch_tables.h"

struct ch_rule_engine_t {

    ch_pool_t *mp;
    ch_rule_engine_context_t *recontext;

    ch_array_header_t *protos;
};


extern ch_rule_engine_t * ch_rule_engine_create(ch_pool_t *mp,
        const char *cfname);


extern int ch_rule_engine_match(ch_rule_engine_t *rengine,
    ch_rule_target_context_t *tcontext);


extern int ch_rule_engine_nmatch(ch_rule_engine_t *rengine,
    ch_rule_target_context_t *tcontext,ch_rule_match_context_t *mcontext);

#endif /*CH_RULE_ENGINE_H*/
