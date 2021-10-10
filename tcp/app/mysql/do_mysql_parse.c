
 static inline int _contains_str(const char *target,size_t target_length,const char *match){
 
     size_t match_length;
     size_t i, i_max;
 
     if(target == NULL||match == NULL)
         return 0;
 
     match_length = strlen(match);
 
     if(match_length == 0||target_length == 0)
         return 0;
 
     /* This is impossible to match */
     if (match_length > target_length) {
         /* No match. */
         return 0;
     }
 
     /* scan for first character, then compare from there until we
      * have a match or there is no room left in the target
      */
     i_max = target_length - match_length;
     for (i = 0; i <= i_max; i++) {
         /* First character matched - avoid func call */
         if (target[i] == match[0]) {
             /* See if remaining matches */
             if (   (match_length == 1)
                     || (memcmp((match + 1), (target + i + 1), (match_length - 1)) == 0))
             {
                 /* Match. */
                 return 1;
             }
         }
     }
 
     /* No match. */
     return 0;
 }

static inline int _is_mysql(void *data,size_t dlen){
	
	ch_mysql_packet_t mpkt;

	if(data == NULL||dlen <10)
		return 0;

	ch_mysql_packet_parse(&mpkt,data,dlen);
	if(mpkt.seq!=0)
		return 0;

	return _contains_str((const char*)data,dlen,"mysql");			
}

static const char * _read_str(ch_pool_t *mp,void *data,size_t dlen,size_t off){

	void *result;
	int has_null =0;

	size_t len = 0,i=0;
	const char *str = (const char*)(data+off);
	if(off>=dlen)
		return "";
	
	for(i=0;i<dlen-off;i++){
		if(str[i]=='\0'){
			has_null = 1;
			break;
		}
		len++;
	}

	if(!has_null)
		return "";

	result = ch_pcalloc(mp,len+1);
	memcpy(result,data+off,len);
	return (const char*)result;
}

static int _is_valid_mysql_version(const char *version){

	size_t i;
	size_t len = strlen(version);
	char c;

	if(len==0)
		return 0;

	for(i=0;i<len;i++){
		c = version[i];

		if(!((c>='0'&&c<='9')||c=='.'))
			return 0;
	}

	return 1;	
}

static void _do_request_packet_content_parse(ch_mysql_session_entry_t *mysql_entry,void *data,size_t dlen,uint8_t seq){

	ch_mysql_session_t *mysql_session = mysql_entry->msession;
	ch_pool_t *mp = mysql_entry->mp;
	
	ch_mysql_session_data_t *sdata = (ch_mysql_session_data_t*)ch_pcalloc(mp,sizeof(*sdata));
	sdata->cur_seq = seq;

	sdata->res_n = 0;
	INIT_LIST_HEAD(&sdata->res_list);

	sdata->req = ch_mysql_request_parse(mysql_entry->mp,data,dlen);

	/*append to list*/
    list_add_tail(&sdata->node,&mysql_session->entries);
	mysql_session->entries_n+=1;	
}

static int _do_request_packet_parse(ch_mysql_session_entry_t *mysql_entry,ch_pp_data_input_t *din){

	ch_mysql_packet_t mysql_pkt_tmp,*mysql_pkt = &mysql_pkt_tmp;

	void *data;
	size_t dlen;

	while(ch_pp_din_has_data(din,0)){

		data = (void*)ch_pp_din_pos(din);
		dlen =ch_pp_din_rdlen(din);
		
		if(data == NULL||dlen<4)
			return PARSE_CONTINUE;

		ch_mysql_packet_parse(mysql_pkt,data,dlen);

#if 0
		if(mysql_pkt->seq!=0){
			/*invalid mysql request packet*/
			return PARSE_BREAK;
		}
#endif
		if(mysql_pkt->plen+4>dlen){
			/*need more data*/
			return PARSE_CONTINUE;
		}

		/*can parse mysql request content*/
		_do_request_packet_content_parse(mysql_entry,data+4,mysql_pkt->plen,mysql_pkt->seq);
		ch_pp_din_pos_update(din,mysql_pkt->plen+4);
	}

	return PARSE_CONTINUE;
}

static int _do_request_parse(ch_mysql_session_entry_t *mysql_entry,ch_pp_data_input_t *din,void *data,size_t dlen){

	int rc;

	ch_mysql_session_t *mysql_session = mysql_entry->msession;

	if(mysql_session == NULL)
		return PARSE_BREAK;

	if(mysql_session->auth_phrase == AUTH_PHRASE_RES){

		/*parse mysql auth request*/
		if(dlen>37){

			mysql_session->user = _read_str(mysql_entry->mp,data,dlen,36);
		}
		mysql_session->auth_phrase = AUTH_PHRASE_REQ;

	}else if(mysql_session->auth_phrase == AUTH_PHRASE_ACK){

		/*parse mysql request */
		 if(ch_pp_din_read_prefare(din,data,dlen)){
			ch_log(CH_LOG_ERR,"Cannot init data input to parse mysql request!");
         	return PARSE_BREAK;
		}

		rc = _do_request_packet_parse(mysql_entry,din);
		if(rc == PARSE_CONTINUE){
			if(ch_pp_din_read_end(din))
				rc = PARSE_BREAK;
		}

		return rc;
	}else{

		return PARSE_BREAK;
	}
}

static void _mysql_session_create(ch_mysql_session_entry_t *mysql_entry,const char *version){
	
	ch_mysql_session_t *session = (ch_mysql_session_t*)ch_pcalloc(mysql_entry->mp,sizeof(ch_mysql_session_t));
	session->auth_phrase = AUTH_PHRASE_RES;
	session->auth_state = ERR_AUTH;
	session->version = version;
	session->user = "";
	session->entries_n = 0;
	INIT_LIST_HEAD(&session->entries);
    mysql_entry->msession = session;
}

static ch_mysql_session_data_t *find_session_data(ch_mysql_session_t *mysql_session,uint8_t seq){

	ch_mysql_session_data_t *sdata = NULL;

    if(list_empty(&mysql_session->entries))
        return NULL;

	list_for_each_entry(sdata,&mysql_session->entries,node){

        if(sdata->cur_seq+1==seq){

            return sdata;
        }
	}

    return NULL;
}

static void _do_response_packet_content_parse(ch_mysql_session_entry_t *mysql_entry,
        ch_mysql_session_data_t *sdata,
        void *data,size_t dlen,uint8_t seq){

    ch_mysql_response_t *res;

	ch_mysql_session_t *session = mysql_entry->msession;

    sdata->cur_seq = seq;

    res = ch_mysql_response_parse(mysql_entry->mp,data,dlen,seq);

    list_add_tail(&res->node,&sdata->res_list);
    sdata->res_n+=1;
}

static int  _do_response_packet_parse(ch_mysql_session_entry_t *mysql_entry,ch_pp_data_input_t *din){

	ch_mysql_packet_t mysql_pkt_tmp,*mysql_pkt = &mysql_pkt_tmp;
    ch_mysql_session_data_t *sdata;
	void *data;
	size_t dlen;
	ch_mysql_session_t *session = mysql_entry->msession;

	while(ch_pp_din_has_data(din,0)){

		data = (void*)ch_pp_din_pos(din);
		dlen =ch_pp_din_rdlen(din);
		
		if(data == NULL||dlen<4)
			return PARSE_CONTINUE;

		ch_mysql_packet_parse(mysql_pkt,data,dlen);

		if(mysql_pkt->seq<1){
			/*invalid mysql response packet*/
			return PARSE_BREAK;
		}

		if(mysql_pkt->plen+4>dlen){
			/*need more data*/
			return PARSE_CONTINUE;
		}

        sdata = find_session_data(session,mysql_pkt->seq);
        if(sdata == NULL){

            return PARSE_BREAK;
        }

		/*can parse mysql request content*/
		_do_response_packet_content_parse(mysql_entry,sdata,data+4,mysql_pkt->plen,mysql_pkt->seq);
		ch_pp_din_pos_update(din,mysql_pkt->plen+4);
	}

	return PARSE_CONTINUE;
}

static int _do_response_parse(ch_mysql_session_entry_t *mysql_entry,ch_pp_data_input_t *din,void *data,size_t dlen){
	
	int rc;
	ch_mysql_packet_t mysql_pkt_tmp,*mysql_pkt = &mysql_pkt_tmp;
	uint8_t res_code;
	const char *version;
	ch_mysql_session_t *mysql_session;

	if(mysql_entry->msession == NULL){
		version = _read_str(mysql_entry->mp,data,dlen,5);
		
        /*
        if(!_is_valid_mysql_version(version))
			return PARSE_BREAK;*/

		_mysql_session_create(mysql_entry,version);

		return PARSE_CONTINUE;
	}

	mysql_session = mysql_entry->msession;
	
	if(mysql_session->auth_phrase == AUTH_PHRASE_RES)
		return PARSE_BREAK;
	
	if(mysql_session->auth_phrase == AUTH_PHRASE_REQ){
		/*parse login response*/
		if(dlen<5)
			return PARSE_BREAK;

		ch_mysql_packet_parse(mysql_pkt,data,dlen);
		if(mysql_pkt->seq!=2)
			return PARSE_BREAK;

		res_code = BVMAKE(data+4,0);
		mysql_session->auth_phrase = AUTH_PHRASE_ACK;
		if(res_code == 0){
			/*login ok*/
			mysql_session->auth_state = OK_AUTH;
		}
		else{
			mysql_session->auth_state = ERR_AUTH;
			return PARSE_BREAK;
		}

        return PARSE_CONTINUE;
	}

	if(mysql_session->auth_phrase == AUTH_PHRASE_ACK){

		/*parse mysql response */
		 if(ch_pp_din_read_prefare(din,data,dlen)){
			ch_log(CH_LOG_ERR,"Cannot init data input to parse mysql response!");
         	return PARSE_BREAK;
		}

		rc = _do_response_packet_parse(mysql_entry,din);
		if(rc == PARSE_CONTINUE){
			if(ch_pp_din_read_end(din))
				rc = PARSE_BREAK;
		}

		return rc;
	}

	return PARSE_CONTINUE;	
}

static int _do_data_parse(ch_tcp_app_t *app ch_unused,ch_proto_session_store_t *pstore ch_unused, ch_tcp_session_t *tsession,
        void *data,size_t dlen,int is_req){

    ch_mysql_session_entry_t *mysql_entry = (ch_mysql_session_entry_t*)tsession->sentry;

    ch_pp_data_input_t *din = is_req?&mysql_entry->data_input_cli:&mysql_entry->data_input_srv;

    if(mysql_entry->msession == NULL){

        if(is_req){
            /*not mysql protocol*/
            return PARSE_BREAK;
        }

        if(_is_mysql(data,dlen)==0)
			return PARSE_BREAK;
    }

    ch_proto_session_entry_update(&mysql_entry->psEntry,dlen,is_req);

	if(is_req)
		return _do_request_parse(mysql_entry,din,data,dlen);

	return _do_response_parse(mysql_entry,din,data,dlen);
}


static int 
do_mysql_request_parse(ch_tcp_app_t *app,ch_proto_session_store_t *pstore,ch_tcp_session_t *tsession,void *data,size_t dlen){

    
    return _do_data_parse(app,pstore,tsession,data,dlen,1);

}


static int 
do_mysql_response_parse(ch_tcp_app_t *app,ch_proto_session_store_t *pstore,ch_tcp_session_t *tsession,void *data,size_t dlen){

    return _do_data_parse(app,pstore,tsession,data,dlen,0);

}
