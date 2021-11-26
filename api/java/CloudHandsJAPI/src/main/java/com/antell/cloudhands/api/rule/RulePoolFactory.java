package com.antell.cloudhands.api.rule;

import com.antell.cloudhands.api.utils.GsonUtils;

import java.util.ArrayList;
import java.util.List;

public class RulePoolFactory {

    private static final String getRulePath(RuleGroup ruleGroup){

        return String.format("%s/%s/%s/%s.json",ruleGroup.getRootDir(),ruleGroup.getEngine(),ruleGroup.getName(),ruleGroup.getName());
    }

    public static final List<RulePool> create(String ruleConfigPath, String engine, List<String> protos,
                                              RuleAction ruleAction, boolean matchThenStop) throws Exception {

        List<RulePool> pools = new ArrayList<>();

        RuleGroupConfig ruleGroupConfig = GsonUtils.loadConfigFromJsonFile(ruleConfigPath,RuleGroupConfig.class);

        for(RuleGroup ruleGroup:ruleGroupConfig.getGroups()){

            if(ruleGroup.isEnable()&&ruleGroup.getEngine().equals(engine)){

                RulePool rulePool = new RulePool(getRulePath(ruleGroup),ruleAction,matchThenStop,false,protos);

                if(!rulePool.isEmpty()){
                    rulePool.setRuleGroup(ruleGroup);
                    pools.add(rulePool);
                }
            }

        }

        return pools;
    }

}
