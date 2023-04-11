sources = files(
	'PDispatcher.c',
	'ch_pdispatcher_context.c',
	'ch_packet_rxtask.c',
	'ch_port_context.c',
	'ch_port_pool.c',
	'ch_packet_rxtask_pool.c'
)

deps = [dpdk, common, thread, shm, ptables, packet, mprocess,statistic,rule,filter]


#deps += [cc.find_library('numa', required: true)]
deps += [cc.find_library('pcap', required: true)]
deps += [cc.find_library('dl', required: false)]
deps += [cc.find_library('m', required: false)]

PDispatcher = executable('PDispatcher',
		sources,
		#c_args: cflags,
		install: true,
		dependencies: [deps])