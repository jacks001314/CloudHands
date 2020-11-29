/*
 *
 *      Filename: ch_packet_udp.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-04-03 10:05:51
 * Last Modified: 2018-04-03 10:05:51
 */

#ifndef CH_PACKET_UDP_H
#define CH_PACKET_UDP_H

#include "ch_packet.h"
#include <rte_ip.h>
#include <rte_udp.h>
#include "ch_rule_constants.h"

typedef struct ch_packet_udp_t ch_packet_udp_t;


struct ch_packet_udp_t {

	ch_packet_t *pkt;
    
    uint8_t is_ipv6;
	uint32_t src_ip;
    uint32_t dst_ip;
    /*for ipv6*/
	uint8_t  src_addr[16]; /**< IP address of source host. */
	uint8_t  dst_addr[16]; /**< IP address of destination host(s). */
    
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t ip_dlen;

	uint32_t tth_len;
	uint32_t payload_len;
	void *pdata;
};

static inline int ch_packet_udp_init_from_pkt_ipv4(ch_packet_udp_t *pkt_udp,ch_packet_t *pkt){

    uint16_t mbdlen;
	
	const struct ipv4_hdr *iph;
	const struct udp_hdr *uh;
	
	iph = (const struct ipv4_hdr*)ch_packet_data_read(pkt, pkt->l2_len, sizeof(*iph));

	if(iph == NULL)
		return -1;

	uh = (const struct udp_hdr*)ch_packet_data_read(pkt,pkt->l2_len+pkt->l3_len, sizeof(*uh));
    
	if(uh == NULL)
		return -1;

    pkt_udp->pkt = pkt;
    pkt_udp->is_ipv6 = 0;
    pkt_udp->src_ip = iph->src_addr; 
    pkt_udp->dst_ip = iph->dst_addr;
    pkt_udp->src_port = rte_be_to_cpu_16(uh->src_port);
    pkt_udp->dst_port = rte_be_to_cpu_16(uh->dst_port);
    pkt_udp->ip_dlen = rte_cpu_to_be_16(iph->total_length)-pkt->l3_len-pkt->l4_len;

    pkt_udp->tth_len = pkt->l2_len+pkt->l3_len+pkt->l4_len;
    mbdlen = pkt->dlen-pkt_udp->tth_len;

    pkt_udp->payload_len = CH_MIN(mbdlen,pkt_udp->ip_dlen);

    pkt_udp->pdata = NULL;
    if(pkt_udp->payload_len>0){
    
        pkt_udp->pdata = ch_packet_data_offset(pkt,void*,pkt_udp->tth_len);
    }

	return 0;
}

static inline int ch_packet_udp_init_from_pkt_ipv6(ch_packet_udp_t *pkt_udp,ch_packet_t *pkt){

    uint16_t mbdlen;
	const struct ipv6_hdr *iph;
	
	const struct udp_hdr *uh;
	
	iph = (const struct ipv6_hdr*)ch_packet_data_read(pkt, pkt->l2_len, sizeof(*iph));

	if(iph == NULL)
		return -1;

	uh = (const struct udp_hdr*)ch_packet_data_read(pkt,pkt->l2_len+pkt->l3_len, sizeof(*uh));
    
	if(uh == NULL)
		return -1;

    pkt_udp->pkt = pkt;
    pkt_udp->is_ipv6 = 1;
    pkt_udp->src_ip = 0; 
    pkt_udp->dst_ip = 0;

    memcpy(pkt_udp->src_addr,iph->src_addr,16); 
    memcpy(pkt_udp->dst_addr,iph->dst_addr,16); 
    
    pkt_udp->src_port = rte_be_to_cpu_16(uh->src_port);
    pkt_udp->dst_port = rte_be_to_cpu_16(uh->dst_port);
    pkt_udp->ip_dlen = rte_cpu_to_be_16(iph->payload_len)-pkt->l3_len-pkt->l4_len;

    pkt_udp->tth_len = pkt->l2_len+pkt->l3_len+pkt->l4_len;
    mbdlen = pkt->dlen-pkt_udp->tth_len;

    pkt_udp->payload_len = CH_MIN(mbdlen,pkt_udp->ip_dlen);

    pkt_udp->pdata = NULL;
    if(pkt_udp->payload_len>0){
    
        pkt_udp->pdata = ch_packet_data_offset(pkt,void*,pkt_udp->tth_len);
    }

	return 0;
}

static inline int ch_packet_udp_init_from_pkt(ch_packet_udp_t *pkt_udp,ch_packet_t *pkt){

    if(pkt->is_ipv6)
        return ch_packet_udp_init_from_pkt_ipv6(pkt_udp,pkt);

    return ch_packet_udp_init_from_pkt_ipv4(pkt_udp,pkt);

}

static inline const char *ch_packet_udp_rule_target_get(ch_packet_t *pkt,int target,unsigned char *buff,size_t bsize){

	const struct udp_hdr *uh;
    const char *result;

    if(pkt == NULL||pkt->data == NULL)
        return NULL;

	uh = (const struct udp_hdr*)ch_packet_data_read(pkt,pkt->l2_len+pkt->l3_len, sizeof(*uh));

	if(uh == NULL)
        return NULL;

    switch(target){

        case TARGET_SRCPORT:
            ch_ultoa((unsigned long)rte_be_to_cpu_16(uh->src_port),(char*)buff,bsize);
            result = (const char*)buff;
            break;

        case TARGET_DSTPORT:
            ch_ultoa((unsigned long)rte_be_to_cpu_16(uh->dst_port),(char*)buff,bsize);
            result = (const char*)buff;
            break;

        default:
            result = NULL;
            break;
    }

    return result;
}

extern void ch_packet_udp_init(void);

#endif /*CH_PACKET_UDP_H*/
