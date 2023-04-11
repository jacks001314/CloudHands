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


static inline int ch_packet_arp_read(ch_packet_record_arp_t *arp_rcd,ch_packet_t *pkt){

	ch_arp_hdr_t *arp;
	ch_ether_hdr_t *eh;
	
	eh = (ch_ether_hdr_t*)ch_packet_data_read(pkt,0,sizeof(*eh));
	arp = (ch_arp_hdr_t*)ch_packet_data_read(pkt, pkt->l2_len, sizeof(*arp));

	if(arp == NULL)
		return -1;

	arp_rcd->arp_hrd = rte_be_to_cpu_16(ch_arp_hdr_get(arp));
	arp_rcd->arp_pro = rte_be_to_cpu_16(ch_arp_pro_get(arp));
	arp_rcd->arp_hln = ch_arp_hlen_get(arp);
	arp_rcd->arp_pln = ch_arp_plen_get(arp);
	arp_rcd->arp_op = rte_be_to_cpu_16(ch_arp_opcode_get(arp));
	arp_rcd->arp_data.arp_sip = arp->arp_data.arp_sip;
	arp_rcd->arp_data.arp_tip = arp->arp_data.arp_tip;

	rte_memcpy(arp_rcd->arp_data.arp_sha.addr,arp->arp_data.arp_sha.addr_bytes,6);
	rte_memcpy(arp_rcd->arp_data.arp_tha.addr,arp->arp_data.arp_tha.addr_bytes,6);
	 
	rte_memcpy(arp_rcd->src_mac.addr,ch_eth_src_addr(eh),6);
	rte_memcpy(arp_rcd->dst_mac.addr,ch_eth_dst_addr(eh),6);

	return 0;
}

extern void ch_packet_arp_init(void);


#endif /*CH_PACKET_ARP_H*/
