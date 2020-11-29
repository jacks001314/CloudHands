#ifndef CH_STAT_TASK_H
#define CH_STAT_TASK_H

typedef struct ch_stat_ring_entry_t ch_stat_ring_entry_t;
typedef struct ch_stat_task_t ch_stat_task_t;

#include <rte_ring.h>
#include <rte_mempool.h>
#include "ch_task.h"
#include "ch_stat_pool.h"

struct ch_stat_task_t {

   ch_task_t task;

   struct rte_mempool *entry_pool;

   struct rte_ring *ring;
   
   ch_stat_pool_t *st_pool;
};

struct ch_stat_ring_entry_t {

    uint64_t time;
    uint64_t pkt_size;
    int pkt_type;
};

extern ch_stat_task_t * ch_stat_task_create(ch_stat_pool_t *st_pool,size_t pool_size,size_t ring_size);

extern void ch_stat_task_entry_put(ch_stat_task_t *task,uint64_t time,uint64_t pkt_size,int pkt_type);

#endif /*CH_STAT_TASK_H*/
