package com.antell.cloudhands.api.packet.udp.dns.record;

import com.antell.cloudhands.api.packet.udp.dns.common.Address;
import com.antell.cloudhands.api.packet.udp.dns.common.DNSDataInput;
import com.antell.cloudhands.api.utils.MessagePackUtil;
import com.antell.cloudhands.api.utils.TextUtils;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.msgpack.core.MessageUnpacker;

import java.io.DataInput;
import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;

/**
 * Address Record - maps a domain name to an Internet address
 */

public class ARecord extends Record {


    private long addr;

    public ARecord() {
    }

    private static int fromArray(byte[] array) {
        return ((array[0] & 0xFF) << 24)
                | ((array[1] & 0xFF) << 16)
                | ((array[2] & 0xFF) << 8)
                | (array[3] & 0xFF);
    }

    private static final byte[] toArray(long addr) {
        byte[] bytes = new byte[4];
        bytes[0] = (byte) ((addr >>> 24) & 0xFF);
        bytes[1] = (byte) ((addr >>> 16) & 0xFF);
        bytes[2] = (byte) ((addr >>> 8) & 0xFF);
        bytes[3] = (byte) (addr & 0xFF);
        return bytes;
    }

    @Override
    public Record getObject() {
        return new ARecord();
    }


    @Override
    public void read(DataInput in) throws IOException {

        addr = in.readInt();
    }

    @Override
    public void read(MessageUnpacker unpacker) throws IOException {

        MessagePackUtil.parseMapHeader(unpacker,true);
        addr = MessagePackUtil.parseLong(unpacker);
    }

    @Override
    public void read(DNSDataInput in) throws IOException {
        addr = fromArray(in.readByteArray(4));
    }

    /**
     * Converts rdata to a String
     */
    @Override
    public String rrToString() {
        return (Address.toDottedQuad(toArray(addr)));
    }

    @Override
    public XContentBuilder rdataToJson(XContentBuilder cb) throws IOException {

        cb.field("address",rrToString());
        return cb;
    }

    @Override
    public void rdataToJsonString(StringBuffer sb) {
        sb.append("{");
        TextUtils.addText(sb, "address", rrToString(), false);
        sb.append("}");
    }

    /**
     * Returns the Internet address
     */
    public InetAddress getAddress() {
        try {
            if (name == null)
                return InetAddress.getByAddress(toArray(addr));
            else
                return InetAddress.getByAddress(name.toString(),
                        toArray(addr));
        } catch (UnknownHostException e) {
            return null;
        }
    }

}
