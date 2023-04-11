sources = files(
	        'ch_ptable.c',
			'ch_entry_pool.c',
			'ch_memory.c',
			'ch_memory_cache.c',
			'ch_plist.c',
			'ch_ptable_watch_task.c'
)

libptables = library('ptables',sources,dependencies:[common,thread,dpdk])
ptables = declare_dependency(link_with: libptables,
	include_directories: include_directories('.'))