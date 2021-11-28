package rule

import (
	"archive/zip"
	"encoding/hex"
	"io/ioutil"
	"os"
	"path/filepath"
	"strconv"
	"strings"
)

func TargetValue(v interface{},isHex bool) string {

	var str string

	switch v:=v.(type) {

	case int:
		str = strconv.FormatInt(int64(v),10)
	case int8:
		str = strconv.FormatInt(int64(v),10)
	case int16:
		str = strconv.FormatInt(int64(v),10)
	case int32:
		str = strconv.FormatInt(int64(v),10)
	case int64:
		str = strconv.FormatInt(v,10)
	case uint8:
		str = strconv.FormatUint(uint64(v),10)

	case uint16:
		str = strconv.FormatUint(uint64(v),10)
	case uint32:
		str = strconv.FormatUint(uint64(v),10)
	case uint64:
		str = strconv.FormatUint(v,10)
	case string:
		str = v

	}

	if len(str) == 0 {

		return ""
	}

	if isHex {

		return hex.EncodeToString([]byte(str))
	}

	return str
}

func TargetValues(values []string,split string,isHex bool) string {

	var b strings.Builder

	n := len(values)

	if n == 0 {
		return ""
	}

	if n == 1 {

		if isHex {

			return hex.EncodeToString([]byte(values[0]))
		}
		return values[0]
	}

	for i,v := range values {

		if isHex {
			v = hex.EncodeToString([]byte(v))
		}
		if(i==n-1){
			b.WriteString(v)
		}else{
			b.WriteString(v)
			b.WriteString(split)
		}
	}

	return b.String()
}

func GetData(data []byte,maxLen int ,offset int,dlen int,isHex bool) string {

	var rlen int
	n:= len(data)

	if n == 0 {
		return ""
	}

	if offset<0 {
		offset = 0
	}

	if dlen <0 {
		dlen = 0
	}

	rlen = n

	if maxLen>0 && rlen>maxLen {
		rlen = maxLen
	}

	if offset>=rlen {
		return ""
	}

	rlen = rlen-offset

	if dlen>0&&dlen<rlen {
		rlen = dlen
	}

	if isHex {

		return hex.EncodeToString(data[offset:rlen])
	}

	return string(data[offset:rlen])

}

func GetDataFromFile (fpath string,offset int,dlen int,isHex bool) string  {

	if data,err := ioutil.ReadFile(fpath);err!=nil {

		return ""
	}else {
		return GetData(data,0,offset,dlen,isHex)
	}

}


func GetRuleFilesFromZipFile(fpath string) []*RuleFile {

	files := make([]*RuleFile,10,20)
	reader,err := zip.OpenReader(fpath)

	defer reader.Close()

	if err!=nil {

		return files
	}

	for _,f := range reader.File {

		fname := f.Name

		if strings.HasSuffix(fname,RuleFileExtName) {

			ruleFile := NewRuleFileFromPath(fname)

			if strings.HasPrefix(ruleFile.Name,ruleFile.Type){

				files = append(files, ruleFile)
			}
		}
	}

	return files
}

func GetRuleFilesFromRuleDir(ruleDir string) []*RuleFile {

	files := make([]*RuleFile,10,20)

	filepath.Walk(ruleDir, func(path string, info os.FileInfo, err error) error {

		if strings.HasSuffix(path,RuleFileExtName){

			ruleFile := NewRuleFileFromPath(path)

			if strings.HasPrefix(ruleFile.Name,ruleFile.Type){

				files = append(files, ruleFile)
			}
		}

		return nil
	})

	return files
}

func GetRulePath(ruleDir string,engine string,rtype string)string {
	return filepath.Join(ruleDir,engine,rtype,rtype+RuleFileExtName)
}

func GetRuleEnginePath(ruleDir string) string {

	return filepath.Join(ruleDir,RuleEngineName)
}


func GetRuleGroupPath(ruleDir string) string {
	return filepath.Join(ruleDir,RuleGroupName)
}


/*



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
}*/
