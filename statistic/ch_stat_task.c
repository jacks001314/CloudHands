#include "ch_stat_task.h"

static int _stat_task_run(ch_task_t *tsk,void *priv_data ch_unused){

    ch_stat_ring_entry_t *entry;
    ch_stat_task_t *task = (ch_stat_task_t*)tsk;


    if(rte_ring_dequeue(task->ring,(void**)(&entry))==0){

        ch_stat_pool_handle(task->st_pool,entry->time,entry->pkt_size,entry->pkt_type);

        rte_mempool_put(task->entry_pool,(void*)entry);

        ch_stat_pool_update(task->st_pool,entry->time);
    }

    return TASK_RETURN_OK;
}

static int _stat_task_init(ch_task_t *task ch_unused,void *priv_data ch_unused){


	return 0;
}

static int _stat_task_exit(ch_task_t *task ch_unused,void *priv_data ch_unused){


	return 0;

}

ch_stat_task_t * ch_stat_task_create(ch_stat_pool_t *st_pool,size_t pool_size,size_t ring_size){

    ch_stat_task_t *stat_task = NULL;

    stat_task = (ch_stat_task_t*)ch_pcalloc(st_pool->mp,sizeof(*stat_task));

   stat_task->task.tsk_id = 1;
   stat_task->task.run = _stat_task_run;
   stat_task->task.init = _stat_task_init;
   stat_task->task.exit = _stat_task_exit;

   stat_task->st_pool = st_pool;

   /*create mbuf pool */	
   stat_task->entry_pool = rte_mempool_create("stat_mpool",
                                              pool_size,
		                                      sizeof(ch_stat_ring_entry_t),0,0,
                                              NULL, NULL, NULL, NULL,rte_socket_id(),0);

   if(stat_task->entry_pool == NULL){

       fprintf(stderr,"Create dpdk memory pool failed!");

       return NULL;
   }

   stat_task->ring = rte_ring_create("stat_ring",ring_size,rte_socket_id(),0);

   if(stat_task->ring == NULL){

       fprintf(stderr,"Create Ring failed!");
       return NULL;
   }


    return stat_task;
}


void ch_stat_task_entry_put(ch_stat_task_t *task,uint64_t time,uint64_t pkt_size,int pkt_type){

    void *entry;
    ch_stat_ring_entry_t *ring_entry;



    int rc = rte_mempool_get(task->entry_pool,&entry);

    if(rc<0){

        fprintf(stderr,"Cannot get entry from mempool!");
        return;
    }

    ring_entry = (ch_stat_ring_entry_t*)entry;

    ring_entry->time = time;
    ring_entry->pkt_size = pkt_size;
    ring_entry->pkt_type = pkt_type;

    if(rte_ring_full(task->ring)){

        fprintf(stderr,"Ring is full!");
        rte_mempool_put(task->entry_pool,entry);

    }else{

        rte_ring_enqueue(task->ring,entry);
    }

}
