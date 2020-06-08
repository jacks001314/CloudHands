package com.antell.cloudhands.api.rule;

import com.antell.cloudhands.api.utils.FileUtils;
import com.antell.cloudhands.api.utils.GsonUtils;
import com.antell.cloudhands.api.utils.TextUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class RulePool {

    private static final Logger log = LoggerFactory.getLogger(RulePool.class);
    private RuleConfig config;
    private RuleAction ruleAction;
    private boolean matchThenStop;
    private RuleContext context;

    public RulePool(String rulePath, RuleAction ruleAction, boolean matchThenStop, boolean fromDir) throws IOException {

        this.config = new RuleConfig();
        this.config.setRules(new ArrayList<>());

        this.ruleAction = ruleAction;
        this.matchThenStop = matchThenStop;
        this.context = new RuleContext();

        loadRules(rulePath,fromDir);
    }

    private void loadRules(String rulePath,boolean fromDir) throws IOException {

        List<String> files = new ArrayList<>();
        if(!fromDir){
            files.add(rulePath);
        }else{

            List<String> dirs = FileUtils.dirs(rulePath);
            dirs.forEach(dir->files.add(String.format("%s/%s/%s.json",rulePath,dir,dir)));
        }

        for(String file :files){
            if(FileUtils.isExisted(file)){

                RuleConfig loadRuleConfig = GsonUtils.loadConfigFromJsonFile(file, RuleConfig.class);
                log.info(String.format("Load rule from file:%s,the number:%s",rulePath,loadRuleConfig.getRules().size()));
                addRules(loadRuleConfig);
            }
        }

        log.info(String.format("Load rule from dir:%s,after merge the number:%s",rulePath,config.getRules().size()));
    }

    private boolean isValidRule(Rule rule){

        if(!rule.isEnable())
            return false;

        if(TextUtils.isEmpty(rule.getProto())||rule.getItems()==null||rule.getItems().size()==0)
            return false;

        return true;
    }

    private void addRules(RuleConfig loadRuleConfig){

        if(loadRuleConfig == null)
            return;

        List<Rule> rules = loadRuleConfig.getRules();

        if(rules == null ||rules.size() == 0)
            return;


        List<Rule> dstRules = config.getRules();

        for(Rule rule:rules){

            if(isValidRule(rule)){
                dstRules.add(rule);
            }
        }

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


    public RuleContext getContext() {
        return context;
    }

    public void setContext(RuleContext context) {
        this.context = context;
    }
}
