package com.antell.cloudhands.api.rule;

import com.antell.cloudhands.api.utils.*;
import net.lingala.zip4j.ZipFile;
import net.lingala.zip4j.exception.ZipException;
import net.lingala.zip4j.model.FileHeader;

import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class RuleUtils {

    public static final String targetValue(long v,boolean isHex){

        String value = Long.toString(v);

        if(TextUtils.isEmpty(value))
            return value;

        return isHex? ByteDataUtils.toHex(value):value;
    }

    public static final String targetValue(String value,boolean isHex){

        if(TextUtils.isEmpty(value))
            return value;

        return isHex? ByteDataUtils.toHex(value):value;
    }

    public static final String targetValue(List<String> values,String split,boolean isHex){

        if(values == null||values.size()==0)
            return "";

        int len = values.size();
        if(len ==1)
            return isHex?ByteDataUtils.toHex(values.get(0)):values.get(0);

        StringBuffer sb = new StringBuffer();

        for(int i = 0;i<len;i++){

            String v = isHex?ByteDataUtils.toHex(values.get(i)):values.get(i);

            if(i==len-1){
                sb.append(v);
            }else{
                sb.append(v);
                sb.append(split);
            }
        }
        return sb.toString();
    }

    public static final String fromFile(String fpath,int offset,int len,boolean isHex){

        try {
            byte[] data = Files.readAllBytes(Paths.get(fpath));

            return getData(data,0,offset,len,isHex);


        } catch (Exception e) {

            return "";
        }
    }

    public static final String fromFile(String fpath,String contentEncode,int maxLen,int offset,int len,boolean isHex){

        try {

            byte[] data = Content.readBody(fpath,contentEncode);
            return getData(data,maxLen,offset,len,isHex);

        } catch (Exception e) {

            return "";
        }
    }

    public static final String fromFile(String fpath,int maxLen,int offset,int len,boolean isHex){

        try {

            byte[] data = Files.readAllBytes(Paths.get(fpath));
            return getData(data,maxLen,offset,len,isHex);

        } catch (Exception e) {

            return "";
        }
    }


    public static final String getData(byte[] data,int maxLen,int offset,int len,boolean isHex){

        int rlen;
        if(data == null||data.length==0)
            return "";

        if(offset<0)
            offset = 0;
        if(len<0)
            len = 0;


        rlen = data.length;
        if(maxLen>0&&rlen>maxLen)
            rlen = maxLen;

        if(offset>=rlen)
            return "";

        rlen = rlen-offset;
        if(len>0&&len<rlen){
            rlen = len;
        }

        if(isHex){
            return ByteDataUtils.toHex(data,offset,rlen);
        }

        return new String(data,offset,rlen,Charset.forName("utf-8"));
    }

    public static List<RuleFile> getRuleFilesFromZipFile(String zipFile) throws ZipException {

        List<RuleFile> results = new ArrayList<>();
        ZipFile zf = new ZipFile(zipFile);

        for(FileHeader fileHeader:zf.getFileHeaders()){

            String name = fileHeader.getFileName();
            if(!TextUtils.isEmpty(name)&&name.endsWith(".json")){

                RuleFile ruleFile = new RuleFile(name);

                if(ruleFile.getName().startsWith(ruleFile.getType()))
                    results.add(ruleFile);
            }
        }

        return results;
    }

    public static List<RuleFile> getRuleFilesFromRuleDir(String ruleDir) throws IOException {

        List<RuleFile> results = new ArrayList<>();

        Files.walkFileTree(Paths.get(ruleDir),new SimpleFileVisitor<Path>(){

            @Override
            public FileVisitResult visitFile(Path path,
                                             BasicFileAttributes attrs) throws IOException {

                String name = path.toString();
                if(name.endsWith(".json")){

                    RuleFile ruleFile = new RuleFile(name);
                    if(!TextUtils.isEmpty(ruleFile.getName())&&ruleFile.getName().startsWith(ruleFile.getType())){

                        results.add(ruleFile);
                    }
                }
                return FileVisitResult.CONTINUE;
            }

            @Override
            public FileVisitResult visitFileFailed(Path file, IOException exc)
                    throws IOException {
                return FileVisitResult.CONTINUE;
            }
        });

        return results;
    }

    public static final String getRulePath(String ruleDir,String engine,String type){

        return String.format("%s/%s/%s/%s.json",ruleDir,engine,type,type);
    }

    public static final String getEnginePath(String ruleDir){

        return String.format("%s/RuleEngine.json",ruleDir);
    }

    public static final String getRuleGroupPath(String ruleDir){

        return String.format("%s/RuleGroup.json",ruleDir);
    }

    public static final RuleGroupConfig loadRuleGroups(String ruleDir) throws IOException {

        return GsonUtils.loadConfigFromJsonFile(getRuleGroupPath(ruleDir),RuleGroupConfig.class);
    }

    public static final RuleGroup findRuleGroup(RuleGroupConfig config,RuleFile ruleFile){

        if(config == null||ruleFile == null)
            return null;

        String engine = ruleFile.getEngine();
        String type = ruleFile.getType();

        if(TextUtils.isEmpty(engine)||TextUtils.isEmpty(type))
            return null;

        List<RuleGroup> groups = config.getGroups();

        if(groups == null||groups.size()==0)
            return null;

        for(RuleGroup ruleGroup:groups){

            if(ruleGroup.getEngine().equals(engine)&&ruleGroup.getName().equals(type))
                return ruleGroup;
        }

        return null;
    }

    public static final void writeRuleGroup(RuleGroupConfig config,String ruleDir){

        GsonUtils.writeJsonToFileWithPretty(getRuleGroupPath(ruleDir),config);
    }


    public static final RuleConfig loadRules(String ruleDir,String engine,String type) throws IOException {

        return GsonUtils.loadFromJsonFile(getRulePath(ruleDir,engine,type),RuleConfig.class);
    }

    public static final Rule findRule(RuleConfig config,long id){

        if(config == null ||id <=0)
            return null;

        List<Rule> rules = config.getRules();

        if(rules == null||rules.size()==0)
            return null;

        for(Rule rule:rules){

            if(rule.getId() == id)
                return rule;
        }

        return null;
    }

    public static final long findMaxRuleId(RuleConfig config){

        long max = 0;
        if(config == null)
            return 0;

        List<Rule> rules = config.getRules();

        if(rules == null||rules.size()==0)
            return 0;

        for(Rule rule:rules){
            long id = rule.getId();
            if(id>max)
                max = id;
        }

        return max;
    }

    public static void writeRules(RuleConfig ruleConfig,String ruleDir,String engine,String type){

        GsonUtils.writeJsonToFileWithPretty(getRulePath(ruleDir,engine,type),ruleConfig);
    }

    public static void updateRuleGroupCounts(RuleGroup ruleGroup,String ruleDir) throws Exception {

        RuleConfig config = loadRules(ruleDir,ruleGroup.getEngine(),ruleGroup.getName());

        if(config == null||config.getRules()==null||config.getRules().size()==0){

            ruleGroup.setRuleCounts(0);
            ruleGroup.setDisableRuleCounts(0);
            ruleGroup.setEnableRuleCounts(0);
        }else{

            long rcount = 0;
            long dcount = 0;
            long ecount = 0;

            List<Rule> rules = config.getRules();
            rcount = rules.size();

            for(Rule rule:rules){

                if(rule.isEnable())
                    ecount++;
                else
                    dcount++;
            }

            ruleGroup.setEnableRuleCounts(ecount);
            ruleGroup.setDisableRuleCounts(dcount);
        }
    }

    public static final void updateRuleGroups(String ruleDir,List<RuleFile> ruleFiles) throws Exception {

        if(ruleFiles == null||ruleFiles.size()==0)
            return;

        RuleGroupConfig config = loadRuleGroups(ruleDir);
        List<RuleGroup> groups = config.getGroups();

        for(RuleFile rfile:ruleFiles){

            RuleGroup ruleGroup = findRuleGroup(config,rfile);
            if(ruleGroup == null){
                ruleGroup = new RuleGroup();
                ruleGroup.setEnable(true);
                ruleGroup.setRootDir(ruleDir);
                ruleGroup.setInline(false);
                ruleGroup.setEngine(rfile.getEngine());
                ruleGroup.setName(rfile.getType());
                ruleGroup.setTime(System.currentTimeMillis());

                updateRuleGroupCounts(ruleGroup,ruleDir);

                groups.add(ruleGroup);
            }else{

                ruleGroup.setTime(System.currentTimeMillis());
                updateRuleGroupCounts(ruleGroup,ruleDir);
            }
        }

        writeRuleGroup(config,ruleDir);
    }


    public static final void uploadRules(String ruleDir,String zipRuleFile) throws Exception {

        List<RuleFile> ruleFiles = getRuleFilesFromZipFile(zipRuleFile);
        if(ruleFiles!=null&&ruleFiles.size()>0){

            ZipFileUtils.unzip(zipRuleFile,ruleDir);

            List<RuleFile> allRuleFiles = getRuleFilesFromRuleDir(ruleDir);
            if(allRuleFiles == null||allRuleFiles.size()<ruleFiles.size()){

                updateRuleGroups(ruleDir,ruleFiles);
            }else{
                updateRuleGroups(ruleDir,allRuleFiles);
            }
        }

        Files.deleteIfExists(Paths.get(zipRuleFile));

    }

    public static final String getRuleDir(String rootDir,RuleFile ruleFile){

        return String.format("%s/%s/%s",rootDir,ruleFile.getEngine(),ruleFile.getType());
    }

    public static final void ruleCopy(String ruleDir,String dstDir,RuleFile ruleFile) throws IOException {

        String rulePath = getRuleDir(ruleDir,ruleFile);

        Files.walkFileTree(Paths.get(rulePath),new SimpleFileVisitor<>(){
            @Override
            public FileVisitResult visitFile(Path path,
                                             BasicFileAttributes attrs) throws IOException {

                String name = path.toString();

                FileUtils.copy(name,String.format("%s/%s/%s/%s",dstDir,ruleFile.getEngine(),ruleFile.getType(),
                        path.getFileName().toString()));

                return FileVisitResult.CONTINUE;
            }

            @Override
            public FileVisitResult visitFileFailed(Path file, IOException exc)
                    throws IOException {
                return FileVisitResult.CONTINUE;
            }
        });
    }

    public static final String zipRuleFile(String ruleDir,List<RuleFile> ruleFiles) throws IOException {

        String name = "/tmp/rules.zip";
        String rootPath = "/tmp/rules/";

        Files.deleteIfExists(Paths.get(name));
        FileUtils.delDirs(rootPath);

        for(RuleFile ruleFile:ruleFiles){

            String rule = getRulePath(ruleDir,ruleFile.getEngine(),ruleFile.getType());

            if(FileUtils.isExisted(rule)){

                ruleCopy(ruleDir,rootPath,ruleFile);
            }
        }

        ZipFileUtils.pzipDir(name,FileUtils.dirs(rootPath).stream().map(e->String.format("%s/%s",rootPath,e)).collect(Collectors.toList()));

        return name;
    }
}
