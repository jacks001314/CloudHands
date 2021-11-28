package main
/*
#cgo CFLAGS: -I /usr/local/include
#cgo LDFLAGS: -L/usr/local/lib -lCloudHandsCGo
 #include <ch_packet_api_reader.h>
 */
import "C"
import (
	"reflect"
	"unsafe"
)

type PacketEntry C.packet_entry_t

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

/*
func main(){

	fpath := os.Args[1]
	var id int =1

	 if rc := C.packet_mmap_open(C.int(id),C.CString(fpath)); rc!=0 {

	 	os.Exit(-1)
	 }


	 //var dnsS dns.DNSSession
	 var ts tcp.TCPSession

	 for {

	 	if pe := (*PacketEntry)(C.packet_entry_read(C.int(id)));pe!=nil {

	 		dec := (*util.MsgUnpacker)(msgpack.NewDecoder(bytes.NewBuffer(pe.Data())))

	 		//dnsS.Parse(dec)
	 		ts.Parse(dec)
	 		fmt.Println(ts.ToJson())
		}

	 }
}
*/