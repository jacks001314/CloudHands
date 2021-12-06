package source

import "github.com/cloudhands/packet"

type Source interface {

	Open() chan packet.SourceEntry

	Close(c chan packet.SourceEntry)
}

