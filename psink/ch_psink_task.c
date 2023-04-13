/*
 * =====================================================================================
 *
 *       Filename:  ch_psink_task.c
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
#include "ch_psink_task.h"
#include "ch_packet.h"
#include "ch_util.h"
#include "ch_packet_tcp.h"
#include "ch_packet_udp.h"

static void _pkt_handle(ch_psink_task_t *psink_task,struct rte_mbuf *mbuf,uint64_t time ch_unused){

	int rc;

	//printf("Receive psink,task_id:%d,psink_len:%d,datalen:%d\,port:%d,queue:%d\n",psink_task->task.tsk_id,mbuf->pkt_len,mbuf->data_len,pq->port->port_id,pq->queue_id);

	ch_packet_t *pkt = ch_packet_get_from_mbuf(mbuf);

	if(unlikely(pkt == NULL)){
	
		rte_pktmbuf_free(mbuf);
		return;
	}

	/*parse packet*/
	rc = ch_packet_parse(pkt,mbuf);
   
    if(rc != PKT_PARSE_OK)
	{
	
		rte_pktmbuf_free(mbuf);
		return;
	}

	if(pkt->pkt_type == PKT_TYPE_TCP){
		
		ch_packet_ref_count_set(pkt,2);
		
		if(ch_process_interface_put(psink_task->psink_context->pint_tcp_context->pint,pkt,1)){

			/*error*/
			ch_packet_free(pkt);
		}

	}else if(pkt->pkt_type == PKT_TYPE_UDP){
		
		ch_packet_ref_count_set(pkt,2);
		
		if(ch_process_interface_put(psink_task->psink_context->pint_udp_context->pint,pkt,1)){	
			/*error*/
			ch_packet_free(pkt);
		}
	}

	if(ch_process_interface_put(psink_task->psink_context->pint_sa_context->pint,pkt,1)){
		/*error*/
		ch_packet_free(pkt);
	}

	
}


static int
copy_jumbo_pcap(struct rte_mempool *mb_pool, struct rte_mbuf *mbuf,void *data,size_t data_len)
{
	/* Copy the first segment. */
	uint16_t len = rte_pktmbuf_tailroom(mbuf);
	struct rte_mbuf *m = mbuf;

	rte_memcpy(rte_pktmbuf_append(mbuf, len), data, len);
	data_len -= len;
	data += len;

	while (data_len > 0) {
		/* Allocate next mbuf and point to that. */
		m->next = rte_pktmbuf_alloc(mb_pool);

		if (unlikely(!m->next))
			return -1;

		m = m->next;

		/* Headroom is not needed in chained mbufs. */
		rte_pktmbuf_prepend(m, rte_pktmbuf_headroom(m));
		m->pkt_len = 0;
		m->data_len = 0;

		/* Copy next segment. */
		len = RTE_MIN(rte_pktmbuf_tailroom(m), data_len);
		rte_memcpy(rte_pktmbuf_append(m, len), data, len);

		mbuf->nb_segs++;
		data_len -= len;
		data += len;
	}

	return mbuf->nb_segs;
}

static inline uint64_t  _pcap_ts_ms(struct timeval *tv) {

    return tv->tv_sec * 1000 + tv->tv_usec/1000;
}

static void _process_pcap(ch_psink_task_t *psink_task,pcap_t *pcap,const char *pcap_file){

    struct rte_mbuf *mbuf;
    struct pcap_pkthdr header,*pkt_hdr = &header;
    void *packet;
	uint16_t buf_size;
    struct rte_mempool *mb_pool = psink_task->pktmbuf_pool;
    unsigned long pkts = 0;
    unsigned long bytes = 0;
    unsigned long min_bytes = 0;
    unsigned long max_bytes = 0;
    
    while(1){

        mbuf = rte_pktmbuf_alloc(mb_pool);
        if(mbuf == NULL){
            /*wait a mbuf */
            usleep(5);
            continue;
        }

        packet = (void*)pcap_next(pcap,pkt_hdr);
        if(packet == NULL){

            rte_pktmbuf_free(mbuf);

            ch_log(CH_LOG_INFO,"This Pcap File:%s read ok,pkts:%lu,bytes:%lu,min_bytes:%lu,max_bytes:%lu",
            pcap_file,pkts,bytes,min_bytes,max_bytes);

            break;   
        } 
       
		/* Now get the space available for data in the mbuf */
		buf_size = rte_pktmbuf_data_room_size(mb_pool) -RTE_PKTMBUF_HEADROOM;

		if (pkt_hdr->caplen <= buf_size) {
			/* pcap packet will fit in the mbuf, can copy it */
			rte_memcpy(rte_pktmbuf_mtod(mbuf, void *), packet,pkt_hdr->caplen);
			mbuf->data_len = (uint16_t)pkt_hdr->caplen;
		} else {
			/* Try read jumbo frame into multi mbufs. */
			if (copy_jumbo_pcap(mb_pool,
						       mbuf,
						       packet,
						       pkt_hdr->caplen) == -1) {
				rte_pktmbuf_free(mbuf);
				continue;
			}
		}

		mbuf->pkt_len = (uint16_t)pkt_hdr->caplen;
		mbuf->port = 0;
		
        pkts += 1;
        bytes += pkt_hdr->caplen;

        if(min_bytes == 0 ||min_bytes>pkt_hdr->caplen)
            min_bytes = pkt_hdr->caplen;

        if(max_bytes == 0||max_bytes<pkt_hdr->caplen)
            max_bytes = pkt_hdr->caplen;

                        
		_pkt_handle(psink_task,mbuf,_pcap_ts_ms(&pkt_hdr->ts));
    }
 
}

static int _psink_task_run(ch_task_t *task,void *priv_data ch_unused){

    char errbuf[PCAP_ERRBUF_SIZE];
    ch_psink_task_t *psink_task = (ch_psink_task_t*)task;
    ch_process_sink_t *psink = psink_task->psink;
    
    pcap_t *pcap;
    const char *pcap_file;
    
    while(1){

        pcap_file = ch_process_sink_pop(psink);
        if(pcap_file == NULL)
        {
            usleep(10);
            continue;
        } 
        ch_log(CH_LOG_INFO,"Receive a pcap file:%s,will read it--------",pcap_file);
    
        pcap = pcap_open_offline(pcap_file,errbuf);
        if(pcap == NULL){
            ch_log(CH_LOG_ERR,"Cannot open pcap file:%s,error:%s",pcap_file,errbuf);
            continue;
        }

        ch_log(CH_LOG_INFO,"Open pcap file:%s ok,will process it ---------",pcap_file);
        
        _process_pcap(psink_task,pcap,pcap_file);

        /*close pcap*/
        pcap_close(pcap);
        /*remove this pcap file*/
        unlink(pcap_file);       
    }

   return TASK_RETURN_OK;
}

/*psink receive task callback function,will be call before receiving psinks */
static int _psink_task_init(ch_task_t *task ch_unused,void *priv_data ch_unused){

   return 0;
}

/*psink receive task callback function,will be call when exited*/
static int _psink_task_exit(ch_task_t *task ch_unused,void *priv_data ch_unused){

   return 0;
}


ch_psink_task_t * ch_psink_task_create(ch_psink_context_t *psink_context){


   ch_psink_task_t *psink_task = NULL;

   psink_task = (ch_psink_task_t*)ch_pcalloc(psink_context->mp,sizeof(*psink_task));

   psink_task->psink = psink_context->psink;
   psink_task->task.tsk_id = 1;
   psink_task->task.run = _psink_task_run;
   psink_task->task.init = _psink_task_init;
   psink_task->task.exit = _psink_task_exit;

   psink_task->psink_context = psink_context;

   /*Bind this task into a cpu core */
   if(ch_core_pool_bind_task(psink_context->cpool,(ch_task_t*)psink_task)){
   
      /* Bind error */
      ch_log(CH_LOG_ERR,"Cannot bind the psink receive task:%u into a cpu core!",1);
      return NULL;
   }
   

   psink_task->pktmbuf_pool = rte_pktmbuf_pool_create(PKT_POOL_NAME,
                                                   psink_context->pkt_mbuf_num,
                                                   0,
                                                   RTE_ALIGN(sizeof(ch_packet_t),RTE_MBUF_PRIV_ALIGN),
                                                   psink_context->pkt_mbuf_size,
                                                   rte_socket_id());

   if(psink_task->pktmbuf_pool == NULL){

        ch_log(CH_LOG_ERR,"Cannot create packet mbuf pool,mbuf.size:%lu,mbuf.num:%lu",
            (unsigned long)psink_context->pkt_mbuf_size,
            (unsigned long)psink_context->pkt_mbuf_num);

        return NULL;
    }

   /*ok!*/
   return psink_task;
}


