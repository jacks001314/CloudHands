
#include "ch_rule_context.h"
#include "ch_rule_constants.h"
#include "ch_rule_utils.h"
#include "ch_log.h"
#include "ch_util.h"

ch_rule_context_t *ch_rule_context_create(void){

    ch_rule_context_t *rcontext = NULL;
    ch_pool_t *mp = ch_pool_create(4096);

    if(mp == NULL)
    {
        ch_log(CH_LOG_ERR,"Cannot create memory pool failed for rule context!");
        return NULL;
    }

    rcontext = (ch_rule_context_t*)ch_palloc(mp,sizeof(*rcontext));

    rcontext->mp = mp;

    rcontext->maps = ch_table_make(mp,16);

    if(rcontext->maps == NULL)
    {
        ch_log(CH_LOG_ERR,"Cannot create tables failed for rule tables!");
        ch_pool_destroy(mp);
        return NULL;
    }

    rcontext->buff = (char*)malloc(1024);
    rcontext->buff_size = 1024; 

    if(rcontext->buff == NULL){

        ch_log(CH_LOG_ERR,"Cannot create memory failed for buffer!");
        ch_pool_destroy(mp);

        return NULL;
    }

    return rcontext;
}


void ch_rule_context_destroy(ch_rule_context_t *rcontext){

    free(rcontext->buff);

    ch_pool_destroy(rcontext->mp);

}

static ch_array_header_t *_find_value_arrays_from_file(ch_rule_context_t *rcontext,char *file_path){

    char line[MAX_LINE_SIZE] = {0};

    ch_array_header_t *arr = (ch_array_header_t*)ch_table_get(rcontext->maps,file_path);

    if(arr == NULL){
        
        FILE *fp = fopen(file_path,"r");
        if(fp == NULL){
            ch_log(CH_LOG_ERR,"Cannot open file:%s to load arrays!",file_path);
            return NULL;
        }

        arr = ch_array_make(rcontext->mp,128,sizeof(char*));
        if(arr == NULL){
            ch_log(CH_LOG_ERR,"Cannot make array for store file lines:%s",file_path);
            fclose(fp);
            return NULL;
        }

        while(1==ch_read_line(line,MAX_LINE_SIZE-1,fp)){

            if(*line == '#'||strlen(line)==0)
                continue;

            *(char**)ch_array_push(arr)= ch_pstrdup(rcontext->mp,line);
        }
        
        ch_table_setn(rcontext->maps,ch_pstrdup(rcontext->mp,file_path),(const char*)arr);

        fclose(fp);
    }

    return arr;
}

static ch_array_header_t * _find_value_arrays_from_inline(ch_rule_context_t *rcontext,const char *key,const char *value){

    if(key == NULL || strlen(key)==0)
        return NULL;


    ch_array_header_t *arr = (ch_array_header_t*)ch_table_get(rcontext->maps,key);

    if(arr == NULL){

        if(value == NULL||strlen(value)==0)
        {
            ch_log(CH_LOG_ERR,"value is emptry,cannot create array inline!");
            return NULL;
        }

        arr = ch_strsplit_to_arrays(rcontext->mp,value,ARR_VALUE_SPLIT);

        ch_table_setn(rcontext->maps,ch_pstrdup(rcontext->mp,key),(const char*)arr);
    }

    return arr;
}

ch_array_header_t *ch_rule_context_find_value_arrays(ch_rule_context_t *rcontext,const char *value){

    char *part,*last,*key,*split_value;

    if(value == NULL||strlen(value)==0)
        return NULL;

    char *cvalue = ch_rule_context_buff_update(rcontext,value);

    part = ch_strtok(cvalue,":",&last);

    if(part == NULL)
        return NULL;

    if(strcmp(part,"file")==0){
        part = ch_strtok(NULL,":",&last);
        if(part == NULL){
            ch_log(CH_LOG_ERR,"Invalid file value,no file path,value:%s",value);
            return NULL;
        }

        return _find_value_arrays_from_file(rcontext,part);

    }else if(strcmp(part,"inline")==0){

        key = ch_strtok(NULL,":",&last);
        if(key == NULL){
            ch_log(CH_LOG_ERR,"Invalid inline value,no key,value:%s",value);
            return NULL;
        }
        split_value = ch_strtok(NULL,":",&last);
        if(split_value == NULL){
            ch_log(CH_LOG_ERR,"Invalid inline value,no value,value:%s",value);
            return NULL;
        }

        return _find_value_arrays_from_inline(rcontext,key,split_value);

    }else{

        ch_log(CH_LOG_ERR,"UNknown value type:%s",part);
        return NULL;
    }
}
