##########################################################
#Copyright(C) 2012 WAF PROJECT TEAM
#Author(A) shajianfeng
##########################################################
JAVA_HOME_ON_HOST=/usr/local/java/jdk-13.0.1
CFLAGS  = ${BUILD_CFLAGS} -ggdb3 -O0 -rdynamic -D_GNU_SOURCE -pthread
CFLAGS += -fPIC -shared -I../../lib/common -I../../lib/shm -I../../statistic  -I../../SessionMonitor  -I${JAVA_HOME_ON_HOST}/include -I${JAVA_HOME_ON_HOST}/include/linux

jni_SOURCES = ../../lib/common/ch_file_open.c \
		  ../../lib/common/ch_errno.c  \
		  ../../lib/common/ch_string.c \
		  ../../lib/common/ch_mpool.c \
		  ../../lib/common/ch_log.c    \
		  ../../lib/common/ch_util.c \
		  ../../lib/shm/ch_shm_mmap.c \
		  ../../lib/shm/ch_shm_memory.c \
		  ../../lib/shm/ch_shm_interface.c \
		  ../../lib/shm/ch_shm_format.c \
		  ../../lib/shm/ch_packet_record.c \
		  ../../statistic/ch_stat_pool.c \
		  ../../statistic/ch_stat_mpool.c \
		  ../../statistic/ch_stat_obj.c \
		  ch_stat_pool_reader.c \
		  ch_packet_record_reader.c \
		  ch_ClamAVScan.c 

jni_package = libCloudHandsJNI.so

.PHONY: all clean install

all:
	gcc ${CFLAGS} ${jni_SOURCES} -o ${jni_package} ${LDLIBS} -lclamav

clean:
	rm -rf ${jni_package}
