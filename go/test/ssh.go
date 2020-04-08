package main

import (
	"golang.org/x/crypto/ssh"
)

func main() {
	var hostKey = ssh.PublicKey
	config := &ssh.ClientConfig {
		User: "jdizzle",
		Auth: []ssh.AuthMethod {

		}
	}
}
