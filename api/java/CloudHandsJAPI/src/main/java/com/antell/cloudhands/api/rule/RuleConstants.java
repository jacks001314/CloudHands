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
    public static final String mail = "mail";
    public static final String ftp = "ftp";
    public static final String ssh = "ssh";
    public static final String telnet = "telnet";

    public static final String smon = "smon";
    public static final String pkt = "pkt";

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

    /*target name for mail session*/
    public static final String mailUser = "user";
    public static final String mailPasswd = "passwd";
    public static final String fromMail = "fromMail";
    public static final String toMails = "toMails";
    public static final String ccMails = "ccMails";
    public static final String title = "title";
    public static final String content = "content";
    public static final String attachNames = "attachNames";
    //public static final String attachContent = "attachContent";

    /*target name for session monitor*/
    public static final String mid = "id";
    public static final String mproto = "proto";
    public static final String mreqData = "mreqData";
    public static final String mresData = "mresData";

    /*target name for ftp*/
    public static final String ftpUser = "user";
    public static final String ftpPasswd = "passwd";
    public static final String ftpLoginCode = "loginCode";
    public static final String ftpCmds = "cmds";
    public static final String ftpCmdArgs = "cmdArgs";
    public static final String ftpCmdResCodes = "cmdCode";
    public static final String ftpCmdResPhrase = "cmdPhrase";


    /*
    *ssh target names
    * */
    public static final String sshClientVersion = "cversion";
    public static final String sshServerVersion = "sversion";
    public static final String sshClientPkts = "cpkts";
    public static final String sshServerPkts = "spkts";
    public static final String sshClientBytes = "cbytes";
    public static final String sshServerBytes = "sbytes";
    public static final String sshClientEncPkts = "cEncPkts";
    public static final String sshServerEncPkts = "sEncPkts";
    public static final String sshClientEncBytes = "cEncBytes";
    public static final String sshServerEncBytes = "sEncBytes";
    public static final String sshClientEncMinBytes = "cEncMinBytes";
    public static final String sshServerEncMinBytes = "sEncMinBytes";
    public static final String sshClientEncMaxBytes = "cEncMaxBytes";
    public static final String sshServerEncMaxBytes = "sEncMaxBytes";

    /*telnet target names*/
    public static final String telnetUser = "user";
    public static final String telnetPasswd = "passwd";
    public static final String telnetContent = "content";


    /*packet target names*/

    public static final String pktL3Proto = "pktL3Proto";
    public static final String pktL4Proto = "pktL4Proto";
    public static final String pktDataSize = "pktDataSize";
    public static final String pktData = "pktData";
    public static final String pktL2HeaderSize="pktL2HeaderSize";
    public static final String pktL2Header = "pktL2Header";
    public static final String pktL3HeaderSize = "pktL3HeaderSize";
    public static final String pktL3Header = "pktL3Header";
    public static final String pktL4HeaderSize = "pktL4HeaderSize";
    public static final String pktL4Header = "pktL4Header";
    public static final String pktPayLoadSize = "pktPayLoadSize";
    public static final String pktPayload = "pktPayload";
    public static final String pktType = "pktType";

    /*proto2 targets maps*/
    private static final Map<String, List<RuleName>> maps = new HashMap<>();

    static {
        RuleName[] httpTargets = {
                new RuleName(srcIP,"源IP"),
                new RuleName(dstIP,"目的IP"),
                new RuleName(srcPort,"源端口"),
                new RuleName(dstPort,"目的端口"),
                new RuleName(method,"请求方法"),
                new RuleName(uri,"请求uri"),
                new RuleName(extName,"文件扩展名"),
                new RuleName(proto,"http协议版本"),
                new RuleName(furi,"全uri"),
                new RuleName(host,"目标主机"),
                new RuleName(ua,"User-Agent"),
                new RuleName(status,"响应状态码"),
                new RuleName(reqHeaderPrefix,"请求头"),
                new RuleName(resHeaderPrefix,"响应头"),
                new RuleName(reqBody,"请求体"),
                new RuleName(resBody,"响应体")
        };

        maps.put(http, Arrays.asList(httpTargets));

        RuleName[] dnsTargets = {
                new RuleName(srcIP,"源IP"),
                new RuleName(dstIP,"目的IP"),
                new RuleName(srcPort,"源端口"),
                new RuleName(dstPort,"目的端口"),
                new RuleName(domain,"请求域名")
        };

        maps.put(dns, Arrays.asList(dnsTargets));

        RuleName[] tcpTargets = {
                new RuleName(srcIP,"源IP"),
                new RuleName(dstIP,"目的IP"),
                new RuleName(srcPort,"源端口"),
                new RuleName(dstPort,"目的端口"),
                new RuleName(reqDataSize,"请求数据大小"),
                new RuleName(resDataSize,"响应数据大小"),
                new RuleName(reqData,"请求数据内容"),
                new RuleName(resData,"响应数据内容")
        };

        maps.put(tcp, Arrays.asList(tcpTargets));
        maps.put(udp,Arrays.asList(tcpTargets));

        RuleName[] mailTargets = {

                new RuleName(srcIP,"源IP"),
                new RuleName(dstIP,"目的IP"),
                new RuleName(srcPort,"源端口"),
                new RuleName(dstPort,"目的端口"),
                new RuleName(mailUser,"用户名"),
                new RuleName(mailPasswd ,"登录密码"),
                new RuleName(fromMail,"发件人邮箱"),
                new RuleName(toMails,"收件人邮箱"),
                new RuleName(ccMails,"抄送人邮箱"),
                new RuleName(title,"邮件标题"),
                new RuleName(content,"邮件正文"),
                new RuleName(attachNames,"附件名称")
                //new RuleTarget(attachContent,"附件内容")
        };

        maps.put(mail, Arrays.asList(mailTargets));

        RuleName[] smonTargets = {

                new RuleName(srcIP,"源IP"),
                new RuleName(dstIP,"目的IP"),
                new RuleName(srcPort,"源端口"),
                new RuleName(dstPort,"目的端口"),
                new RuleName(mid,"监控任务id"),
                new RuleName(mproto ,"监控协议类型"),
                new RuleName(mreqData,"监控会话请求体"),
                new RuleName(mresData ,"监控会话响应体")

        };

        maps.put(smon, Arrays.asList(smonTargets));

        RuleName[] ftpTargets = {

                new RuleName(srcIP,"源IP"),
                new RuleName(dstIP,"目的IP"),
                new RuleName(srcPort,"源端口"),
                new RuleName(dstPort,"目的端口"),
                new RuleName(ftpUser,"ftp登录用户"),
                new RuleName(ftpPasswd ,"ftp登录密码"),
                new RuleName(ftpLoginCode,"ftp登录状态码"),
                new RuleName(ftpCmds ,"ftp命令"),
                new RuleName(ftpCmdArgs ,"ftp命令参数"),
                new RuleName(ftpCmdResCodes ,"ftp命令响应码"),
                new RuleName(ftpCmdResPhrase ,"ftp命令响应短语")
        };

        maps.put(ftp, Arrays.asList(ftpTargets));

        RuleName[] sshTargets = {
                    new RuleName(srcIP,"源IP"),
                    new RuleName(dstIP,"目的IP"),
                    new RuleName(srcPort,"源端口"),
                    new RuleName(dstPort,"目的端口"),
                    new RuleName(sshClientVersion,"ssh客户端软件版本"),
                    new RuleName(sshClientPkts,"ssh客户端发送的数据包数"),
                    new RuleName(sshClientBytes,"ssh客户端发送的数据包字节数"),
                    new RuleName(sshClientEncPkts,"ssh客户端发送的加密包数"),
                    new RuleName(sshClientEncBytes,"ssh客户端发送的加密包字节数"),
                    new RuleName(sshClientEncMinBytes,"ssh客户端发送的最小加密包字节数"),
                    new RuleName(sshClientEncMaxBytes,"ssh客户端发送的最大加密包字节数"),
                    new RuleName(sshServerVersion,"sshf服务端软件版本"),
                    new RuleName(sshServerPkts,"ssh服务端发送的数据包数"),
                    new RuleName(sshServerBytes,"ssh服务端发送的数据包字节数"),
                    new RuleName(sshServerEncPkts,"ssh服务端发送的加密包数"),
                    new RuleName(sshServerEncBytes,"ssh服务端发送的加密包字节数"),
                    new RuleName(sshServerEncMinBytes,"ssh服务端发送的最小加密包字节数"),
                    new RuleName(sshServerEncMaxBytes,"ssh服务端发送的最大加密包字节数"),
            };

        maps.put(ssh, Arrays.asList(sshTargets));

        RuleName[] telnetTargets = {
                new RuleName(srcIP,"源IP"),
                new RuleName(dstIP,"目的IP"),
                new RuleName(srcPort,"源端口"),
                new RuleName(dstPort,"目的端口"),
                new RuleName(telnetUser,"telnet登录用户名"),
                new RuleName(telnetPasswd,"telnet登录密码"),
                new RuleName(telnetContent,"telnet会话内容")
        };

        maps.put(telnet, Arrays.asList(telnetTargets));

        RuleName[] pktTargets = {

                new RuleName(srcIP,"源IP"),
                new RuleName(dstIP,"目的IP"),
                new RuleName(srcPort,"源端口"),
                new RuleName(dstPort,"目的端口"),
                new RuleName(pktL3Proto,"网络层协议"),
                new RuleName(pktL4Proto,"传输层协议"),
                new RuleName(pktDataSize,"包大小"),
                new RuleName(pktData,"包内容"),
                new RuleName(pktL2HeaderSize,"链路层头部大小"),
                new RuleName(pktL2Header,"链路层头部内容"),
                new RuleName(pktL3HeaderSize,"网络层头部大小"),
                new RuleName(pktL3Header,"网络层头部内容"),
                new RuleName(pktL4HeaderSize,"传输层头部大小"),
                new RuleName(pktL4Header,"传输层头部内容"),
                new RuleName(pktPayLoadSize,"包载荷大小"),
                new RuleName(pktPayload,"包载荷内容"),
                new RuleName(pktType,"包类型")
        };

        maps.put(pkt,Arrays.asList(pktTargets));
    }

    public final static List<String> getProtos() {

        String[] protos = {tcp,udp,http,dns,mail,ftp,ssh,telnet,smon,pkt};

        return Arrays.asList(protos);
    }

    public final static List<RuleName> getTargets(String proto){

        return maps.get(proto);
    }

    public final static List<RuleName> getOPs(){

        RuleName[] ops = {
                new RuleName(contains,"包含"),
                new RuleName(startsWith,"以**开头"),
                new RuleName(endsWith,"以**为结尾"),
                new RuleName(regex,"正则匹配"),
                new RuleName(eq,"等于"),
                new RuleName(lt,"小于"),
                new RuleName(gt,"大于"),
                new RuleName(le,"小于等于"),
                new RuleName(ge,"大于等于")
        };

        return Arrays.asList(ops);
    }
}
