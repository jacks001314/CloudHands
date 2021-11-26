package util

import (

	"bytes"
	"fmt"
	"github.com/vmihailenco/msgpack/v5"
)

type MsgUnpacker msgpack.Decoder

func NewMsgUnpacker(data []byte) *MsgUnpacker {

	bbuf := bytes.NewBuffer(data)
	return (*MsgUnpacker)(msgpack.NewDecoder(bbuf))
}

func (unpacker *MsgUnpacker) dec() (*msgpack.Decoder){

	return (*msgpack.Decoder)(unpacker)
}

func unpackName(dec *msgpack.Decoder)  {

	if _,err := dec.DecodeString();err!=nil {
		panic(fmt.Sprintf("Decode name failed:%v",err))
	}
}

func (unpacker *MsgUnpacker) UnpackMapHeader(hasName bool) (n int){

	var err error
	d := unpacker.dec()

	if hasName {
		unpackName(d)
	}

	if n,err = d.DecodeMapLen();err!=nil {

		panic(fmt.Sprintf("Decode MapHeader length failed:%v",err))
	}

	return
}

func (unpacker *MsgUnpacker) UnpackArrayHeader(hasName bool) (n int) {

	var err error
	d := unpacker.dec()

	if hasName {
		unpackName(d)
	}

	if n,err = d.DecodeArrayLen();err!=nil {

		panic(fmt.Sprintf("Decode Array header length failed:%v",err))
	}

	return
}

func (unpacker *MsgUnpacker) UnpackString() (str string){

	var err error

	d := unpacker.dec()
	unpackName(d)

	if str,err=d.DecodeString();err!=nil{

		panic(fmt.Sprintf("Decode String value failed:%v",err))
	}

	return
}

func (unpacker *MsgUnpacker) UnpackInt() (v int){

	var err error

	d := unpacker.dec()
	unpackName(d)

	if v,err = d.DecodeInt();err!=nil {

		panic(fmt.Sprintf("Decode int value failed:%v",v))
	}

	return
}

func (unpacker *MsgUnpacker) UnpackInt8() (v int8){

	var err error

	d := unpacker.dec()
	unpackName(d)

	if v,err = d.DecodeInt8();err!=nil {

		panic(fmt.Sprintf("Decode int8 value failed:%v",v))
	}

	return
}

func (unpacker *MsgUnpacker) UnpackInt16() (v int16){

	var err error

	d := unpacker.dec()
	unpackName(d)

	if v,err = d.DecodeInt16();err!=nil {

		panic(fmt.Sprintf("Decode int16 value failed:%v",v))
	}

	return
}

func (unpacker *MsgUnpacker) UnpackInt32() (v int32){

	var err error

	d := unpacker.dec()
	unpackName(d)

	if v,err = d.DecodeInt32();err!=nil {

		panic(fmt.Sprintf("Decode int32 value failed:%v",v))
	}

	return
}

func (unpacker *MsgUnpacker) UnpackInt64() (v int64){

	var err error

	d := unpacker.dec()
	unpackName(d)

	if v,err = d.DecodeInt64();err!=nil {

		panic(fmt.Sprintf("Decode int64 value failed:%v",v))
	}

	return
}

func (unpacker *MsgUnpacker) UnpackUInt8() (v uint8){

	var err error

	d := unpacker.dec()
	unpackName(d)

	if v,err = d.DecodeUint8();err!=nil {

		panic(fmt.Sprintf("Decode uint8 value failed:%v",v))
	}

	return
}

func (unpacker *MsgUnpacker) UnpackUInt16() (v uint16){

	var err error

	d := unpacker.dec()
	unpackName(d)

	if v,err = d.DecodeUint16();err!=nil {

		panic(fmt.Sprintf("Decode uint16 value failed:%v",v))
	}

	return
}

func (unpacker *MsgUnpacker) UnpackUInt32() (v uint32){

	var err error

	d := unpacker.dec()
	unpackName(d)

	if v,err = d.DecodeUint32();err!=nil {

		panic(fmt.Sprintf("Decode uint32 value failed:%v",v))
	}

	return
}

func (unpacker *MsgUnpacker) UnpackUInt64() (v uint64){

	var err error

	d := unpacker.dec()
	unpackName(d)

	if v,err = d.DecodeUint64();err!=nil {

		panic(fmt.Sprintf("Decode uint64 value failed:%v",v))
	}

	return
}

func (unpacker *MsgUnpacker) UnpackBytes() (v []byte){

	var err error

	d := unpacker.dec()
	unpackName(d)

	if v,err = d.DecodeBytes();err!=nil {

		panic(fmt.Sprintf("Decode bytes value failed:%v",v))
	}

	return
}

func (unpacker *MsgUnpacker) UnpackArrayString(hasName bool) (v []string){

	var err error
	var a string
	d := unpacker.dec()

	n:= unpacker.UnpackArrayHeader(hasName)

	for i := 0;i<n;i++ {


		if a,err = d.DecodeString();err!=nil{

			panic(fmt.Sprintf("Decode ArrayString failed:%v",err))
		}

		v = append(v,a)
	}

	return
}

func (unpacker *MsgUnpacker) UnpackMapString(hasName bool) (v []string){

	var err error
	var a string

	d := unpacker.dec()
	n := unpacker.UnpackMapHeader(hasName)

	for i := 0;i<n;i++ {

		unpackName(d)

		if a,err = d.DecodeString();err!=nil{

			panic(fmt.Sprintf("Decode MapString failed:%v",err))
		}

		v = append(v,a)
	}

	return
}






