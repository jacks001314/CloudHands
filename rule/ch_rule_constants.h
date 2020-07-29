#ifndef CH_RULE_CONSTANTS_H
#define CH_RULE_CONSTANTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *protos
 * */
#define PROTO_UNK -1
#define PROTO_TCP 0
#define PROTO_UDP 1
#define PROTO_HTTP 2
#define PROTO_MAIL 3
#define PROTO_FTP  4
#define PROTO_TELNET 5
#define PROTO_DNS 6
#define PROTO_SSH 7
#define PROTO_SMON 8
#define PROTO_PKT 9

/*
 *targets for common
 * */
#define TARGET_NONE -1
#define TARGET_SRCIP 0
#define TARGET_DSTIP 1
#define TARGET_SRCPORT 2
#define TARGET_DSTPORT 3
#define TARGET_USER 4
#define TARGET_PASSWD 5
#define TARGET_CONTENT 6
#define TARGET_ID 7
#define TARGET_PROTO 8

/*
 *targets for http
 * */
#define TARGET_HTTP_METHOD 10
#define TARGET_HTTP_URI 11
#define TARGET_HTTP_EXTNAME 12
#define TARGET_HTTP_PROTO 13
#define TARGET_HTTP_FURI 14
#define TARGET_HTTP_HOST 15
#define TARGET_HTTP_UA 16
#define TARGET_HTTP_STATUS 17
#define TARGET_HTTP_REQHEADER 18
#define TARGET_HTTP_RESHEADER 19
#define TARGET_HTTP_REQBODY 20
#define TARGET_HTTP_RESBODY 21



/*targets for dns*/
#define TARGET_DNS_DOMAIN 22

/*targets for udp/tcp stream data*/
#define TARGET_STREAM_REQDATA_SIZE 23
#define TARGET_STREAM_RESDATA_SIZE 24
#define TARGET_STREAM_REQDATA 25
#define TARGET_STREAM_RESDATA 26

/*
*targets for mail
*/
#define TARGET_MAIL_FROM_MAIL 27
#define TARGET_MAIL_TO_MAILS 28
#define TARGET_MAIL_CC_MAILS 29
#define TARGET_MAIL_TITLE 30
#define TARGET_MAIL_CONTENT 31
#define TARGET_MAIL_ATTACH_NAMES 32

/*targets for smon*/
#define TARGET_SMON_REQDATA 33
#define TARGET_SMON_RESDATA 34


/*targets for ftp*/
#define TARGET_FTP_LOGIN_CODE 35
#define TARGET_FTP_CMDS 36
#define TARGET_FTP_CMD_ARGS 37
#define TARGET_FTP_CMD_RES_CODES 38
#define TARGET_FTP_CMD_RES_PHRASE 39 

/*targets for ssh*/
#define TARGET_SSH_CVERSION 40
#define TARGET_SSH_SVERSION 41
#define TARGET_SSH_CPKTS 42
#define TARGET_SSH_SPKTS 43
#define TARGET_SSH_CBYTES 44
#define TARGET_SSH_SBYTES 45
#define TARGET_SSH_CENCPKTS 46
#define TARGET_SSH_SENCPKTS 47
#define TARGET_SSH_CENCBYTES 48
#define TARGET_SSH_SENCBYTES 49
#define TARGET_SSH_CENCMINBYTES 50
#define TARGET_SSH_SENCMINBYTES 51
#define TARGET_SSH_CENCMAXBYTES 52
#define TARGET_SSH_SENCMAXBYTES 53

/*targets for packets*/

#define TARGET_PKT_L3_PROTO 54
#define TARGET_PKT_L4_PROTO 55
#define TARGET_PKT_DATA_SIZE 56
#define TARGET_PKT_DATA 57
#define TARGET_PKT_L2_HEADER_SIZE 58
#define TARGET_PKT_L2_HEADER 59
#define TARGET_PKT_L3_HEADER_SIZE 60
#define TARGET_PKT_L3_HEADER 61
#define TARGET_PKT_L4_HEADER_SIZE 62
#define TARGET_PKT_L4_HEADER 63
#define TARGET_PKT_PAYLOAD_SIZE 64
#define TARGET_PKT_PAYLOAD 65
#define TARGET_PKT_TYPE 66

#define ARR_VALUE_SPLIT ","

/*operators*/
#define OP_none -1
#define OP_contains 0
#define OP_startsWith 1
#define OP_endsWith 2
#define OP_eq 3
#define OP_lt 4
#define OP_gt 5
#define OP_le 6
#define OP_ge 7
#define OP_num 8 

#define cmp_return(str1,str2,value) do { \
    if(strncasecmp(str1,str2,strlen(str2))==0)         \
        return value;                    \
}while(0)

static inline int ch_op_value_get(const char *op){

    if(op == NULL||strlen(op)==0)
        return OP_none;

    cmp_return(op,"contains",OP_contains);
    cmp_return(op,"startsWith",OP_startsWith);
    cmp_return(op,"endsWith",OP_endsWith);
    cmp_return(op,"eq",OP_eq);
    cmp_return(op,"lt",OP_lt);
    cmp_return(op,"gt",OP_gt);
    cmp_return(op,"le",OP_le);
    cmp_return(op,"ge",OP_ge);
    
    return OP_none;
}

static inline int ch_target_value_get(const char *target){

    if(target == NULL||strlen(target)== 0)
        return TARGET_NONE;

    cmp_return(target,"srcIP",TARGET_SRCIP);
    cmp_return(target,"dstIP",TARGET_DSTIP);
    cmp_return(target,"srcPort",TARGET_SRCPORT);
    cmp_return(target,"dstPort",TARGET_DSTPORT);
    cmp_return(target,"user",TARGET_USER);
    cmp_return(target,"passwd",TARGET_PASSWD);
    cmp_return(target,"content",TARGET_CONTENT);
    cmp_return(target,"id",TARGET_ID);
    cmp_return(target,"proto",TARGET_PROTO);
    
    cmp_return(target,"method",TARGET_HTTP_METHOD);
    cmp_return(target,"uri",TARGET_HTTP_URI);
    cmp_return(target,"extName",TARGET_HTTP_EXTNAME);
    cmp_return(target,"furi",TARGET_HTTP_FURI);
    cmp_return(target,"host",TARGET_HTTP_HOST);
    cmp_return(target,"ua",TARGET_HTTP_UA);
    cmp_return(target,"status",TARGET_HTTP_STATUS);
    cmp_return(target,"reqHead",TARGET_HTTP_REQHEADER);
    cmp_return(target,"resHead",TARGET_HTTP_RESHEADER);
    cmp_return(target,"reqBody",TARGET_HTTP_REQBODY);
    cmp_return(target,"resBody",TARGET_HTTP_RESBODY);
    
    cmp_return(target,"domain",TARGET_DNS_DOMAIN);
    
    cmp_return(target,"reqDataSize",TARGET_STREAM_REQDATA_SIZE);
    cmp_return(target,"resDataSize",TARGET_STREAM_RESDATA_SIZE);
    cmp_return(target,"reqData",TARGET_STREAM_REQDATA);
    cmp_return(target,"resData",TARGET_STREAM_RESDATA);
    
    cmp_return(target,"fromMail",TARGET_MAIL_FROM_MAIL);
    cmp_return(target,"toMails",TARGET_MAIL_TO_MAILS);
    cmp_return(target,"ccMails",TARGET_MAIL_CC_MAILS);
    cmp_return(target,"title",TARGET_MAIL_TITLE);
    cmp_return(target,"attachNames",TARGET_MAIL_ATTACH_NAMES);

    cmp_return(target,"mreqData",TARGET_SMON_REQDATA);
    cmp_return(target,"mresData",TARGET_SMON_RESDATA);

    cmp_return(target,"loginCode",TARGET_FTP_LOGIN_CODE);
    cmp_return(target,"cmds",TARGET_FTP_CMDS);
    cmp_return(target,"cmdArgs",TARGET_FTP_CMD_ARGS);
    cmp_return(target,"cmdCode",TARGET_FTP_CMD_RES_CODES);
    cmp_return(target,"cmdPhrase",TARGET_FTP_CMD_RES_PHRASE);

    cmp_return(target,"cversion",TARGET_SSH_CVERSION);
    cmp_return(target,"sversion",TARGET_SSH_SVERSION);
    cmp_return(target,"cpkts",TARGET_SSH_CPKTS);
    cmp_return(target,"spkts",TARGET_SSH_SPKTS);
    cmp_return(target,"cbytes",TARGET_SSH_CBYTES);
    cmp_return(target,"sbytes",TARGET_SSH_SBYTES);
    cmp_return(target,"cEncPkts",TARGET_SSH_CENCPKTS);
    cmp_return(target,"sEncPkts",TARGET_SSH_SENCPKTS);
    cmp_return(target,"cEncBytes",TARGET_SSH_CENCBYTES);
    cmp_return(target,"sEncBytes",TARGET_SSH_SENCBYTES);
    cmp_return(target,"cEncMinBytes",TARGET_SSH_CENCMINBYTES);
    cmp_return(target,"sEncMinBytes",TARGET_SSH_SENCMINBYTES);
    cmp_return(target,"cEncMaxBytes",TARGET_SSH_CENCMAXBYTES);
    cmp_return(target,"sEncMaxBytes",TARGET_SSH_SENCMAXBYTES);

    cmp_return(target,"pktL3Proto",TARGET_PKT_L3_PROTO);
    cmp_return(target,"pktL4Proto",TARGET_PKT_L4_PROTO);
    cmp_return(target,"pktDataSize",TARGET_PKT_DATA_SIZE);
    cmp_return(target,"pktData",TARGET_PKT_DATA);

    cmp_return(target,"pktL2HeaderSize",TARGET_PKT_L2_HEADER_SIZE);
    cmp_return(target,"pktL2Header",TARGET_PKT_L2_HEADER);

    cmp_return(target,"pktL3HeaderSize",TARGET_PKT_L3_HEADER_SIZE);
    cmp_return(target,"pktL3Header",TARGET_PKT_L3_HEADER);
    
    cmp_return(target,"pktL4HeaderSize",TARGET_PKT_L4_HEADER_SIZE);
    cmp_return(target,"pktL4Header",TARGET_PKT_L4_HEADER);
    
    cmp_return(target,"pktPayLoadSize",TARGET_PKT_PAYLOAD_SIZE);
    cmp_return(target,"pktPayload",TARGET_PKT_PAYLOAD);
    cmp_return(target,"pktType",TARGET_PKT_TYPE);

    return TARGET_NONE;
}

static inline int ch_proto_value_get(const char *proto){

    if(proto == NULL||strlen(proto)==0)
        return PROTO_UNK;

    cmp_return(proto,"tcp",PROTO_TCP);
    cmp_return(proto,"udp",PROTO_UDP);
    cmp_return(proto,"http",PROTO_HTTP);
    cmp_return(proto,"mail",PROTO_MAIL);
    cmp_return(proto,"ftp",PROTO_FTP);
    cmp_return(proto,"telnet",PROTO_TELNET);
    cmp_return(proto,"dns",PROTO_DNS);
    cmp_return(proto,"ssh",PROTO_SSH);
    cmp_return(proto,"smon",PROTO_SMON);
    cmp_return(proto,"pkt",PROTO_PKT);

    return PROTO_UNK;
}

#endif /*CH_RULE_CONSTANTS_H*/
