
#include "ch_rule_match.h"
#include "ch_rule_utils.h"
#include "ch_log.h"
#include "ch_rule_constants.h"
#include "ch_list.h"
#include "ch_rule.h"

#define MATCH_RULE_STR_EMPTY(str) ((str)==NULL||strlen(str)==0)


static int _match_op_simple(const char *tvalue,const char *mvalue,int op){
    
    switch(op){

        case OP_contains:
            
            return ch_rule_op_contains(tvalue,mvalue);

        case OP_startsWith:
            
            return ch_rule_op_startsWith(tvalue,mvalue);

        case OP_endsWith:
            
            return ch_rule_op_endsWith(tvalue,mvalue);
        
        case OP_eq:
            
            return ch_rule_op_eq(tvalue,mvalue);

        case OP_lt:
            
            return ch_rule_op_lt(tvalue,mvalue);

        case OP_gt:

            return ch_rule_op_gt(tvalue,mvalue);

        case OP_le:

            return ch_rule_op_le(tvalue,mvalue);

        case OP_ge:

            return ch_rule_op_ge(tvalue,mvalue);

        default:
            return 0;

    }
}

static int _match_op_arrays(ch_array_header_t *arr,const char *tvalue,int isAnd,int op){

    int i;
    const char **elts;
    const char *match_value;

    if(arr->nelts<=0)
        return 0;

    elts = (const char**)arr->elts;

    for(i=0;i<arr->nelts;i++){

        match_value = elts[i];

        if(_match_op_simple(tvalue,match_value,op)){
            
            if(!isAnd)
                return 1;

        }else {

            if(isAnd)
                return 0;
        }
    }

    return isAnd;
}

static int _do_match_item(ch_rule_target_context_t *tcontext,ch_rule_item_t *ritem){

    ch_array_header_t *arr;

    const char *match_value = ritem->value;
    const char *target_value = tcontext->target(tcontext,ritem->target,ritem->isHex);

    int match = 0;

    if(MATCH_RULE_STR_EMPTY(match_value)||MATCH_RULE_STR_EMPTY(target_value))
        return 0;

    if(ritem->isArray){

        arr = ritem->arr_values;
        if(arr == NULL||arr->nelts<=0)
            return 0;

        match = _match_op_arrays(arr,target_value,ritem->isAnd,ritem->op); 
    }else{

        match = _match_op_simple(target_value,match_value,ritem->op);
    }

    return ritem->isnot?!match:match;
}

static inline int _is_valid_rule(ch_rule_target_context_t *tcontext,ch_rule_t *rule){

    int proto = rule->proto;

    return (rule->isEnable)&&(proto!=PROTO_UNK)&&(tcontext->isMyProto(tcontext,proto));
}

static int _do_match_rule(ch_rule_target_context_t *tcontext,ch_rule_t *rule){

    ch_rule_item_t *ritem;
    int isAnd = rule->isAnd;

    if(!_is_valid_rule(tcontext,rule))
        return 0;

    list_for_each_entry(ritem,&rule->items,node){

        if(_do_match_item(tcontext,ritem)){

            if(!isAnd)
                return 1;

        }else{

            if(isAnd)
                return 0;
        }
    }

    return isAnd;
}

int ch_rule_match(ch_rule_target_context_t *tcontext,ch_rule_pool_t *rpool){

    ch_rule_t *rule;

    list_for_each_entry(rule,&rpool->rules,node){

        if(_do_match_rule(tcontext,rule))
        {
            ch_log(CH_LOG_INFO,"Rule is Match,RuleID:%lu,RuleType:%s,Proto:%s",
                    (unsigned long)rule->id,
                    rule->type,
                    tcontext->proto);

            return 1;
        }
    }

    return 0;
}

int ch_rule_nmatch(ch_rule_match_context_t *mcontext,ch_rule_target_context_t *tcontext,ch_rule_pool_t *rpool){

    ch_rule_t *rule;
    
    mcontext->match_n = 0;
    INIT_LIST_HEAD(&mcontext->match_rules);

    list_for_each_entry(rule,&rpool->rules,node){

        if(_do_match_rule(tcontext,rule))
        {
            ch_log(CH_LOG_INFO,"Rule is Match,RuleID:%lu,RuleType:%s,Proto:%s",
                    (unsigned long)rule->id,
                    rule->type,
                    tcontext->proto);

            list_add_tail(&rule->match_node,&mcontext->match_rules);
            mcontext->match_n++;

            if(rpool->matchThenStop)
                return 1;
        }

    }

    return mcontext->match_n>0;
}

