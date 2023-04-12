/*
 *
 *      Filename: ch_udp_udp_session_handler.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-04-13 15:31:17
 * Last Modified: 2018-04-13 15:31:17
 */

#ifndef CH_UDP_SESSION_HANDLER_H
#define CH_UDP_SESSION_HANDLER_H

typedef struct ch_udp_session_handler_t ch_udp_session_handler_t;

#include "ch_udp_session_pool.h"
#include "ch_udp_session.h"
#include "ch_udp_context.h"
#include "ch_mpool.h"
#include "ch_mpool_agent.h"
#include "ch_shm_format.h"
#include "ch_data_output.h"
#include "ch_msgpack_store.h"

struct ch_udp_session_handler_t {


	ch_pool_t *mp;

	ch_udp_context_t *udp_context;

	ch_udp_session_pool_t *udp_pool;

    ch_mpool_agent_t *mpa;

	uint32_t task_id;

	ch_shm_format_t *shm_fmt;

	ch_data_output_t *dout;

    ch_msgpack_store_t *dstore;
};


extern ch_udp_session_handler_t *
ch_udp_session_handler_create(ch_pool_t *mp,ch_udp_context_t *udp_context,uint32_t task_id,
	ch_shm_format_t *shm_fmt,
	ch_data_output_t *dout,
	ch_msgpack_store_t *dstore);

extern int
ch_udp_session_packet_handle(ch_udp_session_handler_t *udp_handler,
	ch_udp_session_t *udp_session,ch_packet_udp_t *pkt_udp,int is_new_created);


#endif /*CH_UDP_SESSION_HANDLER_H*/
