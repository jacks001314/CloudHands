/*
 *
 *      Filename: ch_mysql_response.h
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-10-31 12:05:05
 * Last Modified: 2018-10-31 12:05:05
 */

#ifndef CH_MYSQL_RESPONSE_H
#define CH_MYSQL_RESPONSE_H

#define OK_CODE 0
#define ERR_CODE 255
#define EOF_CODE 254

typedef struct ch_mysql_response_t ch_mysql_response_t;

#include "ch_mpool.h"
#include "ch_list.h"

struct ch_mysql_response_t {

    struct list_head node;

	uint8_t code;
    uint8_t seq;
    uint32_t dlen;
    void *data;
};

static inline const char * ch_mysql_response_cname_get(uint8_t code){

    if(code == OK_CODE)
        return "ok";
    
    if(code == ERR_CODE)
        return "error";

    if(code == EOF_CODE)
        return "eof";

    
    return "result";
} 

extern ch_mysql_response_t *ch_mysql_response_parse(ch_pool_t *mp,void *data,size_t dlen,uint8_t seq);

#endif /*CH_MYSQL_RESPONSE_H*/
