package main

import (
	"encoding/json"
	"fmt"
	"net"
)

type Foo struct {
	Field1 string
	Field2 int
}

func main() {
	var foo Foo
	str := `{ "field1": "foobar", "field2": 7 }`
	json.Unmarshal([]byte(str), &foo)
	fmt.Printf("foo: %v\n", foo)

	var aFoo []Foo
	str = `[ { "field1": "one", "field2": 1 }, { "field1": "two", "field2": 2 } ]`
	json.Unmarshal([]byte(str), &aFoo)

	for _, f := range aFoo {
		fmt.Printf("foo: %v\n", f)
	}

	var mFoo map[int]string
	str = `{ "1": "one", "2": "two" }`
	json.Unmarshal([]byte(str), &mFoo)

	for k, v := range mFoo {
		fmt.Printf("foo: %d = %s\n", k, v)
	}

	udp, _ := net.ResolveUDPAddr("udp", "127.0.0.1:1234")
	fmt.Printf("%d\n", udp.Port)
}
