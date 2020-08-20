/*
 *
 *      Filename: ch_packet_udp.c
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-04-03 10:07:14
 * Last Modified: 2018-04-21 17:27:17
 */

#include "ch_packet_udp.h"
#include <rte_udp.h>
#include "ch_log.h"
#include "ch_jhash.h"

static int _packet_udp_parse(ch_packet_t *pkt){

	const struct ipv4_hdr *iph;
	const struct udp_hdr *uh;
	
	uint16_t udp_hlen = sizeof(*uh);

	pkt->pkt_type = PKT_TYPE_UDP;

	iph = (const struct ipv4_hdr*)ch_packet_data_read(pkt, pkt->l2_len, sizeof(*iph));
	uh = (const struct udp_hdr*)ch_packet_data_read(pkt,pkt->parse_off, sizeof(*uh));

	if (unlikely(uh == NULL||iph == NULL)){
	
		ch_log(CH_LOG_ERR,"Invalid udp packet!");
		return PKT_PARSE_DROP;
	}


	pkt->l4_len = udp_hlen;
	pkt->parse_off += pkt->l4_len;

	pkt->hash = ch_jhash_4words_sort(iph->src_addr,
		iph->dst_addr,
		rte_be_to_cpu_16(uh->src_port),
		rte_be_to_cpu_16(uh->dst_port),
		0);

	return PKT_PARSE_OK;
}


static ch_packet_parser_t udp_parser = {

	{NULL,NULL},
	CH_IPPROTO_UDP,
	_packet_udp_parse
};

void ch_packet_udp_init(void){


	ch_packet_parser_register(&udp_parser,L4_INDEX);
}

