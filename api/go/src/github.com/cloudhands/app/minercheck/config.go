package minercheck

import (
	"github.com/cloudhands/utils/fileutils"
	"github.com/cloudhands/utils/jsonutils"
)

type Config struct {

	Fnames []string `json:"fnames"`
	queueSize int   `json:"queueSize"`

	Rconfig *RuleConfig `json:rconfig`

}

type RuleConfig struct {

	RuleGroupPath string `json:"ruleGroupPath"`
	Engine string `json:"engine"`
	Protos []string `json:"protos"`
	MatchThenStop bool `json:"matchThenStop"`

}

func LoadConfig(cfname string) (*Config,error){

	var conf Config
	var rfiles []string

	err:=jsonutils.UNMarshalFromFile(&conf,cfname)

	for _,file:= range conf.Fnames {
		
		for _,rfile:= range fileutils.GetFilesStartsWith(file) {
			
			rfiles = append(rfiles,rfile)
		}
	}
	
	conf.Fnames = rfiles
	
	return &conf,err
}


