#include <stdlib.h>
#include <stdio.h>
#include "ch_cjson_util.h"

int ch_cjson_parse(const char *fname,void *obj,int (*parse)(cJSON *root,void *obj)){

    int rc = -1;

    size_t fsize;
    cJSON *root = NULL;
    FILE *fp = fopen(fname,"r");

    if(fp == NULL){
        return rc;
    }

    fseek(fp, 0, SEEK_END);
    fsize=ftell(fp);

    if(fsize<=0){
        fclose(fp);
        return rc;
    }

    void *buff = calloc(1,fsize+1);

    if(buff == NULL){
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

int ch_cjson_parse_from_content(const char *content,void *obj,int (*parse)(cJSON *root,void *obj)) {

    cJSON *root = NULL;
   
    root = cJSON_Parse(content);

    int rc = parse(root,obj);


    cJSON_Delete(root);

    return rc;
}