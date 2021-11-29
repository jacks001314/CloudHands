package rule

import (
	"errors"
	"github.com/cloudhands/utils/fileutils"
	"strings"
)

var errInvalidArrayValueFormat = errors.New("Invalid rule item array values format")

type RuleItem struct {

	Target string `json:"target"`
	TargetId int  `json:"targetId"`
	Offset   int  `json:"offset"`
	Len 	 int  `json:"len"`
	Op 		string `json:"op"`
	OpId 	int    `json:"opId"`
	Value   string `json:"value"`
	IsAnd   bool   `json:"isAnd"`
	IsArray bool 	`json:"isArray"`
	IsHex   bool 	`json:"isHex"`
	IsNot   bool    `json:"isnot"`

	arrayValues []string

}

func (ri *RuleItem) LoadArrayValues() (err error){

	splits := strings.Split(ri.Value,":")
	n := len(splits)

	if n <2 {

		return errInvalidArrayValueFormat
	}

	switch splits[0] {

	case "file":

		if n!=2 {
			return errInvalidArrayValueFormat
		}

		if ri.arrayValues,err= fileutils.ReadAllLines(splits[1]);err!=nil {
			return
		}

	case "inline":
		if n!=3 {
			return errInvalidArrayValueFormat
		}

		ri.arrayValues = strings.Split(splits[2],ArrSplit)

	default:
		return errInvalidArrayValueFormat
	}

	return
}

func (ri *RuleItem) GetArrayValues() []string {

	return ri.arrayValues
}


