/*
 * =====================================================================================
 *
 *       Filename:  ch_pdump_context.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/06/2017 03:22:18 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  shajf (jacks), csp001314@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "ch_constants.h"
#include "ch_log.h"
#include "ch_config.h"
#include "ch_pdump_context.h"
#include "ch_packet.h"

#define PORT_POOL_NAME "pdump_mpool"

static void do_pdcontext_init(ch_pdump_context_t *pdcontext){

    pdcontext->cpool = NULL;
    pdcontext->ppool = NULL;
	pdcontext->rxtask_pool = NULL;

    pdcontext->log_name = NULL;
    pdcontext->log_level = CH_LOG_ERR;
    
    pdcontext->port_cfname = NULL; 

	pdcontext->packet_rxtasks_n = 1;

    pdcontext->stat_mmap_fname = "/opt/data/cloudhands/store/sa_stat.data";
	pdcontext->stat_time_up = 7*24*3600;
	pdcontext->stat_time_tv = 5*60;

    pdcontext->filter_json_file = NULL;

    pdcontext->pcap_dir = "/opt/data/pcap";
    pdcontext->loop_bytes = 256*1024*1024;

    pdcontext->psink = NULL;
    pdcontext->use_psink = 0;
    pdcontext->psink_mpool_size = 4096;
    pdcontext->psink_queue_size = 65536;
}

static const char *cmd_log(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1,const char *p2){

    ch_pdump_context_t  *pdcontext = (ch_pdump_context_t *)_dcfg;

    pdcontext->log_name = p1;

    if(strcasecmp(p2,"info") == 0){
        pdcontext->log_level = CH_LOG_INFO;
    }else if(strcasecmp(p2,"debug") == 0){
        pdcontext->log_level = CH_LOG_DEBUG;
    }else if(strcasecmp(p2,"notice") == 0){
        pdcontext->log_level = CH_LOG_NOTICE;
    }else if(strcasecmp(p2,"warn") == 0){
        pdcontext->log_level = CH_LOG_WARN;
    }else if(strcasecmp(p2,"error") == 0){
        pdcontext->log_level = CH_LOG_ERR;
    }else if(strcasecmp(p2,"crit") == 0){
        pdcontext->log_level = CH_LOG_CRIT;
    }else if(strcasecmp(p2,"alert") == 0){
        pdcontext->log_level = CH_LOG_ALERT;
    }else if(strcasecmp(p2,"emerg") == 0){
        pdcontext->log_level = CH_LOG_EMERG;
    }else {

        return "unknown log level name!";
    }

    return NULL;
}

static const char *cmd_port_cfname(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    ch_pdump_context_t *pdcontext = (ch_pdump_context_t*)_dcfg;

    pdcontext->port_cfname  = p1;
    
    return NULL;
}

static const char *cmd_port_rx_mask(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    char *endptr;

    ch_pdump_context_t *pdcontext = (ch_pdump_context_t*)_dcfg;

    pdcontext->port_mask = strtoul(p1,&endptr,16);
    
    return NULL;
}

static const char *cmd_packet_rxtasks(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    char *endptr;

    ch_pdump_context_t *pdcontext = (ch_pdump_context_t*)_dcfg;

    pdcontext->packet_rxtasks_n = strtoul(p1,&endptr,10);

	if(pdcontext->packet_rxtasks_n<1){
	
		return "The number of packet receive tasks must gt:1!";
	}

    return NULL;
}

static const char *cmd_stat_mmap_fname(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    ch_pdump_context_t *context = (ch_pdump_context_t*)_dcfg;

	context->stat_mmap_fname = p1;

    return NULL;
}

static const char *cmd_stat_timeup(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    char *endptr;

    ch_pdump_context_t *context = (ch_pdump_context_t*)_dcfg;

    context->stat_time_up = (uint64_t)strtoul(p1,&endptr,10);
    
    return NULL;
}

static const char *cmd_stat_timetv(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    char *endptr;

    ch_pdump_context_t *context = (ch_pdump_context_t*)_dcfg;

    context->stat_time_tv = (uint64_t)strtoul(p1,&endptr,10);
    
    return NULL;
}

static const char *cmd_filter_json_file(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    ch_pdump_context_t *context = (ch_pdump_context_t*)_dcfg;

    context->filter_json_file = p1;

    return NULL;
}

static const char *cmd_pcap_dir(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    ch_pdump_context_t *context = (ch_pdump_context_t*)_dcfg;

    context->pcap_dir = p1;

    return NULL;
}

static const char *cmd_pcap_loop_bytes(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    char *endptr;

    ch_pdump_context_t *context = (ch_pdump_context_t*)_dcfg;

    context->loop_bytes = (size_t)strtoul(p1,&endptr,10);

    return NULL;
}

static const char *cmd_use_psink(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    ch_pdump_context_t *context = (ch_pdump_context_t*)_dcfg;

    context->use_psink = 0;

    if(strcasecmp(p1,"true")==0)
        context->use_psink = 1;


    return NULL;
}

static const char *cmd_psink_mempool_size(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    char *endptr;

    ch_pdump_context_t *context = (ch_pdump_context_t*)_dcfg;

    context->psink_mpool_size = (size_t)strtoul(p1,&endptr,10);
    
    return NULL;
}

static const char *cmd_psink_queue_size(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    char *endptr;

    ch_pdump_context_t *context = (ch_pdump_context_t*)_dcfg;

    context->psink_queue_size = (size_t)strtoul(p1,&endptr,10);
    
    return NULL;
}

static const command_rec pdcontext_directives[] ={
    
    CH_INIT_TAKE2(
            "CHLog",
            cmd_log,
            NULL,
            0,
            "set log name and level"
            ),
     
    CH_INIT_TAKE1(
            "CHPortCFName",
            cmd_port_cfname,
            NULL,
            0,
            "set the port config file path"
            ),
	
    CH_INIT_TAKE1(
            "CHRXTasks",
            cmd_packet_rxtasks,
            NULL,
            0,
            "set packet receive task's number"
            ),

	CH_INIT_TAKE1(
            "CHPortRxMask",
            cmd_port_rx_mask,
            NULL,
            0,
            "set port rx mask"
            ),
	
    CH_INIT_TAKE1(
            "CHStatMMapFName",
            cmd_stat_mmap_fname,
            NULL,
            0,
            "set the statistic mmap file name"
            ),
	
	CH_INIT_TAKE1(
            "CHStatTimeUP",
            cmd_stat_timeup,
            NULL,
            0,
            "set the statistic time up"
            ),

	CH_INIT_TAKE1(
            "CHStatTimeTV",
            cmd_stat_timetv,
            NULL,
            0,
            "set the statistic time tv"
            ),
	
    CH_INIT_TAKE1(
            "CHFilterJsonFile",
            cmd_filter_json_file,
            NULL,
            0,
            "set packet filter rule json file"
            ),
    
    CH_INIT_TAKE1(
            "CHPcapDir",
            cmd_pcap_dir,
            NULL,
            0,
            "set pcap file store dir"
            ),
    
    CH_INIT_TAKE1(
            "CHPcapLoopBytes",
            cmd_pcap_loop_bytes,
            NULL,
            0,
            "set per pcap file max size"
            ),
    
    CH_INIT_TAKE1(
            "CHUsePSink",
            cmd_use_psink,
            NULL,
            0,
            "set use process sink to multi process true/false"
            ),
    
    CH_INIT_TAKE1(
            "CHPSinkMemPoolSize",
            cmd_psink_mempool_size,
            NULL,
            0,
            "set the process sink memory pool size"
            ),
    CH_INIT_TAKE1(
            "CHPSinkQueueSize",
            cmd_psink_queue_size,
            NULL,
            0,
            "set the process sink queue  size"
            ),
};

ch_pdump_context_t * ch_pdump_context_create(ch_pool_t *mp,const char *cfname){


    ch_pdump_context_t * pdcontext = NULL;

	const char *msg;
	
    if(mp == NULL || cfname == NULL||strlen(cfname) == 0){
        return NULL;
    }

    pdcontext = (ch_pdump_context_t*)ch_pcalloc(mp,sizeof(*pdcontext));

    pdcontext->mp = mp;

    do_pdcontext_init(pdcontext);

    msg = ch_process_command_config(pdcontext_directives,(void*)pdcontext,mp,mp,cfname);

    /*config failed*/
    if(msg!=NULL){

        printf("%s\n",msg);
        return NULL;
    }

    return pdcontext;
}

static int _check_cores_ports(ch_core_pool_t *cpool,ch_port_pool_t *ppool){

	if(cpool->config_core_count<=0){
	
		ch_log(CH_LOG_ERR,"no cpu core configged to receive packets!");
		return -1;
	}
	
	if(ppool->port_n<=0||ppool->port_queue_n<=0){
	
		ch_log(CH_LOG_ERR,"no port configged to receive packets!");
		return -1;
	}

	/*ok!*/
	return 0;
}


int ch_pdump_context_start(ch_pdump_context_t *pdcontext){

	/*create cpu core pool*/
	pdcontext->cpool = ch_core_pool_create(pdcontext->mp,NULL);
	if(pdcontext->cpool == NULL){
	
		ch_log(CH_LOG_ERR,"Create cpu core pool failed!");
		return -1;
	}

	/*create packet recieve port pool*/
	pdcontext->ppool = ch_port_pool_create(pdcontext->mp,pdcontext->port_cfname,
		PORT_POOL_NAME,
		NULL,
		pdcontext->port_mask);

	if(pdcontext->ppool == NULL){

		ch_log(CH_LOG_ERR,"Create packet receive port pool failed!");
		return -1;
	}

	if(_check_cores_ports(pdcontext->cpool,pdcontext->ppool)){

		/*failed*/
		ch_log(CH_LOG_ERR,"cpu cores and packet receive ports config is invalid,please reconfig them!");
		return -1;
	}

	/*init packet*/
	ch_packet_init();
	
    pdcontext->st_pool = ch_stat_pool_create(pdcontext->mp,pdcontext->stat_mmap_fname,
		pdcontext->stat_time_up,pdcontext->stat_time_tv);

	if(pdcontext->st_pool == NULL){
	
		ch_log(CH_LOG_ERR,"Cannot create statistic pool!");
		return -1;
	}

	/*Create packet receive task pool*/
	pdcontext->rxtask_pool = ch_packet_rxtask_pool_create(pdcontext);

	if(pdcontext->rxtask_pool == NULL){
	
		ch_log(CH_LOG_ERR,"create packet receive task pool failed!");
		return -1;
	}

    /*create filter engine*/
    pdcontext->filter_engine = ch_filter_engine_create(pdcontext->mp,pdcontext->filter_json_file);
	if(pdcontext->filter_engine == NULL){
	
		ch_log(CH_LOG_ERR,"create filter engine failed!");
		return -1;
	}

    /*create process sink*/
    pdcontext->psink = NULL;
    if(pdcontext->use_psink){

        pdcontext->psink = ch_process_sink_create_for_write(pdcontext->mp,
                pdcontext->pcap_dir,
                pdcontext->psink_mpool_size,
                pdcontext->psink_queue_size);

        if(pdcontext->psink == NULL){

            ch_log(CH_LOG_ERR,"Cannot create process psink-----------");
            return NULL;
        }

    }

	/*start all ports*/
	if(ch_port_pool_setup(pdcontext->ppool)){
	
		ch_log(CH_LOG_ERR,"setup ports pool failed!");
		return -1;
	}

	/*start all cpu cores to start receive packets from ports*/
	if(ch_core_pool_slaves_setup(pdcontext->cpool,NULL)){
	
		ch_log(CH_LOG_ERR,"setup all slave threads failed!");
		return -1;
	}


	ch_core_pool_wait_for_slaves(pdcontext->cpool);
	/*ok!*/

	return 0;
}

void ch_pdump_context_stop(ch_pdump_context_t *pdcontext ch_unused){


}

