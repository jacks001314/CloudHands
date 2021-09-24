/*
 * =====================================================================================
 *
 *       Filename:  do_mysql_format.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  8/31/2021 11:03:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jacksha (shajf), csp001314@163.com
 *   Organization:  
 *
 * =====================================================================================
 */


static void _mysql_session_req_format(msgpack_packer *pk,ch_mysql_request_t *req){

	ch_msgpack_map_start(pk,"req",3);
    ch_msgpack_write_uint8(pk,"opcode",req->opcode);
    ch_msgpack_write_kv(pk,"opname",ch_mysql_request_opname_get(req->opcode));
    if(req->dlen>0&&req->data){
         ch_msgpack_write_bin_kv(pk,"data",req->data,req->dlen);
    }else{
        ch_msgpack_write_bin_kv(pk,"data",(void*)"NULL",4);
    }
}

static void _mysql_session_res_format(msgpack_packer *pk,ch_mysql_session_data_t *sdata){


    ch_msgpack_array_start(pk,"ResArr",sdata->res_n);

    ch_mysql_response_t *res;

    list_for_each_entry(res,&sdata->res_list,node){

        ch_msgpack_map_start(pk,"res",4);

        ch_msgpack_write_uint8(pk,"code",res->code);
        ch_msgpack_write_uint8(pk,"seq",res->seq);
        ch_msgpack_write_kv(pk,"cname",ch_mysql_response_cname_get(res->code));
        if(res->dlen>0&&res->data){
            ch_msgpack_write_bin_kv(pk,"data",res->data,res->dlen);
        }else{
            ch_msgpack_write_bin_kv(pk,"data",(void*)"NULL",4);
        }
    }
}

static void _mysql_session_data_format(msgpack_packer *pk,ch_mysql_session_data_t *sdata){

    if(sdata->res_n>0){
        ch_msgpack_map_start(pk,"entry",2);
        _mysql_session_req_format(pk,sdata->req);
        _mysql_session_res_format(pk,sdata);
    }else{
        ch_msgpack_map_start(pk,"entry",1);
        _mysql_session_req_format(pk,sdata->req);
    }
}

static void do_mysql_session_format(msgpack_packer *pk,void *session_in){

    ch_mysql_session_data_t *sdata;

	ch_mysql_session_entry_t * mysql_entry = (ch_mysql_session_entry_t*)session_in;
    ch_mysql_session_t *session = mysql_entry->msession;

	ch_msgpack_map_start(pk,"mysql",4);
    ch_msgpack_write_uint8(pk,"auth_state",session->auth_state); 
	ch_msgpack_write_kv(pk,"version",session->version);
	ch_msgpack_write_kv(pk,"user",session->user);

    ch_msgpack_array_start(pk,"entries",session->entries_n);

    if(session->entries_n){

        list_for_each_entry(sdata,&session->entries,node){

            _mysql_session_data_format(pk,sdata);
        }
    }
}

