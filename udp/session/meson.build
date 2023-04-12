sources = files(
	'ch_udp_app_context.c',
	'ch_udp_app_pool.c',
	'ch_udp_context.c',
	'ch_udp_session.c',
	'ch_udp_session_handler.c',
	'ch_udp_session_pool.c',
	'ch_udp_session_request_handler.c',
	'ch_udp_session_request_pool.c'
)

libusession = library('usession',sources,dependencies:[common,fstore,shm,rule,filter,ptables,packet])
usession = declare_dependency(link_with: libusession,
	include_directories: include_directories('.'))