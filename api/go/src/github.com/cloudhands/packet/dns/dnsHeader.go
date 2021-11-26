package dns

type dnsHeader struct {
	id int
	flags int
	counts [4]int
}