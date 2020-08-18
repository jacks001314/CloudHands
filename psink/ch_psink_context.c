/*
 * =====================================================================================
 *
 *       Filename:  ch_psink_context.c
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
#include "ch_psink_context.h"
#include "ch_packet.h"

static void do_psink_context_init(ch_psink_context_t *psink_context){

    psink_context->cpool = NULL;
	psink_context->pint_tcp_context = NULL;
	psink_context->pint_sa_context = NULL;
	psink_context->pint_udp_context = NULL;

    psink_context->log_name = NULL;
    psink_context->log_level = CH_LOG_INFO;


	psink_context->pint_tcp_cfname = NULL;

	psink_context->pint_sa_cfname = NULL;
	
	psink_context->pint_udp_cfname = NULL;

    psink_context->psink = NULL;
    psink_context->pcap_dir = NULL;
}

static const char *cmd_log(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1,const char *p2){

    ch_psink_context_t  *psink_context = (ch_psink_context_t *)_dcfg;

    psink_context->log_name = p1;

    if(strcasecmp(p2,"info") == 0){
        psink_context->log_level = CH_LOG_INFO;
    }else if(strcasecmp(p2,"debug") == 0){
        psink_context->log_level = CH_LOG_DEBUG;
    }else if(strcasecmp(p2,"notice") == 0){
        psink_context->log_level = CH_LOG_NOTICE;
    }else if(strcasecmp(p2,"warn") == 0){
        psink_context->log_level = CH_LOG_WARN;
    }else if(strcasecmp(p2,"error") == 0){
        psink_context->log_level = CH_LOG_ERR;
    }else if(strcasecmp(p2,"crit") == 0){
        psink_context->log_level = CH_LOG_CRIT;
    }else if(strcasecmp(p2,"alert") == 0){
        psink_context->log_level = CH_LOG_ALERT;
    }else if(strcasecmp(p2,"emerg") == 0){
        psink_context->log_level = CH_LOG_EMERG;
    }else {

        return "unknown log level name!";
    }

    return NULL;
}

static const char *cmd_pint_tcp_cfname(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){


    ch_psink_context_t *psink_context = (ch_psink_context_t*)_dcfg;

    psink_context->pint_tcp_cfname = p1;

    return NULL;
}

static const char *cmd_pint_udp_cfname(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){


    ch_psink_context_t *psink_context = (ch_psink_context_t*)_dcfg;

    psink_context->pint_udp_cfname = p1;

    return NULL;
}

static const char *cmd_pint_sa_cfname(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){


    ch_psink_context_t *psink_context = (ch_psink_context_t*)_dcfg;

    psink_context->pint_sa_cfname = p1;

    return NULL;
}


static const char *cmd_psink_pcap_dir(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    ch_psink_context_t *context = (ch_psink_context_t*)_dcfg;

    context->pcap_dir = p1;

    return NULL;
}

static const char *cmd_pkt_mbuf_size(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    char *endptr;

    ch_psink_context_t *context = (ch_psink_context_t*)_dcfg;

    context->pkt_mbuf_size = (size_t)strtoul(p1,&endptr,10);

    return NULL;
}

static const char *cmd_pkt_mbuf_num(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    char *endptr;

    ch_psink_context_t *context = (ch_psink_context_t*)_dcfg;

    context->pkt_mbuf_num = (size_t)strtoul(p1,&endptr,10);

    return NULL;
}

static const command_rec psink_context_directives[] ={
    
    CH_INIT_TAKE2(
            "CHLog",
            cmd_log,
            NULL,
            0,
            "set log name and level"
            ),
     
	CH_INIT_TAKE1(
            "CHTCPPintCFName",
            cmd_pint_tcp_cfname,
            NULL,
            0,
            "set process interface tcp context  config file path"
            ),

	CH_INIT_TAKE1(
            "CHSAPintCFName",
            cmd_pint_sa_cfname,
            NULL,
            0,
            "set process interface sa context  config file path"
            ),

	CH_INIT_TAKE1(
            "CHUDPPintCFName",
            cmd_pint_udp_cfname,
            NULL,
            0,
            "set process interface udp context  config file path"
            ),

    CH_INIT_TAKE1(
            "CHPcapDir",
            cmd_psink_pcap_dir,
            NULL,
            0,
            "set the pcap file dir"
            ),
    
    CH_INIT_TAKE1(
            "CHPktMBufSize",
            cmd_pkt_mbuf_size,
            NULL,
            0,
            "set the packet mbuf size"
            ),
    
    CH_INIT_TAKE1(
            "CHPktMBufNum",
            cmd_pkt_mbuf_num,
            NULL,
            0,
            "set the packet mbuf number"
            ),
};

ch_psink_context_t * ch_psink_context_create(ch_pool_t *mp,const char *cfname){


    ch_psink_context_t * psink_context = NULL;

	const char *msg;
	
    if(mp == NULL || cfname == NULL||strlen(cfname) == 0){
        return NULL;
    }

    psink_context = (ch_psink_context_t*)ch_pcalloc(mp,sizeof(*psink_context));

    psink_context->mp = mp;

    do_psink_context_init(psink_context);

    msg = ch_process_command_config(psink_context_directives,(void*)psink_context,mp,mp,cfname);

    /*config failed*/
    if(msg!=NULL){

        printf("%s\n",msg);
        return NULL;
    }

    return psink_context;
}

int ch_psink_context_start(ch_psink_context_t *psink_context){

	psink_context->pint_tcp_context = ch_process_interface_tcp_context_create(psink_context->mp,
		psink_context->pint_tcp_cfname,1); 

	if(psink_context->pint_tcp_context == NULL){
	
		ch_log(CH_LOG_ERR,"Create process tcp interface context failed!");
		return -1;
	}
	

	psink_context->pint_sa_context = ch_process_interface_sa_context_create(psink_context->mp,
		psink_context->pint_sa_cfname,1); 

	if(psink_context->pint_sa_context == NULL){
	
		ch_log(CH_LOG_ERR,"Create process sa interface context failed!");
		return -1;
	}
	
	psink_context->pint_udp_context = ch_process_interface_udp_context_create(psink_context->mp,
		psink_context->pint_udp_cfname,1); 

	if(psink_context->pint_udp_context == NULL){
	
		ch_log(CH_LOG_ERR,"Create process udp interface context failed!");
		return -1;
	}
	
	/*create cpu core pool*/
	psink_context->cpool = ch_core_pool_create(psink_context->mp,NULL);
	if(psink_context->cpool == NULL){
	
		ch_log(CH_LOG_ERR,"Create cpu core pool failed!");
		return -1;
	}

	/*create packet recieve port pool*/
	psink_context->psink = ch_process_sink_create_for_read(psink_context->mp,psink_context->pcap_dir);

	if(psink_context->psink == NULL){

		ch_log(CH_LOG_ERR,"Create process sink failed!");
		return -1;
	}

	/*init packet*/
	ch_packet_init();

	/*Create psink receive task*/
	psink_context->psink_task = ch_psink_task_create(psink_context);

	if(psink_context->psink_task == NULL){
	
		ch_log(CH_LOG_ERR,"create psink task failed!");
		return -1;
	}

	/*start all cpu cores to start receive packets from ports*/
	if(ch_core_pool_slaves_setup(psink_context->cpool,NULL)){
	
		ch_log(CH_LOG_ERR,"setup all slave threads failed!");
		return -1;
	}


	ch_core_pool_wait_for_slaves(psink_context->cpool);
	/*ok!*/

	return 0;
}

void ch_psink_context_stop(ch_psink_context_t *psink_context ch_unused){


}

