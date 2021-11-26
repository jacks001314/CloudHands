package dns

import (
	"fmt"
	"github.com/cloudhands/packet/util"
)

type DNSRequest struct {

	Header *DNSHeader  `json:"header"`
	Questions []*DNSQuestion `json:"questions"`
}

func NewDnsRequest(unpacker *util.MsgUnpacker) *DNSRequest {

	var n int

	if n = unpacker.UnpackMapHeader(true); n != 2 {

		panic(fmt.Sprintf("Invalid msgpack packet of dns session request entry:%d not equal 2",n))
	}

	header := new(DNSHeader)
	header.parse(unpacker)

	n = unpacker.UnpackArrayHeader(true)

	qts := make([]*DNSQuestion, n)

	for i :=0;i<n;i++ {

		q := new(DNSQuestion)
		q.parse(unpacker);
		qts[i] = q;
	}

	return &DNSRequest{Header:header,Questions:qts}
}

func (r *DNSRequest) GetDomain() string {

	for _,q :=range(r.Questions) {

		name := q.Name.Name

		if len(name)>0 {
			return name
		}
	}

	return ""
}
