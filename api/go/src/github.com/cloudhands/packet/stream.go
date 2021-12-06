package packet

import (
	"fmt"
	"github.com/cloudhands/utils/msgunpack"
	"github.com/cloudhands/utils/netutils"
	"github.com/cloudhands/utils/ruleutils"
)

type sessionEndPoint struct {

	IP string 		`json:"ip"`
	Port uint16 	`json:"port"`
	Packets uint64 	`json:"packets"`
	Bytes uint64 	`json:"bytes"`
	StartTime uint64 `json:"startTime"`
	LastTime uint64	  `json:"lastTime"`
	Content []byte	   `json:"content"`
}

type SessionEntry struct {

	SessionID uint64     `json:"sessionID"`
	Req sessionEndPoint   `json:"req"`
	Res sessionEndPoint   `json:"res"`

}

func NewSessionEntryFromUDP(unpacker *msgunpack.MsgUnpacker) *SessionEntry {


	if n := unpacker.UnpackMapHeader(true); n!=13 {

		panic(fmt.Sprintf("Parse SessionEntry Failed,the Map elements must:%d,but now:%d",13,n))
	}

	se := new(SessionEntry)

	se.SessionID = unpacker.UnpackUInt64()
	se.Req.IP = netutils.IPv4Str(unpacker.UnpackUInt32())
	se.Res.IP = netutils.IPv4Str(unpacker.UnpackUInt32())
	se.Req.Port = unpacker.UnpackUInt16()
	se.Res.Port = unpacker.UnpackUInt16()
	se.Req.Packets = unpacker.UnpackUInt64()
	se.Req.Bytes = unpacker.UnpackUInt64()
	se.Res.Packets = unpacker.UnpackUInt64()
	se.Res.Bytes = unpacker.UnpackUInt64()
	se.Req.StartTime = unpacker.UnpackUInt64()
	se.Req.LastTime = unpacker.UnpackUInt64()
	se.Res.StartTime = unpacker.UnpackUInt64()
	se.Res.LastTime = unpacker.UnpackUInt64()

	return se
}


func (s *SessionEntry) GetTargetValue(target string,targetId int,isHex bool,offset int,dlen int) string {

	var result string = ""

	switch targetId {

	case ruleutils.SrcIPId:
		result = ruleutils.TargetValue(s.Req.IP,isHex)

	case ruleutils.DstIPId:
		result = ruleutils.TargetValue(s.Res.IP,isHex)

	case ruleutils.SrcPortId:
		result = ruleutils.TargetValue(s.Req.Port,isHex)

	case ruleutils.DstPortId:
		result = ruleutils.TargetValue(s.Res.Port,isHex)

	case ruleutils.TcpReqDataSizeId:
		result = ruleutils.TargetValue(len(s.Req.Content),isHex)

	case ruleutils.TcpResDataSizeId:
		result = ruleutils.TargetValue(len(s.Res.Content),isHex)

	case ruleutils.TcpReqDataId:
		result = ruleutils.GetData(s.Req.Content,0,offset,dlen,isHex)

	case ruleutils.TcpResDataId:
		result = ruleutils.GetData(s.Res.Content,0,offset,dlen,isHex)


	}

	return result
}





