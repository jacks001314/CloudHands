package com.antell.cloudhands.api.rule;

import java.nio.file.Path;
import java.nio.file.Paths;

public class RuleFile {

    private String engine;
    private String type;
    private String name;

    public RuleFile(String engine,String type){

        this.engine = engine;
        this.type = type;
    }

    public RuleFile(String file){

        Path path = Paths.get(file);
        int count = path.getNameCount();

        name = "";
        type = "";
        engine = "";

        if(count>=3){

            name = path.getName(count-1).toString();
            type = path.getName(count-2).toString();
            engine = path.getName(count-3).toString();
        }
    }

    public String getEngine() {
        return engine;
    }

    public void setEngine(String engine) {
        this.engine = engine;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String toString(){

        return String.format("Engine:%s,Type:%s,Name:%s",engine,type,name);
    }

}
