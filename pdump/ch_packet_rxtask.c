/*
 * =====================================================================================
 *
 *       Filename:  ch_packet_rxtask.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/03/2017 04:13:30 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  shajf (jacks), csp001314@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ch_log.h"
#include "ch_packet_rxtask.h"
#include "ch_packet.h"
#include "ch_util.h"
#include "ch_packet_tcp.h"
#include "ch_packet_udp.h"

#define PREFETCH_OFFSET 3

static void _dump_port(ch_packet_rxtask_t *prxtask,ch_port_t *port){

	uint64_t cur_time = ch_get_current_timems()/1000;

	if(cur_time-prxtask->last_dump_time>3*60){
	
		prxtask->last_dump_time = cur_time;

		ch_port_pool_stat_dump(port,stdout);

		fprintf(stdout,"Dump The Mempool info:--------------------\n");

		rte_mempool_dump(stdout,port->ppool->pktmbuf_pool);

		fflush(stdout);
	}

}

static int _filter_isMyProto(ch_rule_target_context_t *tcontext,int proto){
 
     tcontext = tcontext;
 
     return proto == PROTO_PKT;
}

static int _pkt_is_accept(ch_pdump_context_t *pdcontext,ch_packet_t *pkt){

    ch_rule_target_context_t target_tmp,*rtcontext = &target_tmp;
    ch_packet_rule_context_t tmp,*pcontext = &tmp;

    ch_filter_engine_t *filter_engine = pdcontext->filter_engine;

    if(filter_engine == NULL)
        return 1;

    pcontext->pkt = pkt;

    rtcontext->proto = "pkt";
    rtcontext->data = (void*)pcontext;
    rtcontext->isMyProto = _filter_isMyProto;
    rtcontext->target = ch_packet_target_get;

    return ch_filter_engine_accept(filter_engine,rtcontext);
}

static void _pkt_stat_handle(ch_stat_pool_t *st_pool,ch_packet_t *pkt,uint64_t time){

    int stat_pkt_type = STAT_OTHER;
	uint64_t pkt_size = ch_packet_size(pkt);

	switch(pkt->pkt_type){
	
		case PKT_TYPE_TCP:
			stat_pkt_type = STAT_TCP;
			break;

		case PKT_TYPE_UDP:
			stat_pkt_type = STAT_UDP;
            break;

		case PKT_TYPE_ARP:

			stat_pkt_type = STAT_ARP;
			break;

		case PKT_TYPE_ICMP:

			stat_pkt_type = STAT_ICMP;
			break;
		default:

			break;

	}
	
    ch_stat_pool_handle(st_pool,time,pkt_size,stat_pkt_type);

}

static void _packets_merge(ch_packet_rxtask_t *ptask,ch_packet_t *pkt){

    size_t dlen = 0;
    size_t plen = pkt->mbuf->pkt_len;

    if(ptask->pbuf == NULL||ptask->pbuf_size <=plen){

        ch_log(CH_LOG_INFO,"Need update packet buffer,curSize:%lu,this packet size:%lu,nb_segs:%lu",
                (unsigned long)ptask->pbuf_size,
                (unsigned long)plen,(unsigned long)pkt->mbuf->nb_segs);

        void *nbuf = malloc(plen+32);
        if(nbuf == NULL){

            ch_log(CH_LOG_ERR,"Cannot alloc memory for merget more packets!");
            return;
        }

        if(ptask->pbuf){

            free(ptask->pbuf);
            ptask->pbuf_size = 0;
        }

        ptask->pbuf_size = plen+32;
        ptask->pbuf = nbuf;
    }

    dlen = ch_packets_merge(ptask->pbuf,ptask->pbuf_size,pkt);

    ch_log(CH_LOG_INFO,"Merge packets ok,pktLen:%lu,data_len:%lu,nb_segs:%lu",
            (unsigned long)tcp_pkt->pkt->mbuf->pkt_len,
            (unsigned long)dlen,
            (unsigned long)pkt->mbuf->nb_segs);


}


static void _pkt_handle(ch_packet_rxtask_t *prxtask,ch_port_queue_t *pq ch_unused,struct rte_mbuf *mbuf,uint64_t time){


	int rc;

	//printf("Receive packet,task_id:%d,packet_len:%d,datalen:%d\,port:%d,queue:%d\n",prxtask->task.tsk_id,mbuf->pkt_len,mbuf->data_len,pq->port->port_id,pq->queue_id);

	ch_packet_t *pkt = ch_packet_get_from_mbuf(mbuf);

	if(unlikely(pkt == NULL)){
	
		rte_pktmbuf_free(mbuf);
		return;
	}

	/*parse packet*/
	rc = ch_packet_parse(pkt,mbuf);
    
    _pkt_stat_handle(prxtask->pdcontext->st_pool,pkt,time);
	
    if(rc != PKT_PARSE_OK)
	{
	
		rte_pktmbuf_free(mbuf);
		return;
	}

    if(_pkt_is_accept(prxtask->pdcontext,pkt)==0){

        ch_log(CH_LOG_INFO,"PKT match packet filter rule,will pass it,pkt.type:%d,l3.proto:%d,l4.proto:%d",pkt->pkt_type,pkt->l3_proto,pkt->l4_proto); 
        rte_pktmbuf_free(mbuf);
        return;
    }

    /*write to pcap file*/
    if(mbuf->nb_segs == 1){
        ch_pcap_writer_put(prxtask->pwriter,rte_pktmbuf_mtod(mbuf,void*),mbuf->pkt_len);
    }else{

        _packets_merge(prxtask,pkt);
        ch_pcap_writer_put(prxtask->pwriter,prxtask->pbuf,mbuf->pkt_len);
    }

    rte_pktmbuf_free(mbuf);
}

static uint64_t _read_pkt_from_port_queue(ch_packet_rxtask_t *prxtask,ch_port_queue_t *pq,uint64_t time){
    
    uint64_t packets = 0;
    int j,nb_rx;

    struct rte_mbuf **pkts_burst = prxtask->rx_pkts_burst;

	ch_port_t *port = pq->port;

	nb_rx = rte_eth_rx_burst(port->port_id,pq->queue_id,pkts_burst,port->ppool->pcontext->port_max_pkt_burst);
	
	//_dump_port(prxtask,pq->port);
	
	if(nb_rx == 0){
		return 0;
	}

	packets+=nb_rx;

	/* Prefetch first packets */
	for (j = 0; j < PREFETCH_OFFSET && j < nb_rx; j++) {
		rte_prefetch0(rte_pktmbuf_mtod(
				pkts_burst[j], void *));
	}

	/* Prefetch and process already prefetched packets */
	for (j = 0; j < (nb_rx - PREFETCH_OFFSET); j++) {
		rte_prefetch0(rte_pktmbuf_mtod(pkts_burst[
				j + PREFETCH_OFFSET], void *));
	   
		_pkt_handle(prxtask,pq,pkts_burst[j],time);
	}
	
	/* Process remaining prefetched packets */
	for (; j < nb_rx; j++) {
		_pkt_handle(prxtask,pq,pkts_burst[j],time);
	}

    return packets;
}

/*packet receive task callback function,will receive packets from specified port queues*/
static int _packet_receive_task_run(ch_task_t *task,void *priv_data ch_unused){

   ch_packet_rxtask_t *prxtask = (ch_packet_rxtask_t*)task;
   
   ch_port_queue_t *pq;

   uint64_t time = ch_get_current_timems()/1000;

   uint64_t packets = 0;

   ch_stat_pool_update(prxtask->pdcontext->st_pool);

   list_for_each_entry(pq,&prxtask->port_queues,node){
   
      packets+= _read_pkt_from_port_queue(prxtask,pq,time);
   }

   return TASK_RETURN_OK;
}

/*packet receive task callback function,will be call before receiving packets */
static int _packet_receive_task_init(ch_task_t *task ch_unused,void *priv_data ch_unused){

   return 0;
}

/*packet receive task callback function,will be call when exited*/
static int _packet_receive_task_exit(ch_task_t *task ch_unused,void *priv_data ch_unused){

   return 0;
}

/* Bind port queue to receive packets */
static int _bind_port_queues(ch_port_pool_t *ppool,ch_packet_rxtask_t *prxtask,int queue_n){

   int i;
   ch_port_queue_t *pq;

   for(i = 0;i<queue_n;i++){
   
      pq = ch_port_pool_queue_bind(ppool,prxtask->socket);
      if(pq == NULL){
      
         ch_log(CH_LOG_ERR,"Packet receive task bind port queue failed!");
         return -1;
      }

      /*add port queue to packet receive task*/
      list_add_tail(&pq->node,&prxtask->port_queues);
   }

   /* ok */
   return 0;
}

ch_task_t * ch_packet_rxtask_create(ch_pdump_context_t *pdcontext,uint32_t task_id,int queue_n){


   ch_packet_rxtask_t *prxtask = NULL;
   ch_task_t *task;

   prxtask = (ch_packet_rxtask_t*)ch_pcalloc(pdcontext->mp,sizeof(*prxtask));

   prxtask->task.tsk_id = task_id;
   prxtask->task.run = _packet_receive_task_run;
   prxtask->task.init = _packet_receive_task_init;
   prxtask->task.exit = _packet_receive_task_exit;


   prxtask->last_dump_time = ch_get_current_timems()/1000;

   prxtask->pdcontext = pdcontext;
   INIT_LIST_HEAD(&prxtask->port_queues);

   /*Bind this task into a cpu core */
   if(ch_core_pool_bind_task(pdcontext->cpool,(ch_task_t*)prxtask)){
   
      /* Bind error */
      ch_log(CH_LOG_ERR,"Cannot bind the packet receive task:%u into a cpu core!",task_id);
      return NULL;
   }
   
   task = (ch_task_t*)prxtask;

   prxtask->socket = task->core->socket;

   /*Bind port queues into this packet receive task*/
   if(_bind_port_queues(pdcontext->ppool,prxtask,queue_n)){
   
      /*Error*/
      ch_log(CH_LOG_ERR,"Cannot bind port queues:%d into packet receive task:%u",queue_n,task_id);
      return NULL;
   }

   /*alloc some memory to store packet buf received*/
   prxtask->rx_pkts_burst = (struct rte_mbuf**)malloc(pdcontext->ppool->pcontext->port_max_pkt_burst*sizeof(struct rte_mbuf*));
   if(prxtask->rx_pkts_burst == NULL){
   
      ch_log(CH_LOG_ERR,"No memory alloced for packet receive task:%u",task_id);
      return NULL;
   }

   prxtask->pwriter = ch_pcap_writer_create(pdcontext->mp,pdcontext->pcap_dir,pdcontext->loop_bytes);

   if(prxtask->pwriter == NULL){

       ch_log(CH_LOG_ERR,"Create Pcap writer failed!");
       return NULL;
   }

    prxtask->pbuf = malloc(64*1024);
    if(shandler->pbuf == NULL)
        shandler->pbuf_size = 0;
    else
        shandler->pbuf_size = 64*1024;

   /*ok!*/
   return (ch_task_t*)prxtask;
}


