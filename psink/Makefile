include ../make.include
DPDK=/usr/local/dpdk/dpdk_src
DPDK_FLAGS=$(shell cat ${DPDK}/include/cflags.txt)
DPDK_LIB_FLAGS = $(shell cat ${DPDK}/lib/ldflags.txt)
CFLAGS  = ${BUILD_CFLAGS}  -O0 -rdynamic -D_GNU_SOURCE -DUSE_DPDK ${DPDK_FLAGS} -pthread
CFLAGS += -I../lib/common  -I../lib/thread  -I../packet -I../MProcess -I../rule
CFLAGS += -I${DPDK}/include -include ${DPDK}/include/rte_config.h

LDFLAGS  = ${BUILD_LDFLAGS} ${DPDK_LIB_FLAGS} -lpthread -lpcap -lcjson

common_package = ../lib/common/*.o
cpool_package = ../lib/thread/*.o
pkt_package = ../packet/*.o
mprocess_package = ../MProcess/*.o
rule_package = ../rule/*.o
filter_package = ../filter/*.o

psink_package= PSinkMain

psink_SOURCES = PSinkMain.c \
			   ch_psink_context.c \
			   ch_psink_task.c

psink_OBJECTS =$(patsubst %.c,%.o,$(psink_SOURCES))
psink_DEPENDS =$(patsubst %.c,%.d,$(psink_SOURCES))
psink_ASMFILE =$(patsubst %.c,%.s,$(psink_SOURCES))

quiet_cmd_dpdk = LINK   $@ 
	cmd_psink = ${CC} ${CFLAGS} -o $@ $(psink_OBJECTS)  \
				   $(common_package) $(cpool_package)\
				   $(pkt_package) $(mprocess_package) $(rule_package) $(filter_package)\
				   $(LDFLAGS)

.PHONY: all clean

all: $(psink_package)

$(psink_package): $(psink_OBJECTS) 
	$(call cmd,psink)
-include $(psink_DEPENDS)

$(common_package):
	$(call,make)

$(thread_package):
	$(call,make)

$(pkt_package):
	$(call,make)

$(mprocess_package):
	$(call,make)

$(rule_package):
	$(call,make)

$(filter_package):
	$(call,make)

clean:
	@rm -rf $(psink_OBJECTS) $(psink_DEPENDS) $(psink_ASMFILE) $(psink_package)


