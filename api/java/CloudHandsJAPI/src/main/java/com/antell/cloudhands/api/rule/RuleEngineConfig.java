package com.antell.cloudhands.api.rule;

import java.util.List;

public class RuleEngineConfig {

    private List<RuleEngine> engines;

    public List<RuleEngine> getEngines() {
        return engines;
    }

    public void setEngines(List<RuleEngine> engines) {
        this.engines = engines;
    }
}
