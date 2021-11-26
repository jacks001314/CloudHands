package dns

type dnsQuestion struct {
	name dnsName
	qtype int
	dclass int
}
