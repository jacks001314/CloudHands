package main

import (
	"fmt"
	"github.com/cloudhands/geoip"
	"log"
)

func main()  {

	var path string = "D:\\shajf_dev\\self\\CloudHands\\api\\go\\src\\data\\GeoLite2-City.mmdb"

	var gip geoip.GeoIPMaxminddb

	if err:= gip.Open(path);err!=nil {
		log.Panic(err)
	}

	defer gip.Close()

	if r,err:=gip.Lookup("82.202.253.146");err!=nil {
		log.Panic(err)
	}else {
		fmt.Println(r)
	}
}
