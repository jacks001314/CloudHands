package rule

import (
	"errors"
	"github.com/cloudhands/utils/jsonutils"
	"github.com/cloudhands/utils/timeutils"
)

var errNotFindRuleGroup = errors.New("Cannot find Rule Group!")


type RuleGroup struct {

	RootDir string 				`json:"rootDir"`
	Name string    				`json:"name"`
	Engine string  				`json:"engine"`
	Enable bool    				`json:"enable"`
	ISInline bool  				`json:"isInline"`
	RuleCounts uint64  			`json:"ruleCounts"`
	EnableRuleCounts uint64 	`json:"enableRuleCounts"`
	DisableRuleCounts uint64 	`json:"disableRuleCounts"`
	Time uint64  				`json:"time"`
}

type RuleGroupConfig struct {

	Groups []*RuleGroup `json:"groups"`
}


/*
load rule group from json file
*/
func LoadRuleGroup(ruleDir string) (rgcp *RuleGroupConfig,err error) {

	var rgc RuleGroupConfig

	err = jsonutils.UNMarshalFromFile(&rgc,GetRuleGroupPath(ruleDir))

	return &rgc,err
}

/*
*rebuild rule group when some rule to upload
*/
func (rgc *RuleGroupConfig) ReBuildRuleGroup(ruleDir string,ruleFiles []*RuleFile)  {

	if len(ruleFiles) == 0 {
		return
	}

	for _,rf := range ruleFiles {

		 if rg,err := rgc.FindRuleGroup(rf); err!=nil {

		 	/*create new group*/
		 	rg = new(RuleGroup)
		 	rg.RootDir = ruleDir
		 	rg.Name = rf.Type
		 	rg.Engine = rf.Engine
		 	rg.Enable = true
		 	rg.ISInline = false
		 	rg.Time = timeutils.GetNowTimeMS()

		 	/*reload rule config*/
		 	rg.LoadRules(ruleDir)

		 	rgc.Groups = append(rgc.Groups,rg)
		 }else {
			 rg.Time = timeutils.GetNowTimeMS()
			 rg.LoadRules(ruleDir)
		 }
	}

	/*write back rule group json file*/
	rgc.WriteRuleGroups(ruleDir)

}

/*
 find rule group of rule file
 */

func (rgc *RuleGroupConfig) FindRuleGroup(ruleFile *RuleFile) (rg *RuleGroup,err error) {

	if ruleFile == nil || len(ruleFile.Engine)==0 ||len(ruleFile.Type) ==0 {

		return nil,errNotFindRuleGroup
	}

	for _,rg = range rgc.Groups {

		if rg.Engine == ruleFile.Engine &&rg.Name == ruleFile.Type{

			return rg,nil
		}
	}

	/*not find*/
	return nil,errNotFindRuleGroup
}

/*
*write rule group config into file
 */
func (rgc *RuleGroupConfig) WriteRuleGroups(ruleDir string) error {

	return jsonutils.WriteJsonPretty(rgc,GetRuleGroupPath(ruleDir))
}


/*load rules from rule file*/
func ( rg *RuleGroup)LoadRules(ruleDir string) (rcp *RuleConfig,err error) {

	var rc RuleConfig

	var rcount uint64 = 0
	var ecount uint64 = 0
	var dcount uint64 = 0

	rpath := GetRulePath(ruleDir,rg.Engine,rg.Name)

	if err = jsonutils.UNMarshalFromFile(&rc,rpath); err!=nil {

		return &rc,err
	}

	rcount = uint64(len(rc.Rules))

	/*update target name and op name ids*/
	for _,r:= range rc.Rules {

		if r.IsEnable {
			ecount = ecount+1
		}else{
			dcount = dcount+1
		}

		for _,ri := range r.Items {

			ri.OpId = GetOPId(ri.Op)
			ri.TargetId = GetTargetId(ri.Target)
		}
	}

	rg.RuleCounts = rcount
	rg.EnableRuleCounts = ecount
	rg.DisableRuleCounts = dcount
	rg.Time = timeutils.GetNowTimeMS()


	/*write back rule into file*/
	if rcount >0 {

		jsonutils.WriteJsonPretty(&rc,rpath)
	}

	return &rc,err
}


