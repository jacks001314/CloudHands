/*
 *
 *      Filename: ch_task.c
 *
 *        Author: shajf,csp001314@163.com
 *   Description: ---
 *        Create: 2017-01-17 14:34:40
 * Last Modified: 2017-11-09 10:30:09
 */

#include "ch_task.h"


int  ch_task_run(ch_task_t *tsk){

    int rc;

    if(tsk->init){
        if(tsk->init(tsk,tsk->priv_data)){
            fprintf(stderr,"Init task failed before running!\n");
            return -1;
        }
    }

    fprintf(stdout,"Start to run task[%lu] in cpu core[%lu].\n",(unsigned long)tsk->tsk_id,
		(unsigned long)tsk->core->core_id);

    while(1){

        rc = tsk->run(tsk,tsk->priv_data);
        
        if(rc == TASK_RETURN_EXIT){

            fprintf(stderr,"Task[%lu] will be exit!\n",(unsigned long)tsk->tsk_id);
            if(tsk->exit){
                tsk->exit(tsk,tsk->priv_data);
            }

            return -1;
        }

        if(rc == TASK_RETURN_SLEEP){
            usleep(5);
        }
    }//while

}

static int master_core_run(ch_task_t *task,void *priv_data ch_unused){

    fprintf(stdout,"The Master Core:%lu,The Task is nothing done!\n",(unsigned long)task->core->core_id);
    while(1){

        sleep(60);
    }

    return 0;
}

void ch_master_core_bind(ch_pool_t *mp,ch_core_t *core,ch_task_t *tsk){

    ch_task_t *task =  tsk;

    if(task == NULL)
    {
        task = (ch_task_t*)ch_pcalloc(mp,sizeof(*task));
        task->core = core;
        task->priv_data = NULL;
        task->tsk_id = 0;
        task->init = NULL;
        task->run = master_core_run;
        task->exit = NULL;

        core->tsk = task;
    }else{

        task->core = core;
        core->tsk = task;
    }
}

