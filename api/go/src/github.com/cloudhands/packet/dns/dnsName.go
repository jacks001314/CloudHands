package dns

import (
	"fmt"
	"github.com/cloudhands/packet/util"
)

type DNSName struct {

	Name string `json:"name"`
	Length uint16 `json:"length"`
	Labels uint16 `json:"labels"`
}

func (dn *DNSName)parse(unpacker *util.MsgUnpacker) {

	if n := unpacker.UnpackMapHeader(true); n !=2 {

		panic(fmt.Sprintf("Invalid msgpack packet of dns session name entry:%d not equal 2",n))

	}

	dn.Labels = unpacker.UnpackUInt16()
	dn.Name = unpacker.UnpackString()
	dn.Length = uint16(len(dn.Name))

}