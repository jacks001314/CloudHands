package com.antell.cloudhands.api.packet.tcp.mysql;

import com.antell.cloudhands.api.utils.MessagePackUtil;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.msgpack.core.MessageUnpacker;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class MYSQLSessionResponse {

    private List<MYSQLSessionResponseEntry> responseEntries;

    public MYSQLSessionResponse(MessageUnpacker unpacker) throws IOException {

        responseEntries = new ArrayList<>();

        int n = MessagePackUtil.parseArrayHeader(unpacker,true);

        for(int i = 0;i<n;i++){

            responseEntries.add(new MYSQLSessionResponseEntry(unpacker));
        }
    }

    public  MYSQLSessionResponse(){

        responseEntries = new ArrayList<>();
    }

    public void toJson(XContentBuilder res) throws IOException {

        XContentBuilder cbb = res.startArray("responseEntries");
        responseEntries.forEach(entry-> {
            entry.toJson(cbb);
        });

        cbb.endArray();
    }
}
