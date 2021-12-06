package minercheck

import (
	"github.com/cloudhands/packet"
	"github.com/cloudhands/rule"
	"github.com/cloudhands/source"
	"strings"
)

type MinerCheck struct {

	config *Config

	rulePools []*rule.RulePool

	sources [] source.Source

	matchedChan chan packet.SourceEntry

}

func NewMinerCheck(cfname string) (mchecker *MinerCheck,err error) {

	var rconfig *Config
	var rpools []*rule.RulePool
	var ss []source.Source

	if rconfig,err = LoadConfig(cfname);err!=nil {

		return
	}

	if rpools,err = loadRules(rconfig);err!=nil {
		return
	}

	if ss,err = openSources(rconfig);err!=nil {
		return
	}

	return &MinerCheck{
		config:    rconfig,
		rulePools: rpools,
		sources: ss,
		matchedChan:make(chan packet.SourceEntry),
	},nil


}

func (mc *MinerCheck) GetMatchedChannel() (chan packet.SourceEntry) {

	return mc.matchedChan
}

func openSources(config *Config)(ss []source.Source,err error)  {

	var id int =1
	var s source.Source

	for _,sfile:=range config.Fnames{

		if s,err =  source.NewMMapSource(sfile,id,config.queueSize,new(source.MsgPackPacketParser));err!=nil {

			return
		}

		ss = append(ss,s)
		id = id+1

	}

	return
}

func loadRules(config *Config) (rpools []*rule.RulePool,err error){

	var rgconfig *rule.RuleGroupConfig
	var rp *rule.RulePool

	if rgconfig,err= rule.LoadRuleGroup(config.Rconfig.RuleGroupPath);err!=nil {

		return
	}

	for _,rg := range rgconfig.Groups {

		if rg.Enable && strings.EqualFold(rg.Engine,config.Rconfig.Engine){

			if rp,err= rule.NewRulePool(rg,nil,config.Rconfig.MatchThenStop,config.Rconfig.Protos); err!=nil {

				return
			}
			if !rp.IsEmpty(){
				rpools = append(rpools,rp)
			}
		}
	}

	return
}

func (mc *MinerCheck)check(entry packet.SourceEntry) {

	for _,rp:= range mc.rulePools {

		if rp.Match(entry) {

			/*match*/
			mc.matchedChan<-entry
			if mc.config.Rconfig.MatchThenStop {
				return
			}
		}
	}
}

func (mc *MinerCheck) readSouceAndCheck(source source.Source){

	c:=source.Open()

	for {
		select {
		case entry :=<-c:
			go mc.check(entry)
		}
	}

}

func (mc *MinerCheck) Run(){

	for _,s:= range mc.sources{

		go mc.readSouceAndCheck(s)
	}
}

