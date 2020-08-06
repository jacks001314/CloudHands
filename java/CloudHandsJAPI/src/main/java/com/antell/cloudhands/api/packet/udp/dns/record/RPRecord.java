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
 * Responsible Person Record - lists the mail address of a responsible person
 * and a domain where TXT records are available.
 *
 */

public class RPRecord extends Record {


    private Name mailbox;
    private Name textDomain;

    public RPRecord() {
    }

    @Override
    public Record getObject() {
        return new RPRecord();
    }


    @Override
    public void read(DataInput in) throws IOException {
        mailbox = new Name(in);
        textDomain = new Name(in);
    }

    @Override
    public void read(MessageUnpacker unpacker) throws IOException {

        MessagePackUtil.parseMapHeader(unpacker,true);
        mailbox = new Name(unpacker);
        textDomain = new Name(unpacker);
    }

    @Override
    public void read(DNSDataInput dataInput) throws IOException {
        mailbox = new Name(dataInput);
        this.textDomain = mailbox;
    }

    /**
     * Converts the RP Record to a String
     */
    @Override
    public String rrToString() {
        StringBuffer sb = new StringBuffer();
        sb.append(mailbox);
        sb.append(" ");
        sb.append(textDomain);
        return sb.toString();
    }

    @Override
    public XContentBuilder rdataToJson(XContentBuilder cb) throws IOException {

        cb.field("mailbox",mailbox.toString());
        cb.field("textDomain",textDomain.toString());
        return cb;
    }

    @Override
    public void rdataToJsonString(StringBuffer sb) {
        sb.append("{");
        TextUtils.addText(sb, "mailbox", mailbox.toString(), true);
        TextUtils.addText(sb, "textDomain", textDomain.toString(), false);
        sb.append("}");
    }

    /**
     * Gets the mailbox address of the RP Record
     */
    public Name getMailbox() {
        return mailbox;
    }

    /**
     * Gets the text domain info of the RP Record
     */
    public Name getTextDomain() {
        return textDomain;
    }

}
