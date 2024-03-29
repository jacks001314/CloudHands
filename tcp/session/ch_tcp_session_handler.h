/*
 *
 *      Filename: ch_tcp_session_handler.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-02-02 10:16:36
 * Last Modified: 2018-02-02 10:16:36
 */

#ifndef CH_TCP_SESSION_HANDLER_H
#define CH_TCP_SESSION_HANDLER_H

typedef struct ch_tcp_session_handler_t ch_tcp_session_handler_t;

#include "ch_tcp_context.h"
#include "ch_mpool.h"
#include "ch_tcp_session_pool.h"
#include "ch_packet_tcp.h"
#include "ch_proto_session_store.h"
#include "ch_mpool_agent.h"

struct ch_tcp_session_handler_t {

	ch_pool_t *mp;

	ch_tcp_context_t *tcp_context;

	ch_tcp_session_pool_t *spool;

    ch_proto_session_store_t *pstore;

    ch_mpool_agent_t *mpa;

	uint32_t task_id;
};

extern ch_tcp_session_handler_t * 
ch_tcp_session_handler_create(ch_pool_t *mp,ch_tcp_context_t *tcp_context,uint32_t task_id);

extern int ch_tcp_session_packet_handle(ch_tcp_session_handler_t *shandler,
	ch_tcp_session_t *tcp_session,
	ch_packet_tcp_t *tcp_pkt);

#endif /*CH_TCP_SESSION_HANDLER_H*/
