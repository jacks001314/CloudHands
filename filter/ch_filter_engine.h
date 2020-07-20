#ifndef CH_FILTER_engine_H
#define CH_FILTER_engine_H

typedef struct ch_filter_engine_t ch_filter_engine_t;

#include "ch_mpool.h"
#include "ch_rule_engine_context.h"
#include "ch_tables.h"

struct ch_filter_engine_t {

    ch_pool_t *mp;
    ch_rule_engine_context_t *recontext;

    ch_array_header_t *protos;
};


extern ch_filter_engine_t * ch_filter_engine_create(ch_pool_t *mp,
        const char *cfname);


extern int ch_filter_engine_accept(ch_filter_engine_t *fengine,
    ch_rule_target_context_t *tcontext);

#endif /*CH_FILTER_engine_H*/
