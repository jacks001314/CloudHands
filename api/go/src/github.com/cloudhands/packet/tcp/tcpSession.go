package tcp

import (
	"encoding/json"
	"fmt"
	"github.com/cloudhands/utils/msgunpack"
	"github.com/cloudhands/utils/netutils"
	"github.com/cloudhands/utils/ruleutils"
	"strings"
)

type TCPSession struct {

	ISTimeOut uint8     `json:"isTimeout"`
	TimeoutTV uint16    `json:"timeoutTV"`
	PhaseState uint16   `json:"phaseState"`
	SessionID  uint64   `json:"sessionID"`
	SrcIP string        `json:"srcIP"`
	DstIP string        `json:"dstIP"`
	SrcPort uint16      `json:"srcPort"`
	DstPort uint16      `json:"dstPort"`
	ReqPackets uint64   `json:"reqPackets"`
	ReqBytes   uint64   `json:"reqBytes"`
	ResPackets uint64   `json:"resPackets"`
	ResBytes   uint64   `json:"resBytes"`
	ReqStartTime uint64 `json:"reqStartTime"`
	ReqLastTime uint64 `json:"reqLastTime"`
	ResStartTime uint64 `json:"resStartTime"`
	ResLastTime uint64 `json:"resLastTime"`
	ReqData []byte     `json:"reqData"`
	ResData []byte     `json:"resData"`

}

func (ts *TCPSession) ToJson() string {

	data,_ := json.Marshal(ts)

	return string(data)
}

func (ts *TCPSession)Parse(unpacker *msgunpack.MsgUnpacker){


	if n := unpacker.UnpackMapHeader(false); n!=18 {

		panic(fmt.Sprintf("The number of tcp session message pack:%d not equal 18",n))
	}

	ts.ISTimeOut = unpacker.UnpackUInt8()
	ts.TimeoutTV = unpacker.UnpackUInt16()
	ts.PhaseState = unpacker.UnpackUInt16()
	ts.SessionID = unpacker.UnpackUInt64()
	ts.SrcIP = netutils.IPv4Str(unpacker.UnpackUInt32())
	ts.DstIP = netutils.IPv4Str(unpacker.UnpackUInt32())
	ts.SrcPort = unpacker.UnpackUInt16()
	ts.DstPort = unpacker.UnpackUInt16()
	ts.ReqPackets = unpacker.UnpackUInt64()
	ts.ReqBytes = unpacker.UnpackUInt64()
	ts.ResPackets = unpacker.UnpackUInt64()
	ts.ResBytes = unpacker.UnpackUInt64()
	ts.ReqStartTime = unpacker.UnpackUInt64()
	ts.ReqLastTime = unpacker.UnpackUInt64()
	ts.ResStartTime = unpacker.UnpackUInt64()
	ts.ResLastTime = unpacker.UnpackUInt64()
	ts.ReqData = unpacker.UnpackBytes()
	ts.ResData = unpacker.UnpackBytes()

}

func (ts *TCPSession)CanMatch(proto string)bool {

	return strings.EqualFold(proto,"tcp")
}


func (ts *TCPSession) GetTargetValue(target string,targetId int,isHex bool,offset int,dlen int) string {

	var result string = ""

	switch targetId {

	case ruleutils.SrcIPId:
		result = ruleutils.TargetValue(ts.SrcIP,isHex)

	case ruleutils.DstIPId:
		result = ruleutils.TargetValue(ts.DstIP,isHex)

	case ruleutils.SrcPortId:
		result = ruleutils.TargetValue(ts.SrcPort,isHex)

	case ruleutils.DstPortId:
		result = ruleutils.TargetValue(ts.DstPort,isHex)

	case ruleutils.TcpReqDataSizeId:
		result = ruleutils.TargetValue(len(ts.ReqData),isHex)

	case ruleutils.TcpResDataSizeId:
		result = ruleutils.TargetValue(len(ts.ResData),isHex)

	case ruleutils.TcpReqDataId:
		result = ruleutils.GetData(ts.ReqData,0,offset,dlen,isHex)

	case ruleutils.TcpResDataId:
		result = ruleutils.GetData(ts.ResData,0,offset,dlen,isHex)

	}

	return result
}
