/*
 *
 *      Filename: ch_http.c
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-07-12 16:21:25
 * Last Modified: 2018-09-25 16:00:33
 */

#include "ch_http.h"
#include "ch_config.h"
#include "ch_util.h"
#include "ch_tcp_app_util.h"
#include "ch_log.h"
#include "ch_list.h"
#include "ch_http_session.h"
#include "ch_tcp_app_pool.h"
#include "ch_http_session_entry.h"
#include "ch_packet_record.h"
#include "ch_filter_engine.h"
#include "ch_rule_utils.h"
#include "ch_rule_constants.h"
#include "ch_rule_engine.h"
#include "ch_mpool_agent.h"

typedef struct private_http_context_t private_http_context_t;

struct private_http_context_t {

	const char *req_body_dir;
	const char *res_body_dir;

	int create_dir_type;

    const char *filter_json_file;

    ch_filter_engine_t *filter_engine;

    ch_rule_engine_t *rengine;

    const char *rule_json_file;
};

static  private_http_context_t tmp_context,*g_hcontext = &tmp_context;


#include "do_http_context.c"
#include "do_http_create.c"
#include "do_http_format.c"
#include "do_http_parse.c"

static ch_tcp_app_t * find_by_port_for_http(ch_tcp_app_t *app,ch_proto_session_store_t *pstore ch_unused,ch_packet_tcp_t *tcp_pkt){

    char sbuff[32]={0};
    char dbuff[32]={0};

    private_http_context_t *hcontext = (private_http_context_t*)app->context;

    if(hcontext->rengine == NULL){
        return NULL;
    }
        

    if(ch_packet_rule_match(hcontext->rengine,tcp_pkt->pkt)){

        ch_log(CH_LOG_INFO,"Match Http Session  rule,srcIP:%s,dstIP:%s,srcPort:%d,dstPort:%d",
                ch_ip_to_str(sbuff,32,tcp_pkt->src_ip),
                ch_ip_to_str(dbuff,32,tcp_pkt->dst_ip),
                tcp_pkt->src_port,
                tcp_pkt->dst_port);

        return app;
    }

	return NULL;
}

static ch_tcp_app_t* find_by_content_for_http(ch_tcp_app_t *app ch_unused,ch_proto_session_store_t *pstre ch_unused,ch_packet_tcp_t *tcp_pkt ch_unused,
	void *data ch_unused,size_t dlen ch_unused){

    return NULL;
}


static ch_tcp_app_t http_app = {
    .protocol_id = PROTOCOL_HTTP ,
    .pkt_rcd_type = PKT_RECORD_TYPE_TCP_HTTP,
    .context = NULL,
	.find_by_port = find_by_port_for_http,
	.find_by_content = find_by_content_for_http,
    .proto_session_entry_create = do_http_session_entry_create,
    .proto_session_entry_clean = do_http_session_entry_clean,
    .proto_session_format = do_http_session_format,
	.request_content_parse = do_http_request_parse,
	.response_content_parse = do_http_response_parse 
};



int ch_http_init(ch_tcp_app_pool_t *ta_pool,const char *cfname){

	if(do_http_context_init(ta_pool->mp,g_hcontext,cfname)){
	
		ch_log(CH_LOG_ERR,"Load TCP APP Http config file:%s failed!",cfname);
		return -1;
	}
    
    g_hcontext->filter_engine = ch_filter_engine_create(ta_pool->mp,g_hcontext->filter_json_file);

    if(g_hcontext->filter_engine == NULL){

        ch_log(CH_LOG_ERR,"Cannot create filter Engine for http session filter,cfname:%s",
                g_hcontext->filter_json_file);

        return -1;
    }
    
    g_hcontext->rengine = ch_rule_engine_create(ta_pool->mp,g_hcontext->rule_json_file);

	if(g_hcontext->rengine == NULL){
	
		ch_log(CH_LOG_ERR,"Cannot load http session  rules:%s",g_hcontext->rule_json_file);
		return -1;
	}


	http_app.context = (void*)g_hcontext;

	ch_tcp_app_register(ta_pool,&http_app);

	return 0;
}

