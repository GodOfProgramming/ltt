package main

import (
	"os"
	"os/exec"
)

func main() {
	cmd := exec.Command("ssh")
	cmd.Env = os.Environ()
	cmd.Args = append(cmd.Args, "jdizzle@it-lives")
	cmd.Stderr = os.Stderr
	cmd.Stdin = os.Stdin
	cmd.Stderr = os.Stderr
	cmd.Start()

	cmd.Wait()
}
