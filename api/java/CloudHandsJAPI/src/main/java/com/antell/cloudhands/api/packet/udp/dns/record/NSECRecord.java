package com.antell.cloudhands.api.packet.udp.dns.record;

import com.antell.cloudhands.api.packet.udp.dns.common.DNSDataInput;
import com.antell.cloudhands.api.packet.udp.dns.common.Name;
import com.antell.cloudhands.api.packet.udp.dns.common.TypeBitmap;
import com.antell.cloudhands.api.utils.MessagePackUtil;
import com.antell.cloudhands.api.utils.Text;
import com.antell.cloudhands.api.utils.TextUtils;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.msgpack.core.MessageUnpacker;

import java.io.DataInput;
import java.io.IOException;

/**
 * Next SECure name - this record contains the following name in an
 * ordered list of names in the zone, and a set of types for which
 * records exist for this name.  The presence of this record in a response
 * signifies a negative response from a DNSSEC-signed zone.
 * <p>
 * This replaces the NXT record.
 *
 */

public class NSECRecord extends Record {

    private Name next;
    private TypeBitmap types;

    public NSECRecord() {
    }

    @Override
    public Record getObject() {
        return new NSECRecord();
    }

    @Override
    public void read(DataInput in) throws IOException {
        next = new Name(in);
        types = new TypeBitmap( Text.readBytes(in,2));
    }

    @Override
    public void read(MessageUnpacker unpacker) throws IOException {
        MessagePackUtil.parseMapHeader(unpacker,true);
        next = new Name(unpacker);
        types = new TypeBitmap(MessagePackUtil.parseBin(unpacker));
    }

    @Override
    public void read(DNSDataInput in) throws IOException {
        next = new Name(in);
        types = new TypeBitmap();
    }

    /**
     * Converts rdata to a String
     */
    @Override
    public String rrToString() {
        StringBuffer sb = new StringBuffer();
        sb.append(next);
        if (!types.empty()) {
            sb.append(' ');
            sb.append(types.toString());
        }
        return sb.toString();
    }

    @Override
    public XContentBuilder rdataToJson(XContentBuilder cb) throws IOException {

        cb.field("next",next);
        cb.field("types",types.empty()?"":types.toString());
        return cb;
    }

    @Override
    public void rdataToJsonString(StringBuffer sb) {
        sb.append("{");
        TextUtils.addText(sb, "next", next.toString(), true);
        TextUtils.addText(sb, "types", types.empty()?"":types.toString(), false);
        sb.append("}");
    }

    /**
     * Returns the next name
     */
    public Name getNext() {
        return next;
    }

    /**
     * Returns the set of types defined for this name
     */
    public int[] getTypes() {
        return types.toArray();
    }

    /**
     * Returns whether a specific type is in the set of types.
     */
    public boolean hasType(int type) {
        return types.contains(type);
    }

}
