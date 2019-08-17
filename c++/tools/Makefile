#Makefile

libconsole: libconsole.o libconsole.a
	rm *.o

libconsole.o:
	g++ -c -std=c++17 console.cpp -o libconsole.o

libconsole.a:
	ar rvs libconsole.a libconsole.o

