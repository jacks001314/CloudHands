package dns

import (
	"fmt"
	"github.com/cloudhands/utils/msgunpack"
)

type DNSRecord struct {

	Name DNSName  `json:"reqName"`
	Qtype uint16  `json:"type"`
	Dclass uint16 `json:"dclass"`
	TTL uint32     `json:"ttl"`
	Pos uint16     `json:"pos"`
	Rdata []byte   `json:"rdata"`
}

func (dr *DNSRecord) parse(unpacker *msgunpack.MsgUnpacker) {

	if n := unpacker.UnpackMapHeader(true) ; n != 6 {

		panic(fmt.Sprintf("Invalid msgpack packet of dns record entry:%d not equal 6",n))
	}

	dr.Name.parse(unpacker)
	dr.Qtype = unpacker.UnpackUInt16()
	dr.Dclass = unpacker.UnpackUInt16()
	dr.TTL = unpacker.UnpackUInt32()
	dr.Pos = unpacker.UnpackUInt16()
	dr.Rdata = unpacker.UnpackBytes()

}


