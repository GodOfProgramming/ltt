package main

import (
	"fmt"
	"net"
)

func main() {
	var udp net.UDPAddr
	fmt.Println(udp.String())
	p, _ := net.ResolveUDPAddr("udp", udp.String())
	fmt.Println(p.String())
}
