
#include "ch_rule_pool.h"
#include "ch_log.h"
#include "ch_list.h"
#include "ch_rule_constants.h"
#include "ch_rule_utils.h"
#include "ch_rule.h"

static int _is_valid_rule(ch_rule_pool_t *rpool,cJSON *entry){

    const char *proto_str;
    int proto;

    if(rpool->accept_protos == NULL||rpool->accept_protos->nelts == 0)
        return 1;

    if(!ch_json_bool_value_get(entry,"isEnable"))
        return 0;

    proto_str = ch_json_str_value_get_no_cp(entry,"proto");

    if(proto_str == NULL||strlen(proto_str)==0)
        return 0;

    proto = ch_proto_value_get(proto_str);

    if(proto == PROTO_UNK)
        return 0;

    return ch_rule_int_values_contains(rpool->accept_protos,proto);
}

static int _rules_parse(cJSON *root,void *obj){

    ch_rule_pool_t *rpool = (ch_rule_pool_t*)obj;
    ch_rule_t *rule;
    cJSON *rules;
    int count = 0;
    int i;

    rules = cJSON_GetObjectItem(root,"rules");

    if(rules == NULL||!cJSON_IsArray(rules)){

        ch_log(CH_LOG_ERR,"Empty Rules!");
        return -1;
    }

    for(i=0;i<cJSON_GetArraySize(rules);i++){

        cJSON *entry = cJSON_GetArrayItem(rules,i);
        if(!_is_valid_rule(rpool,entry))
            continue;

        rule = ch_rule_parse(rpool,entry);
        if(rule == NULL)
            continue;

        list_add_tail(&rule->node,&rpool->rules);
        count++;
    }

    ch_log(CH_LOG_INFO,"Load rule number:%d,Engine:%s,Name:%s",
            count,
            rpool->rgroup->engine,
            rpool->rgroup->name);


    return 0;

}

ch_rule_pool_t * ch_rule_pool_create(ch_rule_group_t *rule_group,int matchThenStop,const char *protos){

    char buff[1024] = {0};

    ch_rule_pool_t *rpool;
    const char *cfname; 
    ch_pool_t *mp;

    cfname = ch_rule_cfname_get(rule_group,buff,1024);
    if(strlen(cfname)== 0){

        ch_log(CH_LOG_ERR,"Empty Rule Path!");
        return NULL;
    }


    mp = ch_pool_create(4096);

    if(mp == NULL){

        ch_log(CH_LOG_ERR,"create memory pool failed for rule pools!");
        return NULL;
    }

    rpool = (ch_rule_pool_t*)ch_pcalloc(mp,sizeof(*rpool));

    rpool->mp = mp;
    rpool->matchThenStop = matchThenStop;
    rpool->rgroup = rule_group;
    INIT_LIST_HEAD(&rpool->rules);

    if(protos == NULL||strlen(protos)==0)
        rpool->accept_protos = NULL;
    else
        rpool->accept_protos = ch_protos_to_arrays(mp,protos,",");

    if(ch_cjson_parse(cfname,(void*)rpool,_rules_parse)){

        ch_log(CH_LOG_ERR,"Parse rule failed,path:%s",cfname);
        ch_pool_destroy(mp);
        return NULL;
    }

    return rpool;
}

void ch_rule_pool_destroy(ch_rule_pool_t *rpool){

    ch_pool_destroy(rpool->mp);

}

void ch_rule_pool_dump(FILE *fp,ch_rule_pool_t *rpool){

    
    ch_rule_t *rule;

    fprintf(fp,"Dump Rules:-----------------------------------\n");

    list_for_each_entry(rule,&rpool->rules,node){

        ch_rule_dump(fp,rule);
    }
}

