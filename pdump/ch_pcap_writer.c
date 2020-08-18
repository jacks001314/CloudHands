#include "ch_pcap_writer.h"
#include "ch_log.h"

#define ETH_PCAP_SNAPSHOT_LEN 65535

 static int
 open_pcap_dump(ch_pcap_writer_t *pwriter,uint64_t ts)
 {

    char pcap_filename[MAX_FNAME_SIZE];

    pcap_t *pcap;
    snprintf(pcap_filename,MAX_FNAME_SIZE,"%s/cap_%lu_%lu.tmp",pwriter->pcap_dir,(unsigned long)pwriter->task_id,(unsigned long)ts);
     /*
      * We need to create a dummy empty pcap_t to use it
      * with pcap_dump_open(). We create big enough an Ethernet
      * pcap holder.
      */
     pcap = pcap_open_dead(DLT_EN10MB, ETH_PCAP_SNAPSHOT_LEN);
     if (pcap == NULL) {
         ch_log(CH_LOG_ERR, "Couldn't create dead pcap:%s",pcap_filename);
         return -1;
     }
 
     /* The dumper is created using the previous pcap_t reference */
     pwriter->pdump = pcap_dump_open(pcap, pcap_filename);
     if (pwriter->pdump == NULL) {
         pcap_close(pcap);
         ch_log(CH_LOG_ERR,"Couldn't open %s for writing.",pcap_filename);
         return -1;
     }

    pwriter->ts = ts;

     pcap_close(pcap);
     return 0;
 }

static void pcap_file_rename(ch_pcap_writer_t *pwriter){

    char tmpfile[MAX_FNAME_SIZE];
    char pcapfile[MAX_FNAME_SIZE];

    snprintf(tmpfile,MAX_FNAME_SIZE,"%s/cap_%lu_%lu.tmp",pwriter->pcap_dir,(unsigned long)pwriter->task_id,(unsigned long)pwriter->ts);
    snprintf(pcapfile,MAX_FNAME_SIZE,"%s/cap_%lu_%lu.pcap",pwriter->pcap_dir,(unsigned long)pwriter->task_id,(unsigned long)pwriter->ts);

    rename(tmpfile,pcapfile);
}

static pcap_dumper_t * get_pdumper(ch_process_sink_t *psink,ch_pcap_writer_t *pwriter,size_t psize,uint64_t ts){


    if(pwriter->pdump == NULL||pwriter->w_size+psize>pwriter->loop_bytes){

        /*create new pcap file to store */
        if(pwriter->pdump!=NULL){
                   
            pcap_dump_close(pwriter->pdump);
            pwriter->pdump= NULL;
            /*rename pcap file*/
            pcap_file_rename(pwriter);

            if(psink){

                ch_process_sink_put(psink,(uint8_t)pwriter->task_id,pwriter->ts);
            }
        }

        pwriter->w_size = 0;

        if(open_pcap_dump(pwriter,ts))
            return NULL;
    }

    return pwriter->pdump;
}

ch_pcap_writer_t * ch_pcap_writer_create(ch_pool_t *mp,const char *pcap_dir,size_t loop_bytes,uint32_t task_id){


    ch_pcap_writer_t *pwriter = NULL;

    pwriter = (ch_pcap_writer_t*)ch_pcalloc(mp,sizeof(*pwriter));

    pwriter->pdump = NULL;
    pwriter->w_size = 0;
    pwriter->loop_bytes = loop_bytes;
    pwriter->pcap_dir = pcap_dir;
    pwriter->ts = 0;

    pwriter->task_id = task_id;

    return pwriter;
}

int ch_pcap_writer_put(ch_process_sink_t *psink,ch_pcap_writer_t *pwriter,void *data,size_t dsize){

    struct pcap_pkthdr header,*phdr=&header;
    pcap_dumper_t *pdump;
    gettimeofday(&phdr->ts,NULL);
    struct timeval *tv = &phdr->ts;

    uint64_t ts = tv->tv_sec * 1000 + tv->tv_usec / 1000;

    pdump = get_pdumper(psink,pwriter,dsize,ts);

    if(pdump == NULL)
        return -1;

    phdr->caplen = dsize;
    phdr->len = dsize; 

    pwriter->w_size+=dsize;

    pcap_dump((unsigned char *)pdump,phdr,data);

    return 0;
}

