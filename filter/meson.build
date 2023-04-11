sources = files(
	'ch_filter_engine.c'
)

libfilter = library('filter',sources,dependencies:[common,rule])
filter = declare_dependency(link_with: libfilter,
	include_directories: include_directories('.'))