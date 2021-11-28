package dns

import (
	"fmt"
	"github.com/cloudhands/utils/msgunpack"
)

type DNSHeader struct {
	Id uint16    `json:"id"`
	Flags uint16  `json:"flags"`
	Counts [4]uint16 `json:"counts"`
}

func (h *DNSHeader) parse(unpacker *msgunpack.MsgUnpacker) {

	if n := unpacker.UnpackMapHeader(true); n !=6 {
		panic(fmt.Sprintf("Invalid msgpack packet of dns session header entry: map len:%d not eqal 6",n))

	}

	h.Id = unpacker.UnpackUInt16()
	h.Flags = unpacker.UnpackUInt16()

	for i:=0;i<4;i++{

		h.Counts[i] = unpacker.UnpackUInt16()
	}


}