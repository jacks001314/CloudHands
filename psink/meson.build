sources = files(
	'PSinkMain.c',
	'ch_psink_context.c',
	'ch_psink_task.c'
)

deps = [dpdk, common, thread, packet, mprocess,rule,filter]

deps += [cc.find_library('pcap', required: true)]
deps += [cc.find_library('dl', required: false)]
deps += [cc.find_library('m', required: false)]

PSinkMain = executable('PSinkMain',
		sources,
		#c_args: cflags,
		install: true,
		dependencies: [deps])


