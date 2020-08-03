#include "ch_rule_engine.h"
#include "ch_log.h"
#include "ch_rule_utils.h"

ch_rule_engine_t * ch_rule_engine_create(ch_pool_t *mp,const char *cfname){


    ch_rule_engine_t *rengine = (ch_rule_engine_t*)ch_pcalloc(mp,sizeof(*rengine));

    rengine->mp = mp;
    rengine->recontext = ch_rule_engine_context_create(mp,cfname);

    if(rengine->recontext == NULL){

        ch_log(CH_LOG_ERR,"Create Rule Context Failed for rule Engine,cfname:%s!",cfname);
        return NULL;
    }

    rengine->protos = ch_protos_to_arrays(mp,rengine->recontext->protos,",");

    return rengine;
}


static int _is_my_proto(ch_rule_target_context_t *tcontext,ch_array_header_t *arr){

    int i;
    ch_rule_int_t *int_v,**elts;

    if(arr == NULL||arr->nelts == 0)
        return 0;

    elts = (ch_rule_int_t**)arr->elts;

    for(i = 0;i<arr->nelts;i++){

        int_v = elts[i]; 

        if(tcontext->isMyProto(tcontext,int_v->value))
            return 1;
    }

    return 0;
}

int ch_rule_engine_match(ch_rule_engine_t *rengine,ch_rule_target_context_t *tcontext){


    if(!_is_my_proto(tcontext,rengine->protos))
        return 0;

    return ch_rule_engine_context_run_match(rengine->recontext,tcontext);
}

int ch_rule_engine_nmatch(ch_rule_engine_t *rengine,ch_rule_target_context_t *tcontext,
        ch_rule_match_context_t *mcontext){


    if(!_is_my_proto(tcontext,rengine->protos))
        return 0;

    return ch_rule_engine_context_run_nmatch(rengine->recontext,mcontext,tcontext);
}
