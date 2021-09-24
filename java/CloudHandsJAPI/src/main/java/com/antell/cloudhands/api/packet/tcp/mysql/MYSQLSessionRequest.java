package com.antell.cloudhands.api.packet.tcp.mysql;

import com.antell.cloudhands.api.utils.MessagePackUtil;
import com.google.common.base.Preconditions;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.msgpack.core.MessageUnpacker;

import java.io.IOException;

public class MYSQLSessionRequest {

    private int opcode;
    private String opname;
    private byte[] data;

    public MYSQLSessionRequest(MessageUnpacker unpacker) throws IOException {

        int n = MessagePackUtil.parseMapHeader(unpacker,true);
        Preconditions.checkArgument(n==3,"Invalid mysql-request session messagePack:"+n);

        setOpcode(MessagePackUtil.parseByte(unpacker));
        setOpname(MessagePackUtil.parseText(unpacker));
        setData(MessagePackUtil.parseBin(unpacker));
    }

    public int getOpcode() {
        return opcode;
    }

    public void setOpcode(int opcode) {
        this.opcode = opcode;
    }

    public String getOpname() {
        return opname;
    }

    public void setOpname(String opname) {
        this.opname = opname;
    }

    public byte[] getData() {
        return data;
    }

    public void setData(byte[] data) {
        this.data = data;
    }

    public void toJson(XContentBuilder req) throws IOException {

        req.field("opcode",opcode);
        req.field("opname",opname);
        req.field("data",data);
    }
}
