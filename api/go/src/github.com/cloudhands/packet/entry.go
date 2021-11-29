package packet

import (
	"github.com/cloudhands/utils/msgunpack"
)

type SourceEntry interface {

	ToJson() string
	Parse(unpacker *msgunpack.MsgUnpacker)

	CanMatch(proto string)bool

	GetTargetValue(target string,targetId int,isHex bool,offset int,dlen int) string

}


