package main

import (
    "fmt"
)

func test1() {
    f := func (a int, b int) int { return a + b }
    f2 := func(a int, b int) int { return a - b }

    fmt.Printf("%d\n", f(1, 1))

    f = f2

    fmt.Printf("%d\n", f(1, 1));
}

// Note to self, this does not work
//func test2() {
//    f := func (a int, b int) int { return a + b }
//    f2 := func(a int, b int, c int) int { return a - b - c }
//
//    fmt.Printf("%d\n", f(1, 1))
//
//    f = f2
//
//    fmt.Printf("%d\n", f(2, 1, 1));
//}

func main() {
    test1()
}

