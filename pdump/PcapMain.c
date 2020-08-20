
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <pcap.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define MAX_FILE_NAME 1024
static char errbuf[PCAP_ERRBUF_SIZE];

static int is_endsWith(const char *target,const char *match){
 
     size_t match_length;
     size_t target_length;
 
     if(target == NULL||match == NULL)
         return 0;
 
     match_length = strlen(match);
     target_length = strlen(target);
 
     if(match_length == 0||target_length == 0)
         return 0;
 
     /* This is impossible to match */
     if (match_length > target_length) {
         /* No match. */
         return 0;
     }
 
     if (memcmp(match, (target + (target_length - match_length)), match_length) == 0) {
         return 1;
     }
 
     /* No match. */
     return 0;
 
 }


static int is_valid_pcap_fname(const char *root_dir,const char *name){
    
    return (is_endsWith(name,".pcap")||is_endsWith(name,".pcapng"))&&((strlen(root_dir)+strlen(name)+1)<PATH_MAX); 
}

uint64_t _get_current_timems(void)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static uint64_t get_timestamp(const char *fname){

    const u_char *packet;
    struct pcap_pkthdr tmp,*pkt_hdr = &tmp;
    struct timeval *tv;

    pcap_t *pcap = NULL;

    pcap = pcap_open_offline(fname, errbuf);

    if(pcap == NULL)
        return 0;

    packet = pcap_next(pcap, pkt_hdr);
    if(packet == NULL)
        return 0;

    tv = &pkt_hdr->ts;

    pcap_close(pcap);

    return tv->tv_sec * 1000 + tv->tv_usec / 1000;
} 

static void pcap_file_rename(const char *root_dir,const char *name){

    char fname[MAX_FILE_NAME];
    char nfname[MAX_FILE_NAME];

    uint64_t ts;

    snprintf(fname,MAX_FILE_NAME,"%s/%s",root_dir,name);
    ts = get_timestamp(fname);
    if(ts){
        
        snprintf(nfname,MAX_FILE_NAME,"%s/cap_1_%llu.pcap",root_dir,(unsigned long long)ts);
        
        if(strcmp(fname,nfname)){
            rename(fname,nfname);
            printf("Rename pcap file from:%s to:%s\n",fname,nfname);
        }
    }
}

static size_t format_pcap_files(const char *root_dir){

    size_t n = 0;

	DIR *dir;
	struct dirent *next;

    dir = opendir(root_dir);
    if(!dir)
        return 0;

	while ((next = readdir(dir)) != NULL) {

        if(is_valid_pcap_fname(root_dir,next->d_name)){
            pcap_file_rename(root_dir,next->d_name);
            n++;
        }
	}
	
    closedir(dir);

    return n;

}

int main(int argc,char **argv){

    if(argc<2){
        printf("Usage:<pcap root dir>\n");
        return -1;
    }

    format_pcap_files(argv[1]);

    return 0;    
}

