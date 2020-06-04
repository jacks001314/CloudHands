package com.antell.cloudhands.api.rule;

public class RuleConstants {

    private RuleConstants(){

    }

    /*
    * rule proto names
    * */

    public static final String http = "http";
    public static final String dns = "dns";
    public static final String tcp = "tcp";
    public static final String udp = "udp";

    public static final String arrSplit = ",";

    /*rule op names*/
    public static final String contains = "contains";
    public static final String startsWith = "startsWith";
    public static final String endsWith = "endsWith";
    public static final String regex = "regex";
    public static final String eq = "eq";
    public static final String lt = "lt";
    public static final String gt = "gt";
    public static final String le = "le";
    public static final String ge = "ge";
    public static final String in = "in";

    /*target common name*/
    public static final String srcIP = "srcIP";
    public static final String dstIP = "dstIP";
    public static final String dstPort = "dstPort";
    public static final String srcPort = "srcPort";

    /*target name for http session*/
    public static final String method = "method";
    public static final String uri = "uri";
    public static final String extName = "extName";
    public static final String proto = "proto";
    public static final String furi = "furi";
    public static final String host = "host";
    public static final String ua = "ua";
    public static final String status = "status";
    public static final String reqHeaderPrefix = "reqHead";
    public static final String resHeaderPrefix = "resHead";
    public static final String reqBody = "reqBody";
    public static final String resBody = "resBody";

    /*target name for dns*/
    public static final String domain = "domain";

    /*target name for tcp stream session*/
    public static final String reqDataSize = "reqDataSize";
    public static final String resDataSize = "resDataSize";
    public static final String reqData = "reqData";
    public static final String resData = "resData";

}
