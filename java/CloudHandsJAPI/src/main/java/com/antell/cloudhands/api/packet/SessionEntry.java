package com.antell.cloudhands.api.packet;

import com.antell.cloudhands.api.BinDataInput;
import com.antell.cloudhands.api.DataDump;
import com.antell.cloudhands.api.MsgPackDataInput;
import com.antell.cloudhands.api.rule.RuleConstants;
import com.antell.cloudhands.api.rule.RuleItem;
import com.antell.cloudhands.api.rule.RuleUtils;
import com.antell.cloudhands.api.sink.es.ESIndexable;
import com.antell.cloudhands.api.utils.*;
import org.elasticsearch.common.xcontent.XContentBuilder;

import java.io.DataInput;
import java.io.IOException;
import java.nio.charset.CharacterCodingException;

/**
 * Created by dell on 2018/4/17.
 */
public abstract class SessionEntry implements MsgPackDataInput,BinDataInput, ESIndexable,DataDump{

    private boolean isTimeout;
    private int timeoutTV;
    private long sessionID;
    private long protocolID;
    private String protocol;

    private String portScan;
    private String sdp;
    private boolean isIPV6;

    private final SessionEndPoint req;
    private final SessionEndPoint res;


     public SessionEntry() {
        this.req = new SessionEndPoint();
        this.res = new SessionEndPoint();
    }

    public String getSessionTargetValue(RuleItem ruleItem){

         String target = ruleItem.getTarget();
         boolean isHex = ruleItem.isHex();

        ByteData reqByteData = req.getContent();
        ByteData resByteData = res.getContent();

        if(target.equals(RuleConstants.srcIP)) {
            return RuleUtils.targetValue(IPUtils.ipv4Str(getReqIP()),isHex);
        }
        if(target.equals(RuleConstants.dstIP)) {
            return RuleUtils.targetValue(IPUtils.ipv4Str(getResIP()),isHex);
        }

        if(target.equals(RuleConstants.srcPort)) {
            return RuleUtils.targetValue(getReqPort(),isHex);
        }

        if(target.equals(RuleConstants.dstPort)) {
            return RuleUtils.targetValue(getResPort(),isHex);
        }
        if(target.equals(RuleConstants.reqDataSize)){


            return RuleUtils.targetValue(reqByteData==null?0:reqByteData.getDataSize(),isHex);
        }

        if(target.equals(RuleConstants.resDataSize)){
            return RuleUtils.targetValue(resByteData==null?0:resByteData.getDataSize(),isHex);
        }

        if(target.equals(RuleConstants.reqData)){

            return RuleUtils.getData(reqByteData.getData(),0,ruleItem.getOffset(),ruleItem.getLen(),ruleItem.isHex());
        }

        if(target.equals(RuleConstants.resData)){

            return RuleUtils.getData(resByteData.getData(),0,ruleItem.getOffset(),ruleItem.getLen(),ruleItem.isHex());
        }

        return null;
    }

    protected byte[] readBytes(DataInput in) throws IOException {

        long dataSize = in.readLong();

        byte[] data = new byte[(int) dataSize];

        in.readFully(data, 0, (int) dataSize);
        return data;
    }

    protected String getsrcIPStr(){

         if(isIPV6)
             return IPUtils.ipv6Str(getReqAddr());

         return IPUtils.ipv4Str(getReqIP());
    }
    protected String getdstIPStr(){

        if(isIPV6)
            return IPUtils.ipv6Str(getResAddr());

        return IPUtils.ipv4Str(getResIP());
    }

    @Override
    public XContentBuilder dataToJson(XContentBuilder cb) throws IOException {

        cb.field("sessionID",getSessionID());
        cb.field("protocol", protocol);
        cb.field("isIPV6",isIPV6?1:0);
        cb.field("srcIP", getsrcIPStr());
        cb.field("dstIP",getdstIPStr());
        cb.field("srcPort",getReqPort());
        cb.field("dstPort",getResPort());
        cb.field("reqStartTime", getReqStartTime());
        cb.field("reqLastTime", getReqLastTime());
        cb.field("resStartTime", getResStartTime());
        cb.field("resLastTime", getResLastTime());
        cb.field("timeDate", DateUtils.format(getReqStartTime()));
        cb.field("reqPackets",getReqPackets());
        cb.field("reqBytes",getReqBytes());

        cb.field("resPackets",getResPackets());
        cb.field("resBytes",getResBytes());


        return cb;
    }

    public String dataToJsonString() {

        StringBuffer sb = new StringBuffer("{");
        TextUtils.addText(sb, "sessionID", getSessionID(), true);
        TextUtils.addText(sb, "protocol", protocol, true);
        TextUtils.addText(sb, "isIPV6", isIPV6?1:0, true);
        TextUtils.addText(sb, "srcIP", getsrcIPStr(), true);
        TextUtils.addText(sb, "dstIP", getdstIPStr(), true);
        TextUtils.addText(sb, "srcPort", getReqPort(), true);
        TextUtils.addText(sb, "dstPort", getResPort(), true);
        TextUtils.addText(sb, "reqStartTime", getReqStartTime(), true);
        TextUtils.addText(sb, "reqLastTime", getReqLastTime(), true);
        TextUtils.addText(sb, "resStartTime", getResStartTime(), true);
        TextUtils.addText(sb, "resLastTime", getResLastTime(), true);
        TextUtils.addText(sb, "timeDate", DateUtils.format(getReqStartTime()), true);
        TextUtils.addText(sb, "reqPackets", getReqPackets(), true);
        TextUtils.addText(sb, "reqBytes", getReqBytes(), true);
        TextUtils.addText(sb, "resPackets",getResPackets(), true);
        TextUtils.addText(sb, "resBytes",getResBytes(), false);
        sb.append("}");

        return sb.toString();
    }

    @Override
    public String dataToString() {

        StringBuffer sb = new StringBuffer();

        sb.append("TCP.Session.Entry.info:\n\n");

        TextUtils.addText(sb,"proto",Constants.protoID2String[(int)getProtocolID()]);
        TextUtils.addLong(sb,"sessionID",getSessionID());
        TextUtils.addText(sb,"srcIP", IPUtils.ipv4Str(getReqIP()));
        TextUtils.addText(sb,"dstIP", IPUtils.ipv4Str(getResIP()));
        TextUtils.addInt(sb,"srcPort",getReqPort());
        TextUtils.addInt(sb,"dstPort",getResPort());
        TextUtils.addText(sb,"reqStartTime", DateUtils.format(getReqStartTime()));
        TextUtils.addText(sb,"reqLastTime", DateUtils.format(getReqLastTime()));
        TextUtils.addText(sb,"resStartTime", DateUtils.format(getResStartTime()));
        TextUtils.addText(sb,"resLastTime", DateUtils.format(getResLastTime()));
        TextUtils.addLong(sb,"reqPackets",getReqPackets());
        TextUtils.addLong(sb,"reqBytes",getReqBytes());

        TextUtils.addLong(sb,"resPackets",getResPackets());
        TextUtils.addLong(sb,"resBytes",getResBytes());


        return sb.toString();
    }

    public long getSessionID() {
        return sessionID;
    }

    public void setSessionID(long sessionID) {
        this.sessionID = sessionID;
    }

    public int getTimeoutTV() {
        return timeoutTV;
    }

    public void setTimeoutTV(int timeoutTV) {
        this.timeoutTV = timeoutTV;
    }

    public boolean isTimeout() {
        return isTimeout;
    }

    public void setTimeout(boolean timeout) {
        isTimeout = timeout;
    }

    public void setReqAddr(byte[] addr){

        req.setAddr(addr);
    }
    public byte[] getReqAddr(){

        return req.getAddr();
    }

    public void setResAddr(byte[] addr){

        res.setAddr(addr);
    }
    public byte[] getResAddr(){

        return res.getAddr();
    }

    public void setReqIP(long ip){

        req.setIp(ip);
    }

    public long getReqIP(){

        return req.getIp();
    }

    public void setReqPort(int port){

        req.setPort(port);
    }

    public int getReqPort(){

        return req.getPort();
    }

    public void setReqContent(ByteData content){

        req.setContent(content);
    }

    public ByteData getReqContent(){

        return req.getContent();
    }

    public void setReqPackets(long packets){

        req.setPackets(packets);
    }

    public long getReqPackets(){

        return req.getPackets();
    }

    public void setReqBytes(long bytes){

        req.setBytes(bytes);
    }

    public long getReqBytes(){

        return req.getBytes();
    }

    public void setReqStartTime(long startTime){

        req.setStartTime(startTime);
    }

    public long getReqStartTime(){
        return req.getStartTime();
    }

    public void setReqLastTime(long lastTime){

        req.setLastTime(lastTime);
    }

    public long getReqLastTime(){

        return req.getLastTime();
    }

    public void setResIP(long ip){

        res.setIp(ip);
    }

    public long getResIP(){

        return res.getIp();
    }

    public void setResPort(int port){

        res.setPort(port);
    }

    public int getResPort(){

        return res.getPort();
    }

    public void setResContent(ByteData content){

        res.setContent(content);
    }

    public ByteData getResContent(){

        return res.getContent();
    }


    public void setResPackets(long packets){

        res.setPackets(packets);
    }
    public long getResPackets(){

        return res.getPackets();
    }

    public void setResBytes(long bytes){

        res.setBytes(bytes);
    }

    public long getResBytes(){

        return res.getBytes();
    }

    public void setResStartTime(long startTime){

        res.setStartTime(startTime);
    }

    public long getResStartTime(){
        return res.getStartTime();
    }

    public void setResLastTime(long lastTime){

        res.setLastTime(lastTime);
    }

    public long getResLastTime(){

        return res.getLastTime();
    }

    public long getProtocolID() {
        return protocolID;
    }

    public void setProtocolID(long protocolID) {
        this.protocolID = protocolID;
    }

    public String entryToString() {

        StringBuffer sb = new StringBuffer();
        TextUtils.addLong(sb,"SessionID",sessionID);
        TextUtils.addInt(sb,"TimeoutTV",timeoutTV);
        TextUtils.addText(sb,"isTimeout",isTimeout?"YES":"NO");
        sb.append("Dump Req Endpoint Informath:\n");
        sb.append(req.dataToString());
        sb.append("Dump Res Endpoint Informath:\n");
        sb.append(res.dataToString());

        return sb.toString();
    }

    public String getProtocol() {
        return protocol;
    }

    public void setProtocol(String protocol) {
        this.protocol = protocol;
    }

    public String getPortScan() {
        return portScan;
    }

    public void setPortScan() {

        StringBuffer sb = new StringBuffer();
        sb.append(getReqIP());
        sb.append("|");
        sb.append(getResIP());

        this.portScan = sb.toString();
    }

    public String getSdp() {
        return sdp;
    }

    public void setSdp() {

        StringBuffer sb = new StringBuffer();
        sb.append(getReqIP());
        sb.append("|");
        sb.append(getResIP());
        sb.append("|");
        sb.append(getResPort());

        this.sdp = sb.toString();
    }

    public boolean isIPV6() {
        return isIPV6;
    }

    public void setIPV6(boolean IPV6) {
        isIPV6 = IPV6;
    }
}
