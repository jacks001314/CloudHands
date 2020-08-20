/*
 *
 *      Filename: ch_packet_arp.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-04-09 18:24:46
 * Last Modified: 2018-04-09 18:24:46
 */

#ifndef CH_PACKET_ARP_H
#define CH_PACKET_ARP_H

#include "ch_packet.h"
#include "ch_packet_record_arp.h"
#include <rte_arp.h>

static inline int ch_packet_arp_read(ch_packet_record_arp_t *arp_rcd,ch_packet_t *pkt){

	const struct arp_hdr *arp;
	const struct ether_hdr *eh;

	eh = (const struct ether_hdr*)ch_packet_data_read(pkt,0,sizeof(*eh));
	arp = (const struct arp_hdr*)ch_packet_data_read(pkt, pkt->l2_len, sizeof(*arp));

	if(arp == NULL)
		return -1;

	arp_rcd->arp_hrd = rte_be_to_cpu_16(arp->arp_hrd);
	arp_rcd->arp_pro = rte_be_to_cpu_16(arp->arp_pro);
	arp_rcd->arp_hln = arp->arp_hln;
	arp_rcd->arp_pln = arp->arp_pln;
	arp_rcd->arp_op = rte_be_to_cpu_16(arp->arp_op);
	arp_rcd->arp_data.arp_sip = arp->arp_data.arp_sip;
	arp_rcd->arp_data.arp_tip = arp->arp_data.arp_tip;

	rte_memcpy(arp_rcd->arp_data.arp_sha.addr,arp->arp_data.arp_sha.addr_bytes,6);
	rte_memcpy(arp_rcd->arp_data.arp_tha.addr,arp->arp_data.arp_tha.addr_bytes,6);
	
	rte_memcpy(arp_rcd->src_mac.addr,eh->s_addr.addr_bytes,6);
	rte_memcpy(arp_rcd->dst_mac.addr,eh->d_addr.addr_bytes,6);

	return 0;
}

extern void ch_packet_arp_init(void);


#endif /*CH_PACKET_ARP_H*/
