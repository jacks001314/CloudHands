
#include "ch_rule.h"
#include "ch_rule_constants.h"
#include "ch_rule_utils.h"
#include "ch_log.h"

static ch_rule_item_t *_rule_item_parse(ch_pool_t *mp,cJSON *entry){

    ch_rule_item_t *rule_item;
    const char *target_str;
    int target;
    const char *op_str;
    int op;
    const char *value;
    int isHex;
    int isArray;
    int isAnd;
    int isnot;

    target_str= ch_json_str_value_get_no_cp(entry,"target");
    target = ch_target_value_get(target_str);
    if(target == TARGET_NONE){
        ch_log(CH_LOG_ERR,"Unknown match target:%s",target_str);
        return NULL;
    } 
     
    op_str= ch_json_str_value_get_no_cp(entry,"op");
    op = ch_op_value_get(op_str);
    if(op == OP_none){

        ch_log(CH_LOG_ERR,"Unknown operator:%s",op_str);
        return NULL;
    }

    value = ch_json_str_value_get(mp,entry,"value");
    if(value == NULL||strlen(value)==0){
        ch_log(CH_LOG_ERR,"Invalid value:Empty");
    }

    isArray = ch_json_bool_value_get(entry,"isArray");
    if(isArray){
       if(!ch_rule_op_startsWith(value,"file:")||!ch_rule_op_startsWith(value,"inline:")){
            ch_log(CH_LOG_ERR,"Invalid array value:%s",value);
            return NULL;
        } 
    }
    
    isHex = ch_json_bool_value_get(entry,"isHex");
    isAnd = ch_json_bool_value_get(entry,"isAnd");
    isnot = ch_json_bool_value_get(entry,"isnot");

    rule_item = (ch_rule_item_t*)ch_pcalloc(mp,sizeof(*rule_item));
    rule_item->value = value;
    rule_item->target = target;
    rule_item->op = op;
    rule_item->isAnd = isAnd;
    rule_item->isArray = isArray;
    rule_item->isHex = isHex;
    rule_item->isnot = isnot;

    return rule_item;
}


static inline const char * _str_value_get(ch_pool_t *mp,const char *value){
    
    if(value == NULL||strlen(value)==0)
        return NULL;
    return ch_pstrdup(mp,value);
}

ch_rule_t * ch_rule_parse(ch_pool_t *mp,cJSON *entry){

    cJSON *items;
    ch_rule_t *rule;
    ch_rule_item_t *rule_item;

    const char *proto_str;
    int proto;
    uint64_t id,time;
    const char *type;
    const char *msg;
    const char *name,*value;
    int isEnable;
    int isAnd;

    int i;
    
    isEnable = ch_json_bool_value_get(entry,"isEnable");
    if(!isEnable){
        ch_log(CH_LOG_INFO,"Disable Rule pass it!");
        return NULL;
    }

    proto_str = ch_json_str_value_get_no_cp(entry,"proto");
    proto = ch_proto_value_get(proto_str);

    if(proto == PROTO_UNK){
        ch_log(CH_LOG_ERR,"Invalid proto:%s",proto_str);
        return NULL;
    }

    items = cJSON_GetObjectItem(entry,"items");
    if(items == NULL||!cJSON_IsArray(items)||cJSON_GetArraySize(items)==0){
        ch_log(CH_LOG_ERR,"Invalid rule,items is empty or not arrays");
        return NULL;
    }

    isAnd = ch_json_bool_value_get(entry,"isAnd");
    id = (uint64_t)ch_json_num_value_get(entry,"id");
    time = (uint64_t)ch_json_num_value_get(entry,"time");
    
    type = ch_json_str_value_get_no_cp(entry,"type");
    msg = ch_json_str_value_get_no_cp(entry,"msg");
       
    name = ch_json_str_value_get_no_cp(entry,"name");
    value = ch_json_str_value_get_no_cp(entry,"value");

    
    rule = (ch_rule_t*)ch_pcalloc(mp,sizeof(*rule));
    INIT_LIST_HEAD(&rule->items);

    for(i = 0;i<cJSON_GetArraySize(items);i++){

        cJSON *item_entry = cJSON_GetArrayItem(items,i);
        rule_item = _rule_item_parse(mp,item_entry);
        if(rule_item == NULL){
            ch_log(CH_LOG_ERR,"Invalid rule item,pass this Rule:%lu!",(unsigned long)id);
            return NULL;
        }

        list_add_tail(&rule_item->node,&rule->items);        
    }

    rule->proto = proto;
    rule->id = id;
    rule->time = time;
    rule->type = _str_value_get(mp,type);
    rule->msg = _str_value_get(mp,msg);
    rule->name = _str_value_get(mp,name);
    rule->value = _str_value_get(mp,value);
    rule->isEnable = isEnable;
    rule->isAnd = isAnd;

    return rule;
}

static inline void _dump_string(FILE *fp,const char *key,const char *str,const char *space){

    fprintf(fp,"%s%s:%s,\n",space,key,str);
}

static inline void _dump_bool(FILE *fp,const char *key,int v,const char *space){

    fprintf(fp,"%s%s:%s,\n",space,key,v?"true":"false");
}

static inline void _dump_number(FILE *fp,const char *key,unsigned long v,const char *space){

    fprintf(fp,"%s%s:%lu,\n",space,key,v);
}

void ch_rule_dump(FILE *fp,ch_rule_t *rule){

    const char *sp1 = "     ";
    const char *sp2 = "         ";

    ch_rule_item_t *ritem;
    fprintf(fp,"Dump Rule--------------------------------\n");
    fprintf(fp,"{\n");

    _dump_number(fp,"id",(unsigned long)rule->id,sp1);
    _dump_number(fp,"time",(unsigned long)rule->time,sp1);
    _dump_number(fp,"proto",(unsigned long)rule->proto,sp1);
    _dump_bool(fp,"isEnable",rule->isEnable,sp1);
    _dump_bool(fp,"isAnd",rule->isAnd,sp1);
    _dump_string(fp,"type",rule->type,sp1);
    _dump_string(fp,"msg",rule->msg,sp1);
    _dump_string(fp,"name",rule->name,sp1);
    _dump_string(fp,"value",rule->value,sp1);

    fprintf(fp,"    items:[\n");

    list_for_each_entry(ritem,&rule->items,node){

        fprintf(fp,"%s{\n",sp2);
        _dump_number(fp,"target",(unsigned long)ritem->target,sp2);
        _dump_number(fp,"op",(unsigned long)ritem->op,sp2);
        _dump_bool(fp,"isAnd",ritem->isAnd,sp2);
        _dump_bool(fp,"isArray",ritem->isArray,sp2);
        _dump_bool(fp,"isHex",ritem->isHex,sp2);
        _dump_bool(fp,"isnot",ritem->isnot,sp2);
        _dump_string(fp,"value",ritem->value,sp2);

        fprintf(fp,"%s},\n",sp2);
    }

    fprintf(fp,"    ]\n");
    fprintf(fp,"}\n");
}
