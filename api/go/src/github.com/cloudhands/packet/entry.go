package packet

import (
	"github.com/cloudhands/rule"
	"github.com/cloudhands/utils/msgunpack"
)

type SourceEntry interface {

	ToJson() string
	Parse(unpacker *msgunpack.MsgUnpacker)

	CanMatch(proto string)bool

	GetTargetValue(item rule.RuleItem) string

}


