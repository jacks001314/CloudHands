package dns

import (
	"encoding/json"
	"fmt"
	"github.com/cloudhands/packet"
	"github.com/cloudhands/utils/msgunpack"
	"github.com/cloudhands/utils/ruleutils"
	"strings"
)

type DNSSession struct {

	Domain string 				 `json:"domain"`
	SEntry *packet.SessionEntry  `json:"sessionEntry"`
	Request *DNSRequest          `json:"request"`
	Response *DNSResponse        `json:"response"`

}

func (ds *DNSSession) ToJson() string {

	data,_ := json.Marshal(ds)

	return string(data)
}

func (ds *DNSSession)Parse(unpacker *msgunpack.MsgUnpacker){

	var hasReq bool
	var hasRes bool
	var n int

	if n = unpacker.UnpackMapHeader(false); n!=2 {

		panic(fmt.Sprintf("Invalid msgpack packet of dns session entry:%d not equal 2",n))

	}

	/*parse session entry*/
	ds.SEntry = packet.NewSessionEntryFromUDP(unpacker)


	if n = unpacker.UnpackMapHeader(true); n!=3&&n!=4 {

		panic(fmt.Sprintf("Invalid msgpack packet of dns session entry:%d not equal 3 or 4",n))
	}

	hasReq = unpacker.UnpackUInt8()==1
	hasRes = unpacker.UnpackUInt8()==1

	if hasReq {
		/*has dns request*/
		ds.Request = NewDnsRequest(unpacker)
		ds.Domain = ds.Request.GetDomain()
	}

	if hasRes {
		/*has dns response*/
		ds.Response = NewDnsResponse(unpacker)
	}

}

func (ds *DNSSession) CanMatch(proto string) bool {

	return strings.EqualFold(proto,"dns")
}

func (ds *DNSSession) GetTargetValue(target string,targetId int,isHex bool,offset int,dlen int) string {

	var result string = ""

	switch targetId {

	case ruleutils.DnsDomainId:
		result = ruleutils.TargetValue(ds.Domain,isHex)

	default:
		result = ds.SEntry.GetTargetValue(target,targetId,isHex,offset,dlen)

	}

	return result
}



