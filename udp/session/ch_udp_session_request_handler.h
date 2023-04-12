/*
 *
 *      Filename: ch_udp_session_request_handler.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-09-12 17:59:26
 * Last Modified: 2018-09-12 17:59:26
 */

#ifndef CH_UDP_SESSION_REQUEST_HANDLER_H
#define CH_UDP_SESSION_REQUEST_HANDLER_H

typedef struct ch_udp_session_request_handler_t ch_udp_session_request_handler_t;

#include "ch_udp_session_request_pool.h"
#include "ch_udp_app_pool.h"
#include "ch_mpool.h"
#include "ch_udp_context.h"
#include "ch_udp_session_handler.h"

struct ch_udp_session_request_handler_t {

 	ch_udp_app_pool_t *app_pool;

	ch_pool_t *mp;

	ch_udp_context_t *udp_context;

	ch_udp_session_handler_t *shandler;

	ch_udp_session_request_pool_t *req_pool;

	uint32_t task_id;

};

#define CH_UDP_SESSION_POOL_GET(handler) ((handler)->shandler->udp_pool)

extern ch_udp_session_request_handler_t *
ch_udp_session_request_handler_create(ch_pool_t *mp,
									  ch_udp_context_t *udp_context,
									  ch_udp_app_pool_t *app_pool,
									  ch_udp_session_handler_t *shandler,
									  uint32_t task_id);

extern int
ch_udp_session_request_packet_handle(ch_udp_session_request_handler_t *udp_handler,ch_packet_t *pkt);


#endif /*CH_UDP_SESSION_REQUEST_HANDLER_H*/
