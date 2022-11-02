
package com.antell.cloudhands.api.packet.udp.dns.record;

import com.antell.cloudhands.api.packet.udp.dns.common.DNSDataInput;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.msgpack.core.MessageUnpacker;

import java.io.DataInput;
import java.io.IOException;

/**
 * A class implementing Records with no data; that is, records used in
 * the question section of messages and meta-records in dynamic update.
 */

class EmptyRecord extends Record {


    public EmptyRecord() {
    }

    @Override
    public Record getObject() {
        return new EmptyRecord();
    }

    @Override
    public void read(DataInput in) throws IOException {
    }

    @Override
    public void read(MessageUnpacker unpacker) throws IOException {

    }

    @Override
    public void read(DNSDataInput dataInput) throws IOException {

    }


    @Override
    public String rrToString() {
        return "";
    }

    @Override
    public XContentBuilder rdataToJson(XContentBuilder cb) throws IOException {
        return cb;
    }

    @Override
    public void rdataToJsonString(StringBuffer sb) {
        sb.append("{}");
    }

}