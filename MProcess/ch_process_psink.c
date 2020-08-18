#include "ch_process_psink.h"
#include "ch_log.h"

ch_process_sink_t * ch_process_sink_create_for_write(ch_pool_t *mp,
        const char *pcap_dir,size_t mempool_size,size_t queue_size){

    ch_process_sink_t *psink = (ch_process_sink_t*)ch_pcalloc(mp,sizeof(*psink));

    psink->pcap_dir = pcap_dir;
    psink->pcap_files = 0;
    psink->wait_times = 0;

    psink->ring = rte_ring_create(PCAP_RING_NAME,queue_size,rte_socket_id(),0);

    if(psink->ring == NULL){

        ch_log(CH_LOG_ERR,"Cannot create ring queue:%s,queue size:%lu",PCAP_RING_NAME,
                (unsigned long)queue_size);

        return NULL;
    }

    psink->pcap_file_pool = rte_mempool_create(PCAP_MEMPOOL_NAME,
            mempool_size,
            sizeof(ch_pcap_file_entry_t),
            0,
            0,
            NULL,NULL,NULL,NULL,rte_socket_id(),
            0);

    if(psink->pcap_file_pool == NULL){

        ch_log(CH_LOG_ERR,"Cannot Create mem pool:%s,size:%lu",PCAP_MEMPOOL_NAME,(unsigned long)mempool_size);
        return NULL;
    }

    return psink;
}

ch_process_sink_t * ch_process_sink_create_for_read(ch_pool_t *mp,const char *pcap_dir){

    ch_process_sink_t *psink = (ch_process_sink_t*)ch_pcalloc(mp,sizeof(*psink));

    psink->pcap_dir = pcap_dir;
    psink->pcap_files = 0;
    psink->wait_times = 0;

    psink->ring = rte_ring_lookup(PCAP_RING_NAME);
    if(psink->ring == NULL){

        ch_log(CH_LOG_ERR,"Cannot find ring:%s",PCAP_RING_NAME);
        return NULL;
    }

    psink->pcap_file_pool = rte_mempool_lookup(PCAP_MEMPOOL_NAME);

    if(psink->pcap_file_pool == NULL){

        ch_log(CH_LOG_ERR,"Cannot find mempool:%s",PCAP_MEMPOOL_NAME);
        return NULL;
    }

    return psink;
}

void ch_process_sink_put(ch_process_sink_t *psink,uint8_t id,uint64_t ts){

    void *entry;
    ch_pcap_file_entry_t *pentry;

    /*Try to get a file entry space*/

    while(1){
        
        if(rte_mempool_get(psink->pcap_file_pool,&entry)<0){
            ch_log(CH_LOG_ERR,"Cannot get a pcap file entry,maybe mempool is full,will try ------");
            psink->wait_times +=1;
            usleep(5);
            continue;
        }

        pentry = (ch_pcap_file_entry_t*)entry;

        pentry->ts = ts;
        pentry->id = id;

        while(1){
            if(rte_ring_enqueue(psink->ring,entry) < 0){
                ch_log(CH_LOG_ERR,"Cannot put pcap file into queue,may be queue is full,will try--------");
                psink->wait_times += 1;
                usleep(5);
                continue;
            }

            break;
        }

        break;
    }

}

const char * ch_process_sink_pop(ch_process_sink_t *psink){

    void *entry;
    ch_pcap_file_entry_t *pentry;
    
    if (rte_ring_dequeue(psink->ring,&entry) < 0){
        return NULL;
    }


    pentry = (ch_pcap_file_entry_t*)entry;
    snprintf(psink->pcap_file_name,PCAP_FILENAME_MAXLEN,"%s/%s_%lu_%lu%s",psink->pcap_dir,PCAP_FILE_PREFIX,
            (unsigned long)pentry->id,
            (unsigned long)pentry->ts,
            PCAP_FILE_EXTNAME);

    rte_mempool_put(psink->pcap_file_pool,entry);

    return (const char*)psink->pcap_file_name;
}


void ch_process_sink_dump(ch_process_sink_t *psink,FILE *fp){

    fprintf(fp,"Dump The Psink information -----------------------------\n");
    fprintf(fp,"psink.pcap.files:%lu\n",(unsigned long)psink->pcap_files);
    fprintf(fp,"psink.pcap.waits:%lu\n",(unsigned long)psink->wait_times);

    rte_ring_dump(fp,psink->ring);
    rte_mempool_dump(fp,psink->pcap_file_pool);
}


