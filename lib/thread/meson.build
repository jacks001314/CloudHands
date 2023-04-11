
sources = files(
	         'ch_core_pool.c',
			 'ch_task.c'
)

libthread = library('thread',sources,dependencies:[common,dpdk])
thread = declare_dependency(link_with: libthread,
	include_directories: include_directories('.'))