package rule


/*
The targets names can been provided
*/
const (

)
/*
The ids of targets
*/
/*

The op names can been provided
*/

/*
The ids of op
*/

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

}
