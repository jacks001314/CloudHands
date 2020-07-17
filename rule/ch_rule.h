#ifndef CH_RULE_H
#define CH_RULE_H

typedef struct ch_rule_t ch_rule_t;
typedef struct ch_rule_item_t ch_rule_item_t;

#include "ch_list.h"
#include "ch_cjson.h"

struct ch_rule_item_t {

    struct list_head node;

    const char *value;

    int target;
    int op;
    int isAnd;
    int isArray;
    int isHex;
    int isnot;
};

struct ch_rule_t {

    struct list_head node;
    struct list_head match_node;
    
    int proto;
    uint64_t id;
    uint64_t time;

    const char *type;
    const char *msg;
    const char *name;
    const char *value;

    int isEnable;
    int isAnd;

    struct list_head items;
};

extern ch_rule_t * ch_rule_parse(ch_pool_t *mp,cJSON *entry);

extern void ch_rule_dump(FILE *fp,ch_rule_t *rule);

#endif /*CH_RULE_H*/
