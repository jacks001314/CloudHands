#include "rte_pcap_file_pool.h"

int main(int argc,char **argv){

    struct pcap_pkthdr header,*pkt_hdr = &header;
    const char *dir = argv[1];

    struct rte_pcap_file_pool tmp,*fpool = &tmp;

    rte_pcap_file_pool_init(fpool,dir);

    void *packet = rte_pcap_file_pool_read(fpool,pkt_hdr);

    rte_pcap_file_pool_dump(fpool,stdout);

    rte_pcap_file_pool_fin(fpool);

    return 0;
}
