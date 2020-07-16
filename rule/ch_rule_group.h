#ifndef CH_RULE_GROUP_H
#define CH_RULE_GROUP_H

typedef struct ch_rule_group_pool_t ch_rule_group_pool_t;
typedef struct ch_rule_group_t ch_rule_group_t;

#include "ch_mpool.h"
#include "ch_list.h"
#include "ch_cjson.h"

struct ch_rule_group_pool_t {

    ch_pool_t *mp;
    struct list_head groups;

};

struct ch_rule_group_t {

    struct list_head node;

    const char* rootDir;
    const char* name;
    const char* engine;

    
    int enable;
    int isInline;
    uint64_t time;
    uint32_t ruleCounts;
    uint32_t enableRuleCounts;
    uint32_t disableRuleCounts;
};


static inline const char * ch_rule_cfname_get(ch_rule_group_t *rgp,char *buf,size_t len){

    snprintf(buf,len,"%s/%s/%s/%s.json",rgp->rootDir,rgp->engine,rgp->name,rgp->name);
    return (const char*)buf;
}


static inline void ch_rule_group_dump(FILE* fp,ch_rule_group_t *group){

    fprintf(fp,"rootDir:%s\n",group->rootDir);
    fprintf(fp,"name:%s\n",group->name);
    fprintf(fp,"engine:%s\n",group->engine);
    fprintf(fp,"enable:%s\n",group->enable?"true":"false");
    fprintf(fp,"isInline:%s\n",group->isInline?"true":"false");
    fprintf(fp,"time:%ld\n",(unsigned long)group->time);
    fprintf(fp,"ruleCounts:%ld\n",(unsigned long)group->ruleCounts);
    fprintf(fp,"enableRuleCounts:%ld\n",(unsigned long)group->enableRuleCounts);
    fprintf(fp,"disableRuleCounts:%ld\n",(unsigned long)group->disableRuleCounts);

} 

extern ch_rule_group_pool_t *ch_rule_groups_load(ch_pool_t *mp,const char *cfname);


#endif /*CH_RULE_GROUP_H*/
