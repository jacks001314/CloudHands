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
    public String getTargetValue(RuleItem ruleItem) {

        String target = ruleItem.getTarget();
        boolean isHex = ruleItem.isHex();

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

        String path = "F:\\shajf_dev\\antell\\3.4\\CloudHandsJAPIRepo\\src\\main\\java\\com\\antell\\cloudhands\\api\\rule\\rule.json";

        System.out.println(new String(ByteDataUtils.parseHex("6d6574686f64")));
        RulePool rulePool = new RulePool(path,null,true,false);

        String host = "47.103.51.151";
        SourceTest sourceTest = new SourceTest(host,8080);

        System.out.println(RuleMatch.match(sourceTest,rulePool));
    }

}
