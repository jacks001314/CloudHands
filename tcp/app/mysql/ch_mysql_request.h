/*
 *
 *      Filename: ch_mysql_request.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-10-31 12:03:36
 * Last Modified: 2018-10-31 12:03:36
 */

#ifndef CH_MYSQL_REQUEST_H
#define CH_MYSQL_REQUEST_H

typedef struct ch_mysql_request_t ch_mysql_request_t;

#include "ch_mpool.h"

/* MySQL command codes */
#define MYSQL_SLEEP               0  /* not from client */
#define MYSQL_QUIT                1
#define MYSQL_INIT_DB             2
#define MYSQL_QUERY               3
#define MYSQL_FIELD_LIST          4
#define MYSQL_CREATE_DB           5
#define MYSQL_DROP_DB             6
#define MYSQL_REFRESH             7
#define MYSQL_SHUTDOWN            8
#define MYSQL_STATISTICS          9
#define MYSQL_PROCESS_INFO        10
#define MYSQL_CONNECT             11 /* not from client */
#define MYSQL_PROCESS_KILL        12
#define MYSQL_DEBUG               13
#define MYSQL_PING                14
#define MYSQL_TIME                15 /* not from client */
#define MYSQL_DELAY_INSERT        16 /* not from client */
#define MYSQL_CHANGE_USER         17
#define MYSQL_BINLOG_DUMP         18 /* replication */
#define MYSQL_TABLE_DUMP          19 /* replication */
#define MYSQL_CONNECT_OUT         20 /* replication */
#define MYSQL_REGISTER_SLAVE      21 /* replication */
#define MYSQL_STMT_PREPARE        22
#define MYSQL_STMT_EXECUTE        23
#define MYSQL_STMT_SEND_LONG_DATA 24
#define MYSQL_STMT_CLOSE          25
#define MYSQL_STMT_RESET          26
#define MYSQL_SET_OPTION          27
#define MYSQL_STMT_FETCH          28
#define MYSQL_DAEMON              29
#define MYSQL_BINLOG_DUMP_GTID    30
#define MYSQL_RESET_CONNECTION    31
#define MYSQL_CLONE               32

struct ch_mysql_request_t {

	uint8_t opcode;
    uint32_t dlen;
    void *data;
};

static inline const char * ch_mysql_request_opname_get(uint8_t opcode){

    const char *opname = "unknown";
    if(opcode>32)
        return opname;

    switch(opcode){

        case MYSQL_SLEEP:
            opname = "sleep";
            break;

        case MYSQL_QUIT:
            opname = "quit";
            break;

        case MYSQL_INIT_DB:
            opname = "use";
            break;

        case MYSQL_QUERY:
            opname = "query";
            break;

        case MYSQL_FIELD_LIST:
            opname = "show_column";
            break;

        case MYSQL_CREATE_DB:
            opname = "create_database";
            break;

        case MYSQL_DROP_DB:
            opname = "drop_database";
            break;

        case MYSQL_REFRESH:
            opname = "refresh";
            break;

        case MYSQL_SHUTDOWN:
            opname = "shutdown";
            break;

        case MYSQL_STATISTICS:

            opname = "statistics";
            break;

        case MYSQL_PROCESS_INFO:
            opname = "process_info";
            break;

        case MYSQL_CONNECT:
            opname = "connect";
            break;

        case MYSQL_PROCESS_KILL:
            opname = "process_kill";
            break;

        case MYSQL_DEBUG:
            opname = "debug";
            break;

        case MYSQL_PING:
            opname = "ping";
            break;

        case MYSQL_TIME:
            opname = "sleep";
            break;

        case MYSQL_DELAY_INSERT:
            opname = "delay_insert";
            break;

        case MYSQL_CHANGE_USER:
            opname = "change_user";
            break;

        case MYSQL_BINLOG_DUMP: 
            opname = "binlog_dump";
            break;

        case MYSQL_TABLE_DUMP:
            opname = "table_dump";
            break;

        case MYSQL_CONNECT_OUT:
            opname = "connect_out";
            break;

        case MYSQL_REGISTER_SLAVE:
            opname = "register_slave";
            break;

        case MYSQL_STMT_PREPARE:
            opname = "stmt_prepare";
            break;

        case MYSQL_STMT_EXECUTE:
            opname = "stmt_execute";
            break;

        case MYSQL_STMT_SEND_LONG_DATA:
            opname = "stmt_send_long_data";
            break;

        case MYSQL_STMT_CLOSE:
            opname = "stmt_close";
            break;

        case MYSQL_STMT_RESET:
            opname = "stmt_reset";
            break;

        case MYSQL_SET_OPTION:
            opname = "set_option";
            break;

        case MYSQL_STMT_FETCH:
            opname = "stmt_fetch";
            break;

        case MYSQL_DAEMON:
            opname = "daemon";
            break;

        case MYSQL_BINLOG_DUMP_GTID:
            opname = "binlog_dump_gtid";
            break;

        case MYSQL_RESET_CONNECTION:
            opname = "reset_connection";
            break;

        case MYSQL_CLONE:
            opname = "clone";
            break;

        default:
            break;
    }

    return opname;
}

extern ch_mysql_request_t *ch_mysql_request_parse(ch_pool_t *mp,void *data,size_t dlen);

#endif /*CH_MYSQL_REQUEST_H*/
