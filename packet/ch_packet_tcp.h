/*
 *
 *      Filename: ch_packet_tcp.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2017-12-13 11:27:50
 * Last Modified: 2017-12-13 11:27:50
 */

#ifndef CH_PACKET_TCP_H
#define CH_PACKET_TCP_H

#include "ch_net_util.h"
#include "ch_packet.h"
#include <rte_ip.h>
#include <rte_tcp.h>
#include "ch_rule_constants.h"

typedef struct ch_packet_tcp_t ch_packet_tcp_t;

struct ch_packet_tcp_t {

    ch_packet_t *pkt;
    uint8_t is_ipv6;

    /*for ipv4*/
    uint32_t src_ip;
    uint32_t dst_ip;
    /*for ipv6*/
	uint8_t  src_addr[16]; /**< IP address of source host. */
	uint8_t  dst_addr[16]; /**< IP address of destination host(s). */

    uint16_t src_port;
    uint16_t dst_port;
    uint16_t ip_dlen;

    uint8_t  tcp_flags;
    uint32_t sent_seq;
    uint32_t recv_ack;

    uint32_t tth_len;
    uint32_t payload_len;

    void *pdata;
};

#define CH_TH_OFF(th)	(((th)->data_off & 0xf0) >> 4)

/* TCP flags */
#define	CH_TH_FIN      0x01
#define	CH_TH_SYN	   0x02
#define	CH_TH_RST	   0x04
#define	CH_TH_PUSH	   0x08
#define	CH_TH_ACK	   0x10
#define	CH_TH_URG	   0x20
#define CH_TH_ECNECHO  0x40	/* ECN Echo */
#define CH_TH_CWR	   0x80	/* ECN Cwnd Reduced */

static inline int is_tcp_syn_packet(ch_packet_tcp_t *tcp_pkt){
   
    return (tcp_pkt->tcp_flags&CH_TH_SYN)&&((tcp_pkt->tcp_flags&CH_TH_ACK)==0);
}

static inline int is_tcp_syn_ack_packet(ch_packet_tcp_t *tcp_pkt){
    
    return (tcp_pkt->tcp_flags&CH_TH_SYN)&&(tcp_pkt->tcp_flags&CH_TH_ACK);
}

static inline int is_tcp_fin_packet(ch_packet_tcp_t *tcp_pkt){
    
    return tcp_pkt->tcp_flags&CH_TH_FIN;
}

static inline int is_tcp_rst_packet(ch_packet_tcp_t *tcp_pkt){

    return tcp_pkt->tcp_flags&CH_TH_RST;
}

static inline const char * _get_packet_type_name(ch_packet_tcp_t *pkt){

	if(is_tcp_syn_packet(pkt))
		return "syn";

	if(is_tcp_syn_ack_packet(pkt))
		return "sys-ack";

	if(is_tcp_fin_packet(pkt))
		return "fin";
	if(is_tcp_rst_packet(pkt))
		return "reset";

	if(pkt->payload_len>0)
		return "data";

	return "other";

}

static inline int ch_packet_tcp_init_from_pkt_ipv4(ch_packet_tcp_t *tcp_pkt,ch_packet_t *pkt){

    uint16_t mbdlen;
	
	const struct ipv4_hdr *iph;

	const struct tcp_hdr *th;
	
	iph = (const struct ipv4_hdr*)ch_packet_data_read(pkt, pkt->l2_len, sizeof(*iph));
	if(iph == NULL)
		return -1;

	th = (const struct tcp_hdr*)ch_packet_data_read(pkt,pkt->l2_len+pkt->l3_len, sizeof(*th));

	if(th == NULL)
		return -1;

	tcp_pkt->pkt = pkt;
    tcp_pkt->is_ipv6 = 0;
    tcp_pkt->src_ip = iph->src_addr; 
    tcp_pkt->dst_ip = iph->dst_addr;
    tcp_pkt->src_port = rte_be_to_cpu_16(th->src_port);
    tcp_pkt->dst_port = rte_be_to_cpu_16(th->dst_port);
    tcp_pkt->ip_dlen = rte_cpu_to_be_16(iph->total_length)-pkt->l3_len-pkt->l4_len;

    tcp_pkt->tcp_flags = th->tcp_flags;
    tcp_pkt->sent_seq = rte_be_to_cpu_32(th->sent_seq);
    tcp_pkt->recv_ack = rte_be_to_cpu_32(th->recv_ack);

    tcp_pkt->tth_len = pkt->l2_len+pkt->l3_len+pkt->l4_len;
    mbdlen = pkt->dlen-tcp_pkt->tth_len;

    tcp_pkt->payload_len = CH_MIN(mbdlen,tcp_pkt->ip_dlen);
    tcp_pkt->pdata = NULL;
    if(tcp_pkt->payload_len>0){
    
        tcp_pkt->pdata = ch_packet_data_offset(pkt,void*,tcp_pkt->tth_len);
    }

	return 0;
}

static inline int ch_packet_tcp_init_from_pkt_ipv6(ch_packet_tcp_t *tcp_pkt,ch_packet_t *pkt){

    uint16_t mbdlen;
	
	const struct ipv6_hdr *iph;

	const struct tcp_hdr *th;
	
	iph = (const struct ipv6_hdr*)ch_packet_data_read(pkt, pkt->l2_len, sizeof(*iph));
	if(iph == NULL)
		return -1;

	th = (const struct tcp_hdr*)ch_packet_data_read(pkt,pkt->l2_len+pkt->l3_len, sizeof(*th));

	if(th == NULL)
		return -1;

	tcp_pkt->pkt = pkt;
    tcp_pkt->is_ipv6 = 1;
    rte_memcpy(tcp_pkt->src_addr,iph->src_addr,16); 
    rte_memcpy(tcp_pkt->dst_addr,iph->dst_addr,16); 
    tcp_pkt->src_port = rte_be_to_cpu_16(th->src_port);
    tcp_pkt->dst_port = rte_be_to_cpu_16(th->dst_port);
    tcp_pkt->ip_dlen = rte_cpu_to_be_16(iph->payload_len)-pkt->l3_len-pkt->l4_len;

    tcp_pkt->tcp_flags = th->tcp_flags;
    tcp_pkt->sent_seq = rte_be_to_cpu_32(th->sent_seq);
    tcp_pkt->recv_ack = rte_be_to_cpu_32(th->recv_ack);

    tcp_pkt->tth_len = pkt->l2_len+pkt->l3_len+pkt->l4_len;
    mbdlen = pkt->dlen-tcp_pkt->tth_len;

    tcp_pkt->payload_len = CH_MIN(mbdlen,tcp_pkt->ip_dlen);
    tcp_pkt->pdata = NULL;
    if(tcp_pkt->payload_len>0){
    
        tcp_pkt->pdata = ch_packet_data_offset(pkt,void*,tcp_pkt->tth_len);
    }

	return 0;
}


static inline int ch_packet_tcp_init_from_pkt(ch_packet_tcp_t *tcp_pkt,ch_packet_t *pkt){
    
    if(pkt->is_ipv6)
        return ch_packet_tcp_init_from_pkt_ipv6(tcp_pkt,pkt);
    
    return ch_packet_tcp_init_from_pkt_ipv4(tcp_pkt,pkt);
}

static inline const char *ch_packet_tcp_rule_target_get(ch_packet_t *pkt,int target,unsigned char *buff,size_t bsize){

	const struct tcp_hdr *th;
    const char *result;

    if(pkt == NULL||pkt->data == NULL)
        return NULL;

	th = (const struct tcp_hdr*)ch_packet_data_read(pkt,pkt->l2_len+pkt->l3_len, sizeof(*th));

	if(th == NULL)
        return NULL;

    switch(target){

        case TARGET_SRCPORT:
            result = (const char*)ch_ultoa((unsigned long)rte_be_to_cpu_16(th->src_port),(char*)buff,bsize);
            break;

        case TARGET_DSTPORT:
            result = (const char*)ch_ultoa((unsigned long)rte_be_to_cpu_16(th->dst_port),(char*)buff,bsize);
            break;

        default:
            result = NULL;
            break;
    }

    return result;
}

static inline void ch_packet_tcp_dump(ch_packet_tcp_t *pkt,FILE *fp){

	char buf[64] = {0};

	fprintf(fp,"Dump TCP Packet Informations:\n");
	fprintf(fp,"tcp.packet.type:%s\n",_get_packet_type_name(pkt));
	
	fprintf(fp,"tcp.packet.srcIP:%s\n",ch_ip_to_str(buf,63,pkt->src_ip));
	fprintf(fp,"tcp.packet.srcPort:%lu\n",(unsigned long)pkt->src_port);

	fprintf(fp,"tcp.packet.dstIP:%s\n",ch_ip_to_str(buf,63,pkt->dst_ip));
	fprintf(fp,"tcp.packet.dstPort:%lu\n",(unsigned long)pkt->dst_port);
	
	fprintf(fp,"tcp.packet.ipDataLen:%lu\n",(unsigned long)pkt->ip_dlen);
	fprintf(fp,"tcp.packet.tcpFlags:%lu\n",(unsigned long)pkt->tcp_flags);
	fprintf(fp,"tcp.packet.sentSeq:%lu\n",(unsigned long)pkt->sent_seq);

	fprintf(fp,"tcp.packet.recvAck:%lu\n",(unsigned long)pkt->recv_ack);
	fprintf(fp,"tcp.packet.tthLen:%lu\n",(unsigned long)pkt->tth_len);

	fprintf(fp,"tcp.packet.payloadLen:%lu\n",(unsigned long)pkt->payload_len);

} 

extern void ch_packet_tcp_init(void);


#endif /*CH_PACKET_TCP_H*/
