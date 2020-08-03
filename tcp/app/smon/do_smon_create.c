/*
 *
 *      Filename: do_smon_other.c
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-05-17 17:41:03
 * Last Modified: 2018-07-17 17:01:06
 */


static void * do_smon_session_entry_create(ch_mpool_agent_t *mpa,ch_tcp_app_t *app ch_unused,ch_proto_session_store_t *pstore ch_unused){

	ch_pool_t *mp;

	ch_smon_session_entry_t *smon_entry = NULL;

    if(mpa){

        mp = ch_mpool_agent_alloc(mpa);
    }else{
        mp = ch_pool_create(1024);
    }

	if(mp == NULL){
	
		ch_log(CH_LOG_ERR,"Cannot create memory pool for smon session entry!");
		return NULL;
	}

	smon_entry = (ch_smon_session_entry_t*)ch_pcalloc(mp,sizeof(*smon_entry));

	ch_smon_session_entry_init(smon_entry,mp);


	return (void*)smon_entry;
}


#define SMON_ENTRY_CAN_STORE(entry) ((entry)->req_content_fpath!=NULL || (entry)->res_content_fpath!=NULL)


static void do_smon_session_entry_clean(ch_mpool_agent_t *mpa,ch_tcp_app_t *app ch_unused,ch_proto_session_store_t *pstore,ch_tcp_session_t *tsession){

	ch_smon_session_entry_t *smon_entry = (ch_smon_session_entry_t*)tsession->sentry;

	if (SMON_ENTRY_CAN_STORE(smon_entry)) {

		ch_proto_session_store_write(pstore,tsession,(void*)smon_entry);
	}
	

	ch_smon_session_entry_fin(smon_entry);

    if(mpa){

        ch_mpool_agent_free(mpa,smon_entry->mp);
    }else{
        ch_pool_destroy(smon_entry->mp);
    }

}

