
#include "ch_rule_engine_context.h"
#include "ch_cjson.h"

static int _do_parse(cJSON *root,void *obj){

    ch_rule_engine_context_t *recontext = (ch_rule_engine_context_t*)obj;

    ch_pool_t *mp = recontext->mp;

    recontext->protos = ch_json_str_value_get(mp,root,"protos");
    recontext->engine = ch_json_str_value_get(mp,root,"engine");
    recontext->ruleGroupPath = ch_json_str_value_get(mp,root,"ruleGroupPath");
    recontext->matchThenStop = ch_json_bool_value_get(root,"matchThenStop");

    return 0;
}


static int 

static int _load_rule_pools(ch_rule_engine_context_t *recontext){


}

ch_rule_engine_context_t *ch_rule_engine_context_create(ch_pool_t *mp,const char *cfname){

    ch_rule_engine_context_t *recontext = ch_pcalloc(mp,sizeof(*recontext));

    recontext->mp = mp;
    INIT_LIST_HEAD(&recontext->rpools);

    if(ch_cjson_parse(cfname,(void*)recontext,_do_parse))
    {
        ch_log(CH_LOG_ERR,"Parse Rule Engine Context config path failed,path:%s",cfname);
        return NULL;
    }

    recontext->rule_group_pool = ch_rule_groups_load(mp,recontext->ruleGroupPath);

    if(recontext->rule_group_pool == NULL){

        ch_log(CH_LOG_ERR,"Load Rule Group Fialed from path:%s",recontext->ruleGroupPath);
        return NULL;
    }


    return recontext;
}

void ch_rule_engine_context_destroy(ch_rule_engine_context_t *recontext){


}

int ch_rule_engine_context_run_match(ch_rule_engine_context_t *recontext,ch_rule_target_context_t *tcontext){


}

int ch_rule_engine_context_run_nmatch(ch_rule_engine_context_t *recontext,
        ch_rule_match_context_t *mcontext,
        ch_rule_target_context_t *tcontext){



}
