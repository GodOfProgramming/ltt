#Makefile

libconsole: libconsole.o libconsole.a
	rm *.o

libconsole.o:
	g++ -c -std=c++17 console.cpp -o libconsole.o

libconsole.a:
	ar rvs libconsole.a libconsole.o

tag-parser:
	g++ -o tag-parser tag-parser.cpp

console-test:
	g++ console-test.cpp -o console-test -std=c++17 libconsole.a

