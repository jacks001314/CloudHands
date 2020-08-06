package com.antell.cloudhands.api.packet.udp.dns.record;

import com.antell.cloudhands.api.packet.udp.dns.common.DNSDataInput;
import com.antell.cloudhands.api.utils.Base16;
import com.antell.cloudhands.api.utils.MessagePackUtil;
import com.antell.cloudhands.api.utils.Text;
import com.antell.cloudhands.api.utils.TextUtils;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.msgpack.core.MessageUnpacker;

import java.io.ByteArrayOutputStream;
import java.io.DataInput;
import java.io.IOException;

/**
 * NSAP Address Record.
 *
 */

public class NSAPRecord extends Record {

    private byte[] address;

    public NSAPRecord() {
    }

    @Override
    public Record getObject() {
        return new NSAPRecord();
    }

    private static final byte[] checkAndConvertAddress(String address) {
        if (!address.substring(0, 2).equalsIgnoreCase("0x")) {
            return null;
        }
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        boolean partial = false;
        int current = 0;
        for (int i = 2; i < address.length(); i++) {
            char c = address.charAt(i);
            if (c == '.') {
                continue;
            }
            int value = Character.digit(c, 16);
            if (value == -1) {
                return null;
            }
            if (partial) {
                current += value;
                bytes.write(current);
                partial = false;
            } else {
                current = value << 4;
                partial = true;
            }

        }
        if (partial) {
            return null;
        }
        return bytes.toByteArray();
    }


    @Override
    public void read(DataInput in) throws IOException {
        address = Text.readBytes(in,2);
    }

    @Override
    public void read(MessageUnpacker unpacker) throws IOException {

        MessagePackUtil.parseMapHeader(unpacker,true);
        address = MessagePackUtil.parseBin(unpacker);
    }

    @Override
    public void read(DNSDataInput in) throws IOException {

        address = in.readByteArray();
    }

    /**
     * Returns the NSAP address.
     */
    public byte[] getAddress() {


        return address;
    }


    public String rrToString() {
        return "0x" + Base16.toString(address);
    }

    @Override
    public XContentBuilder rdataToJson(XContentBuilder cb) throws IOException {

        cb.field("address",rrToString());
        return cb;
    }

    @Override
    public void rdataToJsonString(StringBuffer sb) {
        StringBuffer sb2 = new StringBuffer();
        sb.append("{");
        TextUtils.addText(sb, "address", rrToString(), false);
        sb.append("}");
    }

}
