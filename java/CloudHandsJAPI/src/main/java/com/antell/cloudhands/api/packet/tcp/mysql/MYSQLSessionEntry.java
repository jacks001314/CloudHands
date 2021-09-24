package com.antell.cloudhands.api.packet.tcp.mysql;

import com.antell.cloudhands.api.utils.MessagePackUtil;
import com.google.common.base.Preconditions;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.msgpack.core.MessageUnpacker;

import java.io.IOException;

public class MYSQLSessionEntry {

    private MYSQLSessionRequest request;
    private MYSQLSessionResponse response;

    public MYSQLSessionEntry(MessageUnpacker unpacker) throws IOException {

        int n = MessagePackUtil.parseMapHeader(unpacker,true);
        Preconditions.checkArgument(n==1||n==2,"Invalid mysql-session entry messagePack:"+n);

        this.request = new MYSQLSessionRequest(unpacker);
        if(n == 1){

            this.response = new MYSQLSessionResponse();
        }else{

            this.response = new MYSQLSessionResponse(unpacker);
        }

    }

    public MYSQLSessionRequest getRequest() {
        return request;
    }

    public void setRequest(MYSQLSessionRequest request) {
        this.request = request;
    }

    public MYSQLSessionResponse getResponse() {
        return response;
    }

    public void setResponse(MYSQLSessionResponse response) {
        this.response = response;
    }

    public void toJson(XContentBuilder cbb) throws IOException {

        XContentBuilder req = cbb.startObject("request");
        request.toJson(req);
        req.endObject();

        XContentBuilder res = cbb.startObject("response");
        response.toJson(res);
        res.endObject();
    }
}
