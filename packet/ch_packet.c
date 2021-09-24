/*
 *
 *      Filename: ch_packet.c
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2017-11-22 10:14:18
 * Last Modified: 2018-08-01 11:44:27
 */

#include "ch_packet.h"
#include "ch_log.h"
#include "ch_list.h"
#include "ch_packet_ipv4.h"
#include "ch_packet_ipv4.h"
#include "ch_packet_tcp.h"
#include "ch_packet_udp.h"
#include "ch_packet_ether.h"
#include "ch_packet_arp.h"
#include "ch_packet_icmp.h"
#include "ch_packet_ipv6.h"
#include "ch_rule_constants.h"
#include "ch_rule_utils.h"
#include "ch_string.h"

static struct list_head packet_parsers[L_MAX];

void ch_packet_init(void){

	int i;

	for(i =0;i<L_MAX;i++){
	
		INIT_LIST_HEAD(&packet_parsers[i]);
	}

	/*init ipv4 packet parser*/
	ch_packet_ipv4_init();
	
    /*init ipv6 packet parser*/
	//ch_packet_ipv6_init();

	/*init tcp packet parser*/
	ch_packet_tcp_init();

	/*init udp packet parser*/
	ch_packet_udp_init();

	/*init arp packet parser*/
	ch_packet_arp_init();
	
	/*init icmp packet parser*/
	ch_packet_icmp_init();
}

void ch_packet_parser_register(ch_packet_parser_t *parser,int level){

	struct list_head *list = &packet_parsers[level];

	list_add_tail(&parser->node,list);

}

static inline ch_packet_parser_t *_packet_parser_find(int level,uint16_t proto){

	struct list_head *list = &packet_parsers[level];

	ch_packet_parser_t *pkt_parser;

	list_for_each_entry(pkt_parser,list,node){


		if(pkt_parser->proto == proto)
			return pkt_parser;

	}

	return NULL;

}



static inline void _packet_init(ch_packet_t *pkt,struct rte_mbuf *mbuf){

	pkt->mbuf = mbuf;
	pkt->l2_len = 0;
	pkt->l3_len = 0;
	pkt->l4_len = 0;

	pkt->l3_proto = 0;
	pkt->l4_proto = 0;
	pkt->parse_off = 0;
	pkt->hash = 0;
	pkt->pkt_type = PKT_TYPE_OTHER;
    pkt->is_ipv6 = 0;

    pkt->data = rte_pktmbuf_mtod(mbuf,void*);
    pkt->dlen = mbuf->pkt_len;

    pkt->timestamp = 0;
}

int ch_packet_parse(ch_packet_t *pkt,struct rte_mbuf *mbuf){


	int rc;
	ch_packet_parser_t *parser;

	_packet_init(pkt,mbuf);

	/*parse ethernet packet!*/
	rc = ch_packet_ether_parse(pkt);

	if(rc == PKT_PARSE_OK){
	
		/*Parse level 3 protocol*/
		parser = _packet_parser_find(L3_INDEX,pkt->l3_proto);
		if(parser){
		
			rc = parser->parse(pkt);

			if(rc == PKT_PARSE_OK){
			
				/*Parse level 4 protocol*/
				parser = _packet_parser_find(L4_INDEX,pkt->l4_proto);
				if(parser){
				
					rc = parser->parse(pkt);
				}
			}
		}
	}


	return rc;
}

static inline int  _packet_copy(struct rte_mbuf *mi,struct rte_mbuf *m){

	char *dst_data,*src_data;

	if(m->data_len>mi->buf_len)
	{
	
		return -1;
	}

	mi->data_len = m->data_len;
	mi->port = m->port;
	mi->vlan_tci = m->vlan_tci;
	mi->vlan_tci_outer = m->vlan_tci_outer;
	mi->tx_offload = m->tx_offload;
	mi->hash = m->hash;

	mi->next = NULL;
	mi->pkt_len = mi->data_len;
	mi->nb_segs = 1;
	mi->ol_flags = m->ol_flags;
	mi->packet_type = m->packet_type;
	mi->timestamp = m->timestamp;

	dst_data = rte_pktmbuf_mtod(mi, char *);
	src_data = rte_pktmbuf_mtod(m, char *);

	rte_memcpy(dst_data,src_data, m->data_len);


	return 0;

}

ch_packet_t *ch_packet_clone(ch_packet_t *pkt,struct rte_mempool *mp){

	ch_packet_t *mi_pkt;
	int err = 0;
	struct rte_mbuf *mc, *mi, **prev,*md;
	uint32_t pktlen;
	uint8_t nseg;

	if (unlikely ((mc = rte_pktmbuf_alloc(mp)) == NULL))
		return NULL;

	md = pkt->mbuf;

	mi = mc;
	prev = &mi->next;
	pktlen = md->pkt_len;
	nseg = 0;

	do {
		nseg++;

		if(_packet_copy(mi,md) == -1)
		{
		
			err = 1;
			break;
		}

		*prev = mi;
		prev = &mi->next;
	} while ((md = md->next) != NULL &&
	    (mi = rte_pktmbuf_alloc(mp)) != NULL);

	*prev = NULL;
	mc->nb_segs = nseg;
	mc->pkt_len = pktlen;

	/* Allocation of new indirect segment failed */
	if (unlikely (mi == NULL||err)) {
		rte_pktmbuf_free(mc);
		return NULL;
	}

	mi_pkt = ch_packet_get_from_mbuf(mc);
	if(unlikely(mi_pkt == NULL))
	{

		rte_pktmbuf_free(mc);
		return NULL;	
	}

	/*init packet!*/
	mi_pkt->mbuf = mc;
	mi_pkt->l2_len = pkt->l2_len;
	mi_pkt->l3_len = pkt->l3_len;
	mi_pkt->l4_len = pkt->l4_len;
	mi_pkt->l3_proto = pkt->l3_proto;
	mi_pkt->l4_proto = pkt->l4_proto;
	mi_pkt->parse_off = pkt->parse_off;
	mi_pkt->hash = pkt->hash;
	mi_pkt->pkt_type = pkt->pkt_type;
    mi_pkt->timestamp = pkt->timestamp;
    mi_pkt->data = rte_pktmbuf_mtod(mc,void*);
    mi_pkt->dlen = mc->pkt_len;

	return mi_pkt;
}

struct ch_packet_t *ch_packet_part_clone(ch_packet_t *pkt,struct rte_mempool *mp,uint32_t dlen){


	struct rte_mbuf *md,*seg = pkt->mbuf,*m = pkt->mbuf;
	uint32_t copy_len,c_dlen = 0;
	char *buf;
	uint32_t r_dlen = CH_MIN(dlen,m->pkt_len); 
	ch_packet_t *m_pkt;

	if(unlikely(r_dlen == 0)){
	
		ch_log(CH_LOG_DEBUG,"No data can been clone!");
		return NULL;
	}

	if (unlikely ((md = rte_pktmbuf_alloc(mp)) == NULL)){
	
		ch_log(CH_LOG_DEBUG,"Cannot alloc a new mbuf!");

		return NULL;
	
	}

	if(unlikely((m_pkt=ch_packet_get_from_mbuf(md)) == NULL)){
	
		ch_log(CH_LOG_DEBUG,"Cannot get pkt from mbuf!");
		rte_pktmbuf_free(md);

		return NULL;
	}

	buf = rte_pktmbuf_mtod_offset(md, char *, 0);

	/*copy data*/
	while (r_dlen > 0&&seg) {
		
		copy_len = rte_pktmbuf_data_len(seg);

		if (copy_len > r_dlen)
			copy_len = r_dlen;

		rte_memcpy(buf,
			rte_pktmbuf_mtod_offset(seg, char *, 0), copy_len);

		buf += copy_len;
		r_dlen -= copy_len;

		seg = seg->next;
		c_dlen += copy_len;
	}

	/*init new mbuf*/
	md->data_len = c_dlen;
	md->pkt_len = m->pkt_len;
	md->port = m->port;
	md->vlan_tci = m->vlan_tci;
	md->vlan_tci_outer = m->vlan_tci_outer;
	md->tx_offload = m->tx_offload;
	md->hash = m->hash;

	md->next = NULL;
	md->nb_segs = 1;
	md->ol_flags = m->ol_flags;
	md->packet_type = m->packet_type;
	md->timestamp = m->timestamp;
	
	/*init packet!*/
	m_pkt->mbuf = md;
	m_pkt->l2_len = pkt->l2_len;
	m_pkt->l3_len = pkt->l3_len;
	m_pkt->l4_len = pkt->l4_len;
	m_pkt->l3_proto = pkt->l3_proto;
	m_pkt->l4_proto = pkt->l4_proto;
	m_pkt->parse_off = pkt->parse_off;
	m_pkt->hash = pkt->hash;
	m_pkt->pkt_type = pkt->pkt_type;
    m_pkt->timestamp = pkt->timestamp;
    m_pkt->data = rte_pktmbuf_mtod(md,void*);
    m_pkt->dlen = md->pkt_len;

	return m_pkt;
}

void ch_packet_dump(ch_packet_t *pkt,FILE *out){


	fprintf(out,"Dump The Packet Information:\n");
	fprintf(out,"Packet Type:%d\n",(int)pkt->pkt_type);
	fprintf(out,"Packet l2_len:%d\n",(int)pkt->l2_len);
	fprintf(out,"Packet l3_len:%d\n",(int)pkt->l3_len);
	fprintf(out,"Packet l4_len:%d\n",(int)pkt->l4_len);
	fprintf(out,"Packet l3_proto:%d\n",(int)pkt->l3_proto);
	fprintf(out,"Packet l4_proto:%d\n",(int)pkt->l4_proto);
	fprintf(out,"Packet parse_off:%d\n",(int)pkt->parse_off);
	fprintf(out,"Packet hash:%d\n",(int)pkt->hash);
	fprintf(out,"Packet timestamp:%llu\n",(unsigned long long)pkt->timestamp);
	fprintf(out,"Packet pkt_len:%lu\n",(unsigned long)pkt->dlen);

}

const char * ch_packet_target_get(ch_rule_target_context_t *tcontext,ch_rule_target_t *rtarget,int isHex){


    ch_packet_rule_context_t *rcontext = (ch_packet_rule_context_t*)tcontext->data;
    ch_packet_t *pkt = rcontext->pkt;
    const char *result = NULL;
    void *data;

    int target = rtarget->target;

    //memset(rcontext->sbuff,0,PKT_SMALL_BUF_SIZE);
    //memset(rcontext->dbuff,0,PKT_DATA_SIZE);

    if(pkt == NULL||pkt->mbuf == NULL)
        return NULL;

    switch(target){
        
        case TARGET_PKT_TYPE:
            result = (const char*)ch_ultoa((unsigned long)pkt->pkt_type,(char*)rcontext->sbuff,PKT_SMALL_BUF_SIZE);
            break;

        case TARGET_PKT_L3_PROTO:
            result = (const char*)ch_ultoa((unsigned long)pkt->l3_proto,(char*)rcontext->sbuff,PKT_SMALL_BUF_SIZE);
            break;

        case TARGET_PKT_L4_PROTO:
            result = (const char*)ch_ultoa((unsigned long)pkt->l4_proto,(char*)rcontext->sbuff,PKT_SMALL_BUF_SIZE);
            break;

        case TARGET_SRCIP:
        case TARGET_DSTIP:
            result =  ch_packet_ipv4_rule_target_get(pkt,target,rcontext->sbuff,PKT_SMALL_BUF_SIZE);
                break;

        case TARGET_SRCPORT:
        case TARGET_DSTPORT:
                result = NULL;
            if(pkt->pkt_type == PKT_TYPE_TCP){
                result =  ch_packet_tcp_rule_target_get(pkt,target,rcontext->sbuff,PKT_SMALL_BUF_SIZE);
            }else if(pkt->pkt_type == PKT_TYPE_UDP){
                result = ch_packet_udp_rule_target_get(pkt,target,rcontext->sbuff,PKT_SMALL_BUF_SIZE);
            }
                break;
        
        case TARGET_PKT_DATA_SIZE:
            result = (const char*)ch_ultoa((unsigned long)pkt->dlen,(char*)rcontext->sbuff,PKT_SMALL_BUF_SIZE);
            break;

        case TARGET_PKT_DATA:
            result = ch_rule_data_get(rcontext->dbuff,PKT_DATA_SIZE,pkt->data,pkt->dlen,
                    rtarget->offset,rtarget->len,isHex);
            break;
        
        case TARGET_PKT_L2_HEADER_SIZE:
            result = (const char*)ch_ultoa((unsigned long)pkt->l2_len,(char*)rcontext->sbuff,PKT_SMALL_BUF_SIZE); 
            break;
        
        case TARGET_PKT_L2_HEADER:
            data = pkt->data;
            result = ch_rule_data_get(rcontext->dbuff,PKT_DATA_SIZE,data,pkt->l2_len,
                    rtarget->offset,rtarget->len,isHex);
            break;

        case TARGET_PKT_L3_HEADER_SIZE:
            
            result = (const char*)ch_ultoa((unsigned long)pkt->l3_len,(char*)rcontext->sbuff,PKT_SMALL_BUF_SIZE); 
            break;
        
        case TARGET_PKT_L3_HEADER:
            data = ch_packet_data_offset(pkt,void*,pkt->l2_len);
            result = ch_rule_data_get(rcontext->dbuff,PKT_DATA_SIZE,data,pkt->l3_len,
                    rtarget->offset,rtarget->len,isHex);
            break;
        
        case TARGET_PKT_L4_HEADER_SIZE:
            
            result = (const char*)ch_ultoa((unsigned long)pkt->l4_len,(char*)rcontext->sbuff,PKT_SMALL_BUF_SIZE); 
            break;
        
        case TARGET_PKT_L4_HEADER:
            data = ch_packet_data_offset(pkt,void*,pkt->l2_len+pkt->l3_len);
            result = ch_rule_data_get(rcontext->dbuff,PKT_DATA_SIZE,data,pkt->l4_len,
                    rtarget->offset,rtarget->len,isHex);
            break;

        case TARGET_PKT_PAYLOAD_SIZE:
            
            result = (const char*)ch_ultoa((unsigned long)(pkt->dlen-pkt->l2_len-pkt->l3_len-pkt->l4_len),(char*)rcontext->sbuff,PKT_SMALL_BUF_SIZE); 
            break;
        
        case TARGET_PKT_PAYLOAD:
            data = ch_packet_data_offset(pkt,void*,pkt->l2_len+pkt->l3_len+pkt->l4_len);
            result = ch_rule_data_get(rcontext->dbuff,PKT_DATA_SIZE,data,
                    pkt->dlen-pkt->l2_len-pkt->l3_len-pkt->l4_len,
                    rtarget->offset,rtarget->len,isHex);
            break;
        
        default:
            result = NULL;
            break;
    }

    return result;
}

static int _isMyProto(ch_rule_target_context_t *tcontext,int proto){
 
     tcontext = tcontext;
     return proto == PROTO_PKT;
}

int ch_packet_rule_match(ch_rule_engine_t *rengine,ch_packet_t *pkt){

    ch_packet_rule_context_t tmp,*pcontext = &tmp;
    ch_rule_target_context_t target_tmp,*rtcontext = &target_tmp;

    pcontext->pkt = pkt;
    
    rtcontext->proto = "pkt";
    rtcontext->data = (void*)pcontext;
    rtcontext->isMyProto = _isMyProto;
    rtcontext->target = ch_packet_target_get;

    if(rengine == NULL)
        return 0;

    return ch_rule_engine_match(rengine,rtcontext);

}

size_t ch_packets_merge(void *pbuf,size_t pbsize,ch_packet_t *pkt){

    size_t dlen = 0,len = 0;
    void *p = pbuf;
    struct rte_mbuf *m = pkt->mbuf;
    
    while (m != NULL) {

        len = rte_pktmbuf_data_len(m);
        if(dlen+len>=pbsize)
            break;

        rte_memcpy(p, rte_pktmbuf_mtod(m, void *),len);
        p+=len;
        dlen+=len;
        m = m->next;
    }
    
    return dlen;
}

#define pkt_cpy(d,s) do {   \
    d->mbuf = s->mbuf;  \
    d->pkt_type = s->pkt_type;\
    d->l2_len = s->l2_len; \
    d->l3_len = s->l3_len; \
    d->l4_len = s->l4_len; \
    d->l3_proto = s->l3_proto;\
    d->l4_proto = s->l4_proto;\
    d->parse_off = s->parse_off;\
    d->hash = s->hash;\
    d->is_ipv6 = s->is_ipv6;\
    d->timestamp = s->timestamp;\
    d->dlen = s->dlen;\
    d->data = s->data;\
}while(0)

ch_packet_t* ch_packets_copy(void *pbuf,size_t pbsize,ch_packet_t *pkt){

    size_t len;
    struct rte_mbuf *m = pkt->mbuf;
    ch_packet_t *npacket = (ch_packet_t*)pbuf;

    pkt_cpy(npacket,pkt);

    pbuf = pbuf+sizeof(*pkt);
    pbsize = pbsize-sizeof(*pkt);

    if(m->nb_segs<=1){

        len = rte_pktmbuf_data_len(m);
        rte_memcpy(pbuf,rte_pktmbuf_mtod(m, void *),len);
    }else{

        len = ch_packets_merge(pbuf,pbsize,pkt); 
    }

    npacket->data = pbuf;
    npacket->dlen = len;

    return npacket;
}
