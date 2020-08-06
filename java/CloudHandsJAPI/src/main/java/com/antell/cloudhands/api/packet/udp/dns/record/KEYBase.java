
package com.antell.cloudhands.api.packet.udp.dns.record;

import com.antell.cloudhands.api.packet.udp.dns.common.DNSDataInput;
import com.antell.cloudhands.api.utils.*;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.msgpack.core.MessageUnpacker;

import java.io.DataInput;
import java.io.IOException;
import java.security.PublicKey;

/**
 * The base class for KEY/DNSKEY records, which have identical formats
 */

public abstract class KEYBase extends Record {

    protected int flags, proto, alg;
    protected byte[] key;
    protected PublicKey publicKey = null;

    protected KEYBase() {
    }

    @Override
    public void read(DataInput in) throws IOException {
        flags = in.readUnsignedShort();
        proto = in.readUnsignedByte();
        alg = in.readUnsignedByte();

        key = Text.readBytes(in,2);
    }

    @Override
    public void read(DNSDataInput in) throws IOException {

        flags = in.readU16();
        proto = in.readU8();
        alg = in.readU8();
        if (in.remaining() > 0) {
            key = in.readByteArray();
        }

    }

    @Override
    public void read(MessageUnpacker unpacker) throws IOException {

        MessagePackUtil.parseMapHeader(unpacker,true);
        MessagePackUtil.parseMapHeader(unpacker,true);

        flags = MessagePackUtil.parseInt(unpacker);
        proto = MessagePackUtil.parseInt(unpacker);
        alg = MessagePackUtil.parseInt(unpacker);

        key = MessagePackUtil.parseBin(unpacker);
    }
    /**
     * Converts the DNSKEY/KEY Record to a String
     */
    @Override
    public String rrToString() {
        StringBuffer sb = new StringBuffer();
        sb.append(flags);
        sb.append(" ");
        sb.append(proto);
        sb.append(" ");
        sb.append(alg);
        if (key != null) {
            sb.append(" ");
            sb.append(Base64.toString(key));
        }
        return sb.toString();
    }

    @Override
    public XContentBuilder rdataToJson(XContentBuilder cb) throws IOException {

        cb.field("flags",flags);
        cb.field("proto",proto);
        cb.field("alg",alg);
        cb.field("key",key == null?"":Base64.toString(key));
        return cb;
    }

    @Override
    public void rdataToJsonString(StringBuffer sb) {
        sb.append("{");
        TextUtils.addText(sb, "flags", flags, true);
        TextUtils.addText(sb, "proto", proto, true);
        TextUtils.addText(sb, "alg", alg, true);
        TextUtils.addText(sb, "key", key == null?"":Base64.toString(key), false);
        sb.append("}");
    }

    /**
     * Returns the flags describing the key's properties
     */
    public int
    getFlags() {
        return flags;
    }

    /**
     * Returns the protocol that the key was created for
     */
    public int
    getProtocol() {
        return proto;
    }

    /**
     * Returns the key's algorithm
     */
    public int
    getAlgorithm() {
        return alg;
    }

    /**
     * Returns the binary data representing the key
     */
    public byte[]
    getKey() {
        return key;
    }


    /**
     * Returns a PublicKey corresponding to the data in this key.
     *
     * @throws DNSSEC.DNSSECException The key could not be converted.
     */
    public PublicKey
    getPublicKey() throws DNSSEC.DNSSECException {
        if (publicKey != null)
            return publicKey;

        publicKey = DNSSEC.toPublicKey(this);
        return publicKey;
    }

}
