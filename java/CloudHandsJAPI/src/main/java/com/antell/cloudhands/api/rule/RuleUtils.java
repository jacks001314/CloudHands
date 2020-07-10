package com.antell.cloudhands.api.rule;

import com.antell.cloudhands.api.utils.ByteDataUtils;
import com.antell.cloudhands.api.utils.Content;
import com.antell.cloudhands.api.utils.FileUtils;
import com.antell.cloudhands.api.utils.TextUtils;

import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

public class RuleUtils {

    public static final String targetValue(long v,boolean isHex){

        String value = Long.toString(v);

        if(TextUtils.isEmpty(value))
            return value;

        return isHex? ByteDataUtils.toHex(value):value;
    }

    public static final String targetValue(String value,boolean isHex){

        if(TextUtils.isEmpty(value))
            return value;

        return isHex? ByteDataUtils.toHex(value):value;
    }

    public static final String targetValue(List<String> values,String split,boolean isHex){

        if(values == null||values.size()==0)
            return "";

        int len = values.size();
        if(len ==1)
            return isHex?ByteDataUtils.toHex(values.get(0)):values.get(0);

        StringBuffer sb = new StringBuffer();

        for(int i = 0;i<len;i++){

            String v = isHex?ByteDataUtils.toHex(values.get(i)):values.get(i);

            if(i==len-1){
                sb.append(v);
            }else{
                sb.append(v);
                sb.append(split);
            }
        }
        return sb.toString();
    }

    public static final String fromFile(String fpath,boolean isHex){

        try {
            byte[] data = Files.readAllBytes(Paths.get(fpath));
            return isHex?ByteDataUtils.toHex(data):new String(data, Charset.forName("utf-8"));
        } catch (Exception e) {

            return "";
        }
    }

    public static final String fromFile(String fpath,String target,boolean isHex){

        int maxLen = 0;
        int len = 0;

        if(!FileUtils.isExisted(fpath))
            return "";

        String[] splits = target.split("\\.");

        if(splits.length==2){
            len = Integer.parseInt(splits[1]);
        }else if(splits.length==3){
            maxLen = Integer.parseInt(splits[1]);
            len = Integer.parseInt(splits[2]);
        }

        byte[] data = Content.readBody(fpath,null,maxLen,len);

        return isHex?ByteDataUtils.toHex(data):new String(data,Charset.forName("utf-8"));

    }


}
