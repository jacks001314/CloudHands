/*
 * =====================================================================================
 *
 *       Filename:  ch_pdump_context.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/06/2017 02:53:58 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  shajf (jacks), csp001314@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef CH_pdump_CONTEXT_H
#define CH_pdump_CONTEXT_H

typedef struct ch_pdump_context_t ch_pdump_context_t;

#include "ch_log.h"
#include "ch_mpool.h"
#include "ch_core_pool.h"
#include "ch_port_pool.h"
#include "ch_packet_rxtask_pool.h"
#include "ch_stat_pool.h"
#include "ch_filter_engine.h"
#include "ch_process_psink.h"

struct ch_pdump_context_t {

   ch_pool_t *mp;

   ch_core_pool_t *cpool;

   ch_port_pool_t *ppool;

   ch_packet_rxtask_pool_t *rxtask_pool;

   ch_stat_pool_t *st_pool;

   ch_filter_engine_t *filter_engine;

   ch_process_sink_t *psink;

   /*config log name and level*/
   const char *log_name;
   int log_level;

   /*config port config file path*/
   const char *port_cfname;

   /*config port mask*/
   uint32_t port_mask;

   /*config packet receive task's number*/
   uint32_t packet_rxtasks_n;

   /*statistic configs*/
	const char *stat_mmap_fname;
	uint64_t stat_time_up;
	uint64_t stat_time_tv;

    const char *filter_json_file;

    const char *pcap_dir;
    size_t loop_bytes;

    int use_psink;
    size_t psink_mpool_size;
    size_t psink_queue_size;
};


extern ch_pdump_context_t * ch_pdump_context_create(ch_pool_t *mp,const char *cfname);

extern int ch_pdump_context_start(ch_pdump_context_t *pdcontext);

extern void ch_pdump_context_stop(ch_pdump_context_t *pdcontext);

#endif /*CH_pdump_CONTEXT_H*/

