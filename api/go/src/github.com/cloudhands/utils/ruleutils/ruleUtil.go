package ruleutils

import (
	"encoding/hex"
	"io/ioutil"
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



func GetRulePath(ruleDir string,engine string,rtype string)string {
	return filepath.Join(ruleDir,engine,rtype,rtype+RuleFileExtName)
}

func GetRuleEnginePath(ruleDir string) string {

	return filepath.Join(ruleDir,RuleEngineName)
}


func GetRuleGroupPath(ruleDir string) string {
	return filepath.Join(ruleDir,RuleGroupName)
}
