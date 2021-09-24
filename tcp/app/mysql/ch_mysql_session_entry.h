/*
 *
 *      Filename: ch_mysql_session_entry.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2021-8-31 9:47:52
 * Last Modified: 2021-8-31 9:47:52
 */

#ifndef CH_MYSQL_SESSION_ENTRY_H
#define CH_MYSQL_SESSION_ENTRY_H

typedef struct ch_mysql_session_entry_t ch_mysql_session_entry_t;

#include "ch_mpool.h"
#include "ch_pp_data_input.h"
#include "ch_proto_session_entry.h"
#include "ch_mysql_session.h"

struct ch_mysql_session_entry_t {


    ch_proto_session_entry_t psEntry;

    ch_pool_t *mp;

    ch_pp_data_input_t data_input_cli;
    ch_pp_data_input_t data_input_srv;

    ch_mysql_session_t *msession;

};


extern void ch_mysql_session_entry_init(ch_mysql_session_entry_t *mysql_entry,ch_pool_t *mp,ch_buffer_t *g_buffer);


#endif /*CH_MYSQL_SESSION_ENTRY_H*/

