.PHONY: basic_loader
basic_loader:
	gcc -Wall -O3 -o $@ $@.c -l:libbpf.a -l:libelf.a -l:libz.a

.PHONY: server
server:
	gcc -Wall -O3 -o $@ $@.c -l:libbpf.a -l:libelf.a -l:libz.a

.PHONY: xdp
xdp:
	clang -Wall -target bpf -O3 -c $@.c -o $@.o
