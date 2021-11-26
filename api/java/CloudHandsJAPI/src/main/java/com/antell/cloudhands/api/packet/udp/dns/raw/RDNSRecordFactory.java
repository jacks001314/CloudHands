package com.antell.cloudhands.api.packet.udp.dns.raw;

import com.antell.cloudhands.api.packet.udp.dns.common.BasicDNSDataInput;
import com.antell.cloudhands.api.packet.udp.dns.record.*;
import org.elasticsearch.common.Strings;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.elasticsearch.common.xcontent.XContentFactory;
import org.springframework.util.Base64Utils;

import java.io.IOException;

public class RDNSRecordFactory {


    public static Record create(RDNSRecord rdnsRecord){

        Record proto, rec;

        proto = Type.getProto(rdnsRecord.getType());
        if(proto==null)
            return null;

        rec = proto.getObject();

        rec.setType(rdnsRecord.getType());
        rec.setDclass(rdnsRecord.getDclass());
        rec.setTTL(rdnsRecord.getTtl());
        rec.setName(rdnsRecord.getName());

        byte[] rdata = rdnsRecord.getRdata();

        if(rdata!=null&&rdata.length>0) {

            try {
                rec.read(new BasicDNSDataInput(rdata,rdnsRecord.getPos()));
            } catch (Exception e) {
                e.printStackTrace();

            }
        }

        return rec;
    }

    public static void main(String[] args) throws IOException {

        String b = "AngyA2Nkbgt0ZW5jZW50aXB2NsAX";
        byte[] rdata =Base64Utils.decodeFromString(b);

        String nn = new String(rdata);

        RDNSRecord rdnsRecord = new RDNSRecord("dev",Type.CNAME,1,143,58,rdata);

        Record record = create(rdnsRecord);

        XContentBuilder cb = XContentFactory.jsonBuilder();
        cb.startObject();
        record.rdataToJson(cb);
        cb.endObject();

        System.out.println(Strings.toString(cb));
    }
}
