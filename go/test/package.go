package main

import (
    "fmt"
)

type V struct {
    X, Y int
}

func (v *V) Calc() int {
    return v.X + v.Y
}

func main() {
    v := V{1, 1}

    fmt.Println(v.Calc())
}
