package com.antell.cloudhands.api.packet.udp.dns.record;

import com.antell.cloudhands.api.packet.udp.dns.common.DNSDataInput;
import com.antell.cloudhands.api.packet.udp.dns.common.TypeBitmap;
import com.antell.cloudhands.api.utils.*;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.msgpack.core.MessageUnpacker;

import java.io.DataInput;
import java.io.IOException;

/**
 * Next SECure name 3 - this record contains the next hashed name in an
 * ordered list of hashed names in the zone, and a set of types for which
 * records exist for this name. The presence of this record in a response
 * signifies a negative response from a DNSSEC-signed zone.
 * <p>
 * This replaces the NSEC and NXT records, when used.
 *
 */

public class NSEC3Record extends Record {

    public static class Flags {
        /**
         * NSEC3 flags identifiers.
         */

        private Flags() {
        }

        /**
         * Unsigned delegation are not included in the NSEC3 chain.
         */
        public static final int OPT_OUT = 0x01;
    }

    public static class Digest {
        private Digest() {
        }

        /**
         * SHA-1
         */
        public static final int SHA1 = 1;
    }

    public static final int SHA1_DIGEST_ID = Digest.SHA1;

    private static final long serialVersionUID = -7123504635968932855L;

    private int hashAlg;
    private int flags;
    private int iterations;
    private byte[] salt;
    private byte[] next;
    private TypeBitmap types;

    private static final Base32 b32 = new Base32(Base32.Alphabet.BASE32HEX,
            false, false);

    public NSEC3Record() {
    }

    @Override
    public Record getObject() {
        return new NSEC3Record();
    }

    @Override
    public void read(DataInput in) throws IOException {

        hashAlg = in.readUnsignedByte();
        flags = in.readUnsignedByte();
        iterations = in.readUnsignedShort();

        salt = Text.readBytes(in,2);
        next = Text.readBytes(in,2);

        types = new TypeBitmap( Text.readBytes(in,2));
    }

    @Override
    public void read(MessageUnpacker unpacker) throws IOException {

        MessagePackUtil.parseMapHeader(unpacker,true);
        hashAlg = MessagePackUtil.parseInt(unpacker);
        flags = MessagePackUtil.parseInt(unpacker);
        iterations = MessagePackUtil.parseInt(unpacker);
        salt = MessagePackUtil.parseBin(unpacker);
        next = MessagePackUtil.parseBin(unpacker);
        types = new TypeBitmap(MessagePackUtil.parseBin(unpacker));
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

        int next_length = in.readU8();
        next = in.readByteArray(next_length);
        types = new TypeBitmap(in);
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
        sb.append(' ');
        sb.append(b32.toString(next));

        if (!types.empty()) {
            sb.append(' ');
            sb.append(types.toString());
        }

        return sb.toString();
    }

    @Override
    public XContentBuilder rdataToJson(XContentBuilder cb) throws IOException {

        cb.field("hashAlg",hashAlg);
        cb.field("flags",flags);
        cb.field("iterations",iterations);
        cb.field("salt",salt == null?"":Base16.toString(salt));
        cb.field("next",b32.toString(next));
        cb.field("types",types.empty()?"":types.toString());

        return cb;
    }

    @Override
    public void rdataToJsonString(StringBuffer sb) {
        sb.append("{");
        TextUtils.addText(sb, "hashAlg", hashAlg, true);
        TextUtils.addText(sb, "flags", flags, true);
        TextUtils.addText(sb, "iterations", iterations, true);
        TextUtils.addText(sb, "salt",salt == null?"":Base16.toString(salt), true);
        TextUtils.addText(sb, "next", b32.toString(next), true);
        TextUtils.addText(sb, "types", types.empty()?"":types.toString(), false);
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

    /**
     * Returns the next hash
     */
    public byte[] getNext() {
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
