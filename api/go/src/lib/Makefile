##########################################################
#Copyright(C) 2012 WAF PROJECT TEAM
#Author(A) shajianfeng
##########################################################
CFLAGS  = ${BUILD_CFLAGS} -ggdb3 -O0 -rdynamic -D_GNU_SOURCE -pthread
CFLAGS += -fPIC -shared -I../../lib/common -I../../lib/shm

cgo_SOURCES = ../../lib/common/ch_file_open.c \
		  ../../lib/common/ch_errno.c  \
		  ../../lib/common/ch_string.c \
		  ../../lib/common/ch_mpool.c \
		  ../../lib/common/ch_log.c    \
		  ../../lib/common/ch_util.c \
		  ../../lib/common/ch_fileacc.c \
		  ../../lib/common/ch_time.c \
		  ../../lib/common/ch_printf.c \
		  ../../lib/common/ch_file_readwrite.c \
		  ../../lib/common/ch_cpystrn.c \
		  ../../lib/common/ch_strtime.c \
		  ../../lib/common/ch_file_fullrw.c \
		  ../../lib/shm/ch_shm_mmap.c \
		  ../../lib/shm/ch_shm_memory.c \
		  ../../lib/shm/ch_shm_interface.c \
		  ../../lib/shm/ch_shm_format.c \
		  ../../lib/shm/ch_packet_record.c \
		  ch_packet_api_reader.c 

cgo_package = libCloudHandsCGo.so

.PHONY: all clean install

all:
	gcc ${CFLAGS} ${cgo_SOURCES} -o ${cgo_package} ${LDLIBS}

clean:
	rm -rf ${cgo_package}
