package geoip

type GeoResult struct {

	Country struct {
		ISOCode string `maxminddb:"iso_code"`
		Names struct{
			Name string `maxminddb:"zh-CN"`
		} `maxminddb:"names"`
	} `maxminddb:"country"`

	Location struct{
		Longitude float64 `maxminddb:"longitude"`
		Latitude  float64 `maxminddb:"latitude"`
	} `maxminddb:"location"`
}
