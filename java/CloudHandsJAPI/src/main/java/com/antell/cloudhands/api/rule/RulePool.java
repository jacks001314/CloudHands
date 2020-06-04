package com.antell.cloudhands.api.rule;

public class RulePool {

    private RuleConfig config;
    private RuleAction ruleAction;
    private boolean matchThenStop;

    public RulePool(String rulePath, RuleAction ruleAction){

        this.ruleAction = ruleAction;
    }

    public RulePool(String ruleDir, String type, RuleAction ruleAction){

        this.ruleAction = ruleAction;
    }

    public RuleConfig getConfig() {
        return config;
    }

    public void setConfig(RuleConfig config) {
        this.config = config;
    }

    public RuleAction getRuleAction() {
        return ruleAction;
    }

    public void setRuleAction(RuleAction ruleAction) {
        this.ruleAction = ruleAction;
    }

    public boolean isMatchThenStop() {
        return matchThenStop;
    }

    public void setMatchThenStop(boolean matchThenStop) {
        this.matchThenStop = matchThenStop;
    }
}
