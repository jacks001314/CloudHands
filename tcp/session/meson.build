sources = files(
	'ch_assemble_fragment.c',
	'ch_proto_session_store.c',
	'ch_tcp_app_context.c',
	'ch_tcp_app_pool.c',
	'ch_tcp_context.c',
	'ch_tcp_session.c',
	'ch_tcp_session_endpoint.c',
	'ch_tcp_session_handler.c',
	'ch_tcp_session_pool.c',
	'ch_tcp_session_request_handler.c',
	'ch_tcp_session_request_pool.c'
)

libtsession = library('tsession',sources,dependencies:[common,fstore,shm,rule,filter,ptables,packet])
tsession = declare_dependency(link_with: libtsession,
	include_directories: include_directories('.'))