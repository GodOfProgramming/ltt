package testpackage

type V struct {
    X, Y int
}

func (v *V) Calc() int {
    return v.X + v.Y
}
