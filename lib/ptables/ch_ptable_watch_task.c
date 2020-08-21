#include "ch_ptable_watch_task.h"
#include "ch_log.h"



void ch_ptable_watch_task_add(ch_ptable_watch_task_t *watch,ch_ptable_t *ptable){

    list_add_tail(&ptable->node,&watch->ptables);
}


static inline struct list_head * entry_header(ch_ptable_t *ptable,size_t idx){

    return ptable->entries+idx;
}

static inline uint64_t  _entry_is_timeout(ch_ptable_t *ptable,ch_ptable_entry_t *ptable_entry,uint64_t cur_time){

	uint64_t tv = cur_time-ptable_entry->last_time;

    if(tv>ptable->entry_timeout)
        return tv;

    return 0;
}

static void _watch_ptable(ch_ptable_t *ptable){

	size_t c = 0;

    struct list_head *h;
    size_t i;
    uint64_t cur_time = ch_get_current_timems()/1000; 
	uint64_t tv;

	ch_ptable_entry_t *entry,*tmp_entry;

    ch_rwlock_read_lock(&ptable->rwlock);
	for(i = 0; i<ptable->tbl_size; i++){
    
        h = entry_header(ptable,i);

        /*get read lock*/
        list_for_each_entry_safe(entry,tmp_entry,h,entry){

			tv = _entry_is_timeout(ptable,entry,cur_time);

            if(tv){
                if(rte_ring_full(ptable->timeout_entries))
                {
                    ch_log(CH_LOG_INFO,"Ptable:%s timeout entries queue is full-----");
                    break;
                }

                entry->tv = tv;
				c+=1;
                rte_ring_enqueue(ptable->timeout_entries,(void*)entry);
            }
        }

    }
    
    ch_rwlock_read_unlock(&ptable->rwlock);

    ch_log(CH_LOG_INFO,"PTable:%s Thread:%d has timeout entries:%lu that been add to timeout ring!",
            ptable->name,
            rte_lcore_id(),
            (unsigned long)c);

}


static int _ptable_watch_task_run(ch_task_t *task,void *priv_data ch_unused){

    ch_ptable_watch_task_t *watch = (ch_ptable_watch_task_t*)task;

    ch_ptable_t *ptable;


    while(1){
        
        list_for_each_entry(ptable,&watch->ptables,node){
            
            /*wait current timeout entries has been processed*/
            if(!rte_ring_empty(ptable->timeout_entries))
                continue;
            
            _watch_ptable(ptable);
        }

        sleep(watch->check_tv);
    }

    return TASK_RETURN_OK;
}

static int _ptable_watch_task_init(ch_task_t *task,void *priv_data){

    task = task;
    priv_data = priv_data;

    return 0;
}

static int _ptable_watch_task_exit(ch_task_t *task,void *priv_data){

    task = task;
    priv_data = priv_data;

    return 0;
}

ch_ptable_watch_task_t * ch_ptable_watch_task_create(ch_pool_t *mp,uint64_t check_tv){

    ch_ptable_watch_task_t *watch = (ch_ptable_watch_task_t*)ch_pcalloc(mp,sizeof(*watch));

    watch->task.tsk_id = 0;

    watch->task.init = _ptable_watch_task_init;
    watch->task.run = _ptable_watch_task_run;
    watch->task.exit = _ptable_watch_task_exit;

    watch->mp = mp;
    watch->check_tv = check_tv;
    INIT_LIST_HEAD(&watch->ptables);

    return watch;
}
