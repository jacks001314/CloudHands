package com.antell.cloudhands.api.rule;

public class RuleName {

    private String name;
    private String cname;

    public RuleName(String name, String cname){

        this.name = name;
        this.cname = cname;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getCname() {
        return cname;
    }

    public void setCname(String cname) {
        this.cname = cname;
    }
}
