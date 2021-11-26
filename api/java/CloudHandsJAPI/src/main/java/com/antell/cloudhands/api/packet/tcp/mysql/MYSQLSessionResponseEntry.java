package com.antell.cloudhands.api.packet.tcp.mysql;

import com.antell.cloudhands.api.utils.MessagePackUtil;
import com.google.common.base.Preconditions;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.msgpack.core.MessageUnpacker;

import java.io.IOException;

public class MYSQLSessionResponseEntry {

    private int code;
    private String cname;
    private int seq;
    private byte[] data;

    public MYSQLSessionResponseEntry(MessageUnpacker unpacker) throws IOException {

        int n = MessagePackUtil.parseMapHeader(unpacker,true);
        Preconditions.checkArgument(n==4,"Invalid mysql-response entry messagePack:"+n);

        setCode(MessagePackUtil.parseByte(unpacker));
        setSeq(MessagePackUtil.parseByte(unpacker));
        setCname(MessagePackUtil.parseText(unpacker));
        setData(MessagePackUtil.parseBin(unpacker));
    }

    public int getCode() {
        return code;
    }

    public void setCode(int code) {
        this.code = code;
    }

    public String getCname() {
        return cname;
    }

    public void setCname(String cname) {
        this.cname = cname;
    }

    public int getSeq() {
        return seq;
    }

    public void setSeq(int seq) {
        this.seq = seq;
    }

    public byte[] getData() {
        return data;
    }

    public void setData(byte[] data) {
        this.data = data;
    }

    public void toJson(XContentBuilder cbb) throws IOException {

        cbb.field("code",code);
        cbb.field("cname",cname);
        cbb.field("seq",seq);
        cbb.field("data",data);

    }
}
