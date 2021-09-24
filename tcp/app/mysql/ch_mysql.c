/*
 *
 *      Filename: ch_mysql.c
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2021 10:03:29
 * Last Modified: 2021-8-29 18:21:05
 */

#include "ch_mysql.h"
#include "ch_config.h"
#include "ch_log.h"
#include "ch_list.h"
#include "ch_packet_record.h"
#include "ch_mysql_session_entry.h"
#include "ch_mysql_session.h"
#include "ch_tcp_app_util.h"
#include "ch_mpool_agent.h"

typedef struct private_mysql_context_t private_mysql_context_t;
#define MYSQL_PORTS_MAX 64

struct private_mysql_context_t {

	uint16_t mysql_ports[MYSQL_PORTS_MAX];

};

static private_mysql_context_t tmp_mysql_context,*g_mysql_context = &tmp_mysql_context;

#include "do_mysql_context.c"
#include "do_mysql_create.c"
#include "do_mysql_parse.c"
#include "do_mysql_format.c"

static ch_tcp_app_t * find_by_port_for_mysql(ch_tcp_app_t *app,ch_proto_session_store_t *pstore ch_unused,ch_packet_tcp_t *tcp_pkt){

	private_mysql_context_t *mysql_context = (private_mysql_context_t*)app->context;

	if(ch_ports_equal(mysql_context->mysql_ports,MYSQL_PORTS_MAX,tcp_pkt->src_port,tcp_pkt->dst_port))
		return app;

	return NULL;
}

static ch_tcp_app_t* find_by_content_for_mysql(ch_tcp_app_t *app ch_unused,ch_proto_session_store_t *pstre ch_unused,ch_packet_tcp_t *tcp_pkt ch_unused,
	void *data ch_unused,size_t dlen ch_unused){

    return NULL;
}

static ch_tcp_app_t mysql_app = {
    .protocol_id = PROTOCOL_MYSQL,
    .pkt_rcd_type =PKT_RECORD_TYPE_TCP_MYSQL,
    .context = NULL,
	.find_by_port = find_by_port_for_mysql ,
	.find_by_content = find_by_content_for_mysql,
    .proto_session_entry_create = do_mysql_session_entry_create,
    .proto_session_entry_clean = do_mysql_session_entry_clean,
    .proto_session_format = do_mysql_session_format,
	.request_content_parse = do_mysql_request_parse,
	.response_content_parse = do_mysql_response_parse 
};



int ch_mysql_init(ch_tcp_app_pool_t *ta_pool,const char *cfname){

	if(do_mysql_context_init(ta_pool->mp,g_mysql_context,cfname)){
	
		ch_log(CH_LOG_ERR,"Load TCP APP ssh config file:%s failed!",cfname);
		return -1;
	}


	mysql_app.context = (void*)g_mysql_context;


	ch_tcp_app_register(ta_pool,&mysql_app);

	return 0;
}


