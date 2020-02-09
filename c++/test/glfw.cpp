#include <GLFW/glfw3.h>
#include <iostream>

#define MARK std::cout << __LINE__ << std::endl

int main()
{
  MARK;
  glfwSetErrorCallback([](int error, const char* desc) {
    std::cout << "Error (" << error << "): " << desc << std::endl;
  });

  MARK;
  if (!glfwInit()) {
    std::cout << "Failed to init glfw" << std::endl;
    exit(1);
  }

  MARK;
  auto window = glfwCreateWindow(640, 480, "Test", nullptr, nullptr);

  MARK;
  if (window == nullptr) {
    std::cout << "Failed to create glfw window" << std::endl;
    glfwTerminate();
    exit(1);
  }

  MARK;
  glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int, int action, int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
  });

  MARK;
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  MARK;
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

