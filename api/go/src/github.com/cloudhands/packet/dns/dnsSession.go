package dns

import "github.com/cloudhands/packet"

type DNSSession struct {

	sessionEntry packet.SessionEntry
	request dnsRequest
	response dnsResponse

}
