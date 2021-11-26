package main

import (
	"fmt"
	"strconv"
)

type point struct {

	x int
	y int
	z int
}

type mypoint struct {

	x point
	y point

	xx int
}


func getStrs()(res []string) {


	for i:=0; i<3;i++ {

		res = append(res,strconv.FormatInt(int64(i),10))
	}

	return
}

func main(){


	m := &mypoint{
		x: point{},
		xx:    0,
	}

	m.x.x = 1234

	fmt.Println(m.x)

	//fmt.Println(getStrs())
}
