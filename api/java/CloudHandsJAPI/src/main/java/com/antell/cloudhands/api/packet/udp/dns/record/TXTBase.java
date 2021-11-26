package com.antell.cloudhands.api.packet.udp.dns.record;

import com.antell.cloudhands.api.packet.udp.dns.common.DNSDataInput;
import com.antell.cloudhands.api.utils.MessagePackUtil;
import com.antell.cloudhands.api.utils.Text;
import com.antell.cloudhands.api.utils.TextUtils;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.msgpack.core.MessageUnpacker;

import java.io.DataInput;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Implements common functionality for the many record types whose format
 * is a list of strings.
 */

public abstract class TXTBase extends Record {

    protected List<String> strings;

    protected TXTBase() {
    }

    @Override
    public void read(DNSDataInput in) throws IOException {

        strings = new ArrayList<>(2);
        while (in.remaining() > 0) {
            byte[] b = in.readCountedString();
            strings.add(byteArrayToString(b,false));
        }
    }

    @Override
    public void read(DataInput in) throws IOException {

        strings = new ArrayList<>();
        int n = in.readUnsignedShort();

        for(int i= 0;i<n;i++){

            String text = Text.readString(in,2);
            strings.add(text);
        }

    }

    @Override
    public void read(MessageUnpacker unpacker) throws IOException {

        MessagePackUtil.parseMapHeader(unpacker,true);
        int n = MessagePackUtil.parseArrayHeader(unpacker,true);
        strings = new ArrayList<>();
        for(int i= 0;i<n;i++){
            strings.add(MessagePackUtil.parseText2(unpacker));
        }

    }
    /**
     * converts to a String
     */
    @Override
    public String rrToString() {
        StringBuffer sb = new StringBuffer();

        for(String text :strings){

            sb.append(text);
            sb.append(" ");
        }

        return sb.toString();
    }

    @Override
    public XContentBuilder rdataToJson(XContentBuilder cb) throws IOException {

        cb.field("strings",strings);
        return cb;
    }

    @Override
    public void rdataToJsonString(StringBuffer sb) {
        sb.append("{");
        TextUtils.addText(sb, "strings", strings, false);
        sb.append("}");
    }

    /**
     * Returns the text strings
     *
     * @return A list of Strings corresponding to the text strings.
     */
    public List getStrings() {
        return strings;
    }

    /**
     * Returns the text strings
     *
     * @return A list of byte arrays corresponding to the text strings.
     */
    public List getStringsAsByteArrays() {
        return strings;
    }


}
