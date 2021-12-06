package main

import (
	"flag"
	"fmt"
	"github.com/cloudhands/app/minercheck"
	"os"
)

func main(){

	var mc *minercheck.MinerCheck
	var err error

	fname:=flag.String("cfile","","The mining check config file path")
	flag.Parse()

	if mc,err= minercheck.NewMinerCheck(*fname);err!=nil {

		fmt.Println(fmt.Sprintf("Cannot create miner checker by config file:%s,error:%v",*fname,err))
		os.Exit(-1)
	}

	mc.Run()
	ch := mc.GetMatchedChannel()

	for {

		select {

		case entry:=<-ch:
			fmt.Println(entry.ToJson())
		}
	}
}