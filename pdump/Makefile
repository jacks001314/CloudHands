include ../make.include
DPDK=/usr/local/dpdk/dpdk_src
DPDK_FLAGS=$(shell cat ${DPDK}/include/cflags.txt)
DPDK_LIB_FLAGS = $(shell cat ${DPDK}/lib/ldflags.txt)
CFLAGS  = ${BUILD_CFLAGS}  -O0 -rdynamic -D_GNU_SOURCE -DUSE_DPDK ${DPDK_FLAGS} -pthread
CFLAGS += -I../lib/common  -I../lib/shm -I../lib/thread -I../packet -I../statistic
CFLAGS += -I../rule -I../filter
CFLAGS += -I${DPDK}/include -include ${DPDK}/include/rte_config.h

LDFLAGS  = ${BUILD_LDFLAGS} ${DPDK_LIB_FLAGS} -lpthread -lcjson -lpcap

common_package = ../lib/common/*.o
cpool_package = ../lib/thread/*.o
shm_package = ../lib/shm/*.o
pkt_package = ../packet/*.o
stat_package = ../statistic/*.o
rule_package = ../rule/*.o
filter_package = ../filter/*.o

pdump_package= PDumpMain

pdump_SOURCES = PDumpMain.c \
			   ch_pdump_context.c \
			   ch_packet_rxtask.c\
			   ch_port_context.c \
			   ch_port_pool.c \
			   ch_packet_rxtask_pool.c\
			   ch_pcap_writer.c

pdump_OBJECTS =$(patsubst %.c,%.o,$(pdump_SOURCES))
pdump_DEPENDS =$(patsubst %.c,%.d,$(pdump_SOURCES))
pdump_ASMFILE =$(patsubst %.c,%.s,$(pdump_SOURCES))

quiet_cmd_dpdk = LINK   $@ 
	cmd_pdump = ${CC} ${CFLAGS} -o $@ $(pdump_OBJECTS)  \
				   $(common_package) $(cpool_package) $(shm_package) $(ptables_package)\
				   $(pkt_package) $(stat_package) $(rule_package) $(filter_package)\
				   $(LDFLAGS)

.PHONY: all clean

all: $(pdump_package)

$(pdump_package): $(pdump_OBJECTS) 
	$(call cmd,pdump)
-include $(pdump_DEPENDS)

$(common_package):
	$(call,make)

$(shm_package):
	$(call,make)

$(thread_package):
	$(call,make)

$(ptables_package):
	$(call,make)

$(pkt_package):
	$(call,make)

$(stat_package):
	$(call,make)

$(rule_package):
	$(call,make)

$(filter_package):
	$(call,make)

clean:
	@rm -rf $(pdump_OBJECTS) $(pdump_DEPENDS) $(pdump_ASMFILE) $(pdump_package)


