#ifndef CH_PCAP_H
#define CH_PCAP_H

typedef struct ch_pcap_writer_t ch_pcap_writer_t;

#include <pcap.h>
#include "ch_mpool.h"

#define MAX_FNAME_SIZE 256

struct ch_pcap_writer_t {

    pcap_dumper_t *pdump;

    size_t w_size;

    size_t loop_bytes;

    const char *pcap_dir;

    uint64_t ts;
    
};

extern ch_pcap_writer_t * ch_pcap_writer_create(ch_pool_t *mp,const char *pcap_dir,size_t loop_bytes);

extern int ch_pcap_writer_put(ch_pcap_writer_t *pwriter,void *data,size_t dsize);

#endif /*CH_PCAP_H*/
