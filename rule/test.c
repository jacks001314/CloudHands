#include <stdio.h>
#include <stdlib.h>

#include "ch_rule_group.h"
#include "ch_rule_utils.h"
#include "ch_log.h"
#include "ch_rule_engine_context.h"

static inline void arr_dump(ch_array_header_t *arr){

    int i;
    char **elts = (char**)arr->elts;

    for(i = 0;i<arr->nelts;i++){

        printf("%s\n",elts[i]);
    }

}

static _is_MYProto_for_http(ch_rule_target_context_t *tcontext,int proto){

    return proto == PROTO_HTTP;
}

static const char *host_v,*uri_v;

static const char * http_target_get(ch_rule_target_context_t *tcontext,ch_rule_target_t *rtarget,int isHex){

    int target = rtarget->target;
    const char *result;

    if(target == TARGET_HTTP_HOST){
        result = host_v;
    }else if(target == TARGET_HTTP_URI)
    {
        result = uri_v;
    }

    printf("%s:%s\n",rtarget->target_str,result);
    return result;
}


static void _dump_rule_pools(ch_rule_engine_context_t *recontext){

    ch_rule_pool_t *rpool;

    list_for_each_entry(rpool,&recontext->rpools,node){

        ch_rule_pool_dump(stdout,rpool);
    }
}

static inline void test_rule_match(char **argv){

    const char *cfname = argv[1];
    host_v = argv[2];
    uri_v = argv[3];

    ch_rule_match_context_t tmp,*mcontext = &tmp;
    ch_rule_target_context_t t_tmp,*tcontext = &t_tmp;

    INIT_LIST_HEAD(&mcontext->match_rules);

    mcontext->match_n = 0;
    tcontext->proto = "http";
    tcontext->isMyProto = _is_MYProto_for_http;
    tcontext->target = http_target_get;

    ch_pool_t *mp = ch_pool_create(1024);
    ch_log_init(mp,"/tmp/test.log",CH_LOG_INFO);

    ch_rule_engine_context_t *recontext = ch_rule_engine_context_create(mp,cfname);

    printf("Protos:%s\n",recontext->protos);

    int smatch = ch_rule_engine_context_run_match(recontext,tcontext);

    printf("Simple Match:%s\n",smatch?"true":"false");

    int nmatch = ch_rule_engine_context_run_nmatch(recontext,mcontext,tcontext);

    printf("Muilti Match:%s,%d\n",nmatch?"true":"false",mcontext->match_n);

    _dump_rule_pools(recontext);

    ch_rule_engine_context_destroy(recontext);
    ch_pool_destroy(mp);
}


static inline void test_ops(char **argv){

    const char *target = argv[1];
    const char *match = argv[2];

    printf("%s contains %s :%d\n",target,match,ch_rule_op_contains(target,match));
    printf("%s startsWith %s :%d\n",target,match,ch_rule_op_startsWith(target,match));
    printf("%s endsWith %s :%d\n",target,match,ch_rule_op_endsWith(target,match));
    printf("%s eq %s :%d\n",target,match,ch_rule_op_eq(target,match));
    printf("%s lt %s :%d\n",target,match,ch_rule_op_lt(target,match));
    printf("%s gt %s :%d\n",target,match,ch_rule_op_gt(target,match));
    printf("%s le %s :%d\n",target,match,ch_rule_op_le(target,match));
    printf("%s ge %s :%d\n",target,match,ch_rule_op_ge(target,match));
}


static inline void test_split(char **argv){

    const char *str = argv[1];
    const char *s = argv[2];

    ch_pool_t *mp = ch_pool_create(1024);
    ch_array_header_t *arr =ch_protos_to_arrays(mp,str,s);
    int i;
    ch_rule_int_t *elt;
    ch_rule_int_t **elts = (ch_rule_int_t**)arr->elts;

    for(i = 0;i<arr->nelts;i++){

        elt = elts[i];
        printf("%d\n",elt->value);
    }

    printf("str:%s,contains http proto:%s\n",str,ch_rule_int_values_contains(arr,PROTO_HTTP)?"true":"false");
    ch_pool_destroy(mp);
}

static inline void test_rule_group(char **argv){

    const char *fname = argv[1];


    ch_pool_t *mp = ch_pool_create(4096);

    ch_rule_group_pool_t *gp = ch_rule_groups_load(mp,fname);

    ch_rule_group_t *rgp;

    list_for_each_entry(rgp,&gp->groups,node){

       ch_rule_group_dump(stdout,rgp);
       printf("-----------------------------\n");
    }


    ch_pool_destroy(mp);
}

int main(int argc,char **argv){

    test_rule_match(argv);
    //test_rule_group(argv);
    //test_rule_context(argv);

}

