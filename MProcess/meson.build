sources = files(
	'ch_process_interface.c',
	'ch_process_interface_tcp_context.c',
	'ch_process_interface_udp_context.c',
	'ch_process_interface_sa_context.c',
	'ch_process_psink.c'
)

libmprocess = library('mprocess',sources,dependencies:[common,packet,rule,filter,dpdk])
mprocess = declare_dependency(link_with: libmprocess,
	include_directories: include_directories('.'))