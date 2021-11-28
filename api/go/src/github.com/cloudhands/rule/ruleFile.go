package rule

import "github.com/cloudhands/utils/fileutils"

type RuleFile struct {

	Engine string  `json:"engine"`
	Type string 	`json:"type"`
	Name string 	`json:"name"`
}

func NewRuleFile(engine string,ttype string,name string) *RuleFile {

	return &RuleFile{
		Engine: engine,
		Type:   ttype,
		Name:   name,
	}
}

func NewRuleFileFromPath(file string) *RuleFile {

	paths := fileutils.GetFilePaths(file)
	n := len(paths)

	name := ""
	ttype := ""
	engine := ""

	if n>3 {

		name = paths[n-1]
		ttype = paths[n-2]
		engine = paths[n-3]

	}

	return &RuleFile{
		Engine: engine,
		Type:   ttype,
		Name:   name,
	}

}

