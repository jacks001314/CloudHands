package com.antell.cloudhands.api.rule;

import java.util.List;

public class Rule {

    private String proto;

    private long id;
    private String type;
    private String msg;

    private boolean isEnable;
    private boolean isAnd;

    private List<RuleItem> items;

    private String name;
    private String value;
    private long time;

    private RulePool rulePool;

    public String getProto() {
        return proto;
    }

    public void setProto(String proto) {
        this.proto = proto;
    }

    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

    public boolean isAnd() {
        return isAnd;
    }

    public void setAnd(boolean and) {
        isAnd = and;
    }

    public List<RuleItem> getItems() {
        return items;
    }

    public void setItems(List<RuleItem> items) {
        this.items = items;
    }

    public boolean isEnable() {
        return isEnable;
    }

    public void setEnable(boolean enable) {
        isEnable = enable;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }

    public long getTime() {
        return time;
    }

    public void setTime(long time) {
        this.time = time;
    }

    public RulePool getRulePool() {
        return rulePool;
    }

    public void setRulePool(RulePool rulePool) {
        this.rulePool = rulePool;
    }
}
