/*
 *
 *      Filename: ch_smon_app.c
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-07-16 14:16:33
 * Last Modified: 2018-09-13 17:19:21
 */

#include "ch_smon_app.h"
#include "ch_config.h"
#include "ch_log.h"
#include "ch_fpath.h"
#include "ch_smon_session.h"
#include "ch_packet_record.h"
#include "ch_mpool_agent.h"
#include "ch_rule_engine.h"
#include "ch_net_util.h"

typedef struct private_smon_app_context_t private_smon_app_context_t;


struct private_smon_app_context_t {

    ch_rule_engine_t *rengine;
    const char *rule_json_file;
	
    const char *req_body_dir;
	const char *res_body_dir;


	int create_body_dir_type;
	
	ch_fpath_t *req_fpath;
	ch_fpath_t *res_fpath;

    size_t max_req_size;
    size_t max_res_size;
};

static  private_smon_app_context_t tmp_context,*g_mcontext = &tmp_context;


#include "do_smon_app_context.c"


static int _is_accept(ch_udp_app_t *app,ch_packet_udp_t *pkt_udp){
    
    char sbuff[32] = {0};
    char dbuff[32] = {0};

	private_smon_app_context_t *mcontext = (private_smon_app_context_t*)app->context;

    if(mcontext->rengine == NULL)
        return 0;

    if(ch_packet_rule_match(mcontext->rengine,pkt_udp->pkt)){

        ch_log(CH_LOG_INFO,"Match UDP Session Monitor rule,srcIP:%s,dstIP:%s,srcPort:%d,dstPort:%d",
                ch_ip_to_str(sbuff,32,pkt_udp->src_ip),
                ch_ip_to_str(dbuff,32,pkt_udp->dst_ip),
                pkt_udp->src_port,
                pkt_udp->dst_port);

        return 1;
    }

    return 0;
}

static ch_udp_app_session_t * _smon_app_session_create(ch_mpool_agent_t *mpa,ch_udp_app_t *app,ch_packet_udp_t *pkt_udp){

	ch_pool_t *mp;
	ch_udp_app_session_t *app_session;


	if(!_is_accept(app,pkt_udp))
		return NULL;

    if(mpa){

        mp = ch_mpool_agent_alloc(mpa);
    }else{
        mp = ch_pool_create(1024);
    }

	if(mp == NULL){

		ch_log(CH_LOG_ERR,"Cannot create memory pool for smon session!");
		return NULL;
	
	}
	
	app_session = (ch_udp_app_session_t*)ch_smon_session_create(mp,pkt_udp->dst_port);

	if(app_session){
	
		app_session->app = app;
		app_session->mp = mp;

	}

	return app_session;

}

static int _smon_is_request(ch_udp_app_session_t *app_session,ch_packet_udp_t *pkt_udp){

	ch_smon_session_t *smon_session = (ch_smon_session_t*)app_session;
	return smon_session->dst_port == pkt_udp->dst_port;
}

#define IS_NEED_CREATE_FILE_BODY(session,is_req) (is_req?(session)->req_content_fp == NULL:(session)->res_content_fp == NULL)

static inline int is_over_size(size_t max,size_t cur_size){

    if(max<=0)
        return 0;

    return cur_size>=max;
}

static inline int _do_content_process(ch_smon_session_t *smon_session,private_smon_app_context_t *context,
	void *data,size_t dlen,int is_req){

    int is_over;
	char *fname,*fname_tmp;

	ch_fpath_t *fpath = is_req?context->req_fpath:context->res_fpath;
    
    is_over = is_req?is_over_size(context->max_req_size,smon_session->cur_req_size):is_over_size(context->max_res_size,smon_session->cur_res_size);

    if(is_over){

        ch_log(CH_LOG_INFO,"UDP Session Monitor %s content is over,max:%lu,cur:%lu",is_req?"Request":"Response",
                (unsigned long)(is_req?context->max_req_size:context->max_res_size),
                (unsigned long)(is_req?smon_session->cur_req_size:smon_session->cur_res_size));

        return 0;
    }

	if(IS_NEED_CREATE_FILE_BODY(smon_session,is_req)){
	
		ch_fpath_name_create(fpath,&fname,&fname_tmp);
	
		if(fname == NULL){
		
			ch_log(CH_LOG_ERR,"Can not create a file used to store smon body!");
			return -1;
		}

		if(ch_smon_session_content_fpath_init(smon_session,fname,is_req)){
		
			ch_log(CH_LOG_ERR,"Can not open a file used to store smon body,fname:%s",fname);
			return -1;

		}
	}

	ch_smon_session_content_write(smon_session,data,dlen,is_req);

	return 0;
}

static inline int _is_parse_ok(private_smon_app_context_t *mcontext,ch_smon_session_t *smon_session){

    return is_over_size(mcontext->max_req_size,smon_session->cur_req_size)&&is_over_size(mcontext->max_res_size,smon_session->cur_res_size);

}

static int _smon_req_pkt_process(ch_udp_app_session_t *app_session,ch_packet_udp_t *pkt_udp){

	private_smon_app_context_t *context = (private_smon_app_context_t*)app_session->app->context;

	ch_smon_session_t *smon_session = (ch_smon_session_t*)app_session;

	if(pkt_udp->pdata == NULL || pkt_udp->payload_len == 0){
	
		return PROCESS_CONTINUE;
	}
	
	if(_do_content_process(smon_session,context,pkt_udp->pdata,pkt_udp->payload_len,1)){
	
		return PROCESS_ERR;
	}
    
    if(_is_parse_ok(context,smon_session)){

        ch_log(CH_LOG_INFO,"Parse UDP Session Monitor is ok,maxReqSize:%lu,maxResSize:%lu,curReqSize:%lu,curResSize:%lu",
                (unsigned long)context->max_req_size,
                (unsigned long)context->max_res_size,
                (unsigned long)smon_session->cur_req_size,
                (unsigned long)smon_session->cur_res_size);

        return PROCESS_DONE;
    }

	return PROCESS_CONTINUE;

}

static int _smon_res_pkt_process(ch_udp_app_session_t *app_session,ch_packet_udp_t *pkt_udp){

	private_smon_app_context_t *context = (private_smon_app_context_t*)app_session->app->context;

	ch_smon_session_t *smon_session = (ch_smon_session_t*)app_session;

	if(pkt_udp->pdata == NULL || pkt_udp->payload_len == 0){
	
		return PROCESS_CONTINUE;
	}
	
	if(_do_content_process(smon_session,context,pkt_udp->pdata,pkt_udp->payload_len,0)){
	
		return PROCESS_ERR;
	}
    
    if(_is_parse_ok(context,smon_session)){

        ch_log(CH_LOG_INFO,"Parse UDP Session Monitor is ok,maxReqSize:%lu,maxResSize:%lu,curReqSize:%lu,curResSize:%lu",
                (unsigned long)context->max_req_size,
                (unsigned long)context->max_res_size,
                (unsigned long)smon_session->cur_req_size,
                (unsigned long)smon_session->cur_res_size);

        return PROCESS_DONE;
    }

	return PROCESS_CONTINUE;

}

static ssize_t _smon_session_write(ch_udp_app_session_t *app_session,ch_data_output_t *dout){

	ssize_t rc,len = 0;

	ch_smon_session_t *smon_session = (ch_smon_session_t*)app_session;

	CH_DOUT_UINT64_WRITE(dout,0,len,rc);

	rc = ch_smon_session_write(smon_session,dout);

	if(rc == -1)
		return -1;

	return len+rc;

}

static int _smon_session_store(ch_udp_app_session_t *app_session,ch_msgpack_store_t *dstore){

	ch_smon_session_t *smon_session = (ch_smon_session_t*)app_session;

	return ch_smon_session_store(smon_session,dstore,0);
}

static void _smon_session_dump(ch_udp_app_session_t *app_session,FILE *fp){

	ch_smon_session_t *smon_session = (ch_smon_session_t*)app_session;

	ch_smon_session_dump(smon_session,fp);

}

static void _smon_session_fin(ch_mpool_agent_t *mpa,ch_udp_app_session_t *app_session){

	ch_smon_session_t *smon_session = (ch_smon_session_t*)app_session;

	ch_smon_session_destroy(smon_session);

    if(mpa)
    {
        ch_mpool_agent_free(mpa,smon_session->mp);
    }else{
        ch_pool_destroy(smon_session->mp);
    }
}

static ch_udp_app_t smon_app = {
	.context = NULL,
	.type = PKT_RECORD_TYPE_UDP_SMON, 
	.app_session_create = _smon_app_session_create,
	.process_request_session = NULL,
	.request_session_equal = NULL,
	.is_request = _smon_is_request,
	.req_pkt_process = _smon_req_pkt_process,
	.res_pkt_process = _smon_res_pkt_process,
	.app_session_write = _smon_session_write,
	.app_session_store = _smon_session_store,
	.app_session_dump = _smon_session_dump,
    .app_session_isMyProto = NULL,
    .app_session_target_get = NULL,
	.app_session_fin = _smon_session_fin
};


int ch_smon_app_init(ch_udp_app_pool_t *upool,const char *cfname){

	if(do_smon_app_context_init(upool->mp,g_mcontext,cfname)){
	
		ch_log(CH_LOG_ERR,"Load UDP APP Smon config file:%s failed!",cfname);
		return -1;
	}
    
    g_mcontext->rengine = ch_rule_engine_create(upool->mp,g_mcontext->rule_json_file);

	if(g_mcontext->rengine == NULL){
	
		ch_log(CH_LOG_ERR,"Cannot load udp session monitor rules:%s",g_mcontext->rule_json_file);
		return -1;
	}

	g_mcontext->req_fpath = ch_fpath_create(upool->mp,g_mcontext->req_body_dir,
		g_mcontext->create_body_dir_type,0,0);
	
	g_mcontext->res_fpath = ch_fpath_create(upool->mp,g_mcontext->res_body_dir,
		g_mcontext->create_body_dir_type,0,0);

	if(g_mcontext->req_fpath == NULL || g_mcontext->res_fpath == NULL){
	
		ch_log(CH_LOG_ERR,"Cannot create file path for store smon body!");
		return -1;
	}
	
	smon_app.context = (void*)g_mcontext;
    smon_app.app_pool = upool;

	ch_udp_app_register(upool,&smon_app);

	return 0;
}

