/*
 *
 *      Filename: ch_tcp_session_request_handler.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-02-02 10:15:09
 * Last Modified: 2018-02-02 10:15:09
 */

#ifndef CH_TCP_SESSION_REQUEST_HANDLER_H
#define CH_TCP_SESSION_REQUEST_HANDLER_H

typedef struct ch_tcp_session_request_handler_t ch_tcp_session_request_handler_t;

#include "ch_tcp_session_handler.h"
#include "ch_tcp_session_request_pool.h"
#include "ch_packet.h"
#include "ch_mpool_agent.h"
#include "ch_mpool.h"
#include "ch_tcp_context.h"
#include "ch_tcp_app_pool.h"

#define PROCESSOR_RET_OK 0
#define PROCESSOR_RET_DROP -1

struct ch_tcp_session_request_handler_t {

	ch_pool_t *mp;
	ch_tcp_context_t *tcp_context;
	ch_tcp_app_pool_t *ta_pool;

	ch_tcp_session_handler_t *shandler;

	ch_tcp_session_request_pool_t *req_pool;

    ch_mpool_agent_t *mpa;

	uint64_t cur_session_id;
};

#define session_pool_get(req_handler) (req_handler->shandler->spool)

extern ch_tcp_session_request_handler_t *
ch_tcp_session_request_handler_create(ch_pool_t *mp,
	ch_tcp_context_t *tcp_context,
	ch_tcp_app_pool_t *ta_pool,
	ch_tcp_session_handler_t *shandler);

extern int ch_tcp_session_request_packet_handle(ch_tcp_session_request_handler_t *req_handler,ch_packet_tcp_t *tcp_pkt);

#endif /*CH_TCP_SESSION_REQUEST_HANDLER_H*/
