#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int main(int argc,char **argv){

    struct pcap_pkthdr packet;
    pcap_t *pcap;
    int count = 0;
    const char *pcap_file;
    char errbuf[PCAP_ERRBUF_SIZE]={0};
    unsigned long max_len = 0;
    int is_v = 0;

    if(argc<2){

        printf("Usage:<pcapFile>[is_v]\n");
        return -1;
    }

    pcap_file = argv[1];

    if(argc==3)
        is_v = 1;

    if( NULL==(pcap=pcap_open_offline(pcap_file, errbuf)))
    {
            printf("pcap_open_offline() return NULL.\nerrbuf:%s\n", errbuf);
            return -1;
                                
    }

    while(1){

        if(NULL==pcap_next(pcap, &packet))
            break;

        count++;

        if(max_len<packet.caplen)
            max_len = packet.caplen;

        if(is_v)
            printf("pkt info:{pkt_len:%lu,len:%lu}\n",packet.caplen,packet.len);
    }

    printf("pkt.count:%d,maxLen:%lu\n",count,max_len);

    pcap_close(pcap);
    return 0;
}

