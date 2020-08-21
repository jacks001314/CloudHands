#ifndef CH_PTABLE_WATCH_TASK_H
#define CH_PTABLE_WATCH_TASK_H

typedef struct ch_ptable_watch_task_t ch_ptable_watch_task_t;

#include "ch_ptable.h"
#include "ch_task.h"

struct ch_ptable_watch_task_t {

    ch_task_t task;

    struct list_head ptables;

    ch_pool_t *mp;

    uint64_t check_tv;
};

extern ch_ptable_watch_task_t * ch_ptable_watch_task_create(ch_pool_t *mp,uint64_t check_tv);

extern void ch_ptable_watch_task_add(ch_ptable_watch_task_t *watch,ch_ptable_t *ptable);

//extern void ch_ptable_watch_task_run(ch_ptable_watch_task_t *watch);


#endif /*CH_PTABLE_WATCH_TASK_H*/
