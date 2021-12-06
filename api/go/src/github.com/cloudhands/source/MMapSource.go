package source

/*
#cgo CFLAGS: -I /usr/local/include
#cgo LDFLAGS: -L/usr/local/lib -lCloudHandsCGo
#include <ch_packet_api_reader.h>
*/
import "C"

import (
	"fmt"
	"github.com/cloudhands/packet"
	"reflect"
	"unsafe"
)

type PacketEntry C.packet_entry_t

type MMapSource struct {

	fname string

	id int

	chanSize int

	parser PacketParser
}

func NewMMapSource(fname string ,id int,chanSize int,parser PacketParser) (s *MMapSource,err error){

	s = &MMapSource{
		fname: fname,
		id:    id,
		chanSize: chanSize,
		parser: parser,
	}

	if rc := C.packet_mmap_open(C.int(id),C.CString(fname)); rc!=0 {

		return nil,fmt.Errorf("Cannot open mmap file:%s to read packet!",fname)
	}


	return s,nil
}

func packetEntry(p *PacketEntry) *C.packet_entry_t {
	return (*C.packet_entry_t)(unsafe.Pointer(p))
}
// Data returns contained packet.
func (p *PacketEntry) Data() []byte {

	var d []byte
	pe := packetEntry(p)
	sh := (*reflect.SliceHeader)(unsafe.Pointer(&d))
	sh.Data = uintptr(pe.data)
	sh.Len = int(pe.dataSize)
	sh.Cap = int(pe.dataSize)
	return d
}

func (s *MMapSource) Open() (c chan packet.SourceEntry){

	if s.chanSize>1 {

		c = make(chan packet.SourceEntry,s.chanSize)
	}else {

		c = make(chan packet.SourceEntry)
	}

	go func() {
		for {

			if pe := (*PacketEntry)(C.packet_entry_read(C.int(s.id)));pe!=nil {
				se := s.parser.Parse(uint8(pe.ptype),pe.Data())
				c<-se
			}
		}
	}()

	return c
}

func (s *MMapSource) Close(c chan packet.SourceEntry){

	C.packet_entry_read(C.int(s.id))

	close(c)
}
