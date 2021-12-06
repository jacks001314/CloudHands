package main

import (
	"flag"
	"fmt"
	"github.com/cloudhands/source"
	"os"
)

func main(){

	var pp source.PacketParser


	fname:=flag.String("fname","","mmap file path to read")
	id:= flag.Int("id",0,"mmap file id to been read")
	chanSize:=flag.Int("chansize",10000,"the chan size to store packet source entry to been read")

	flag.Parse()

	fmt.Println(fmt.Sprintf("Start to read packet entry from mmap file:%s,id:%d,chanSize:%d",fname,id,chanSize))

	pp = new(source.MsgPackPacketParser)
	if ss,err:= source.NewMMapSource(*fname,*id,*chanSize,pp);err!=nil {

		fmt.Println(fmt.Sprintf("cannot open mmap file to read:%v",err))
		os.Exit(-1)
	}else{

		c := ss.Open()

		for {
			select {
			case se :=<-c:
				fmt.Println(se.ToJson())
			}
		}

		ss.Close(c)
	}
}
