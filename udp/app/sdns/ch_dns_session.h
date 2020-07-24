/*
 *
 *      Filename: ch_dns_session.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-05-08 09:44:55
 * Last Modified: 2018-05-08 09:44:55
 */

#ifndef CH_DNS_SESSION_H
#define CH_DNS_SESSION_H

typedef struct ch_dns_session_t ch_dns_session_t;

#include "ch_mpool.h"
#include "ch_dns_request.h"
#include "ch_dns_response.h"
#include "ch_data_output.h"
#include "ch_udp_app_pool.h"
#include "ch_msgpack_store.h"

struct ch_dns_session_t {

	ch_udp_app_session_t app_session;

	ch_pool_t *mp;
	ch_dns_requst_t *dns_req;
	ch_dns_response_t *dns_res;
};

extern ch_dns_session_t * ch_dns_session_create(ch_pool_t *mp);

extern void ch_dns_session_dump(ch_dns_session_t *ds,FILE *fp);

extern ssize_t ch_dns_session_write(ch_dns_session_t *ds,ch_data_output_t *dout);

extern int ch_dns_session_store(ch_dns_session_t *ds,ch_msgpack_store_t *dstore);

extern const char *ch_dns_session_domain_get(ch_dns_session_t *ds);

#endif /*CH_DNS_SESSION_H*/
