/*
 * =====================================================================================
 *
 *       Filename:  do_ssh_create.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/17/2018 02:07:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jacksha (shajf), csp001314@163.com
 *   Organization:  
 *
 * =====================================================================================
 */


static void * do_ssh_session_entry_create(ch_mpool_agent_t *mpa,ch_tcp_app_t *app ch_unused,ch_proto_session_store_t *pstore ch_unused){

	ch_pool_t *mp;

	ch_ssh_session_entry_t *ssh_entry = NULL;

    if(mpa){

        mp = ch_mpool_agent_alloc(mpa);
    }else{
        mp = ch_pool_create(1024);
    }

	if(mp == NULL){
	
		ch_log(CH_LOG_ERR,"Cannot create memory pool for ssh session entry!");
		return NULL;
	}

	ssh_entry = (ch_ssh_session_entry_t*)ch_pcalloc(mp,sizeof(*ssh_entry));

	ch_ssh_session_entry_init(ssh_entry,mp,&pstore->g_buffer);


	return (void*)ssh_entry;
}


#define SSH_ENTRY_CAN_STORE(entry) ((entry)->srv_data.state!=SSH_STATE_INIT||(entry)->cli_data.state!=SSH_STATE_INIT)

static void do_ssh_session_entry_clean(ch_mpool_agent_t *mpa,ch_tcp_app_t *app ch_unused,ch_proto_session_store_t *pstore,ch_tcp_session_t *tsession){

	ch_ssh_session_entry_t *ssh_entry = (ch_ssh_session_entry_t*)tsession->sentry;

	if (SSH_ENTRY_CAN_STORE(ssh_entry)) {

		ch_proto_session_store_write(pstore,tsession,(void*)ssh_entry);
	}
	
    if(mpa){

        ch_mpool_agent_free(mpa,ssh_entry->mp);
    }else{
        ch_pool_destroy(ssh_entry->mp);
    }


}

