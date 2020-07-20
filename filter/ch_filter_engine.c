#include "ch_filter_engine.h"
#include "ch_log.h"
#include "ch_rule_utils.h"

ch_filter_engine_t * ch_filter_engine_create(ch_pool_t *mp,
        const char *cfname){


    ch_filter_engine_t *fengine = (ch_filter_engine_t*)ch_pcalloc(mp,sizeof(*fengine));

    fengine->mp = mp;
    fengine->recontext = ch_rule_engine_context_create(mp,cfname);

    if(fengine->recontext == NULL){

        ch_log(CH_LOG_ERR,"Create Rule Context Failed for Filter Engine,cfname:%s!",cfname);
        return NULL;
    }

    fengine->protos = ch_protos_to_arrays(mp,fengine->recontext->protos,",");

    return fengine;
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

int ch_filter_engine_accept(ch_filter_engine_t *fengine,
        ch_rule_target_context_t *tcontext){


    if(!_is_my_proto(tcontext,fengine->protos))
        return 1;

    return !ch_rule_engine_context_run_match(fengine->recontext,tcontext);
}

