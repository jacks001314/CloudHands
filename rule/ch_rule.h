#ifndef CH_RULE_H
#define CH_RULE_H

typedef struct ch_rule_t ch_rule_t;
typedef struct ch_rule_item_t ch_rule_item_t;
typedef struct ch_rule_target_t ch_rule_target_t;

#include "ch_list.h"
#include "ch_cjson.h"
#include "ch_rule_pool.h"


struct ch_rule_target_t {

    const char *target_str;
    int target;
    size_t offset;
    size_t len;
};

struct ch_rule_item_t {

    struct list_head node;
    ch_array_header_t *arr_values;
    const char *value;
    ch_rule_target_t *target;
    const char *op_str;
    int op;
    int isAnd;
    int isArray;
    int isHex;
    int isnot;
};

struct ch_rule_t {

    struct list_head node;
    struct list_head match_node;

    ch_rule_pool_t *rpool;

    const char *proto_str;
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

extern ch_rule_t * ch_rule_parse(ch_rule_pool_t *rpool,cJSON *entry);

extern void ch_rule_dump(FILE *fp,ch_rule_t *rule);

#endif /*CH_RULE_H*/
