package main

import (
	"crypto/rand"
	"encoding/base64"
	"fmt"
	"os"

	"golang.org/x/crypto/nacl/box"
)

func main() {
	publicKey, privateKey, err := box.GenerateKey(rand.Reader)
	if err != nil {
		fmt.Println("could not generate public private keypair")
		os.Exit(1)
	}
	publicKeyB64 := base64.StdEncoding.EncodeToString(publicKey[:])
	privateKeyB64 := base64.StdEncoding.EncodeToString(privateKey[:])

	fmt.Printf("%s, %s", publicKeyB64, privateKeyB64)
}
