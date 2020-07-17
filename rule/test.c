#include <stdio.h>
#include <stdlib.h>

#include "ch_rule_group.h"
#include "ch_rule_utils.h"
#include "ch_rule_context.h"


static inline void arr_dump(ch_array_header_t *arr){

    int i;
    char **elts = (char**)arr->elts;

    for(i = 0;i<arr->nelts;i++){

        printf("%s\n",elts[i]);
    }

}

static inline void test_rule_context(char **argv){

    const char *value = argv[1];

    ch_rule_context_t *rcontext = ch_rule_context_create();

    ch_array_header_t *arr = ch_rule_context_find_value_arrays(rcontext,value);

    arr_dump(arr);

    printf("-----------------------------\n");
    arr = ch_rule_context_find_value_arrays(rcontext,value);

    arr_dump(arr);

    ch_rule_context_destroy(rcontext);
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

    test_rule_group(argv);
    //test_rule_context(argv);

}

