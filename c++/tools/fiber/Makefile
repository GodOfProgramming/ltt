#makefile

fiber.spec:
	g++ -std=c++17 -I $(CSPEC_INCLUDE) -o $@ libcspec.a fiber.spec.cpp -lpthread

fiber.test:
	g++ -std=c++17 -o $@ fiber.test.cpp -lpthread

