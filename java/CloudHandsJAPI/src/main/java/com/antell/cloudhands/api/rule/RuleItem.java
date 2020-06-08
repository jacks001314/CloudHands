package com.antell.cloudhands.api.rule;

public class RuleItem {

    private String target;
    private String op;
    private String value;

    private boolean isArray;
    private boolean isHex;
    private boolean isnot;

    public String getTarget() {
        return target;
    }

    public void setTarget(String target) {
        this.target = target;
    }

    public String getOp() {
        return op;
    }

    public void setOp(String op) {
        this.op = op;
    }

    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }

    public boolean isHex() {
        return isHex;
    }

    public void setHex(boolean hex) {
        isHex = hex;
    }

    public boolean isIsnot() {
        return isnot;
    }

    public void setIsnot(boolean isnot) {
        this.isnot = isnot;
    }

    public boolean isArray() {
        return isArray;
    }

    public void setArray(boolean array) {
        isArray = array;
    }
}
