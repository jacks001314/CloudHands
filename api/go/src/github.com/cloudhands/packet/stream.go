package packet

type sessionEndPoint struct {

	ip uint32
	port uint16
	packets uint64
	bytes uint64
	startTime uint64
	lastTime uint64
	content []byte

}

type SessionEntry struct {

	req sessionEndPoint
	res sessionEndPoint

}




