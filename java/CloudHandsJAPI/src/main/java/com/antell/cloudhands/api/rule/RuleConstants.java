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

    /*http proto2 targets maps*/
    private static final Map<String, List<RuleTarget>> maps = new HashMap<>();

    static {
        RuleTarget[] httpTargets = {
                new RuleTarget(srcIP,"源IP"),
                new RuleTarget(dstIP,"目的IP"),
                new RuleTarget(srcPort,"源端口"),
                new RuleTarget(dstPort,"目的端口"),
                new RuleTarget(method,"请求方法"),
                new RuleTarget(uri,"请求uri"),
                new RuleTarget(extName,"文件扩展名"),
                new RuleTarget(proto,"http协议版本"),
                new RuleTarget(furi,"全uri"),
                new RuleTarget(host,"目标主机"),
                new RuleTarget(ua,"User-Agent"),
                new RuleTarget(status,"响应状态码"),
                new RuleTarget(reqHeaderPrefix,"请求头"),
                new RuleTarget(resHeaderPrefix,"响应头"),
                new RuleTarget(reqBody,"请求体"),
                new RuleTarget(resBody,"响应体")
        };

        maps.put(http, Arrays.asList(httpTargets));

        RuleTarget[] dnsTargets = {
                new RuleTarget(srcIP,"源IP"),
                new RuleTarget(dstIP,"目的IP"),
                new RuleTarget(srcPort,"源端口"),
                new RuleTarget(dstPort,"目的端口"),
                new RuleTarget(domain,"请求域名")
        };

        maps.put(dns, Arrays.asList(dnsTargets));

        RuleTarget[] tcpTargets = {
                new RuleTarget(srcIP,"源IP"),
                new RuleTarget(dstIP,"目的IP"),
                new RuleTarget(srcPort,"源端口"),
                new RuleTarget(dstPort,"目的端口"),
                new RuleTarget(reqDataSize,"请求数据大小"),
                new RuleTarget(resDataSize,"响应数据大小"),
                new RuleTarget(reqData,"请求数据内容"),
                new RuleTarget(resData,"响应数据内容")
        };

        maps.put(tcp, Arrays.asList(tcpTargets));
        maps.put(udp,Arrays.asList(tcpTargets));

        RuleTarget[] mailTargets = {

                new RuleTarget(srcIP,"源IP"),
                new RuleTarget(dstIP,"目的IP"),
                new RuleTarget(srcPort,"源端口"),
                new RuleTarget(dstPort,"目的端口"),
                new RuleTarget(mailUser,"用户名"),
                new RuleTarget(mailPasswd ,"登录密码"),
                new RuleTarget(fromMail,"发件人邮箱"),
                new RuleTarget(toMails,"收件人邮箱"),
                new RuleTarget(ccMails,"抄送人邮箱"),
                new RuleTarget(title,"邮件标题"),
                new RuleTarget(content,"邮件正文"),
                new RuleTarget(attachNames,"附件名称")
                //new RuleTarget(attachContent,"附件内容")
        };

        maps.put(mail, Arrays.asList(mailTargets));

        RuleTarget[] smonTargets = {

                new RuleTarget(srcIP,"源IP"),
                new RuleTarget(dstIP,"目的IP"),
                new RuleTarget(srcPort,"源端口"),
                new RuleTarget(dstPort,"目的端口"),
                new RuleTarget(mid,"监控任务id"),
                new RuleTarget(mproto ,"监控协议类型"),
                new RuleTarget(mreqData,"监控会话请求体"),
                new RuleTarget(mresData ,"监控会话响应体")

        };

        maps.put(smon, Arrays.asList(smonTargets));

        RuleTarget[] ftpTargets = {

                new RuleTarget(srcIP,"源IP"),
                new RuleTarget(dstIP,"目的IP"),
                new RuleTarget(srcPort,"源端口"),
                new RuleTarget(dstPort,"目的端口"),
                new RuleTarget(ftpUser,"ftp登录用户"),
                new RuleTarget(ftpPasswd ,"ftp登录密码"),
                new RuleTarget(ftpLoginCode,"ftp登录状态码"),
                new RuleTarget(ftpCmds ,"ftp命令"),
                new RuleTarget(ftpCmdArgs ,"ftp命令参数"),
                new RuleTarget(ftpCmdResCodes ,"ftp命令响应码"),
                new RuleTarget(ftpCmdResPhrase ,"ftp命令响应短语")
        };

        maps.put(ftp, Arrays.asList(ftpTargets));

        RuleTarget[] sshTargets = {
                    new RuleTarget(srcIP,"源IP"),
                    new RuleTarget(dstIP,"目的IP"),
                    new RuleTarget(srcPort,"源端口"),
                    new RuleTarget(dstPort,"目的端口"),
                    new RuleTarget(sshClientVersion,"ssh客户端软件版本"),
                    new RuleTarget(sshClientPkts,"ssh客户端发送的数据包数"),
                    new RuleTarget(sshClientBytes,"ssh客户端发送的数据包字节数"),
                    new RuleTarget(sshClientEncPkts,"ssh客户端发送的加密包数"),
                    new RuleTarget(sshClientEncBytes,"ssh客户端发送的加密包字节数"),
                    new RuleTarget(sshClientEncMinBytes,"ssh客户端发送的最小加密包字节数"),
                    new RuleTarget(sshClientEncMaxBytes,"ssh客户端发送的最大加密包字节数"),
                    new RuleTarget(sshServerVersion,"sshf服务端软件版本"),
                    new RuleTarget(sshServerPkts,"ssh服务端发送的数据包数"),
                    new RuleTarget(sshServerBytes,"ssh服务端发送的数据包字节数"),
                    new RuleTarget(sshServerEncPkts,"ssh服务端发送的加密包数"),
                    new RuleTarget(sshServerEncBytes,"ssh服务端发送的加密包字节数"),
                    new RuleTarget(sshServerEncMinBytes,"ssh服务端发送的最小加密包字节数"),
                    new RuleTarget(sshServerEncMaxBytes,"ssh服务端发送的最大加密包字节数"),
            };

        maps.put(ssh, Arrays.asList(sshTargets));

        RuleTarget[] telnetTargets = {
                new RuleTarget(srcIP,"源IP"),
                new RuleTarget(dstIP,"目的IP"),
                new RuleTarget(srcPort,"源端口"),
                new RuleTarget(telnetUser,"telnet登录用户名"),
                new RuleTarget(telnetPasswd,"telnet登录密码"),
                new RuleTarget(telnetContent,"telnet会话内容")
        };

        maps.put(telnet, Arrays.asList(telnetTargets));
    }

    public final static List<String> getProtos() {

        String[] protos = {tcp,udp,http,dns,mail,ftp,ssh,telnet,smon};

        return Arrays.asList(protos);
    }

    public final static List<RuleTarget> getTargets(String proto){

        return maps.get(proto);
    }

}
