#ifndef CH_RULE_H
#define CH_RULE_H

typedef struct ch_rule_t ch_rule_t;
typedef struct ch_rule_item_t ch_rule_item_t;

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

    int proto;
    uint64_t id;
    uint64_t time;

    const char *type;
    const char *msg;

    int isEnable;
    int isAnd;

    struct list_head items;
};

#endif /*CH_RULE_H*/
