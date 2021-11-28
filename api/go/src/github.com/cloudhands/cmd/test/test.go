package main

import (
	"encoding/hex"
	"encoding/json"
	"fmt"
	"github.com/cloudhands/rule"
	"os"
	"path/filepath"
	"strconv"
	"strings"
)

type Item struct {

	A int  `json:"a"`
	B string `json:"b"`
}

type Student struct {

	Name string `json:"name"`
	Age int 	`json:"age"`
	KS []string  `json:"ks"`

	M *Item  `json:"m"`
}

func print(v interface{}){


	var ss = []string{"fs","fer"}

	for i,vvv :=range ss {

		fmt.Println(i,vvv)
	}

	var b strings.Builder

	b.WriteString("shajf")
	b.WriteString(",")
	b.WriteString("cao")

	fmt.Println(b.String())

	switch vv:=v.(type) {

	case string:
		fmt.Println(hex.EncodeToString([]byte(vv)))

	case int:
		fmt.Println(hex.EncodeToString([]byte(strconv.FormatInt(int64(vv),10))))
	case uint64:

		fmt.Println("he:",vv)
	}

}

func testWalk(fdir string){

	filepath.Walk(fdir, func(path string, info os.FileInfo, err error) error {
		
		fmt.Println(path)
		return nil
	})

}

func testLoadRuleGroup(){

	//fpath := "D:\\shajf_dev\\self\\CloudHands\\api\\go\\src\\data\\RuleGroup.json"
	rootDir := "D:\\shajf_dev\\self\\CloudHands\\api\\go\\src\\data"

	if rgc,err := rule.LoadRuleGroup(rootDir); err!=nil {

		fmt.Println(err)
	}else{

		//data,_ := json.Marshal(rg)

		//fmt.Println(string(data))

		rf := rule.NewRuleFile("mining","tcp","tcp.json")

		if rg,err:= rgc.FindRuleGroup(rf);err!=nil {

			fmt.Println(err)
		}else{


			rc,_ := rg.LoadRules(rootDir)

			data,_ := json.MarshalIndent(rc,"","\t")

			//jsonutils.WriteJsonPretty(rgc,"D:\\shajf_dev\\self\\CloudHands\\api\\go\\src\\data\\RuleGroup1.json")

			fmt.Println(string(data))
		}

	}


}

func main(){

	//fmt.Println(rule.TargetValue("wocaonima001314",true))
	//fmt.Println(rule.TargetValues([]string{"shajf"},",",true))

	//fmt.Println(rule.GetData([]byte("woshiyigeshibi"),0,0,0,false))

	//fmt.Println(strings.HasPrefix("shajf00","shajf12"))
	//javas := make([]string,5,10)

	//fpath := "D:\\shajf_dev\\self\\CloudHands\\api\\go"
	//testWalk(fpath)

	testLoadRuleGroup()
	//paths := util.GetFilePaths(fpath)

	//fmt.Println(len(paths),paths)

	/*
	var stu Student


	print("shajferewrewrewrew")
	print(1234)
	//print(uint64(1233445))
	str := "{\"name\":\"shajf\",\"age\":40,\"ks\":[\"ss\",\"ff\"],\"m\":{\"a\":1234,\"b\":\"fuck\"}}"

	str1 := "DNS"
	fmt.Println(strings.EqualFold(str1,"dns1"))

	json.Unmarshal([]byte(str),&stu)

//	fmt.Println(fmt.Sprintf("%v",stu))

	fmt.Println(stu.M.B)
*/
}
