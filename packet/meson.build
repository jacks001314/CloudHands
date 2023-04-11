sources = files(
	'ch_packet.c',
	'ch_packet_ether.c',
	'ch_packet_ipv4.c',
	'ch_packet_ipv6.c',
	'ch_packet_tcp.c',
	'ch_packet_udp.c',
	'ch_packet_arp.c',
	'ch_packet_icmp.c'
)

libpacket = library('packet',sources,dependencies:[common,rule,filter,shm,dpdk])
packet = declare_dependency(link_with: libpacket,
	include_directories: include_directories('.'))