#include "ch_rule_group.h"
#include "ch_log.h"

static ch_rule_group_t * _parse_rule_group(cJSON *entry,ch_pool_t *mp) {

    ch_rule_group_t *rule_group = (ch_rule_group_t*)ch_pcalloc(mp,sizeof(*rule_group));

    rule_group->rootDir = ch_json_str_value_get(mp,entry,"rootDir");
    rule_group->name = ch_json_str_value_get(mp,entry,"name");
    rule_group->engine = ch_json_str_value_get(mp,entry,"engine");
    rule_group->enable = ch_json_bool_value_get(entry,"enable");
    rule_group->isInline = ch_json_bool_value_get(entry,"isInline");
    rule_group->ruleCounts = (uint32_t)ch_json_num_value_get(entry,"ruleCounts");
    rule_group->enableRuleCounts = (uint32_t)ch_json_num_value_get(entry,"enableRuleCounts");
    rule_group->disableRuleCounts = (uint32_t)ch_json_num_value_get(entry,"disableRuleCounts");
    rule_group->time = (uint64_t)ch_json_num_value_get(entry,"time");

    return rule_group;
} 

static int _rule_groups_parse(cJSON *root,void *obj) {

    ch_rule_group_t *rule_group;
    int i;
    ch_rule_group_pool_t *rgp = (ch_rule_group_pool_t*)obj;

    cJSON *groups = cJSON_GetObjectItem(root,"groups");
    if(groups == NULL||!cJSON_IsArray(groups)){

        ch_log(CH_LOG_ERR,"The RuleGroup.json groups is emtry!");
        return -1;
    }

    for(i=0;i<cJSON_GetArraySize(groups);i++){

        cJSON *entry = cJSON_GetArrayItem(groups,i);

        rule_group = _parse_rule_group(entry,rgp->mp);

        list_add_tail(&rule_group->node,&rgp->groups);
    }

    return 0;
}

ch_rule_group_pool_t *ch_rule_groups_load(ch_pool_t *mp,const char *cfname){

    ch_rule_group_pool_t *rgp = (ch_rule_group_pool_t*)ch_palloc(mp,sizeof(*rgp));

    rgp->mp = mp;
    INIT_LIST_HEAD(&rgp->groups);

    if(ch_cjson_parse(cfname,(void*)rgp,_rule_groups_parse)){

        ch_log(CH_LOG_ERR,"Parse rule group json file:%s failed!",cfname);
        return NULL;
    }


    return rgp;
}
