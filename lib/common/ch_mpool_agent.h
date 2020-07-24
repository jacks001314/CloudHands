/*
 * =====================================================================================
 *
 *       Filename:  ch_mpool_agent.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/12/2018 04:31:32 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jacksha (shajf), csp001314@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef CH_MPOOL_AGNENT_H
#define CH_MPOOL_AGNENT_H

typedef struct ch_mpool_agent_t ch_mpool_agent_t;

#include "ch_mpool.h"
#include "ch_list.h"
#include "ch_log.h"

struct ch_mpool_agent_t {

    struct list_head cache_list; 

    unsigned long max_cache_n;
    unsigned long cur_cache_n;

    unsigned long pool_size;
    unsigned long pre_alloc_n;

    unsigned long from_caches;
    unsigned long from_mems;

    unsigned long allocs;
    unsigned long frees;
};

extern int ch_mpool_agent_init(ch_mpool_agent_t *mpa,
        unsigned long max_cache_n,unsigned long pool_size,unsigned long pre_alloc_n);


extern ch_mpool_agent_t *ch_mpool_agent_create(ch_pool_t *mp,
        unsigned long max_cache_n,unsigned long pool_size,
        unsigned long pre_alloc_n);


extern ch_pool_t *ch_mpool_agent_alloc(ch_mpool_agent_t *mpa);

extern void ch_mpool_agent_free(ch_mpool_agent_t *mpa,ch_pool_t *mp);

extern void ch_mpool_agent_log(ch_mpool_agent_t *mpa);


extern void ch_mpool_agent_dump(ch_mpool_agent_t *mpa,FILE *out);

#endif /* CH_MPOOL_AGNENT_H */
