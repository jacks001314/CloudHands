package dns

type dnsResponse struct {

	header dnsHeader

	questions []dnsQuestion
	anws []dnsRecord
	auths []dnsRecord
	adds []dnsRecord

}



