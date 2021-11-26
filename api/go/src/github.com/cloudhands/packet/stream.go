package packet

import (
		"fmt"
		"github.com/cloudhands/packet/util"
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

func NewSessionEntryFromUDP(unpacker *util.MsgUnpacker) *SessionEntry {


	if n := unpacker.UnpackMapHeader(true); n!=13 {

		panic(fmt.Sprintf("Parse SessionEntry Failed,the Map elements must:%d,but now:%d",13,n))
	}

	se := new(SessionEntry)

	se.SessionID = unpacker.UnpackUInt64()
	se.Req.IP = util.IPv4Str(unpacker.UnpackUInt32())
	se.Res.IP = util.IPv4Str(unpacker.UnpackUInt32())
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



