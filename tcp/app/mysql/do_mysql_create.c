/*
 * =====================================================================================
 *
 *       Filename:  do_mysql_create.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  8/31/2021 02:07:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jacksha (shajf), csp001314@163.com
 *   Organization:  
 *
 * =====================================================================================
 */


static void * do_mysql_session_entry_create(ch_mpool_agent_t *mpa,ch_tcp_app_t *app ch_unused,ch_proto_session_store_t *pstore ch_unused){

	ch_pool_t *mp;

	ch_mysql_session_entry_t *mysql_entry = NULL;

    if(mpa){

        mp = ch_mpool_agent_alloc(mpa);
    }else{
        mp = ch_pool_create(1024);
    }

	if(mp == NULL){
	
		ch_log(CH_LOG_ERR,"Cannot create memory pool for mysql session entry!");
		return NULL;
	}

	mysql_entry = (ch_mysql_session_entry_t*)ch_pcalloc(mp,sizeof(*mysql_entry));

	ch_mysql_session_entry_init(mysql_entry,mp,&pstore->g_buffer);


	return (void*)mysql_entry;
}

static void do_mysql_session_entry_clean(ch_mpool_agent_t *mpa,ch_tcp_app_t *app ch_unused,ch_proto_session_store_t *pstore,ch_tcp_session_t *tsession){

	ch_mysql_session_entry_t *mysql_entry = (ch_mysql_session_entry_t*)tsession->sentry;

	if (mysql_entry->msession&&mysql_entry->msession->auth_phrase ==AUTH_PHRASE_ACK) {

		ch_proto_session_store_write(pstore,tsession,(void*)mysql_entry);
	}
	
    if(mpa){

        ch_mpool_agent_free(mpa,mysql_entry->mp);
    }else{
        ch_pool_destroy(mysql_entry->mp);
    }


}

