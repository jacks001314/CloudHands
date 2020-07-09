package com.antell.cloudhands.api.rule;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

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

    /*http proto2 targets maps*/
    private static final Map<String, List<RuleTarget>> maps = new HashMap<>();

    static {
        RuleTarget[] httpTargets = {
                new RuleTarget("srcIP","源IP"),
                new RuleTarget("dstIP","目的IP"),
                new RuleTarget("srcPort","源端口"),
                new RuleTarget("dstPort","目的端口"),
                new RuleTarget("method","请求方法"),
                new RuleTarget("uri","请求uri"),
                new RuleTarget("extName","文件扩展名"),
                new RuleTarget("proto","http协议版本"),
                new RuleTarget("furi","全uri"),
                new RuleTarget("host","目标主机"),
                new RuleTarget("ua","User-Agent"),
                new RuleTarget("status","响应状态码"),
                new RuleTarget("reqHead","请求头"),
                new RuleTarget("resHead","响应头"),
                new RuleTarget("reqBody","请求体"),
                new RuleTarget("resBody","响应体")
        };

        maps.put("http", Arrays.asList(httpTargets));

        RuleTarget[] dnsTargets = {
                new RuleTarget("srcIP","源IP"),
                new RuleTarget("dstIP","目的IP"),
                new RuleTarget("srcPort","源端口"),
                new RuleTarget("dstPort","目的端口"),
                new RuleTarget("domain","请求域名")
        };

        maps.put("dns", Arrays.asList(dnsTargets));

        RuleTarget[] tcpTargets = {
                new RuleTarget("srcIP","源IP"),
                new RuleTarget("dstIP","目的IP"),
                new RuleTarget("srcPort","源端口"),
                new RuleTarget("dstPort","目的端口"),
                new RuleTarget("reqDataSize","请求方法"),
                new RuleTarget("resDataSize","请求uri"),
                new RuleTarget("reqData","文件扩展名"),
                new RuleTarget("resData","http协议版本")
        };

        maps.put("tcp", Arrays.asList(tcpTargets));

    }


    public final static List<String> getProtos(){

        String[] protos = {"http","dns","tcp"};

        return Arrays.asList(protos);
    }

    public final static List<RuleTarget> getTargets(String proto){

        return maps.get(proto);
    }

}
