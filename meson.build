project('CloudHands', 'C',
    version: run_command(find_program('cat', 'more'),
        files('VERSION'), check:false).stdout().strip(),

	default_options: [
		'buildtype=release',
		'default_library=static',
		'warning_level=3'
		#'werror=true'
	],
	meson_version: '>= 0.61.2'
)

#run_command('bash','install_dep.sh')

cloudhands_conf = configuration_data()

# set up some global vars for compiler, platform, configuration, etc.
cc = meson.get_compiler('c')


add_project_arguments('-DALLOW_EXPERIMENTAL_API', language: 'c')
add_project_arguments('-D_GNU_SOURCE', language: 'c')
add_project_arguments('-DUSE_DPDK', language: 'c')

# enable extra warnings and disable any unwanted warnings
warning_flags = [
	'-Wno-pedantic',
	'-Wno-format-truncation',
]

foreach arg: warning_flags
	if cc.has_argument(arg)
		add_project_arguments(arg, language: 'c')
	endif
endforeach

message('>>>>>>>>>>>>> find msgpack lib <<<<<<<<<<<<<<')

msgpack_names = ['msgpack-c', 'msgpack']
msgpack = dependency('', required: false)
foreach n:msgpack_names
    msgpack = dependency(n, required: false)
	if not msgpack.found()
		msgpack = cc.find_library(n, required: false)
	endif
	if msgpack.found()
		break
	endif
endforeach

if not msgpack.found()
	error('unable to msgpack')
endif

cjson = dependency('cjson',required:false)
if not cjson.found()
	cjson = cc.find_library('cjson', required: false)
endif

if not cjson.found()
	error('unable to cjson')
endif

dpdk = dependency('libdpdk', required: true)

if dpdk.version().version_compare('>18.02.1') 
    add_project_arguments('-DUSE_DPDK_NEW_VERSION', language: 'c')
endif 

dpdk_libs_path = join_paths(get_option('prefix'), get_option('libdir'))

message(get_option('install_dir'))

subdir('lib')
subdir('rule')
subdir('filter')
subdir('packet')
subdir('MProcess')
subdir('statistic')

subdir('PDispatcher')
subdir('tcp')
subdir('StreamAnalyze')
subdir('udp')

subdir('pdump')
subdir('psink')