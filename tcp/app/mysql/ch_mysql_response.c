
#include "ch_mysql_response.h"

ch_mysql_response_t *ch_mysql_response_parse(ch_pool_t *mp,void *data,size_t dlen,uint8_t seq){

    ch_mysql_response_t *mres = (ch_mysql_response_t*)ch_pcalloc(mp,sizeof(*mres));

    mres->code = *(uint8_t*)data;
    mres->dlen = dlen-1;
    mres->data = ch_pcalloc(mp,mres->dlen);
    memcpy(mres->data,data+1,mres->dlen);

    mres->seq = seq;

    return mres;

}
