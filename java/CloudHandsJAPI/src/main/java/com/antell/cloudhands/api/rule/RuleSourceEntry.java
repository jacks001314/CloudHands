package com.antell.cloudhands.api.rule;

public interface RuleSourceEntry {

    boolean canMatch(String proto);
    String getTargetValue(RuleItem ruleItem);

}
