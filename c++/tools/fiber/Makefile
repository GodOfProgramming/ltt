#makefile

fiber.o:
	g++ -std=c++17 -c -I $(CSPEC_INCLUDE) -o $@ fiber.cpp

fiber.spec:
	g++ -std=c++17 -I $(CSPEC_INCLUDE) -o $@ libcspec.a fiber.spec.cpp -lpthread

