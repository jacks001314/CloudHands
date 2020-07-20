
#include "ch_rule_engine_context.h"
#include "ch_cjson.h"
#include "ch_rule_match.h"
#include "ch_log.h"

static int _do_parse(cJSON *root,void *obj){

    ch_rule_engine_context_t *recontext = (ch_rule_engine_context_t*)obj;

    ch_pool_t *mp = recontext->mp;

    recontext->protos = ch_json_str_value_get(mp,root,"protos");
    recontext->engine = ch_json_str_value_get(mp,root,"engine");
    recontext->ruleGroupPath = ch_json_str_value_get(mp,root,"ruleGroupPath");
    recontext->matchThenStop = ch_json_bool_value_get(root,"matchThenStop");

    return 0;
}


static int is_my_rule_group(ch_rule_engine_context_t *recontext,ch_rule_group_t *rule_group){

    if(!rule_group->enable)
        return 0;

    return strcasecmp(recontext->engine,rule_group->engine)==0;
}

static void _load_rule_pools(ch_rule_engine_context_t *recontext){

    ch_rule_pool_t *rpool;
    ch_rule_group_t *rule_group;

    list_for_each_entry(rule_group,&recontext->rule_group_pool->groups,node){

        if(is_my_rule_group(recontext,rule_group)){

            rpool = ch_rule_pool_create(rule_group,recontext->matchThenStop,ch_pstrdup(recontext->mp,recontext->protos));

            if(rpool == NULL){

                ch_log(CH_LOG_ERR,"Cannot create rule pool ,engine:%s,name:%s,protos:%s",
                        rule_group->engine,rule_group->name,recontext->protos);
            }else{

                ch_log(CH_LOG_INFO,"Create Rule Pool,engine:%s,name:%s,protos:%s is ok!",
                        rule_group->engine,rule_group->name,recontext->protos);

                list_add_tail(&rpool->node,&recontext->rpools);

            }
        }
    }

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

    _load_rule_pools(recontext);

    return recontext;
}

void ch_rule_engine_context_destroy(ch_rule_engine_context_t *recontext){

    ch_rule_pool_t *rpool,*tmp_rpool;

    list_for_each_entry_safe(rpool,tmp_rpool,&recontext->rpools,node){

        ch_rule_pool_destroy(rpool);
    }
}

int ch_rule_engine_context_run_match(ch_rule_engine_context_t *recontext,ch_rule_target_context_t *tcontext){

    ch_rule_pool_t *rpool;

    list_for_each_entry(rpool,&recontext->rpools,node){

        if(ch_rule_match(tcontext,rpool))
            return 1;
    }

    return 0;
}

int ch_rule_engine_context_run_nmatch(ch_rule_engine_context_t *recontext,
        ch_rule_match_context_t *mcontext,
        ch_rule_target_context_t *tcontext){


    int count = 0;
    ch_rule_pool_t *rpool;

    list_for_each_entry(rpool,&recontext->rpools,node){

        if(ch_rule_nmatch(mcontext,tcontext,rpool)){

            count++;

            if(recontext->matchThenStop)
                return 1;
        }
    }

    return count>0;
}

