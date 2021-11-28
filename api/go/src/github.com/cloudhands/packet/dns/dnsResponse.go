package dns

import (
	"fmt"
	"github.com/cloudhands/utils/msgunpack"
)

type DNSResponse struct {

	Header *DNSHeader   `json:"header"`

	Questions []*DNSQuestion `json:"questions"`
	Anws []*DNSRecord  `json:"answers"`
	Auths []*DNSRecord  `json:"auths"`
	Adds []*DNSRecord   `json:"adds"`
}

func makeRecords(unpacker *msgunpack.MsgUnpacker) []*DNSRecord{

	unpacker.UnpackMapHeader(true)
	n := unpacker.UnpackArrayHeader(true)
	records := make([]*DNSRecord,n)

	for i := 0;i<n;i++ {

		rec := new(DNSRecord)
		rec.parse(unpacker)
		records[i] = rec
	}

	return records
}

func makeQuestions(unpacker *msgunpack.MsgUnpacker) []*DNSQuestion {

	n := unpacker.UnpackArrayHeader(true)

	questions := make([]*DNSQuestion,n)

	for i:=0;i<n;i++ {

		q := new(DNSQuestion)
		q.parse(unpacker)
		questions[i] = q
	}

	return questions
}

func NewDnsResponse(unpacker *msgunpack.MsgUnpacker) *DNSResponse {

	if n := unpacker.UnpackMapHeader(true); n!=5 {

		panic(fmt.Sprintf("Invalid msgpack packet of dns session response entry:%d not equal 5",n))
	}

	h := new(DNSHeader)
	h.parse(unpacker)

	questions := makeQuestions(unpacker)
	answers := makeRecords(unpacker)
	auths := makeRecords(unpacker)
	adds := makeRecords(unpacker)

	return &DNSResponse{
		Header:    h,
		Questions: questions,
		Anws:     answers,
		Auths:     auths,
		Adds:      adds,
	}

}





