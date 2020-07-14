package main

import (
	"fmt"
	"hash/fnv"
	"os"
)

func main() {
	s := os.Args[1]
	hash := fnv.New64a()
	hash.Write([]byte(s))
	fmt.Printf("%d\n", hash.Sum64())
}
