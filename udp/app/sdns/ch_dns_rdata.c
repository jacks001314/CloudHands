#include "ch_dns_rdata.h"

ch_dns_rdata_t * ch_dns_rdata_parse(ch_pool_t *mp,ch_dns_data_input_t *din){

	uint16_t dclass,type,dlen;
    uint32_t ttl;
    void *data;
    
    ch_dns_name_t tmp_name;

	ch_dns_rdata_t *rdata = NULL;

	if(ch_dns_data_input_rdlen(din)<10){
	
		ch_log(CH_LOG_ERR,"Invalid DNS Rdata!");
		return NULL;
	}

	if(ch_dns_name_parse(mp,din,&tmp_name))
	{
		ch_log(CH_LOG_ERR,"Parse rdata.name failed!");
		return NULL;
	}

	type = ch_dns_data_input_uint16_read(din);
	dclass = ch_dns_data_input_uint16_read(din);
    ttl = ch_dns_data_input_uint32_read(din); 
    dlen = ch_dns_data_input_uint16_read(din);


	if(dlen>ch_dns_data_input_rdlen(din)){
	
		ch_log(CH_LOG_ERR,"Invalid rdata len:%d!",(int)dlen);
		return NULL;

	}

    rdata = (ch_dns_rdata_t*)ch_pcalloc(mp,sizeof(*rdata));
	ch_dns_name_clone(mp,&rdata->name,&tmp_name);

	rdata->type = type;
	rdata->dclass = dclass;

	rdata->ttl = ttl;
	rdata->dlen = dlen;

    data = rdata->dlen>0?ch_dns_data_input_pos(din):NULL;
    
    if(rdata->dlen>0&&data){

		/*copy rdata data*/
		void *addr = ch_pcalloc(mp,rdata->dlen+1);
		memcpy(addr,data,rdata->dlen);
		data = addr;
	}

    rdata->data = data;
    rdata->pos = din->pos>din->base?(uint16_t)(din->pos-din->base-1):0;

	ch_dns_data_input_pos_update(din,rdata->dlen);
	return rdata;
}

void ch_dns_rdata_dump(ch_dns_rdata_t *rdata,FILE *fp){

	fprintf(fp,"\n\n");
	ch_dns_name_dump(&rdata->name,fp);
	fprintf(fp,"rdata.type:%u\n",rdata->type);
	fprintf(fp,"rdata.class:%u\n",rdata->dclass);
	fprintf(fp,"rdata.ttl:%lu\n",(unsigned long)rdata->ttl);
	fprintf(fp,"rdata.dlen:%u\n",rdata->dlen);
	fprintf(fp,"rdata.data:\n");
	
    if(rdata->data == NULL||rdata->dlen==0)
    {
        fprintf(fp,"null\n");
    }else{
        unsigned char *encode = (unsigned char*)ch_bytes2hex_malloc(rdata->data,rdata->dlen); 
        fprintf(fp,"%s\n",encode);
        free(encode);
    }

}

void ch_dns_rdata_store(ch_dns_rdata_t *rdata,ch_msgpack_store_t *dstore){

    ch_msgpack_store_map_start(dstore,"rdata",6);

    ch_dns_name_store(&rdata->name,dstore);
    ch_msgpack_store_write_uint16(dstore,"type",rdata->type);
    ch_msgpack_store_write_uint16(dstore,"dclass",rdata->dclass);
    ch_msgpack_store_write_uint32(dstore,"ttl",rdata->ttl);
    ch_msgpack_store_write_uint16(dstore,"pos",rdata->pos);
    ch_msgpack_store_write_bin_kv(dstore,"rdata",rdata->data,rdata->dlen);

}

ssize_t ch_dns_rdata_write(ch_dns_rdata_t *rdata,ch_data_output_t *dout){

	ssize_t len = 0,rc;
	
	CH_DNS_NAME_WRITE(dout,&rdata->name,len,rc);
	CH_DOUT_UINT16_WRITE(dout,rdata->type,len,rc);
	CH_DOUT_UINT16_WRITE(dout,rdata->dclass,len,rc);
	CH_DOUT_UINT32_WRITE(dout,rdata->ttl,len,rc);
	CH_DOUT_UINT16_WRITE(dout,rdata->pos,len,rc);

    CH_DOUT_STRING16_WRITE(dout,(unsigned char*)rdata->data,rdata->dlen,len,rc);

	return len;
}
