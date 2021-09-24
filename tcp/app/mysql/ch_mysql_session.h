/*
 *
 *      Filename: ch_mysql_session.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-10-30 16:21:52
 * Last Modified: 2018-10-30 16:21:52
 */

#ifndef CH_MYSQL_SESSION_H
#define CH_MYSQL_SESSION_H

typedef struct ch_mysql_packet_t ch_mysql_packet_t;
typedef struct ch_mysql_session_t ch_mysql_session_t;

typedef struct ch_mysql_session_data_t ch_mysql_session_data_t;

#include "ch_mysql_request.h"
#include "ch_mysql_response.h"
#include "ch_list.h"
#include "ch_mpool.h"

#define OK_AUTH 0
#define ERR_AUTH 1

#define AUTH_PHRASE_RES 0
#define AUTH_PHRASE_REQ 1
#define AUTH_PHRASE_ACK 2
 
struct ch_mysql_session_data_t {

    struct list_head node;
    uint8_t cur_seq;
    ch_mysql_request_t *req;
    
    uint32_t res_n;

    struct list_head res_list;

};

struct ch_mysql_session_t {

    uint8_t auth_phrase;
    uint8_t auth_state;
    const char *version;
    const char *user;

    uint32_t entries_n;
    struct list_head entries; 
};

struct ch_mysql_packet_t {

	uint32_t plen;
	uint8_t seq;

};

#define BVMAKE(dt,i) (*((uint8_t*)(dt+i)))

static inline int ch_mysql_packet_parse(ch_mysql_packet_t *mpkt,void *data,size_t dlen){

	if(data == NULL || dlen<4){
	
		return -1;
	}

	mpkt->plen = (uint32_t)(BVMAKE(data,0)|(BVMAKE(data,1)<<8)|(BVMAKE(data,2)<<16));
	mpkt->seq = BVMAKE(data,3);

	return 0;
}

#endif /*CH_MYSQL_SESSION_H*/
