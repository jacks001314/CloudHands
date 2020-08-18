#ifndef CH_PSINK_TASK_H
#define CH_PSINK_TASK_H

typedef struct ch_psink_task_t ch_psink_task_t;

#include "ch_task.h"
#include "ch_process_psink.h"
#include "ch_psink_context.h"
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <pcap.h>

struct ch_psink_task_t {

    ch_task_t task;

    ch_process_sink_t *psink;

    ch_psink_context_t *psink_context;

    struct rte_mempool *pktmbuf_pool;

};

extern ch_psink_task_t * ch_psink_task_create(ch_psink_context_t *psink_context);

#endif /*CH_PSINK_TASK_H*/
