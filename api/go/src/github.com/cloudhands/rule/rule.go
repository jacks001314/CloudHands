package rule

import (
	"errors"
	"github.com/cloudhands/utils/jsonutils"
)

var errNotFindRule = errors.New("Cannot find Rule!")

type Rule struct {

	Proto string 	`json:proto`
	Id uint64 		`json:"id"`
	Type string 	`json:"type"`
	Msg  string 	`json:"msg"`
	IsEnable bool	`json:"isEnable"`
	IsAnd bool 		`json:"isAnd"`
	Name  string 	`json:"name"`
	Value string 	`json:"value"`
	Time  uint64 	`json:"time"`

	Items []*RuleItem `json:"items"`
}

type RuleConfig struct {

	Rules []*Rule  `json:"rules"`
}


/*find a rule by id */
func (rc *RuleConfig) FindRule(id uint64) ( r *Rule,err error) {

	for _,r = range rc.Rules {

		if r.Id == id {

			return r,nil
		}
	}

	return nil,errNotFindRule
}

/*find max rule id*/
func (rc *RuleConfig) FindMaxRuleId() uint64 {

	var max uint64 = 0

	for _,r := range rc.Rules {

		if r.Id>max {

			max = r.Id
		}
	}

	return max
}

/*write rules into rule file*/
func (rc *RuleConfig) WriteRules(ruleDir string,engine string,rtype string ) error {
	return jsonutils.WriteJsonPretty(rc,GetRulePath(ruleDir,engine,rtype))
}

