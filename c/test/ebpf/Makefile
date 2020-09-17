.PHONY: basic_loader
basic_loader:
	g++ -Wall -O3 -o $@ $@.cpp -l:libbpf.a -l:libelf.a -l:libz.a

.PHONY: server
server:
	g++ -Wall -O3 -o $@ $@.cpp -l:libbpf.a -l:libelf.a -l:libz.a

.PHONY: xdp
bpf:
	clang -Wall -target bpf -O3 -c $@.c -o $@.o
