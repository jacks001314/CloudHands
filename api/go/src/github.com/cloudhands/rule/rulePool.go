package rule

import (
	"github.com/cloudhands/packet"
	"strings"
)

type AfterMatchActionFunc func(entry packet.SourceEntry,rule *Rule) error

type RulePool struct {

	actionFunc AfterMatchActionFunc

	rconfig *RuleConfig

	ruleGroup *RuleGroup

	matchThenStop bool

	acceptProtos []string

}

func (rp *RulePool) acceptRule(r *Rule) bool{

	if !r.IsEnable {
		return false
	}

	if rp.acceptProtos == nil ||len(rp.acceptProtos) == 0 ||r.Proto=="" {

		return true
	}

	for _,p:= range rp.acceptProtos {

		if strings.EqualFold(p,r.Proto){

			return true
		}
	}

	return false
}

func NewRulePool(rg *RuleGroup,actionFunc AfterMatchActionFunc,matchThenStop bool,protos []string) (rpool *RulePool,err error) {

	var rp = &RulePool{
		actionFunc:    actionFunc,
		rconfig:       new(RuleConfig),
		ruleGroup:     rg,
		matchThenStop: matchThenStop,
		acceptProtos:  protos,
	}

	var rules []*Rule

	/*load rules*/
	if rconfig,err:= rg.LoadRules(rg.RootDir,true); err !=nil {

		return rp,err
	}else {
		for _,rule:= range rconfig.Rules{

			if rp.acceptRule(rule){

				rules = append(rules,rule)
			}
		}
	}


	rp.rconfig.Rules = rules

	return rp,nil
}

func (rp *RulePool) IsEmpty() bool {

	return len(rp.rconfig.Rules)==0
}

func (rp *RulePool) Match(sentry packet.SourceEntry) bool {

	var matchCount int = 0

	if rp.rconfig == nil || len(rp.rconfig.Rules) == 0 {

		return false
	}

	for _,rule := range rp.rconfig.Rules {

		if rule.IsEnable {

			if sentry.CanMatch(rule.Proto) {

				if ISMatch(sentry,rule) {

					matchCount = matchCount+1
					if rp.actionFunc!=nil {

						rp.actionFunc(sentry,rule)
					}

					if rp.matchThenStop {
						break
					}
				}

			}
		}
	}
	return matchCount>0
}

