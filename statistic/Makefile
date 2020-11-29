##########################################################
#Copyright(C) 2012 WAF PROJECT TEAM
#Author(A) shajianfeng
##########################################################

include ../make.include
DPDK=/usr/local/dpdk/dpdk_src
DPDK_FLAGS=$(shell cat ${DPDK}/include/cflags.txt)
CFLAGS  = ${BUILD_CFLAGS}  -O0 -rdynamic -D_GNU_SOURCE -DUSE_DPDK ${DPDK_FLAGS} -pthread
CFLAGS += -I../lib/common -I../lib/thread -I${DPDK}/include -include ${DPDK}/include/rte_config.h

ch_stat_SOURCES = ch_stat_mpool.c \
				  ch_stat_pool.c \
				  ch_stat_obj.c \
				  ch_stat_task.c 

ch_stat_OBJECTS = $(patsubst %.c,%.o,$(ch_stat_SOURCES))
ch_stat_DEPENDS = $(patsubst %.c,%.d,$(ch_stat_SOURCES))
ch_stat_ASMFILE = $(patsubst %.c,%.s,$(ch_stat_SOURCES))

.PHONY: all clean

all: $(ch_stat_OBJECTS)

clean:
	@rm -fr $(ch_stat_OBJECTS) $(ch_stat_DEPENDS) $(ch_stat_ASMFILE) $(ch_stat_package)
	@rm -fr *.d *.o *.s 

