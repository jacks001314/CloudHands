package packet

import (
	"github.com/cloudhands/packet/util"
	)

type SourceEntry interface {

	ToJson() string
	Parse(unpacker *util.MsgUnpacker)

}


