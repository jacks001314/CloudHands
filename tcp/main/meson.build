sources = files(
	'ch_tcp_session_task.c',
	'ch_tcp_session_task_pool.c',
	'ch_tcp_work.c',
	'TCPMain.c'
)

deps = [dpdk, common, thread, fstore,shm, ptables, packet, mprocess,statistic,rule,filter,tapp,tsession]


#deps += [cc.find_library('numa', required: true)]
#deps += [cc.find_library('pcap', required: true)]
#deps += [cc.find_library('dl', required: false)]
#deps += [cc.find_library('m', required: false)]

TCPMain = executable('TCPMain',
		sources,
		#c_args: cflags,
		install: true,
		dependencies: [deps])