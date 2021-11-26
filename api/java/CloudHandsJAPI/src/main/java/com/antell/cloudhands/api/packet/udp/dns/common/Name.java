
package com.antell.cloudhands.api.packet.udp.dns.common;

import com.antell.cloudhands.api.utils.MessagePackUtil;
import com.antell.cloudhands.api.utils.Text;
import com.google.common.base.Preconditions;
import org.apache.kafka.common.protocol.types.Field;
import org.msgpack.core.MessageUnpacker;

import java.io.DataInput;
import java.io.IOException;

/**
 * A representation of a domain name.  It may either be absolute (fully
 * qualified) or relative.
 */

public class Name implements Comparable {

    /* The name data */
    private String name;

    private int length;
    private int labels;

    /* Precomputed hashcode. */
    private int hashcode;

    private static final int LABEL_NORMAL = 0;
    private static final int LABEL_COMPRESSION = 0xC0;
    private static final int LABEL_MASK = 0xC0;
    /** The maximum length of a Name */
    private static final int MAXNAME = 255;

    /** The maximum length of a label a Name */
    private static final int MAXLABEL = 63;

    /** The maximum number of labels in a Name */
    private static final int MAXLABELS = 128;

    /** The maximum number of cached offsets */
    private static final int MAXOFFSETS = 8;

    public Name(String name){

        this.name = name;
        this.length = name==null?0:name.length();

    }

    public Name(DNSDataInput in) throws ParseException {

        StringBuffer sb = new StringBuffer();

        int len;
        boolean done = false;
        int pos;
        int base_pos = in.getBase_pos();
        boolean savedState = false;

        while (!done&&in.remaining()>0) {
            len = in.readU8();
            switch (len & LABEL_MASK) {
                case LABEL_NORMAL:

                    if (len == 0) {
                        done = true;
                    } else {
                        sb.append(new String(in.readByteArray(len)));
                        sb.append(".");
                    }

                    break;
                case LABEL_COMPRESSION:
                    pos = in.readU8();
                    pos += (len & ~LABEL_MASK) << 8;

                    if(base_pos == 0||pos<=base_pos){
                        done = true;
                    }
                    else{

                        pos = pos-base_pos-1;
                        if (pos >= in.current() - 2) {
                            done = true;
                        }
                        if (!savedState) {
                            in.save();
                            savedState = true;
                        }
                        in.jump(pos);
                    }
                    break;
                default:
                    break;
            }
        }

        if (savedState) {
            in.restore();
        }

        String sbStr = sb.toString();
        if(sbStr.endsWith("."))
            this.name = sbStr.substring(0,sbStr.length()-1);
        else
            this.name = sbStr;

        //this.name = sb.toString();
        this.length = name.length();
    }

    public Name(DataInput in) throws IOException {

        labels = in.readUnsignedShort();
        name = Text.readString(in,2);
        length = name.length();

    }

    public Name(MessageUnpacker unpacker) throws IOException {

        int n = MessagePackUtil.parseMapHeader(unpacker,true);
        Preconditions.checkArgument(n==2,"Invalid msgpack packet of udp session name entry:"+n);

        labels = MessagePackUtil.parseInt(unpacker);
        name = MessagePackUtil.parseText(unpacker);
        length = name.length();
    }
    /**
     * Is this name a wildcard?
     */
    public boolean isWild() {
        if (labels == 0)
            return false;
        return (name.charAt(0) ==  (byte)1 && name.charAt(1) == (byte) '*');
    }

    /**
     * Is this name absolute?
     */
    public boolean isAbsolute() {
        if (labels == 0)
            return false;
        return name.charAt(labels - 1) == 0;
    }



    /**
     * Convert the nth label in a Name to a String
     *
     * @param n The label to be converted to a (printable) String.  The first
     *          label is 0.
     */
    public String getLabelString(int n) {

        if(n<0)
            return null;

        String[] splits = name.split(".");

        if(splits == null ||splits.length<=n)
            return null;

        return splits[n];
    }

    /**
     * Are these two Names equivalent?
     */
    @Override
    public boolean equals(Object arg) {
        if (arg == this)
            return true;
        if (arg == null || !(arg instanceof Name))
            return false;
        Name d = (Name) arg;
        if (d.hashcode == 0)
            d.hashCode();
        if (hashcode == 0)
            hashCode();

        if (d.hashcode != hashcode)
            return false;

        if (d.getLabels() != labels)
            return false;

        if(d.getLength()!=length)
            return false;

        String dname = d.getName();

        return name.equalsIgnoreCase(dname);
    }

    /**
     * Computes a hashcode based on the value
     */
    @Override
    public int hashCode() {
        if (hashcode != 0)
            return (hashcode);
        int code = 0;

        String lname = name.toLowerCase();

        for (int i = 0; i < lname.length(); i++)
            code += ((code << 3) + lname.charAt(i));

        hashcode = code;
        return hashcode;
    }

    /**
     * Compares this Name to another Object.
     *
     * @param o The Object to be compared.
     * @return The value 0 if the argument is a name equivalent to this name;
     * a value less than 0 if the argument is less than this name in the canonical
     * ordering, and a value greater than 0 if the argument is greater than this
     * name in the canonical ordering.
     * @throws ClassCastException if the argument is not a Name.
     */
    @Override
    public int compareTo(Object o) {
        Name arg = (Name) o;

        if (this == arg)
            return (0);

        return name.toLowerCase().compareTo(arg.getName().toLowerCase());
    }

    public int getLength() {
        return length;
    }

    public void setLength(int length) {
        this.length = length;
    }

    public int getLabels() {
        return labels;
    }

    public void setLabels(int labels) {
        this.labels = labels;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getName(){

        return name;
    }

    public String toString(){

        return name;
    }
}
