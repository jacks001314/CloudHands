##########################################################
#Copyright(C) 2012 WAF PROJECT TEAM
#Author(A) shajianfeng
##########################################################

include ../../../make.include
DPDK=/usr/local/dpdk/dpdk_src
DPDK_FLAGS=$(shell cat ${DPDK}/include/cflags.txt)
CFLAGS  = ${BUILD_CFLAGS}  -O0 -rdynamic -D_GNU_SOURCE -DUSE_DPDK ${DPDK_FLAGS}
CFLAGS += -I../../../lib/common -I../../../packet -I../../../lib/ptables -I../../../lib/shm -I../../../lib/thread 
CFLAGS += -I../../../MProcess  -I../../../filter -I../../../rule
CFLAGS += -I../ -I../../ -I${DPDK}/include -include ${DPDK}/include/rte_config.h
CFLAGS +=  -fPIC

ch_dns_SOURCES = ch_dns_app.c \
				 ch_dns_session.c \
				 ch_dns_header.c \
				 ch_dns_request.c \
				 ch_dns_response.c \
				 ch_dns_name.c \
				 ch_dns_rdata.c 

ch_dns_OBJECTS = $(patsubst %.c,%.o,$(ch_dns_SOURCES))
ch_dns_DEPENDS = $(patsubst %.c,%.d,$(ch_dns_SOURCES))
ch_dns_ASMFILE = $(patsubst %.c,%.s,$(ch_dns_SOURCES))

.PHONY: all clean

all: $(ch_dns_OBJECTS)

clean:
	@rm -fr $(ch_dns_OBJECTS) $(ch_dns_DEPENDS) $(ch_dns_ASMFILE) $(ch_dns_package)
	@rm -fr *.d *.o *.s 

