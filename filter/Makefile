##########################################################
#Copyright(C) 2012 WAF PROJECT TEAM
#Author(A) shajianfeng
##########################################################

include ../make.include
CFLAGS += -I../lib/common -I../rule

ch_filter_SOURCES = ch_filter_engine.c 

ch_filter_OBJECTS = $(patsubst %.c,%.o,$(ch_filter_SOURCES))
ch_filter_DEPENDS = $(patsubst %.c,%.d,$(ch_filter_SOURCES))
ch_filter_ASMFILE = $(patsubst %.c,%.s,$(ch_filter_SOURCES))

.PHONY: all clean

all: $(ch_filter_OBJECTS)

clean:
	@rm -fr $(ch_filter_OBJECTS) $(ch_filter_DEPENDS) $(ch_filter_ASMFILE) $(ch_filter_package)
	@rm -fr *.d *.o *.s 

