package com.antell.cloudhands.api.rule;

import com.antell.cloudhands.api.source.SourceEntry;
import com.antell.cloudhands.api.utils.ByteDataUtils;
import org.elasticsearch.common.xcontent.XContentBuilder;

import java.io.IOException;

public class SourceTest implements SourceEntry {

    private String host;
    private int port;

    public SourceTest(String host,int port){

        this.host = host;
        this.port = port;
    }
    public String getHost() {
        return host;
    }

    public void setHost(String host) {
        this.host = host;
    }

    public int getPort() {
        return port;
    }

    public void setPort(int port) {
        this.port = port;
    }

    @Override
    public String getObjectId() {
        return null;
    }

    @Override
    public String dataToString() {
        return null;
    }

    @Override
    public boolean canMatch(String proto) {
        return proto.equals("tcp");
    }

    @Override
    public String getTargetValue(String target, boolean isHex) {

        if(target.equals("host"))
            return isHex? ByteDataUtils.toHex(host.getBytes()):host;

        if(target.equals("port"))
            return String.format("%d",port);

        return null;
    }

    @Override
    public XContentBuilder dataToJson(XContentBuilder cb) throws IOException {
        return null;
    }

    @Override
    public String getIndexMapping() {
        return null;
    }

    @Override
    public String getIndexNamePrefix() {
        return null;
    }

    @Override
    public String getIndexDocType() {
        return null;
    }

    public static void main(String[] args) throws IOException {

        String path = "F:\\shajf_dev\\antell\\3.4\\CloudHandsJAPI\\src\\main\\java\\com\\antell\\cloudhands\\api\\rule\\rule.json";

        RulePool rulePool = new RulePool(path,null,true,false);

        String host = """
                Host: 91.215.169.111
                User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.108 Safari/537.36
                Content-Length: 20
                Arch: amd64
                Content-Type: application/octet-stream
                Cores: 1
                Mem: 1846
                Os: linux
                Osname: centos
                Osversion: 7.7.1908
                Root: false
                Uuid: 89163e9d-e13c-4b5f-401d-29fd4ecfa412
                Version: 29
                Accept-Encoding: gzip
                """;

        SourceTest sourceTest = new SourceTest(host,8080);

        System.out.println(RuleMatch.match(sourceTest,rulePool));
    }

}
