#include "rte_pcap_file_pool.h"
#include <stdlib.h>
#include <string.h>

static char errbuf[PCAP_ERRBUF_SIZE];

static int make_pcap_file_entry(struct rte_pcap_file *fentry,const char *fname){

    char *endptr;

    /*cap_{id}_{ts}.pcap*/

    fentry->id = strtoull(fname+4,&endptr,10);
    if(endptr == NULL)
        return -1;

    fentry->ts = strtoull(endptr+1,&endptr,10);

    return endptr == NULL||strcmp(endptr,".pcap")?-1:0;
}

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
    
    return (is_endsWith(name,".pcap"))&&((strlen(root_dir)+strlen(name)+1)<PCAP_FILE_NAME_LEN); 
}


static void insert_sort(struct rte_pcap_file_pool *fpool,struct rte_pcap_file *fentry){

    int i;

    struct rte_pcap_file *pfile,*next_pfile;
    if(fpool->num == PCAP_FILE_POOL_SIZE){
     
        pfile = &fpool->files[fpool->num-1];

        if(pfile->ts<=fentry->ts)
            return; 
    }

    /*need insert a new file entry*/
    if(fpool->num == 0){
        pfile = &fpool->files[0];
        pfile->id = fentry->id;
        pfile->ts = fentry->ts;
        fpool->num+=1;
        return;
    }

    i = (int)(fpool->num-1);

    while(i>=0){


        pfile = &fpool->files[i];
        next_pfile = (i==(PCAP_FILE_POOL_SIZE-1))?pfile:&fpool->files[i+1];

        if(pfile->ts<=fentry->ts){

            /*insert*/
            next_pfile->id = fentry->id;
            next_pfile->ts = fentry->ts;
            break;
        }
        /*move*/
        next_pfile->id = pfile->id;
        next_pfile->ts = pfile->ts;
        i = i-1;
    }

    if(i<0){

        pfile = &fpool->files[0];
        pfile->id = fentry->id;
        pfile->ts = fentry->ts;
    }

    if(fpool->num<PCAP_FILE_POOL_SIZE)
        fpool->num += 1;

}

static uint16_t load_some_pcap_files(struct rte_pcap_file_pool *fpool){

    struct rte_pcap_file tmp,*fentry = &tmp;
    const char *root_dir;
	DIR *dir;
	struct dirent *next;

    root_dir = fpool->dir;

    fpool->pcap = NULL;
    fpool->pos = 0;
    fpool->num = 0;

    dir = opendir(root_dir);
    if(!dir)
        return 0;

	while ((next = readdir(dir)) != NULL) {

        if(is_valid_pcap_fname(root_dir,next->d_name)){

            if(make_pcap_file_entry(fentry,next->d_name)==0){

                insert_sort(fpool,fentry);
            }
        }

	}
	
    closedir(dir);

    return fpool->num;
}


void rte_pcap_file_pool_init(struct rte_pcap_file_pool *fpool,const char *dir){

    fpool->dir = dir;
    fpool->pcap = NULL;

    fpool->pos = 0;
    fpool->num = 0;

    fpool->fentry = NULL;
}

static pcap_t *
open_pcap_file(const char *root_dir,struct rte_pcap_file *fentry)
{

    pcap_t *pcap = NULL;

    char fname[PCAP_FILE_NAME_LEN];

    snprintf(fname,PCAP_FILE_NAME_LEN,"%s/%s_%lu_%llu.%s",
            root_dir,PCAP_FILE_PREFIX,fentry->id,fentry->ts,PCAP_FILE_EXTNAME);
    
    pcap = pcap_open_offline(fname, errbuf);

    if(pcap == NULL){

        /*error pcap file,remove it*/
        unlink(fname);
    }

    return pcap;
}

static pcap_t * _open_pcap(struct rte_pcap_file_pool *fpool){

    struct rte_pcap_file *fentry;

    pcap_t *pcap = NULL;


    if(fpool->pos>=fpool->num){

        if(load_some_pcap_files(fpool)==0)
            return NULL;
    }

    while(fpool->pos<fpool->num){

        fentry = &fpool->files[fpool->pos];
        fpool->pos++;

        pcap = open_pcap_file(fpool->dir,fentry);

        if(pcap)
        {
            /*ok*/
            fpool->pcap = pcap;
            fpool->fentry = fentry;

            return pcap;
        }
    }

    return NULL;
}

static void _close_pcap(struct rte_pcap_file_pool *fpool){

    struct rte_pcap_file *fentry = fpool->fentry;

    char fname[PCAP_FILE_NAME_LEN];
    snprintf(fname,PCAP_FILE_NAME_LEN,"%s/%s_%lu_%llu.%s",
            fpool->dir,PCAP_FILE_PREFIX,fentry->id,fentry->ts,PCAP_FILE_EXTNAME);

	pcap_close(fpool->pcap);
	fpool->pcap = NULL;
    fpool->fentry = NULL;

    unlink(fname);
}

const u_char * rte_pcap_file_pool_read(struct rte_pcap_file_pool *fpool,struct pcap_pkthdr *pkt_hdr){

    const u_char *packet = NULL;
    pcap_t *pcap;

    if(fpool->pcap)
        pcap = fpool->pcap;
    else
        pcap = _open_pcap(fpool);

    /*no pcap to read*/
    if(pcap == NULL)
        return NULL;

    packet = pcap_next(pcap, pkt_hdr);

    if(packet == NULL){
        /*this pcap file read over,close it and remove it*/

        _close_pcap(fpool);
    }

    return packet;
}

void rte_pcap_file_pool_fin(struct rte_pcap_file_pool *fpool){

    if(fpool->pcap)
        _close_pcap(fpool);

    fpool->pcap = NULL;
}

void rte_pcap_file_pool_dump(struct rte_pcap_file_pool *fpool,FILE *out){

    struct rte_pcap_file *fentry;
    fprintf(out,"fpool.dir:%s\n",fpool->dir);
    fprintf(out,"fpool.pos:%lu\n",(unsigned long)fpool->pos);
    fprintf(out,"fpool.num:%lu\n",(unsigned long)fpool->num);

    int i = 0;

    for(;i<fpool->num;i++){

        fentry = &fpool->files[i];

        fprintf(out,"FName:%s/%s_%lu_%llu.%s\n",fpool->dir,PCAP_FILE_PREFIX,
                (unsigned long)fentry->id,
                (unsigned long long)fentry->ts,PCAP_FILE_EXTNAME);
    }
}
