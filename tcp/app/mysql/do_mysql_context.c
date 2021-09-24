
/*
 *
 *      Filename: do_mysql_context.c
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2021-08-29 19:41:26
 * Last Modified: 2021-08-29 19:41:46
 */

static const char *cmd_mysql_ports(cmd_parms * cmd ch_unused, void *_dcfg, int argc,char *const argv[]){

    private_mysql_context_t *context = (private_mysql_context_t*)_dcfg;

	return ch_config_ports(context->mysql_ports,argc,argv,MYSQL_PORTS_MAX);
}

static const command_rec mysqlcontext_directives[] = {
    
	CH_INIT_TAKE_ARGV(
            "CHTCPAPPMYSQLPorts",
            cmd_mysql_ports,
            NULL,
            0,
            "set mysql ports"
            ),

};


static void _mysql_context_dump(private_mysql_context_t *context){

	fprintf(stdout,"Dump mysql Config Context Informations:\n");

    ch_dump_ports(context->mysql_ports,"tcp.mysql",MYSQL_PORTS_MAX);
}

static int do_mysql_context_init(ch_pool_t *mp,private_mysql_context_t *context,const char *cfname){

    const char * msg = NULL;
	

    msg = ch_process_command_config(mysqlcontext_directives,(void*)context,mp,mp,cfname);

    /*config failed*/
    if(msg!=NULL){

        ch_log(CH_LOG_ERR,"Config tcp app mysql  context error:%s",msg);
        return -1;
    }

	_mysql_context_dump(context);

    return 0;

}

