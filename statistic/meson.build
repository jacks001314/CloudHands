sources = files(
	'ch_stat_mpool.c',
	'ch_stat_pool.c',
	'ch_stat_obj.c',
	'ch_stat_task.c'
)

libstatistic = library('statistic',sources,dependencies:[common,thread,dpdk])
statistic = declare_dependency(link_with: libstatistic,
	include_directories: include_directories('.'))