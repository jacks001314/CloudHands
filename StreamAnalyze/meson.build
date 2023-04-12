sources = files(
	'StreamAnalyze.c',
	'ch_sa_context.c',
	'ch_sa_tcp_session_request_handler.c',
	'ch_sa_tcp_session_handler.c',
	'ch_sa_session_task.c',
	'ch_sa_session_task_pool.c',
	'ch_sa_work.c',
	'ch_sa_packet_dp.c',
	'ch_sa_udp_session_pool.c',
	'ch_sa_udp_session_handler.c',
	'ch_sa_icmp_handler.c',
	'ch_sa_arp_handler.c',
	'ch_sa_data_store_pool.c'
)

deps = [dpdk, common, thread, fstore,shm, ptables, packet, mprocess,statistic,rule,filter,tsession]


SAMain = executable('SAMain',
		sources,
		#c_args: cflags,
		install: true,
		dependencies: [deps])