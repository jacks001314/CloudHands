sources = files(
	'PDumpMain.c',
	'ch_pdump_context.c',
	'ch_packet_rxtask.c',
	'ch_port_context.c',
	'ch_port_pool.c',
	'ch_packet_rxtask_pool.c',
	'ch_pcap_writer.c'
)

deps = [dpdk, common, thread, shm, packet, mprocess,statistic,rule,filter]


deps += [cc.find_library('pcap', required: true)]
deps += [cc.find_library('dl', required: false)]
deps += [cc.find_library('m', required: false)]

PDumpMain = executable('PDumpMain',
		sources,
		#c_args: cflags,
		install: true,
		dependencies: [deps])