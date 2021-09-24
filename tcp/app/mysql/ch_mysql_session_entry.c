
#include "ch_mysql_session_entry.h"

void ch_mysql_session_entry_init(ch_mysql_session_entry_t *mysql_entry,ch_pool_t *mp,ch_buffer_t *g_buffer){


	ch_proto_session_entry_init(&mysql_entry->psEntry);
    mysql_entry->mp = mp;


	ch_pp_din_init(&mysql_entry->data_input_cli,g_buffer,mp); 
	ch_pp_din_init(&mysql_entry->data_input_srv,g_buffer,mp); 

    mysql_entry->msession = NULL;

}
