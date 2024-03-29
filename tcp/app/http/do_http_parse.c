/*
 *
 *      Filename: do_http_parse.c
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-05-16 15:50:50
 * Last Modified: 2018-09-10 11:25:36
 */



#define HAS_BODY(session,is_req) (is_req?((session)->headers_in.content_length_n>0||(session)->headers_in.chunked):\
	((session)->headers_out.content_length_n>0||(session)->headers_out.chunked))

#define HEADER_END_STATE_SET(session,is_req) do {								     \
	int has_body = HAS_BODY(session,is_req);									 \
	if(is_req)																		 \
		session->parse_phase = has_body?PARSE_PHASE_REQ_BODY:PARSE_PHASE_STATUS_LINE;\
	else																			 \
		session->parse_phase = has_body?PARSE_PHASE_RES_BODY:PARSE_PHASE_DONE;		 \
}while(0)

typedef struct _http_rule_context_data_t _http_rule_context_data_t;

struct _http_rule_context_data_t {

    int is_res;
    ch_tcp_session_t *tsession;
    ch_http_session_t *http_session;
    char buff[256];
};

static int _http_isMyProto(ch_rule_target_context_t *tcontext,int proto){

    tcontext = tcontext;
    return proto == PROTO_HTTP;
} 

static const char * _http_target_get(ch_rule_target_context_t *tcontext,ch_rule_target_t *rtarget,int isHex){

    _http_rule_context_data_t *priv_data = (_http_rule_context_data_t*)tcontext->data;
    ch_http_session_t *hsession = priv_data->http_session;
    ch_tcp_session_t *tsession = priv_data->tsession;
    int target = rtarget->target;
    const char *target_str = rtarget->target_str;

    int is_res = priv_data->is_res;

    const char *result = NULL;

    if(hsession == NULL||tsession == NULL)
        return NULL;

    memset(priv_data->buff,0,256);

    switch(target){

        case TARGET_SRCIP:
            result = (const char*)ch_ip_to_str(priv_data->buff,256,ch_tcp_session_srcip_get(tsession));
            break;

        case TARGET_DSTIP:
            result = (const char*)ch_ip_to_str(priv_data->buff,256,ch_tcp_session_dstip_get(tsession));
            break;

        case TARGET_SRCPORT:
            snprintf(priv_data->buff,256,"%d",(int)ch_tcp_session_srcport_get(tsession));
            result =  (const char*)priv_data->buff;
            break;

        case TARGET_DSTPORT:
            snprintf(priv_data->buff,256,"%d",(int)ch_tcp_session_dstport_get(tsession));
            result = (const char*)priv_data->buff;
            break;

        case TARGET_HTTP_METHOD:
            result =  (const char*)hsession->method;
            break;

        case TARGET_HTTP_URI:
            result =  (const char*)hsession->uri;
            break;

        case TARGET_HTTP_EXTNAME:
            result = NULL;
            break;

        case TARGET_HTTP_PROTO:
            result =  "http";
            break;

        case TARGET_HTTP_FURI:
            snprintf(priv_data->buff,256,"http://%s/%s",hsession->host,hsession->uri);
            result = (const char*)priv_data->buff;
            break;

        case TARGET_HTTP_HOST:
            result =  (const char*)hsession->host;
            break;

        case TARGET_HTTP_UA:
            if(hsession->headers_in.user_agent)
                result =  (const char*)hsession->headers_in.user_agent->val;
            else
                result = NULL;

            break;

        case TARGET_HTTP_STATUS:

            if(!priv_data->is_res){
                result =  NULL;
            }else{
                snprintf(priv_data->buff,256,"%d",(int)hsession->status_code);
                result = (const char*)priv_data->buff;
            }
            break;

        case TARGET_HTTP_REQHEADER:

            result = ch_http_session_header_value_find(hsession,
                    ch_rule_dot_key_get(target_str,"reqHead."),
                    1);
            break;

        case TARGET_HTTP_RESHEADER:
            
            if(!is_res)
                result = NULL;
            else
                result = ch_http_session_header_value_find(hsession,
                    ch_rule_dot_key_get(target_str,"resHead."),
                    0);
                
            break;

        case TARGET_HTTP_REQBODY:
            result = NULL;
            break;

        case TARGET_HTTP_RESBODY:
            result = NULL;
            break;

        default:
            result = NULL;
            break;
    }

    if(result !=NULL&&isHex){

        size_t dlen = strlen(result);
        if(dlen == 0)
            return NULL;

        return ch_rule_to_hex(hsession->mp,(unsigned char*)result,dlen);
    }

    return result;
}

static int _do_is_accept(ch_tcp_app_t *app,ch_tcp_session_t *tsession,ch_http_session_t *hsession,int is_res){

    ch_rule_target_context_t target_tmp,*rtcontext = &target_tmp;

    _http_rule_context_data_t tmp,*rcontext_data = &tmp;

    private_http_context_t *hcontext = (private_http_context_t*)app->context;
    ch_filter_engine_t *filter_engine = hcontext->filter_engine;

    if(filter_engine == NULL)
        return 1;

    rcontext_data->is_res = is_res;
    rcontext_data->tsession = tsession;
    rcontext_data->http_session = hsession;

    rtcontext->proto = "http";
    rtcontext->data = (void*)rcontext_data;
    rtcontext->isMyProto = _http_isMyProto;
    rtcontext->target = _http_target_get;

    return ch_filter_engine_accept(filter_engine,rtcontext);
}

static int _http_status_line_parse(ch_http_session_t *session,ch_pp_data_line_t *line){

	ch_http_status_line_t status,*status_line = &status;

	ch_http_status_line_init(status_line);

	if(ch_http_status_line_parse(status_line,line)!=CH_OK){
	
		ch_log(CH_LOG_ERR,"Invalid http response status line!");

		return -1;
	}

	session->parse_phase = PARSE_PHASE_RES_HEADER;

	ch_http_session_status_line_set(session,status_line);

	return 0;
}

static int _http_header_parse(ch_http_session_t *session,ch_pp_data_line_t *line,int is_req){

	int rc;
	ch_http_header_t header,*hptr = &header;
	
	ch_http_header_init(hptr);
	
	rc = ch_http_header_line_parse(hptr,line);

    if (rc != CH_OK) {
		return -1;
	
	}

	/*ok*/
	ch_http_session_header_add(session,hptr,is_req);
	return 0;
}

static int 
_http_body_file_create(ch_proto_body_store_t *bstore,ch_http_session_t *session,int is_req){

	ch_http_session_body_t *body;
	char *fname,*fname_tmp;

	ch_fpath_t *fpath = is_req?bstore->req_fpath:bstore->res_fpath;

	ch_fpath_name_create(fpath,&fname,&fname_tmp);
	
	if(fname == NULL){
	
		ch_log(CH_LOG_ERR,"Can not create a file used to store http body!");
		return -1;
	}

	body = ch_http_session_body_create(session->mp,(const char*)fname,0);

	if(body == NULL)
		return -1;

	if(is_req)
		session->req_body = body;
	else
		session->res_body = body;

	return 0;
}

#define HTTP_BODY_PARSE_STATE_SET(session,is_req) do {	\
	if(is_req)											\
		session->parse_phase = PARSE_PHASE_STATUS_LINE; \
	else												\
		session->parse_phase = PARSE_PHASE_DONE;		\
}while(0)

static int _http_body_chunked_parse(ch_http_session_t *session,ch_pp_data_input_t *din,int is_req){

	void *chunk_data = NULL;
	size_t chunk_data_len = 0;
	int rc = CH_AGAIN;
	size_t size;
	ch_http_chunked_t *chunk;

	ch_http_session_body_t *body = is_req?session->req_body:session->res_body;

	if(body->chunk == NULL){
	
		ch_http_session_body_chunk_init(body);
		if(body->chunk == NULL){
		
			ch_log(CH_LOG_ERR,"Cannot allocate instance of body chunk!");
			return -1;
		}
	}

	chunk = body->chunk;
	chunk->din = din;

	/*start to parse data chunked */
	for ( ;; ) {

		chunk_data = NULL;
		chunk_data_len = 0;

		rc = ch_http_chunked_parse(chunk);

		if (rc == CH_OK) {

			/* a chunk has been parsed successfully */
			size = ch_pp_din_rdlen(din);
			chunk_data = (void*)ch_pp_din_pos(din);

			if (size > chunk->size) {

				chunk_data_len = chunk->size;
				ch_pp_din_pos_update(din,chunk->size);
				chunk->size = 0;

			} else {

				chunk_data_len = size;
				chunk->size -= size;
				ch_pp_din_skip_to_end(din);
			}

			if(is_req)
				session->headers_in.content_length_n += chunk_data_len;
			else
				session->headers_out.content_length_n += chunk_data_len;

			ch_http_session_body_append(session,chunk_data,chunk_data_len,is_req);

			continue;
		}

		if (rc == CH_DONE) {

			/* a whole body has been parsed successfully */
			HTTP_BODY_PARSE_STATE_SET(session,is_req);
			break;
		}

		if (rc == CH_AGAIN) {

			/* amount of data we want to see next time */

			break;
		}

		/* invalid */

		ch_log(CH_LOG_ERR,"invalid chunked body");

		return -1;
	}

	return 0;
}


static int _http_body_direct_append(ch_http_session_t *session,ch_pp_data_input_t *din,int is_req){

	void *data;
	size_t dlen;

	data = (void*)ch_pp_din_pos(din);
	dlen = ch_pp_din_rdlen(din);


	ch_http_session_body_append(session,data,dlen,is_req);

	if(ch_is_http_body_complete(session,is_req))
		HTTP_BODY_PARSE_STATE_SET(session,is_req);


	ch_pp_din_skip_to_end(din);

	return 0;
}


#define NEED_CREATE_FILE_BODY(session,is_req) (is_req?(session)->req_body == NULL:(session)->res_body == NULL)

static int 
_http_body_parse(ch_tcp_app_t *app,ch_proto_session_store_t *pstore,ch_http_session_t *session,ch_pp_data_input_t *din,int is_req){

    private_http_context_t *hcontext = (private_http_context_t*)app->context;

    ch_proto_body_store_t *bstore = ch_proto_store_body_find_create(pstore,hcontext->req_body_dir,hcontext->res_body_dir,
            hcontext->create_dir_type,app->protocol_id); 

    if(bstore == NULL){
    
        return -1;
    }

	if(NEED_CREATE_FILE_BODY(session,is_req)){
	
		if(_http_body_file_create(bstore,session,is_req)){
		
			ch_log(CH_LOG_ERR,"Create http  body file store failed!");
			return -1;
		}
	}

	if(ch_http_body_is_chunked(session,is_req))
		/* parse body chunked */
		return _http_body_chunked_parse(session,din,is_req);
	
	return _http_body_direct_append(session,din,is_req);

}

static int _http_request_line_parse(ch_http_session_t *session,ch_pp_data_line_t *line){

	ch_http_request_line_t tmp,*req_line = &tmp;

	ch_http_request_line_init(req_line);

	
	if(CH_OK!=ch_http_request_line_parse(req_line,line)){
	
		/*ERROR*/
		return -1;
	}

	/* the request line has been parsed successfully */
	ch_http_session_request_line_set(session,req_line);
	
	if(session->http_version == CH_HTTP_VERSION_9){		
		session->parse_phase = PARSE_PHASE_RES_BODY;	
	}else{												
		session->parse_phase = PARSE_PHASE_REQ_HEADER;	
	}													

	return 0;
}



static int 
do_http_request_parse(ch_tcp_app_t *app,ch_proto_session_store_t *pstore,
        ch_tcp_session_t *tsession,void *data,size_t dlen){

	int rc = PARSE_CONTINUE;
	ch_pp_data_input_t *din;
	ch_pp_data_line_t tmp,*line=&tmp;
	ch_http_session_t *session = NULL;
	ch_http_session_entry_t *hsentry = (ch_http_session_entry_t*)tsession->sentry;
	



	session = ch_http_sentry_session_get(hsentry,1);
	if(session == NULL){

		/* no memory space to allocate a new session,so drop this tentry */
		ch_log(CH_LOG_ERR,"No memory to allocate a new session,so drop this session!");
		return PARSE_BREAK; 
	}

	ch_proto_session_entry_update(&session->psEntry,dlen,1);

	din = &hsentry->cur_req_din;

	if(ch_pp_din_read_prefare(din,data,dlen)){
	
		ch_log(CH_LOG_ERR,"Cannot init data input to parse http resquest!");
		return PARSE_BREAK;
	}
		
	/* start to parse */
	while(ch_pp_din_has_data(din,0)){

		switch(session->parse_phase){
		
		case PARSE_PHASE_STATUS_LINE:
		case PARSE_PHASE_RES_HEADER:
		case PARSE_PHASE_RES_BODY:
		case PARSE_PHASE_DONE:
			/*Thist request parsed ok,wait corresponding response data,
			 *so add it into session entry request session list and get a new session
			 *to continue parse
			 * */
			ch_http_sentry_session_add(hsentry,session);
			session = ch_http_sentry_session_get(hsentry,1);
			
			if(session == NULL){

				/* no memory space to allocate a new session,so drop this tentry */
				ch_log(CH_LOG_ERR,"No memory to allocate a new session,so drop this session!");
				rc = PARSE_BREAK;
				goto out;
			}
			break;

		case PARSE_PHASE_REQ_LINE:
			
			/*Try to read a line from data input*/
			if(ch_pp_data_input_line_read(din,line)){
			
				/*cannot get a complete line ,then continue wait more data*/
				rc = PARSE_CONTINUE;
				goto out;
			}

			/*parse status line*/
			if(_http_request_line_parse(session,line))
			{
				ch_log(CH_LOG_ERR,"Parse Http Request  line failed:%s",
					(const char*)ch_pstrndup(session->mp,(const char*)line->line,line->len));
				rc = PARSE_BREAK;
				goto out;
			}

			/*check extName black list from uri*/


			break;

		case PARSE_PHASE_REQ_HEADER:
			
			/*Try to read a header line from data input*/
			if(ch_pp_data_input_line_read(din,line))
			{
				/*cannot get a complete line ,then continue wait more data*/
				rc = PARSE_CONTINUE;
				goto out;
			}

			if(line->len == 0){
				/*parse header done*/
				HEADER_END_STATE_SET(session,1);

                /**add filter*/
                if(0==_do_is_accept(app,tsession,session,0)){

                    ch_log(CH_LOG_INFO,"This Http Session match filter rules in request parser phase,method:%s,host:%s,uri:%s",
                            session->method?(const char*)session->method:"no Method",
                            session->host?(const char*)session->host:"no host",
                            session->uri?(const char*)session->uri:"no uri");

                    session->is_pass = 1;
                    rc = PARSE_BREAK;
                    goto out;
                }

			}else{
				/*parse header*/
				if(_http_header_parse(session,line,1)){
				
					ch_log(CH_LOG_ERR,"Parse Http Request header failed:%s",
						(const char*)ch_pstrndup(session->mp,(const char*)line->line,line->len));
					rc = PARSE_BREAK;
					goto out;
				}

			}

			break;

		case PARSE_PHASE_REQ_BODY:
			
			/*parse http request body */
			if(_http_body_parse(app,pstore,session,din,1)){
			
				/* error */
				ch_log(CH_LOG_ERR,"Parse http request body failed!");
				rc = PARSE_BREAK;
				goto out;
			}

		default:
			break;
		}//switch
	}/*while*/

	if(session->parse_phase >PARSE_PHASE_REQ_BODY){
	
		/*should store current session*/
		ch_http_sentry_session_add(hsentry,session);
	}

out:
	if(ch_pp_din_read_end(din)){
	
		ch_log(CH_LOG_ERR,"Cannot complete data input end for parse http request!");
		return PARSE_BREAK;
	}
	return rc;
}

static void _http_session_store(ch_tcp_app_t *app,ch_proto_session_store_t *pstore,
        ch_tcp_session_t *tsession,ch_http_session_entry_t *hsentry,ch_http_session_t *session){

    app = app;

	if(session->is_pass==0){
		ch_http_sentry_session_remove(hsentry,session);
		if(ch_proto_session_store_write(pstore,tsession,(void*)session)){
			ch_log(CH_LOG_ERR,"Write http session failed!");
		}
	}else{
	
		ch_http_sentry_session_discard(hsentry,session);
		ch_log(CH_LOG_INFO,"This http session match filter rules,Discard a http session!");

	}

}

static int 
do_http_response_parse(ch_tcp_app_t *app,ch_proto_session_store_t *pstore,
        ch_tcp_session_t *tsession,void *data,size_t dlen){


	int rc = PARSE_CONTINUE;
	ch_pp_data_input_t *din;
	ch_pp_data_line_t tmp,*line=&tmp;

	ch_http_session_t *session = NULL;
	ch_http_session_entry_t *hsentry = (ch_http_session_entry_t*)tsession->sentry;
	
	session = ch_http_sentry_session_get(hsentry,0);

	if(session == NULL){

		/* no request??? */
		ch_log(CH_LOG_ERR,"Cannot find http request for this response data!");
		return PARSE_BREAK; 
	}

	ch_proto_session_entry_update(&session->psEntry,dlen,0);
	
	if(session->parse_phase>PARSE_PHASE_DONE){

		ch_log(CH_LOG_ERR,"Invalid parse phase status!");
		return PARSE_BREAK;
	}

	din = &hsentry->cur_res_din;

	if(ch_pp_din_read_prefare(din,data,dlen)){
	
		ch_log(CH_LOG_ERR,"Cannot init data input to parse http response!");
		return PARSE_BREAK;
	}
		
	/* start to parse */
	while(ch_pp_din_has_data(din,0)){

		switch(session->parse_phase){

		case PARSE_PHASE_DONE:
		
			/*has a new http respones,should output current session,and get a new session,then continue to parse */	
			_http_session_store(app,pstore,tsession,hsentry,session);
			session = ch_http_sentry_session_get(hsentry,0);
			if(session == NULL){
				/* no request??? */
				ch_log(CH_LOG_ERR,"Cannot find http request for this response data!");
				rc = PARSE_BREAK;
				goto out;
			}

			break;

		case PARSE_PHASE_STATUS_LINE:
			/*Try to read a line from data input*/
			if(ch_pp_data_input_line_read(din,line)){
			
				/*cannot get a complete line ,then continue wait more data*/
				rc = PARSE_CONTINUE;
				goto out;
			}
			/*parse status line*/
			if(_http_status_line_parse(session,line))
			{
				ch_log(CH_LOG_ERR,"Parse Http Response status line failed:%s",
					(const char*)ch_pstrndup(session->mp,(const char*)line->line,line->len));
				rc = PARSE_BREAK;
				goto out;
			}

			break;

		case PARSE_PHASE_RES_HEADER:
			/*Try to read a header line from data input*/
			if(ch_pp_data_input_line_read(din,line))
			{
				/*cannot get a complete line ,then continue wait more data*/
				rc = PARSE_CONTINUE;
				goto out;
			}

			if(line->len == 0){
				/*parse header done*/
				HEADER_END_STATE_SET(session,0);
                /**add filter*/
                if(0==_do_is_accept(app,tsession,session,1)){
                    
                    ch_log(CH_LOG_INFO,"This Http Session match filter rules in response parser phase,method:%s,host:%s,uri:%s",
                            session->method?(const char*)session->method:"no Method",
                            session->host?(const char*)session->host:"no host",
                            session->uri?(const char*)session->uri:"no uri");

                    session->is_pass = 1;
                    rc = PARSE_BREAK;
                    goto out;
                }
			}else{
				/*parse header*/
				if(_http_header_parse(session,line,0)){
				
					ch_log(CH_LOG_ERR,"Parse Http Resposne header failed:%s",
						(const char*)ch_pstrndup(session->mp,(const char*)line->line,line->len));
					rc = PARSE_BREAK;
					goto out;
				}

			}
			break;

		case PARSE_PHASE_RES_BODY:
			
			/*parse http response body */
			if(_http_body_parse(app,pstore,session,din,0)){
			
				/* error */
				ch_log(CH_LOG_ERR,"Parse http response body failed!");
				rc = PARSE_BREAK;
				goto out;
			}

			break;

		default:
			break;
		}//switch
	} //while
	
	if(session->parse_phase==PARSE_PHASE_DONE){
	
		/*should store session*/
		_http_session_store(app,pstore,tsession,hsentry,session);

		if(!is_http_session_keepalive(session)){
		
			rc = PARSE_DONE;
		}
	}

out:
	if(ch_pp_din_read_end(din)){
	
		ch_log(CH_LOG_ERR,"Cannot complete data input end for parse http response!");
		return PARSE_BREAK;
	}
	return rc;

}

