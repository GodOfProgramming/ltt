#include <iostream>
#include <cstdlib>

template <typename... Args>
void println(Args&&... args) {
	((std::cout << std::forward<Args>(args)), ...) << '\n';
}

#define LOG_FATAL(...) 	println("fatal: ", __VA_ARGS__); exit(1)
#define LOG_ERROR(...) 	println("error: ", __VA_ARGS__)
#define LOG_WARN(...) 	println("warn:  ", __VA_ARGS__)
#define LOG_INFO(...) 	println("info:  ", __VA_ARGS__)
#define LOG_DEBUG(...) 	println(__FILE__, " (", __LINE__, "): ", __VA_ARGS__)
#define LOG(level, ...) LOG_##level(__VA_ARGS__)

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	LOG(DEBUG, "debug message");
	LOG(INFO, "info message");
	LOG(WARN, "warn message");
	LOG(ERROR, "error message");
	LOG(FATAL, "fatal");
	return 0;
}

