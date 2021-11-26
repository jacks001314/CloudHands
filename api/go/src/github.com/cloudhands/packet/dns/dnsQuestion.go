package dns

import (
	"fmt"
	"github.com/cloudhands/packet/util"
)

type DNSQuestion struct {
	Name DNSName `json:"reqName"`
	Qtype uint16  `json:"type"`
	Dclass uint16  `json:"dclass"`
}


func (dq *DNSQuestion) parse(unpacker *util.MsgUnpacker) {

	if n := unpacker.UnpackMapHeader(true); n != 3 {

		panic(fmt.Sprintf("Invalid msgpack packet of dns session question entry:%d not equal 3",n))
	}

	dq.Name.parse(unpacker)
	dq.Qtype = unpacker.UnpackUInt16()
	dq.Dclass = unpacker.UnpackUInt16()

}
