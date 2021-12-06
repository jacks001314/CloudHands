package source

import (
	"bytes"
	"github.com/cloudhands/packet"
	"github.com/cloudhands/packet/dns"
	"github.com/cloudhands/packet/tcp"
	"github.com/cloudhands/utils/msgunpack"
	"github.com/vmihailenco/msgpack/v5"
)

type MsgPackPacketParser struct {

}

func (p *MsgPackPacketParser) Parse(ptype uint8,data []byte) (pse packet.SourceEntry) {

	dec := (*msgunpack.MsgUnpacker)(msgpack.NewDecoder(bytes.NewBuffer(data)))


	switch ptype {

	case PKT_RECORD_TYPE_SESSION_TCP:
		pse = new(tcp.TCPSession)

	case PKT_RECORD_TYPE_UDP_DNS:
		pse = new(dns.DNSSession)

	default:
		pse = nil
	}

	if pse !=nil {

		pse.Parse(dec)
	}

	return pse
}
