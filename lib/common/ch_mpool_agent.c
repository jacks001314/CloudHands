#include <limits.h>
#include "ch_mpool_agent.h"

int ch_mpool_agent_init(ch_mpool_agent_t *mpa,unsigned long max_cache_n,unsigned long pool_size,unsigned long pre_alloc_n){

    unsigned long i = 0;

    ch_pool_t *mp;

    INIT_LIST_HEAD(&mpa->cache_list);
    mpa->max_cache_n = max_cache_n==0?100000:max_cache_n;
    mpa->cur_cache_n = 0;
    mpa->pool_size = pool_size == 0?4096:pool_size;
    mpa->pre_alloc_n = pre_alloc_n==0?1000:pre_alloc_n;

    mpa->from_mems = 0;
    mpa->from_caches = 0;

    mpa->allocs = 0;
    mpa->frees = 0;

    for(i = 0;i<mpa->pre_alloc_n;i++){


        mp = ch_pool_create(mpa->pool_size);

        if(mp){

            mpa->cur_cache_n = mpa->cur_cache_n+1;
            list_add(&mp->node,&mpa->cache_list);

        }
    }

    return 0;

}

ch_mpool_agent_t *ch_mpool_agent_create(ch_pool_t *mp,
        unsigned long max_cache_n,unsigned long pool_size,
        unsigned long pre_alloc_n){


    ch_mpool_agent_t *mpa = (ch_mpool_agent_t*)ch_pcalloc(mp,sizeof(*mpa));

    ch_mpool_agent_init(mpa,max_cache_n,pool_size,pre_alloc_n);

    return mpa;
}

ch_pool_t *ch_mpool_agent_alloc(ch_mpool_agent_t *mpa){


    ch_pool_t *mp = NULL;

    if(!list_empty(&mpa->cache_list)){
        
        mp = list_first_entry(&mpa->cache_list,ch_pool_t,node);
        
        list_del(&mp->node);
        mpa->cur_cache_n = mpa->cur_cache_n-1;
        mpa->from_caches = (mpa->from_caches+1)%ULONG_MAX;
    }else{

        mp = ch_pool_create(mpa->pool_size);
        mpa->from_mems = (mpa->from_mems+1)%ULONG_MAX;
    }

    if(mp){

        ch_pool_reset(mp);
    }

    mpa->allocs = (mpa->allocs+1)%ULONG_MAX;
    return mp;
}

void ch_mpool_agent_free(ch_mpool_agent_t *mpa,ch_pool_t *mp){

    if(mpa->cur_cache_n<mpa->max_cache_n){

        ch_pool_reset(mp);
        list_add(&mp->node,&mpa->cache_list);
        mpa->cur_cache_n = mpa->cur_cache_n+1;

    }else{

        ch_pool_destroy(mp);
    }

    mpa->frees = (mpa->frees+1)%ULONG_MAX;
}

void ch_mpool_agent_log(ch_mpool_agent_t *mpa){


    ch_log(CH_LOG_INFO,"MaxCaches:%lu,CurCaches:%lu,PSize:%lu,PreAllocs:%lu,FromCaches:%lu,FromMems:%lu,allocs:%lu,frees:%lu",
            mpa->max_cache_n,mpa->cur_cache_n,mpa->pool_size,mpa->pre_alloc_n,mpa->from_caches,mpa->from_mems,mpa->allocs,mpa->frees);

} 

void ch_mpool_agent_dump(ch_mpool_agent_t *mpa,FILE *out){


    fprintf(out,"MaxCaches:%lu,CurCaches:%lu,PSize:%lu,PreAllocs:%lu,FromCaches:%lu,FromMems:%lu,allocs:%lu,frees:%lu\n",
            mpa->max_cache_n,mpa->cur_cache_n,mpa->pool_size,mpa->pre_alloc_n,mpa->from_caches,mpa->from_mems,mpa->allocs,mpa->frees);


    fflush(out);
} 

