package main

import (
	"fmt"
	"time"
)

const (
	REPS = 10000000
	LEN  = 1000
)

func main() {
	z := make([]int, LEN)

	before := time.Now()

	for i := 0; i < REPS; i++ {
		x, y := 0, 0
		for j := 0; j < LEN; j++ {
			z[j] = x + y*i
			x++
			y++
		}
	}

	after := time.Since(before)

	fmt.Printf("nanos: %.7f\n", float64(after.Nanoseconds())/float64(REPS))
}
