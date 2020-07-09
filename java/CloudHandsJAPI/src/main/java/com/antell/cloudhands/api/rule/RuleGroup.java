package com.antell.cloudhands.api.rule;

public class RuleGroup {

    private String rootDir;
    private String name;
    private String engine;


    private boolean enable;
    private boolean isInline;
    private long ruleCounts;
    private long enableRuleCounts;
    private long disableRuleCounts;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getEngine() {
        return engine;
    }

    public void setEngine(String engine) {
        this.engine = engine;
    }

    public boolean isEnable() {
        return enable;
    }

    public void setEnable(boolean enable) {
        this.enable = enable;
    }

    public boolean isInline() {
        return isInline;
    }

    public void setInline(boolean inline) {
        isInline = inline;
    }

    public long getRuleCounts() {
        return ruleCounts;
    }

    public void setRuleCounts(long ruleCounts) {
        this.ruleCounts = ruleCounts;
    }

    public long getEnableRuleCounts() {
        return enableRuleCounts;
    }

    public void setEnableRuleCounts(long enableRuleCounts) {
        this.enableRuleCounts = enableRuleCounts;
    }

    public long getDisableRuleCounts() {
        return disableRuleCounts;
    }

    public void setDisableRuleCounts(long disableRuleCounts) {
        this.disableRuleCounts = disableRuleCounts;
    }

    public String getRootDir() {
        return rootDir;
    }

    public void setRootDir(String rootDir) {
        this.rootDir = rootDir;
    }
}
