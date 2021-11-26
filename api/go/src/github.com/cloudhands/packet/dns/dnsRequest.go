package dns

type dnsRequest struct {

	header dnsHeader
	questions []dnsQuestion
}
