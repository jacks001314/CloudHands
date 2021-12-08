package main

import (
	"fmt"
	"golang.org/x/net/context"
)
import "github.com/go-redis/redis/v8"

func main() {

	var err error
	var ctx = context.Background()
	rdb := redis.NewClient(&redis.Options{
		Addr:               "192.168.198.128:6379",
		Password:           "",
		DB:                 0,
	})

	err = rdb.Set(ctx,"a","cccccfg",0).Err()


	val,err:= rdb.Get(ctx,"a").Result()

	fmt.Println(val,err)

}
