package main

import (
    "fmt"
)

func main() {
    var x *int
    var y int = 0

    x = &y

    fmt.Printf("Address: %p\n", x)
}

