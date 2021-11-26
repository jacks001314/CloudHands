package util

import (

	"bytes"
	"github.com/vmihailenco/msgpack/v5"
)

type msgUnpacker msgpack.Decoder

func NewMsgUnpacker(data []byte) *msgUnpacker  {

	bbuf := bytes.NewBuffer(data)
	return (*msgUnpacker)(msgpack.NewDecoder(bbuf))
}

func (unpacker *msgUnpacker) dec() (*msgpack.Decoder){

	return (*msgpack.Decoder)(unpacker)
}

func (unpacker *msgUnpacker) UnpackMapHeader(hasName bool) (n int,err error){

	d := unpacker.dec()

	if hasName {
		_,err = d.DecodeString()
	}

	n,err = d.DecodeMapLen()

	return
}

func (unpacker *msgUnpacker) UnpackArrayHeader(hasName bool) (n int ,err error) {

	d := unpacker.dec()

	if hasName {
		_,err = d.DecodeString()
	}

	n,err = d.DecodeArrayLen()
	return
}

func (unpacker *msgUnpacker) UnpackString() (str string,err error){

	d := unpacker.dec()
	_,err = d.DecodeString()

	if err!=nil{
		return "",err
	}

	return d.DecodeString()
}

func (unpacker *msgUnpacker) UnpackInt() (v int,err error){

	d := unpacker.dec()
	_,err = d.DecodeString()

	if err!=nil{
		return
	}

	return d.DecodeInt()
}

func (unpacker *msgUnpacker) UnpackInt8() (v int8,err error){

	d := unpacker.dec()
	_,err = d.DecodeString()

	if err!=nil{
		return
	}

	return d.DecodeInt8()
}

func (unpacker *msgUnpacker) UnpackInt16() (v int16,err error){

	d := unpacker.dec()
	_,err = d.DecodeString()

	if err!=nil{
		return
	}

	return d.DecodeInt16()
}

func (unpacker *msgUnpacker) UnpackInt32() (v int32,err error){

	d := unpacker.dec()
	_,err = d.DecodeString()

	if err!=nil{
		return
	}

	return d.DecodeInt32()
}

func (unpacker *msgUnpacker) UnpackInt64() (v int64,err error){

	d := unpacker.dec()
	_,err = d.DecodeString()

	if err!=nil{
		return
	}

	return d.DecodeInt64()
}

func (unpacker *msgUnpacker) UnpackUInt8() (v uint8,err error){

	d := unpacker.dec()
	_,err = d.DecodeString()

	if err!=nil{
		return
	}

	return d.DecodeUint8()
}

func (unpacker *msgUnpacker) UnpackUInt16() (v uint16,err error){

	d := unpacker.dec()
	_,err = d.DecodeString()

	if err!=nil{
		return
	}

	return d.DecodeUint16()
}

func (unpacker *msgUnpacker) UnpackUInt32() (v uint32,err error){

	d := unpacker.dec()
	_,err = d.DecodeString()

	if err!=nil{
		return
	}

	return d.DecodeUint32()
}

func (unpacker *msgUnpacker) UnpackUInt64() (v uint64,err error){

	d := unpacker.dec()
	_,err = d.DecodeString()

	if err!=nil{
		return
	}

	return d.DecodeUint64()
}

func (unpacker *msgUnpacker) UnpackBytes() (v []byte,err error){

	d := unpacker.dec()
	_,err = d.DecodeString()

	if err!=nil{
		return
	}

	return d.DecodeBytes()
}

func (unpacker *msgUnpacker) UnpackArrayString(hasName bool) (v []string,err error){

	d := unpacker.dec()

	if hasName {
		_, err = d.DecodeString()
		if err!=nil{
			return
		}
	}

	n,err := d.DecodeArrayLen()
	if err!=nil{
		return
	}

	for i := 0;i<n;i++ {

		a,err := d.DecodeString()

		if err!=nil{
			return
		}

		v = append(v,a)
	}

	return
}

func (unpacker *msgUnpacker) UnpackMapString(hasName bool) (v []string,err error){

	d := unpacker.dec()

	if hasName {
		_, err = d.DecodeString()
		if err!=nil{
			return
		}
	}

	n,err := d.DecodeMapLen()
	if err!=nil{
		return
	}

	for i := 0;i<n;i++ {

		_,err := d.DecodeString()

		if err!=nil{
			return
		}

		a,err := d.DecodeString()

		if err!=nil{
			return
		}

		v = append(v,a)
	}

	return
}






