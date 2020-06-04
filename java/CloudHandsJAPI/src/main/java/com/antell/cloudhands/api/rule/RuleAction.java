package com.antell.cloudhands.api.rule;

import com.antell.cloudhands.api.source.SourceEntry;

public interface RuleAction {

    void action(SourceEntry sourceEntry, Rule rule);
}
