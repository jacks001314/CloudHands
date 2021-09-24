package com.antell.cloudhands.api.packet.tcp.mysql;

import com.antell.cloudhands.api.packet.SessionEntry;
import com.antell.cloudhands.api.packet.tcp.TCPSessionEntry;
import com.antell.cloudhands.api.rule.RuleItem;
import com.antell.cloudhands.api.source.AbstractSourceEntry;
import com.antell.cloudhands.api.utils.Constants;
import com.antell.cloudhands.api.utils.IPUtils;
import com.antell.cloudhands.api.utils.MessagePackUtil;
import com.antell.cloudhands.api.utils.TextUtils;
import com.google.common.base.Preconditions;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.msgpack.core.MessageUnpacker;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class MYSQLSession extends AbstractSourceEntry {

    private SessionEntry sessionEntry;
    private int authState;
    private String version;
    private String user;
    private List<MYSQLSessionEntry> entries;


    public MYSQLSession(MessageUnpacker unpacker) throws IOException {

        sessionEntry = new TCPSessionEntry();
        entries = new ArrayList<>();

        parse(unpacker);
    }

    private void parse(MessageUnpacker unpacker) throws IOException {

        /*parse session entry */
        sessionEntry.parse(unpacker);

        int n = MessagePackUtil.parseMapHeader(unpacker,false);
        Preconditions.checkArgument(n==4,"Invalid mysql session messagePack:"+n);

        setAuthState(MessagePackUtil.parseByte(unpacker));
        setVersion(MessagePackUtil.parseText(unpacker));
        setUser(MessagePackUtil.parseText(unpacker));

        n = MessagePackUtil.parseArrayHeader(unpacker,true);

        for(int i = 0;i<n;i++){

            entries.add(new MYSQLSessionEntry(unpacker));
        }

    }

    public SessionEntry getSessionEntry() {
        return sessionEntry;
    }

    @Override
    public String dataToString() {

        StringBuffer sb = new StringBuffer();

       TextUtils.addText(sb,"authState",authState==0?"ok":"failed");
       TextUtils.addText(sb,"version",version);
       TextUtils.addText(sb,"user",user);


        return sb.toString();
    }

    public String toString(){

        return dataToString();
    }

    @Override
    public XContentBuilder dataToJson(XContentBuilder cb) throws IOException {

        XContentBuilder seCB = cb.startObject("sessionEntry");
        sessionEntry.dataToJson(seCB);
        seCB.endObject();

        cb.field("authState",authState);
        cb.field("version",version);
        cb.field("user",user);

        XContentBuilder cbb = cb.startArray("entries");

        entries.forEach(entry-> {
            try {
                entry.toJson(cbb);
            } catch (IOException e) {
                e.printStackTrace();
            }
        });

        cbb.endArray();


        return cb;
    }

    @Override
    public String getIndexMapping() {

        String mapping = "{" +
                "\"properties\":{" +
                "\"id\":{\"type\":\"keyword\"}," +
                "\"sessionEntry\":{" +
                "\"properties\":{" +
                "\"sessionID\":{\"type\":\"long\"}," +
                "\"protocol\":{\"type\":\"keyword\"}," +
                "\"srcIP\":{\"type\":\"keyword\"}," +
                "\"dstIP\":{\"type\":\"keyword\"}," +
                "\"srcPort\":{\"type\":\"integer\"}," +
                "\"dstPort\":{\"type\":\"integer\"}," +
                "\"reqStartTime\":{\"type\":\"long\"}," +
                "\"resStartTime\":{\"type\":\"long\"}," +
                "\"timeDate\":{\"type\":\"date\",\"format\":\"yyyy-MM-dd HH:mm:ss\"}," +
                "\"reqPackets\":{\"type\":\"long\"}," +
                "\"reqBytes\":{\"type\":\"long\"}," +
                "\"reqPBytes\":{\"type\":\"long\"}," +
                "\"resPackets\":{\"type\":\"long\"}," +
                "\"resBytes\":{\"type\":\"long\"}," +
                "\"resPBytes\":{\"type\":\"long\"}" +
                "}" +
                "}," +
                "\"authState\":{\"type\":\"integer\"}," +
                "\"version\":{\"type\":\"keyword\"}," +
                "\"user\":{\"type\":\"keyword\"}," +
                "\"srcIPLocation\":{" +
                "\"properties\":{" +
                "\"location\":{\"type\":\"keyword\"}," +
                "\"country\":{\"type\":\"keyword\"}," +
                "\"city\":{\"type\":\"keyword\"}," +
                "\"longitude\":{\"type\":\"double\"}," +
                "\"latitude\":{\"type\":\"double\"}" +
                "}" +
                "}," +
                "\"dstIPLocation\":{" +
                "\"properties\":{" +
                "\"location\":{\"type\":\"keyword\"}," +
                "\"country\":{\"type\":\"keyword\"}," +
                "\"city\":{\"type\":\"keyword\"}," +
                "\"longitude\":{\"type\":\"double\"}," +
                "\"latitude\":{\"type\":\"double\"}" +
                "}" +
                "}" +
                "}" +
                "}";
        return mapping;
    }

    @Override
    public String getIndexNamePrefix() {
        return "log_tcp_session_mysql";
    }

    @Override
    public String getIndexDocType() {
        return Constants.ESLOGDOCTYPE;
    }

    @Override
    public String getSrcIP() {
        return IPUtils.ipv4Str(sessionEntry.getReqIP());
    }

    @Override
    public String getDstIP() {
        return IPUtils.ipv4Str(sessionEntry.getResIP());
    }

    public String getProto(){

        return "ssh";
    }

    public long getSrcIPI(){

        return sessionEntry.getReqIP();
    }

    public long getDstIPI(){
        return sessionEntry.getResIP();
    }

    public long getTime(){

        return sessionEntry.getReqStartTime();
    }


    @Override
    public boolean canMatch(String proto) {
        return false;
    }

    @Override
    public String getTargetValue(RuleItem ruleItem) {

        return null;
    }

    public int getAuthState() {
        return authState;
    }

    public void setAuthState(int authState) {
        this.authState = authState;
    }

    public String getVersion() {
        return version;
    }

    public void setVersion(String version) {
        this.version = version;
    }

    public String getUser() {
        return user;
    }

    public void setUser(String user) {
        this.user = user;
    }

    public List<MYSQLSessionEntry> getEntries() {
        return entries;
    }

    public void setEntries(List<MYSQLSessionEntry> entries) {
        this.entries = entries;
    }
}
