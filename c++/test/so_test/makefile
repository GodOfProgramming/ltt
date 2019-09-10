#makefile

foo.so: foo.o
	g++ -shared -o libfoo.so $<

foo.o: foo.cpp
	g++ -c -Wall -Werror -fpic $< -o $@

