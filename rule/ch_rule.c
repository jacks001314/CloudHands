
#include "ch_rule.h"
#include "ch_rule_constants.h"
#include "ch_rule_utils.h"
#include "ch_log.h"
#include "ch_util.h"
#define MAX_LINE_SIZE 256
static ch_array_header_t *_load_value_arrays_from_file(ch_pool_t *mp,char *file_path){

    char line[MAX_LINE_SIZE] = {0};

    ch_array_header_t *arr = ch_array_make(mp,128,sizeof(char*));;

    if(arr != NULL){
        
        FILE *fp = fopen(file_path,"r");
        if(fp == NULL){
            ch_log(CH_LOG_ERR,"Cannot open file:%s to load arrays!",file_path);
            return NULL;
        }

        while(1==ch_read_line(line,MAX_LINE_SIZE-1,fp)){

            if(*line == '#'||strlen(line)==0)
                continue;

            *(char**)ch_array_push(arr)= ch_pstrdup(mp,line);
        }
        
        fclose(fp);
    }

    return arr;
}

static ch_array_header_t * _load_value_arrays_from_inline(ch_pool_t *mp,const char *value){

    return ch_strsplit_to_arrays(mp,value,ARR_VALUE_SPLIT);
}

ch_array_header_t *_load_value_arrays(ch_pool_t *mp,const char *value){

    char *part,*last,*key,*split_value;

    if(value == NULL||strlen(value)==0)
        return NULL;

    part = ch_strtok((char*)value,":",&last);

    if(part == NULL)
        return NULL;

    if(strcmp(part,"file")==0){
        part = ch_strtok(NULL,":",&last);
        if(part == NULL){
            ch_log(CH_LOG_ERR,"Invalid file value,no file path,value:%s",value);
            return NULL;
        }

        return _load_value_arrays_from_file(mp,part);

    }else if(strcmp(part,"inline")==0){

        key = ch_strtok(NULL,":",&last);
        if(key == NULL){
            ch_log(CH_LOG_ERR,"Invalid inline value,no key,value:%s",value);
            return NULL;
        }
        split_value = ch_strtok(NULL,":",&last);
        if(split_value == NULL){
            ch_log(CH_LOG_ERR,"Invalid inline value,no value,value:%s",value);
            return NULL;
        }

        return _load_value_arrays_from_inline(mp,split_value);

    }else{

        ch_log(CH_LOG_ERR,"UNknown value type:%s",part);
        return NULL;
    }
}

static ch_rule_target_t *_rule_target_create(ch_pool_t *mp,cJSON *entry){

    const char *target_str;
    int target;
    ch_rule_target_t *rule_target;

    target_str= ch_json_str_value_get(mp,entry,"target");
    target = ch_target_value_get(target_str);
    if(target == TARGET_NONE){
        ch_log(CH_LOG_ERR,"Unknown match target:%s",target_str);
        return NULL;
    } 

    rule_target = (ch_rule_target_t*)ch_pcalloc(mp,sizeof(*rule_target));

    rule_target->target = target;
    rule_target->target_str = target_str;
    rule_target->offset = (size_t)ch_json_num_value_get(entry,"offset");
    rule_target->len = (size_t)ch_json_num_value_get(entry,"len");

    return rule_target;
}

static ch_rule_item_t *_rule_item_parse(ch_rule_pool_t *rpool,cJSON *entry){

    ch_array_header_t *arr = NULL;
    ch_pool_t *mp = rpool->mp;
    ch_rule_item_t *rule_item;
    ch_rule_target_t *rule_target;

    const char *op_str;
    int op;
    const char *value;
    int isHex;
    int isArray;
    int isAnd;
    int isnot;

    rule_target = _rule_target_create(mp,entry);

    if(rule_target == NULL){
        ch_log(CH_LOG_ERR,"Create match target failed!");
        return NULL;
    } 
     
    op_str= ch_json_str_value_get(mp,entry,"op");
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
       if(!ch_rule_op_startsWith(value,"file:")&&!ch_rule_op_startsWith(value,"inline:")){
            ch_log(CH_LOG_ERR,"Invalid array value:%s",value);
            return NULL;
        }

       arr = _load_value_arrays(mp,value);

       if(arr == NULL){

           ch_log(CH_LOG_ERR,"Cannot load arrays value:%s",value);
           return NULL;
       }
    }
    
    isHex = ch_json_bool_value_get(entry,"isHex");
    isAnd = ch_json_bool_value_get(entry,"isAnd");
    isnot = ch_json_bool_value_get(entry,"isnot");

    rule_item = (ch_rule_item_t*)ch_pcalloc(mp,sizeof(*rule_item));
    rule_item->value = value;
    rule_item->target = rule_target;
    rule_item->op_str = op_str;
    rule_item->op = op;
    rule_item->isAnd = isAnd;
    rule_item->isArray = isArray;
    rule_item->isHex = isHex;
    rule_item->isnot = isnot;
    rule_item->arr_values = arr;

    return rule_item;
}


static inline const char * _str_value_get(ch_pool_t *mp,const char *value){
    
    if(value == NULL||strlen(value)==0)
        return NULL;
    return ch_pstrdup(mp,value);
}

ch_rule_t * ch_rule_parse(ch_rule_pool_t *rpool,cJSON *entry){

    
    cJSON *items;
    ch_rule_t *rule;
    ch_rule_item_t *rule_item;
    ch_pool_t *mp = rpool->mp;
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

    proto_str = ch_json_str_value_get(mp,entry,"proto");
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
        rule_item = _rule_item_parse(rpool,item_entry);
        if(rule_item == NULL){
            ch_log(CH_LOG_ERR,"Invalid rule item,pass this Rule:%lu!",(unsigned long)id);
            return NULL;
        }

        list_add_tail(&rule_item->node,&rule->items);        
    }

    rule->proto_str = proto_str;
    rule->proto = proto;
    rule->id = id;
    rule->time = time;
    rule->type = _str_value_get(mp,type);
    rule->msg = _str_value_get(mp,msg);
    rule->name = _str_value_get(mp,name);
    rule->value = _str_value_get(mp,value);
    rule->isEnable = isEnable;
    rule->isAnd = isAnd;
    rule->rpool = rpool;

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
    _dump_string(fp,"proto",rule->proto_str,sp1);
    _dump_bool(fp,"isEnable",rule->isEnable,sp1);
    _dump_bool(fp,"isAnd",rule->isAnd,sp1);
    _dump_string(fp,"type",rule->type,sp1);
    _dump_string(fp,"msg",rule->msg,sp1);
    _dump_string(fp,"name",rule->name,sp1);
    _dump_string(fp,"value",rule->value,sp1);

    fprintf(fp,"    items:[\n");

    list_for_each_entry(ritem,&rule->items,node){

        fprintf(fp,"%s{\n",sp2);
        _dump_string(fp,"target.str",ritem->target->target_str,sp2);
        _dump_number(fp,"target.num",ritem->target->target,sp2);
        _dump_number(fp,"target.offset",ritem->target->offset,sp2);
        _dump_number(fp,"target.len",ritem->target->len,sp2);

        _dump_string(fp,"op",ritem->op_str,sp2);
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
