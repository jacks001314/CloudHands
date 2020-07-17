#include <stdlib.h>
#include <stdio.h>
#include "ch_cjson.h"
#include "ch_log.h"

int ch_cjson_parse(const char *fname,void *obj,int (*parse)(cJSON *root,void *obj)){

    int rc = -1;

    size_t fsize;
    cJSON *root = NULL;
    FILE *fp = fopen(fname,"r");

    if(fp == NULL){

        ch_log(CH_LOG_ERR,"Cannot open json file:%s to parse!",fname);
        return rc;
    }

    fseek(fp, 0, SEEK_END);
    fsize=ftell(fp);

    if(fsize<=0){

        ch_log(CH_LOG_ERR,"cjson file:%s is empty!",fname);
        fclose(fp);
        return rc;
    }

    void *buff = calloc(1,fsize+1);

    if(buff == NULL){

        ch_log(CH_LOG_ERR,"Cannot alloc memory:%d to parse cjson:%s",(int)fsize,fname);
        fclose(fp);
        return rc;
    }

    fseek(fp,0,SEEK_SET);

    fread(buff,fsize,1,fp);
    
    root = cJSON_Parse((const char*)buff);

    rc = parse(root,obj);

    fclose(fp);
    free(buff);
    cJSON_Delete(root);

    return rc;
}


