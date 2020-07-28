/*
 *
 *      Filename: ch_dns_rdata.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-04-25 16:46:19
 * Last Modified: 2018-04-25 16:46:19
 */

#ifndef CH_DNS_RDATA_H
#define CH_DNS_RDATA_H

typedef struct ch_dns_rdata_t ch_dns_rdata_t;

#include "ch_dns_data_input.h"
#include "ch_dns_name.h"
#include "ch_list.h"
#include "ch_util.h"

struct ch_dns_rdata_t {

	struct list_head node;

	ch_dns_name_t name;

	uint16_t  type;
	uint16_t dclass;

	uint32_t ttl;
	void *data;
	uint16_t dlen;

    uint16_t pos;
};


extern ch_dns_rdata_t * ch_dns_rdata_parse(ch_pool_t *mp,ch_dns_data_input_t *din);

extern void ch_dns_rdata_dump(ch_dns_rdata_t *rdata,FILE *fp);

extern void ch_dns_rdata_store(ch_dns_rdata_t *rdata,ch_msgpack_store_t *dstore);

extern ssize_t ch_dns_rdata_write(ch_dns_rdata_t *rdata,ch_data_output_t *dout);


#define CH_DNS_RDATA_WRITE(dout,rdata,len,rc) do {  \
	if(-1 == (rc = ch_dns_rdata_write(rdata,dout))) \
		return -1;                                  \
	len+=rc;                                        \
}while(0)

#endif /*CH_DNS_RDATA_H*/
