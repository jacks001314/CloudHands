#include "ch_packet_api_reader.h"
#include "ch_mpool.h"
#include "ch_shm_format.h"
#include "ch_packet_record.h"

#define MAX_PKT_RCD_CONTEXT_N 64

typedef struct ch_packet_record_reader_context_t ch_packet_record_reader_context_t;


struct ch_packet_record_reader_context_t {
	ch_pool_t *mp;
	ch_shm_format_t *shm_fmt;
	ch_shm_entry_iterator_t *eiter;
    packet_entry_t mpentry;
};

static ch_packet_record_reader_context_t pkt_rcd_contexts[MAX_PKT_RCD_CONTEXT_N];

static inline ch_packet_record_reader_context_t * _pkt_rcd_context_get(int id){

	if(id >= MAX_PKT_RCD_CONTEXT_N||id<0)
		return NULL;

	return &pkt_rcd_contexts[id];
}



int  packet_mmap_open(int fid,const char *fpath){

	  ch_packet_record_reader_context_t *pcontext;
	  ch_shm_format_t *shm_fmt;
	  ch_pool_t *mp;
	  const char *open_fname;
	  
	  pcontext = _pkt_rcd_context_get(fid);
	  if(pcontext == NULL)
		  return -1;

	  if(fpath == NULL || strlen(fpath) == 0)
		  return -1;

	  /*create global  memory pool*/
	  mp = ch_pool_create(1024);
	  if(mp == NULL){
		  return -1;
	  }
	  
	  shm_fmt = ch_shm_format_pkt_with_mmap_create(mp,fpath,0,0,0,0);
	  if(shm_fmt == NULL)
		  return -1;


	  pcontext->mp = mp;

	  pcontext->shm_fmt = shm_fmt;

	  pcontext->eiter = NULL;

    
	  return 0;
  
}


int  packet_shm_open(int sid,const char *key,int proj_id){

	  ch_packet_record_reader_context_t *pcontext;
	  ch_shm_format_t *shm_fmt;
	  ch_pool_t *mp;
	  
	  
	  pcontext = _pkt_rcd_context_get(sid);
	  if(pcontext == NULL)
		  return -1;
	  
	  if(key == NULL|| strlen(key) == 0)
		  return -1;

	  /*create global memory pool*/
	  
	  mp = ch_pool_create(1024);
	  if(mp == NULL){
		  return -1;
	  }
	  
	  shm_fmt = ch_shm_format_pkt_with_shm_create(mp,key,proj_id,0,0,0,0);
	  if(shm_fmt == NULL)
		  return -1;
	  
	  pcontext->mp = mp;

	  pcontext->shm_fmt = shm_fmt;

	  pcontext->eiter = NULL;
	  
	  return 0;

}

static inline int _prepare_iter_ok(ch_packet_record_reader_context_t *pcontext){

	if(pcontext == NULL)
		return 0;

    if(pcontext->eiter)
		return 1;

    pcontext->eiter = ch_shm_entry_iterator_prefare(pcontext->shm_fmt);

    if(pcontext->eiter == NULL)
		return 0;
     
    return 1;
}


packet_entry_t *packet_entry_read(int id){

	  ch_packet_record_reader_context_t *pcontext = NULL;
	  ch_shm_format_t *shm_fmt;
	  ch_shm_entry_iterator_t *eiter;

	  ch_packet_record_t *pkt_rcd;
	  ch_shm_record_t *shm_rcd;
	  
	  pcontext = _pkt_rcd_context_get(id);
	  if(pcontext == NULL)
		  return NULL;

     if(_prepare_iter_ok(pcontext) == 0){
		 /* no data to be read */
		 return NULL;
     }

	 shm_fmt = pcontext->shm_fmt;
	 eiter = pcontext->eiter;

     shm_rcd = eiter->next(eiter);

	 if(shm_rcd == NULL){
		 /* read one data chunk completely*/
		 ch_shm_entry_iterator_commit(shm_fmt,eiter);

		 pcontext->eiter = NULL;
		 return NULL;
	 } 

     packet_entry_t *pentry = &pcontext->mpentry;

	 pkt_rcd = (ch_packet_record_t*)shm_rcd;
     /* read ok */
     pentry->ptype = pkt_rcd->type;
     pentry->metaDataSize = pkt_rcd->meta_data_size;
     pentry->time = pkt_rcd->time;
     pentry->dataSize = (uint64_t)shm_rcd->data_len;
     pentry->data = shm_rcd->data;

	return pentry;

}

/*close */
void packet_close(int id){

      ch_packet_record_reader_context_t *pcontext = NULL;

	  pcontext = _pkt_rcd_context_get(id);
	  if(pcontext == NULL)
		  return;

	  if(pcontext->eiter){
	  
		  ch_shm_entry_iterator_commit(pcontext->shm_fmt,pcontext->eiter);
	  }

	  ch_pool_destroy(pcontext->mp);

}
