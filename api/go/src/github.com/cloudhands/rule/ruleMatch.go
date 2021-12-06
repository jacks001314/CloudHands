package rule

import (
	"github.com/cloudhands/packet"
	constants "github.com/cloudhands/utils/ruleutils"
	"regexp"
	"strconv"
	"strings"
)

type OPActionFunc func(tvalue string,value string,ri *RuleItem) bool

var OPActionMap = map[string]OPActionFunc{

	constants.OPcontains: func(tvalue string, value string, ri *RuleItem) bool {

		if ri.IsArray {

			for _,s := range ri.GetArrayValues() {

				if strings.Contains(tvalue,s){

					if !ri.IsAnd {
						/*or*/
						return true
					}
				}else{

					if ri.IsAnd {
						return false
					}
				}
			}

			return ri.IsAnd
		}else{

			return strings.Contains(tvalue,value)
		}
	},

	constants.OPstartsWith: func(tvalue string, value string, ri *RuleItem) bool {

		if ri.IsArray {

			for _,s := range ri.GetArrayValues() {

				if strings.HasPrefix(tvalue,s){

					if !ri.IsAnd {
						/*or*/
						return true
					}
				}else{

					if ri.IsAnd {
						return false
					}
				}
			}

			return ri.IsAnd
		}else{

			return strings.HasPrefix(tvalue,value)
		}
	},

	constants.OPendsWith: func(tvalue string, value string, ri *RuleItem) bool {

		if ri.IsArray {

			for _,s := range ri.GetArrayValues() {

				if strings.HasSuffix(tvalue,s){

					if !ri.IsAnd {
						/*or*/
						return true
					}
				}else{

					if ri.IsAnd {
						return false
					}
				}
			}

			return ri.IsAnd
		}else{

			return strings.HasSuffix(tvalue,value)
		}
	},
	constants.OPregex: func(tvalue string, value string, ri *RuleItem) bool {

		if ri.IsArray {

			for _,s := range ri.GetArrayValues() {

				reg := regexp.MustCompile(s)

				if reg.MatchString(tvalue) {

					if !ri.IsAnd {
						/*or*/
						return true
					}
				}else{

					if ri.IsAnd {
						return false
					}
				}
			}

			return ri.IsAnd
		}else{
			reg := regexp.MustCompile(value)
			return reg.MatchString(tvalue)
		}
	},

	constants.OPeq: func(tvalue string, value string, ri *RuleItem) bool {

		if ri.IsArray {

			for _,s := range ri.GetArrayValues() {

				if strings.EqualFold(tvalue,s){

					if !ri.IsAnd {
						/*or*/
						return true
					}
				}else{

					if ri.IsAnd {
						return false
					}
				}
			}

			return ri.IsAnd
		}else{

			return strings.EqualFold(tvalue,value)
		}
	},

	constants.OPlt: func(tvalue string, value string, ri *RuleItem) bool {

		var tint int64
		var sint int64
		var err error

		if ri.IsArray {

			for _,s := range ri.GetArrayValues() {

				if tint,err = strconv.ParseInt(tvalue,10,64); err!=nil {

					return false
				}
				if sint,err = strconv.ParseInt(s,10,64); err!=nil {

					return false
				}

				if tint<sint {

					if !ri.IsAnd {
						/*or*/
						return true
					}
				}else{

					if ri.IsAnd {
						return false
					}
				}
			}

			return ri.IsAnd
		}else{
			if tint,err = strconv.ParseInt(tvalue,10,64); err!=nil {

				return false
			}
			if sint,err = strconv.ParseInt(value,10,64); err!=nil {

				return false
			}
			return tint<sint
		}
	},

	constants.OPgt: func(tvalue string, value string, ri *RuleItem) bool {

		var tint int64
		var sint int64
		var err error

		if ri.IsArray {

			for _,s := range ri.GetArrayValues() {

				if tint,err = strconv.ParseInt(tvalue,10,64); err!=nil {

					return false
				}
				if sint,err = strconv.ParseInt(s,10,64); err!=nil {

					return false
				}

				if tint>sint {

					if !ri.IsAnd {
						/*or*/
						return true
					}
				}else{

					if ri.IsAnd {
						return false
					}
				}
			}

			return ri.IsAnd
		}else{
			if tint,err = strconv.ParseInt(tvalue,10,64); err!=nil {

				return false
			}
			if sint,err = strconv.ParseInt(value,10,64); err!=nil {

				return false
			}
			return tint>sint
		}
	},

	constants.OPle: func(tvalue string, value string, ri *RuleItem) bool {

		var tint int64
		var sint int64
		var err error

		if ri.IsArray {

			for _,s := range ri.GetArrayValues() {

				if tint,err = strconv.ParseInt(tvalue,10,64); err!=nil {

					return false
				}
				if sint,err = strconv.ParseInt(s,10,64); err!=nil {

					return false
				}

				if tint<=sint {

					if !ri.IsAnd {
						/*or*/
						return true
					}
				}else{

					if ri.IsAnd {
						return false
					}
				}
			}

			return ri.IsAnd
		}else{
			if tint,err = strconv.ParseInt(tvalue,10,64); err!=nil {

				return false
			}
			if sint,err = strconv.ParseInt(value,10,64); err!=nil {

				return false
			}
			return tint<=sint
		}
	},
	constants.OPge: func(tvalue string, value string, ri *RuleItem) bool {

		var tint int64
		var sint int64
		var err error

		if ri.IsArray {

			for _,s := range ri.GetArrayValues() {

				if tint,err = strconv.ParseInt(tvalue,10,64); err!=nil {

					return false
				}
				if sint,err = strconv.ParseInt(s,10,64); err!=nil {

					return false
				}

				if tint>=sint {

					if !ri.IsAnd {
						/*or*/
						return true
					}
				}else{

					if ri.IsAnd {
						return false
					}
				}
			}

			return ri.IsAnd
		}else{
			if tint,err = strconv.ParseInt(tvalue,10,64); err!=nil {

				return false
			}
			if sint,err = strconv.ParseInt(value,10,64); err!=nil {

				return false
			}
			return tint>=sint
		}
	},
}

func opMatch(tvalue string,op string,value string ,ri *RuleItem) bool {

	if tvalue == "" ||op == ""||value == "" {

		return false
	}


	if opfun := OPActionMap[op] ; opfun == nil {

		return false
	}else {

		return opfun(tvalue,value,ri)
	}

}

func doMatch(sentry packet.SourceEntry, ri *RuleItem) bool {

	tvalue := sentry.GetTargetValue(ri.Target,ri.TargetId,ri.IsHex,ri.Offset,ri.Len)

	match := opMatch(tvalue,ri.Op,ri.Value,ri)

	if ri.IsNot {

		return !match
	}

	return match
}

func ISMatch(sentry packet.SourceEntry, rule *Rule) bool {

	for _, ri := range rule.Items {

		if doMatch(sentry, ri) {

			if !rule.IsAnd {

				return true
			}
		} else {
			if rule.IsAnd {

				return false
			}
		}
	}

	return rule.IsAnd
}



