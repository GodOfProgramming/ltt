package main

import (
	"context"
	"fmt"
	"time"
)

func main() {
	fmt.Println("doing thing")
	ctx := context.Background()
	go func(ctx context.Context) {
		time.Sleep(time.Second * 10)
		fmt.Println("done")
	}(ctx)
}
