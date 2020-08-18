/*
 * =====================================================================================
 *
 *       Filename:  ch_psink_context.h
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

#ifndef CH_PSINK_CONTEXT_H
#define CH_PSINK_CONTEXT_H

typedef struct ch_psink_context_t ch_psink_context_t;

#include "ch_log.h"
#include "ch_mpool.h"
#include "ch_core_pool.h"
#include "ch_process_interface_tcp_context.h"
#include "ch_process_interface_sa_context.h"
#include "ch_process_interface_udp_context.h"
#include "ch_process_psink.h"
#include "ch_psink_task.h"

struct ch_psink_context_t {

   ch_pool_t *mp;

   ch_core_pool_t *cpool;

   ch_psink_task_t *psink_task;

   ch_process_interface_tcp_context_t *pint_tcp_context;
   
   ch_process_interface_sa_context_t  *pint_sa_context;
   
   ch_process_interface_udp_context_t  *pint_udp_context;

   ch_process_sink_t *psink;

   /*config log name and level*/
   const char *log_name;
   int log_level;
   
   /*config process interface tcp  config file path*/
   const char *pint_tcp_cfname;

   /*Config process interface stream analyze config file path*/
   const char *pint_sa_cfname;

   /*Config process interface udp config file path*/
   const char *pint_udp_cfname;

   const char *pcap_dir;

   size_t pkt_mbuf_size;
   size_t pkt_mbuf_num;

};


extern ch_psink_context_t * ch_psink_context_create(ch_pool_t *mp,const char *cfname);

extern int ch_psink_context_start(ch_psink_context_t *psink_context);

extern void ch_psink_context_stop(ch_psink_context_t *psink_context);

#endif /*CH_PSINK_CONTEXT_H*/

