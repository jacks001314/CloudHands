package com.antell.cloudhands.api.packet.udp.dns.raw;

import com.antell.cloudhands.api.packet.udp.dns.common.DClass;
import com.antell.cloudhands.api.packet.udp.dns.common.Name;
import com.antell.cloudhands.api.packet.udp.dns.record.Type;
import com.antell.cloudhands.api.utils.ByteDataUtils;
import com.antell.cloudhands.api.utils.MessagePackUtil;
import com.antell.cloudhands.api.utils.TextUtils;
import com.google.common.base.Preconditions;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.msgpack.core.MessageUnpacker;

import java.io.DataInput;
import java.io.IOException;

public class RDNSRecord {

    private Name name;
    private int type;
    private int dclass;
    private long ttl;
    private int pos;

    private byte[] rdata;

    public RDNSRecord(String name,int type,int dclass,long ttl,int pos,byte[] rdata){

        this.name = new Name(name);
        this.type = type;
        this.dclass = dclass;
        this.ttl = ttl;
        this.pos = pos;
        this.rdata = rdata;
    }

    public RDNSRecord(DataInput in) throws IOException {

        name = new Name(in);
        type = in.readUnsignedShort();
        dclass = in.readUnsignedShort();
        ttl = in.readInt();

        int dlen = in.readUnsignedShort();
        rdata = new byte[dlen];

        in.readFully(rdata,0,dlen);

    }

    public RDNSRecord(MessageUnpacker unpacker) throws IOException {

        int n = MessagePackUtil.parseMapHeader(unpacker,true);
        Preconditions.checkArgument(n==6,"Invalid msgpack packet of dns record entry:"+n);
        name = new Name(unpacker);
        type = MessagePackUtil.parseInt(unpacker);
        dclass = MessagePackUtil.parseInt(unpacker);
        ttl = MessagePackUtil.parseLong(unpacker);
        pos = MessagePackUtil.parseInt(unpacker);
        rdata = MessagePackUtil.parseBin(unpacker);

    }

    /**
     * Converts a Record into a Json format
     * */
    public XContentBuilder toJson(XContentBuilder cb) throws IOException {

        cb.field("name",name.toString());
        cb.field("ttl",ttl);
        cb.field("dclassStr", DClass.string(dclass));
        cb.field("dclass",dclass);
        cb.field("typeStr", Type.string(type));
        cb.field("type",type);
        cb.field("pos",pos);
        cb.field("rdata",rdata==null?"".getBytes():rdata);

        return cb;
    }

    public void toJsonString(StringBuffer sb) {
        sb.append("{");
        TextUtils.addText(sb, "name", name.toString(), true);
        TextUtils.addText(sb, "ttl", ttl, true);
        TextUtils.addText(sb, "dclassStr", DClass.string(dclass), true);
        TextUtils.addText(sb, "dclass",dclass, true);
        TextUtils.addText(sb, "typeStr", Type.string(type), true);
        TextUtils.addText(sb, "type", type, false);
        TextUtils.addText(sb,"pos",pos,true);
        TextUtils.addText(sb,"rdata", ByteDataUtils.toHex(rdata));
        sb.append("}");
    }

    /**
     * Converts the rdata portion of a Record into a String representation
     */
    public String rdataToString() {
        return "{}";
    }

    public Name getName() {
        return name;
    }

    public void setName(Name name) {
        this.name = name;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public int getDclass() {
        return dclass;
    }

    public void setDclass(int dclass) {
        this.dclass = dclass;
    }

    public long getTtl() {
        return ttl;
    }

    public void setTtl(long ttl) {
        this.ttl = ttl;
    }

    public byte[] getRdata() {
        return rdata;
    }

    public void setRdata(byte[] rdata) {
        this.rdata = rdata;
    }

    public int getPos() {
        return pos;
    }

    public void setPos(int pos) {
        this.pos = pos;
    }
}
