##########################################################
#Copyright(C) 2012 WAF PROJECT TEAM
#Author(A) shajianfeng
##########################################################

include ../make.include
CFLAGS += -I../lib/common

ch_rule_SOURCES = ch_cjson.c \
				 ch_rule_group.c \
				 ch_rule_match.c \
				 ch_rule.c \
				 ch_rule_pool.c \
				 ch_rule_engine_context.c\
				 ch_rule_engine.c

ch_rule_OBJECTS = $(patsubst %.c,%.o,$(ch_rule_SOURCES))
ch_rule_DEPENDS = $(patsubst %.c,%.d,$(ch_rule_SOURCES))
ch_rule_ASMFILE = $(patsubst %.c,%.s,$(ch_rule_SOURCES))

.PHONY: all clean

all: $(ch_rule_OBJECTS)

clean:
	@rm -fr $(ch_rule_OBJECTS) $(ch_rule_DEPENDS) $(ch_rule_ASMFILE) $(ch_rule_package)
	@rm -fr *.d *.o *.s 

