package main

import (
	"crypto/ed25519"
	"crypto/rand"
	"encoding/base64"
	"fmt"
)

func main() {
	pub, priv, _ := ed25519.GenerateKey(rand.Reader)
	pub64 := base64.StdEncoding.EncodeToString(pub)
	priv64 := base64.StdEncoding.EncodeToString(priv)
	fmt.Printf("pub key = '%s'\n", pub64)
	fmt.Printf("priv key = '%s'\n", priv64)
}
