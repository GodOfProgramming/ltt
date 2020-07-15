.PHONY: server
server:
	gcc -Wall -o $@ $@.c -l:libbpf.a -l:libelf.a -l:libz.a

.PHONY: basic_loader
basic_loader:
	gcc -Wall -o $@ $@.c -l:libbpf.a -l:libelf.a -l:libz.a

.PHONY: xdp_drop
xdp_drop:
	clang -Wall -target bpf -c $@.c -o $@.o