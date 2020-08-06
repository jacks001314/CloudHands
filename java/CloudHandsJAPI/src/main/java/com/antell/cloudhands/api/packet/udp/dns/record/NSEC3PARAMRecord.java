package com.antell.cloudhands.api.packet.udp.dns.record;

import com.antell.cloudhands.api.packet.udp.dns.common.DNSDataInput;
import com.antell.cloudhands.api.utils.Base16;
import com.antell.cloudhands.api.utils.MessagePackUtil;
import com.antell.cloudhands.api.utils.Text;
import com.antell.cloudhands.api.utils.TextUtils;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.msgpack.core.MessageUnpacker;

import java.io.DataInput;
import java.io.IOException;

/**
 * Next SECure name 3 Parameters - this record contains the parameters (hash
 * algorithm, salt, iterations) used for a valid, complete NSEC3 chain present
 * in a zone. Zones signed using NSEC3 must include this record at the zone apex
 * to inform authoritative servers that NSEC3 is being used with the given
 * parameters.
 */

public class NSEC3PARAMRecord extends Record {

    private int hashAlg;
    private int flags;
    private int iterations;
    private byte salt[];

    public NSEC3PARAMRecord() {
    }

    @Override
    public Record getObject() {
        return new NSEC3PARAMRecord();
    }

    @Override
    public void read(DataInput in) throws IOException {
        hashAlg = in.readUnsignedByte();
        flags = in.readUnsignedByte();
        iterations = in.readUnsignedShort();

        salt = Text.readBytes(in,2);
    }

    @Override
    public void read(MessageUnpacker unpacker) throws IOException {

        MessagePackUtil.parseMapHeader(unpacker,true);
        hashAlg = MessagePackUtil.parseInt(unpacker);
        flags = MessagePackUtil.parseInt(unpacker);
        iterations = MessagePackUtil.parseInt(unpacker);
        salt = MessagePackUtil.parseBin(unpacker);
    }

    @Override
    public void read(DNSDataInput in) throws IOException {

        hashAlg = in.readU8();
        flags = in.readU8();
        iterations = in.readU16();

        int salt_length = in.readU8();
        if (salt_length > 0) {
            salt = in.readByteArray(salt_length);
        } else {
            salt = null;
        }
    }

    /**
     * Converts rdata to a String
     */
    @Override
    public String rrToString() {
        StringBuffer sb = new StringBuffer();
        sb.append(hashAlg);
        sb.append(' ');
        sb.append(flags);
        sb.append(' ');
        sb.append(iterations);
        sb.append(' ');
        if (salt == null)
            sb.append('-');
        else
            sb.append(Base16.toString(salt));

        return sb.toString();
    }

    @Override
    public XContentBuilder rdataToJson(XContentBuilder cb) throws IOException {

        cb.field("hashAlg",hashAlg);
        cb.field("flags",flags);
        cb.field("iterations",iterations);
        cb.field("salt",salt == null?"":Base16.toString(salt));

        return cb;
    }

    @Override
    public void rdataToJsonString(StringBuffer sb) {
        sb.append("{");
        TextUtils.addText(sb, "hashAlg", hashAlg, true);
        TextUtils.addText(sb, "flags", flags, true);
        TextUtils.addText(sb, "iterations", iterations, true);
        TextUtils.addText(sb, "salt",salt == null?"":Base16.toString(salt), false);
        sb.append("}");
    }

    /**
     * Returns the hash algorithm
     */
    public int getHashAlgorithm() {
        return hashAlg;
    }

    /**
     * Returns the flags
     */
    public int getFlags() {
        return flags;
    }

    /**
     * Returns the number of iterations
     */
    public int getIterations() {
        return iterations;
    }

    /**
     * Returns the salt
     */
    public byte[] getSalt() {
        return salt;
    }


}
