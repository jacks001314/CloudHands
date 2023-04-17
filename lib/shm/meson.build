sources = files(
	     'ch_shm_interface.c',
		 'ch_shm_mmap.c',
		 'ch_shm_memory.c',
		 'ch_shm_format.c',
		 'ch_tcp_record.c',
		 'ch_packet_record.c',
		 'ch_packet_record_session_tcp.c',
		 'ch_packet_record_session_udp.c',
		 'ch_packet_record_arp.c',
		 'ch_packet_record_icmp.c'
)

SHMInfoSources= files (
	'ch_shm_interface.c',
	'ch_shm_mmap.c',
	'ch_shm_memory.c',
	'ch_shm_format.c',
	'ch_packet_record.c',
	'SHMInfoDump.c'
)

libshm = library('shm',sources,dependencies:[common])
shm = declare_dependency(link_with: libshm,
	include_directories: include_directories('.'))


SHMInfoDump = executable('SHMInfoDump',
		SHMInfoSources,
		#c_args: cflags,
		install: true,
		dependencies: [common])