#ifndef CH_PROCESS_PSINK_H
#define CH_PROCESS_PSINK_H

typedef struct ch_process_sink_t ch_process_sink_t;
typedef struct ch_pcap_file_entry_t ch_pcap_file_entry_t;

#include <rte_mempool.h>
#include <rte_ring.h>

#include "ch_mpool.h"

#define PCAP_FILENAME_MAXLEN 1024
#define PCAP_MEMPOOL_NAME "psink_mempool"
#define PCAP_RING_NAME "psink_ring"
#define PCAP_FILE_PREFIX "cap"
#define PCAP_FILE_EXTNAME ".pcap"

struct ch_process_sink_t {


    const char *pcap_dir;

    uint64_t pcap_files;
    uint64_t wait_times;

    struct rte_ring *ring;

    struct rte_mempool *pcap_file_pool;

    char pcap_file_name[PCAP_FILENAME_MAXLEN];
};

struct ch_pcap_file_entry_t {

    uint8_t id;
    uint64_t ts;
};

extern ch_process_sink_t * ch_process_sink_create_for_write(ch_pool_t *mp,
        const char *pcap_dir,size_t mempool_size,size_t queue_size);

extern ch_process_sink_t * ch_process_sink_create_for_read(ch_pool_t *mp,const char *pcap_dir);

extern void ch_process_sink_put(ch_process_sink_t *psink,uint8_t id,uint64_t ts);

extern const char * ch_process_sink_pop(ch_process_sink_t *psink);

extern void ch_process_sink_dump(ch_process_sink_t *psink,FILE *fp);


#endif /*CH_PROCESS_PSINK_H*/
