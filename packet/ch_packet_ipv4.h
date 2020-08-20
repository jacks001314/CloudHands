/*
 *
 *      Filename: ch_packet_ipv4.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2017-12-13 10:18:53
 * Last Modified: 2017-12-13 10:18:53
 */

#ifndef CH_PACKET_IPV4_H
#define CH_PACKET_IPV4_H

#include "ch_packet.h"
#include <rte_ip.h>
#include "ch_rule_constants.h"
#include "ch_net_util.h"

extern void ch_packet_ipv4_init(void);

static inline uint32_t ch_packet_ipv4_srcip(ch_packet_t *pkt){

	const struct ipv4_hdr *ip4h;

	ip4h = (const struct ipv4_hdr*)ch_packet_data_read(pkt, pkt->l2_len, sizeof(*ip4h));

	if(unlikely(ip4h == NULL))
		return 0;

	return ip4h->src_addr;
}

static inline uint32_t ch_packet_ipv4_dstip(ch_packet_t *pkt){

	const struct ipv4_hdr *ip4h;

	ip4h = (const struct ipv4_hdr*)ch_packet_data_read(pkt, pkt->l2_len, sizeof(*ip4h));

	if(unlikely(ip4h == NULL))
		return 0;

	return ip4h->dst_addr;
}

static inline const char * ch_packet_ipv4_rule_target_get(ch_packet_t *pkt,int target,unsigned char *buff,size_t bsize){

    const char *result;

    if(pkt == NULL||pkt->data == NULL||pkt->l3_proto!=CH_ETH_P_IP)
        return NULL;

	const struct ipv4_hdr *ip4h;

	ip4h = (const struct ipv4_hdr*)ch_packet_data_read(pkt, pkt->l2_len, sizeof(*ip4h));

	if(unlikely(ip4h == NULL))
		return 0;

    switch(target){

        case TARGET_SRCIP:
            result = (const char*)ch_ip_to_str((char*)buff,bsize,ip4h->src_addr);
            break;

        case TARGET_DSTIP:
            result = (const char*)ch_ip_to_str((char*)buff,bsize,ip4h->dst_addr);
            break;

        default:
            result = NULL;
            break;

    }

    return result;
}

#endif /*CH_PACKET_IPV4_H*/
