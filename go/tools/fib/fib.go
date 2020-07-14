package main

import (
	"fmt"
	"os"
	"strconv"
)

func fib(count int) int {
	prev := 0
	curr := 1

	for i := 0; i < count; i++ {
		tmp := curr
		curr += prev
		prev = tmp
	}

	return prev
}

func main() {
	if len(os.Args) < 1 {
		fmt.Println("You need to supply an argument")
		os.Exit(1)
	}

	countStr := os.Args[1]

	count, err := strconv.Atoi(countStr)
	if err != nil {
		fmt.Printf("Please enter a number, you entered %s\n", countStr)
		os.Exit(1)
	}

	if count < 0 {
		fmt.Println("You need to use a postive number")
		os.Exit(1)
	}

	result := fib(count)

	fmt.Println(result)
}
