package com.antell.cloudhands.api.utils;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import org.springframework.util.Base64Utils;

import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Base64;

public class GsonUtils {

    public static final <T> T loadFromJsonFile(String path,Class<T> type) throws IOException {

        Gson gson = new Gson();
        if(Files.exists(Paths.get(path))){
            return gson.fromJson(Files.newBufferedReader(Paths.get(path)),type);
        }else {
            return null;
        }
    }
    public static final <T> T loadConfigFromJsonFile(String path,Class<T> type) throws IOException{

        return loadFromJsonFile(path,type);
    }


    public static final <T> T loadConfigFromJson(String json,Class<T> type) throws IOException {

        Gson gson = new Gson();

        return gson.fromJson(json,type);
    }


    public static String toJson(Object object,boolean isEncode){

        Gson gson = new Gson();
        String res = gson.toJson(object);

        return isEncode? String.valueOf(Base64Utils.encode(res.getBytes())) :res;
    }

    private static void writeJson(Gson gson,Object object,String path){

        String json = gson.toJson(object);

        try {

            Path fpath = Paths.get(path);
            Path ppath = fpath.getParent();


            if(!Files.exists(ppath)){
                Files.createDirectories(ppath);
            }

            Files.deleteIfExists(fpath);
            Files.write(fpath,json.getBytes());
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    public static final void writeJsonToFile(String path,Object object){

        writeJson(new Gson(),object,path);
    }

    public static final void writeJsonToFileWithPretty(String path, Object object){

        writeJson(new GsonBuilder().setPrettyPrinting().create(),object,path);
    }

    public static String decodeFromBase64(String json){

        return new String(Base64.getDecoder().decode(json.getBytes()), Charset.forName("utf-8"));
    }


}
