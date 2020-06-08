package com.antell.cloudhands.api.rule;

import com.antell.cloudhands.api.utils.TextUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class RuleContext {

    private static final Logger log = LoggerFactory.getLogger(RuleContext.class);

    private Map<String, List<String>> maps;

    public RuleContext(){

        maps = new HashMap<>();
    }

    private List<String> getArrayFromFile(String fname){

        List<String> arrays = maps.get(fname);

        if(arrays == null){

            /*try to load from file*/
            try {

                arrays = Files.readAllLines(Paths.get(fname))
                .stream().map(e->e.trim()).filter(e->!TextUtils.isEmpty(e)&&!e.startsWith("#")).collect(Collectors.toList());
                maps.put(fname,arrays);

                log.info(String.format("Load rule value number:%d from file:%s",arrays.size(),fname));

            } catch (IOException e) {

            }
        }

        return arrays;

    }

    private List<String> getArrayFromSplits(String name,String value){

        List<String> arr = maps.get(name);

        if(arr == null) {

            arr = new ArrayList<>();

            for (String s : value.split(RuleConstants.arrSplit)) {

                arr.add(s);
            }

            log.info(String.format("Load rule value number:%d from splits name:%s,value:%s",arr.size(),name,value));
            maps.put(name,arr);
        }

        return arr;
    }

    public List<String> findArray(String value){

        String[] splits = value.split(":");
        String type = splits[0];

        if(type.equals("file")){

            return getArrayFromFile(splits[1]);
        }else if(type.equals("inline")){

            return getArrayFromSplits(splits[1],splits[2]);
        }else{

            log.error(String.format("Unkown array type:%s",type));
            return new ArrayList<>();
        }

    }


}
