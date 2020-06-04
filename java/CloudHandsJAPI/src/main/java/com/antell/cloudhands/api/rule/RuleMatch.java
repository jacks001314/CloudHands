package com.antell.cloudhands.api.rule;

import com.antell.cloudhands.api.source.SourceEntry;
import com.antell.cloudhands.api.utils.TextUtils;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class RuleMatch {

    private RuleMatch(){

    }

    private interface OPAction {

        boolean isMatch(String tvalue,String value);
    }


    private static Map<String,OPAction> opMaps = new HashMap<>();

    static {

        opMaps.put(RuleConstants.contains, (tvalue, value) -> tvalue.contains(value));
        opMaps.put(RuleConstants.startsWith,(tvalue, value) -> tvalue.startsWith(value));
        opMaps.put(RuleConstants.endsWith,(tvalue, value) -> tvalue.endsWith(value));
        opMaps.put(RuleConstants.regex,(tvalue, value) -> {
            Pattern pattern = Pattern.compile(value);
            Matcher matcher = pattern.matcher(tvalue);
            return matcher.matches();
        });

        opMaps.put(RuleConstants.eq,(tvalue, value) -> tvalue.equals(value));
        opMaps.put(RuleConstants.lt,(tvalue, value) -> Integer.parseInt(tvalue)<Integer.parseInt(value));
        opMaps.put(RuleConstants.gt,(tvalue, value) -> Integer.parseInt(tvalue)>Integer.parseInt(value));
        opMaps.put(RuleConstants.le,(tvalue, value) -> Integer.parseInt(tvalue)<=Integer.parseInt(value));
        opMaps.put(RuleConstants.ge,(tvalue, value) -> Integer.parseInt(tvalue)>=Integer.parseInt(value));

        opMaps.put(RuleConstants.in,(tvalue, value) -> {

            String[] splits = value.split(RuleConstants.arrSplit);
            for(String s:splits){

                if(s.equals(value))
                    return true;
            }
            return false;
        });

    }

    private static boolean opMatch(String tvalue,String op,String value){

        if(TextUtils.isEmpty(tvalue)|| TextUtils.isEmpty(op)|| TextUtils.isEmpty(value))
            return false;

        OPAction action = opMaps.get(op);
        if(action == null)
            return false;

        return action.isMatch(tvalue,value);
    }

    private static boolean doMatch(SourceEntry sourceEntry, RuleItem ruleItem){

        String tvalue = sourceEntry.getTargetValue(ruleItem.getTarget(),ruleItem.isHex());

        boolean res = opMatch(tvalue,ruleItem.getOp(),ruleItem.getValue());

        return ruleItem.isIsnot()?!res:res;
    }

    private static boolean isMatch(SourceEntry sourceEntry, Rule rule){

        boolean isAnd = rule.isAnd();

        List<RuleItem> ruleItems = rule.getItems();

        for(RuleItem item:ruleItems){

            boolean res = doMatch(sourceEntry,item);

            if(isAnd){
                if(!res)
                    return false;
            }else{
                if(res)
                    return true;
            }
        }

        return isAnd;
    }

    public static boolean match(SourceEntry sourceEntry, RulePool rulePool){

        boolean isMatch = false;

        RuleConfig ruleConfig = rulePool.getConfig();
        RuleAction action = rulePool.getRuleAction();

        if(ruleConfig == null)
            return false;

        List<Rule> rules = ruleConfig.getRules();
        if(rules == null ||rules.size() == 0)
            return false;

        for(Rule rule:rules){

            if(TextUtils.isEmpty(rule.getProto())||!sourceEntry.canMatch(rule.getProto()))
                continue;

            boolean res = isMatch(sourceEntry,rule);
            if(res){

                isMatch = true;
                if(action!=null){
                    action.action(sourceEntry,rule);
                }

                if(rulePool.isMatchThenStop())
                    break;
            }
        }

        return isMatch;
    }

}
