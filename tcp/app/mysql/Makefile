##########################################################
#Copyright(C) 2012 WAF PROJECT TEAM
#Author(A) shajianfeng
##########################################################

include ../../../make.include
DPDK=/usr/local/dpdk/dpdk_src
DPDK_FLAGS=$(shell cat ${DPDK}/include/cflags.txt)
CFLAGS  = ${BUILD_CFLAGS}  -O0 -rdynamic -D_GNU_SOURCE -DUSE_DPDK ${DPDK_FLAGS}
CFLAGS += -I../../../lib/common -I../../../packet -I../../../lib/ptables -I../../../lib/shm -I../../../lib/fstore
CFLAGS += -I../../../lib/thread -I../../../MProcess -I../../../rule -I../../../filter
CFLAGS += -I../ -I../../ -I${DPDK}/include -include ${DPDK}/include/rte_config.h
CFLAGS +=  -fPIC

ch_mysql_SOURCES = ch_mysql.c \
				  ch_mysql_session_entry.c \
				  ch_mysql_request.c \
				  ch_mysql_response.c 

ch_mysql_OBJECTS = $(patsubst %.c,%.o,$(ch_mysql_SOURCES))
ch_mysql_DEPENDS = $(patsubst %.c,%.d,$(ch_mysql_SOURCES))
ch_mysql_ASMFILE = $(patsubst %.c,%.s,$(ch_mysql_SOURCES))

.PHONY: all clean

all: $(ch_mysql_OBJECTS)

clean:
	@rm -fr $(ch_mysql_OBJECTS) $(ch_mysql_DEPENDS) $(ch_mysql_ASMFILE) $(ch_mysql_package)
	@rm -fr *.d *.o *.s 

