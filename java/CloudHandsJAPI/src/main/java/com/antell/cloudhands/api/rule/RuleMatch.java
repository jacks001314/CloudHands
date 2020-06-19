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

        boolean isMatch(RuleContext context, String tvalue, String value, RuleItem ruleItem);
    }


    private static Map<String,OPAction> opMaps = new HashMap<>();

    static {

        opMaps.put(RuleConstants.contains, (context, tvalue, value, ruleItem) -> {

            if(ruleItem.isArray()){

                for(String s:context.findArray(value)){

                    if(tvalue.contains(s)){
                        if(!ruleItem.isAnd())
                            return true;
                    }else{
                        if(ruleItem.isAnd())
                            return false;
                    }
                }

                return ruleItem.isAnd();

            }
            return tvalue.contains(value);
        });

        opMaps.put(RuleConstants.startsWith,(context, tvalue, value, ruleItem) -> {

            if(ruleItem.isArray()){

                for(String s:context.findArray(value)){
                    if(tvalue.startsWith(s)){
                        if(!ruleItem.isAnd())
                            return true;
                    }else{
                        if(ruleItem.isAnd())
                            return false;
                    }
                }

                return ruleItem.isAnd();
            }
            return tvalue.startsWith(value);
        });


        opMaps.put(RuleConstants.endsWith,(context, tvalue, value, ruleItem) -> {

            if(ruleItem.isArray()){

                for(String s:context.findArray(value)){
                    if(tvalue.endsWith(s)){
                        if(!ruleItem.isAnd())
                            return true;
                    }else{
                        if(ruleItem.isAnd())
                            return false;
                    }
                }

                return ruleItem.isAnd();
            }
            return tvalue.endsWith(value);
        });

        opMaps.put(RuleConstants.regex,(context, tvalue, value, ruleItem) -> {

            if(ruleItem.isArray()){

                for(String s:context.findArray(value)){

                    Pattern pattern = Pattern.compile(s);
                    Matcher matcher = pattern.matcher(tvalue);
                    if(matcher.matches()){
                        if(!ruleItem.isAnd())
                            return true;
                    }else {
                        if(ruleItem.isAnd())
                            return false;
                    }
                }

                return ruleItem.isAnd();
            }

            Pattern pattern = Pattern.compile(value);
            Matcher matcher = pattern.matcher(tvalue);
            return matcher.matches();
        });

        opMaps.put(RuleConstants.eq,(context, tvalue, value, ruleItem) -> {

            if(ruleItem.isArray()){

                for(String s:context.findArray(value)){

                    if(tvalue.equals(s)){
                        if(!ruleItem.isAnd())
                            return true;
                    }else{
                        if(ruleItem.isAnd())
                            return false;
                    }
                }

                return ruleItem.isAnd();
            }

            return tvalue.equals(value);
        });

        opMaps.put(RuleConstants.lt,(context, tvalue, value, ruleItem) -> {

            if(ruleItem.isArray()){

                for(String s:context.findArray(value)){

                    if(Integer.parseInt(tvalue)<Integer.parseInt(s)){
                        if(!ruleItem.isAnd())
                            return true;
                    }else{
                        if(ruleItem.isAnd())
                            return false;
                    }
                }

                return ruleItem.isAnd();
            }

            return Integer.parseInt(tvalue)<Integer.parseInt(value);
        });

        opMaps.put(RuleConstants.gt,(context, tvalue, value, ruleItem) -> {

            if(ruleItem.isArray()){

                for(String s:context.findArray(value)){

                    if(Integer.parseInt(tvalue)>Integer.parseInt(s)){

                        if(!ruleItem.isAnd())
                            return true;
                    }else{
                        if(ruleItem.isAnd())
                            return false;
                    }

                }
                return ruleItem.isAnd();
            }

            return Integer.parseInt(tvalue)>Integer.parseInt(value);
        });

        opMaps.put(RuleConstants.le,(context, tvalue, value, ruleItem) -> {

            if(ruleItem.isArray()){

                for(String s:context.findArray(value)){

                    if(Integer.parseInt(tvalue)<=Integer.parseInt(s)){
                        if(!ruleItem.isAnd())
                            return true;
                    }else{
                        if(ruleItem.isAnd())
                            return false;
                    }
                }
                return ruleItem.isAnd();
            }

            return Integer.parseInt(tvalue)<=Integer.parseInt(value);
        });

        opMaps.put(RuleConstants.ge,(context, tvalue, value, ruleItem) -> {

            if(ruleItem.isArray()){

                for(String s:context.findArray(value)){

                    if(Integer.parseInt(tvalue)>=Integer.parseInt(s)){

                        if(!ruleItem.isAnd())
                            return true;
                    }else{
                        if(ruleItem.isAnd())
                            return false;
                    }

                }
                return ruleItem.isAnd();
            }

            return Integer.parseInt(tvalue)>=Integer.parseInt(value);
        });

    }

    private static boolean opMatch(RuleContext context, String tvalue, String op, String value, RuleItem ruleItem){

        if(TextUtils.isEmpty(tvalue)|| TextUtils.isEmpty(op)|| TextUtils.isEmpty(value))
            return false;

        OPAction action = opMaps.get(op);
        if(action == null)
            return false;

        return action.isMatch(context,tvalue,value,ruleItem);
    }

    private static boolean doMatch(RuleContext context, SourceEntry sourceEntry, RuleItem ruleItem){

        String tvalue = sourceEntry.getTargetValue(ruleItem.getTarget(),ruleItem.isHex());

        boolean res = opMatch(context,tvalue,ruleItem.getOp(),ruleItem.getValue(),ruleItem);

        return ruleItem.isIsnot()?!res:res;
    }

    private static boolean isMatch(RuleContext context, SourceEntry sourceEntry, Rule rule){

        boolean isAnd = rule.isAnd();

        List<RuleItem> ruleItems = rule.getItems();

        for(RuleItem item:ruleItems){

            boolean res = doMatch(context,sourceEntry,item);

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

            if(!rule.isEnable())
                continue;

            if(TextUtils.isEmpty(rule.getProto())||!sourceEntry.canMatch(rule.getProto()))
                continue;

            boolean res = isMatch(rulePool.getContext(),sourceEntry,rule);
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
