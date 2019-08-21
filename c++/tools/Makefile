#Makefile

libconsole: FORCE libconsole.o libconsole.a
	rm *.o

libconsole.o: FORCE
	g++ -c -std=c++17 console.cpp -o libconsole.o

libconsole.a: FORCE
	ar rvs libconsole.a libconsole.o

tag-parser:
	g++ -o tag-parser tag-parser.cpp

console-test: FORCE
	g++ console-test.cpp libconsole.a -o console-test -std=c++17

FORCE:

