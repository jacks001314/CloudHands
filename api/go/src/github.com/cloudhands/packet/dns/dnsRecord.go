package dns

type dnsRecord struct {

	name dnsName
	qtype int
	dclass int
	ttl uint64
	pos uint32
	rdata []byte
}


