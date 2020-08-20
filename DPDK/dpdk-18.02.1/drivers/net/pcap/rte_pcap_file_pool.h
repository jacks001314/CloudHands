#ifndef _RTE_PCAP_FILE_H_
#define _RTE_PCAP_FILE_H_

#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <stdint.h>

#define PCAP_FILE_POOL_SIZE 100
#define PCAP_FILE_PREFIX "cap"
#define PCAP_FILE_EXTNAME "pcap"
#define PCAP_FILE_NAME_LEN 1024

struct rte_pcap_file {

    uint8_t id;
    uint64_t ts;
};

struct rte_pcap_file_pool {

    pcap_t *pcap; /*current pcap to read*/

    struct rte_pcap_file *fentry;

    const char *dir; //pcap file store root dir
    
    uint16_t pos;
    uint16_t num;

    struct rte_pcap_file files[PCAP_FILE_POOL_SIZE];

};

void rte_pcap_file_pool_init(struct rte_pcap_file_pool *fpool,const char *dir);

const u_char * rte_pcap_file_pool_read(struct rte_pcap_file_pool *fpool,struct pcap_pkthdr *pkt_hdr);

void rte_pcap_file_pool_fin(struct rte_pcap_file_pool *fpool);

void rte_pcap_file_pool_dump(struct rte_pcap_file_pool *fpool,FILE *out);

#endif /*_RTE_PCAP_FILE_H_*/
