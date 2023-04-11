sources = files(
	         'ch_fpath.c',
			 'ch_file_store.c',
			 'ch_sfile.c'
)

libfstore = library('fstore',sources,dependencies:[common])
fstore = declare_dependency(link_with: libfstore,
	include_directories: include_directories('.'))