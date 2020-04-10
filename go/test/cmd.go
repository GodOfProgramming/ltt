package main

import (
	"os"
	"os/exec"
)

func main() {
	cmd := exec.Command("/usr/bin/ssh")
	cmd.Env = os.Environ()
	cmd.Args = append(cmd.Args, "jdizzle@it-lives")
	cmd.Stderr = os.Stderr
	cmd.Stderr = os.Stderr
	cmd.Start()

	cmd.Wait()
}
