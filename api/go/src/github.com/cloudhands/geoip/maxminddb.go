package geoip

import (
	"fmt"
	"github.com/oschwald/maxminddb-golang"
	"net"
	"strings"
)

type GeoIPMaxminddb struct {

	dr *maxminddb.Reader
}

func (db *GeoIPMaxminddb) Open(dbpath string) (err error){

	var dr *maxminddb.Reader

	if dr,err = maxminddb.Open(dbpath); err!=nil {

		return fmt.Errorf("Cannot open maxminddb:%s to read",dbpath)
	}

	db.dr = dr

	return
}

func (db *GeoIPMaxminddb)Lookup(ip string) (gr *GeoResult,err error)  {


	nip := net.ParseIP(ip)

	var record GeoResult

	 if err = db.dr.Lookup(nip, &record);err!=nil {

	 	return
	 }

	 if strings.EqualFold(record.Country.ISOCode,"TW"){
	 	record.Country.ISOCode = "CN"
	 	record.Country.Names.Name = "中国"
	 }

	 if strings.EqualFold(record.Country.ISOCode,"KOR"){
		 record.Country.Names.Name = "韩国"
	 }

	 return &record,nil
}



func (db *GeoIPMaxminddb)Close()  {

	db.dr.Close()
}
