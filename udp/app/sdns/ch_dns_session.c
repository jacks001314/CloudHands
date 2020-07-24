/*
 *
 *      Filename: ch_dns_session.c
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-05-10 14:48:56
 * Last Modified: 2018-05-10 14:54:34
 */

#include "ch_dns_session.h"

ch_dns_session_t * ch_dns_session_create(ch_pool_t *mp){


	ch_dns_session_t *dns_session = (ch_dns_session_t*)ch_palloc(mp,sizeof(*dns_session));

	dns_session->mp = mp;
	dns_session->dns_req = NULL;
	dns_session->dns_res = NULL;


	return dns_session;
}

void ch_dns_session_dump(ch_dns_session_t *ds,FILE *fp){


	fprintf(fp,"dns.session:\n");

	if(ds->dns_req == NULL){
	
		fprintf(fp,"dns.session.request:empty!");
	}else{
		ch_dns_request_dump(ds->dns_req,fp);
	}

	if(ds->dns_res == NULL){
	
		fprintf(fp,"dns.session.response:empty!");

	}else{
	
		ch_dns_response_dump(ds->dns_res,fp);
	}
}

ssize_t ch_dns_session_write(ch_dns_session_t *ds,ch_data_output_t *dout){

	ssize_t rc,len = 0;
	if(ds->dns_req == NULL &&ds->dns_res == NULL)
		return 0;

	CH_DOUT_UINT8_WRITE(dout,ds->dns_req?1:0,len,rc);
	CH_DOUT_UINT8_WRITE(dout,ds->dns_res?1:0,len,rc);
	if(ds->dns_req){
	
		CH_DNS_REQUEST_WRITE(dout,ds->dns_req,len,rc);
	}
	if(ds->dns_res){
	
		CH_DNS_RESPONSE_WRITE(dout,ds->dns_res,len,rc);

	}

	return len;
}

int ch_dns_session_store(ch_dns_session_t *ds,ch_msgpack_store_t *dstore){

	if(ds->dns_req == NULL &&ds->dns_res == NULL)
		return -1;

    int n = 3;
    if((ds->dns_req!=NULL)&&(ds->dns_res!=NULL))
        n = 4;

    ch_msgpack_store_map_start(dstore,"dns",n);
    ch_msgpack_store_write_uint8(dstore,"hasReq",ds->dns_req?1:0);
    ch_msgpack_store_write_uint8(dstore,"hasRes",ds->dns_res?1:0);
	
    if(ds->dns_req){
	
		ch_dns_request_store(dstore,ds->dns_req);
	}
	if(ds->dns_res){
	
		ch_dns_response_store(dstore,ds->dns_res);

	}

    return 0;
}

const char *ch_dns_session_domain_get(ch_dns_session_t *ds){

    ch_dns_requst_t *dns_req = ds->dns_req;
    if(dns_req == NULL)
        return NULL;

    return ch_dns_request_name_get(dns_req);
}
