sources =[]

headers = []

includes = []

apps=['tftp','smon','sdns']

foreach app:apps 

	subdir(app)
	includes += include_directories(app)

endforeach

libuapp = library('uapp',sources,dependencies:[common,fstore,shm,rule,filter,ptables,packet,usession])
uapp = declare_dependency(link_with: libuapp,
	include_directories: includes)
