package geoip

type GeoIP interface {

	Open(dbpath string) (err error)
	Lookup(ip string) (*GeoResult,error)
	Close()
}
