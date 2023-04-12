sources =[]

headers = []

includes = []

apps=['ftp','http','mail','mysql','smon','ssh','telnet']

foreach app:apps 

	subdir(app)
	includes += include_directories(app)

endforeach

libtapp = library('tapp',sources,dependencies:[common,fstore,shm,rule,filter,ptables,packet,tsession])
tapp = declare_dependency(link_with: libtapp,
	include_directories: includes)
