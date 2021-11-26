package com.antell.cloudhands.api.packet.udp.dns.record;

import com.antell.cloudhands.api.packet.udp.dns.common.DNSDataInput;
import com.antell.cloudhands.api.packet.udp.dns.common.Name;
import com.antell.cloudhands.api.utils.MessagePackUtil;
import com.antell.cloudhands.api.utils.TextUtils;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.msgpack.core.MessageUnpacker;

import java.io.DataInput;
import java.io.IOException;

/**
 * Start of Authority - describes properties of a zone.
 *
 */

public class SOARecord extends Record {

    private Name host;
    private Name admin;
    private long serial;
    private long refresh;
    private long retry;
    private long expire;
    private long minimum;

    public SOARecord() {
    }

    @Override
    public Record getObject() {
        return new SOARecord();
    }

    @Override
    public void read(DataInput in) throws IOException {
        host = new Name(in);
        admin = new Name(in);
        serial = in.readInt();
        refresh = in.readInt();
        retry = in.readInt();
        expire = in.readInt();
        minimum = in.readInt();
    }

    @Override
    public void read(MessageUnpacker unpacker) throws IOException {

        MessagePackUtil.parseMapHeader(unpacker,true);
        host = new Name(unpacker);
        admin = new Name(unpacker);
        serial = MessagePackUtil.parseLong(unpacker);
        refresh = MessagePackUtil.parseLong(unpacker);
        retry = MessagePackUtil.parseLong(unpacker);
        expire = MessagePackUtil.parseLong(unpacker);
        minimum = MessagePackUtil.parseLong(unpacker);
    }

    @Override
    public void read(DNSDataInput in) throws IOException {

        host = new Name(in);
        admin = new Name(in);
        serial = in.readU32();
        refresh = in.readU32();
        retry = in.readU32();
        expire = in.readU32();
        minimum = in.readU32();
    }

    /**
     * Convert to a String
     */
    @Override
    public String rrToString() {
        StringBuffer sb = new StringBuffer();
        sb.append(host);
        sb.append(" ");
        sb.append(admin);
        sb.append(" ");
        sb.append(serial);
        sb.append(" ");
        sb.append(refresh);
        sb.append(" ");
        sb.append(retry);
        sb.append(" ");
        sb.append(expire);
        sb.append(" ");
        sb.append(minimum);
        return sb.toString();
    }

    @Override
    public XContentBuilder rdataToJson(XContentBuilder cb) throws IOException {

        cb.field("host",host.getName());
        cb.field("admin",admin.getName());
        cb.field("serial",serial);
        cb.field("refresh",refresh);
        cb.field("retry",retry);
        cb.field("expire",expire);
        cb.field("minimum",minimum);

        return cb;
    }

    @Override
    public void rdataToJsonString(StringBuffer sb) {
        sb.append("{");
        TextUtils.addText(sb, "host",host.getName(), true);
        TextUtils.addText(sb, "admin",admin.getName(), true);
        TextUtils.addText(sb, "serial",serial, true);
        TextUtils.addText(sb, "refresh",refresh, true);
        TextUtils.addText(sb, "retry",retry, true);
        TextUtils.addText(sb, "expire",expire, true);
        TextUtils.addText(sb, "minimum",minimum, false);
        sb.append("}");
    }

    /**
     * Returns the primary name server
     */
    public Name
    getHost() {
        return host;
    }

    /**
     * Returns the zone administrator's address
     */
    public Name
    getAdmin() {
        return admin;
    }

    /**
     * Returns the zone's serial number
     */
    public long
    getSerial() {
        return serial;
    }

    /**
     * Returns the zone refresh interval
     */
    public long
    getRefresh() {
        return refresh;
    }

    /**
     * Returns the zone retry interval
     */
    public long
    getRetry() {
        return retry;
    }

    /**
     * Returns the time until a secondary expires a zone
     */
    public long
    getExpire() {
        return expire;
    }

    /**
     * Returns the minimum TTL for records in the zone
     */
    public long
    getMinimum() {
        return minimum;
    }

}
