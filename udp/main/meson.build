sources = files(
	'ch_udp_session_task.c',
	'ch_udp_session_task_pool.c',
	'ch_udp_work.c',
	'UDPMain.c'
)

deps = [dpdk, common, thread, fstore,shm, ptables, packet, mprocess,statistic,rule,filter,uapp,usession]


#deps += [cc.find_library('numa', required: true)]
#deps += [cc.find_library('pcap', required: true)]
#deps += [cc.find_library('dl', required: false)]
#deps += [cc.find_library('m', required: false)]

UDPMain = executable('UDPMain',
		sources,
		#c_args: cflags,
		install: true,
		dependencies: [deps])