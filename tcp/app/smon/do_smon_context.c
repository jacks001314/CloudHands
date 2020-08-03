/*
 *
 *      Filename: do_smon_context.c
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-07-12 17:30:26
 * Last Modified: 2018-07-30 10:44:01
 */

static const char *cmd_smon_reqbody_dir(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

	private_smon_context_t *mcontextt = (private_smon_context_t*)_dcfg;

	mcontextt->req_body_dir = p1;

	return NULL;
}

static const char *cmd_smon_resbody_dir(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

	private_smon_context_t *mcontextt = (private_smon_context_t*)_dcfg;

	mcontextt->res_body_dir = p1;

	return NULL;
}

static const char *cmd_smon_rule_json_file(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){


    private_smon_context_t *mcontext = (private_smon_context_t*)_dcfg;

    mcontext->rule_json_file = p1;

	return NULL;
}

static const char *cmd_smon_body_dir_create_type(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

	int rc;

	private_smon_context_t *mcontextt = (private_smon_context_t*)_dcfg;

	rc = ch_fpath_create_type_get(p1);
	if(rc == -1){
	
		return "Invalid smon header dir create type config item!";

	}

	mcontextt->create_dir_type = rc;
	return NULL;
}

static const char *cmd_smon_req_max_size(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    char *endptr;

    private_smon_context_t *mcontext = (private_smon_context_t*)_dcfg;

    mcontext->max_req_size = (size_t)strtoul(p1,&endptr,10);

	return NULL;
}

static const char *cmd_smon_res_max_size(cmd_parms *cmd ch_unused, void *_dcfg, const char *p1){

    char *endptr;

    private_smon_context_t *mcontext = (private_smon_context_t*)_dcfg;

    mcontext->max_res_size = (size_t)strtoul(p1,&endptr,10);

	return NULL;
}

static const command_rec mcontext_directives[] = {
    
    CH_INIT_TAKE1(
            "CHTCPAPPSmonReqBodyDir",
            cmd_smon_reqbody_dir,
            NULL,
            0,
            "set the store path of smon request body"
            ),

    CH_INIT_TAKE1(
            "CHTCPAPPSmonResBodyDir",
            cmd_smon_resbody_dir,
            NULL,
            0,
            "set the store path of smon response body"
            ),

    CH_INIT_TAKE1(
            "CHTCPAPPSmonBodyDirCreateType",
            cmd_smon_body_dir_create_type,
            NULL,
            0,
            "set the type created of smon body dir by time"
            ),


	CH_INIT_TAKE1(
            "CHTCPAPPSmonRuleJsonFile",
            cmd_smon_rule_json_file,
            NULL,
            0,
            "set  session monitor rule json file path"
            ),

	CH_INIT_TAKE1(
            "CHTCPAPPSmonReqMaxSize",
            cmd_smon_req_max_size,
            NULL,
            0,
            "set  session monitor request content max size"
            ),

	CH_INIT_TAKE1(
            "CHTCPAPPSmonResMaxSize",
            cmd_smon_res_max_size,
            NULL,
            0,
            "set  session monitor response content max size"
            ),
};


static void _smon_context_dump(private_smon_context_t *mcontext){

	fprintf(stdout,"Dump Session Monitor Context Informations:\n");
	fprintf(stdout,"session.monitor.reqBodyDir:%s\n",mcontext->req_body_dir);
	fprintf(stdout,"session.monitor.resBodyDir:%s\n",mcontext->res_body_dir);
	fprintf(stdout,"session.monitor.ruleJsonFile:%s\n",mcontext->rule_json_file);
	fprintf(stdout,"session.monitor.reqMaxSize:%lu\n",(unsigned long)mcontext->max_req_size);
	fprintf(stdout,"session.monitor.resMaxSize:%lu\n",(unsigned long)mcontext->max_res_size);

}

static int do_smon_context_init(ch_pool_t *mp,private_smon_context_t *mcontext,const char *cfname){

    const char * msg = NULL;
	

    msg = ch_process_command_config(mcontext_directives,(void*)mcontext,mp,mp,cfname);

    /*config failed*/
    if(msg!=NULL){

        ch_log(CH_LOG_ERR,"Config tcp app smon  context error:%s",msg);
        return -1;
    }

	_smon_context_dump(mcontext);

    return 0;

}
