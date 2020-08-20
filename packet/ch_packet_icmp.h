/*
 *
 *      Filename: ch_packet_icmp.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-04-12 12:00:36
 * Last Modified: 2018-04-12 12:00:36
 */
#ifndef CH_PACKET_ICMP_H
#define CH_PACKET_ICMP_H

#include "ch_packet.h"
#include <rte_ip.h>
#include <rte_icmp.h>
#include "ch_packet_record_icmp.h"


static inline int ch_packet_icmp_read(ch_packet_record_icmp_t *icmp_rcd,ch_packet_t *pkt){

	const struct ipv4_hdr *iph;
	
	const struct icmp_hdr *icmp;
    size_t dlen = 0;
    size_t hlen = 0;

	iph = (const struct ipv4_hdr*)ch_packet_data_read(pkt, pkt->l2_len, sizeof(*iph));

	if(iph == NULL)
		return -1;

	icmp = (const struct icmp_hdr*)ch_packet_data_read(pkt,pkt->l2_len+pkt->l3_len, sizeof(*icmp));

	if(icmp == NULL)
		return -1;

	icmp_rcd->icmp_type = icmp->icmp_type;
	icmp_rcd->icmp_code = icmp->icmp_code;
	icmp_rcd->icmp_ttl = iph->time_to_live;

	icmp_rcd->icmp_cksum = rte_be_to_cpu_16(icmp->icmp_cksum);
	icmp_rcd->icmp_ident = rte_be_to_cpu_16(icmp->icmp_ident);
	icmp_rcd->icmp_seq_nb = rte_be_to_cpu_16(icmp->icmp_seq_nb);

	icmp_rcd->icmp_sip = iph->src_addr;
	icmp_rcd->icmp_tip = iph->dst_addr;

    hlen = pkt->l2_len+pkt->l3_len+sizeof(*icmp);
    dlen = pkt->dlen-hlen;

    if(dlen>0){

        icmp_rcd->dlen = dlen;
        icmp_rcd->data = ch_packet_data_offset(pkt,void*,hlen);
    }else{
        icmp_rcd->dlen = 0;
        icmp_rcd->data = NULL;
    }

	return 0;
}

extern void ch_packet_icmp_init(void);


#endif /*CH_PACKET_ICMP_H*/
